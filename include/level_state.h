#ifndef GUARD_LEVEL_STATE_H
#define GUARD_LEVEL_STATE_H

/*
 * The level state's data types: a level's progress word (union
 * level_record), which the 0x68-byte progress block (C++, class
 * GameProgress, game_progress.hpp) holds one of per level. The level state
 * itself (`gLevelState`, 0x1CC bytes) and its room block are C++ classes,
 * LevelState and LevelProgress (level_state.hpp); C sees `struct
 * level_state` only as an incomplete type.
 */

#include "level_data.h"
#include "constants/level_flags.h"
#include "constants/mask_level.h"

/* One level's progress word (`GameProgress::levels[level]`;
 * constants/level_flags.h, LEVEL_FLAG_*). */
struct level_save {
    u32 cleared:1; // LEVEL_FLAG_CRYSTAL
    u32 flag1:1;   // LEVEL_FLAG_CRATE_GEM
    u32 flag2:1;   // LEVEL_FLAG_GEM_PATH_GEM
    u32 time:13;   // best time, tenths of a second (0 = none; UpdateGameFrame)
    // bits 16-31: nothing uses them (the u32 bitfield word sizes the struct)
};

/* The same word with halfword bitfields: the level select's reads load it
 * with `ldrh` (byte 2 of the progress block itself holds four more flags
 * LoadLevelSelectRecord tests). */
struct level_save_h {
    u16 cleared:1;
    u16 flag1:1;
    u16 flag2:1;
    u16 time:13; // best time, tenths of a second (0 = none; UpdateGameFrame)
};

/* The same word, read a byte at a time. (No `u8 [3]` for the upper bytes:
 * an array member would give this struct, and so union level_record,
 * BLKmode, and UpdateGameFrame's word read of `w.time` (`ldr`, then two
 * shifts) would become an `ldrh`.) */
struct level_save_b {
    u8 cleared:1;
    u8 flag1:1;
    u8 flag2:1;
};

/* A level's progress word, read through whichever access width its user
 * needs. It merges game_frame.cpp's `union level_best_time` (`f` was `w`,
 * `raw` is `low`; #656, batch 8). */
union level_record {
    struct level_save w;
    struct level_save_h h;
    struct level_save_b b;
    // the low halfword as a whole: UpdateGameFrame's "no best time yet"
    // test (`low & LEVEL_FLAG_TIME_MASK`) reads it as one
    u16 low;
};

COMPILE_TIME_ASSERT(level_state_h, sizeof(union level_record) == 4);

#endif /* GUARD_LEVEL_STATE_H */
