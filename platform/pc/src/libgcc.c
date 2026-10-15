/* The 32-bit division helpers of libgcc's ARM lib1funcs (lib/libgcc,
 * libgcc.h), which the game calls by name; a host's libgcc has none. */

#include <libgcc.h>

u32 __udivsi3(u32 dividend, u32 divisor)
{
    return dividend / divisor;
}

s32 __divsi3(s32 dividend, s32 divisor)
{
    return dividend / divisor;
}

s32 __modsi3(s32 dividend, s32 divisor)
{
    return dividend % divisor;
}

u32 __umodsi3(u32 dividend, u32 divisor)
{
    return dividend % divisor;
}

/* And the 64-bit one GAX2 calls by name (GaxChannelInit's mix-rate
 * reciprocal); a 64-bit host's libgcc has it as native division. */
u64 __udivdi3(u64 n, u64 d)
{
    return n / d;
}
