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

/* numerator / denominator in 24.8. */
s32 FixedDiv(s32 numerator, s32 denominator)
{
    return __divsi3(numerator << 8, denominator);
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

/* The 8.8 (s16) versions: 1 / denominator, numerator / denominator and
 * lhs * rhs, each result truncated to s16. */
s32 FixedInverse16(s32 denominator)
{
    return (s16)__divsi3(0x10000, (s16)denominator);
}

s32 FixedDiv16(s32 numerator, s32 denominator)
{
    return (s16)__divsi3((s16)numerator << 8, (s16)denominator);
}

s32 FixedMul16(s32 lhs, s32 rhs)
{
    s32 a = (s16)lhs;
    s32 b = (s16)rhs;
    a = a * b;
    a = (a << 8) >> 16;
    return a;
}
