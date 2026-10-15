/* The pc platform's lib/gax/src/gax_dma_stop.c (platform/pc/platform.mk,
 * "Units"): GAX_stop and GaxStopDma as the original, without GAX2's DMA
 * settle delay (GAX_DMA_WAIT, ARM asm). */

#include "gax_host.h"

void GAX_stop(void)
{
    struct GaxInfoHandler *info = GAX_INFO();
    u32 zero = 0;

    info->playing = zero;
    gGaxPlayerState->state = zero;
    REG_SOUNDCNT_X = 0;
    REG_DMA1CNT_H = 0x8640;
    REG_DMA1CNT_H = 0xc8 << 3;
    REG_TM0CNT = 0;
}

void GaxStopDma(u32 dmaIdx)
{
    vu16 *reg = (vu16 *)(REG_ADDR_DMA1CNT_H + dmaIdx * 12);

    *reg = 0x8640;
    *reg = 0xc8 << 3;
}
