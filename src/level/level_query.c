#include "core.h"
#include "level_state.h"
#include "audio.h"
#include "actor.h"
#include "memory.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop). Continues the
 * medal-results tally chain documented in docs/rom_map.md ("A per-level
 * completion-time cascade, and a medal-table tally chain") - see
 * docs/matching/issue-38-medal-results-tally.md for the full write-up
 * of this chunk (this first part covers the front slice, up to but not
 * including LevelHasEntityType, which follows further down this file - it's
 * contiguous with both halves in ROM, and ended up grouped with the
 * wrapper functions that call it).
 *
 * `gLevelTable` is the level table (`struct level_info`, level_data.h):
 * this file reads each level's `theme` (the gThemeMusicCues offset,
 * PlayRoomMusic) and `rooms` (its room list, CountLevelCrates).
 * `struct MedalListItem` and `struct MedalItemList` below are this file's
 * views of level_data.h's `struct level_room` and `struct level_room_list`. */

/* One entry of a `level_info.rooms` list. `linkedObj`'s own `+0x1c`
 * field is a pointer to a further nested structure (see CountCrateEntities's
 * `list` parameter in entity_flags.c) - not itself named here, since
 * only this one offset into it is read anywhere in this file. */
struct MedalListItem {
    u8 unused_00[4];
    void *linkedObj;  /* +0x04 */
    s32 type;           /* +0x08: dispatch selector - see CountLevelCrates */
    u8 unused_0c[4];
    u16 catIndex;          /* +0x10: category index passed to CountCategoryCrates */
};

struct MedalItemList {
    s32 count;                    /* +0x00 */
    struct MedalListItem **items;  /* +0x04: array of `count` item pointers */
    struct MedalListItem *extra1;   /* +0x08: single extra item, may be NULL */
    struct MedalListItem *extra2;    /* +0x0c: single extra item, may be NULL */
};

extern void *gEntityFlags;

/* Wrapper: if bit 0 of `flags` is set, tears down `self` via
 * `OperatorDelete` (the documented UI-overlay-manager-family destroy
 * call). */
void sub_802425C(void *self, s32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

void nullsub_25(void)
{
}

/* Per-level medal tally: sums, across `gLevelTable[idx]`'s
 * item list (`items[]`, plus the two extra single-item slots), a
 * per-item value - `CountCrateEntities(gEntityFlags, item->linkedObj's
 * +0x1c list)` for `type` 0-2, `CountCategoryCrates(item->catIndex)` for
 * `type == 3`, 0 otherwise (including `type < 0`). The same 4-branch
 * dispatch is inlined three times in the ROM (once per source: the
 * `items[]` array, `extra1`, `extra2`) rather than calling a shared
 * helper - `CountRoomCrates` (below) is a separate, standalone
 * instance of the same dispatch body, not something this function
 * reaches through. Each dispatch compiles as a genuine `switch` here
 * (rather than an if/else-if chain) - only that shape reproduces the
 * ROM's exact "test all three conditions inline, jump out to
 * out-of-line handler blocks" layout for this compiler. */
s32 CountLevelCrates(s32 idx)
{
    s32 total = 0;
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[idx].rooms;
    s32 i;

    for (i = 0; i < list->count; i++) {
        struct MedalListItem *item = list->items[i];
        s32 v = 0;
        s32 type = item->type;

        switch (type) {
            case 0:
            case 1:
            case 2:
                v = CountCrateEntities(gEntityFlags, *(void **)((u8 *)item->linkedObj + 0x1c));
                break;
            case 3:
                v = CountCategoryCrates(item->catIndex);
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
                v = CountCrateEntities(gEntityFlags, *(void **)((u8 *)item->linkedObj + 0x1c));
                break;
            case 3:
                v = CountCategoryCrates(item->catIndex);
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
                v = CountCrateEntities(gEntityFlags, *(void **)((u8 *)item->linkedObj + 0x1c));
                break;
            case 3:
                v = CountCategoryCrates(item->catIndex);
                break;
        }
        total += v;
    }

    return total;
}

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * the code above - see this file's header comment and
 * docs/matching/issue-38-medal-results-tally.md for the full write-up.
 * This slice picks up with LevelHasEntityType (contiguous with the
 * trailing CountLevelCrates above in ROM, so it lives here instead of getting its
 * own file - see docs/workflow.md's "one .c file per contiguous ROM
 * region" rule) and runs through SelectRoom, the last matched function
 * before the BeginSlide..ShowSlidePicture run (parked/left in
 * asm/code_3_2_17_24590.s). The `MedalListItem`/`MedalItemList`
 * structs are the ones declared at the top of this file. */

/* The level-progress record these functions take: `&level_state.level`
 * (level_state.h), so `item` is the level state's `cat`. */
struct level_progress {
    s32 level;                      // 0x00 - index into gLevelTable
    s32 itemIndex;                  // 0x04 - cursor into the entry's item list
    u8 unk_08[0x10];
    struct MedalListItem *item;     // 0x18 - the current item (SelectRoom)
};

struct AudioContext;

/* Scans `gLevelTable[idx]`'s item list (`items[]`, plus the two
 * extra single-item slots, same shape CountLevelCrates above
 * walks) for the first non-type-3 item whose `linkedObj->0x1c` nested
 * structure (see CountCrateEntities's `list` parameter) has a nonzero `u16` at
 * halfword index `flagIdx` in its own `+0x10` table pointer - i.e. "is
 * flag `flagIdx` set on any list item's flag table". Returns as soon as
 * a set flag is found (or a table read comes back nonzero), otherwise
 * 0. `LevelHasYellowGemEntity`/`34`/`40`/`4C`/`58` below are thin wrappers baking in
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
s32 LevelHasEntityType(s32 idx, s32 flagIdx)
{
    register s32 fi asm("ip") = flagIdx;
    s32 result = 0;
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[idx].rooms;
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

/* `self->item == gLevelTable[self->level].rooms->extra2` - i.e.
 * "does self's cached value (see SelectRoom) match this medal entry's
 * item list's `extra2` slot" - a sibling read of the same field
 * EnterGemPathRoom copies out. */
s32 IsInGemPathRoom(struct level_progress *self)
{
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[self->level].rooms;
    s32 result = 0;
    s32 cached = (s32)self->item;

    if (cached == (s32)list->extra2) {
        result = 1;
    }
    return result;
}

/* Same as IsInGemPathRoom but against the item list's `extra1` slot. */
s32 IsInBonusRoom(struct level_progress *self)
{
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[self->level].rooms;
    s32 result = 0;
    s32 cached = (s32)self->item;

    if (cached == (s32)list->extra1) {
        result = 1;
    }
    return result;
}

/* LevelHasYellowGemEntity/34/40/4C/58: five thin wrappers over LevelHasEntityType with
 * the medal "flag index" constant baked in (0xc, 9, 0xb, 0xa, 8 -
 * plausibly per-medal-type slot indices into the halfword table
 * LevelHasEntityType reads). */
s32 LevelHasYellowGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, 0xc);
}

