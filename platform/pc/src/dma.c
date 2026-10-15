/* DmaSet (gba/dma_macros.h): the channel's registers in gHostIo, and an
 * immediate transfer done at once. A transfer timed to VBlank, HBlank or
 * a sound FIFO is only left in the registers, for the platform's frame
 * loop to do (phase 2, #1093). */

#include <string.h>

#include "gba/gba.h"

void HostDmaSet(int channel, uintptr_t src, uintptr_t dest, uint32_t control)
{
    vu32 *regs = (vu32 *)(REG_BASE + REG_OFFSET_DMA0SAD + 12 * channel);
    u32 cnt = control >> 16;
    u32 count = control & 0xFFFF;
    u32 unit = (cnt & DMA_32BIT) ? sizeof(u32) : sizeof(u16);
    s32 srcStep;
    s32 destStep;
    u32 i;

    /* The GBA's 32-bit address registers get the address's low bits. */
    regs[0] = (u32)src;
    regs[1] = (u32)dest;
    regs[2] = control;

    if (!(cnt & DMA_ENABLE) || (cnt & DMA_START_MASK) != DMA_START_NOW) {
        return;
    }
    if (count == 0) {
        count = channel == 3 ? 0x10000 : 0x4000;
    }

    switch (cnt & (DMA_SRC_DEC | DMA_SRC_FIXED)) {
    case DMA_SRC_DEC:
        srcStep = -(s32)unit;
        break;
    case DMA_SRC_FIXED:
        srcStep = 0;
        break;
    default:
        srcStep = unit;
        break;
    }
    switch (cnt & DMA_DEST_RELOAD) {
    case DMA_DEST_DEC:
        destStep = -(s32)unit;
        break;
    case DMA_DEST_FIXED:
        destStep = 0;
        break;
    default: /* DMA_DEST_INC, DMA_DEST_RELOAD */
        destStep = unit;
        break;
    }

    for (i = 0; i < count; i++) {
        memcpy((void *)dest, (const void *)src, unit);
        src += srcStep;
        dest += destStep;
    }

    /* Done: the enable bit clears (DmaWait) unless the DMA repeats. */
    if (!(cnt & DMA_REPEAT)) {
        regs[2] = control & ~((u32)DMA_ENABLE << 16);
    }
}
