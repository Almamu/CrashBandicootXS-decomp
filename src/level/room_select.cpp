#include "audio.hpp"
#include "level_state.hpp"
extern "C" {
#include "core.h"
#include "actor.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
}

/* The room cursor of the level state's room block (LevelProgress,
 * include/level_state.hpp): the room's music, the next room, the gem
 * path and bonus rooms and SelectRoom. ROM 0x08024498-0x08024590, the end
 * of GitHub issue #38's chunk; split from level_query.cpp (#770). Built
 * with old_agbcp. See docs/matching/archive/issue-38-medal-results-tally.md. */

/* Resolves which sound cue to play for a medal-results screen event:
 * SONG_DRUMS while `gLevelState->maskLevel` is MASK_LEVEL_INVINCIBLE,
 * SONG_BONUS_ROUND if `IsInBonusRoom` (the
 * item-list `extra1`-matches-cached-value check) is true, otherwise a
 * byte looked up from the per-level sound-cue-ID table
 * `gThemeMusicCues` at `gLevelTable[level]`'s `+0x04`
 * field (a byte offset into that table) - then plays it via
 * `PlaySong`. The `IsInBonusRoom` call's result is truncated to `u8`
 * before the nonzero test, matching this codebase's established
 * `(u8)funcCall(...) != 0` idiom for a callee whose real return value
 * is only byte-wide (see e.g. src/player/action_ctrl_moves.cpp). */
void LevelProgress::PlayRoomMusic()
{
    s32 mode = gLevelState->maskLevel;
    u32 id;

    if (mode == MASK_LEVEL_INVINCIBLE) {
        id = SONG_DRUMS;
    } else if ((u8)IsInBonusRoom() != 0) {
        id = SONG_BONUS_ROUND;
    } else {
        u32 offset = gLevelTable[level].theme;

        id = gThemeMusicCues[offset];
    }

    gAudioContext->PlaySong(id);
}

/* Advances `roomIndex` (a cursor into `gLevelTable[level]`'s
 * item list) by one if it's still below `count - 1`; returns whether
 * it advanced. */
s32 LevelProgress::NextRoom()
{
    s32 advanced = 0;
    const struct level_room_list *list = gLevelTable[level].rooms;
    s32 threshold = list->count - 1;
    s32 cur = roomIndex;

    if (cur < threshold) {
        roomIndex = cur + 1;
        advanced = 1;
    }
    return advanced;
}

/* Copies `gLevelTable[level]`'s item list's `extra2` slot
 * into `cat` - the write-side counterpart of IsInGemPathRoom's
 * read. */
void LevelProgress::EnterGemPathRoom()
{
    const struct level_room_list *list = gLevelTable[level].rooms;

    cat = list->extra2;
}

/* Same as EnterGemPathRoom but for the item list's `extra1` slot. */
void LevelProgress::EnterBonusRoom()
{
    const struct level_room_list *list = gLevelTable[level].rooms;

    cat = list->extra1;
}

/* If `gLevelTable[level]`'s item list is nonempty, caches
 * `list->rooms[roomIndex]` into `cat`. Returns whether the list
 * was nonempty either way - the trailing `-x|x` bit-trick reproduces
 * the ROM's own idiom for a bare `return expr != 0;` (as opposed to the
 * `if (expr != 0)` earlier in the same function, which compiles as a
 * plain compare-and-branch) - see matching_decomp_register_pinning-
 * style notes in docs/matching.md for other instances of this split. */
s32 LevelProgress::SelectRoom()
{
    const struct level_room_list *list = gLevelTable[level].rooms;

    if (list->count != 0) {
        s32 cur = roomIndex;

        cat = list->rooms[cur];
    }
    {
        s32 v = list->count;

        return (u32)(-v | v) >> 31;
    }
}
