#include "core.h"
#include "vram_pool.h"

/* GitHub issue #12 Phase 2: 0x0800E560-0x0800EEF0, the lower-address
 * half of the remaining tail of the physics/collision subsystem's
 * per-edge handler family (see docs/matching/issue-12-physics-collision.md's
 * "Phase 1" appendix for the confirmed dispatch map both
 * sub_0800D18C/sub_800E08C, src/system/game_loop47.c, dispatch into).
 * `self` throughout is the same "collision box" object every other
 * function in this subsystem operates on - offsets kept raw rather
 * than a named struct, matching every already-matched sibling in this
 * file family (game_loop6.c-game_loop47.c). */

extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern void sub_8009150(void *manager, void *objArg);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern struct tile_asset_cache *gUnknown_030012B8;
extern u8 sub_8006DF8(struct tile_asset_cache *self, s32 recordId);
extern void *gUnknown_0300130C;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012D8;
extern void *gUnknown_030012E4;
extern void *gUnknown_030012C0;
extern void *gUnknown_030012B4;
extern u8 gStaticData_0816BB98[];
extern void *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);
extern void sub_8022FEC(void *self);
extern void sub_8022CA0(void *self, u8 arg1);
extern void sub_80259D4(void *self, s32 n);
extern s32 sub_802599C(void *self, s32 n);
extern void sub_8025A64(void *unused0, s32 x, s32 y, u8 p3, u32 p5, u8 flag6);
extern void sub_800EAFC(void *self, u32 arg1);
extern void sub_800EEF0(void *self, u32 arg1);

/* NAKED transcription, not real C: `r3` (`&self[0x50]`) and `r4`
 * (constant `1`) both stay live across a `bl sub_8025CA4` call from
 * one conditional arm to a shared tail several branches later
 * (matching this compiler's callee-saved-register preservation, but
 * a plain C draft never reproduced the ROM's exact choice of which
 * two values stay pinned that far apart), and the two-way
 * `_0800E60C`/tail-of-`_0800E5AE`-block convergence onto a single
 * shared return block needed the same `goto`-style block-ordering
 * control this subsystem's other NAKED functions already use. Both
 * `sub_0800D18C`'s and `sub_800E08C`'s per-edge jump tables' case 3
 * eventually reach this handler transitively (via `sub_800E7A8`'s own
 * "walk to next neighbor whose dispatch id is 4" case, per
 * docs/matching/issue-12-physics-collision.md's dispatch map) as the
 * dispatch-id-4 target `sub_800EEF0` itself calls back into via
 * `sub_800E7A8` at its own bottom.
 *
 * Arms `self`'s `+0x48` frame-countdown timer to `0x168` (360) the
 * first time it's seen at its sentinel value (`-0x2a`), clearing
 * `+0x51`'s retry counter alongside it. While that countdown is
 * still running and `self`'s own `+0x50` byte is zero, bumps `+0x51`
 * each call; once it passes 4, calls `sub_800E7A8(self, 0, 0, 0)`
 * (the "give up, hand off" case). Otherwise (still under the retry
 * cap), sets `self+0x4d` bit `0x80`, marks
 * `gUnknown_030012D8+0x80 = 1`, arms a fresh `+0x4f = 6` sub-timer,
 * and spawns a pair of particle effects (`sub_8025CA4`, effect kind
 * `0xe`) at `self`'s position, offset `-6`/`+3` pixels on Y/X. Once
 * the `+0x48` countdown itself expires (`<= 0`), calls
 * `sub_800E7A8(self, 0, 0, 0)` unconditionally instead. */
NAKED void sub_800E560(void *self)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #8\n\t"
        "add r7, r0, #0\n\t"
        "ldr r1, [r7, #0x48]\n\t"
        "mov r0, #0x2a\n\t"
        "neg r0, r0\n\t"
        "cmp r1, r0\n\t"
        "bne 1f\n\t"
        "mov r0, #0xb4\n\t"
        "lsl r0, r0, #1\n\t"
        "str r0, [r7, #0x48]\n\t"
        "add r1, r7, #0\n\t"
        "add r1, #0x51\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
    "1:\n\t"
        "ldr r0, [r7, #0x48]\n\t"
        "cmp r0, #0\n\t"
        "ble 2f\n\t"
        "add r3, r7, #0\n\t"
        "add r3, #0x50\n\t"
        "ldrb r0, [r3]\n\t"
        "cmp r0, #0\n\t"
        "bne 3f\n\t"
        "add r1, r7, #0\n\t"
        "add r1, #0x51\n\t"
        "ldrb r0, [r1]\n\t"
        "add r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #4\n\t"
        "bls 4f\n\t"
        "add r0, r7, #0\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "bl sub_800E7A8\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "add r1, r7, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x80\n\t"
        "ldrb r2, [r1]\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r1]\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #1\n\t"
        "add r0, #0x80\n\t"
        "strb r1, [r0]\n\t"
        "add r2, r7, #0\n\t"
        "add r2, #0x4f\n\t"
        "mov r0, #6\n\t"
        "strb r0, [r2]\n\t"
        "strb r1, [r3]\n\t"
    "5:\n\t"
        "ldr r1, [r7]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r2, [r7, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "sub r2, #6\n\t"
        "ldr r6, 12f\n\t"
        "ldr r0, [r6]\n\t"
        "mov r3, #0xe\n\t"
        "str r3, [sp]\n\t"
        "add r5, sp, #4\n\t"
        "mov r4, #1\n\t"
        "strb r4, [r5]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025CA4\n\t"
        "ldr r1, [r7]\n\t"
        "asr r1, r1, #8\n\t"
        "add r1, #3\n\t"
        "ldr r2, [r7, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r0, [r6]\n\t"
        "mov r3, #0\n\t"
        "str r3, [sp]\n\t"
        "strb r4, [r5]\n\t"
        "bl sub_8025CA4\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "11: .4byte gUnknown_030012D8\n"
    "12: .4byte gUnknown_030012E4\n"
    "2:\n\t"
        "add r0, r7, #0\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0\n\t"
        "mov r3, #0\n\t"
        "bl sub_800E7A8\n\t"
    "3:\n\t"
        "add sp, #8\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}

