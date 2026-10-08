#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include "level_state.h"
#include <libgcc.h>
#include "system.h"
#include "audio.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* JetpackPlayer's small methods, five of its states, its destructor and
 * its state dispatch (#664 part 11e, include/vehicle.hpp). See
 * docs/matching/archive/issue-56-0x0802f0dc-actor.md. */

/* Drains the `gJetpackQueuedWumpa` reward accumulator QueueWumpa fills:
 * all at once (repeated `CollectWumpa` calls) while
 * `gJetpackPlayerInactive` is set; otherwise, once a
 * `gJetpackWumpaDispenseTimer` cooldown elapses, dispenses one of four
 * tiers of collected wumpas at the player's position, sized by the
 * accumulator, and plays a cue. */
void JetpackPlayer::DispenseWumpa()
{
    s32 acc = gJetpackQueuedWumpa;

    if (acc == 0)
        return;

    if (gJetpackPlayerInactive != 0) {
        do {
            CollectWumpa(gLevelState);
            gJetpackQueuedWumpa--;
        } while (gJetpackQueuedWumpa != 0);
        return;
    }

    if (gJetpackWumpaDispenseTimer != 0) {
        gJetpackWumpaDispenseTimer--;
        return;
    }

    gJetpackWumpaDispenseTimer = 0xf;

    if (acc <= 9) {
        SpawnJetpackCollectedWumpa(x, y, 1);
        gJetpackQueuedWumpa -= 1;
    } else if (acc <= 0x13) {
        SpawnJetpackCollectedWumpa(x, y, 2);
        gJetpackQueuedWumpa -= 2;
    } else if (acc <= 0x27) {
        SpawnJetpackCollectedWumpa(x, y, 4);
        gJetpackQueuedWumpa -= 4;
    } else {
        SpawnJetpackCollectedWumpa(x, y, 8);
        gJetpackQueuedWumpa -= 8;
    }

    PlaySfx(gAudioContext, SFX_WUMPA, 0x100);
}

/* Counts a bomber in the air this frame (Update plays their engine sound
 * by the count). */
s32 JetpackPlayer::CountBomber()
{
    return ++gJetpackBomberCount;
}

/* Slot 6: the hit points as a percentage of the maximum (100 or 120); a
 * player with any left shows at least 1. */
s32 JetpackPlayer::GetHp()
{
    s32 v;
    s32 r;

    if (gJetpackPlayerMaxHp == 0x64)
        return hp;

    v = hp;
    r = __divsi3(v * 0x64, 0x78);
    if (r == 0 && v > 0)
        r = 1;
    return r;
}

/* Sets the actors' checkpoint 0x7800 past the player. */
void JetpackPlayer::SetCheckpoint()
{
    SetActorCheckpoint(z + 0x7800);
}

s32 JetpackPlayer::IsPauseLocked()
{
    return gJetpackPauseLocked;
}

/* While `gJetpackFlashTimer` counts down, the player's palette flashes:
 * frames 0-2 of gJetpackFlashPalettes and back. */
void JetpackPlayer::AnimatePalette()
{
    if (gJetpackFlashTimer != 0) {
        s32 frame;

        gJetpackFlashTimer--;
        frame = __divsi3(gJetpackFlashTimer, 3);
        if (frame > 2)
            frame = 5 - frame;
        QueueVramDmaTransfer((void *)gJetpackFlashPalettes[frame], (void *)OBJ_PLTT, 0x20, 0x10);
    }
}

/* Gives back `delta` percent of the maximum hit points, while the player
 * is active. */
void JetpackPlayer::Heal(s32 delta)
{
    if (gJetpackPlayerInactive == 0) {
        s32 max = gJetpackPlayerMaxHp;
        s32 add = __divsi3(delta * max, 0x64);
        s32 v = hp + add;

        hp = v;
        if (v > max)
            hp = max;
    }
}

/* Feeds `delta` wumpas into the `gJetpackQueuedWumpa` accumulator
 * (DispenseWumpa drains it), arming its cooldown when it was empty;
 * not in time trials. */
void JetpackPlayer::QueueWumpa(s32 delta)
{
    if (gLevelState->timeTrial == 0) {
        if (gJetpackQueuedWumpa == 0)
            gJetpackWumpaDispenseTimer = 0xf;
        gJetpackQueuedWumpa += delta;
    }
}

/* State 7, resuming at a checkpoint: once the animation is done, flying
 * (state 1), with the input enabled. */
void JetpackPlayer::StateResume()
{
    if (animDone != 0) {
        SetState(1, 0);
        SetCellAnimSpeed(0x28);
        gJetpackInputEnabled = 1;
        gJetpackPlayerInactive = 0;
    }
}

/* State 6, through a ring: animation 5 back to 0 once it's done, and
 * flying again (state 1) after 0x32 frames. */
void JetpackPlayer::StateBoost()
{
    if (animIndex == 5 && animDone != 0) {
        animIndex = 0;
        animTimer = anims[0].duration;
        animDone = 0;
        animTime = 0;
    }

    if (stateTime == 0x32) {
        state = 1;
        stateTime = 0;
        SetCellAnimSpeed(0x28);
    }
}

/* State 4, shot down: falls faster and faster (to 0x140), the screen
 * fades out past y 0x7080, and the category exits (a death) past
 * 0xE100. */
void JetpackPlayer::StateFall()
{
    s32 v = gJetpackPlayerVelY + 9;
    s32 sign;

    gJetpackPlayerVelY = v;
    MAKE_ABS_BRANCHLESS(v, sign);
    if (v > 0x140)
        gJetpackPlayerVelY = 0x140;

    if (gJetpackFadeStarted == 0 && y > 0x7080) {
        FadeBrightness(0, 2, 1);
        gJetpackFadeStarted = 1;
    }

    if (y > 0xE100)
        SetActorCategoryExitStatus(CATEGORY_EXIT_DEATH);
}

/* State 5, the course's end: flies into the screen, the screen fades out
 * (and the pause menu locks) past depth 0x8200, and the category exits
 * (cleared) past 0xA000. */
void JetpackPlayer::StateFinish()
{
    s32 v;

    z += 0x200;

    v = z - INT_TO_Q8(GetCellAnimDistance());
    depth = v;

    if (gJetpackFadeStarted == 0 && v > 0x8200) {
        FadeBrightness(0, 2, 1);
        gJetpackFadeStarted = 1;
        gJetpackPauseLocked = 1;
    }

    if (depth > 0xA000)
        SetActorCategoryExitStatus(CATEGORY_EXIT_CLEARED);
}

/* State 0, flying in: once past y 0x1E00, flying (state 1), with the
 * input enabled. */
void JetpackPlayer::StateEnter()
{
    if (y > 0x1E00) {
        SetState(1, 0);
        SetCellAnimSpeed(0x28);
        gJetpackInputEnabled = 1;
        gJetpackPlayerInactive = 0;
    }
}

/* Slot 1: collects the wumpas still queued and frees the two VRAM tile
 * buffers (AllocTiles). */
JetpackPlayer::~JetpackPlayer()
{
    if (gJetpackQueuedWumpa != 0) {
        do {
            CollectWumpa(gLevelState);
            gJetpackQueuedWumpa--;
        } while (gJetpackQueuedWumpa != 0);
    }

    FreeVramTileBlock(gJetpackPlayerTiles[0]);
    FreeVramTileBlock(gJetpackPlayerTiles[1]);
}

/* The state method for `state`, through the pointer-to-member table
 * stateFuncs (gJetpackPlayerStateFuncs). */
void JetpackPlayer::RunState()
{
    (this->*stateFuncs[state])();
}

u8 IsJetpackPlayerInactive(void)
{
    return gJetpackPlayerInactive;
}
