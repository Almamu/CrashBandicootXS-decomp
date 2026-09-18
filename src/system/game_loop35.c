#include "core.h"

/* GitHub issue #13: 0x0800FC70-0x08010A0C, continuing the physics/
 * collision subsystem (see game_loop17.c's header comment and
 * docs/matching/issue-13-graphics-fc70.md). This function sits
 * between the still-raw `sub_800FF0C` (real bytes in
 * asm/code_3_2_17_e560_ff0c.s) and `sub_80104E4` (real bytes in the
 * new asm/code_3_2_17_e560_104e4.s), so it needs its own file rather
 * than joining an existing one - see docs/workflow.md's "one file per
 * contiguous ROM region" rule. `self` throughout is the same
 * "collision box" object every other function in this subsystem
 * operates on - offsets kept raw rather than a named struct, matching
 * every already-matched sibling in this file family
 * (game_loop22.c-game_loop34.c). See
 * docs/matching/issue-13-fc70-second-continuation.md for the
 * register-pinning technique this needed. */

extern void *gUnknown_030012CC;
extern void sub_8007A84(void *self, void *part);

/* Unless `self`'s own `+0x4d` state byte has bit 7 set or its low 7
 * bits are already nonzero, resets `self+0x38` to 0 and clamps
 * `self+0x30`'s index to the `self+0x20`-pointer-to-manager/
 * `self+0x2d`-tag/0x1c-stride hitbox-record's own `+0x16` count
 * (the same table-lookup convention `sub_800D040`, game_loop6.c,
 * establishes). Always tail-fires `sub_8007A84(gUnknown_030012CC,
 * self)`, then - only if `self+0x38` ended up nonzero - clears
 * `self+0xc` bit 3. */
void sub_8010480(void *selfArg)
{
    /* Pinned to r4: the ROM keeps `self` in r4 for the whole function
     * (matching every sibling in this file family). */
    register u8 *self asm("r4") = selfArg;
    u8 state = self[0x4d];

    if ((state & 0x80) == 0) {
        u8 masked7f = state & 0x7f;

        if (masked7f == 0) {
            /* Reuses the already-zero `masked7f` register for this
             * store rather than a fresh `0` immediate, matching the
             * ROM's own register reuse (`strb r1,[r0]` right after
             * computing `r1 = state & 0x7f`). */
            self[0x38] = masked7f;

            {
                register s32 idx asm("r3") = 0;
                {
                    register void *p asm("r0") = *(void **)(self + 0x20);
                    register u8 *tagAddr asm("r2") = self + 0x2d;
                    {
                        register void *table asm("r1") = *(void **)p;
                        register u8 tag asm("r5") = *tagAddr;
                        /* Register-pinned r0: the ROM computes this
                         * address as `offset(r0) + table(r1)`, not
                         * `table + offset` - writing the addition with
                         * the offset as the left operand is what makes
                         * this compiler pick the same destination
                         * register (r0, the offset's own register)
                         * instead of reusing `table`'s (r1). */
                        register u8 *record asm("r0") =
                            (u8 *)(tag * 0x1c + (s32)table);
                        u8 limit = record[0x16];

                        if (idx >= limit) {
                            idx = limit - 1;
                        }
                    }
                }
                *(s32 *)(self + 0x30) = idx;
            }
        }
    }

    sub_8007A84(gUnknown_030012CC, self);

    if (self[0x38] != 0) {
        /* Anchored: the ROM computes the `~8` clear-mask at runtime
         * (`movs r0,#9; rsbs r0,r0,#0`, the negative-constant
         * register-pinned mask idiom - see
         * matching_decomp_register_pinning and sub_80109A4's own use
         * of it, game_loop30.c) rather than folding it into an 8-bit
         * AND immediate, which a plain `self[0xc] &= -9;` always did
         * instead. `self` is passed as an input purely so this
         * compiler knows the asm still depends on it - without that,
         * it reused r4 in place for the `self[0x38] != 0` check just
         * above, corrupting the address this block reads/writes. */
        asm volatile(
            "mov r0, #0x9\n\t"
            "neg r0, r0\n\t"
            "ldrb r1, [r4, #0xc]\n\t"
            "and r0, r0, r1\n\t"
            "strb r0, [r4, #0xc]\n\t"
            :
            : "r"(self)
            : "r0", "r1", "cc", "memory"
        );
    }
}
