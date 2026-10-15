extern "C" {
#include "core.h"
#include "engine/gfx/gfx.h"
#include "engine/util/util.h"
#include "engine/sys/system.h"
}

/* Sits right after src/engine/sys/key_input.cpp's matched functions and before
 * src/engine/util/fixed_math.cpp - the only function in this address range,
 * non-adjacent to bios_util.cpp's MemCopy32/UpdateCtrl since
 * main.cpp/memory.c/irq.c/key_input.cpp sit between them. Replaced the raw
 * asm/code_3_1.s (now deleted - this was its only function). */

/* Sets up BG2 for an affine full-screen image (mode 1, BG2 as an
 * affine background), computes a scale-only (no-rotation - `PB`/`PC`
 * stay 0) affine matrix from `FixedInverse16(0x100)` called twice, and
 * the matching `BG2X`/`BG2Y` reference point centering the image on
 * screen, then DMAs `palette` into palette RAM and loads `asset`'s
 * tile/tilemap data into VRAM via the already-matched
 * `LoadTaggedAsset`. Used for full-screen bitmap-style images (the
 * cutscene pictures, `graphics/cutscenes/`).
 *
 * The two `FixedInverse16` results each need a genuinely separate
 * unsigned-truncate (for the raw `BG2PA`/`BG2PD` halfword store) and
 * signed-truncate (for the offset arithmetic). The second result's
 * left-shift-by-16 (the first step of splitting it into its unsigned
 * and signed halves) has to be emitted as its own statement
 * (`shiftedD`) right after the second `FixedInverse16` call - before the
 * first result's sign-extension/offset math runs - to match the ROM's
 * instruction order; deriving `rawD`/`scaleD` straight from
 * `rawDFull` instead defers that shift until first use and puts it in
 * the wrong place. Likewise, `yLow`'s `0x4FB0` base has to be assigned
 * as its own statement right after `xLow` is fully computed, before
 * `rawD`/`scaleD` are split out of `shiftedD` - matching ROM's early
 * load of that constant - rather than folded into one final
 * `0x4FB0 - scaleD * 80` expression evaluated after the split.
 *
 * Separately, keeping `zero` (for the always-zero `BG2PB`/`BG2PC`
 * writes) as a plain local variable assigned right after the first
 * call - rather than two fresh `0` literals - is what keeps it alive
 * across the second `FixedInverse16` call, which in turn is what pushes
 * `asset`/`palette` into `r9`/`r8` instead of `r8`/`r6`.
 *
 * `BG2X_H`/`BG2Y_H` are computed inline in their own assignment
 * (`REG_BG2X_H = (xLow & 0x0FFF0000) >> 16;`) rather than through an
 * intermediate `xHigh`/`yHigh` variable: storing through a variable
 * first defers the pointer-advance between consecutive register
 * writes until after the mask/shift is computed, whereas ROM advances
 * the pointer immediately after each store. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void ShowBitmapScreen(void *asset, void *palette)
{
    u16 rawA;
    s32 zero;
    s32 shiftedD;
    /* `rawD` is needed: inlined, the code is 1 instruction longer */
    u16 rawD;
    s32 xLow, yLow;

    REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(3) | BGCNT_256COLOR | BGCNT_SCREENBASE(8);
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;

    rawA = FixedInverse16(0x100);
    zero = 0;
    shiftedD = FixedInverse16(0x100) << 16;
    /* The picture's centre (0x7788 = 120 * 0xFF in Q8, the ROM's
     * constant) less the scaled screen centre. */
    xLow = DISPLAY_ART_CENTER_X * 0xFF - (s16)rawA * DISPLAY_CENTER_X;
    yLow = DISPLAY_ART_CENTER_Y * 0xFF;
    rawD = (u16)((u32)shiftedD >> 16);
    yLow -= (s16)(shiftedD >> 16) * DISPLAY_CENTER_Y;

    REG_BG2PA = rawA;
    REG_BG2PB = zero;
    REG_BG2PC = zero;
    REG_BG2PD = rawD;
    REG_BG2X_L = xLow;
    REG_BG2X_H = (xLow & 0x0FFF0000) >> 16;
    REG_BG2Y_L = yLow;
    REG_BG2Y_H = (yLow & 0x0FFF0000) >> 16;

    DmaCopy16(3, palette, PLTT, BG_PLTT_SIZE);

    LoadTaggedAsset(asset, (void *)VRAM);
}
