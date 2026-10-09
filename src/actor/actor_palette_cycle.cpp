#include "actor_self.hpp"
#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include <libgcc.h>
#include "actor.h"
#include "vehicle.h"
#include "gfx.h"
#include "globals.h"
}

/* The actor zone's BG palette cycle (gActorPaletteCycle*): restore,
 * save, the per-frame DMA step, the per-category seed and the switch,
 * ROM 0x0802AB08-0x0802AC28.
 * Built with old_agbcp and -fno-implement-inlines, like actor.cpp, where
 * it was until #770. See docs/matching/archive/issue-50-actor-2a69c.md. */

/* Loads the palette cycle's cursor and bound (gActorPaletteCycleFrame/
 * gActorPaletteCycleTarget, see UpdateActorPaletteCycle) from their saved
 * copies and restarts the DMA timer. */
void RestoreActorPaletteCycle(void)
{
    gActorPaletteCycleFrame = gSavedActorPaletteCycleFrame;
    gActorPaletteCycleTarget = gSavedActorPaletteCycleTarget;
    gActorPaletteCycleTimer = 0;
}

/* The inverse of RestoreActorPaletteCycle. */
void SaveActorPaletteCycle(void)
{
    gSavedActorPaletteCycleFrame = gActorPaletteCycleFrame;
    gSavedActorPaletteCycleTarget = gActorPaletteCycleTarget;
}

/* While gActorPaletteCycleEnabled: DMAs one 0x1c0-byte frame of
 * gActorPaletteCycleFrames (the cursor's) to BG palette RAM, and every
 * 0x24 calls moves the cursor one step toward the bound (holding it
 * there); SaveActorPaletteCycle/SetActorPaletteCycle swap the ends for a
 * ping-pong. */
void UpdateActorPaletteCycle(void)
{
    if (!gActorPaletteCycleEnabled)
        return;
    QueueVramDmaTransfer((void *)gActorPaletteCycleFrames[gActorPaletteCycleFrame], (void *)BG_PLTT,
                         0x1c0, 0x10);
    if (++gActorPaletteCycleTimer > 0x23) {
        gActorPaletteCycleTimer = 0;
        {
            s32 *cur = &gActorPaletteCycleFrame;
            s32 target = gActorPaletteCycleTarget;
            s32 v = *cur;
            s32 r;

            if (target - v >= 0) {
                r = v;
                if (target != r)
                    r++;
            } else {
                r = v - 1;
            }
            *cur = r;
        }
    }
}

/* Seeds the palette cycle's cursor and bound from the per-category tables
 * (gActorPaletteCycleStartFrames/gActorPaletteCycleTargetFrames) and
 * restarts the DMA timer. */
void SetActorPaletteCycle(s32 idx)
{
    gActorPaletteCycleFrame = gActorPaletteCycleStartFrames[idx];
    gActorPaletteCycleTarget = gActorPaletteCycleTargetFrames[idx];
    gActorPaletteCycleTimer = 0;
}

/* Turns the palette cycle on or off, resetting the cursor, the bound and
 * the timer, and saves that state (SaveActorPaletteCycle). */
void EnableActorPaletteCycle(u8 flag)
{
    gActorPaletteCycleEnabled = flag;
    gActorPaletteCycleFrame = 0;
    gActorPaletteCycleTarget = 0;
    gActorPaletteCycleTimer = 0;
    SaveActorPaletteCycle();
}
