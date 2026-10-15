#include "game/entities/actor_self.hpp"
#include "game/vehicles/vehicle.hpp"
#include "engine/actor/collected_spawns.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "match.h"
#include <libgcc.h>
#include "engine/actor/actor.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
}

using Engine::Actor::CollectedSpawns;

/* The collected-spawn list (CollectedSpawns, at most 15 spawns): the
 * test, the append and the reset, ROM 0x0802AA80-0x0802AB08.
 * Built with old_agbcp and -fno-implement-inlines, like actor_self.cpp (actor.cpp then), where
 * it was until #770. See docs/matching/archive/issue-50-actor-2a69c.md. */

/* Whether `spawn` is among the first `count` of `spawns`. */
bool CollectedSpawns::Contains(struct actor_spawn *spawn)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if (spawns[i] == spawn)
            return 1;
    }
    return 0;
}

/* Appends `spawn` to `spawns` (15 entries at most), unless it is
 * NULL, the list is full or it is already there. */
void CollectedSpawns::Add(struct actor_spawn *spawn)
{
    s32 i;

    if (count == 0xf || spawn == NULL)
        return;
    for (i = 0; i < count; i++) {
        if (spawns[i] == spawn)
            return;
    }
    spawns[count++] = spawn;
}

void CollectedSpawns::Clear()
{
    count = 0;
}
