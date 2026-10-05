#include "core.h"

/* Sits right after actor_part60.c's `CreateYeti` and before
 * actor_part61.c's `YetiStateCaught` - the whole contiguous range that used
 * to be `asm/code_3_2_20_28568_c99c_e058.s`. */

/* A parameterized twin of `LoadYetiGraphics`'s (actor_part75.c) 16x16
 * triangular-fill dot-pattern loop, taking the destination buffer
 * (`dst`) and seed byte (`seed`) as real parameters instead of the
 * fixed stack buffer/`0`-or-`0x80` seed constants `LoadYetiGraphics` uses for
 * its own two inline copies of this same loop. No known caller anywhere
 * in the matched portion of this ROM region (`LoadYetiGraphics` always
 * inlines the loop itself rather than calling this) - kept byte-exact
 * regardless, per this project's standing convention for functions
 * without a confirmed call site. UNUSED.
 *
 * The condition is written as the "fill with 0xff" test so the 0xff
 * store comes first, as in the ROM. */
void sub_802E058(u8 *dst, u8 seed)
{
    s32 y, x;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if ((u32)(x - 3) > 9 || y <= 2 || y > 12)
                dst[y * 16 + x] = 0xff;
            else
                dst[y * 16 + x] = seed++;
        }
    }
}

asm(".align 2, 0");
