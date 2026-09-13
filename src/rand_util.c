#include "core.h"

/* Sits right after sub_8000DF8 (ROM 0x08000DF8, in src/string_util2.c)
 * and before sub_8000E6C (still raw in asm/code_3_1_3.s); a standard C
 * library LCG (multiplier 0x41C64E6D, increment 0x3039 aka 12345) fed
 * from a global seed in IWRAM. */

extern u32 gUnknown_030007E4;
extern u16 sub_803AF1C(u16 rnd, s32 max);

/* Seeds the RNG. */
void sub_8000E10(u32 seed)
{
    gUnknown_030007E4 = seed;
}

/* Advances the RNG and returns a value in [0, max) via sub_803AF1C. */
u16 sub_8000E1C(s32 max)
{
    gUnknown_030007E4 = gUnknown_030007E4 * 0x41C64E6D + 0x3039;
    return sub_803AF1C((u16)(gUnknown_030007E4 >> 4), max);
}

/* Advances the RNG and returns the next raw 16-bit value. */
u16 sub_8000E4C(void)
{
    gUnknown_030007E4 = gUnknown_030007E4 * 0x41C64E6D + 0x3039;
    return (u16)(gUnknown_030007E4 >> 4);
}
