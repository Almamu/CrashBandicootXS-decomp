#ifndef GUARD_LEVEL_STATE_H
#define GUARD_LEVEL_STATE_H

/*
 * The level/session state object `gLevelState` points at (0x1CC
 * bytes). Its accessor family is src/system/game_loop2.c
 * (FreezeLevelClock through CheckAllCratesBroken); game_loop55.c's level loop still carries
 * its own copy of the same layout (`struct level_state` there).
 *
 * The first 0x68 bytes are the per-attempt block the frame loop
 * snapshots into `checkpointData`/`saveData` and restores from (game_loop55.c).
 */

/* The level's category record (level_state.cat) - one of the medal
 * table's list items (game_loop18.c's `struct MedalListItem`). */
struct level_category
{
    u8 unk_00[8];
    s32 kind;                       // 0x08 - 0-2 plain level, 3 actor-category level
    u8 unk_0c[4];
    u16 category;                   // 0x10
};

struct level_state_1c8;

struct level_state
{
    u8 unk_00[2];                   // 0x000 - packed lives (bits 0-6), wumpa (9-15) and maskLevel (7-8) (UnpackSaveData/PackSaveData)
    u8 flags;                       // 0x002 - bits 0-3: colored gems (CountGems); bits 4-7: powers (HasTurboRun, HasSuperBodySlam, HasTornadoSpin, HasDoubleJump)
    u8 unk_03;
    u32 levelFlags[0x19];           // 0x004 - one word per level, indexed by `level` (GetLevelFlags); bit 0 crystal, bits 1-2 clear gems
    s32 unk_68;                     // 0x068
    s32 wumpa;                      // 0x06C - at 100 it wraps and adds a life (CollectWumpa)
    s32 crateCount;                 // 0x070 - crates broken (AddBrokenCrate); reaching `crateTotal` awards the crate gem (levelFlags bit 1)
    s32 lives;                      // 0x074 - 5 at the start (ResetLives), capped at 99
    s32 maskLevel;                  // 0x078 - 0-3; 3 plays the invincibility jingle (SetMaskLevel)
    s32 deaths;                     // 0x07C - maskless hits since the last checkpoint (AddDeath, ResetDeaths)
    s32 unk_80;                     // 0x080
    s32 maskAssistDeaths;           // 0x084 - deaths after which the start marker hands out a mask (graphics_loading_1e990.c)
    s32 crateAssistDeaths;          // 0x088 - from the level table (sub_8023110, 5 by default); once `deaths` reaches it outside a time trial, CreateCrate turns placement-flagged "?" crates (and kind 0xF) into Aku Aku, checkpoint or life crates
    u8 timeTrial;                   // 0x08C - nonzero: no lives lost, the clock runs (StartTimeTrial)
    u8 unk_8d[3];
    s32 minutes;                    // 0x090 - the time-trial clock (TickLevelClock), capped at 99
    s32 seconds;                    // 0x094
    s32 tenths;                     // 0x098
    s32 frames;                     // 0x09C - 0-5, one tenth every 6 frames
    s32 countdown;                  // 0x0A0 - frames the clock stays frozen (FreezeLevelClock adds seconds * 60)
    u8 inBonusRound;                // 0x0A4 - RequestBonusRound (player event 15) .. EndBonusRound
    u8 bonusRoundDone;              // 0x0A5 - the bonus platform stays inactive
    u8 inGemPath;                   // 0x0A6 - RequestGemPath (player event 16) .. EndGemPath
    u8 gemPathDone;                 // 0x0A7 - the gem-path platform stays inactive
    u8 spawnAtStart;                // 0x0A8 - the player is placed on the room's start marker (ArmStartSpawn)
    u8 switchPressed;               // 0x0A9 - the switch crate was hit (PressSwitchCrate)
    u8 unk_aa[2];
    s32 pendingSwitchCrates;                     // 0x0AC - amount PressSwitchCrate adds to crateCount
    s32 savedWumpa;                 // 0x0B0 - saved on bonus-round entry (UpdateGameFrame), restored or added to by EndBonusRound
    s32 savedCrateCount;            // 0x0B4 - same, also for the gem path; PressSwitchCrate credits it while inBonusRound
    s32 savedLives;                 // 0x0B8 - same
    s32 crateTotal;                 // 0x0BC - the level's crate count (CountLevelCrates), crateCount's target
    s32 unk_c0;                     // 0x0C0 - bit mask (sub_802314C/sub_8023158)
    s32 level;                      // 0x0C4 - also the head of the progress record (game_loop18.c's struct level_progress)
    s32 unk_c8;                     // 0x0C8
    s32 checkpointCrateCount;       // 0x0CC - checkpoint copy of crateCount (SetCheckpoint/RestoreCheckpoint)
    u8 checkpointSwitchPressed;     // 0x0D0 - checkpoint copy of switchPressed
    u8 unk_d1[3];
    s32 checkpointX;                // 0x0D4 - the player's position at the checkpoint
    s32 checkpointY;                // 0x0D8
    struct level_category *cat;     // 0x0DC - level_progress.item
    u8 unk_e0;                      // 0x0E0 - checkpoint flag (SetCheckpoint)
    u8 unk_e1[3];
    u8 checkpointData[0x68];        // 0x0E4 - the first 0x68 bytes at the last checkpoint (SetCheckpoint/RestoreCheckpoint)
    u8 saveData[0x68];              // 0x14C - the committed progress: restored before each level, updated when one is won, packed for the save menus (PackSaveData)
    void *savedBitmap;              // 0x1B4
    s32 bonusPlatform;              // 0x1B8 - the bonus-round platform object (SetBonusPlatform)
    s32 gemPlatform;                // 0x1BC - the gem-path platform object (SetGemPlatform)
    s32 crateGemX;                  // 0x1C0 - where the crate gem appears (SetCrateGemPos); low halves go to SpawnCrateGem
    s32 crateGemY;                  // 0x1C4
    struct level_state_1c8 *boss; // 0x1C8 - SetLevelBoss
};

COMPILE_TIME_ASSERT(sizeof(struct level_state) == 0x1CC);

/* The record `level_state.boss` points at (GetBossHealth). */
struct level_state_1c8
{
    u8 unk_00[0x10];
    s32 hits;                       // 0x10 - hits taken; GetBossHealth returns 3 minus this
};

#endif /* GUARD_LEVEL_STATE_H */
