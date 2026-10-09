#include "gax_internal.h"

/* GAX2's per-frame DMA1/Timer0 direct-sound-output follow-up to
 * GAX2_init's play-start: once a song is loaded (magic == "GAX2") and
 * `state` is non-zero, a fresh `state == 1` (just-started) primes
 * SOUNDCNT_X, advances `state` to 2, and reloads Timer0 from
 * `timerReload`'s per-song sample-rate divisor; if the song header asks for
 * fatal errors to be shown (`showErrors`) and one hasn't already been
 * reported (`playDone`), shows GAX2's fatal-error screen (GaxFatalError);
 * finally, when `outHalf == 1`, re-arms DMA1 for Direct Sound A output
 * from `outBuf` (same DMA1CNT_H settle-delay quirk as GaxResetSoundHardware) and
 * clears the `playDone` flag. */
void GAX_irq(void)
{
    if (gGaxPlayerState->magic != 0x47415832) {
        return;
    }
    if (gGaxPlayerState->state == 0) {
        return;
    }
    /* `state` is volatile (gax_internal.h), so it's read again here, as in
     * the ROM. */
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
        /* Real hardware settle delay, not padding - see GaxResetSoundHardware's
         * doc comment in gax_hw_reset.c for why this can't be written
         * as plain "adds r3, r3, #0" text. */
        // clang-format off
        asm(".byte 0x1b, 0x1c\n\t"
            "mov r8, r8\n\t"
            "mov r8, r8\n\t"
            "mov r8, r8");
        // clang-format on
        REG_DMA1CNT_H = 0xc8 << 3;
        REG_DMA1SAD = (u32)p->outBuf;
        REG_DMA1CNT_H = 0xB660;
    }

    gGaxPlayerState->playDone = 0;
}
