#include "core.h"
#include "actor.h"

/* Dedicated deep investigation (docs/matching/issue-9-10-0x0800ceac-graphics.md):
 * these two functions sit in the still-raw span `tools/report_units.py`
 * tracked as parked (`base_object=None`) between the just-closed
 * `sub_800CD00` (src/graphics/actor_part109.c, issue #9/#10) and the
 * already-matched `sub_800D040` (src/system/game_loop6.c, issue #12,
 * the physics/collision subsystem's documented entry point). Both are
 * called *only* from `sub_0800D18C` (asm/code_3_2_17_d18c.s, the
 * subsystem's ~1960-byte collision-response commit function,
 * docs/matching/issue-12-physics-collision.md) - recategorized
 * `graphics` -> `game_loop` here to match that caller, the same
 * recategorization issue #12 already applied to `sub_800D040` itself.
 *
 * `sub_800CF70` is the function `docs/rom_map.md` (line 2137) already
 * partially flagged: "144 bytes before the physics/collision
 * subsystem's stated 0x0800D000 start, calls the same linked-list
 * walkers that subsystem uses and reaches the same 28-byte-record
 * chain - functionally part of it despite sitting just outside the
 * documented boundary". Reading the real bytes confirms that note
 * exactly (see its own doc comment below) and additionally reveals its
 * sibling `sub_800CEAC`, entirely unremarked anywhere until now. */

extern void sub_803AFE4(void *buf, s32 arg1, s32 arg2);
extern void sub_803AFDC(void *buf, s32 arg1, s32 arg2);
extern u8 sub_8001688(void *buf1, void *buf2);
extern void *gUnknown_030012D8;
extern void *sub_801070C(void *obj); /* "get next" */
extern void *sub_8010708(void *obj); /* "get prev" */

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

/* Same `+4`/`+6`/`+8`/`+9` `{s16 xOff, s16 yOff, u8 w, u8 h}` hitbox
 * quad layout `sub_800D040`/`sub_800CD00` already document, but here
 * the caller (`sub_0800D18C`) passes a pointer directly to the quad
 * itself (`playerRecordBase + 4`), not the 28-byte record's own base -
 * so this function only ever sees the 4-byte-wide quad and never reads
 * the record's own leading word. */
struct hitbox_quad {
    s16 xOff;
    s16 yOff;
    u8 w;
    u8 h;
};

/* Called once by `sub_0800D18C` (its only caller), passing the
 * player's (`gUnknown_030012D8`) own hitbox quad (`player's +0x20`
 * table, indexed by the player's own `+0x2d` tag, at the record's
 * `+4` quad) together with `self`'s own cached `x>>8`/`y>>8` shift
 * values and `self`'s own already-built AABB (`box`, built by the
 * caller from `self`'s own `+0x20` table at the top of `sub_0800D18C`
 * - the same convention `sub_800D040`'s "AABB1" documents).
 *
 * Builds a *hybrid* AABB - the player's hitbox dimensions, positioned
 * at `self`'s location (`quad->xOff + xOffset`, `quad->yOff +
 * yOffset`) - i.e. "if a player-shaped box were standing where `self`
 * currently is". When `gUnknown_030012D8+0x90` (an unconfirmed player
 * state/mode byte, not documented elsewhere under this exact offset -
 * `+0x92`/`+0x94` are separately documented state bytes right next to
 * it, see `docs/matching/issue-18-0x08014f8c-actor.md`) is nonzero,
 * the box is widened by 4 (2 either side: position shifted left by 2,
 * width padded by 4) before the mirror step - a "wide mode" hitbox
 * variant.
 *
 * The hybrid box is then mirrored horizontally/vertically around
 * `(xOffset, yOffset)` according to the PLAYER's own `+0x28` mirror
 * flags (bits 4/5 - the same mirror-flag convention `sub_800D040`
 * documents, just keyed off the player's flags instead of `self`'s,
 * since the box represents the player's shape, not `self`'s).
 *
 * Finally tests the hybrid box against `box` (`self`'s own real AABB)
 * via `sub_8001688` and returns the boolean overlap result: "would a
 * player-shaped hitbox at `self`'s position overlap `self`'s own
 * actual hitbox" - used by `sub_0800D18C` to decide whether to treat
 * `self` as blocking/pushing a player-sized object at that spot (its
 * caller follows a `1` result with a `sub_801070C`(self) "get next"
 * list-walk step, consistent with a "can something occupy this slot"
 * gate feeding further list traversal).
 *
 * `self` itself (the first argument) is loaded into a callee-saved
 * register by the ROM's own prologue but never referenced again after
 * that - genuinely unused, not a transcription slip (confirmed against
 * the raw disassembly: no further read of r1's original register
 * contents `self` was copied from).
 *
 * NAKED transcription, not real C: the same single-inlined-build AABB
 * shape `sub_8007B98` (src/graphics/actor_part.c) already documents as
 * resistant to gcc 2.9 register allocation even in its simplest form
 * ("about 10 of ~73 instructions... which anonymous scratch register"
 * gaps), compounded here by the extra `player+0x90` branch selecting
 * between two slightly different operand sequences before the shared
 * tail - not re-attempted as C given that established precedent (see
 * `docs/matching/issue-9-10-0x0800aaec-graphics.md`'s own `sub_800CD00`
 * writeup for the same judgment call on a related shape). Verified
 * byte-exact via the isolated `cpp`/`agbcc`/`as` + `objcopy`/`cmp`
 * pipeline against `baserom.gba`'s own bytes at
 * `0x0800CEAC`-`0x0800CF70` (only the two `bl` and two `.word`
 * relocation sites differ, which resolve correctly once linked). */
