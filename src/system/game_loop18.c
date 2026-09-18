#include "core.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * game_loop17.c - see that file's header comment and
 * docs/matching/issue-38-medal-results-tally.md for the full write-up.
 * This slice picks up with sub_8024344 (contiguous with game_loop17.c's
 * trailing sub_8024278 in ROM, so it lives here instead of getting its
 * own file - see docs/workflow.md's "one .c file per contiguous ROM
 * region" rule) and runs through sub_802455C, the last matched function
 * before the sub_8024590..sub_8024708 run (parked/left in
 * asm/code_3_2_17_24590.s). */
struct MedalTableEntry {
    u8 unused_00[4];
    u32 cueTableOffset; /* +0x04: byte offset into gStaticData_0816CD80, see sub_8024498 below */
    u8 unused_08[0x18];
    void *itemList; /* +0x20: -> struct MedalItemList, see game_loop17.c's sub_8024278 */
};
COMPILE_TIME_ASSERT(sizeof(struct MedalTableEntry) == 0x24);

extern struct MedalTableEntry gStaticData_0816C86C[];

struct MedalListItem {
    u8 unused_00[4];
    void *linkedObj;  /* +0x04 */
    s32 type;           /* +0x08 */
    u8 unused_0c[4];
    u16 catIndex;          /* +0x10 */
};

struct MedalItemList {
    s32 count;                    /* +0x00 */
    struct MedalListItem **items;  /* +0x04 */
    struct MedalListItem *extra1;   /* +0x08 */
    struct MedalListItem *extra2;    /* +0x0c */
};

extern void *gUnknown_030012B4;
extern s32 sub_8025894(void *self, void *list);
extern s32 sub_802968C(u16 catIndex);

struct AudioContext;

/* Scans `gStaticData_0816C86C[idx]`'s item list (`items[]`, plus the two
 * extra single-item slots, same shape sub_8024278 in game_loop17.c
 * walks) for the first non-type-3 item whose `linkedObj->0x1c` nested
 * structure (see sub_8025894's `list` parameter) has a nonzero `u16` at
 * halfword index `flagIdx` in its own `+0x10` table pointer - i.e. "is
 * flag `flagIdx` set on any list item's flag table". Returns as soon as
 * a set flag is found (or a table read comes back nonzero), otherwise
 * 0. `sub_8024428`/`34`/`40`/`4C`/`58` below are thin wrappers baking in
 * a constant `flagIdx`.
 *
 * `flagIdx` is kept in `ip`/r12 for the whole function (via the
 * `register ... asm("ip")` pin) rather than a normally-allocated
 * register, matching the ROM's own register-pressure-driven choice -
 * without the pin, gcc allocates it to a plain low register instead and
 * the codegen diverges throughout. The `table`/`v` locals in each of
 * the three (loop, extra1, extra2) flag-table reads also need explicit
 * register pins (and, for `table`, a plain-integer type instead of a
 * pointer type) to reproduce the ROM's exact register choice and
 * operand order for the `table + shift` address computation - without
 * the plain-integer type, C's usual pointer-arithmetic canonicalization
 * (which always puts the pointer operand first) overrides the source
 * order regardless of how the addition is written, so `shift + table`
 * and `table + shift` compiled identically until `table` stopped being
 * a pointer type. See docs/workflow.md's register-pinning techniques
 * and docs/matching.md for other instances of this class of fix. */
s32 sub_8024344(s32 idx, s32 flagIdx)
{
    register s32 fi asm("ip") = flagIdx;
    s32 result = 0;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[idx].itemList;
    s32 i = 0;

    if (result < list->count) {
        struct MedalListItem **itemPtr = list->items;
        s32 shift = fi << 1;
        s32 n = list->count;

        do {
            struct MedalListItem *item = *itemPtr;

            if (item->type != 3) {
                void *nested = *(void **)((u8 *)item->linkedObj + 0x1c);
                register u32 table asm("r1") = (u32)*(u8 **)((u8 *)nested + 0x10);
                register u16 v asm("r3") = *(u16 *)(shift + table);

                result = (u32)(-(s32)v | v) >> 31;
            }
            itemPtr++;
            i++;
        } while (i < n && result == 0);
    }

    if (result == 0 && list->extra1 != 0 && list->extra1->type != 3) {
        void *nested = *(void **)((u8 *)list->extra1->linkedObj + 0x1c);
        register u32 table asm("r0") = (u32)*(u8 **)((u8 *)nested + 0x10);
        s32 shift = fi << 1;
        register u16 v asm("r3") = *(u16 *)(shift + table);

        result = (u32)(-(s32)v | v) >> 31;
    }

    if (result == 0 && list->extra2 != 0 && list->extra2->type != 3) {
        void *nested = *(void **)((u8 *)list->extra2->linkedObj + 0x1c);
        register u32 table asm("r0") = (u32)*(u8 **)((u8 *)nested + 0x10);
        s32 shift = fi << 1;
        register u16 v asm("r3") = *(u16 *)(shift + table);

        result = (u32)(-(s32)v | v) >> 31;
    }

    return result;
}

/* `self->0x18 == gStaticData_0816C86C[self->0].itemList->extra2` - i.e.
 * "does self's cached value (see sub_802455C) match this medal entry's
 * item list's `extra2` slot" - a sibling read of the same field
 * sub_8024524 copies out. */
s32 sub_80243E0(void *self)
{
    u8 *s = (u8 *)self;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[*(s32 *)s].itemList;
    s32 result = 0;
    s32 cached = *(s32 *)(s + 0x18);

    if (cached == (s32)list->extra2) {
        result = 1;
    }
    return result;
}

/* Same as sub_80243E0 but against the item list's `extra1` slot. */
s32 sub_8024404(void *self)
{
    u8 *s = (u8 *)self;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[*(s32 *)s].itemList;
    s32 result = 0;
    s32 cached = *(s32 *)(s + 0x18);

    if (cached == (s32)list->extra1) {
        result = 1;
    }
    return result;
}

/* sub_8024428/34/40/4C/58: five thin wrappers over sub_8024344 with
 * the medal "flag index" constant baked in (0xc, 9, 0xb, 0xa, 8 -
 * plausibly per-medal-type slot indices into the halfword table
 * sub_8024344 reads). */
s32 sub_8024428(s32 idx)
{
    return sub_8024344(idx, 0xc);
}

s32 sub_8024434(s32 idx)
{
    return sub_8024344(idx, 9);
}

s32 sub_8024440(s32 idx)
{
    return sub_8024344(idx, 0xb);
}

s32 sub_802444C(s32 idx)
{
    return sub_8024344(idx, 0xa);
}

s32 sub_8024458(s32 idx)
{
    return sub_8024344(idx, 8);
}

/* Standalone instance of sub_8024278's (game_loop17.c) per-item
 * dispatch body, taking the item pointer directly instead of resolving
 * it from the medal table - see sub_8024278's comment for the shared
 * semantics. */
s32 sub_8024464(struct MedalListItem *item)
{
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
    return v;
}

extern void *gUnknown_030012C0;
extern void *gUnknown_030012BC;
extern void sub_8001B54(struct AudioContext *self, u32 id);
extern u8 gStaticData_0816CD80[];

/* Resolves which sound cue to play for a medal-results screen event:
 * `0x12` while `gUnknown_030012C0`'s mode field (`+0x78`, see
 * src/graphics/actor_part7.c) is 3, `6` if `sub_8024404` (the
 * item-list `extra1`-matches-cached-value check) is true, otherwise a
 * byte looked up from the per-level sound-cue-ID table
 * `gStaticData_0816CD80` at `gStaticData_0816C86C[self->0]`'s `+0x04`
 * field (a byte offset into that table) - then plays it via
 * `sub_8001B54`. The `sub_8024404` call's result is truncated to `u8`
 * before the nonzero test, matching this codebase's established
 * `(u8)funcCall(...) != 0` idiom for a callee whose real return value
 * is only byte-wide (see e.g. src/graphics/actor_part38c.c). */
void sub_8024498(void *self)
{
    s32 mode = *(s32 *)((u8 *)gUnknown_030012C0 + 0x78);
    u32 id;

    if (mode == 3) {
        id = 0x12;
    } else if ((u8)sub_8024404(self) != 0) {
        id = 6;
    } else {
        u32 offset = gStaticData_0816C86C[*(s32 *)self].cueTableOffset;

        id = gStaticData_0816CD80[offset];
    }

    sub_8001B54(gUnknown_030012BC, id);
}

/* Advances `self->4` (a cursor into `gStaticData_0816C86C[self->0]`'s
 * item list) by one if it's still below `count - 1`; returns whether
 * it advanced. */
s32 sub_80244F0(void *self)
{
    u8 *s = (u8 *)self;
    s32 advanced = 0;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[*(s32 *)s].itemList;
    s32 threshold = list->count - 1;
    s32 cur = *(s32 *)(s + 4);

    if (cur < threshold) {
        *(s32 *)(s + 4) = cur + 1;
        advanced = 1;
    }
    return advanced;
}

/* Copies `gStaticData_0816C86C[self->0]`'s item list's `extra2` slot
 * into `self->0x18` - the write-side counterpart of sub_80243E0's
 * read. */
void sub_8024524(void *self)
{
    u8 *s = (u8 *)self;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[*(s32 *)s].itemList;

    *(s32 *)(s + 0x18) = (s32)list->extra2;
}

/* Same as sub_8024524 but for the item list's `extra1` slot. */
void sub_8024540(void *self)
{
    u8 *s = (u8 *)self;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[*(s32 *)s].itemList;

    *(s32 *)(s + 0x18) = (s32)list->extra1;
}

/* If `gStaticData_0816C86C[self->0]`'s item list is nonempty, caches
 * `list->items[self->4]` into `self->0x18`. Returns whether the list
 * was nonempty either way - the trailing `-x|x` bit-trick reproduces
 * the ROM's own idiom for a bare `return expr != 0;` (as opposed to the
 * `if (expr != 0)` earlier in the same function, which compiles as a
 * plain compare-and-branch) - see matching_decomp_register_pinning-
 * style notes in docs/matching.md for other instances of this split. */
s32 sub_802455C(void *self)
{
    u8 *s = (u8 *)self;
    struct MedalItemList *list = (struct MedalItemList *)gStaticData_0816C86C[*(s32 *)s].itemList;

    if (list->count != 0) {
        s32 cur = *(s32 *)(s + 4);

        *(s32 *)(s + 0x18) = (s32)list->items[cur];
    }
    {
        s32 v = list->count;

        return (u32)(-v | v) >> 31;
    }
}
