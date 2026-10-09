#include "actor_self.hpp"
#include "vehicle.hpp"

extern "C" {
#include "math_util.h"
#include "match.h"
#include <libgcc.h>
#include "actor.h"
#include "vehicle.h"
#include "gfx.h"
#include "globals.h"
}

/* The vehicle levels' category hooks before ActorSelf in the ROM
 * (0x0802A69C-0x0802A700): JetpackReloadPlayerTiles-PolarReachCourseEnd
 * and IsTouchingPlayer.
 * Built with old_agbcp and -fno-implement-inlines, like actor.cpp, where
 * it was until #770. See docs/matching/archive/issue-50-actor-2a69c.md. */

/* The vehicle levels' category hooks (gActorCategoryVtables' slots):
 * each ignores its argument and calls its method on the player
 * (gActorList, the list's root). */
void JetpackReloadPlayerTiles(void *arg0)
{
    ((JetpackPlayer *)gActorList)->AllocTiles();
}

void PolarReloadPlayerTiles(void *arg0)
{
    ((PolarPlayer *)gActorList)->AllocTiles();
}

void JetpackReachCourseEnd(void *arg0)
{
    ((JetpackPlayer *)gActorList)->FinishRun();
}

void PolarReachCourseEnd(void *arg0)
{
    ((PolarPlayer *)gActorList)->FinishRun();
}

/* The selected category's slot 9 (its player contact test) on `self`. */
s32 IsTouchingPlayer(ActorSelf *self)
{
    return ((s32 (*)(ActorSelf *))gActorCategoryVtable->fn[9])(self);
}
