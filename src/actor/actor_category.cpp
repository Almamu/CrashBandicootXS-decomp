#include "actor_self.hpp"
#include "vehicle.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "globals.h"
}

/* The category hooks that go through the category vtable, the actor
 * teardown and the exit status, ROM 0x0802A5AC-0x0802A69C; split from
 * actor_spawn.cpp (#770).
 *
 * The category vtable (include/actor_anim.h's 13-slot
 * `struct category_vtable`, set by SelectActorCategory) is a struct of
 * plain function pointers and two values, not a C++ vtable (#765). C++
 * since #664 part 11b (docs/cplusplus.md). */

/* Slot 12 (`isPauseLocked`: PolarIsPauseLocked, JetpackIsPauseLocked),
 * negated. The slot's functions return `s32`, but the ROM treats the
 * result as a `bool` (the one cast #765 keeps), so `!` is the ROM's
 * `movs r1, #1; eors r0, r1` on the call's r0. Through an `s32` g++
 * copies the result to r1 and builds the 1 in r0; the C wrote the two
 * instructions in asm and the first C++ pinned the result to r0 (#662
 * round 2). Its caller (InitActorCategory) zero-extends the result, as
 * for a `bool`. */
bool CanPauseActorCategory(void)
{
    return !((bool (*)(void))gActorCategoryVtable->isPauseLocked)();
}

void ReloadActorCategoryGraphics(void)
{
    gActorCategoryVtable->loadBossGraphics();
    gActorCategoryVtable->reloadPlayerTiles();
}

/* Deletes every actor, the list's root (the player) last, then the draw
 * list's buffer. */
void DestroyAllActors(void)
{
    ActorSelf *n;
    ActorSelf *next;

    if (gActorCategoryVtable->destroyBoss != NULL)
        gActorCategoryVtable->destroyBoss();

    n = gActorList->next;
    while (n != gActorList) {
        next = n->next;
        delete n;
        n = next;
    }
    delete gActorList;

    mem_free(gActorDrawList);
}

void UpdateActorCategoryBg2(void)
{
    if (gActorCategoryVtable->updateBossBg2 != NULL)
        gActorCategoryVtable->updateBossBg2();
}

void SetActorCategoryExitStatus(s32 arg0)
{
    gActorCategoryExitStatus = arg0;
}

/* Both forward the player's IsPauseLocked untouched: it returns `s32`
 * (vehicle.hpp; the bodies, in jetpack_player.cpp and
 * polar_player_states.cpp, return a byte), so there is no re-narrowing
 * and the epilogue returns through `pop {r1}`. The old NAKED note blamed
 * a TU-wide allocator quirk; it was just the missing return value. */
s32 JetpackIsPauseLocked(void)
{
    return ((JetpackPlayer *)gActorList)->IsPauseLocked();
}

s32 PolarIsPauseLocked(void)
{
    return ((PolarPlayer *)gActorList)->IsPauseLocked();
}
