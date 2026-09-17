#include "core.h"

/* Sits right after src/system/irq.c's matched functions and before
 * src/util/math_util.c - the only function in this address range,
 * non-adjacent to boot_util.c's sub_800014C/nullsub_9 since
 * main.c/memory.c/irq.c sit between them. Replaced the raw
 * asm/code_3_1.s (now deleted - this was its only function). */

extern s32 sub_800090C(s32 arg0);
extern void LoadTaggedAsset(void *asset, void *dest);

/* Sets up BG2 for an affine full-screen image (mode 1, BG2 as an
 * affine background), computes a scale-only (no-rotation - `PB`/`PC`
 * stay 0) affine matrix from `sub_800090C(0x100)` called twice, and
 * the matching `BG2X`/`BG2Y` reference point centering the image on
 * screen, then DMAs `palette` into palette RAM and loads `asset`'s
 * tile/tilemap data into VRAM via the already-matched
 * `LoadTaggedAsset`. Used for full-screen bitmap-style images (the
 * intro sequence's individual frames, per the `graphics/intro/`
 * assets this ties into).
 *
 * The two `sub_800090C` results each need a genuinely separate
 * unsigned-truncate (for the raw `BG2PA`/`BG2PD` halfword store) and
 * signed-truncate (for the offset arithmetic). The second result's
 * left-shift-by-16 (the first step of splitting it into its unsigned
 * and signed halves) has to be emitted as its own statement
 * (`shiftedD`) right after the second `sub_800090C` call - before the
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
 * across the second `sub_800090C` call, which in turn is what pushes
 * `asset`/`palette` into `r9`/`r8` instead of `r8`/`r6`.
 *
 * `BG2X_H`/`BG2Y_H` are computed inline in their own assignment
 * (`REG_BG2X_H = (xLow & 0x0FFF0000) >> 16;`) rather than through an
 * intermediate `xHigh`/`yHigh` variable: storing through a variable
 * first defers the pointer-advance between consecutive register
 * writes until after the mask/shift is computed, whereas ROM advances
 * the pointer immediately after each store. */
void sub_80007EC(void *asset, void *palette)
{
    u16 rawA;
    s32 zero;
    s32 rawDFull;
    s32 shiftedD;
    s16 scaleA;
    u16 rawD;
    s16 scaleD;
    s32 xLow, yLow;
    u32 val;
    struct dma_regs *dma;

    REG_BG2CNT = 0x088F;
    REG_DISPCNT = 0x1F44;

    rawA = sub_800090C(0x100);
    zero = 0;
    rawDFull = sub_800090C(0x100);
    shiftedD = rawDFull << 16;
    scaleA = (s16)rawA;
    xLow = 0x7788 - scaleA * 120;
    yLow = 0x4FB0;
    rawD = (u16)((u32)shiftedD >> 16);
    scaleD = (s16)(shiftedD >> 16);
    yLow -= scaleD * 80;

    REG_BG2PA = rawA;
    REG_BG2PB = zero;
    REG_BG2PC = zero;
    REG_BG2PD = rawD;
    REG_BG2X_L = xLow;
    REG_BG2X_H = (xLow & 0x0FFF0000) >> 16;
    REG_BG2Y_L = yLow;
    REG_BG2Y_H = (yLow & 0x0FFF0000) >> 16;

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)palette;
    dma->dst = 0x05000000;
    dma->cnt = 0x80000100;
    val = dma->cnt;

    LoadTaggedAsset(asset, (void *)0x06000000);
}