/* Case-2 handler ("dispatch id 0xe") both `sub_0800D18C`'s and
 * `sub_800E08C`'s per-edge jump tables select - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Switches
 * `self` into a fresh sub-state (`+0x4e = 0x15`, hitbox tag `+0x2d =
 * 0x14`), rebuilds its hitbox record (`sub_80087C0`/`sub_80087B4`/
 * `sub_800872C`, the same trio every hitbox-rebuild call in this
 * subsystem uses), registers it with the object-pool grid
 * (`sub_8009150`), re-derives a low-nibble sub-animation value from
 * the freshly selected hitbox record's `+0x14` byte via
 * `sub_8006DF8`'s tile-asset-cache lookup, plays SFX `0x11`, and
 * arms a `+0x4f` countdown of `0x3c` (60) frames. */
void sub_800E620(void *selfArg)
{
    u8 *self = selfArg;
    u8 *entry;
    u8 lo;

    self[0x4e] = 0x15;
    {
        /* Anchored: the ROM loads the `0x14` immediate before computing
         * `&self[0x2d]` for this store (the opposite order from the
         * previous `self[0x4e] = 0x15` store just above, which computes
         * its address first) - a plain C `self[0x2d] = 0x14;` here
         * always picks the address-first order for both stores. `addr2d`
         * is kept as the live `&self[0x2d]` pointer (matching the ROM's
         * own r5) rather than recomputed, since the ROM's later tag
         * read reuses this same register. */
        register u8 *addr2d asm("r5");

        asm volatile(
            "mov r0, #0x14\n\t"
            "add r5, %1, #0\n\t"
            "add r5, r5, #0x2d\n\t"
            "strb r0, [r5]\n\t"
            : "=r"(addr2d)
            : "r"(self)
            : "r0", "cc", "memory"
        );

        sub_80087C0(self);
        sub_80087B4(self);
        sub_800872C(self, 0);
        /* Anchored: the ROM materializes the `0x10` immediate before
         * loading `self[0xc]`, not after - a plain C `self[0xc] |=
         * 0x10;` (in either operand order) always loads the field
         * first here. */
        {
            register u32 flagsResult asm("r0");

            asm volatile(
                "mov r0, #0x10\n\t"
                "ldrb r1, [%1, #0xc]\n\t"
                "orr r0, r1\n\t"
                "strb r0, [%1, #0xc]\n\t"
                : "=r"(flagsResult)
                : "r"(self)
                : "r1", "cc", "memory"
            );
        }
        sub_8009150(gUnknown_0300130C, self);

        {
            register u8 **p2 asm("r0") = *(u8 ***)(self + 0x20);
            register u8 *table2 asm("r1") = *p2;
            register u8 tag2 asm("r2") = *addr2d;
            register u8 *entry2 asm("r1");

        asm volatile(
            "lsl r0, %2, #3\n\t"
            "sub r0, r0, %2\n\t"
            "lsl r0, r0, #2\n\t"
            "add %0, %1, r0\n\t"
            : "=r"(entry2)
            : "r"(table2), "r"(tag2)
            : "r0", "cc"
        );
            entry = entry2;
        }
    }
    lo = sub_8006DF8(gUnknown_030012B8, entry[0x14]);
    /* Empty compiler barrier: forces the u8->u32 zero-extend implied by
     * `lo`'s use below to happen as its own step (matching the ROM's
     * `lsls r0,r0,0x18; lsrs r0,r0,0x18`), rather than letting the
     * optimizer fuse it into the `& 0xf` mask below into a single
     * shift-mask-shift sequence. */
    asm volatile("" : "+r"(lo));
    /* Anchored: the ROM computes `&self[0x29]` *before* masking `lo`
     * down to its low nibble (a plain C `self[0x29] = (self[0x29] &
     * ~0xf) | (lo & 0xf);` here always computes the mask first
     * regardless of source statement order), and materializes the
     * `~0xf` clear-mask at runtime (`movs r1,#0x10; rsbs r1,r1,#0`,
     * the negative-constant register-pinned mask idiom - see
     * matching_decomp_register_pinning and sub_8010480's own use of
     * it, game_loop35.c) rather than folding it into an 8-bit AND
     * immediate, ORing into the mask register (not the freshly-
     * extracted low-nibble register) before storing - transcribed as
     * one block to pin the whole sequence's order and registers at
     * once. */
    {
        register u8 rawLo asm("r0") = lo;

        asm volatile(
            "add r2, %1, #0\n\t"
            "add r2, r2, #0x29\n\t"
            "mov r1, #0xf\n\t"
            "and r0, r1\n\t"
            "mov r1, #0x10\n\t"
            "neg r1, r1\n\t"
            "ldrb r3, [r2]\n\t"
            "and r1, r3\n\t"
            "orr r1, r0\n\t"
            "strb r1, [r2]\n\t"
            : "+r"(rawLo)
            : "r"(self)
            : "r1", "r2", "r3", "cc", "memory"
        );
    }

    PlaySfx(gUnknown_030012BC, 0x11, 0x100);
    self[0x4f] = 0x3c;
}

/* Case-5 handler both `sub_0800D18C`'s and `sub_800E08C`'s per-edge
 * jump tables select unconditionally - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Spawns
 * a particle-effect object (`sub_8025BAC`, kind `0x2a`) at `self`'s
 * position (minus 10 pixels on X), initializes it (clearing flag bits
 * `+0xc`/`+0x28`, arming `+0x64`/`+0x54`/`+0x58`/`+0x5c` with a fixed
 * "settle" trajectory), then switches `self` itself into sub-state
 * `+0x2d = 0x1b`, rebuilds its own hitbox record, plays SFX `0x17`,
 * notifies `sub_80259D4` unless `self`'s `+8` id field is the
 * sentinel `0xffff`, conditionally reactivates the viewport
 * (`sub_8022FEC`, gated on `gStaticData_0816BB98[self+0x4e]`), and
 * ends by telling `sub_8022CA0` whether `self`'s `+0x50` byte is
 * nonzero before resetting `self`'s own `+0x4d` state byte to `1`. */
