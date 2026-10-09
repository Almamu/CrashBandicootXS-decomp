#ifndef GUARD_LEVEL_STATE_HPP
#define GUARD_LEVEL_STATE_HPP

/* The level state as C++ (#750, docs/cplusplus.md): LevelState, the
 * object gLevelState points at (gLevelStateSingleton; GetLevelState,
 * level_state.cpp, makes the one instance), and LevelProgress, its room
 * block. Their functions are methods, mapped to their C names by
 * cxx_symbols.txt: LevelState's in level_state.cpp (the accessors),
 * spawn_markers.cpp (the constructor, InitLevelState), level_cutscene.cpp
 * (the destructor, DestroyLevelState, UNUSED: the game never leaves
 * MainLoop; PlayCutscene), game_frame.cpp (UpdateGameFrame),
 * bonus_round.cpp, level_state.cpp and inline_copies_misc.cpp (GetLives);
 * LevelProgress's in level_query.cpp, room_select.cpp, play_room.cpp
 * and room.cpp. The progress blocks stay level_state.h's
 * plain struct game_progress, which the save slots and the menus share.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to
 * emit; the pragma keeps g++ from emitting out-of-line copies of inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

extern "C" {
#include "core.h"
#include "level_state.h"
}

class BossCtrl;
struct entity_flags;

/*
 * The room block of the level state (`room`, +0x0C4), whose methods are
 * the room functions (UpdateGameFrame calls them on `room`): SelectRoom,
 * NextRoom, EnterBonusRoom and the others in level_query.cpp and
 * room_select.cpp, PlayRoom, RunRoom, UpdateRoomFrame and SetupRoomBlend
 * (play_room.cpp) and ResumeRoomAfterPause (room.cpp). Until
 * #750 it was level_state.h's struct level_progress, and the functions
 * took it as `self`. It merges the four file-local views `level_progress`
 * (level_query.c), `level_start_args` (play_room.c; `spawnX`/`spawnY`
 * were `checkpointX`/`checkpointY`), `gl_self` (run_room.c; `widget` was
 * `cat`) (#574, batch 9e) and play_room.cpp's `level_ctx` (`blend` was
 * `cat`; #656, batch 7). Offsets in brackets are the level state's.
 */
class LevelProgress
{
public:
    // level_query.cpp
    s32 IsInGemPathRoom();
    s32 IsInBonusRoom();
    // room_select.cpp
    void PlayRoomMusic();
    s32 NextRoom();
    void EnterGemPathRoom();
    void EnterBonusRoom();
    s32 SelectRoom();
    s32 PlayRoom();              // play_room.cpp
    void ResumeRoomAfterPause(); // room.cpp
    void UpdateRoomFrame();      // play_room.cpp
    void SetupRoomBlend();
    s32 RunRoom(); // play_room.cpp

    s32 level; // 0x00 (0x0C4) - index into gLevelTable
    // 0x04 (0x0C8) - the current room's index in the level's room list
    // (SelectRoom, NextRoom, GetRoomIndex)
    s32 roomIndex;
    // 0x08 (0x0CC) - checkpoint copy of crateCount (SetCheckpoint/RestoreCheckpoint)
    s32 checkpointCrateCount;
    u8 checkpointSwitchPressed; // 0x0C (0x0D0) - checkpoint copy of switchPressed
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
};

COMPILE_TIME_ASSERT(level_state_hpp, offsetof(LevelProgress, cat) == 0x18);
COMPILE_TIME_ASSERT(level_state_hpp, offsetof(LevelProgress, checkpointFlags) == 0x1C);
COMPILE_TIME_ASSERT(level_state_hpp, sizeof(LevelProgress) == 0x20);

/*
 * The level/session state (0x1CC bytes): the lives, wumpa, crates, mask,
 * time-trial clock, bonus-round and gem-path flags, the current room and
 * the progress blocks. Until #750 it was level_state.h's struct
 * level_state, which this class derived from, and its functions were C
 * functions taking it as `self`; the methods compile to the same code.
 * The first 0x68 bytes are the per-attempt progress block (`progress`)
 * the frame loop snapshots into `checkpointData`/`saveData` and restores
 * from (UpdateGameFrame).
 */
class LevelState
{
public:
    LevelState();               // InitLevelState, spawn_markers.cpp
    ~LevelState();              // DestroyLevelState, level_cutscene.cpp
    void PlayCutscene(s32 idx); // level_cutscene.cpp
    void UpdateGameFrame();     // game_frame.cpp
    // bonus_round.cpp
    void EndBonusRound(u8 arg1);
    void SetCheckpointAtPlayer(u8 arg1);
    void StartTimeTrial(); // level_state.cpp
    s32 GetLives();        // inline_copies_misc.cpp

