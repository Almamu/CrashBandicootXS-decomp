#include "actor_self.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "system.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "globals.h"
}

/* ROM 0x0802A018-0x0802A4D4, inside the actor zone that starts at
 * SetupActorVramPool (0x080291A4): the category hooks that test an actor
 * against the player (PolarIsTouchingPlayer, JetpackIsTouchingPlayer),
 * the category's frame (RunActorCategoryFrame) and the jetpack shot's
 * target search (FindShotTarget). C++ since #664 part 11b
 * (docs/cplusplus.md); an old_agbcc object (current agbcc schedules the
 * `asr`s differently).
 *
 * All three tests are the same 3-axis (Z, Y, X) box overlap, as in
 * UpdateYeti, IsTouchingYeti, DetonateNearbyPolarNitros and
 * IsTouchingAirship: each actor's box (`box`, +0x38) moved to its
 * position in whole units, the two compared: actor_self.hpp's
 * BoxOverlap of two WorldBoxes. */

static inline u8 ActorsOverlap(ActorSelf *pl, ActorSelf *self)
{
    return BoxOverlap(WorldBox(pl), WorldBox(self));
}

s32 PolarIsTouchingPlayer(ActorSelf *self)
{
    ActorSelf **plAddr = &gActorList;

    if (gPolarPlayerInactive != 0)
        return 0;
    return ActorsOverlap(*plAddr, self);
}

s32 JetpackIsTouchingPlayer(ActorSelf *self)
{
    ActorSelf **plAddr = &gActorList;

    if (gJetpackPlayerInactive != 0)
        return 0;
    return ActorsOverlap(*plAddr, self);
}

/* The category's frame: the category vtable's slot 3 (the frame's
 * start) and slot 10 once the course length is passed (the course end),
 * the spawns that have scrolled into view (slot 1, SpawnActor for the
 * polar levels), then every actor's Update, and the visible ones' Draw in
 * draw-order (gHeapSortActorsByKeyFunc on their `sortKey`).
 *
 * The spawn loop is a plain `while` whose exit test gcc copies ahead of
 * the loop (jump.c's duplicate_loop_exit_test), so the loop body starts at
 * a label and re-reads every global. The test must stay free of inline
 * functions (their block notes stop the copy), hence the macro, and
 * builds the "next record" address in the `GetActorSpawnZ` order (`off`,
 * then `base + 0x14`, then the sum) through two locals. */

/* "The next sub-effect record's threshold has scrolled into view":
 * `gActorSpawnTable[idx + 1].depth + gActorSpawnOffset <= scroll +
 * vtable slot 7` (read as a value), bounded by record 0's entry count. */
#define SUB_EFFECT_DUE()                                                       \
    (gActorSpawnIndex < gActorSpawnTable->link                                 \
     && (off = gActorSpawnIndex * 0x14, tb = (u8 *)gActorSpawnTable + 0x14, \
         *(s32 *)(tb + off)) + gActorSpawnOffset                              \
            <= scroll + (s32)gActorCategoryVtable->fn[7])

s32 RunActorCategoryFrame(void)
{
    s32 scroll;
    ActorSelf *n;
    s32 i;
    u8 *tb;
    s32 off;

    if (gActorCategoryVtable->fn[3] != NULL)
        gActorCategoryVtable->fn[3]();
    gActorCategoryExitStatus = CATEGORY_EXIT_NONE;
    scroll = GetCellAnimDistance();
    if (scroll - gActorSpawnOffset > gActorSpawnTable->depth)
        gActorCategoryVtable->fn[10]();
    if (gActorSpawnsPaused != 0) {
        gActorSpawnOffset += GetCellAnimFrameStep();
    } else {
        while (SUB_EFFECT_DUE()) {
            ((void (*)(void *, s32, s32))gActorCategoryVtable->fn[1])(
                (u8 *)gActorSpawnTable + (gActorSpawnIndex * 0x14 + 8), gActorSpawnUseBonus,
                INT_TO_Q8(gActorSpawnOffset));
            gActorSpawnIndex++;
        }
    }

    n = gActorList;
    do {
        ActorSelf *next = n->next;

        n->Update();
        n = next;
    } while (n != gActorList);

    gActorDrawCount = 0;
    n = gActorList;
    do {
        if (n->visible != 0)
            gActorDrawList[gActorDrawCount++] = n;
        n = n->next;
    } while (n != gActorList);
    gHeapSortActorsByKeyFunc(gActorDrawCount, gActorDrawList);

    for (i = 0; i < gActorDrawCount; i++)
        gActorDrawList[i]->Draw();
    gActorCategoryFrameCount++;
    return gActorCategoryExitStatus;
}

/* The first actor other than `self` that can be shot (slot 5 returns 0;
 * the ROM tests its low byte) and whose box overlaps `self`'s, or 0. */
HpActor *FindShotTarget(ActorSelf *self)
{
    HpActor *n = (HpActor *)gActorList->next;

    do {
        if (n != self && (u8)n->IsUnshootable() == 0 && ActorsOverlap(self, n))
            return n;
        n = (HpActor *)n->next;
    } while (n != gActorList);
    return 0;
}