/* NAKED transcription, not real C: a zero constant (`r5`) stays live
 * in a single register from the top of the function (used once for
 * the spawn call's 6th argument) all the way to near the bottom
 * (reused for `gUnknown_030012D8[0x80] = 0`), and `self` is kept in a
 * single register (`r4`) throughout despite two intervening calls -
 * a plain C draft of this function always split `self` across a
 * second register (`r5`) for its own tail-section reads and never
 * reproduced the far-apart constant reuse, matching neither this
 * function's own push list (`{r4,r5,lr}`, not `{r4,r5,r6,lr}`) nor
 * its instruction count. Transcribed instruction-for-instruction from
 * the ROM disassembly instead, the same escape hatch already
 * established for this subsystem's other resistant functions (see
 * docs/matching/issue-12-physics-collision.md). */
NAKED void sub_800E6B0(void *self)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "sub sp, #8\n\t"
        "add r4, r0, #0\n\t"
        "ldr r3, [r4]\n\t"
        "asr r3, r3, #8\n\t"
        "sub r3, #0xa\n\t"
        "ldr r1, [r4, #4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r0, 1f\n\t"
        "ldr r0, [r0]\n\t"
        "str r1, [sp]\n\t"
        "mov r5, #0\n\t"
        "str r5, [sp, #4]\n\t"
        "mov r1, #0x2a\n\t"
        "mov r2, #0\n\t"
        "bl sub_8025BAC\n\t"
        "mov r1, #5\n\t"
        "neg r1, r1\n\t"
        "ldrb r2, [r0, #0xc]\n\t"
        "and r1, r2\n\t"
        "strb r1, [r0, #0xc]\n\t"
        "add r2, r0, #0\n\t"
        "add r2, #0x28\n\t"
        "mov r1, #0x11\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "strb r1, [r2]\n\t"
        "ldr r1, 2f\n\t"
        "mov r2, #8\n\t"
        "mov r3, #0x10\n\t"
        "neg r3, r3\n\t"
        "str r1, [r0, #0x64]\n\t"
        "str r1, [r0, #0x54]\n\t"
        "str r2, [r0, #0x58]\n\t"
        "str r3, [r0, #0x5c]\n\t"
        "mov r0, #0x1b\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x2d\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "ldr r0, 3f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #0x17\n\t"
        "bl PlaySfx\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "ldr r0, 4f\n\t"
        "cmp r1, r0\n\t"
        "beq 5f\n\t"
        "ldr r0, 6f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_80259D4\n\t"
    "5:\n\t"
        "ldr r0, 7f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4e\n\t"
        "ldrb r1, [r1]\n\t"
        "add r0, r1, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 8f\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8022FEC\n\t"
    "8:\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x50\n\t"
        "ldrb r3, [r2]\n\t"
        "neg r1, r3\n\t"
        "orr r1, r3\n\t"
        "lsr r1, r1, #0x1f\n\t"
        "bl sub_8022CA0\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r2, [r1]\n\t"
        "and r0, r2\n\t"
        "strb r0, [r1]\n\t"
        "ldr r0, 10f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x80\n\t"
        "strb r5, [r0]\n\t"
        "mov r2, #1\n\t"
        "mov r0, #0x80\n\t"
        "ldrb r3, [r1]\n\t"
        "and r0, r3\n\t"
        "orr r0, r2\n\t"
        "strb r0, [r1]\n\t"
        "add sp, #8\n\t"
        "pop {r4, r5}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012E4\n"
    "2: .4byte 0xFFFFFE80\n"
    "3: .4byte gUnknown_030012BC\n"
    "4: .4byte 0x0000FFFF\n"
    "6: .4byte gUnknown_030012B4\n"
    "7: .4byte gStaticData_0816BB98\n"
    "9: .4byte gUnknown_030012C0\n"
    "10: .4byte gUnknown_030012D8\n"
    );
}

/* Case-3 handler both `sub_0800D18C`'s and `sub_800E08C`'s per-edge
 * jump tables select (see docs/matching/issue-12-physics-collision.md's
 * dispatch map): counts `self` into `gUnknown_030012D8+0x91`'s
 * "objects handled this frame" tally (saturating at a nonzero value -
 * only the very first caller of the frame actually increments it,
 * gated on `arg2`), then walks `self`'s "get prev" (`arg3 == 4`) or
 * "get next" (`arg3 == 8`) neighbor chain past every node whose
 * `+0x4d & 0x7f` state is already `1`, stopping at the first node
 * that isn't (or the last reachable node if the whole chain is state
 * `1`). Neither `arg3` value falls back to `self` itself as the
 * target. Finally, unless `gStaticData_0816BBDA[target+0x4e]` is
 * nonzero, dispatches to `sub_800E888(target, arg1)` - the shared
 * tail every one of this handler's paths converges on.
 *
 * NAKED transcription, not real C: two independent list-walk loops
 * (one per neighbor direction) each keep their own walk cursor and a
 * shared "last accepted node" register live across the loop body,
 * merging into a single downstream register (`r2`) at three different
 * convergence points - the same "which register stays live across a
 * merge" shape `sub_8010914`/`sub_801095C` (game_loop30.c) already
 * document as gcc-2.9-resistant for a single such loop, doubled here.
 * See docs/matching/issue-12-physics-collision.md. */
