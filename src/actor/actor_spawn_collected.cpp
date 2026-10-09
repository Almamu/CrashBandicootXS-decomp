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

/* The collected-spawn list (gCollectedSpawns, at most 15 spawns): the
 * test, the append and the reset, ROM 0x0802AA80-0x0802AB08.
 * Built with old_agbcp and -fno-implement-inlines, like actor.cpp, where
 * it was until #770. See docs/matching/archive/issue-50-actor-2a69c.md. */

/* Whether `self` is among gCollectedSpawns' first gCollectedSpawnCount
 * entries. */
s32 IsSpawnCollected(void *self)
{
    s32 i;

    for (i = 0; i < gCollectedSpawnCount; i++) {
        if (gCollectedSpawns[i] == self)
            return 1;
    }
    return 0;
}

/* Appends `self` to gCollectedSpawns (15 entries at most), unless it is
 * NULL, the list is full or it is already there. */
void MarkSpawnCollected(void *self)
{
    s32 i;

    if (gCollectedSpawnCount == 0xf || self == NULL)
        return;
    for (i = 0; i < gCollectedSpawnCount; i++) {
        if (gCollectedSpawns[i] == self)
            return;
    }
    gCollectedSpawns[gCollectedSpawnCount++] = self;
}

void ClearCollectedSpawns(void)
{
    gCollectedSpawnCount = 0;
}
