#include "core.h"

/* Sets up the currently-selected category's runtime state:
 * `gUnknown_03001418` = `&gStaticData_081756C4[type]` (the category's
 * shared vtable, `type` being the first argument), `gUnknown_03001414`
 * = the 4th argument (a variant-selector byte), `gUnknown_03001400` =
 * the 2nd argument (the category's `sub_effect_table` array pointer,
 * see `struct sub_effect_record` in actor_part94.c/include/actor_anim.h),
 * resets `gUnknown_03001404`/`gUnknown_0300141C`/`gUnknown_03001420` to
 * 0, draws the vtable's slot-0 function pointer via `sub_803AD84`
 * (arg2/arg3 as x/y - the 5th argument, stack-passed, per the ROM's own
 * `ldr r6, [sp, #0x20]`), then runs a two-pass scan over the
 * `gUnknown_03001400[]` array comparing each entry's threshold field
 * (see `struct sub_effect_record.field_00`) plus `gUnknown_03001420`'s
 * offset against `gUnknown_03001418->fn[8]` (the vtable's own +0x20
 * slot, reinterpreted as a threshold): the first pass finds the last
 * entry whose adjusted threshold the vtable slot still exceeds
 * (`gUnknown_03001404`, an index into the table), the second draws
 * every entry from index 0 up to that point via `sub_803AD84`, mem_
 * alloc's a 0xc8-byte scratch buffer (`gUnknown_03001408`), and fires
 * one more `sub_803AD7C` visibility check if the vtable's own +8 slot
 * is set, before resetting `gUnknown_03001424`'s call counter to 0.
 *
 * The ROM passes a 6th argument: `[sp, #0x1c]` is the 5th (handed to
 * vtable slot 2), `[sp, #0x20]` the 6th (`y`).
 *
 * Matched in the second near-miss sweep. The old draft had the ROM's
 * instruction sequence but swapped `&gUnknown_03001404` (ROM: r7) and
 * `base` (ROM: r8): `base` had more references. The zeroing store now
 * goes through a local pointer `idx`, and `asm("" : : "r"(idx))` after
 * `sub_8029B2C` adds one reference to it. That extra-reference nudge
 * emits no code; it just makes the pointer outrank `base`. The scan
 * loops still use the global directly, which gives the ROM's loop-local
 * copies of the address. Matches under both compilers.
 * NextThreshold is actor_part94.c's `sub_802A51C` address shape,
 * returned as a pointer so the load lands after the limit. */
#include "memory.h"
#include "actor_anim.h"

asm(".set _call_via_r1, sub_803AD7C\n"
    ".set _call_via_r3, sub_803AD84");

extern const struct category_vtable *gUnknown_03001418;
extern u8 gUnknown_03001414;
extern struct sub_effect_record *gUnknown_03001400;
extern s32 gUnknown_03001404;
extern u8 gUnknown_0300141C;
extern s32 gUnknown_03001420;
extern u8 *gUnknown_03001408;
extern s32 gUnknown_03001424;
extern s32 sub_8029B2C(void);

/* `table[idx + 1].field_00`, with the record-boundary constant added
 * to the base before the index (same shape as actor_part94.c's
 * `sub_802A51C`). */
static inline s32 *NextThreshold(struct sub_effect_record *table, s32 idx)
{
    u8 *b = (u8 *)table;
    s32 off = idx * 0x14;

    b = b + 0x14;
    return (s32 *)(b + off);
}

void SelectActorCategory(s32 type, struct sub_effect_record *table, s32 x, u8 variant, s32 arg4, s32 y)
{
    struct sub_effect_record *t;
    u8 **buf;
    s32 base;
    s32 *idx;

    gUnknown_03001418 = &gStaticData_081756C4[type];
    gUnknown_03001414 = variant;
    gUnknown_03001400 = table;
    idx = &gUnknown_03001404;
    *idx = 0;
    gUnknown_0300141C = 0;
    gUnknown_03001420 = 0;
    ((void (*)(s32, s32))gUnknown_03001418->fn[0])(x, y);
    base = sub_8029B2C();
    asm("" : : "r"(idx)); /* extra reference: `idx` outranks `base` */
    t = gUnknown_03001400;
    while (gUnknown_03001404 < t->field_04
           && *NextThreshold(t, gUnknown_03001404) < (s32)gUnknown_03001418->fn[8] + base)
        gUnknown_03001404++;
    buf = &gUnknown_03001408;
    *buf = mem_alloc(0xc8, 0x80000000);
    if (gUnknown_03001418->fn[2] != NULL)
        ((void (*)(s32))gUnknown_03001418->fn[2])(arg4);
    while (gUnknown_03001404 < gUnknown_03001400->field_04
           && *NextThreshold(gUnknown_03001400, gUnknown_03001404) <= (s32)gUnknown_03001418->fn[7] + base) {
        ((void (*)(void *, s32, s32))gUnknown_03001418->fn[1])((u8 *)gUnknown_03001400 + (gUnknown_03001404 * 0x14 + 8),
                                                               gUnknown_03001414, 0);
        gUnknown_03001404++;
    }
    gUnknown_03001424 = 0;
}