NAKED u8 sub_800CEAC(void *self, struct hitbox_quad *quad, struct aabb *box,
                      s32 xOffset, s32 yOffset)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "sub sp, #0x10\n\t"
        "add r6, r1, #0\n\t"
        "mov r8, r2\n\t"
        "add r7, r3, #0\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x90\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "mov r0, #0\n\t"
        "ldrsh r1, [r6, r0]\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r6, r0]\n\t"
        "ldrb r5, [r6, #5]\n\t"
        "add r1, r1, r7\n\t"
        "sub r1, #2\n\t"
        "ldr r0, [sp, #0x28]\n\t"
        "add r2, r2, r0\n\t"
        "ldrb r4, [r6, #4]\n\t"
        "add r4, #4\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_803AFDC\n\t"
        "b 2f\n\t"
        ".align 2, 0\n"
    "8: .4byte gUnknown_030012D8\n"
    "1:\n\t"
        "mov r0, #0\n\t"
        "ldrsh r1, [r6, r0]\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r6, r0]\n\t"
        "ldrb r4, [r6, #4]\n\t"
        "ldrb r5, [r6, #5]\n\t"
        "add r1, r1, r7\n\t"
        "ldr r0, [sp, #0x28]\n\t"
        "add r2, r2, r0\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_803AFDC\n\t"
    "2:\n\t"
        "ldr r3, 9f\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "lsl r0, r7, #1\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r2, [sp, #8]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp]\n\t"
    "3:\n\t"
        "ldr r0, [r3]\n\t"
        "add r0, #0x28\n\t"
        "ldrb r0, [r0]\n\t"
        "lsl r0, r0, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 4f\n\t"
        "ldr r1, [sp, #0x28]\n\t"
        "lsl r0, r1, #1\n\t"
        "ldr r1, [sp, #4]\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #4]\n\t"
    "4:\n\t"
        "mov r0, r8\n\t"
        "mov r1, sp\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 5f\n\t"
        "mov r0, #0\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "9: .4byte gUnknown_030012D8\n"
    "5:\n\t"
        "mov r0, #1\n\t"
    "6:\n\t"
        "add sp, #0x10\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}

/* `sub_0800D18C`'s single-step neighbor probe, called while its own
 * 5-slot "recently touched" ring-buffer counter (`gUnknown_030012D8
 * +0x94`-adjacent counter at the caller's own stack cache) is `<= 4`
 * (confirmed at the call site: `cmp r3,#4; bgt` skips the call
 * entirely and uses `self` directly instead).
 *
 * Reads `self`'s doubly-linked neighbor pointers both ways
 * (`sub_801070C` = "get next", `sub_8010708` = "get prev" - the
 * established pair, see `src/system/game_loop7.c`). If *neither*
 * exists, returns `self` unchanged with no other side effect - `self`
 * is isolated in the list.
 *
 * Otherwise (at least one neighbor exists) sets `*foundFlag = 1` - an
 * out-param the caller pre-loads with its own edge-code byte before
 * the call and only overwrites when a neighbor is actually found.
 *
 * If there is no "prev" neighbor, or `prev`'s own `+0x4d & 0x7f`
 * state byte reads `1` (the same early-out gate `sub_800D040`'s own
 * header documents - excluded from the physics AABB tests entirely),
 * returns `self` unchanged.
 *
 * Otherwise builds `prev`'s own AABB from the shared `self+0x20`
 * table (indexed by `prev`'s own `+0x2d` tag, quad at record `+4`,
 * `+4`/`+6`/`+8`/`+9` layout) offset by `prev.x>>8`/`prev.y>>8` and
 * mirrored per `prev`'s own `+0x28` flags - the exact "AABB1" shape
 * `sub_800D040`'s header already documents at length, just for `prev`
 * instead of `self`. Tests it against the caller-supplied `box` via
 * `sub_8001688`; on overlap, returns `prev` instead of `self` - so the
 * caller can substitute the actual colliding neighbor in place of the
 * object it started the probe from.
 *
 * This confirms and sharpens `docs/rom_map.md`'s existing note (line
 * 2137): it doesn't just "call the same linked-list walkers" and
 * "reach the same 28-byte-record chain" in passing - the entire
 * function body *is* one AABB-build-and-overlap-test cycle from that
 * subsystem, gated by the subsystem's own `+0x4d` state-exclusion
 * convention, operating on the "prev" neighbor specifically. It is
 * "functionally part of" the physics/collision subsystem in the
 * strongest sense: same table convention, same early-out gate, same
 * overlap primitive, same caller.
 *
 * NAKED transcription, not real C: the same established
 * single-inlined-AABB-build shape as `sub_800CEAC` above (see its own
 * header comment) layered on a `sub_801070C`/`sub_8010708`
 * neighbor-list read - `sub_800E494`/`sub_800E4E4`
 * (src/system/game_loop7.c) already document that even the *simpler*
 * neighbor-list-walk shape (no AABB build at all) resists gcc 2.9's
 * exact instruction scheduling for this project's compiler on a
 * `0x7f`-mask-before-load ordering; combined with the AABB-build
 * resistance, re-attempting C here was not a good use of time given
 * both known-resistant shapes are stacked in the same function.
 * Verified byte-exact via the isolated `cpp`/`agbcc`/`as` +
 * `objcopy`/`cmp` pipeline against `baserom.gba`'s own bytes at
 * `0x0800CF70`-`0x0800D040` (only the four `bl` relocation sites
 * differ, resolving correctly once linked). */
