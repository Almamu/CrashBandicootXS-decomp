#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The methods the other polar actors call on PolarPlayer (gActorList)
 * and its destructor (#664 part 11c, include/vehicle.hpp), ROM
 * 0x0802BFD4-0x0802C1BC, between polar_player_states.cpp and
 * polar_player_dispatch.cpp. */

/* The course's end (PolarReachCourseEnd, actor_category_hooks.cpp): once, the finish
 * countdown (state 10 when it runs out, Update), the yeti stops, and the
 * player is inactive with the steering off. */
void PolarPlayer::FinishRun()
{
    if (gPolarPlayerInactive == 0) {
        gPolarFinishTimer = 0x16;
        SetCellAnimSpeed(0x24);
        LIMIT_MIN(gPolarPlayerVelY, 0);
        StopYeti();
        gPolarPlayerInactive = 1;
        gPolarSteerEnabled = 0;
    }
}

/* Caught by the yeti (yeti_update.cpp): the steering off, the player halted
 * and inactive, Aku Aku's masks gone, and state 7 (anim 6). */
void PolarPlayer::Catch()
{
    gPolarSteerEnabled = 0;
    gPolarPlayerHalted = 1;
    gPolarAkuAku->ClearMask();
    gPolarPlayerInactive = 1;
    SetState(7, 6);
    gAudioContext->PlaySfx(SFX_YETI_CATCH, 0x100);
}

/* Feeds `n` wumpas into the `gPolarQueuedWumpa` accumulator
 * (DispenseWumpa drains it), arming its cooldown when it was empty; not
 * in time trials. */
void PolarPlayer::QueueWumpa(s32 n)
{
    if (gLevelState->timeTrial == 0) {
        if (gPolarQueuedWumpa == 0)
            gPolarWumpaDispenseTimer = 0xf;
        gPolarQueuedWumpa += n;
    }
}

/* The extra life crate's life. */
void PolarPlayer::GiveLife()
{
    gLevelState->AddLife();
}

/* A boost pad at `x` (polar_course_objects.cpp): only while running, dashing or
 * boosted (states 1-3), the player is put on the pad, plays anim 2 and
 * is boosted (state 3) with the steering off; the bear's speed jumps
 * (0x5a from a dash, 0x55 from a run). */
void PolarPlayer::Boost(s32 x)
{
    if ((u32)(state - 1) <= 2) {
        animIndex = 2;
        animTimer = anims[2].duration;
        animDone = 0;
        if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold)
            animTime = 0;

        this->x = x;

        if (state == 2)
            SetCellAnimSpeed(0x5a);
        else if (state == 1)
            SetCellAnimSpeed(0x55);

        state = 3;
        stateTime = 0;
        gPolarSteerEnabled = 0;
    }
}

/* An Aku Aku crate's mask: the third one makes the player invulnerable
 * for 500 frames. */
void PolarPlayer::GiveMask()
{
    if (gPolarAkuAku->AddMask() == 3)
        gPolarInvulnTimer = 500;
}

/* A launcher (polar_objects.cpp): only while running, dashing or boosted
 * and not invulnerable, the launch (state 5, anim 3) with the steering
 * off. */
void PolarPlayer::Launch()
{
    if ((u32)(state - 1) <= 2) {
        if (gPolarInvulnTimer == 0) {
            gPolarSteerEnabled = 0;
            SetState(5, 3);
            gPolarPlayerVelY = 0xFFFFF880;
            SetCellAnimSpeed(0x1c);
        }
    }
}

/* Slot 1: collects the wumpas still queued and frees the two VRAM tile
 * buffers (AllocTiles). */
PolarPlayer::~PolarPlayer()
{
    if (gPolarQueuedWumpa != 0) {
        do {
            gLevelState->CollectWumpa();
            gPolarQueuedWumpa--;
        } while (gPolarQueuedWumpa != 0);
    }

    FreeVramTileBlock(gPolarPlayerTiles[0]);
    FreeVramTileBlock(gPolarPlayerTiles[1]);
}