NAKED void sub_800E7A8(void *self, u32 arg1, u32 arg2, u32 arg3)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "add r5, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r6, r1, #0x18\n\t"
        "lsl r2, r2, #0x18\n\t"
        "cmp r2, #0\n\t"
        "beq 1f\n\t"
        "ldr r2, 10f\n\t"
        "ldr r0, [r2]\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x91\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0\n\t"
        "bne 9f\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
        "ldr r0, [r2]\n\t"
        "add r0, #0x91\n\t"
        "ldrb r1, [r0]\n\t"
        "add r1, #1\n\t"
        "strb r1, [r0]\n\t"
    "1:\n\t"
        "cmp r3, #4\n\t"
        "bne 2f\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8010708\n\t"
        "add r4, r0, #0\n\t"
        "cmp r4, #0\n\t"
        "beq 3f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #1\n\t"
        "beq 3f\n\t"
    "4:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_8010708\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "beq 5f\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #1\n\t"
        "beq 5f\n\t"
        "add r4, r2, #0\n\t"
        "b 4b\n\t"
        ".align 2, 0\n"
    "10: .4byte gUnknown_030012D8\n"
    "2:\n\t"
        "cmp r3, #8\n\t"
        "bne 8f\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_801070C\n\t"
        "add r4, r0, #0\n\t"
        "cmp r4, #0\n\t"
        "beq 3f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #1\n\t"
        "bne 6f\n\t"
    "3:\n\t"
        "add r2, r5, #0\n\t"
        "b 7f\n\t"
    "5:\n\t"
        "add r2, r4, #0\n\t"
        "b 7f\n\t"
    "6:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801070C\n\t"
        "add r2, r0, #0\n\t"
        "cmp r2, #0\n\t"
        "beq 5b\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #1\n\t"
        "beq 5b\n\t"
        "add r4, r2, #0\n\t"
        "b 6b\n\t"
    "7:\n\t"
        "ldr r0, 11f\n\t"
        "add r1, r2, #0\n\t"
        "add r1, #0x4e\n\t"
        "ldrb r1, [r1]\n\t"
        "add r0, r1, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "bne 9f\n\t"
        "add r0, r2, #0\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_800E888\n\t"
        "b 9f\n\t"
        ".align 2, 0\n"
    "11: .4byte gStaticData_0816BBDA\n"
    "8:\n\t"
        "add r0, r5, #0\n\t"
        "add r1, r6, #0\n\t"
        "bl sub_800E888\n\t"
    "9:\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}

/* `sub_800E7A8`'s (and, transitively, both of the subsystem's
 * top-level dispatchers') shared "actually apply the collision
 * response" landing point - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Early-
 * outs when `self+0x4d & 0x7f == 1` (already fully handled this
 * frame). Otherwise: registers `self` with the object-pool grid,
 * resets its `+0x4d` state byte to `0x81` and clears
 * `gUnknown_030012D8+0x80`, switches `self` into hitbox tag `0x1d`
 * and rebuilds its hitbox record, re-derives its `+0x29` low-nibble
 * sub-animation value (same `sub_8006DF8` tile-asset-cache lookup
 * `sub_800E620` uses) and clamps `self+0x30`'s index to the newly
 * selected hitbox record's own `+0x16` count, conditionally
 * reactivates the viewport (`sub_8022FEC`, gated on
 * `gStaticData_0816BB98[self+0x4e]`), flips one bit of
 * `gUnknown_030012B4`'s bit-grid keyed by `self+8`, calls
 * `sub_800EDBC` (neighbor "impact spread" propagation), then
 * dispatches a 23-case jump table on `self`'s freshly-cached
 * `+0x4e` state id to one of this subsystem's other per-state leaf
 * handlers (`sub_801085C`/`sub_801089C`/`sub_800F368`/`sub_800F2BC`/
 * `sub_800EAFC`/`sub_800ED08`/`sub_800EEF0`/`sub_8022EA8`, or a
 * SFX-3-plus-particle-spawn fallback) before converging on a shared
 * epilogue.
 *
 * NAKED transcription, not real C: a 23-case jump table (the largest
 * in this subsystem after `sub_0800D18C`'s own three) inside a
 * function that also needs `r8`/`sb` as two extra callee-saved
 * accumulators live across most of the body (`self+0x4e`'s address
 * cached in `sb`, a `1`/`self`-derived flag cached in `r8`) - the
 * same confirmed gcc-2.9-resistant shape `sub_800D040`'s own doc
 * comment documents, again here. See
 * docs/matching/issue-12-physics-collision.md. */
