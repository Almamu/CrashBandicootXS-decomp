#include "actor_self.hpp"
#include "vehicle.hpp"
#include "level_state.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "globals.h"
}

/* gActorSpawnTable: the category's spawn table (struct actor_spawn_table,
 * actor.h; category_descriptor.spawnTable), set up by SelectActorCategory.
 * The per-spawn accessors below are what the homing actors
 * (AimJetpackPlane, AimPolarPenguin) use to fly to a target spawn: its
 * X/Y/Z, its kind (which picks their speed) and its next target. */

/* Trivial getter - the big loading-loop call counter set by
 * InitActorCategory's own loop tail (still raw). */
s32 GetActorCategoryFrameCount(void)
{
    return gActorCategoryFrameCount;
}

/* Trivial getter, Q8.8-converted. */
s32 GetActorSpawnOffset(void)
{
    return INT_TO_Q8(gActorSpawnOffset);
}

void ResumeActorSpawns(void)
{
    gActorSpawnsPaused = 0;
}

void PauseActorSpawns(void)
{
    gActorSpawnsPaused = 1;
}

/* Spawn `idx`'s next target: the spawn index a homing actor goes to after
 * this one, -1 at the end of the chain. */
s32 GetActorSpawnNextTarget(s32 idx)
{
    return gActorSpawnTable->entries[idx].next;
}

/* Spawn `idx`'s Z: its depth, offset by gActorSpawnOffset, Q8.8-converted. */
s32 GetActorSpawnZ(s32 idx)
{
    return INT_TO_Q8(gActorSpawnTable->entries[idx].spawn.z + gActorSpawnOffset);
}

/* Spawn `idx`'s Y and X, Q8.8-converted. */
s32 GetActorSpawnY(s32 idx)
{
    return INT_TO_Q8(gActorSpawnTable->entries[idx].spawn.y);
}

s32 GetActorSpawnX(s32 idx)
{
    return INT_TO_Q8(gActorSpawnTable->entries[idx].spawn.x);
}

/* Spawn `idx`'s kind as SpawnActor picks it (`kind`, `altKind` in a time
 * trial (gLevelState+0x8c), `bonusKind` while gActorSpawnUseBonus is set),
 * less 0x20: the index into the homing actors' speed tables
 * (iwram_data.cpp) - see docs/rom_map.md's "sub_802A5xx siblings" entry.
 * Each kind is read as `table->entries[idx].spawn.kind`: the three share
 * the table pointer plus the scaled index, and the field offsets (8, 9,
 * 10) fold into the `ldrb`s. Through a pointer to the entry, gcc adds
 * `entries`' offset first. */
s32 GetActorSpawnKindIndex(s32 idx)
{
    struct actor_spawn_table *table = gActorSpawnTable;
    u8 v;

    v = table->entries[idx].spawn.kind;
    if (gLevelState->timeTrial != 0) {
        v = table->entries[idx].spawn.altKind;
    } else if (gActorSpawnUseBonus != 0) {
        v = table->entries[idx].spawn.bonusKind;
    }
    return v - ACTOR_KIND_PATH_POINT;
}
