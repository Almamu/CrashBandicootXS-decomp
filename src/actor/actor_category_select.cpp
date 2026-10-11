#include "actor_self.hpp"

extern "C" {
#include "core.h"
#include "memory.h"
#include "actor_anim.h"
#include "actor.h"
}

/* Sets up the currently-selected category's runtime state:
 * `gActorCategoryVtable` = `&gActorCategoryVtables[type]` (the category's
 * shared vtable, `type` being the first argument), `gActorSpawnUseBonus`
 * = the 4th argument (a variant-selector byte), `gActorSpawnTable` =
 * the 2nd argument (the category's `spawnTable`, a struct
 * actor_spawn_table, actor.h),
 * resets `gActorSpawnIndex`/`gActorSpawnsPaused`/`gActorSpawnOffset` to
 * 0, draws the vtable's slot-0 function pointer via `_call_via_r3`
 * (arg2/arg3 as x/y - the 5th argument, stack-passed, per the ROM's own
 * `ldr r6, [sp, #0x20]`), then runs a two-pass scan over the
 * spawns comparing each one's depth (`spawn.z`) plus `gActorSpawnOffset`'s
 * offset against `gActorCategoryVtable->skipDistance` (the table's +0x20
 * slot, a value): the first pass finds the last
 * entry whose adjusted threshold the vtable slot still exceeds
 * (`gActorSpawnIndex`, an index into the table), the second draws
 * every entry from index 0 up to that point via `_call_via_r3`, mem_
 * alloc's a 0xc8-byte scratch buffer (`gActorDrawList`), and fires
 * one more `_call_via_r1` visibility check if the vtable's own +8 slot
 * is set, before resetting `gActorCategoryFrameCount`'s call counter to 0.
 *
 * The ROM passes a 6th argument: `[sp, #0x1c]` is the 5th (handed to
 * vtable slot 2), `[sp, #0x20]` the 6th (`y`).
 *
 * The skip loop's test reads `t->entries[gActorSpawnIndex].spawn.z`, and
 * `base` is declared where GetCellAnimDistance sets it (the spawn loop's
 * test is RunActorCategoryFrame's SPAWN_DUE, actor_category_frame.cpp). Matched in the second
 * near-miss sweep with `&gActorSpawnIndex` (ROM: r7) and `base` (ROM:
 * r8) swapped unless the store went through a local pointer `idx` with
 * one extra `MATCH_USE(idx)` reference; #662 rounds 2-4 found it was
 * global-alloc's ranking (references times floor_log2 over the live
 * length): the address had 4 references over 62 insns (0.129) against
 * `base`'s 7 over 105 (0.133). #662 round 5: the comma-expression test
 * (then the record's address through two locals) left fewer insns in the
 * address's life than an inline's (60, 0.1333), and the `for` puts `base`
 * at 105 insns (0.1333); on the tie global.c's allocno_compare takes the
 * lower pseudo first, and a `base` declared at its first set is created
 * after the address's pseudo. As a `while`, or with `base` declared at
 * the top, the two still swap. Read as a field of the spawn table (#855),
 * the depths need neither the locals nor an inline. */

void SelectActorCategory(s32 type, struct actor_spawn_table *table,
                         struct anim_table_record *animTable, u8 active, s32 variant,
                         s32 checkpoint)
{
    struct actor_spawn_table *t;

    gActorCategoryVtable = &gActorCategoryVtables[type];
    gActorSpawnUseBonus = active;
    gActorSpawnTable = table;
    gActorSpawnIndex = 0;
    gActorSpawnsPaused = 0;
    gActorSpawnOffset = 0;
    gActorCategoryVtable->createPlayer(animTable, checkpoint);
    s32 base = GetCellAnimDistance();

    t = gActorSpawnTable;
    for (; gActorSpawnIndex < t->count &&
           t->entries[gActorSpawnIndex].spawn.z < gActorCategoryVtable->skipDistance + base;
         gActorSpawnIndex++)
        ;
    gActorDrawList = (ActorSelf **)mem_alloc(50 * sizeof(ActorSelf *), MEM_HEAP_IWRAM);
    if (gActorCategoryVtable->createBoss != NULL)
        gActorCategoryVtable->createBoss(variant);
    while (gActorSpawnIndex < gActorSpawnTable->count &&
           gActorSpawnTable->entries[gActorSpawnIndex].spawn.z <=
               gActorCategoryVtable->spawnDistance + base) {
        gActorCategoryVtable->spawn(&gActorSpawnTable->entries[gActorSpawnIndex].spawn,
                                    gActorSpawnUseBonus, 0);
        gActorSpawnIndex++;
    }
    gActorCategoryFrameCount = 0;
}
