#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "audio.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* PolarPlayer's three landing and finish states, the methods the other
 * polar actors call on the player (gActorList) and its destructor (#664
 * part 11c, include/vehicle.hpp), ROM 0x0802BED8-0x0802C1BC, between
 * polar_player_states.cpp and polar_player_dispatch.cpp. */

/* State 5, launched by a launcher: rises and falls by `gPolarPlayerVelY`
 * (gravity 0x60, to 0x780) and, back on the ground (y 0x2800), runs again
 * (state 1, anim 4) with the steering on. */
void PolarPlayer::StateLaunched()
{
    s32 total = y + gPolarPlayerVelY;

    y = total;
    gPolarPlayerVelY += 0x60;
    LIMIT_MAX(gPolarPlayerVelY, 0x780);

    if (total > 0x2800) {
        y = 0x2800;
        gPolarSteerEnabled = 1;
        SetCellAnimSpeed(0x24);
        SetState(1, 4);
    }
}

/* State 10, at the finish line: once the animation is done, the leap
 * (state 11, anim 7, sfx SFX_POLAR_FINISH_LEAP); the bear, left behind,
 * runs on riderless when the player is high enough. */
void PolarPlayer::StateFinish()
{
    if (animDone != 0) {
        PlaySfx(gAudioContext, SFX_POLAR_FINISH_LEAP, 0x100);
        gPolarPlayerVelY = 0xFFFFF980;
        SetState(0xb, 7);
        if (y > 0x2000)
            gRiderlessPolar = CreateActor(2, x, 0x2800, z, 0);
    }
}

/* State 9, landing on the bear: once the animation is done, running
 * (state 1) with the steering on. */
void PolarPlayer::StateLand()
{
    if (animDone != 0) {
        gPolarSteerEnabled = 1;
        SetState(1, 0);
        SetCellAnimSpeed(0x24);
    }
}

/* The course's end (PolarReachCourseEnd, actor.cpp): once, the finish
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

/* Caught by the yeti (yeti_update.c): the steering off, the player halted
 * and inactive, Aku Aku's masks gone, and state 7 (anim 6). */
void PolarPlayer::Catch()
{
    gPolarSteerEnabled = 0;
    gPolarPlayerHalted = 1;
    gPolarAkuAku->ClearMask();
    gPolarPlayerInactive = 1;
    SetState(7, 6);
    PlaySfx(gAudioContext, SFX_YETI_CATCH, 0x100);
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
    AddLife(gLevelState);
}

/* A boost pad at `x` (polar_aku_aku.cpp): only while running, dashing or
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
            CollectWumpa(gLevelState);
            gPolarQueuedWumpa--;
        } while (gPolarQueuedWumpa != 0);
    }

    FreeVramTileBlock(gPolarPlayerTiles[0]);
    FreeVramTileBlock(gPolarPlayerTiles[1]);
}
