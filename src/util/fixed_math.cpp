extern "C" {
#include "core.h"
#include <agb_syscall.h>
#include <libgcc.h>
#include "util.h"
#include "math_util.h"
}

/* Fixed-point helpers on the game's 24.8 coordinates (the s16 ones on
 * 8.8 values).
 *
 * Squared distance between (x1, y1) and (x2, y2): drops the fraction of
 * each delta, squares, sums, and shifts the result back to 24.8. */
s32 FixedDistSq(s32 x1, s32 x2, s32 y1, s32 y2)
{
    s32 dx = Q8_TO_INT(x1 - x2);
    s32 dxSq = dx * dx;
    s32 dy = Q8_TO_INT(y1 - y2);
    s32 dySq = dy * dy;
    return INT_TO_Q8(dxSq + dySq);
}


/* Distance between (x1, y1) and (x2, y2): FixedDistSq's sum of squares
 * through the BIOS Sqrt, shifted back to 24.8. */
u32 FixedDist(s32 x1, s32 x2, s32 y1, s32 y2)
{
    s32 dx = Q8_TO_INT(x1 - x2);
    s32 dxSq = dx * dx;
    s32 dy = Q8_TO_INT(y1 - y2);
    s32 dySq = dy * dy;
    return INT_TO_Q8((u16)Sqrt(dxSq + dySq));
}

/* arg0 / arg1 in 24.8. */
s32 FixedDiv(s32 arg0, s32 arg1)
{
    return __divsi3(arg0 << 8, arg1);
}

/* a * b in 24.8: drops the fraction of the larger operand before
 * multiplying (instead of shifting the product), which keeps the
 * product from overflowing. */
s32 FixedMul(s32 a, s32 b)
{
    if (a > b) {
        a = Q8_TO_INT(a);
    } else {
        b = Q8_TO_INT(b);
    }
    return a * b;
}

/* The 8.8 (s16) versions: 1 / arg0, arg0 / arg1 and arg0 * arg1, each
 * result truncated to s16. */
s32 FixedInverse16(s32 arg0)
{
    return (s16)__divsi3(0x10000, (s16)arg0);
}

s32 FixedDiv16(s32 arg0, s32 arg1)
{
    return (s16)__divsi3((s16)arg0 << 8, (s16)arg1);
}

s32 FixedMul16(s32 arg0, s32 arg1)
{
    s32 a = (s16)arg0;
    s32 b = (s16)arg1;
    a = a * b;
    a = (a << 8) >> 16;
    return a;
}
