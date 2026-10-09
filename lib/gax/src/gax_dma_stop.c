#include "gax_internal.h"

/* Stops GAX2's Direct Sound A/Timer0 output - the exact counterpart to
 * GAX_irq's play-start follow-up in gax_playback_ticker.c: clears the
 * current player's Info handler's `playing` flag, resets `state` to 0,
 * disables SOUNDCNT_X, then re-arms and disarms DMA1CNT_H back to its
 * "off" value (`0x0640`, no bit 15) around GAX2's settle delay
 * (GAX_DMA_WAIT, gax_internal.h), and finally stops Timer0
 * (`REG_TM0CNT = 0`, a single 32-bit store covering both TM0CNT_L/H). */
void GAX_stop(void)
{
    struct GaxInfoHandler *info = GAX_INFO();
    u32 zero = 0;

    info->playing = zero;
    gGaxPlayerState->state = zero;
    REG_SOUNDCNT_X = 0;
    REG_DMA1CNT_H = 0x8640;
    GAX_DMA_WAIT();
    REG_DMA1CNT_H = 0xc8 << 3;
    REG_TM0CNT = 0;
}

/* A generic single-DMA-channel "off" helper: `dmaIdx` selects DMA1/2/3
 * (each block's registers are a fixed 0xc bytes apart, so
 * `REG_ADDR_DMA1CNT_H + dmaIdx*12` reaches DMA(dmaIdx+1)CNT_H), and this
 * disables it via the same arm-then-disarm settle delay GAX_stop above
 * and GaxResetSoundHardware use. Not actually called from GAX_stop itself
 * (which duplicates the DMA1-specific instructions inline instead) - this
 * project hasn't found a caller for it yet in the functions matched so
 * far. */
void GaxStopDma(u32 dmaIdx)
{
    vu16 *reg = (vu16 *)(REG_ADDR_DMA1CNT_H + dmaIdx * 12);

    *reg = 0x8640;
    GAX_DMA_WAIT();
    *reg = 0xc8 << 3;
}
