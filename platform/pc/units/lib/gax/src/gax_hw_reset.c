/* The pc platform's lib/gax/src/gax_hw_reset.c (platform/pc/platform.mk,
 * "Units"): GaxResetSoundHardware as the original, without GAX2's DMA
 * settle delay (GAX_DMA_WAIT, ARM asm). The registers it sets are host
 * memory, which nothing reads: the platform's audio output takes the
 * mixed blocks from GaxMixFrame (audio.c), not from a DMA to FIFO_A. */

#include "gax_host.h"

void GaxResetSoundHardware(void)
{
    vu16 *fifo;
    u16 zero;
    s32 i;

    REG_DMA1CNT_H = 0x8640;
    REG_DMA1CNT_H = 0xc8 << 3;
    REG_DMA1CNT = 4;
    REG_SOUNDCNT_X = 0;
    REG_SOUNDCNT_H = 0x0B04;
    fifo = (vu16 *)REG_ADDR_FIFO_A;
    zero = 0;
    for (i = 7; i >= 0; i--) {
        *fifo = zero;
    }
    REG_SOUNDBIAS_H = 0x42;
    REG_DMA1DAD = (u32)REG_ADDR_FIFO_A;
}
