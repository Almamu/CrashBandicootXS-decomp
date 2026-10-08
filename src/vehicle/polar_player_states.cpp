#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "audio.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* PolarPlayer's wumpa dispenser, its pause lock and five of its states
 * (#664 part 11c, include/vehicle.hpp), ROM 0x0802BC68-0x0802BED8, between
 * polar_player.cpp and polar_player_actions.cpp. DispenseWumpa is the
 * twin of JetpackPlayer::DispenseWumpa (jetpack_player.cpp) on the polar
 * globals; the three end-of-run states are the course's three exits
 * (SetActorCategoryExitStatus). */

/* Drains the `gPolarQueuedWumpa` reward accumulator QueueWumpa fills:
 * all at once (repeated `CollectWumpa` calls) while
 * `gPolarPlayerInactive` is set; otherwise, once a
 * `gPolarWumpaDispenseTimer` cooldown elapses, dispenses one of four
 * tiers of collected wumpas at the player's position, sized by the
 * accumulator, and plays a cue. */
void PolarPlayer::DispenseWumpa()
{
    s32 acc = gPolarQueuedWumpa;

    if (acc == 0)
        return;

    if (gPolarPlayerInactive != 0) {
        do {
            CollectWumpa(gLevelState);
            gPolarQueuedWumpa--;
        } while (gPolarQueuedWumpa != 0);
        return;
    }

    if (gPolarWumpaDispenseTimer != 0) {
        gPolarWumpaDispenseTimer--;
        return;
    }

    gPolarWumpaDispenseTimer = 0xf;

    if (acc <= 9) {
        SpawnPolarCollectedWumpa(x, y, 1);
        gPolarQueuedWumpa -= 1;
    } else if (acc <= 0x13) {
        SpawnPolarCollectedWumpa(x, y, 2);
        gPolarQueuedWumpa -= 2;
    } else if (acc <= 0x27) {
        SpawnPolarCollectedWumpa(x, y, 4);
        gPolarQueuedWumpa -= 4;
    } else {
        SpawnPolarCollectedWumpa(x, y, 8);
        gPolarQueuedWumpa -= 8;
    }

    PlaySfx(gAudioContext, SFX_WUMPA, 0x100);
}

/* The pause menu is locked (PolarIsPauseLocked, actor_spawn.cpp). */
s32 PolarPlayer::IsPauseLocked()
{
    return gPolarPauseLocked;
}

/* State 13, recovering: after 20 frames, back to running (state 1) with
 * the steering on, undoing what Hurt sets. No code found enters it. */
void PolarPlayer::StateRecover()
{
    if (stateTime > 0x13) {
        gPolarSteerEnabled = 1;
        gPolarPlayerInactive = 0;
        SetState(1, 0);
        SetCellAnimSpeed(0x24);
    }
}

/* State 11, the leap over the finish line: rises by `gPolarPlayerVelY`
 * (slowing by 0x2d a frame) into the screen; the screen fades out (and
 * the pause menu locks) past depth 0x16FF, and the category exits
 * (cleared) past 0x3FF. */
void PolarPlayer::StateFinishLeap()
{
    y += gPolarPlayerVelY;
    gPolarPlayerVelY += 0x2d;
    z += 0x3c;
    depth = INT_TO_Q8(GetCellAnimDistance()) - z;

    if (gPolarFadeStarted == 0 && depth <= 0x16FF) {
        FadeBrightness(0, 2, 1);
        gPolarPauseLocked = 1;
        gPolarFadeStarted = 1;
    }

    if (depth <= 0x3FF)
        SetActorCategoryExitStatus(CATEGORY_EXIT_CLEARED);
}

/* State 8, carried off by the yeti: the same flight, and the category
 * exits as the boss's kill. */
void PolarPlayer::StateCarriedOff()
{
    y += gPolarPlayerVelY;
    gPolarPlayerVelY += 0x2d;
    z += 0x3c;
    depth = INT_TO_Q8(GetCellAnimDistance()) - z;

    if (gPolarFadeStarted == 0 && depth <= 0x16FF) {
        FadeBrightness(0, 2, 1);
        gPolarFadeStarted = 1;
    }

    if (depth <= 0x3FF)
        SetActorCategoryExitStatus(CATEGORY_EXIT_BOSS_DEATH);
}

/* State 6, knocked off the bear: sinks by 0x100 a frame; the screen
 * fades out below y -0x4000, and the category exits (a death) below
 * -0x7200. */
void PolarPlayer::StateKnockedOff()
{
    y += -0x100;

    if (gPolarFadeStarted == 0 && y < (s32)0xFFFFC000) {
        FadeBrightness(0, 2, 1);
        gPolarFadeStarted = 1;
    }

    if (y < (s32)0xFFFF8E00)
        SetActorCategoryExitStatus(CATEGORY_EXIT_DEATH);
}

/* State 3, boosted: after 30 frames the steering is back on, and the
 * player runs (state 1), or dashes (state 2) while B is held. */
void PolarPlayer::StateBoost()
{
    if (stateTime == 0x1e) {
        gPolarSteerEnabled = 1;

        /* The mask is a variable the test overwrites: the ROM loads the
         * state's 2 again, where a plain `gKeys.all & 2` lets CSE reuse
         * the mask's register for it. */
        u32 keys = gKeys.all;
        u32 bit = B_BUTTON;

        bit = keys &= bit;
        if ((u16)bit == 0) {
            SetState(1, 0);
            SetCellAnimSpeed(0x24);
        } else {
            state = 2;
            stateTime = 0;
            SetCellAnimSpeed(0x38);
        }
    }
}