NAKED void *sub_800CF70(void *self, struct aabb *box, u8 *foundFlag)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #0x10\n\t"
        "mov sl, r1\n\t"
        "add r5, r2, #0\n\t"
        "add r7, r0, #0\n\t"
        "bl sub_801070C\n\t"
        "add r4, r0, #0\n\t"
        "add r0, r7, #0\n\t"
        "bl sub_8010708\n\t"
        "add r6, r0, #0\n\t"
        "cmp r4, #0\n\t"
        "bne 1f\n\t"
        "cmp r6, #0\n\t"
        "beq 5f\n\t"
    "1:\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r5]\n\t"
        "cmp r6, #0\n\t"
        "beq 5f\n\t"
        "add r1, r6, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #1\n\t"
        "beq 5f\n\t"
        "ldr r1, [r6, #0x20]\n\t"
        "add r2, r6, #0\n\t"
        "add r2, #0x2d\n\t"
        "ldrb r3, [r2]\n\t"
        "lsl r0, r3, #3\n\t"
        "sub r0, r0, r3\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, r1, r0\n\t"
        "add r3, r1, #4\n\t"
        "ldr r0, [r6]\n\t"
        "asr r0, r0, #8\n\t"
        "mov r8, r0\n\t"
        "ldr r0, [r6, #4]\n\t"
        "asr r0, r0, #8\n\t"
        "mov sb, r0\n\t"
        "mov r0, #4\n\t"
        "ldrsh r1, [r1, r0]\n\t"
        "mov r0, #2\n\t"
        "ldrsh r2, [r3, r0]\n\t"
        "ldrb r4, [r3, #4]\n\t"
        "ldrb r5, [r3, #5]\n\t"
        "add r1, r8\n\t"
        "add r2, sb\n\t"
        "mov r0, sp\n\t"
        "bl sub_803AFE4\n\t"
        "mov r0, sp\n\t"
        "add r1, r4, #0\n\t"
        "add r2, r5, #0\n\t"
        "bl sub_803AFDC\n\t"
        "add r3, r7, #0\n\t"
        "add r3, #0x28\n\t"
        "ldrb r1, [r3]\n\t"
        "lsl r0, r1, #0x1b\n\t"
        "cmp r0, #0\n\t"
        "bge 2f\n\t"
        "mov r1, r8\n\t"
        "lsl r0, r1, #1\n\t"
        "ldr r1, [sp]\n\t"
        "ldr r2, [sp, #8]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp]\n\t"
    "2:\n\t"
        "ldrb r3, [r3]\n\t"
        "lsl r0, r3, #0x1a\n\t"
        "cmp r0, #0\n\t"
        "bge 3f\n\t"
        "mov r3, sb\n\t"
        "lsl r0, r3, #1\n\t"
        "ldr r1, [sp, #4]\n\t"
        "ldr r2, [sp, #0xc]\n\t"
        "add r1, r1, r2\n\t"
        "sub r0, r0, r1\n\t"
        "str r0, [sp, #4]\n\t"
    "3:\n\t"
        "mov r0, sp\n\t"
        "mov r1, sl\n\t"
        "bl sub_8001688\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 5f\n\t"
        "add r7, r6, #0\n\t"
    "5:\n\t"
        "add r0, r7, #0\n\t"
        "add sp, #0x10\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1"
    );
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
