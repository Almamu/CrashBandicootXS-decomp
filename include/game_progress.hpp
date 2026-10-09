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
#include "level_state.h"
}

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
