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

            if (type == 0x1a)
                type =
                    *(const s16 *)((const u8 *)list->params + list->paramOffsets[item->param] + 8);

            // clang-format off
            switch (type) {
            case 0x18: case 0x1a: case 0x1b: case 0x1c: case 0x1d:
                break;
            case 0x15: case 0x16: case 0x17: case 0x19:
            case 0x1e: case 0x1f: case 0x20: case 0x21: case 0x22:
            case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
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
 * bit `n` of `bits0` (floor-divided into a 32-bit-word row, same idiom as
 * `SetBitmapBit` in entity_bitmap.cpp). */
void LevelEntityFlags::SetGone(s32 n)
{
    u8 *base = (u8 *)this;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 8;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    *word |= mask;
}

/* Tests bit `n` of `bits0`, the committed "gone" set `SetEntityIdGone`
 * sets. */
s32 LevelEntityFlags::IsGone(s32 n)
{
    u8 *base = (u8 *)this;
    s32 result = 0;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 8;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    if (*word & mask) {
        result = 1;
    }
    return result;
}

/* Tests bit `n` of `bits1` (`self+0x208`), the committed "activated"
 * set. */
s32 LevelEntityFlags::IsActivated(s32 n)
{
    u8 *base = (u8 *)this;
    s32 result = 0;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 0x208;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    if (*word & mask) {
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
 * this was finally matched. This is a true leaf function in the ROM
 * (no `push`/`pop` at all - `self` lives in `ip`/`r12` for the whole
 * function, which old_agbcp does on its own). The ROM shifts the word
 * index to a byte offset before it builds `bits1`'s address
 * (`movs r1, #0x82; lsls; add r1, ip`) and adds the two: the offset is
 * its own statement and is added to the separately loaded `bits1`
 * pointer (`committed += word` would shift after the address, and
 * `&bits1[word]` adds the constant to the offset first). The copy then
 * reuses the shifted index (`live += word`). This replaced an integer
 * address pinned to r1 (#662 round 2). */
void LevelEntityFlags::SetActivated(s32 n)
{
    s32 word = n / 32;
    s32 offset = word * 4;
    u32 *committed = bits1;
    u32 *live;
    u32 mask;

    committed = (u32 *)((u8 *)committed + offset);
    mask = 1 << (n % 32);
    *committed |= mask;
    live = bits1Copy;
    live += word;
    *live |= mask;
}

/* Sets bit `n` of `bits1Copy` (`self+0x308`) only: the live "activated"
 * set, committed at the next checkpoint (ActivateIronSwitchCrate: the
 * switch itself and the outline crates it makes solid). */
void LevelEntityFlags::MarkActivated(s32 n)
{
    u8 *base = (u8 *)this;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 0x308;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    *word |= mask;
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
