#include "actor_self.hpp"

extern "C" {
#include "core.h"
#include "match.h"
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
 * Matched in the second near-miss sweep. The old draft had the ROM's
 * instruction sequence but swapped `&gActorSpawnIndex` (ROM: r7) and
 * `base` (ROM: r8): `base` had more references. The zeroing store now
 * goes through a local pointer `idx`, and `MATCH_USE(idx)` after
 * `GetCellAnimDistance` adds one reference to it. That extra-reference nudge
 * emits no code; it just makes the pointer outrank `base`. The scan
 * loops still use the global directly, which gives the ROM's loop-local
 * copies of the address. Matches under both compilers, in C and in C++
 * (#664 part 11b), and the C++ still needs the nudge; #662 round 2:
 * `idx` dropped, used in the scan loops, the reset in an inline, or a
 * `bool active` all differ. #662 round 3: global-alloc's ranking (the
 * references over the live length) decides r7 against r8; no -f flag or
 * pair of flags, and either compiler, leaves the swap.
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
    ActorSelf ***buf;
    s32 base;
    s32 *idx;

    gActorCategoryVtable = &gActorCategoryVtables[type];
    gActorSpawnUseBonus = active;
    gActorSpawnTable = table;
    idx = &gActorSpawnIndex;
    *idx = 0;
    gActorSpawnsPaused = 0;
    gActorSpawnOffset = 0;
    gActorCategoryVtable->createPlayer(animTable, checkpoint);
    base = GetCellAnimDistance();
    MATCH_USE(idx); /* extra reference: `idx` outranks `base` */
    t = gActorSpawnTable;
    while (gActorSpawnIndex < t->link &&
           *NextThreshold(t, gActorSpawnIndex) < gActorCategoryVtable->skipDistance + base)
        gActorSpawnIndex++;
    buf = &gActorDrawList;
    *buf = (ActorSelf **)mem_alloc(0xc8, MEM_HEAP_IWRAM);
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