NAKED void sub_800E888(void *self, u32 arg1)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sb\n\t"
        "mov r6, r8\n\t"
        "push {r6, r7}\n\t"
        "sub sp, #8\n\t"
        "add r4, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r6, r1, #0x18\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #1\n\t"
        "bne 1f\n\t"
        "b 32f\n\t"
    "1:\n\t"
        "mov r7, #0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801070C\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "cmp r6, #0\n\t"
        "bne 2f\n\t"
        "mov r7, #1\n\t"
    "2:\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r1, [r4, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r4, #0xc]\n\t"
        "ldr r0, 6f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r4, #0\n\t"
        "bl sub_8009150\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r3, [r2]\n\t"
        "and r0, r3\n\t"
        "mov r1, #0\n\t"
        "strb r0, [r2]\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, #0x80\n\t"
        "strb r1, [r0]\n\t"
        "mov r5, #1\n\t"
        "mov r8, r5\n\t"
        "mov r0, #0x80\n\t"
        "ldrb r1, [r2]\n\t"
        "and r0, r1\n\t"
        "orr r0, r5\n\t"
        "strb r0, [r2]\n\t"
        "mov r0, #0x1d\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #0x2d\n\t"
        "strb r0, [r5]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087C0\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_80087B4\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800872C\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r2, [r5]\n\t"
        "lsl r0, r2, #3\n\t"
        "sub r0, r0, r2\n\t"
        "lsl r0, r0, #2\n\t"
        "add r1, r1, r0\n\t"
        "ldr r0, 8f\n\t"
        "ldr r0, [r0]\n\t"
        "ldrb r1, [r1, #0x14]\n\t"
        "bl sub_8006DF8\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "add r2, r4, #0\n\t"
        "add r2, #0x29\n\t"
        "mov r1, #0xf\n\t"
        "and r0, r1\n\t"
        "mov r1, #0x10\n\t"
        "neg r1, r1\n\t"
        "ldrb r3, [r2]\n\t"
        "and r1, r3\n\t"
        "orr r1, r0\n\t"
        "strb r1, [r2]\n\t"
        "mov r2, #3\n\t"
        "ldr r0, [r4, #0x20]\n\t"
        "ldr r1, [r0]\n\t"
        "ldrb r3, [r5]\n\t"
        "lsl r0, r3, #3\n\t"
        "sub r0, r0, r3\n\t"
        "lsl r0, r0, #2\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #0x16]\n\t"
        "cmp r2, r0\n\t"
        "blt 3f\n\t"
        "sub r2, r0, #1\n\t"
    "3:\n\t"
        "str r2, [r4, #0x30]\n\t"
        "ldr r0, 9f\n\t"
        "mov r5, #0x4e\n\t"
        "add r5, r5, r4\n\t"
        "mov sb, r5\n\t"
        "ldrb r1, [r5]\n\t"
        "add r0, r1, r0\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 4f\n\t"
        "ldr r0, 10f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_8022FEC\n\t"
    "4:\n\t"
        "ldrh r3, [r4, #8]\n\t"
        "ldr r0, 11f\n\t"
        "ldr r2, [r0]\n\t"
        "add r0, r3, #0\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r1, r0, #2\n\t"
        "mov r5, #0x84\n\t"
        "lsl r5, r5, #1\n\t"
        "add r2, r2, r5\n\t"
        "add r2, r2, r1\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r3, r0\n\t"
        "mov r1, r8\n\t"
        "lsl r1, r0\n\t"
        "ldr r0, [r2]\n\t"
        "orr r0, r1\n\t"
        "str r0, [r2]\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800EDBC\n\t"
        "mov r1, sb\n\t"
        "ldrb r0, [r1]\n\t"
        "cmp r0, #0x16\n\t"
        "bls 5f\n\t"
        "b 32f\n\t"
    "5:\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, 12f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "6: .4byte gUnknown_0300130C\n"
    "7: .4byte gUnknown_030012D8\n"
    "8: .4byte gUnknown_030012B8\n"
    "9: .4byte gStaticData_0816BB98\n"
    "10: .4byte gUnknown_030012C0\n"
    "11: .4byte gUnknown_030012B4\n"
    "12: .4byte 13f\n"
    "13:\n\t"
        ".4byte 29f\n\t"
        ".4byte 32f\n\t"
        ".4byte 14f\n\t"
        ".4byte 16f\n\t"
        ".4byte 19f\n\t"
        ".4byte 32f\n\t"
        ".4byte 17f\n\t"
        ".4byte 32f\n\t"
        ".4byte 32f\n\t"
        ".4byte 15f\n\t"
        ".4byte 21f\n\t"
        ".4byte 18f\n\t"
        ".4byte 19f\n\t"
        ".4byte 19f\n\t"
        ".4byte 21f\n\t"
        ".4byte 22f\n\t"
        ".4byte 23f\n\t"
        ".4byte 25f\n\t"
        ".4byte 27f\n\t"
        ".4byte 21f\n\t"
        ".4byte 21f\n\t"
        ".4byte 21f\n\t"
        ".4byte 32f\n\t"
    "14:\n\t"
        "cmp r6, #0\n\t"
        "bne 32f\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_801085C\n\t"
        "b 32f\n\t"
    "15:\n\t"
        "cmp r6, #0\n\t"
        "bne 32f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_801089C\n\t"
        "b 32f\n\t"
    "16:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800F368\n\t"
        "b 32f\n\t"
    "17:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_800F2BC\n\t"
        "b 32f\n\t"
    "18:\n\t"
        "cmp r6, #0\n\t"
        "bne 32f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_800EAFC\n\t"
        "b 32f\n\t"
    "19:\n\t"
        "ldr r0, 20f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #3\n\t"
        "bl PlaySfx\n\t"
        "b 32f\n\t"
        ".align 2, 0\n"
    "20: .4byte gUnknown_030012BC\n"
    "21:\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #0\n\t"
        "bl sub_800EEF0\n\t"
        "b 32f\n\t"
    "22:\n\t"
        "cmp r6, #0\n\t"
        "bne 32f\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_800ED08\n\t"
        "b 32f\n\t"
    "23:\n\t"
        "ldr r0, 24f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #1\n\t"
        "bl sub_8022EA8\n\t"
        "b 32f\n\t"
        ".align 2, 0\n"
    "24: .4byte gUnknown_030012C0\n"
    "25:\n\t"
        "ldr r0, 26f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #2\n\t"
        "bl sub_8022EA8\n\t"
        "b 32f\n\t"
        ".align 2, 0\n"
    "26: .4byte gUnknown_030012C0\n"
    "27:\n\t"
        "ldr r0, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #3\n\t"
        "bl sub_8022EA8\n\t"
        "b 32f\n\t"
        ".align 2, 0\n"
    "28: .4byte gUnknown_030012C0\n"
    "29:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #3\n\t"
        "ldr r0, 30f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #3\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r7, [r3]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025CA4\n\t"
        "cmp r6, #0\n\t"
        "bne 32f\n\t"
        "ldr r0, 31f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #3\n\t"
        "bl PlaySfx\n\t"
    "32:\n\t"
        "add sp, #8\n\t"
        "pop {r3, r4}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "30: .4byte gUnknown_030012E4\n"
    "31: .4byte gUnknown_030012BC\n"
    );
}

/* Case-11 handler of `sub_800E888`'s own 23-case jump table (dispatch
 * id `0xb`) - see docs/matching/issue-12-physics-collision.md's
 * dispatch map. Plays SFX 3, then (the first time `self`'s `+0x51`
 * retry counter is exactly `9`) rolls a random "escalation level"
 * (`1`/`4`/`7`/`8`, weighted via three `rand()` thresholds) into that
 * same byte. Dispatches a second, 10-case jump table on
 * `(self+0x51 - 1)` (clamped, values above 10 fall to the same
 * "final" case as 0): cases 5 down through 0 deliberately
 * *fall through* into each other without their own return, cascading
 * multiple `sub_8025CA4` particle spawns at slightly different
 * offsets around `self` the further the level counted down (a
 * escalating "more debris" burst); case 6 fires a screen-shake
 * (`sub_803AD88`, effect `0x1a`) plus SFX; case 7 spawns a
 * `sub_8025A64` bonus object and notifies `sub_80259D4`; case 9 spawns
 * one final small `sub_8025CA4` puff. All paths converge on a shared
 * epilogue.
 *
 * NAKED transcription, not real C: the cascading-fallthrough case
 * chain (five case blocks with no trailing `break`/`return`, each
 * one's own registers staying live into the next) isn't expressible
 * as a C `switch` without `goto`-chaining every case in exactly ROM
 * order, which this compiler's O2 pass then reorders/re-schedules
 * differently per block regardless - transcribed instruction-for-
 * instruction instead, the same escape hatch already established for
 * this subsystem's other jump-table-heavy functions. See
 * docs/matching/issue-12-physics-collision.md. */
