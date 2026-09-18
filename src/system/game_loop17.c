#include "core.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop). Continues the
 * medal-results tally chain documented in docs/rom_map.md ("A per-level
 * completion-time cascade, and a medal-table tally chain") - see
 * docs/matching/issue-38-medal-results-tally.md for the full write-up
 * of this chunk (this file covers the front slice, up to but not
 * including sub_8024344, which is parked/left in
 * asm/code_3_2_17_24344.s - see that write-up for why).
 *
 * `gStaticData_0816C86C` is the confirmed 36-slot medal table (see
 * `struct threshold_table_entry` in src/graphics/oam_count.c and
 * src/graphics/settings_menu6.c) - this file's functions resolve one
 * more of that struct's `unused` bytes: `+0x20` (a pointer to a small
 * `{count, items[], extra1, extra2}` list header, see sub_8024278
 * below). Kept as this file's own local copy of the struct rather than
 * editing the other two files' already-matched copies (this project's
 * established per-translation-unit convention for this particular
 * global, see the comment on `struct threshold_table_entry` in
 * settings_menu6.c). */
struct MedalTableEntry {
    u8 unused_00[0x20];
    void *itemList; /* +0x20: -> struct MedalItemList, see sub_8024278 */
};
COMPILE_TIME_ASSERT(sizeof(struct MedalTableEntry) == 0x24);

extern struct MedalTableEntry gStaticData_0816C86C[];

/* One entry of a `MedalTableEntry.itemList`. `linkedObj`'s own `+0x1c`
 * field is a pointer to a further nested structure (see sub_8025894's
 * `list` parameter in game_loop12.c) - not itself named here, since
 * only this one offset into it is read anywhere in this file. */
struct MedalListItem {
    u8 unused_00[4];
    void *linkedObj;  /* +0x04 */
    s32 type;           /* +0x08: dispatch selector - see sub_8024278 */
    u8 unused_0c[4];
    u16 catIndex;          /* +0x10: category index passed to sub_802968C */
};

struct MedalItemList {
    s32 count;                    /* +0x00 */
    struct MedalListItem **items;  /* +0x04: array of `count` item pointers */
    struct MedalListItem *extra1;   /* +0x08: single extra item, may be NULL */
    struct MedalListItem *extra2;    /* +0x0c: single extra item, may be NULL */
};

extern void *gUnknown_030012B4;
extern s32 sub_8025894(void *self, void *list);
extern s32 sub_802968C(u16 catIndex);
extern void sub_8026ED0(void *self);

/* Wrapper: if bit 0 of `flags` is set, tears down `self` via
 * `sub_8026ED0` (the documented UI-overlay-manager-family destroy
 * call). */
void sub_802425C(void *self, s32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

void nullsub_25(void)
{
}

/* Per-level medal tally: sums, across `gStaticData_0816C86C[idx]`'s
 * item list (`items[]`, plus the two extra single-item slots), a
 * per-item value - `sub_8025894(gUnknown_030012B4, item->linkedObj's
 * +0x1c list)` for `type` 0-2, `sub_802968C(item->catIndex)` for
 * `type == 3`, 0 otherwise (including `type < 0`). The same 4-branch
 * dispatch is inlined three times in the ROM (once per source: the
 * `items[]` array, `extra1`, `extra2`) rather than calling a shared
 * helper - `sub_8024464` (game_loop18.c) is a separate, standalone
 * instance of the same dispatch body, not something this function
 * reaches through. Each dispatch compiles as a genuine `switch` here
 * (rather than an if/else-if chain) - only that shape reproduces the
 * ROM's exact "test all three conditions inline, jump out to
 * out-of-line handler blocks" layout for this compiler. */
s32 sub_8024278(s32 idx)
{
    s32 total = 0;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[idx].itemList;
    s32 i;

    for (i = 0; i < list->count; i++) {
        struct MedalListItem *item = list->items[i];
        s32 v = 0;
        s32 type = item->type;

        switch (type) {
            case 0:
            case 1:
            case 2:
                v = sub_8025894(gUnknown_030012B4, *(void **)((u8 *)item->linkedObj + 0x1c));
                break;
            case 3:
                v = sub_802968C(item->catIndex);
                break;
        }
        total += v;
    }

    if (list->extra1 != 0) {
        struct MedalListItem *item = list->extra1;
        s32 v = 0;
        s32 type = item->type;

        switch (type) {
            case 0:
            case 1:
            case 2:
                v = sub_8025894(gUnknown_030012B4, *(void **)((u8 *)item->linkedObj + 0x1c));
                break;
            case 3:
                v = sub_802968C(item->catIndex);
                break;
        }
        total += v;
    }

    if (list->extra2 != 0) {
        struct MedalListItem *item = list->extra2;
        s32 v = 0;
        s32 type = item->type;

        switch (type) {
            case 0:
            case 1:
            case 2:
                v = sub_8025894(gUnknown_030012B4, *(void **)((u8 *)item->linkedObj + 0x1c));
                break;
            case 3:
                v = sub_802968C(item->catIndex);
                break;
        }
        total += v;
    }

    return total;
}
