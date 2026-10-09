#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "yeti.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* PolarPlayer's states and the methods the other polar actors call on
 * it (#664 part 11c, include/vehicle.hpp), ROM 0x0802BC68-0x0802C270,
 * between polar_player.cpp (old_agbcp) and polar_pickups.cpp. Its
 * destructor is the class's key method, so g++ emits gPolarPlayerVtable
 * here. The actions and the state dispatch were polar_player_actions.cpp
 * and polar_player_dispatch.cpp until #771.
 *
 * First the wumpa dispenser, the pause lock and eight of the states,
 * ROM 0x0802BC68-0x0802BFD4. DispenseWumpa is the
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
            gLevelState->CollectWumpa();
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

    gAudioContext->PlaySfx(SFX_WUMPA, 0x100);
}

/* The pause menu is locked (PolarIsPauseLocked, actor_category.cpp). */
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
        gAudioContext->PlaySfx(SFX_POLAR_FINISH_LEAP, 0x100);
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

/* The methods the other polar actors call on PolarPlayer (gActorList)
 * and its destructor, ROM 0x0802BFD4-0x0802C208. */

/* The course's end (PolarReachCourseEnd, actor_category_hooks.cpp): once, the finish
 * countdown (state 10 when it runs out, Update), the yeti stops, and the
 * player is inactive with the steering off. */
void PolarPlayer::FinishRun()
{
    if (gPolarPlayerInactive == 0) {
        gPolarFinishTimer = 0x16;
        SetCellAnimSpeed(0x24);
        LIMIT_MIN(gPolarPlayerVelY, 0);
        Yeti::Stop();
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

/* The polar player's state dispatch (#664 part 11a) and
 * IsPolarPlayerInactive, ROM 0x0802C208-0x0802C270. */

/* The state method for `state`, through the pointer-to-member table
 * stateFuncs (gPolarPlayerStateFuncs). */
void PolarPlayer::RunState()
{
    (this->*stateFuncs[state])();
}

/* Whether the player is inactive (no caller). */
u8 IsPolarPlayerInactive(void)
{
    return gPolarPlayerInactive;
}
