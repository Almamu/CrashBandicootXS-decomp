extern "C" {
#include "core.h"
#include <libgcc.h>
#include "util.h"
}

/* Sits right after strlen (ROM 0x08000DF8, in src/util/string.cpp)
 * and before InitBresenhamLine (still raw in asm/code_3_1_3.s); a standard C
 * library LCG (multiplier 0x41C64E6D, increment 0x3039 aka 12345) fed
 * from a global seed in IWRAM. */

/* Seeds the RNG. */
void srand(u32 seed)
{
    gRandSeed = seed;
}

/* Advances the RNG and returns a value in [0, max) via __umodsi3. */
u16 RandRange(s32 max)
{
    gRandSeed = gRandSeed * 0x41C64E6D + 0x3039;
    return __umodsi3((u16)(gRandSeed >> 4), max);
}

/* Advances the RNG and returns the next raw 16-bit value. */
u16 rand(void)
{
    gRandSeed = gRandSeed * 0x41C64E6D + 0x3039;
    return (u16)(gRandSeed >> 4);
}
