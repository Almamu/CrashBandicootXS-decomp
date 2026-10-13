#include "actor_self.hpp"
#include "vehicle.hpp"
#include "level_state.hpp"
#include "actor_spawns.hpp"
#include "actor_category.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "globals.h"
}

/* ActorSpawns::table: the category's spawn table (struct
 * actor_spawn_table, actor.h; category_descriptor.spawnTable), set up by
 * ActorCategory::Select. The per-spawn accessors below are what the homing
 * actors (AimJetpackPlane, AimPolarPenguin) use to fly to a target spawn:
 * its X/Y/Z, its kind (which picks their speed) and its next target. */

/* The frames the category has run since Select (`frameCount`, which
 * RunFrame counts): the jetpack rings' timing. */
s32 ActorCategory::GetFrameCount()
{
    return frameCount;
}

/* Trivial getter, Q8.8-converted. */
s32 ActorSpawns::GetOffset()
{
    return INT_TO_Q8(offset);
}

void ActorSpawns::Resume()
{
    paused = 0;
}

void ActorSpawns::Pause()
{
    paused = 1;
}

/* Spawn `idx`'s next target: the spawn index a homing actor goes to after
 * this one, -1 at the end of the chain. */
s32 ActorSpawns::GetNextTarget(s32 idx)
{
    return table->entries[idx].next;
}

/* Spawn `idx`'s Z: its depth, offset by `offset`, Q8.8-converted. */
s32 ActorSpawns::GetZ(s32 idx)
{
    return INT_TO_Q8(table->entries[idx].spawn.z + offset);
}

/* Spawn `idx`'s Y and X, Q8.8-converted. */
s32 ActorSpawns::GetY(s32 idx)
{
    return INT_TO_Q8(table->entries[idx].spawn.y);
}

s32 ActorSpawns::GetX(s32 idx)
{
    return INT_TO_Q8(table->entries[idx].spawn.x);
}

/* Spawn `idx`'s kind as SpawnActor picks it (`kind`, `altKind` in a time
 * trial (gLevelState+0x8c), `bonusKind` while `useBonus` is set),
 * less 0x20: the index into the homing actors' speed tables
 * (iwram_data.cpp) - see docs/rom_map.md's "sub_802A5xx siblings" entry.
 * Each kind is read as `t->entries[idx].spawn.kind`: the three share
 * the table pointer plus the scaled index, and the field offsets (8, 9,
 * 10) fold into the `ldrb`s. Through a pointer to the entry, gcc adds
 * `entries`' offset first. */
s32 ActorSpawns::GetKindIndex(s32 idx)
{
    struct actor_spawn_table *t = table;
    u8 v;

    v = t->entries[idx].spawn.kind;
    if (gLevelState->timeTrial != 0) {
        v = t->entries[idx].spawn.altKind;
    } else if (useBonus != 0) {
        v = t->entries[idx].spawn.bonusKind;
    }
    return v - ACTOR_KIND_PATH_POINT;
}
