#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "level.h"
#include "globals.h"
}

/* The polar course's goal, boost pad and checkpoint crate (#664 part
 * 11d, include/vehicle.hpp), ROM 0x0802D59C-0x0802D7B0, between
 * polar_aku_aku.cpp and yeti_update.cpp: each one's Update and
 * constructor. */

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
            gAudioContext->PlaySfx(SFX_BOOST_PAD, 0x100);
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
            gAudioContext->PlaySfx(SFX_CHECKPOINT, 0x100);
            gLevelState->AddBrokenCrate();
            SetActorCheckpoint(z);
            CreatePolarCheckpointText(x, y - 0xF00, z);
        }

        if (animIndex == 0 && IsTouchingYeti(this)) {
            RestartAnim(3);
            palette = 1;
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            gLevelState->AddBrokenCrate();
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
        gAudioContext->PlaySfx(SFX_CHECKPOINT, 0x100);
    }
}
