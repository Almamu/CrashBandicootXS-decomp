#include "actor_self.hpp"
#include "vehicle.hpp"
#include "engine/actor/actor_category.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "match.h"
#include <libgcc.h>
#include "engine/actor/actor.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
}

using Engine::Actor::ActorCategory;
using Game::Entities::ActorSelf;
using Game::Vehicles::Jetpack::JetpackPlayer;
using Game::Vehicles::Polar::PolarPlayer;

/* The vehicle levels' category hooks before ActorSelf in the ROM
 * (0x0802A69C-0x0802A700): JetpackReloadPlayerTiles-PolarReachCourseEnd
 * (JetpackPlayer's and PolarPlayer's static ReloadPlayerTiles and
 * ReachCourseEnd since #915) and ActorCategory::IsTouchingPlayer.
 * Built with old_agbcp and -fno-implement-inlines, like actor.cpp, where
 * it was until #770. See docs/matching/archive/issue-50-actor-2a69c.md. */

/* The vehicle levels' category hooks (ActorCategory::vtables' slots):
 * each calls its method on the player
 * (gActorList, the list's root). */
void JetpackPlayer::ReloadPlayerTiles()
{
    gActorList->AsJetpackPlayer()->AllocTiles();
}

void PolarPlayer::ReloadPlayerTiles()
{
    gActorList->AsPolarPlayer()->AllocTiles();
}

void JetpackPlayer::ReachCourseEnd()
{
    gActorList->AsJetpackPlayer()->FinishRun();
}

void PolarPlayer::ReachCourseEnd()
{
    gActorList->AsPolarPlayer()->FinishRun();
}

/* The selected category's slot 9 (its player contact test) on `self`. */
s32 ActorCategory::IsTouchingPlayer(ActorSelf *self)
{
    return vtable->isTouchingPlayer(self);
}
