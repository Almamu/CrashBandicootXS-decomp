#include "core.h"

/* Sits right after the still-raw remainder of asm/code_3_1_6.s'
 * SIO/link-cable and overlay_ui functions and before the small
 * fade/screen-mode utility cluster this file documents - see
 * docs/rom_map.md "A fourth thing in this file". This cluster's
 * parked functions interleave with the matched ones - see
 * docs/matching.md for the full split. */

#if NON_MATCHING
extern void sub_80013FC(s32 factor);
extern void sub_80006A8(void *arg0);
extern u16 gUnknown_03000A80[512];
extern u16 gUnknown_03000E80[512];

/* Backs the real palette (0x05000000) up into `gUnknown_03000A80`,
 * then, for each factor 0/2/4/.../16, blends it toward black via the
 * already-matched `sub_80013FC` into `gUnknown_03000E80` and DMAs
 * that result into the real palette, waiting one VBlank between each
 * step - a textbook fade-to-black animation. Once fully faded, sets
 * up the hardware blend registers (`REG_BLDCNT`/`REG_BLDY`) and
 * restores the original backed-up palette.
 *
 * Parked: the ROM caches the blended-buffer address (`gUnknown_
 * 03000E80`) in a register across the loop while recomputing the
 * other two DMA fields (the 0x05000000 destination and the 0x80000200
 * control word) fresh every iteration, plus an extra "rename" copy of
 * the DMA register pointer right before the loop. This compiler's
 * loop-invariant hoisting pass won't reproduce that specific split:
 * introducing a local variable for the buffer address to get it
 * cached consistently hoists at least one of the other two fields
 * as well (or, with an inline-asm register-clobber barrier on the
 * destination value to block just that hoist, drops the buffer
 * caching instead) - every combination tried moves the "which fields
 * get cached" line, but never lands on the ROM's exact split. Same
 * root cause as the `sub_8009150` loop-invariant-hoisting gap
 * documented in docs/matching.md, applied to a different pattern. */
void sub_80014A4(void)
{
    struct dma_regs *dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    s32 factor;
    u32 val;

    dma->src = 0x05000000;
    dma->dst = (u32)gUnknown_03000A80;
    dma->cnt = 0x80000200;
    val = dma->cnt;

    for (factor = 0; factor <= 0x10; factor += 2) {
        sub_80013FC(factor);
        sub_80006A8(0);
        dma->src = (u32)gUnknown_03000E80;
        dma->dst = 0x05000000;
        dma->cnt = 0x80000200;
        val = dma->cnt;
    }

    REG_BLDCNT = 0xff;
    REG_BLDY = 0x10;

    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)gUnknown_03000A80;
    dma->dst = 0x05000000;
    dma->cnt = 0x80000200;
    val = dma->cnt;
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

/* `gUnknown_030007E8.field_0 != -1` - the same "idle" sentinel
 * documented on the struct in fade_util.c, exposed here as a plain
 * s32 read (this file doesn't share that struct definition, per this
 * project's per-file raw-offset convention). */
extern s32 gUnknown_030007E8;

s32 sub_8001510(void)
{
    return gUnknown_030007E8 != -1;
}