NAKED void sub_800EAFC(void *self, u32 arg1)
{
    asm(
        "push {r4, r5, r6, lr}\n\t"
        "sub sp, #8\n\t"
        "add r4, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r6, r1, #0x18\n\t"
        "ldr r0, 2f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #3\n\t"
        "bl PlaySfx\n\t"
        "add r5, r4, #0\n\t"
        "add r5, #0x51\n\t"
        "ldrb r0, [r5]\n\t"
        "cmp r0, #9\n\t"
        "bne 1f\n\t"
        "bl rand\n\t"
        "lsl r0, r0, #0x10\n\t"
        "lsr r0, r0, #0x18\n\t"
        "add r1, r0, #0\n\t"
        "cmp r0, #0x56\n\t"
        "bhi 3f\n\t"
        "mov r0, #1\n\t"
        "b 6f\n\t"
        ".align 2, 0\n"
    "2: .4byte gUnknown_030012BC\n"
    "3:\n\t"
        "cmp r0, #0xd3\n\t"
        "bhi 4f\n\t"
        "mov r0, #4\n\t"
        "b 6f\n\t"
    "4:\n\t"
        "cmp r1, #0xec\n\t"
        "bhi 5f\n\t"
        "mov r0, #7\n\t"
        "b 6f\n\t"
    "5:\n\t"
        "mov r0, #8\n\t"
    "6:\n\t"
        "strb r0, [r5]\n\t"
    "1:\n\t"
        "add r0, r4, #0\n\t"
        "add r0, #0x51\n\t"
        "ldrb r0, [r0]\n\t"
        "sub r0, #1\n\t"
        "cmp r0, #9\n\t"
        "bls 7f\n\t"
        "b 8f\n\t"
    "7:\n\t"
        "lsl r0, r0, #2\n\t"
        "ldr r1, 9f\n\t"
        "add r0, r0, r1\n\t"
        "ldr r0, [r0]\n\t"
        "mov pc, r0\n\t"
        ".align 2, 0\n"
    "9: .4byte 10f\n"
    "10:\n\t"
        ".4byte 8f\n\t"
        ".4byte 27f\n\t"
        ".4byte 26f\n\t"
        ".4byte 25f\n\t"
        ".4byte 24f\n\t"
        ".4byte 23f\n\t"
        ".4byte 20f\n\t"
        ".4byte 14f\n\t"
        ".4byte 8f\n\t"
        ".4byte 11f\n"
    "11:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "ldr r0, 12f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #0xff\n\t"
        "str r3, [sp]\n\t"
        "add r4, sp, #4\n\t"
        "mov r3, #0\n\t"
        "strb r3, [r4]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025CA4\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "12: .4byte gUnknown_030012E4\n"
    "14:\n\t"
        "ldr r0, 16f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #3\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "ldr r0, 17f\n\t"
        "cmp r1, r0\n\t"
        "beq 15f\n\t"
        "ldr r5, 18f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_802599C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 15f\n\t"
        "ldr r0, [r5]\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "bl sub_80259D4\n\t"
    "15:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #3\n\t"
        "ldr r0, 19f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #3\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r6, [r3]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025A64\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "16: .4byte gUnknown_030012BC\n"
    "17: .4byte 0x0000FFFF\n"
    "18: .4byte gUnknown_030012B4\n"
    "19: .4byte gUnknown_030012E4\n"
    "20:\n\t"
        "ldr r0, 21f\n\t"
        "ldr r2, [r0]\n\t"
        "ldrb r1, [r2, #0xc]\n\t"
        "lsr r0, r1, #7\n\t"
        "cmp r0, #0\n\t"
        "beq 13f\n\t"
        "ldr r1, [r2, #0x18]\n\t"
        "add r1, #0x68\n\t"
        "mov r3, #0\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r2, r0\n\t"
        "ldr r4, [r1, #4]\n\t"
        "mov r1, #0\n\t"
        "mov r2, #0x1a\n\t"
        "mov r3, #0\n\t"
        "bl sub_803AD88\n\t"
        "ldr r0, 22f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r1, #1\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "bl PlaySfx\n\t"
        "b 13f\n\t"
        ".align 2, 0\n"
    "21: .4byte gUnknown_030012D8\n"
    "22: .4byte gUnknown_030012BC\n"
    "23:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "sub r1, #1\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #3\n\t"
        "ldr r0, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #3\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r6, [r3]\n\t"
        "mov r3, #1\n\t"
        "bl sub_8025CA4\n\t"
    "24:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "add r1, #1\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #1\n\t"
        "ldr r0, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #3\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r6, [r3]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025CA4\n\t"
    "25:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "sub r1, #3\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #3\n\t"
        "ldr r0, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #1\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r6, [r3]\n\t"
        "mov r3, #1\n\t"
        "bl sub_8025CA4\n\t"
    "26:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "add r1, #3\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #2\n\t"
        "ldr r0, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #1\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r6, [r3]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025CA4\n\t"
    "27:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "add r1, #5\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #2\n\t"
        "ldr r0, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #2\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r6, [r3]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025CA4\n\t"
    "8:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "sub r1, #5\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #3\n\t"
        "ldr r0, 28f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #2\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r6, [r3]\n\t"
        "mov r3, #1\n\t"
        "bl sub_8025CA4\n\t"
    "13:\n\t"
        "add sp, #8\n\t"
        "pop {r4, r5, r6}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "28: .4byte gUnknown_030012E4\n"
    );
}

