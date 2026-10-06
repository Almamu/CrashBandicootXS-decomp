#include "gax_internal.h"
#include "match.h"

/* Stops GAX2's Direct Sound A/Timer0 output - the exact counterpart to
 * GAX_irq's play-start follow-up in gax_playback_ticker.c: clears the
 * current channel's `chan->4`-embedded flag byte at `+0x1a` (an "active"/
 * "note on" flag, same object as gax_channel_note_cut.c's `self`), resets
 * `state` to 0, disables SOUNDCNT_X, then re-arms and disarms DMA1CNT_H
 * back to its "off" value (`0x0640`, no bit 15) using the same real-
 * hardware settle-delay quirk as GaxResetSoundHardware (gax_hw_reset.c) - the
 * settle-delay `.byte` sequence can't be written as plain
 * "adds r3, r3, #0" text, see that function's doc comment for why - and
 * finally stops Timer0 (`REG_TM0CNT = 0`, a single 32-bit store covering
 * both TM0CNT_L/H). */
void GAX_stop(void)
{
    void *chan = gGaxPlayerState->channels[gGaxPlayerState->curChannelIdx];
    MATCH_HOLD_REG(u8 *, inner, r0) = *(u8 **)((u8 *)chan + 4);
    u32 zero = 0;

    inner[0x1a] = zero;
    gGaxPlayerState->state = zero;
    REG_SOUNDCNT_X = 0;
    REG_DMA1CNT_H = 0x8640;
    /* Real hardware settle delay, not padding - see GaxResetSoundHardware's doc
     * comment in gax_hw_reset.c for why this can't be written as plain
     * "adds r3, r3, #0" text. */
    asm(".byte 0x1b, 0x1c\n\t"
        "mov r8, r8\n\t"
        "mov r8, r8\n\t"
        "mov r8, r8");
    REG_DMA1CNT_H = 0xc8 << 3;
    REG_TM0CNT = 0;
}

/* A generic single-DMA-channel "off" helper: `dmaIdx` selects DMA1/2/3
 * (each block's registers are a fixed 0xc bytes apart, so
 * `REG_ADDR_DMA1CNT_H + dmaIdx*12` reaches DMA(dmaIdx+1)CNT_H), and this
 * disables it via the same arm-then-disarm settle-delay quirk GAX_stop
 * above and GaxResetSoundHardware use. Not actually called from GAX_stop itself
 * (which duplicates the DMA1-specific instructions inline instead) - this
 * project hasn't found a caller for it yet in the functions matched so
 * far. */
void GaxStopDma(u32 dmaIdx)
{
    vu16 *reg = (vu16 *)(REG_ADDR_DMA1CNT_H + dmaIdx * 12);

    *reg = 0x8640;
    asm(".byte 0x1b, 0x1c\n\t"
        "mov r8, r8\n\t"
        "mov r8, r8\n\t"
        "mov r8, r8");
    *reg = 0xc8 << 3;
}
