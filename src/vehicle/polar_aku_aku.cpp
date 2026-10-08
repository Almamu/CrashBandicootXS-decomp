#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "level_state.h"
#include "audio.h"
#include "actor.h"
#include "level.h"
#include "globals.h"
}

/* Aku Aku's masks, the goal, the boost pad and the checkpoint crate
 * (#664 part 11d, include/vehicle.hpp), ROM 0x0802D3A8-0x0802D5D4, between
 * polar_objects.cpp and yeti_update.c. The mask level is the level's
 * (gLevelState->maskLevel, SetMaskLevel); PolarAkuAku::Refresh
 * (polar_objects.cpp) shows it. */

/* All masks gone (PolarPlayer::Catch). */
void PolarAkuAku::ClearMask()
{
    SetMaskLevel(gLevelState, MASK_LEVEL_NONE);
    Refresh(0);
}

/* A mask lost (PolarPlayer::Hurt and Shock): the level, one down
 * unless none is left, and a lost mask shown. */
s32 PolarAkuAku::RemoveMask()
{
    s32 level;

    PlaySfx(gAudioContext, SFX_AKU_AKU_LOSE, 0x100);
    level = gLevelState->maskLevel;
    if (level != MASK_LEVEL_NONE) {
        level -= 1;
        SetMaskLevel(gLevelState, level);
    }
    Refresh(1);
    return level;
}

/* A mask gained (PolarPlayer::GiveMask): the level, one up to
 * invincible. */
s32 PolarAkuAku::AddMask()
{
    s32 level;

    PlaySfx(gAudioContext, SFX_AKU_AKU_GAIN, 0x100);
    level = gLevelState->maskLevel;
    if (level != MASK_LEVEL_INVINCIBLE) {
        level += 1;
        SetMaskLevel(gLevelState, level);
    }
    Refresh(0);
    return level;
}

/* Behind the player (SpawnPolarAkuAku, actor_factory.cpp: at its
 * position), with the mask level `level`. */
PolarAkuAku::PolarAkuAku(const struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 level)
    : ActorSelf(rec, x - 0x1000, y - 0x1E00, z - 0x200)
{
    SetMaskLevel(gLevelState, level);
    Refresh(0);
}

/* The mask level (no caller). */
void PolarAkuAku::SetMask(s32 level)
{
    SetMaskLevel(gLevelState, level);
}

/* The mask level (no caller). */
s32 GetPolarMaskLevel(void)
{
    return gLevelState->maskLevel;
}

/* Shown after 5 frames; the player finishes the run on touch. */
void PolarGoal::Update()
{
    if (stateTime > 5)
        visible = 1;

    if ((u8)IsTouchingPlayer(this))
        static_cast<PolarPlayer *>(gActorList)->FinishRun();

    ActorSelf::Update();
}

PolarGoal::PolarGoal(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
    visible = 0;
}

/* Boosts the player at its x on touch, with SFX_BOOST_PAD the first
 * time. */
void PolarBoostPad::Update()
{
    if ((u8)IsTouchingPlayer(this)) {
        static_cast<PolarPlayer *>(gActorList)->Boost(x);
        if (once == 0) {
            PlaySfx(gAudioContext, SFX_BOOST_PAD, 0x100);
            once = 1;
        }
    }

    ActorSelf::Update();
}

/* Its look, from its side of the course: left (x < -20), middle or right
 * (x > 20). */
PolarBoostPad::PolarBoostPad(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
    s32 side = Q8_TO_INT(x);
    s32 idx;

    if (side < -0x14) {
        idx = 0;
    } else {
        idx = 1;
        if (side > 0x14)
            idx = 2;
    }

    RestartAnim(idx);
    once = 0;
}

/* Whole (animation 0): opened by the player (animation 1, the checkpoint
 * set at its z and the CHECKPOINT text above it), or broken by the yeti
 * (animation 3), and deleted once that animation is done. */
void PolarCheckpointCrate::Update()
{
    if (animIndex == 0) {
        if ((u8)IsTouchingPlayer(this)) {
            RestartAnim(1);
            PlaySfx(gAudioContext, SFX_CHECKPOINT, 0x100);
            AddBrokenCrate(gLevelState);
            SetActorCheckpoint(z);
            CreatePolarCheckpointText(x, y - 0xF00, z);
        }

        if (animIndex == 0 && IsTouchingYeti(this)) {
            RestartAnim(3);
            palette = 1;
            PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
            AddBrokenCrate(gLevelState);
        }
    }

    if (animIndex == 3 && animDone != 0)
        delete this;
    else
        ActorSelf::Update();
}

/* Already open (animation 2) when it is the checkpoint the run restarted
 * from. */
PolarCheckpointCrate::PolarCheckpointCrate(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
    if (GetActorCheckpoint() == z) {
        RestartAnim(2);
        PlaySfx(gAudioContext, SFX_CHECKPOINT, 0x100);
    }
}
