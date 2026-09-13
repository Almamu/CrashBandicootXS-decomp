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

extern s32 sub_803A95C(s32 arg0);

/* Same shape as sub_80008B4, but feeds the squared distance through an
 * (presumably integer square root) helper and rescales the low 16 bits
 * of the result back up by 8 - looks like an actual (non-squared)
 * distance calculation built on top of sub_80008B4's math. */
u32 sub_80008CC(s32 x1, s32 x2, s32 y1, s32 y2)
{
    s32 dx = (x1 - x2) >> 8;
    s32 dxSq = dx * dx;
    s32 dy = (y1 - y2) >> 8;
    s32 dySq = dy * dy;
    return (u16)sub_803A95C(dxSq + dySq) << 8;
}

extern s32 sub_803ADB4(s32 arg0, s32 arg1);

s32 sub_80008F0(s32 arg0, s32 arg1)
{
    return sub_803ADB4(arg0 << 8, arg1);
}

/* Halves whichever of the two fixed-point values is larger before
 * multiplying them - looks like an overflow-avoidance trick for a
 * scale/interpolation calculation. */
s32 sub_80008FC(s32 a, s32 b)
{
    if (a > b) {
        a >>= 8;
    } else {
        b >>= 8;
    }
    return a * b;
}

/* sub_803ADB4 looks like an atan2-style angle lookup (see its use in
 * sub_800697C as `sub_803ADB4(total * 100, 0x48)` in src/graphics.c) -
 * these three wrappers pass fixed/sign-extended 16-bit operands through
 * to it and sign-extend the 16-bit result back. */
s32 sub_800090C(s32 arg0)
{
    return (s16)sub_803ADB4(0x10000, (s16)arg0);
}

s32 sub_8000924(s32 arg0, s32 arg1)
{
    return (s16)sub_803ADB4((s16)arg0 << 8, (s16)arg1);
}

s32 sub_800093C(s32 arg0, s32 arg1)
{
    s32 a = (s16)arg0;
    s32 b = (s16)arg1;
    a = a * b;
    a = (a << 8) >> 16;
    return a;
}
