#include "vehicle.hpp"

extern "C" {
#include "core.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "level.h"
#include "globals.h"
}

/* Counts how many of category `categoryIdx`'s spawns (its
 * category_descriptor.spawnTable, struct actor_spawn_table in actor.h)
 * have a `kind` in one of two fixed sets - a different set depending on
 * the category's own `type` field. */
s32 CountCategoryCrates(s32 categoryIdx)
{
    struct actor_spawn_table *table;
    s32 total;
    s32 count;
    s32 i;

    count = 0;
    table = gActorCategories[categoryIdx].spawnTable;
    if (gActorCategories[categoryIdx].type == CATEGORY_TYPE_POLAR) {
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


void AddActorMissedNitro(void)
{
    gActorMissedNitros++;
}

s32 GetActorMissedNitros(void)
{
    return gActorMissedNitros;
}

s32 GetActorCheckpoint(void)
{
    return gActorCheckpoint;
}

/* codegen: LevelState::SetCheckpointAtPlayer takes a flag (level_state.hpp); this
 * caller passes the state only and leaves r1 as it is. docs/headers_plan.md */
extern void SetCheckpointAtPlayer_1(void *self) asm("SetCheckpointAtPlayer");

/* Re-bases the category's secondary tick counter from `arg0` (net of
 * `GetActorSpawnOffset`'s current Q8.8 offset), resets the active-instance
 * counters, and re-syncs the frame-tick snapshot for a freshly
 * (re)selected category. */
void SetActorCheckpoint(s32 arg0)
{
    gActorCheckpoint = arg0 - GetActorSpawnOffset();
    gActorCategoryDeaths = 0;
    gActorCategoryBossDeaths = 0;
    SetCheckpointAtPlayer_1(gLevelState);
    gActorCheckpointMissedNitros = gActorMissedNitros;
    SaveActorPaletteCycle();
}

/* True once the running active-instance count reaches the current
 * category's `maskAssistDeaths` threshold. The cast to `s32` matches the
 * ROM's own signed comparison (`blt`) - `maskAssistDeaths` is declared `u32`
 * in actor_anim.h (its sign isn't otherwise pinned down), and the
 * unsigned usual-arithmetic-conversion comparison that produces
 * compiles to the unsigned `bcc` instead (see docs/workflow.md
 * step 3). */
bool IsActorMaskAssistDue(void)
{
    return gActorCategoryDeaths >= (s32)gActorCategories[gActorCategory].maskAssistDeaths;
}
