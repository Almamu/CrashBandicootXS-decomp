/* The pc platform's lib/gax/src/gax_playback_ticker.c (platform/pc/
 * platform.mk, "Units"): GAX_irq as the original, without GAX2's DMA
 * settle delay (GAX_DMA_WAIT, ARM asm). The DMA register holds only 32
 * bits of the output buffer's address, and nothing reads it: the
 * platform's audio output takes the blocks from GaxMixFrame (audio.c). */

#include "gax_host.h"

void GAX_irq(void)
{
    if (gGaxPlayerState->magic != 0x47415832) {
        return;
    }
    if (gGaxPlayerState->state == 0) {
        return;
    }
    if (gGaxPlayerState->state == 1) {
        struct GaxPlayerState *p = gGaxPlayerState;

        REG_SOUNDCNT_X = 0x80;
        p->state = 2;
        REG_TM0CNT_H = 0;
        REG_TM0CNT = (0x10000 - p->timerReload) | 0x00C00000;
    }

    {
        struct GaxSongHeader *songData = gGaxPlayerState->songPtr;

        if (songData->showErrors != 0 && gGaxPlayerState->playDone == 0) {
            GaxFatalError(gGaxErrNameIrq, gGaxErrPlayNotFinished);
        }
    }

    if (gGaxPlayerState->outHalf == 1) {
        struct GaxPlayerState *p = gGaxPlayerState;

        REG_DMA1CNT_H = 0x8640;
        REG_DMA1CNT_H = 0xc8 << 3;
        REG_DMA1SAD = (u32)(uintptr_t)p->outBuf;
        REG_DMA1CNT_H = 0xB660;
    }

    gGaxPlayerState->playDone = 0;
}
