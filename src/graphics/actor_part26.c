#include "core.h"

/* Same boss-weapon subsystem as actor_part20.c - see that file's header
 * comment and docs/matching/issue-58-0x08030334-actor.md. */

extern s32 gAirshipStateTimer;

/* Sets BG palette bank 1's last color (index 15) to either a near-white
 * flash color or a dim default, gated by bit 3 of `gAirshipStateTimer`
 * (a flags word driving this effect's per-frame look). */
void UpdateAirshipFlashColor(void)
{
    vu16 *bank1 = (vu16 *)(BG_PLTT + 0x20);

    if (gAirshipStateTimer & 8) {
        bank1[0xf] = 0x7fff;
    } else {
        bank1[0xf] = 0x1f;
    }
}

extern s32 __divsi3(s32 arg0, s32 arg1);
extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);
extern s32 gAirshipHitFlashTimer;
extern u8 gAirshipHitFlashPalettes[];

/* While `gAirshipHitFlashTimer`'s DMA-refresh counter is armed, decrements
 * it and re-queues one "frame" of `gAirshipHitFlashPalettes`'s palette
 * animation strip into BG palette bank 1 - `__divsi3` picks a
 * triangle-wave frame index (0-2, mirrored back down for 3-4) so the
 * animation ping-pongs. */
void AnimateAirshipPalette(void)
{
    s32 v;

    if (gAirshipHitFlashTimer == 0) {
        return;
    }
    gAirshipHitFlashTimer--;

    v = __divsi3(gAirshipHitFlashTimer, 3);
    if (v > 2) {
        v = 5 - v;
    }

    QueueVramDmaTransfer(gAirshipHitFlashPalettes + (v << 5), (void *)(BG_PLTT + 0x20), 0x20, 0x10);
}
