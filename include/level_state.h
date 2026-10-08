#ifndef GUARD_LEVEL_STATE_H
#define GUARD_LEVEL_STATE_H

/*
 * The level/session state object `gLevelState` points at (0x1CC
 * bytes). Its accessor family is src/level/level_state.cpp
 * (FreezeLevelClock through CheckAllCratesBroken); the level loop
 * (UpdateGameFrame, game_frame.cpp) and the bonus-round/checkpoint
 * functions (bonus_round.c) use it too.
 *
 * The first 0x68 bytes are the per-attempt block the frame loop
 * snapshots into `checkpointData`/`saveData` and restores from (game_frame.cpp).
 */

#include "level_data.h"
#include "constants/level_flags.h"
#include "constants/mask_level.h"
#include "constants/packed_stats.h"

struct level_state_1c8;

/*
 * The room block of `struct level_state` (`room`, +0x0C4): the record the
 * room functions take (game_frame.c passes `&gLevelState->room`):
 * SelectRoom, NextRoom, EnterBonusRoom and the other level_query.c
 * functions, PlayRoom (play_room.c), RunRoom (run_room.c),
 * ResumeRoomAfterPause (room.c), UpdateRoomFrame and SetupRoomBlend
 * (room_frame.c). It merges the four file-local views `level_progress`
 * (level_query.c), `level_start_args` (play_room.c; `spawnX`/`spawnY`
 * were `checkpointX`/`checkpointY`), `gl_self` (run_room.c; `widget` was
 * `cat`) (#574, batch 9e) and room_frame.cpp's `level_ctx` (`blend` was
 * `cat`; #656, batch 7). Offsets in brackets are the level state's.
 */
struct level_progress {
    s32 level; // 0x00 (0x0C4) - index into gLevelTable
    // 0x04 (0x0C8) - the current room's index in the level's room list
    // (SelectRoom, NextRoom, GetRoomIndex)
    s32 roomIndex;
    // 0x08 (0x0CC) - checkpoint copy of crateCount (SetCheckpoint/RestoreCheckpoint)
    s32 checkpointCrateCount;
    u8 checkpointSwitchPressed; // 0x0C (0x0D0) - checkpoint copy of switchPressed
    u8 unk_0d[3];
    // 0x10 (0x0D4) - the player's position at the checkpoint, where PlayRoom
    // places the player
    s32 checkpointX;
    s32 checkpointY; // 0x14 (0x0D8)
    // 0x18 (0x0DC) - the current room (SelectRoom); kind 3 is a stage
    // played in an actor category
    const struct level_room *cat;
    // 0x1C (0x0E0) - checkpoint flags (SetCheckpoint, SetCheckpointAtPlayer);
    // bit 0: PlayRoom starts the player X-mirrored. Not the level state's
    // own `flags` (+0x002, the gems and powers)
    u8 checkpointFlags;
    u8 unk_1d[3];
};

COMPILE_TIME_ASSERT(level_state_h, offsetof(struct level_progress, cat) == 0x18);
COMPILE_TIME_ASSERT(level_state_h, offsetof(struct level_progress, checkpointFlags) == 0x1C);
COMPILE_TIME_ASSERT(level_state_h, sizeof(struct level_progress) == 0x20);

