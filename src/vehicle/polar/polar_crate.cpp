#include "vehicle.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "yeti.hpp"
#include "collected_spawns.hpp"

extern "C" {
#include "math_util.h"
#include "actor.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
}

/* The polar crates' shared update (#664 part 11d, include/vehicle.hpp),
 * ROM 0x0802C4C8-0x0802C7A8, between polar_pickups.cpp and
 * polar_nitro.cpp: PolarCrate's Update (its key method, so
 * gPolarCrateVtable is emitted here) and three crate kinds'. */

/* Every crate kind's update ends here: the yeti breaks it, and once the
 * broken animation has played it is deleted. */
void PolarCrate::Update()
{
    if (animIndex != 0x12 && Yeti::IsTouching(this)) {
        gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
        gLevelState->AddBrokenCrate();
        Break();
    }

    if (animIndex == 0x12 && animDone != 0) {
        delete this;
        return;
    }

    ActorSelf::Update();
}

/* By its record: 1, 3 or 5 wumpas (0x1C-0x1E), or a mask (0x1F). */
void PolarQuestionCrate::Update()
{
    if (animIndex != 0x12 && (u8)IsTouchingPlayer(this)) {
        gLevelState->AddBrokenCrate();

        switch (record->index) {
        case POLAR_KIND_QUESTION_CRATE_WUMPA_1:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            static_cast<PolarPlayer *>(gActorList)->QueueWumpa(1);
            break;
        case POLAR_KIND_QUESTION_CRATE_WUMPA_3:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            static_cast<PolarPlayer *>(gActorList)->QueueWumpa(3);
            break;
        case POLAR_KIND_QUESTION_CRATE_WUMPA_5:
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            static_cast<PolarPlayer *>(gActorList)->QueueWumpa(5);
            break;
        case POLAR_KIND_QUESTION_CRATE_AKU_AKU:
            static_cast<PolarPlayer *>(gActorList)->GiveMask();
            break;
        }

        Break();
    }

    PolarCrate::Update();
}

/* An extra life, once: the spawn is marked collected (CreateActor builds
 * a question crate in its place from then on). */
void PolarLifeCrate::Update()
{
    if (animIndex != 0x12) {
        if ((u8)IsTouchingPlayer(this)) {
            gAudioContext->PlaySfx(SFX_EXTRA_LIFE, 0x100);
            gLevelState->AddBrokenCrate();
            static_cast<PolarPlayer *>(gActorList)->GiveLife();
            CollectedSpawns::Add(spawn);
            Break();
            palette = 1;
        }

        if (animIndex != 0x12 && Yeti::IsTouching(this)) {
            gAudioContext->PlaySfx(SFX_CRATE_BREAK, 0x100);
            gLevelState->AddBrokenCrate();
            Break();
            palette = 1;
        }
    }

    PolarCrate::Update();
}

/* Hurts the player and explodes on touch (the yeti sets it off too); one
 * left behind (depth past 0xA000) counts as missed. Once broken, its box
 * is the blast's (gPolarNitroCrateBox), and 0x14 frames on it sets off
 * the nitros next to it. */
void PolarNitroCrate::Update()
{
    if (animIndex != 0x12) {
        if (depth > INT_TO_Q8(160)) {
            AddActorMissedNitro();
            delete this;
            return;
        }
        if ((u8)IsTouchingPlayer(this)) {
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            gLevelState->AddBrokenCrate();
            static_cast<PolarPlayer *>(gActorList)->Hurt();
            Explode();
        } else if (Yeti::IsTouching(this)) {
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
            gLevelState->AddBrokenCrate();
            Explode();
        }
    } else {
        box = gPolarNitroCrateBox;
        if (stateTime == 0x14)
            DetonateNearby();
    }

    PolarCrate::Update();
}
