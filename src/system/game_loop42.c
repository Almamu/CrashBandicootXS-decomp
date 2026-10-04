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

extern void SetAabbPos(void *buf, s32 arg1, s32 arg2);
extern void SetAabbSize(void *buf, s32 arg1, s32 arg2);
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

#include "box_part.h"

/* The player as these two read it: box_part's mirror bits at +0x28 and
 * a "wide hitbox" byte at +0x90. */
struct ceac_player {
    u8 unk_00[0x28];
    u32 unk_28_0:4;     // 0x28
    u32 mirrorX:1;
    u32 mirrorY:1;
    u32 unk_28_6:2;
    u8 unk_29[0x67];
    u8 wide;            // 0x90
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
 * Real C (issue #9-#11 NAKED retry, matches under both compilers; the
 * file is built with old_agbcc for `sub_800CF70`): the wide-mode x is
 * `x += xOffset; x -= 2;` as two statements (the ROM's `adds r1, r1, r7;
 * subs r1, #2`). */
u8 sub_800CEAC(void *self, struct hitbox_quad *quad, struct aabb *box,
               s32 xOffset, s32 yOffset)
{
    struct aabb b;

    if (((struct ceac_player *)gUnknown_030012D8)->wide) {
        s32 x = quad->xOff;
        s32 y = quad->yOff;
        u8 h = quad->h;
        s32 w;

        x += xOffset;
        x -= 2;
        y += yOffset;
        w = quad->w + 4;
        SetAabbPos(&b, x, y);
        SetAabbSize(&b, w, h);
    } else {
        s32 x = quad->xOff;
        s32 y = quad->yOff;
        u8 w = quad->w;
        u8 h = quad->h;

        SetAabbPos(&b, x + xOffset, y + yOffset);
        SetAabbSize(&b, w, h);
    }
    if (((struct ceac_player *)gUnknown_030012D8)->mirrorX)
        b.field_0 = xOffset * 2 - (b.field_0 + b.field_8);
    if (((struct ceac_player *)gUnknown_030012D8)->mirrorY)
        b.field_4 = yOffset * 2 - (b.field_4 + b.field_c);
    if (sub_8001688(box, &b))
        return 1;
    return 0;
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
 * Real C under old_agbcc (issue #9-#11 NAKED retry; the `0x7f` mask
 * loaded before the `ldrb` is old_agbcc's tell). `self` goes through a
 * local copy of the parameter: that is what makes the ROM copy r0 last,
 * after `box`/`foundFlag`, right before the first call. The mirror bits
 * read are `self`'s, not `prev`'s. */
struct box_part *sub_800CF70(struct box_part *selfArg, struct aabb *box, u8 *foundFlag)
{
    struct box_part *self = selfArg;
    struct box_part *next = sub_801070C(self);
    struct box_part *prev = sub_8010708(self);

    if (next == NULL && prev == NULL)
        return self;
    *foundFlag = 1;
    if (prev == NULL || (prev->physMode & 0x7f) == 1)
        return self;
    {
        u8 *rec = (u8 *)&(*prev->keyframes)[prev->frame];
        struct hitbox_quad *q = (struct hitbox_quad *)(rec + 4);
        struct aabb b;
        s32 px = prev->x >> 8;
        s32 py = prev->y >> 8;
        s32 x = q->xOff;
        s32 y = q->yOff;
        u8 w = q->w;
        u8 h = q->h;

        SetAabbPos(&b, x + px, y + py);
        SetAabbSize(&b, w, h);
        if (self->mirrorX)
            b.field_0 = px * 2 - (b.field_0 + b.field_8);
        if (self->mirrorY)
            b.field_4 = py * 2 - (b.field_4 + b.field_c);
        if (sub_8001688(&b, box))
            self = prev;
    }
    return self;
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
