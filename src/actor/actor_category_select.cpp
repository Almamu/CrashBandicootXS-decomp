#include "actor_self.hpp"
#include "cell_anim.hpp"
#include "actor_spawns.hpp"
#include "actor_category.hpp"

extern "C" {
#include "core.h"
#include "memory.h"
#include "actor_anim.h"
#include "actor.h"
}

using Engine::Gfx::CellAnim;

/* Sets up the currently-selected category's runtime state:
 * `ActorCategory::vtable` = `&ActorCategory::vtables[type]` (the category's
 * shared vtable, `type` being the first argument), `ActorSpawns::useBonus`
 * = the 4th argument (a variant-selector byte), `ActorSpawns::table` =
 * the 2nd argument (the category's `spawnTable`, a struct
 * actor_spawn_table, actor.h),
 * resets `ActorSpawns::index`/`ActorSpawns::paused`/`ActorSpawns::offset` to
 * 0, draws through the vtable's slot-0 function pointer
 * (its 3rd/4th arguments as x/y - the 5th argument, stack-passed, per the ROM's own
 * `ldr r6, [sp, #0x20]`), then runs a two-pass scan over the
 * spawns comparing each one's depth (`spawn.z`) plus `ActorSpawns::offset`'s
 * offset against `ActorCategory::vtable->skipDistance` (the table's +0x20
 * slot, a value): the first pass finds the last
 * entry whose adjusted threshold the vtable slot still exceeds
 * (`ActorSpawns::index`, an index into the table), the second draws
 * every entry from index 0 up to that point through the same pointer, mem_
 * alloc's a 0xc8-byte scratch buffer (`ActorCategory::drawList`), and fires
 * one more visibility check through a function pointer if the vtable's own +8 slot
 * is set, before resetting `ActorCategory::frameCount`'s call counter to 0.
 *
 * The ROM passes a 6th argument: `[sp, #0x1c]` is the 5th (handed to
 * vtable slot 2), `[sp, #0x20]` the 6th (`y`).
 *
 * The skip loop's test reads `t->entries[ActorSpawns::index].spawn.z`, and
 * `base` is declared where CellAnim::GetDistance sets it (the spawn loop's
 * test is ActorCategory::RunFrame's SPAWN_DUE, actor_category_frame.cpp). Matched in the second
 * near-miss sweep with `&ActorSpawns::index` (ROM: r7) and `base` (ROM:
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

void ActorCategory::Select(s32 type, struct actor_spawn_table *table,
                           struct anim_table_record *animTable, u8 active, s32 variant,
                           s32 checkpoint)
{
    struct actor_spawn_table *t;

    vtable = &vtables[type];
    ActorSpawns::useBonus = active;
    ActorSpawns::table = table;
    ActorSpawns::index = 0;
    ActorSpawns::paused = 0;
    ActorSpawns::offset = 0;
    vtable->createPlayer(animTable, checkpoint);
    s32 base = CellAnim::GetDistance();

    t = ActorSpawns::table;
    for (; ActorSpawns::index < t->count &&
           t->entries[ActorSpawns::index].spawn.z < vtable->skipDistance + base;
         ActorSpawns::index++)
        ;
    drawList = (ActorSelf **)mem_alloc(50 * sizeof(ActorSelf *), MEM_HEAP_IWRAM);
    if (vtable->createBoss != NULL)
        vtable->createBoss(variant);
    while (ActorSpawns::index < ActorSpawns::table->count &&
           ActorSpawns::table->entries[ActorSpawns::index].spawn.z <=
               vtable->spawnDistance + base) {
        vtable->spawn(&ActorSpawns::table->entries[ActorSpawns::index].spawn, ActorSpawns::useBonus,
                      0);
        ActorSpawns::index++;
    }
    frameCount = 0;
}