s32 LevelHasBlueGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, 9);
}

s32 LevelHasGreenGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, 0xb);
}

s32 LevelHasRedGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, 0xa);
}

s32 LevelHasGemPathGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, 8);
}

/* Standalone instance of CountLevelCrates's (above) per-item
 * dispatch body, taking the item pointer directly instead of resolving
 * it from the medal table - see CountLevelCrates's comment for the shared
 * semantics. */
s32 CountRoomCrates(struct MedalListItem *item)
{
    s32 v = 0;
    s32 type = item->type;

    switch (type) {
        case 0:
        case 1:
        case 2:
            v = CountCrateEntities(gEntityFlags, *(void **)((u8 *)item->linkedObj + 0x1c));
            break;
        case 3:
            v = CountCategoryCrates(item->catIndex);
            break;
    }
    return v;
}

extern struct level_state *gLevelState;

/* Resolves which sound cue to play for a medal-results screen event:
 * `0x12` while `gLevelState`'s mode field (`+0x78`, see
 * src/objects/sprite_anim.c) is 3, `6` if `IsInBonusRoom` (the
 * item-list `extra1`-matches-cached-value check) is true, otherwise a
 * byte looked up from the per-level sound-cue-ID table
 * `gThemeMusicCues` at `gLevelTable[self->level]`'s `+0x04`
 * field (a byte offset into that table) - then plays it via
 * `PlaySong`. The `IsInBonusRoom` call's result is truncated to `u8`
 * before the nonzero test, matching this codebase's established
 * `(u8)funcCall(...) != 0` idiom for a callee whose real return value
 * is only byte-wide (see e.g. src/player/action_ctrl_moves.c). */
void PlayRoomMusic(struct level_progress *self)
{
    s32 mode = gLevelState->maskLevel;
    u32 id;

    if (mode == 3) {
        id = 0x12;
    } else if ((u8)IsInBonusRoom(self) != 0) {
        id = 6;
    } else {
        u32 offset = gLevelTable[self->level].theme;

        id = gThemeMusicCues[offset];
    }

    PlaySong(gAudioContext, id);
}

/* Advances `self->itemIndex` (a cursor into `gLevelTable[self->level]`'s
 * item list) by one if it's still below `count - 1`; returns whether
 * it advanced. */
s32 NextRoom(struct level_progress *self)
{
    s32 advanced = 0;
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[self->level].rooms;
    s32 threshold = list->count - 1;
    s32 cur = self->itemIndex;

    if (cur < threshold) {
        self->itemIndex = cur + 1;
        advanced = 1;
    }
    return advanced;
}

/* Copies `gLevelTable[self->level]`'s item list's `extra2` slot
 * into `self->item` - the write-side counterpart of IsInGemPathRoom's
 * read. */
void EnterGemPathRoom(struct level_progress *self)
{
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[self->level].rooms;

    self->item = list->extra2;
}

/* Same as EnterGemPathRoom but for the item list's `extra1` slot. */
void EnterBonusRoom(struct level_progress *self)
{
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[self->level].rooms;

    self->item = list->extra1;
}

/* If `gLevelTable[self->level]`'s item list is nonempty, caches
 * `list->items[self->itemIndex]` into `self->item`. Returns whether the list
 * was nonempty either way - the trailing `-x|x` bit-trick reproduces
 * the ROM's own idiom for a bare `return expr != 0;` (as opposed to the
 * `if (expr != 0)` earlier in the same function, which compiles as a
 * plain compare-and-branch) - see matching_decomp_register_pinning-
 * style notes in docs/matching.md for other instances of this split. */
s32 SelectRoom(struct level_progress *self)
{
    struct MedalItemList *list = (struct MedalItemList *)gLevelTable[self->level].rooms;

    if (list->count != 0) {
        s32 cur = self->itemIndex;

        self->item = list->items[cur];
    }
    {
        s32 v = list->count;

        return (u32)(-v | v) >> 31;
    }
}
