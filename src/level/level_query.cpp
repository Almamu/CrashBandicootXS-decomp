#include "audio.hpp"
extern "C" {
#include "core.h"
#include "level_state.h"
#include "actor.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
}

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop). Continues the
 * medal-results tally chain documented in docs/rom_map.md ("A per-level
 * completion-time cascade, and a medal-table tally chain") - see
 * docs/matching/archive/issue-38-medal-results-tally.md for the full write-up
 * of this chunk (this first part covers the front slice, up to but not
 * including LevelHasEntityType, which follows further down this file - it's
 * contiguous with both halves in ROM, and ended up grouped with the
 * wrapper functions that call it).
 *
 * `gLevelTable` is the level table (`struct level_info`, level_data.h):
 * this file reads each level's `theme` (the gThemeMusicCues offset,
 * PlayRoomMusic) and `rooms` (its room list, CountLevelCrates).
 * Each level's rooms are a `const struct level_room_list` of `struct
 * level_room` records (level_data.h; this file's `MedalItemList` and
 * `MedalListItem` were views of them). A room's entities are its
 * descriptor's `entities` list, whose `typeCounts` LevelHasEntityType
 * reads. */

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/): the
 * destructor (sub_802425C; g++'s deleting destructor frees `this` when
 * bit 0 of its __in_chrg is set) and the empty constructor (nullsub_25,
 * which returns `this` by leaving r0 alone) of a class with no fields and
 * no vtable that nothing creates. C++ since the #664 cleanup; the rest of
 * the file has C linkage. */
class UnusedLevelObject
{
public:
    UnusedLevelObject();  // nullsub_25
    ~UnusedLevelObject(); // sub_802425C
};

UnusedLevelObject::~UnusedLevelObject()
{
}

UnusedLevelObject::UnusedLevelObject()
{
}

/* Per-level medal tally: sums, across `gLevelTable[idx]`'s
 * item list (`rooms[]`, plus the two extra single-item slots), a
 * per-item value - `CountCrateEntities(gEntityFlags, item->desc->entities)` for
 * `kind` 0-2, `CountCategoryCrates(item->param.catIndex)` for `kind == 3`, 0
 * otherwise (including `kind < 0`). The same 4-branch
 * dispatch is inlined three times in the ROM (once per source: the
 * `rooms[]` array, `extra1`, `extra2`) rather than calling a shared
 * helper - `CountRoomCrates` (below) is a separate, standalone
 * instance of the same dispatch body, not something this function
 * reaches through. Each dispatch compiles as a genuine `switch` here
 * (rather than an if/else-if chain) - only that shape reproduces the
 * ROM's exact "test all three conditions inline, jump out to
 * out-of-line handler blocks" layout for this compiler. */
s32 CountLevelCrates(s32 idx)
{
    s32 total = 0;
    const struct level_room_list *list = gLevelTable[idx].rooms;
    s32 i;

    for (i = 0; i < list->count; i++) {
        const struct level_room *item = list->rooms[i];
        s32 v = 0;
        s32 type = item->kind;

        switch (type) {
        case ROOM_KIND_ON_FOOT:
        case ROOM_KIND_UNDERWATER:
        case ROOM_KIND_HOVER:
            v = CountCrateEntities(gEntityFlags, item->desc->entities);
            break;
        case ROOM_KIND_CATEGORY:
            v = CountCategoryCrates(item->param.catIndex);
            break;
        }
        total += v;
    }

    if (list->extra1 != 0) {
        const struct level_room *item = list->extra1;
        s32 v = 0;
        s32 type = item->kind;

        switch (type) {
        case ROOM_KIND_ON_FOOT:
        case ROOM_KIND_UNDERWATER:
        case ROOM_KIND_HOVER:
            v = CountCrateEntities(gEntityFlags, item->desc->entities);
            break;
        case ROOM_KIND_CATEGORY:
            v = CountCategoryCrates(item->param.catIndex);
            break;
        }
        total += v;
    }

    if (list->extra2 != 0) {
        const struct level_room *item = list->extra2;
        s32 v = 0;
        s32 type = item->kind;

        switch (type) {
        case ROOM_KIND_ON_FOOT:
        case ROOM_KIND_UNDERWATER:
        case ROOM_KIND_HOVER:
            v = CountCrateEntities(gEntityFlags, item->desc->entities);
            break;
        case ROOM_KIND_CATEGORY:
            v = CountCategoryCrates(item->param.catIndex);
            break;
        }
        total += v;
    }

    return total;
}

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * the code above - see this file's header comment and
 * docs/matching/archive/issue-38-medal-results-tally.md for the full write-up.
 * This slice picks up with LevelHasEntityType (contiguous with the
 * trailing CountLevelCrates above in ROM, so it lives here instead of getting its
 * own file - see docs/workflow.md's "one .c file per contiguous ROM
 * region" rule) and runs through SelectRoom, the last matched function
 * before the BeginSlide..ShowSlidePicture run (parked/left in
 * asm/code_3_2_17_24590.s). */

/* Scans `gLevelTable[idx]`'s item list (`rooms[]`, plus the two
 * extra single-item slots, same shape CountLevelCrates above
 * walks) for the first non-type-3 item whose entity list
 * (`desc->entities`) has a nonzero count of entity type `flagIdx`
 * (`typeCounts[flagIdx]`) - i.e. "does any room of the level have an
 * entity of type `flagIdx`". Returns as soon as
 * a set flag is found (or a table read comes back nonzero), otherwise
 * 0. `LevelHasYellowGemEntity`/`34`/`40`/`4C`/`58` below are thin wrappers baking in
 * a constant `flagIdx`.
 *
 * `flagIdx` stays in `ip`/r12 for the whole function, as in the ROM,
 * with no pin (old_agbcp; agbcp needs pins for the table reads). */
s32 LevelHasEntityType(s32 idx, s32 flagIdx)
{
    s32 result = 0;
    const struct level_room_list *list = gLevelTable[idx].rooms;
    s32 i = 0;

    if (result < list->count) {
        const struct level_room *const *itemPtr = list->rooms;

        do {
            const struct level_room *item = *itemPtr;

            if (item->kind != ROOM_KIND_CATEGORY) {
                result = item->desc->entities->typeCounts[flagIdx] != 0;
            }
            itemPtr++;
            i++;
        } while (i < list->count && result == 0);
    }

    if (result == 0 && list->extra1 != 0 && list->extra1->kind != ROOM_KIND_CATEGORY) {
        result = list->extra1->desc->entities->typeCounts[flagIdx] != 0;
    }

    if (result == 0 && list->extra2 != 0 && list->extra2->kind != ROOM_KIND_CATEGORY) {
        result = list->extra2->desc->entities->typeCounts[flagIdx] != 0;
    }

    return result;
}

/* `self->cat == gLevelTable[self->level].rooms->extra2` - i.e.
 * "does self's cached value (see SelectRoom) match this medal entry's
 * item list's `extra2` slot" - a sibling read of the same field
 * EnterGemPathRoom copies out. */
s32 IsInGemPathRoom(struct level_progress *self)
{
    const struct level_room_list *list = gLevelTable[self->level].rooms;
    s32 result = 0;
    s32 cached = (s32)self->cat;

    if (cached == (s32)list->extra2) {
        result = 1;
    }
    return result;
}

/* Same as IsInGemPathRoom but against the item list's `extra1` slot. */
s32 IsInBonusRoom(struct level_progress *self)
{
    const struct level_room_list *list = gLevelTable[self->level].rooms;
    s32 result = 0;
    s32 cached = (s32)self->cat;

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
    return LevelHasEntityType(idx, ENTITY_YELLOW_GEM);
}

s32 LevelHasBlueGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, ENTITY_BLUE_GEM);
}

s32 LevelHasGreenGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, ENTITY_GREEN_GEM);
}

s32 LevelHasRedGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, ENTITY_RED_GEM);
}

s32 LevelHasGemPathGemEntity(s32 idx)
{
    return LevelHasEntityType(idx, ENTITY_GEM_PATH_GEM);
}

/* Standalone instance of CountLevelCrates's (above) per-item
 * dispatch body, taking the item pointer directly instead of resolving
 * it from the medal table - see CountLevelCrates's comment for the shared
 * semantics. */
s32 CountRoomCrates(const struct level_room *item)
{
    s32 v = 0;
    s32 type = item->kind;

    switch (type) {
    case ROOM_KIND_ON_FOOT:
    case ROOM_KIND_UNDERWATER:
    case ROOM_KIND_HOVER:
        v = CountCrateEntities(gEntityFlags, item->desc->entities);
        break;
    case ROOM_KIND_CATEGORY:
        v = CountCategoryCrates(item->param.catIndex);
        break;
    }
    return v;
}

/* Resolves which sound cue to play for a medal-results screen event:
 * SONG_DRUMS while `gLevelState->maskLevel` is MASK_LEVEL_INVINCIBLE,
 * SONG_BONUS_ROUND if `IsInBonusRoom` (the
 * item-list `extra1`-matches-cached-value check) is true, otherwise a
 * byte looked up from the per-level sound-cue-ID table
 * `gThemeMusicCues` at `gLevelTable[self->level]`'s `+0x04`
 * field (a byte offset into that table) - then plays it via
 * `PlaySong`. The `IsInBonusRoom` call's result is truncated to `u8`
 * before the nonzero test, matching this codebase's established
 * `(u8)funcCall(...) != 0` idiom for a callee whose real return value
 * is only byte-wide (see e.g. src/player/action_ctrl_moves.cpp). */
void PlayRoomMusic(struct level_progress *self)
{
    s32 mode = gLevelState->maskLevel;
    u32 id;

    if (mode == MASK_LEVEL_INVINCIBLE) {
        id = SONG_DRUMS;
    } else if ((u8)IsInBonusRoom(self) != 0) {
        id = SONG_BONUS_ROUND;
    } else {
        u32 offset = gLevelTable[self->level].theme;

        id = gThemeMusicCues[offset];
    }

    gAudioContext->PlaySong(id);
}

/* Advances `self->roomIndex` (a cursor into `gLevelTable[self->level]`'s
 * item list) by one if it's still below `count - 1`; returns whether
 * it advanced. */
s32 NextRoom(struct level_progress *self)
{
    s32 advanced = 0;
    const struct level_room_list *list = gLevelTable[self->level].rooms;
    s32 threshold = list->count - 1;
    s32 cur = self->roomIndex;

    if (cur < threshold) {
        self->roomIndex = cur + 1;
        advanced = 1;
    }
    return advanced;
}

/* Copies `gLevelTable[self->level]`'s item list's `extra2` slot
 * into `self->cat` - the write-side counterpart of IsInGemPathRoom's
 * read. */
void EnterGemPathRoom(struct level_progress *self)
{
    const struct level_room_list *list = gLevelTable[self->level].rooms;

    self->cat = list->extra2;
}

/* Same as EnterGemPathRoom but for the item list's `extra1` slot. */
void EnterBonusRoom(struct level_progress *self)
{
    const struct level_room_list *list = gLevelTable[self->level].rooms;

    self->cat = list->extra1;
}

/* If `gLevelTable[self->level]`'s item list is nonempty, caches
 * `list->rooms[self->roomIndex]` into `self->cat`. Returns whether the list
 * was nonempty either way - the trailing `-x|x` bit-trick reproduces
 * the ROM's own idiom for a bare `return expr != 0;` (as opposed to the
 * `if (expr != 0)` earlier in the same function, which compiles as a
 * plain compare-and-branch) - see matching_decomp_register_pinning-
 * style notes in docs/matching.md for other instances of this split. */
s32 SelectRoom(struct level_progress *self)
{
    const struct level_room_list *list = gLevelTable[self->level].rooms;

    if (list->count != 0) {
        s32 cur = self->roomIndex;

        self->cat = list->rooms[cur];
    }
    {
        s32 v = list->count;

        return (u32)(-v | v) >> 31;
    }
}
