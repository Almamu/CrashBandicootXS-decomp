#ifndef GUARD_GAME_PROGRESS_HPP
#define GUARD_GAME_PROGRESS_HPP

/* The 0x68-byte progress block as C++ (#766, docs/cplusplus.md, "The
 * progress block as a class"): class GameProgress, until #766
 * level_state.h's struct game_progress. Its count functions are const
 * methods (src/save/game_progress.cpp) under their C names
 * (cxx_symbols.txt). No C file reads the layout (src/data/ and lib/ never
 * name it), so the fields are the class's.
 *
 * No vtable, constructor or destructor: the level state, its snapshots
 * and the save slots hold it by value, copied with MemCopy32.
 *
 * `#pragma interface`: no vtable to emit, and no out-of-line copies of
 * inline methods. */
#pragma interface

extern "C" {
#include "core.h"
#include "level_data.h"
#include "constants/level_flags.h"
#include "constants/mask_level.h"
}

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

COMPILE_TIME_ASSERT(game_progress_hpp, sizeof(union level_record) == 4);

/*
 * The progress block: the head of the level state (`progress`), its
 * snapshots `checkpointData` (the last checkpoint) and `saveData` (the
 * committed progress), the block PackSaveData returns to the menus (the
 * level select, the pause menu, the power dialog and the save menu's
 * summaries, which count it: CountGems, CountCrystals,
 * GetCompletionPercent, ...) and a save slot's `progress`
 * (save_data.hpp). It merges level_menu.h's `struct menu_save` and the
 * raw `u8 [0x68]` blocks (#656, batch 8).
 */
class GameProgress
{
public:
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

    /* src/save/game_progress.cpp */
    s32 GetLives() const;             // GetProgressLives
    s32 CountPlatinumRelics() const;  // CountPlatinumRelics
    s32 CountGoldRelics() const;      // CountGoldRelics
    s32 CountSapphireRelics() const;  // CountSapphireRelics
    s32 CountRelics() const;          // CountRelics
    s32 CountGems() const;            // CountGems
    s32 CountClearGems() const;       // CountClearGems
    s32 CountCrystals() const;        // CountCrystals
    s32 GetCompletionPercent() const; // GetCompletionPercent
};

COMPILE_TIME_ASSERT(game_progress_hpp, offsetof(GameProgress, flags) == 0x02);
COMPILE_TIME_ASSERT(game_progress_hpp, offsetof(GameProgress, levels) == 0x04);
COMPILE_TIME_ASSERT(game_progress_hpp, sizeof(GameProgress) == 0x68);

#endif /* !GUARD_GAME_PROGRESS_HPP */
