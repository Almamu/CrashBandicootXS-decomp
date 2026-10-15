#define POLAR_WUMPA_CONSTRUCTOR_OUT_OF_LINE
#include "game/vehicles/vehicle.hpp"
#include "engine/audio/audio.hpp"
#include "game/level/level_state.hpp"
#include "engine/actor/actor_bg.hpp"
#include "engine/actor/actor_category.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "engine/actor/actor.h"
#include "engine/gfx/gfx.h"
#include "game/level/level.h"
#include "globals.h"
}

using Engine::Actor::ActorBg;
using Engine::Actor::ActorCategory;
using Game::Entities::ActorSelf;
using Game::Vehicles::Polar::PolarCollectedWumpa;
using Game::Vehicles::Polar::PolarPlayer;
using Game::Vehicles::Polar::PolarWumpa;

/* The polar wumpas (#664 part 11d, engine/include/game/vehicles/vehicle.hpp), ROM
 * 0x0802C270-0x0802C4C8, between polar_player_states.cpp and
 * polar_crate.cpp: the collected wumpa (PolarCollectedWumpa) and the
 * course's wumpa (PolarWumpa). */

/* Flies to the HUD's wumpa counter (0x1000, 0x1000), then is deleted with
 * SFX_HUD_COLLECT; on the way, its animation runs on (UpdateActor's
 * step, with no depth test or state timer). */
void PolarCollectedWumpa::Update()
{
    sortKey = 1;
    x += velX;
    y += velY;

    if (!(x > INT_TO_Q8(16) && y > INT_TO_Q8(16))) {
        gAudioContext->PlaySfx(SFX_HUD_COLLECT, 0x100);
        delete this;
        return;
    }

    animTime += animTimer;
    animDone = 0;
    if (GetAnimFrameBaseOffset() >= anims[animIndex].loopThreshold) {
        animTime -= INT_TO_Q8(anims[animIndex].loopThreshold - anims[animIndex].loopBase);
        animDone = 1;
    }
}

/* At its screen position (x and y are screen coordinates), at scale
 * 1.25: DrawActor's (ActorSelf::Draw) sprite (DrawFrameAt,
 * actor_self.hpp). */
void PolarCollectedWumpa::Draw()
{
    DrawFrameAt(Q8_TO_INT(x), Q8_TO_INT(y), Q8(1.25));
}

/* Counts in its fruit. */
PolarCollectedWumpa::~PolarCollectedWumpa()
{
    s32 i;

    for (i = 0; i < count; i++)
        gLevelState->CollectWumpa();
}

/* At (x, y) on the screen, from the BG's centre, aimed at the wumpa
 * counter: the speed covers the Manhattan distance in 1/0x800 steps.
 * CreateJetpackCollectedWumpa (hovercraft.c) is its twin. */
PolarCollectedWumpa::PolarCollectedWumpa(const struct anim_table_record *rec, s32 x, s32 y,
                                         s32 count)
    : ActorSelf(rec, x, y, 1)
{
    s32 nx, ny;
    s32 dist;
    s32 steps;

    this->count = count;
    this->y += ActorBg::GetCenterY();
    nx = this->x + ActorBg::GetCenterX();
    this->x = nx;
    dist = ABS_BRANCHLESS(nx - 0x1000);
    ny = this->y;
    dist += ABS_BRANCHLESS(ny - 0x1000);
    steps = dist / 0x800;
    velX = (0x1000 - nx) / steps;
    velY = (0x1000 - ny) / steps;
}

/* Collected by the player: one wumpa. */
void PolarWumpa::Update()
{
    if ((u8)ActorCategory::IsTouchingPlayer(this)) {
        static_cast<PolarPlayer *>(gActorList)->QueueWumpa(1);
        delete this;
    } else {
        ActorSelf::Update();
    }
}

/* The out-of-line copy (CreatePolarWumpa, no caller; vehicle.hpp has the
 * inline one). */
PolarWumpa::PolarWumpa(const struct anim_table_record *rec, s32 x, s32 y, s32 z)
    : ActorSelf(rec, x, y, z)
{
}
