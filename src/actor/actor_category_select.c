#include "core.h"
#include "match.h"

/* Sets up the currently-selected category's runtime state:
 * `gActorCategoryVtable` = `&gActorCategoryVtables[type]` (the category's
 * shared vtable, `type` being the first argument), `gUnknown_03001414`
 * = the 4th argument (a variant-selector byte), `gActorSpawnTable` =
 * the 2nd argument (the category's `spawnTable` array pointer,
 * see `struct sub_effect_record` in actor_spawn.c/include/actor_anim.h),
 * resets `gActorSpawnIndex`/`gActorSpawnsPaused`/`gActorSpawnOffset` to
 * 0, draws the vtable's slot-0 function pointer via `_call_via_r3`
 * (arg2/arg3 as x/y - the 5th argument, stack-passed, per the ROM's own
 * `ldr r6, [sp, #0x20]`), then runs a two-pass scan over the
 * `gActorSpawnTable[]` array comparing each entry's threshold field
 * (see `struct sub_effect_record.field_00`) plus `gActorSpawnOffset`'s
 * offset against `gActorCategoryVtable->fn[8]` (the vtable's own +0x20
 * slot, reinterpreted as a threshold): the first pass finds the last
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
 * copies of the address. Matches under both compilers.
 * NextThreshold is actor_spawn.c's `sub_802A51C` address shape,
 * returned as a pointer so the load lands after the limit. */
#include "memory.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"

/* `table[idx + 1].field_00`, with the record-boundary constant added
 * to the base before the index (same shape as actor_spawn.c's
 * `sub_802A51C`). */
static inline s32 *NextThreshold(struct sub_effect_record *table, s32 idx)
{
    u8 *b = (u8 *)table;
    s32 off = idx * 0x14;

    b = b + 0x14;
    return (s32 *)(b + off);
}

void SelectActorCategory(s32 type, struct sub_effect_record *table, void *animTable, u8 active, s32 variant, s32 checkpoint)
{
    struct sub_effect_record *t;
    struct actor_self ***buf;
    s32 base;
    s32 *idx;

    gActorCategoryVtable = &gActorCategoryVtables[type];
    gUnknown_03001414 = active;
    gActorSpawnTable = table;
    idx = &gActorSpawnIndex;
    *idx = 0;
    gActorSpawnsPaused = 0;
    gActorSpawnOffset = 0;
    ((void (*)(void *, s32))gActorCategoryVtable->fn[0])(animTable, checkpoint);
    base = GetCellAnimDistance();
    MATCH_USE(idx); /* extra reference: `idx` outranks `base` */
    t = gActorSpawnTable;
    while (gActorSpawnIndex < t->field_04
           && *NextThreshold(t, gActorSpawnIndex) < (s32)gActorCategoryVtable->fn[8] + base)
        gActorSpawnIndex++;
    buf = &gActorDrawList;
    *buf = mem_alloc(0xc8, 0x80000000);
    if (gActorCategoryVtable->fn[2] != NULL)
        ((void (*)(s32))gActorCategoryVtable->fn[2])(variant);
    while (gActorSpawnIndex < gActorSpawnTable->field_04
           && *NextThreshold(gActorSpawnTable, gActorSpawnIndex) <= (s32)gActorCategoryVtable->fn[7] + base) {
        ((void (*)(void *, s32, s32))gActorCategoryVtable->fn[1])((u8 *)gActorSpawnTable + (gActorSpawnIndex * 0x14 + 8),
                                                               gUnknown_03001414, 0);
        gActorSpawnIndex++;
    }
    gActorCategoryFrameCount = 0;
}
