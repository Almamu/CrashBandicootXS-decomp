#include "vehicle.hpp"
#include "actor_palette_cycle.hpp"
#include "actor_category_stats.hpp"
#include "actor_spawns.hpp"
#include "actor_category.hpp"

extern "C" {
#include "core.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "level.h"
#include "globals.h"
}

using Engine::Actor::ActorCategory;
using Engine::Actor::ActorCategoryStats;
using Engine::Actor::ActorPaletteCycle;
using Engine::Actor::ActorSpawns;

/* Counts how many of category `categoryIdx`'s spawns (its
 * category_descriptor.spawnTable, struct actor_spawn_table in actor.h)
 * have a `kind` in one of two fixed sets - a different set depending on
 * the category's own `type` field. */
s32 ActorCategory::CountCrates(s32 categoryIdx)
{
    struct actor_spawn_table *table;
    s32 total;
    s32 count;
    s32 i;

    count = 0;
    table = descriptors[categoryIdx].spawnTable;
    if (descriptors[categoryIdx].type == CATEGORY_TYPE_POLAR) {
        i = 0;
        total = table->count;
        /* Manual pre-rotated `if (count<total) do {...} while (++i<total)`
         * instead of a plain `for` - gcc 2.9's own loop rotation of a
         * `for` here strength-reduces the ascending index into a
         * countdown (`sub r?,#1; cmp r?,#0`), losing the ROM's separate
         * up-counting index register and its extra callee-saved `r5`
         * (`adds r5,#1; cmp r5,r2; blt`) - see docs/workflow.md step 3. */
        if (count < total) {
            do {
                u8 v = table->entries[i].spawn.kind;
                if (v == POLAR_KIND_BASIC_CRATE || v == POLAR_KIND_CHECKPOINT_CRATE ||
                    v == POLAR_KIND_NITRO_CRATE || v == POLAR_KIND_LIFE_CRATE ||
                    v == POLAR_KIND_AKU_AKU_CRATE || v == POLAR_KIND_FOUR_WUMPA_CRATE ||
                    v == POLAR_KIND_QUESTION_CRATE_WUMPA_1 ||
                    v == POLAR_KIND_QUESTION_CRATE_WUMPA_3 ||
                    v == POLAR_KIND_QUESTION_CRATE_WUMPA_5 ||
                    v == POLAR_KIND_QUESTION_CRATE_AKU_AKU || v == POLAR_KIND_QUESTION_CRATE_LIFE) {
                    count++;
                }
                i++;
            } while (i < total);
        }
    } else {
        /* This branch's loop counts `total2` down to 0 beside the
         * index, which gcc strength-reduces into a walk over the table
         * pointer itself, the ROM's `subs r2,#1`/`adds r3,#0x14` (with
         * `while (i < total2)` it walks copies of both). A pointer to the
         * entries would add `entries`' offset (8) ahead of the loop. The
         * entry guard
         * reuses `count` (always 0 here, since this branch is only
         * reached before the type==0 branch's loop ever runs) rather
         * than testing `total2 > 0` directly - the ROM's own
         * `cmp r4,r2` compares the (zero) accumulator against the
         * count, not the count against a literal 0 (see
         * docs/workflow.md step 3). */
        s32 total2 = table->count;

        if (count < total2) {
            i = 0;
            do {
                u8 v = table->entries[i].spawn.kind;

                if ((u8)(v - JETPACK_KIND_HEALTH_CRATE) <=
                    JETPACK_KIND_QUESTION_CRATE_LIFE - JETPACK_KIND_HEALTH_CRATE) {
                    count++;
                } else {
                    /* Reads the field again: cse turns the second read
                     * into the ROM's `adds r0, r1, #0` copy of `v`. */
                    if (table->entries[i].spawn.kind == JETPACK_KIND_PARACHUTE_NITRO ||
                        table->entries[i].spawn.kind == JETPACK_KIND_UNK30) {
                        count++;
                    }
                }
                i++;
                total2--;
            } while (total2 != 0);
        }
    }

    return count;
}

void ActorCategoryStats::AddMissedNitro()
{
    missedNitros++;
}

s32 ActorCategoryStats::GetMissedNitros()
{
    return missedNitros;
}

s32 ActorCategoryStats::GetCheckpoint()
{
    return checkpoint;
}

/* codegen: LevelState::SetCheckpointAtPlayer takes a flag (level_state.hpp); this
 * caller passes the state only and leaves r1 as it is. docs/headers.md */
extern void SetCheckpointAtPlayer_1(void *self) asm("SetCheckpointAtPlayer");

/* The checkpoint at course distance `z` (net of `ActorSpawns::GetOffset`'s
 * current Q8.8 offset): resets the deaths, moves the level state's
 * checkpoint to the player, and saves the missed nitros and the palette
 * cycle to restore on a retry. */
void ActorCategoryStats::SetCheckpoint(s32 z)
{
    checkpoint = z - ActorSpawns::GetOffset();
    deaths = 0;
    bossDeaths = 0;
    SetCheckpointAtPlayer_1(gLevelState);
    checkpointMissedNitros = missedNitros;
    ActorPaletteCycle::Save();
}

/* True once the deaths since the checkpoint reach the current
 * category's `maskAssistDeaths` threshold. The ROM compares signed
 * (`blt`), so the threshold is an `s32` (actor_anim.h), as
 * LevelState's and level_info's are. */
bool ActorCategoryStats::IsMaskAssistDue()
{
    return deaths >= ActorCategory::descriptors[ActorCategory::current].maskAssistDeaths;
}
