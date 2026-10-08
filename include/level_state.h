#ifndef GUARD_LEVEL_STATE_H
#define GUARD_LEVEL_STATE_H

/*
 * The level state's data types: a level's progress word (union
 * level_record) and the 0x68-byte progress block (struct game_progress)
 * the save slots, the menus and the level state share. The level state
 * itself (`gLevelState`, 0x1CC bytes) and its room block are C++ classes,
 * LevelState and LevelProgress (level_state.hpp); C sees `struct
 * level_state` only as an incomplete type.
 */

#include "level_data.h"
#include "constants/level_flags.h"
#include "constants/mask_level.h"

/* One level's progress word (`game_progress.levels[level]`;
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

/*
 * The 0x68-byte progress block: the head of the level state
 * (`progress`), its snapshots `checkpointData` (the last checkpoint) and
 * `saveData` (the committed progress), the block PackSaveData returns
 * to the menus (the level select, the pause menu, the power dialog and
 * the save menu's summaries, which count it: CountGems, CountCrystals,
 * GetCompletionPercent, ...) and a save slot's `progress`
 * (save_data.h). It merges level_menu.h's `struct menu_save` and the
 * raw `u8 [0x68]` blocks (#656, batch 8).
 */
struct game_progress {
    // 0x00 - the packed stats, a halfword (UnpackSaveData/PackSaveData):
    // gcc reads and writes each field through the narrowest access that
    // holds it (`lives` and `wumpa` a byte each, `maskLevel` the halfword)
    u16 lives:7;     // 0-99
    u16 maskLevel:2; // MASK_LEVEL_*
    u16 wumpa:7;     // 0-99
    // 0x02 - bits 0-3: colored gems (CountGems, DrawPauseGemsPage); bits 4-7:
    // powers (HasTurboRun, HasSuperBodySlam, HasTornadoSpin, HasDoubleJump);
    // bits 5/7/6 make level-select pages 1/2/3 reachable
    u8 flags;
    // 0x04 - one word per level, indexed by `level` (GetLevelFlags);
    // LEVEL_FLAG_* (crystal, the two clear gems, best time); five per
    // level-select page
    union level_record levels[LEVEL_COUNT];
};

COMPILE_TIME_ASSERT(level_state_h, sizeof(union level_record) == 4);
COMPILE_TIME_ASSERT(level_state_h, offsetof(struct game_progress, flags) == 0x02);
COMPILE_TIME_ASSERT(level_state_h, offsetof(struct game_progress, levels) == 0x04);
COMPILE_TIME_ASSERT(level_state_h, sizeof(struct game_progress) == 0x68);

#endif /* GUARD_LEVEL_STATE_H */