/* Case-15 handler of `sub_800E888`'s own 23-case jump table (dispatch
 * id `0xf`) - see docs/matching/issue-12-physics-collision.md's
 * dispatch map. Plays SFX 3, then switches on `self+0x48 & 7`: `1`
 * plays SFX 3 again, notifies `sub_80259D4` unless `self`'s `+8` id
 * is the sentinel `0xffff` (or is already scheduled per
 * `sub_802599C`), and spawns a `sub_8025A64` bonus object 3 pixels
 * below `self`; `2` forwards to `sub_800EAFC` (the escalating-debris
 * handler above); `3` clears `self+0x4d` bit `0x7f` and calls
 * `sub_800EEF0(self, 1)`; any other value (including `0`) does
 * nothing further.
 *
 * NAKED transcription, not real C: a plain C `switch` on this exact
 * 4-value/1-default shape always lowers to a binary-search-style
 * compare chain (`==2` first, then `>2`, then `==1`) and spills two
 * extra callee-saved registers (`r8`/`r9`) for `self`/`arg1` across
 * the `PlaySfx`/`sub_802599C`/`sub_80259D4`/`sub_8025A64` calls -
 * never the ROM's own strictly-ascending `==1`/`<=1`/`==2`/`==3`
 * compare order (equivalent to a 4-entry gcc-2.9 jump-table-avoidance
 * chain keyed low-to-high) using only `r4`-`r7`. Transcribed
 * instruction-for-instruction instead, the same escape hatch already
 * established for this subsystem's other resistant functions. See
 * docs/matching/issue-12-physics-collision.md. */
NAKED void sub_800ED08(void *self, u32 arg1)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "sub sp, #8\n\t"
        "add r4, r0, #0\n\t"
        "lsl r1, r1, #0x18\n\t"
        "lsr r7, r1, #0x18\n\t"
        "ldr r5, 1f\n\t"
        "ldr r0, [r5]\n\t"
        "mov r6, #0x80\n\t"
        "lsl r6, r6, #1\n\t"
        "mov r1, #3\n\t"
        "add r2, r6, #0\n\t"
        "bl PlaySfx\n\t"
        "ldr r1, [r4, #0x48]\n\t"
        "mov r0, #7\n\t"
        "and r1, r0\n\t"
        "cmp r1, #1\n\t"
        "beq 2f\n\t"
        "cmp r1, #1\n\t"
        "ble 3f\n\t"
        "cmp r1, #2\n\t"
        "beq 4f\n\t"
        "cmp r1, #3\n\t"
        "beq 5f\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_030012BC\n"
    "2:\n\t"
        "ldr r0, [r5]\n\t"
        "mov r1, #3\n\t"
        "add r2, r6, #0\n\t"
        "bl PlaySfx\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "ldr r0, 6f\n\t"
        "cmp r1, r0\n\t"
        "beq 7f\n\t"
        "ldr r5, 8f\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_802599C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "bne 7f\n\t"
        "ldr r0, [r5]\n\t"
        "ldrh r1, [r4, #8]\n\t"
        "bl sub_80259D4\n\t"
    "7:\n\t"
        "ldr r1, [r4]\n\t"
        "asr r1, r1, #8\n\t"
        "ldr r2, [r4, #4]\n\t"
        "asr r2, r2, #8\n\t"
        "add r2, #3\n\t"
        "ldr r0, 9f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r3, #3\n\t"
        "str r3, [sp]\n\t"
        "add r3, sp, #4\n\t"
        "strb r7, [r3]\n\t"
        "mov r3, #0\n\t"
        "bl sub_8025A64\n\t"
        "b 3f\n\t"
        ".align 2, 0\n"
    "6: .4byte 0x0000FFFF\n"
    "8: .4byte gUnknown_030012B4\n"
    "9: .4byte gUnknown_030012E4\n"
    "4:\n\t"
        "add r0, r4, #0\n\t"
        "add r1, r7, #0\n\t"
        "bl sub_800EAFC\n\t"
        "b 3f\n\t"
    "5:\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x80\n\t"
        "ldrb r2, [r1]\n\t"
        "and r0, r2\n\t"
        "strb r0, [r1]\n\t"
        "add r0, r4, #0\n\t"
        "mov r1, #1\n\t"
        "bl sub_800EEF0\n\t"
    "3:\n\t"
        "add sp, #8\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}

/* Neighbor "impact spread" propagation, called once from
 * `sub_800E888`'s own body (not through either jump table) - see
 * docs/matching/issue-12-physics-collision.md's dispatch map. Derives
 * a base spread budget from `self`'s hitbox record's own `+9` byte
 * (`+1`, scaled by 256), then walks `self`'s "get next" neighbor
 * chain (`sub_801070C`), redistributing that budget across each
 * visited node's `+0x40`/`+0x44` "remaining spread" fields (first
 * node gets the whole thing computed from `self`'s own `+4`/`+0x44`
 * state, every node after that gets a running remainder carried
 * forward via `sb`), nudging each node's `+0x4c` byte toward 0 by the
 * caller-supplied `arg1`-derived step, re-registering it with the
 * object-pool grid, and - for any node whose `gStaticData_0816BBC4`
 * row is set, `self`'s own `+0x48` is clear, and its accumulated
 * `+0x44` spread exceeds `0x1600` - "graduating" it into state `0x48
 * = 1` (unless a neighbor-adjacency/`+0x4d` gate blocks it). Stops
 * when the walk runs out of neighbors.
 *
 * NAKED transcription, not real C: `r8`/`sb`/`sl` all stay live as
 * three extra callee-saved accumulators throughout the whole
 * neighbor-walk loop (`self` itself, the running spread remainder,
 * and the per-node base budget respectively) - the same confirmed
 * gcc-2.9-resistant "long-lived triple accumulator" shape
 * `sub_0800D18C`/`sub_800D040`'s own doc comments already document,
 * here combined with a loop whose back-edge condition itself depends
 * on which of two different `sb` initializations ran (a fresh `0` the
 * first iteration, the cached base budget every iteration after) -
 * not reproducible from a plain C loop. See
 * docs/matching/issue-12-physics-collision.md. */
