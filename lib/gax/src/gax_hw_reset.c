#include "gax_internal.h"

/* Hardware sound-register reset for GAX2's Direct Sound A output: briefly
 * re-arms then disarms DMA1CNT_H with GAX2's settle delay between the two
 * writes (GAX_DMA_WAIT, gax_internal.h), resets DMA1's word
 * count/control, disables SOUNDCNT_X, sets SOUNDCNT_H for Direct Sound A
 * on timer0 (0x0B04), flushes FIFO_A (8 zero halfwords - same idiom as
 * GAX_resume in gax_dma_control.c), writes SOUNDBIAS_H, and points DMA1's
 * destination register at FIFO_A. */
void GaxResetSoundHardware(void)
{
    vu16 *fifo;
    u16 zero;
    s32 i;

    REG_DMA1CNT_H = 0x8640;
    GAX_DMA_WAIT();
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
    REG_DMA1DAD = REG_ADDR_FIFO_A;
}
