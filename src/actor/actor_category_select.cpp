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
 * the 2nd argument (the category's `spawnTable` array pointer,
 * see `struct sub_effect_record` in actor_spawn.cpp/include/actor_anim.h),
 * resets `gActorSpawnIndex`/`gActorSpawnsPaused`/`gActorSpawnOffset` to
 * 0, draws the vtable's slot-0 function pointer via `_call_via_r3`
 * (arg2/arg3 as x/y - the 5th argument, stack-passed, per the ROM's own
 * `ldr r6, [sp, #0x20]`), then runs a two-pass scan over the
 * `gActorSpawnTable[]` array comparing each entry's threshold field
 * (see `struct sub_effect_record.depth`) plus `gActorSpawnOffset`'s
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
 * The skip loop's test is RunActorCategoryFrame's SUB_EFFECT_DUE
 * (actor_category_frame.cpp): the next record's address built through
 * the two locals `off` and `tb`, in a comma expression, and `base` is
 * declared where GetCellAnimDistance sets it. Matched in the second
 * near-miss sweep with `&gActorSpawnIndex` (ROM: r7) and `base` (ROM:
 * r8) swapped unless the store went through a local pointer `idx` with
 * one extra `MATCH_USE(idx)` reference; #662 rounds 2-4 found it was
 * global-alloc's ranking (references times floor_log2 over the live
 * length): the address had 4 references over 62 insns (0.129) against
 * `base`'s 7 over 105 (0.133). #662 round 5: the comma-expression test
 * leaves fewer insns in the address's life than NextThreshold's inline
 * (60, 0.1333), and the `for` puts `base` at 105 insns (0.1333); on the
 * tie global.c's allocno_compare takes the lower pseudo first, and a
 * `base` declared at its first set is created after the address's
 * pseudo. As a `while`, or with `base` declared at the top, the two
 * still swap.
 * NextThreshold is actor_spawn.cpp's `GetActorSpawnZ` address shape,
 * returned as a pointer so the load lands after the limit. */

/* `table[idx + 1].depth`, with the record-boundary constant added
 * to the base before the index (same shape as actor_spawn.cpp's
 * `GetActorSpawnZ`). */
static inline s32 *NextThreshold(struct sub_effect_record *table, s32 idx)
{
    u8 *b = (u8 *)table;
    s32 off = idx * 0x14;

    b = b + 0x14;
    return (s32 *)(b + off);
}

void SelectActorCategory(s32 type, struct sub_effect_record *table,
                         struct anim_table_record *animTable, u8 active, s32 variant,
                         s32 checkpoint)
{
    struct sub_effect_record *t;
    u8 *tb;
    s32 off;

    gActorCategoryVtable = &gActorCategoryVtables[type];
    gActorSpawnUseBonus = active;
    gActorSpawnTable = table;
    gActorSpawnIndex = 0;
    gActorSpawnsPaused = 0;
    gActorSpawnOffset = 0;
    gActorCategoryVtable->createPlayer(animTable, checkpoint);
    s32 base = GetCellAnimDistance();

    t = gActorSpawnTable;
    for (; gActorSpawnIndex < t->link &&
           (off = gActorSpawnIndex * 0x14, tb = (u8 *)t + 0x14, *(s32 *)(tb + off)) <
               gActorCategoryVtable->skipDistance + base;
         gActorSpawnIndex++)
        ;
    gActorDrawList = (ActorSelf **)mem_alloc(0xc8, MEM_HEAP_IWRAM);
    if (gActorCategoryVtable->createBoss != NULL)
        gActorCategoryVtable->createBoss(variant);
    while (gActorSpawnIndex < gActorSpawnTable->link &&
           *NextThreshold(gActorSpawnTable, gActorSpawnIndex) <=
               gActorCategoryVtable->spawnDistance + base) {
        gActorCategoryVtable->spawn(
            (struct actor_spawn *)((u8 *)gActorSpawnTable + (gActorSpawnIndex * 0x14 + 8)),
            gActorSpawnUseBonus, 0);
        gActorSpawnIndex++;
    }
    gActorCategoryFrameCount = 0;
}
