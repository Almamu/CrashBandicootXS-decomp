#include "core.h"

/* Fixed-point (8.8-ish) squared-distance-like helper: subtracts,
 * arithmetic-shifts right by 8 (divides by 256), squares, sums, then
 * shifts back left by 8. */
s32 sub_80008B4(s32 x1, s32 x2, s32 y1, s32 y2)
{
    s32 dx = (x1 - x2) >> 8;
    s32 dxSq = dx * dx;
    s32 dy = (y1 - y2) >> 8;
    s32 dySq = dy * dy;
    return (dxSq + dySq) << 8;
}