struct level_state {
    // 0x000 - packed lives (bits 0-6), wumpa (9-15) and maskLevel (7-8)
    // (UnpackSaveData/PackSaveData; PACKED_STATS_*)
    u8 packedStats[2];
    // 0x002 - bits 0-3: colored gems (CountGems); bits 4-7: powers
    // (HasTurboRun, HasSuperBodySlam, HasTornadoSpin, HasDoubleJump)
    u8 flags;
    u8 unk_03;
    // 0x004 - one word per level, indexed by `level`
    // (GetLevelFlags); LEVEL_FLAG_* (crystal, the two clear gems, best time)
    u32 levelFlags[LEVEL_COUNT];
    s32 unk_68; // 0x068
    s32 wumpa;  // 0x06C - at 100 it wraps and adds a life (CollectWumpa)
    // 0x070 - crates broken (AddBrokenCrate); reaching
    // `crateTotal` awards the crate gem (LEVEL_FLAG_CRATE_GEM)
    s32 crateCount;
    s32 lives;     // 0x074 - 5 at the start (ResetLives), capped at 99
    s32 maskLevel; // 0x078 - MASK_LEVEL_*; invincible plays SONG_DRUMS (SetMaskLevel)
    s32 deaths;    // 0x07C - maskless hits since the last checkpoint (AddDeath, ResetDeaths)
    // 0x080 - 5 at game start (UpdateGameFrame); SetUnusedAssistDeaths/
    // GetUnusedAssistDeaths, but nothing reads it
    s32 unusedAssistDeaths;
    // 0x084 - deaths after which the start marker hands out a mask (spawn_start_marker.cpp)
    s32 maskAssistDeaths;
    // 0x088 - from the level table (SetCrateAssistDeaths, 5 by default); once
    // `deaths` reaches it outside a time trial, CreateCrate turns placement-flagged
    // "?" crates (and kind 0xF) into Aku Aku, checkpoint or life crates
    s32 crateAssistDeaths;
    u8 timeTrial; // 0x08C - nonzero: no lives lost, the clock runs (StartTimeTrial)
    u8 unk_8d[3];
    s32 minutes;       // 0x090 - the time-trial clock (TickLevelClock), capped at 99
    s32 seconds;       // 0x094
    s32 tenths;        // 0x098
    s32 frames;        // 0x09C - 0-5, one tenth every 6 frames
    s32 countdown;     // 0x0A0 - frames the clock stays frozen (FreezeLevelClock adds seconds * 60)
    u8 inBonusRound;   // 0x0A4 - RequestBonusRound (player event 15) .. EndBonusRound
    u8 bonusRoundDone; // 0x0A5 - the bonus platform stays inactive
    u8 inGemPath;      // 0x0A6 - RequestGemPath (player event 16) .. EndGemPath
    u8 gemPathDone;    // 0x0A7 - the gem-path platform stays inactive
    u8 spawnAtStart;   // 0x0A8 - the player is placed on the room's start marker (ArmStartSpawn)
    u8 switchPressed;  // 0x0A9 - the switch crate was hit (PressSwitchCrate)
    u8 unk_aa[2];
    s32 pendingSwitchCrates; // 0x0AC - amount PressSwitchCrate adds to crateCount
    // 0x0B0 - saved on bonus-round entry (UpdateGameFrame), restored or added to by EndBonusRound
    s32 savedWumpa;
    // 0x0B4 - same, also for the gem path; PressSwitchCrate credits it while inBonusRound
    s32 savedCrateCount;
    s32 savedLives; // 0x0B8 - same
    s32 crateTotal; // 0x0BC - the level's crate count (CountLevelCrates), crateCount's target
    // 0x0C0 - bit mask, zeroed by InitLevelState; only the unused
    // SetUnusedFlags/TestUnusedFlags touch it
    s32 unusedFlags;
    // 0x0C4 - the current level and room, and the checkpoint
    struct level_progress room;
    // 0x0E4 - the first 0x68 bytes at the last checkpoint (SetCheckpoint/RestoreCheckpoint)
    u8 checkpointData[0x68];
    // 0x14C - the committed progress: restored before each level,
    // updated when one is won, packed for the save menus (PackSaveData)
    u8 saveData[0x68];
    struct entity_flags
        *savedBitmap;    // 0x1B4 - gEntityFlags, while a bonus round or gem path has its own
    void *bonusPlatform; // 0x1B8 - the bonus-round platform object (SetBonusPlatform)
    void *gemPlatform;   // 0x1BC - the gem-path platform object (SetGemPlatform)
    // 0x1C0 - where the crate gem appears (SetCrateGemPos); low halves go to SpawnCrateGem
    s32 crateGemX;
    s32 crateGemY;                // 0x1C4
    struct level_state_1c8 *boss; // 0x1C8 - SetLevelBoss
};

COMPILE_TIME_ASSERT(level_state_h, offsetof(struct level_state, room) == 0x0C4);
COMPILE_TIME_ASSERT(level_state_h, offsetof(struct level_state, checkpointData) == 0x0E4);
COMPILE_TIME_ASSERT(level_state_h, sizeof(struct level_state) == 0x1CC);

/* The record `level_state.boss` points at (GetBossHealth). */
struct level_state_1c8 {
    u8 unk_00[0x10];
    s32 hits; // 0x10 - hits taken; GetBossHealth returns 3 minus this
};

#endif /* GUARD_LEVEL_STATE_H */