    // level_state.cpp
    void FreezeLevelClock(s32 secs);
    void TickLevelClock();
    void AddBrokenCrate();
    void PressSwitchCrate();
    void *GetBonusPlatform();
    void SetCrateAssistDeaths(s32 value);
    void SetMaskAssistDeaths(s32 value);
    void SetUnusedAssistDeaths(s32 value);
    s32 GetCrateAssistDeaths();
    s32 GetMaskAssistDeaths();
    s32 GetUnusedAssistDeaths();
    void AddPendingSwitchCrates(s32 delta);
    void SetUnusedFlags(s32 mask);
    s32 TestUnusedFlags(s32 mask);
    void ClearPowers();
    void GiveTornadoSpin();
    void GiveSuperBodySlam();
    void GiveTurboRun();
    void GiveDoubleJump();
    s32 HasTornadoSpin();
    s32 HasSuperBodySlam();
    s32 HasTurboRun();
    s32 HasDoubleJump();
    void ResetCrateCount();
    void ResetWumpa();
    void ResetLives();
    void SetMaskLevel(s32 state);
    void SetLives(s32 value);
    void RaiseMaskLevel();
    void LoseLife();
    s32 GetWumpa();
    s32 GetClockTenths();
    s32 GetClockSeconds();
    s32 GetClockMinutes();
    u8 IsGemPathDone();
    void ClearGemPathDone();
    void SetGemPathDone();
    u8 IsInGemPath();
    void ClearInGemPath();
    u8 IsBonusRoundDone();
    void ClearBonusRoundDone();
    void SetBonusRoundDone();
    u8 IsInBonusRound();
    void ClearInBonusRound();
    u8 IsSwitchPressed();
    void ClearSwitchPressed();
    void ClearTimeTrial();
    s32 GetDeaths();
    void AddDeath();
    void ResetDeaths();
    u8 GetSpawnAtStart();
    void ClearSpawnAtStart();
    void ArmStartSpawn();
    void SetLevelBoss(void *value);
    s32 GetRoomIndex();
    s32 GetCurrentLevel();
    void SetCurrentLevel(s32 value);
    s32 LevelHasYellowGem(s32 idx);
    s32 LevelHasBlueGem(s32 idx);
    s32 LevelHasGreenGem(s32 idx);
    s32 LevelHasRedGem(s32 idx);
    s32 LevelHasGemPathGem(s32 idx);
    s32 GetBossHealth();
    s32 GetBossIndex();
    u8 *GetLevelFlags(s32 idx);
    u8 *GetCurrentLevelFlags();
    s32 GetCrateCount();
    s32 IsCrystalSaved();
    void CollectWumpa();
    void AddLife();
    void CheckAllCratesBroken();
    void SetGemPlatform(void *value);
    void SetBonusPlatform(void *value);
    void SetCrateGemPos(s32 *point);
    void RequestGemPath();
    void RequestBonusRound();
    void RestoreCheckpoint();
    void SetCheckpoint(s32 flag, s32 *pair);
    void EndGemPath(u8 flag);
    void PlayNewGameCutscene();
    void PlayIntroCutscene();
    void ShowCompanyLogos();
    void PlayBootCutscene();
    void UnpackSaveData(const struct game_progress *src);
    struct game_progress *PackSaveData();

    // 0x000 - the per-attempt progress block, snapshotted into
    // `checkpointData`/`saveData` and restored from them (UpdateGameFrame)
    struct game_progress progress;
    s32 unk_68; // 0x068 - zeroed at game start (UpdateGameFrame); nothing reads it
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
    u8 timeTrial;      // 0x08C - nonzero: no lives lost, the clock runs (StartTimeTrial)
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
    LevelProgress room;
    // 0x0E4 - the first 0x68 bytes at the last checkpoint (SetCheckpoint/RestoreCheckpoint)
    struct game_progress checkpointData;
    // 0x14C - the committed progress: restored before each level,
    // updated when one is won, packed for the save menus (PackSaveData)
    struct game_progress saveData;
    struct entity_flags
        *savedBitmap;    // 0x1B4 - gEntityFlags, while a bonus round or gem path has its own
    void *bonusPlatform; // 0x1B8 - the bonus-round platform object (SetBonusPlatform)
    void *gemPlatform;   // 0x1BC - the gem-path platform object (SetGemPlatform)
    // 0x1C0 - where the crate gem appears (SetCrateGemPos); low halves go to SpawnCrateGem
    s32 crateGemX;
    s32 crateGemY; // 0x1C4
    // 0x1C8 - the boss's controller (SetLevelBoss); GetBossHealth reads its
    // `counter`, the hits taken
    BossCtrl *boss;
};

COMPILE_TIME_ASSERT(level_state_hpp, offsetof(LevelState, unk_68) == 0x068);
COMPILE_TIME_ASSERT(level_state_hpp, offsetof(LevelState, room) == 0x0C4);
COMPILE_TIME_ASSERT(level_state_hpp, offsetof(LevelState, checkpointData) == 0x0E4);
COMPILE_TIME_ASSERT(level_state_hpp, offsetof(LevelState, saveData) == 0x14C);
COMPILE_TIME_ASSERT(level_state_hpp, offsetof(LevelState, savedBitmap) == 0x1B4);
COMPILE_TIME_ASSERT(level_state_hpp, sizeof(LevelState) == 0x1CC);

#endif /* !GUARD_LEVEL_STATE_HPP */
