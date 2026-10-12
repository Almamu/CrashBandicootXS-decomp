#include "spawners.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "memory.h"
#include "level.h"
}

/* The room's entity flags (LevelEntityFlags, include/entity_flags.hpp;
 * gEntityFlags): the "gone" and "activated" bitmaps by entity id, and the
 * crate count of a room's entity list. C++ since the #664 cleanup, built
 * with old_agbcp (current agbcc was the C's); the methods keep their C
 * names (cxx_symbols.txt); the callers call them as methods of
 * gEntityFlags (#762). */

/* GitHub issue #41: 0x08025894-0x08025FC8. Counts, across every group
 * in `list` and every entity in each group,
 * how many items have an "effective type" that falls in
 * `[0x15, 0x27]` but is not one of `{0x18, 0x1a, 0x1b, 0x1c, 0x1d}`
 * (the ROM's jump table sends those five cases to the no-increment
 * path, everything else in range to the increment path, anything
 * outside the range skips the table read entirely via the `bhi`
 * short-circuit). An item's `type` is used directly unless it's `0x1a`,
 * in which case the effective type is instead looked up indirectly:
 * `list->paramOffsets[item->param]` gives a byte offset into
 * `list->params`, and the effective type is the `s16` eight bytes past
 * that.
 *
 * Under old_agbcp (the Makefile's OLD_AGBCC_OBJS) the plain lookup
 * matches: under agbcc the C needed the lookup as an `asm volatile` block
 * and `i`'s init split in two (docs/matching/archive/issue-41-game-loop-25894.md). */
s32 LevelEntityFlags::CountCrateEntities(const struct level_entity_list *list)
{
    s32 count = 0;
    s32 i;

    for (i = list->groupCount - 1; i >= 0; i--) {
        const struct level_entity_group *group = &list->groups[i];
        s32 j;

        for (j = 0; j < group->count; j++) {
            const struct level_entity *item = &group->entities[j];
            s32 type = item->type;

            if (type == ENTITY_OUTLINE_CRATE)
                type = *(const s16 *)(list->params + list->paramOffsets[item->param] + 8);

            // clang-format off
            switch (type) {
            case ENTITY_IRON_SWITCH_CRATE: case ENTITY_OUTLINE_CRATE: case ENTITY_NITRO_SWITCH_CRATE:
            case ENTITY_IRON_CRATE: case ENTITY_IRON_ARROW_CRATE:
                break;
            case ENTITY_BASIC_CRATE: case ENTITY_CHECKPOINT_CRATE: case ENTITY_AKU_AKU_CRATE:
            case ENTITY_ARROW_CRATE: case ENTITY_LIFE_CRATE: case ENTITY_NITRO_CRATE:
            case ENTITY_MYSTERY_CRATE: case ENTITY_BOUNCY_WUMPA_CRATE: case ENTITY_REINFORCED_CRATE:
            case ENTITY_TNT_CRATE: case ENTITY_SLOT_CRATE:
            case ENTITY_TIME_CRATE_1: case ENTITY_TIME_CRATE_2: case ENTITY_TIME_CRATE_3:
                count++;
                break;
            }
            // clang-format on
        }
    }
    return count;
}

/* The bit accessors of LevelEntityFlags (entity_flags.hpp), by entity id `n`:
 * `bits0` is the committed "gone" set (collected, broken or killed;
 * SpawnRoomEntities skips those) and `bits1` the committed "activated"
 * set (a checkpoint, life, "?" or slot crate opened, an iron switch
 * crate pressed and the outline crates it made solid; CreateCrate builds
 * those in their used state). `bits0Copy`/`bits1Copy` are the live copies, committed back at a
 * checkpoint: "Set" writes the committed set and "Mark" only the live
 * copy (like MarkEntityGone's bits0Copy write).
 *
 * UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Sets
 * bit `n` of `bits0` (word `n / 32`, the same idiom as `SetBitmapBit` in
 * entity_bitmap.cpp). */
void LevelEntityFlags::SetGone(s32 n)
{
    s32 word = n / 32;

    bits0[word] |= 1 << (n % 32);
}

/* Tests bit `n` of `bits0`, the committed "gone" set `SetEntityIdGone`
 * sets. */
bool LevelEntityFlags::IsGone(s32 n)
{
    s32 result = 0;

    if (bits0[n / 32] & (1 << (n % 32))) {
        result = 1;
    }
    return result;
}

/* Tests bit `n` of `bits1` (`self+0x208`), the committed "activated"
 * set. */
bool LevelEntityFlags::IsActivated(s32 n)
{
    s32 result = 0;

    if (bits1[n / 32] & (1 << (n % 32))) {
        result = 1;
    }
    return result;
}

/* Sets bit `n` in *both* `bits1` (`self+0x208`) and `bits1Copy`
 * (`self+0x308`): the activation is committed at once, so it survives a
 * death before the next checkpoint (the checkpoint, life, "?" and slot
 * crates).
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/archive/naked-sub_80259d4-matched.md for the derivation of how
 * this was first matched. This is a true leaf function in the ROM
 * (no `push`/`pop` at all - `self` lives in `ip`/`r12` for the whole
 * function, which old_agbcp does on its own). Indexing the two arrays
 * directly gives the ROM's code: the shifted word index is computed once
 * and added to each array's address (#855; it was a byte-offset pointer
 * before). */
void LevelEntityFlags::SetActivated(s32 n)
{
    s32 word = n / 32;

    bits1[word] |= 1 << (n % 32);
    bits1Copy[word] |= 1 << (n % 32);
}

/* Sets bit `n` of `bits1Copy` (`self+0x308`) only: the live "activated"
 * set, committed at the next checkpoint (ActivateIronSwitchCrate: the
 * switch itself and the outline crates it makes solid). */
void LevelEntityFlags::MarkActivated(s32 n)
{
    bits1Copy[n / 32] |= 1 << (n % 32);
}

/* UNUSED - no caller anywhere in the ROM (checked src/ and asm/). Stores
 * the Q8 `val` as pixels in `pos` (`self+4`), as SpawnRoomEntities does. */
void LevelEntityFlags::SetPos(s32 val)
{
    pos = Q8_TO_INT(val);
}

/* DestroyEntityFlags: nothing to tear down; g++'s deleting destructor
 * frees the object when bit 0 of its __in_chrg is set (the same shape as
 * EntitySpawner's, entity_spawner.cpp). */
LevelEntityFlags::~LevelEntityFlags()
{
}

/* InitEntityFlags: clears `list` and `pos`. */
LevelEntityFlags::LevelEntityFlags()
{
    list = 0;
    pos = 0;
}
