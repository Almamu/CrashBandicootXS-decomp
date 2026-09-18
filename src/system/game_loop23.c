#include "core.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). `sub_8010674` prepended
 * ahead of the already-matched `sub_80106DC` run below - it's
 * immediately ROM-adjacent (no gap), so it joins this file rather
 * than getting its own per docs/workflow.md's "one file per
 * contiguous ROM region" rule. See
 * docs/matching/issue-13-fc70-second-continuation.md for the
 * register-pinning/toolchain-bug notes this one needed. */

extern void *sub_803AD7C(void *arg0, void *arg1);

/* AABB-overlap test between `self`'s own table-driven half-width/
 * half-height box (centered on `self`'s own position, via the same
 * `sub_803AD7C` table-trampoline convention `sub_8007048`/
 * `sub_80070D4`, graphics.c, already establish - here at the table's
 * own `+0x10`/`+0x14` offset pair) and a caller-supplied `struct aabb
 * *`. Short-circuits true (skipping the real test) when `self+0xc`
 * bit 4 is set, or when `self+0x44` is nonzero. */
u32 sub_8010674(void *selfArg, struct aabb *boxArg)
{
    /* self/box pinned to r5/r6: the ROM keeps both live across the
     * whole function (self is dead by the AABB-build block below and
     * gets reused there for `top`; box stays live until the very last
     * compare). result is pinned to r1, matching the ROM's own
     * shared merge point for both the shortcut-true and real-test
     * return paths (`adds r1,r7,#0` / already-r1; `adds r0,r1,#0`) -
     * declaring it (and the function's return type) as `u32` rather
     * than `u8` avoids this compiler's narrow-register-return
     * zero-extend dance (an unconditional `lsls r0,r0,#24; lsrs
     * r0,r0,#24` a `u8`-typed register return always adds, that the
     * ROM never has - callers here still only read the low byte, so
     * the wider C return type changes nothing observable). */
    register u8 *self asm("r5") = selfArg;
    register struct aabb *box asm("r6") = boxArg;
    u8 skip = (self[0xc] >> 4) & 1;
    register u32 result asm("r1");

    if (*(s32 *)(self + 0x44) != 0) {
        skip = 1;
    }

    if (!skip) {
        register void *table asm("r1") = *(void **)(self + 0x18);
        register u8 *rec asm("r0") =
            sub_803AD7C(self + *(s16 *)((u8 *)table + 0x10),
                        *(void **)((u8 *)table + 0x14));
        register s32 left asm("r4");
        register s32 right asm("r1");
        register s32 top asm("r5");
        register s32 bottom asm("r3");
        u8 success;

        /* Anchored: builds `self`'s AABB (half-extents from the
         * `sub_803AD7C` record's `+4`/`+5` raw w/h bytes, shifted by
         * 7 rather than 8) directly into the ROM's own register
         * choices. Plain C here always let this compiler's scheduler
         * hoist the `self->y` load ahead of the still-pending
         * `rec[5] << 7` shift, stealing r3/r0 from each other (the
         * same "which anonymous scratch register" gap already
         * NAKED-parked for `sub_8007B00`/`sub_8007B98`,
         * actor_part.c) - anchored as one literal block instead,
         * matching `self`'s own register (r5, reused here for `top`
         * once `self` is dead) and `rec`'s (r0, reused for the
         * `self->y` load once `rec` is dead). */
        asm volatile(
            "ldrb r1, [r0, #4]\n\t"
            "lsl r2, r1, #7\n\t"
            "ldrb r0, [r0, #5]\n\t"
            "lsl r3, r0, #7\n\t"
            "ldr r1, [r5]\n\t"
            "sub r4, r1, r2\n\t"
            "ldr r0, [r5, #4]\n\t"
            "sub r5, r0, r3\n\t"
            "add r1, r1, r2\n\t"
            "add r3, r0, r3\n\t"
            : "=r"(left), "=r"(right), "=r"(top), "=r"(bottom)
            : "r"(rec), "r"(self)
            : "r0", "r2", "cc", "memory"
        );

        /* `box->field_0`/`box->field_4` are each read once, into r2,
         * and reused for both their own edge compare and the
         * opposite edge's sum (`ble`/`bge` short-circuiting straight
         * past the remaining checks on failure) - the register-pinned
         * locals force that single load/reuse. The sum itself
         * (`box->field_0 + box->field_8`, `box->field_4 +
         * box->field_c`) is anchored too: this compiler always
         * computes it in-place into whichever operand's register is
         * written first in the C expression, but the ROM keeps the
         * running edge value (r2) as the *first* source operand while
         * still landing the sum in the freshly-loaded field's own
         * register (r0) - a dest-vs-first-operand split no source
         * reordering here reproduced (the same "which anonymous
         * scratch register" gap as the AABB-build block above).
         * `success` is deliberately left as a plain (non-`register`)
         * local: pinning it to r7 (matching the ROM's own accumulator
         * choice) hits a confirmed toolchain bug where this compiler
         * never adds an inline-asm-clobbered r7 to the function's own
         * push/pop list (see docs/status/game_loop.md's `sub_8025A64`
         * entry for the same bug elsewhere) - left natural, the
         * ordinary if/else control flow below happens to allocate
         * `success` to r7 anyway, and the compiler *does* then track
         * it correctly for save/restore. */
        success = 0;
        {
            register s32 boxX asm("r2") = box->field_0;
            if (left > boxX) {
                register s32 boxRight asm("r0");
                asm volatile(
                    "ldr r0, [r6, #8]\n\t"
                    "add r0, r2, r0\n\t"
                    : "=r"(boxRight)
                    : "r"(box), "r"(boxX)
                    : "cc"
                );
                if (right < boxRight) {
                    register s32 boxY asm("r2") = box->field_4;
                    if (top > boxY) {
                        register s32 boxBottom asm("r0");
                        asm volatile(
                            "ldr r0, [r6, #0xc]\n\t"
                            "add r0, r2, r0\n\t"
                            : "=r"(boxBottom)
                            : "r"(box), "r"(boxY)
                            : "cc"
                        );
                        if (bottom < boxBottom) {
                            success = 1;
                        }
                    }
                }
            }
        }
        result = success;
    } else {
        result = skip;
    }

    return result;
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

extern void *gUnknown_030012D8;
extern void sub_8010B6C(void *arg);

/* Refreshes the viewport's own collision box (`sub_8010B6C` on
 * `gUnknown_030012D8+0x108`), then increments its `+0x92` counter by
 * one as long as it isn't already zero (a saturating-at-zero
 * "recently hit" style counter, never incremented back up from 0). */
void sub_80106DC(void)
{
    u8 *p = (u8 *)gUnknown_030012D8;
    u8 *p2;

    sub_8010B6C(p + 0x108);
    p2 = (u8 *)gUnknown_030012D8 + 0x92;
    if (*p2 != 0) {
        *p2 = *p2 + 1;
    }
}

/* Neighbor-list "get prev" accessor - reads `self+0x60`, the field
 * `sub_800FEB0` (game_loop17.c) zeroes on reset. */
void *sub_8010708(void *selfArg)
{
    u8 *self = selfArg;
    return *(void **)(self + 0x60);
}

/* Neighbor-list "get next" accessor - reads `self+0x5c`. */
void *sub_801070C(void *selfArg)
{
    u8 *self = selfArg;
    return *(void **)(self + 0x5c);
}

/* Neighbor-list "set prev" mutator - writes `self+0x60`. */
void sub_8010710(void *selfArg, void *val)
{
    u8 *self = selfArg;
    *(void **)(self + 0x60) = val;
}

/* Neighbor-list "set next" mutator - writes `self+0x5c`. */
void sub_8010714(void *selfArg, void *val)
{
    u8 *self = selfArg;
    *(void **)(self + 0x5c) = val;
}

/* UNUSED - no caller anywhere in the ROM (checked every asm/*.s,
 * expected/*.s and src/*.c file) - trivial constant accessor, always
 * returns 3. */
u32 sub_8010718(void)
{
    return 3;
}
