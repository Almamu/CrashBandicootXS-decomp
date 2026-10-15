#include "game/entities/actor_self.hpp"
#include "game/vehicles/vehicle.hpp"
#include "game/level/level_state.hpp"
#include "engine/actor/actor_category.hpp"

extern "C" {
#include "core.h"
#include "engine/util/math_util.h"
#include "engine/sys/memory.h"
#include "engine/actor/actor_anim.h"
#include "engine/actor/actor.h"
#include "game/bosses/bosses.h"
#include "globals.h"
}

using Engine::Actor::ActorCategory;
using Game::Entities::ActorSelf;
using Game::Vehicles::Jetpack::JetpackPlayer;
using Game::Vehicles::Polar::PolarPlayer;

/* The category hooks that go through the category vtable, the actor
 * teardown and the exit status, ROM 0x0802A5AC-0x0802A69C; split from
 * actor_spawn.cpp (#770).
 *
 * The category vtable (include/engine/actor/actor_anim.h's 13-slot
 * `struct category_vtable`, set by ActorCategory::Select) is a struct of
 * plain function pointers and two values, not a C++ vtable (#765). C++
 * since #664 part 11b (docs/cplusplus.md). */

/* Slot 12 (`isPauseLocked`: PolarPlayer::IsPlayerPauseLocked,
 * JetpackPlayer::IsPlayerPauseLocked),
 * negated. The slot's functions return `s32`, but the ROM treats the
 * result as a `bool` (the one cast #765 keeps), so `!` is the ROM's
 * `movs r1, #1; eors r0, r1` on the call's r0. Through an `s32` g++
 * copies the result to r1 and builds the 1 in r0; the C wrote the two
 * instructions in asm and the first C++ pinned the result to r0 (#662
 * round 2). Its caller (ActorCategory::Init) zero-extends the result, as
 * for a `bool`. */
bool ActorCategory::CanPause()
{
    return !((bool (*)(void))vtable->isPauseLocked)();
}

void ActorCategory::ReloadGraphics()
{
    vtable->loadBossGraphics();
    vtable->reloadPlayerTiles();
}

/* Deletes every actor, the list's root (the player) last, then the draw
 * list's buffer. */
void ActorCategory::DestroyActors()
{
    ActorSelf *n;
    /* `next` is needed: inlined, the code is 1 instruction shorter */
    ActorSelf *next;

    if (vtable->destroyBoss != NULL)
        vtable->destroyBoss();

    n = gActorList->next;
    while (n != gActorList) {
        next = n->next;
        delete n;
        n = next;
    }
    delete gActorList;

    mem_free(drawList);
}

void ActorCategory::UpdateBg2()
{
    if (vtable->updateBossBg2 != NULL)
        vtable->updateBossBg2();
}

void ActorCategory::SetExitStatus(s32 status)
{
    exitStatus = status;
}

/* Slot 12's hooks, JetpackIsPauseLocked and PolarIsPauseLocked: static
 * members since #915, named IsPlayerPauseLocked because the method they
 * call has the name IsPauseLocked (a static member can't overload it).
 * Both forward the player's IsPauseLocked untouched: it returns `s32`
 * (vehicle.hpp; the bodies, in jetpack_player.cpp and
 * polar_player_states.cpp, return a byte), so there is no re-narrowing
 * and the epilogue returns through `pop {r1}`. The old NAKED note blamed
 * a TU-wide allocator quirk; it was just the missing return value. */
s32 JetpackPlayer::IsPlayerPauseLocked()
{
    return gActorList->AsJetpackPlayer()->IsPauseLocked();
}

s32 PolarPlayer::IsPlayerPauseLocked()
{
    return gActorList->AsPolarPlayer()->IsPauseLocked();
}