NAKED void sub_800EDBC(void *self, u32 arg1)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, sl\n\t"
        "mov r6, sb\n\t"
        "mov r5, r8\n\t"
        "push {r5, r6, r7}\n\t"
        "sub sp, #4\n\t"
        "mov r8, r0\n\t"
        "mov r0, #0xfe\n\t"
        "str r0, [sp]\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1, #0x20]\n\t"
        "mov r2, r8\n\t"
        "add r2, #0x2d\n\t"
        "ldrb r3, [r2]\n\t"
        "lsl r1, r3, #3\n\t"
        "sub r1, r1, r3\n\t"
        "lsl r1, r1, #2\n\t"
        "ldr r0, [r0]\n\t"
        "add r0, r0, r1\n\t"
        "ldrb r0, [r0, #9]\n\t"
        "add r0, #1\n\t"
        "lsl r0, r0, #8\n\t"
        "mov sl, r0\n\t"
        "mov r0, r8\n\t"
        "bl sub_801070C\n\t"
        "add r5, r0, #0\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1, #0x44]\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "mov r2, #0xfc\n\t"
        "str r2, [sp]\n\t"
    "1:\n\t"
        "cmp r5, #0\n\t"
        "beq 12f\n\t"
        "mov r3, r8\n\t"
        "cmp r3, #0\n\t"
        "beq 12f\n\t"
        "cmp r0, #0\n\t"
        "beq 2f\n\t"
        "ldr r0, [r3, #0x40]\n\t"
        "b 3f\n\t"
    "2:\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1, #4]\n\t"
    "3:\n\t"
        "str r0, [r5, #0x40]\n\t"
        "ldr r7, [r5, #0x40]\n\t"
        "ldr r0, [r5, #4]\n\t"
        "sub r7, r7, r0\n\t"
        "cmp r7, #0\n\t"
        "bge 4f\n\t"
        "mov r7, #0\n\t"
    "4:\n\t"
        "mov r2, #0\n\t"
        "mov sb, r2\n\t"
    "5:\n\t"
        "cmp r5, #0\n\t"
        "beq 12f\n\t"
        "ldr r0, [r5, #0x44]\n\t"
        "cmp r0, #0\n\t"
        "beq 6f\n\t"
        "mov r3, sb\n\t"
        "add r0, r7, r3\n\t"
        "str r0, [r5, #0x44]\n\t"
        "ldr r0, [r5, #0x40]\n\t"
        "add r0, sb\n\t"
        "b 7f\n\t"
    "6:\n\t"
        "str r7, [r5, #0x44]\n\t"
        "ldr r0, [r5, #4]\n\t"
        "add r0, sl\n\t"
    "7:\n\t"
        "str r0, [r5, #0x40]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, #0x4c\n\t"
        "mov r1, #0\n\t"
        "ldrsb r1, [r2, r1]\n\t"
        "cmp r1, #0\n\t"
        "ble 8f\n\t"
        "mov r1, #0\n\t"
    "8:\n\t"
        "ldr r3, [sp]\n\t"
        "lsl r0, r3, #0x18\n\t"
        "asr r0, r0, #0x18\n\t"
        "add r0, r1, r0\n\t"
        "strb r0, [r2]\n\t"
        "mov r0, #0x10\n\t"
        "ldrb r1, [r5, #0xc]\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r5, #0xc]\n\t"
        "ldr r0, 13f\n\t"
        "ldr r0, [r0]\n\t"
        "add r1, r5, #0\n\t"
        "bl sub_8009150\n\t"
        "add r6, r5, #0\n\t"
        "add r6, #0x4e\n\t"
        "ldrb r2, [r6]\n\t"
        "ldr r3, 14f\n\t"
        "add r0, r2, r3\n\t"
        "ldrb r0, [r0]\n\t"
        "cmp r0, #0\n\t"
        "beq 11f\n\t"
        "mov r1, r8\n\t"
        "ldr r0, [r1, #0x48]\n\t"
        "cmp r0, #0\n\t"
        "bne 11f\n\t"
        "ldr r1, [r5, #0x44]\n\t"
        "mov r0, #0xb0\n\t"
        "lsl r0, r0, #5\n\t"
        "cmp r1, r0\n\t"
        "ble 11f\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_801070C\n\t"
        "add r4, r0, #0\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_8010708\n\t"
        "cmp r4, #0\n\t"
        "bne 10f\n\t"
        "cmp r0, #0\n\t"
        "beq 10f\n\t"
        "ldrb r6, [r6]\n\t"
        "cmp r6, #0xa\n\t"
        "bne 11f\n\t"
        "add r1, r5, #0\n\t"
        "add r1, #0x4d\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r1, [r1]\n\t"
        "and r0, r1\n\t"
        "cmp r0, #0\n\t"
        "bne 11f\n\t"
    "10:\n\t"
        "add r1, r5, #0\n\t"
        "add r1, #0x4f\n\t"
        "mov r0, #0\n\t"
        "strb r0, [r1]\n\t"
        "mov r0, #1\n\t"
        "str r0, [r5, #0x48]\n\t"
    "11:\n\t"
        "add r0, r5, #0\n\t"
        "bl sub_801070C\n\t"
        "add r5, r0, #0\n\t"
        "mov r2, sb\n\t"
        "cmp r2, #0\n\t"
        "bne 5b\n\t"
        "mov sb, sl\n\t"
        "b 5b\n\t"
        ".align 2, 0\n"
    "13: .4byte gUnknown_0300130C\n"
    "14: .4byte gStaticData_0816BBC4\n"
    "12:\n\t"
        "add sp, #4\n\t"
        "pop {r3, r4, r5}\n\t"
        "mov r8, r3\n\t"
        "mov sb, r4\n\t"
        "mov sl, r5\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}
