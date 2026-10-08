#ifndef __LEVEL_H__
#define __LEVEL_H__

/* The level subsystem (src/level/): the level state and its accessors,
 * the level layers and BG scroll layers, rooms and their entities, the
 * entity spawners, the camera, terrain and collision maps, and the game
 * frame. Every function src/level/ defines, with the prototype of its
 * definition (the BG layers are C++ classes, include/bg_layer.hpp), and
 * the globals and tables the level files use
 * (docs/headers_plan.md). OperatorNew and the other new/delete operators
 * (camera.c) are in memory.h. A .c file that needs a different local
 * declaration for codegen keeps it as an asm-label alias with a
 * `codegen:` comment.
 *
 * Many functions take a file-local view of their object; this header
 * declares those by tag. */

#include "core.h"
#include "level_state.h"
#include "level_data.h"
#include "constants/bosses.h"
#include "bg_scroll_layer.h"
#include "constants/entities.h"
#include "constants/chunk_tokens.h"

struct camera;
struct level_progress;
struct part_list;
struct tile_slot_pool;

/* The terrain types of the level collision maps (bg_layer_base.cpp): a
 * cell's low byte picks one (0x24 and above are solid, 0 is empty, and
 * GetTerrainHeights stops at 0x23), `modeValue` is a value per collision mode
 * (GetSolidTerrainModeValue) and `heights` the surface height of each of the cell's
 * 8 pixel columns per mode, 0-7, 0xFF where there is none (GetTerrainHeights,
 * GetSolidTerrainHeights). */
struct terrain_type {
    u8 modeValue[4];
    u8 heights[4][8];
};

/* The 16-slot decode/LRU tile-record cache used throughout this cluster
 * of files (`bg_layer_base.cpp`/`tile_cache.cpp`/`collision_map.c`, gLevelLayers->tiles; docs/rom_map.md's
 * "Collision/terrain-map streamer" / "`GetCollisionChunk` (16-slot LRU
 * cache/decode dispatcher)"). `id[N]` holds the record ID currently
 * decoded into the matching 256-byte `buf[N]` slot; `nextSlot` is the
 * ring-buffer eviction cursor this function advances every time it
 * decodes a new record (evicting slot `(nextSlot - 1) & 0xf`, i.e. the
 * slot filled just before the current cursor position). The descriptor
 * this cache is built from (`source` below, populated by `SetCollisionSource`
 * in collision_map.c) is kept as raw offsets rather than its own struct -
 * it's never allocated by any function in this cluster, so its full
 * shape isn't confirmed enough to commit to one. (terrain_probe_axes.c
 * reads `unk010`/`unk014` as the width and height in tiles.) */
struct tile_cache {
    void *source; /* 0x000 */
    /* 0x004 - gLevelLayers's camera offset + source->4; DecodeCollisionChunk's decode-table base */
    void *decodeBase;
    s32 unk008;        /* 0x008 - source->0x1a << 3; not read anywhere in this cluster */
    s32 unk00c;        /* 0x00c - source->0x1c << 3; not read anywhere in this cluster */
    s32 unk010;        /* 0x010 - copy of source->0x1a; not read anywhere in this cluster */
    s32 unk014;        /* 0x014 - copy of source->0x1c; not read anywhere in this cluster */
    s32 width;         /* 0x018 - tiles, copy of source->0x16 */
    s32 height;        /* 0x01c - tiles, copy of source->0x18; not read anywhere in this cluster */
    u8 buf[16][0x100]; /* 0x020 - 0x1020, 16 decoded 256-byte chunks */
    s32 id[16];        /* 0x1020 - 0x105c, record IDs resident in `buf` */
    s32 nextSlot;      /* 0x1060 */
};

/* A terrain probe's position (ProbeTerrain and its helpers). */
struct probe_pos {
    s32 x;
    s32 y;
};

/* The level-layers singleton (gLevelLayersSingleton, gLevelLayers): the
 * level's scroll position and limits, BG layer 0 (a PooledBgLayer, 0x60
 * bytes) and the three other BG layers (BgLayers, 0x5C bytes each), the
 * collision tile cache and the loaded level asset. The C view of the
 * class LevelLayers (include/bg_layer.hpp, src/level/level_layers.cpp). */
struct level_layers {
    s32 maxScrollX;                    // 0x00 - pixels
    s32 maxScrollY;                    // 0x04
    s32 scrollX;                       // 0x08 - pixels
    s32 scrollY;                       // 0x0C
    struct bg_scroll_layer *layer0;    // 0x10 - a PooledBgLayer
    struct bg_scroll_layer *layers[3]; // 0x14
    struct tile_cache *tiles;          // 0x20 - 0x1064 bytes
    void *asset;                       // 0x24
    u8 assetOwned;                     // 0x28
    // 0x29 - the terrain kind the last probe hit, recorded while `probeFlag` is set
    // (ProbeTerrainX/Y; terrain_probe_axes.c's `nibble`)
    u8 kind;
    // 0x2A - CollidePlayer sets it around its ground probe (player_collide.c's `busy`,
    // terrain_probe_axes.c's `flagHeld`)
    u8 probeFlag;
    // 0x2B - set when the room's blend mode is 1 (SetupRoomBlend): sprites then
    // take layer 0's priority minus one (GetSpriteObjPriority)
    u8 raiseObjPriority;
};

/* The entity flags (`gEntityFlags`, 0x408 bytes, InitEntityFlags; built
 * by UpdateGameFrame): the room's entity list and two pairs of bitmaps,
 * one bit per entity id (entity_flags.cpp's bit accessors, SetEntityIdGone
 * through MarkEntityIdActivated). SpawnRoomEntities skips the entities set in
 * `bits0` and copies `bits0`/`bits1` to `bits0Copy`/`bits1Copy` when the
 * room loads; MarkEntityGone and its inline copies set an entity's bit
 * in `bits0Copy` when it is collected, broken or killed, and
 * SetCheckpointAtPlayer copies the two back. The names are room_entities.cpp's
 * (`struct lk_self`); dingodile.c called the object `struct entity_flags`
 * (`bits0Copy` was `bitmap`, the list `struct collect_info`), time_trial.cpp
 * `struct collision_map` (`seen`), the enemy spawners' text_popup.h a
 * `struct level_record_table **`. */
struct entity_flags {
    const struct level_entity_list *list; // 0x000 - the room's entities and their parameters
    s32 pos;                              // 0x004 - SpawnRoomEntities's position argument >> 8
    u32 bits0[64];                        // 0x008
    u32 bits0Copy[64];                    // 0x108
    u32 bits1[64];                        // 0x208
    u32 bits1Copy[64];                    // 0x308
};

/* The entity spawner (`gEntitySpawner`, CreateEntitySpawner): the table
 * of spawn functions SpawnEntity calls by entity type. */
struct entity_spawner {
    const void *funcs; // 0x00 - gEntitySpawnFuncs
    s32 count;         // 0x04 - its length
};

/* The camera's followed object (gPlayer, or the camera lead, class
 * CameraLead in level_select.hpp). */
struct camera_target {
    s32 x;           // 0x00 - Q8
    s32 y;           // 0x04 - Q8
    u8 unk_08[0x1C]; // 0x08-0x23
    u8 dirFlags;     // 0x24 - struct player.dir: PLAYER_DIR_* (player.h; mode 2 look-ahead)
    u8 unk_25[3];    // 0x25-0x27
    u8 flags;        // 0x28 - bit 4 is the mirror flag
};

/* The camera (`gCamera`, 0x18 bytes, allocated by PlayRoom; camera.c):
 * a Q8 position, a Q8 look-ahead offset, the followed object and the
 * mode. run_room.cpp called it `struct gl_scratch`, action_ctrl_event.c
 * `struct follow_state` and level_select.c `struct follow_owner`. */
struct camera {
    s32 x;                        // 0x00 - Q8
    s32 y;                        // 0x04 - Q8
    s32 vx;                       // 0x08 - Q8 look-ahead
    s32 vy;                       // 0x0C - Q8 look-ahead
    struct camera_target *target; // 0x10
    s32 mode;                     // 0x14 - 1/2 select StepCameraFacing/StepCameraDirectional
};

/* src/level/pooled_bg_layer.cpp (the BG layers' methods are BgLayer's and
 * PooledBgLayer's, include/bg_layer.hpp) */
extern void nullsub_26(void);

/* src/level/bg_layer_base.cpp: the terrain tile cache's lookups (the BG
 * layer methods are BgLayerBase's, include/bg_layer.hpp) */
extern void *GetCollisionChunk(struct tile_cache *self, s32 recordId);
extern void *GetTerrainHeights(struct tile_cache *self, s32 x, s32 y);
extern void *GetSolidTerrainHeights(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut);
extern s8 GetSolidTerrainModeValue(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut);
extern void DecodeCollisionChunk(struct tile_cache *self, s32 recordId, void *dest);

/* src/level/bonus_round.c */
extern void EndBonusRound(struct level_state *self, u8 arg1);
extern void SetCheckpointAtPlayer(struct level_state *self, u8 arg1);

/* src/level/camera.c */
extern void StepCameraDirectional(struct camera *cam);
extern void StepCameraFacing(struct camera *cam);
extern void SnapCamera(struct camera *cam);
extern void UpdateCamera(struct camera *cam);

/* src/level/collision_map.c */
extern u16 GetCollisionCell(struct tile_cache *self, s32 x, s32 y);
extern void SetCollisionSource(struct tile_cache *self, struct level_layer_desc *source);
extern s32 SetBitmapBit(void *self, s32 n);
extern void ClearBitmapBit(void *self, s32 n);
extern void ClearBitmap(void *dst);
extern void *InitBitmap(void *self);

/* src/level/entity_flags.cpp: LevelEntityFlags's methods (include/spawners.hpp)
 * under their C names (cxx_symbols.txt), for the callers. */
extern s32 CountCrateEntities(void *self, const struct level_entity_list *list);
extern s32 IsEntityIdGone(void *self, s32 n);
extern s32 IsEntityIdActivated(void *self, s32 n);
extern void SetEntityIdActivated(void *self, s32 n);
extern void MarkEntityIdActivated(void *self, s32 n);

/* src/level/game_frame.cpp */
extern void UpdateGameFrame(struct level_state *self);

/* src/level/level_cutscene.cpp */
extern void PlayCutscene(void *self, s32 idx);

/* src/level/level_layers.cpp: LevelLayers's methods (include/bg_layer.hpp)
 * under their C names (cxx_symbols.txt), for the C callers */
extern void LoadRoom(struct level_layers *self, const struct level_room *args);
extern void SetLevelScroll(struct level_layers *self, s32 x, s32 y);
extern void CommitLevelScroll(struct level_layers *self);
extern void ScrollLevelLayers(struct level_layers *self);
extern void ResetLevelLayers(struct level_layers *self);
extern s32 sub_80269DC(void *self, s32 arg1, s32 *arg2, s32 arg3);
extern s32 sub_80269F8(void *self, s32 arg1, s32 *arg2, s32 arg3);
extern s32 sub_8026A14(void);

/* src/level/level_query.cpp (C linkage) */
extern s32 CountLevelCrates(s32 idx);
extern s32 LevelHasEntityType(s32 idx, s32 flagIdx);
extern s32 IsInGemPathRoom(struct level_progress *self);
extern s32 IsInBonusRoom(struct level_progress *self);
extern s32 LevelHasYellowGemEntity(s32 idx);
extern s32 LevelHasBlueGemEntity(s32 idx);
extern s32 LevelHasGreenGemEntity(s32 idx);
extern s32 LevelHasRedGemEntity(s32 idx);
extern s32 LevelHasGemPathGemEntity(s32 idx);
extern s32 CountRoomCrates(const struct level_room *item);
extern void PlayRoomMusic(struct level_progress *self);
extern s32 NextRoom(struct level_progress *self);
extern void EnterGemPathRoom(struct level_progress *self);
extern void EnterBonusRoom(struct level_progress *self);
extern s32 SelectRoom(struct level_progress *self);

/* src/level/level_state.cpp */
extern void FreezeLevelClock(struct level_state *self, s32 seconds);
extern void TickLevelClock(struct level_state *self);
extern void AddBrokenCrate(struct level_state *self);
extern void PressSwitchCrate(struct level_state *self);
extern void *GetBonusPlatform(struct level_state *self);
extern void SetCrateAssistDeaths(struct level_state *self, s32 value);
extern void SetMaskAssistDeaths(struct level_state *self, s32 value);
extern void SetUnusedAssistDeaths(struct level_state *self, s32 value);
extern s32 GetCrateAssistDeaths(struct level_state *self);
extern s32 GetMaskAssistDeaths(struct level_state *self);
extern s32 GetUnusedAssistDeaths(struct level_state *self);
extern void AddPendingSwitchCrates(struct level_state *self, s32 delta);
extern void SetUnusedFlags(struct level_state *self, s32 mask);
extern s32 TestUnusedFlags(struct level_state *self, s32 mask);
extern void ClearPowers(struct level_state *self);
extern void GiveTornadoSpin(struct level_state *self);
extern void GiveSuperBodySlam(struct level_state *self);
extern void GiveTurboRun(struct level_state *self);
extern void GiveDoubleJump(struct level_state *self);
extern s32 HasTornadoSpin(struct level_state *self);
extern s32 HasSuperBodySlam(struct level_state *self);
extern s32 HasTurboRun(struct level_state *self);
extern s32 HasDoubleJump(struct level_state *self);
extern void ResetCrateCount(struct level_state *self);
extern void ResetWumpa(struct level_state *self);
extern void ResetLives(struct level_state *self);
extern void SetMaskLevel(void *self, s32 state);
extern void SetLives(struct level_state *self, s32 value);
extern void RaiseMaskLevel(struct level_state *self);
extern void LoseLife(struct level_state *self);
extern s32 GetWumpa(struct level_state *self);
extern s32 GetClockTenths(struct level_state *self);
extern s32 GetClockSeconds(struct level_state *self);
extern s32 GetClockMinutes(struct level_state *self);
extern u8 IsGemPathDone(struct level_state *self);
extern void ClearGemPathDone(struct level_state *self);
extern void SetGemPathDone(struct level_state *self);
extern u8 IsInGemPath(struct level_state *self);
extern void ClearInGemPath(struct level_state *self);
extern u8 IsBonusRoundDone(struct level_state *self);
extern void ClearBonusRoundDone(struct level_state *self);
extern void SetBonusRoundDone(struct level_state *self);
extern u8 IsInBonusRound(struct level_state *self);
extern void ClearInBonusRound(struct level_state *self);
extern u8 IsSwitchPressed(struct level_state *self);
extern void ClearSwitchPressed(struct level_state *self);
extern void ClearTimeTrial(struct level_state *self);
extern s32 GetDeaths(struct level_state *self);
extern void AddDeath(struct level_state *self);
extern void ResetDeaths(struct level_state *self);
extern u8 GetSpawnAtStart(struct level_state *self);
extern void ClearSpawnAtStart(struct level_state *self);
extern void ArmStartSpawn(struct level_state *self);
extern void SetLevelBoss(struct level_state *self, void *value);
extern s32 GetRoomIndex(struct level_state *self);
extern s32 GetCurrentLevel(struct level_state *self);
extern void SetCurrentLevel(struct level_state *self, s32 value);
extern s32 LevelHasYellowGem(void *self, s32 idx);
extern s32 LevelHasBlueGem(void *self, s32 idx);
extern s32 LevelHasGreenGem(void *self, s32 idx);
extern s32 LevelHasRedGem(void *self, s32 idx);
extern s32 LevelHasGemPathGem(void *self, s32 idx);
extern s32 GetBossHealth(struct level_state *self);
extern s32 GetBossIndex(struct level_state *self);
extern u8 *GetLevelFlags(struct level_state *self, s32 idx);
extern u8 *GetCurrentLevelFlags(struct level_state *self);
extern s32 GetCrateCount(struct level_state *self);
extern s32 IsCrystalSaved(struct level_state *self);
extern void CollectWumpa(struct level_state *self);
extern void AddLife(struct level_state *self);
extern void CheckAllCratesBroken(void *self);
extern void SetGemPlatform(struct level_state *self, void *value);
extern void SetBonusPlatform(struct level_state *self, void *value);
extern void SetCrateGemPos(struct level_state *self, s32 *point);
extern void RequestGemPath(struct level_state *self);
extern void RequestBonusRound(struct level_state *self);
extern void RestoreCheckpoint(struct level_state *self);
extern void SetCheckpoint(void *self, s32 flag, s32 *pair);
extern void EndGemPath(struct level_state *self, u8 flag);
extern void PlayNewGameCutscene(void *self);
extern void PlayIntroCutscene(void *self);
extern void ShowCompanyLogos(void *unused);
extern void PlayBootCutscene(void *self);
extern void nullsub_24(void);
extern void UnpackSaveData(struct level_state *self, const struct game_progress *src);
extern struct game_progress *PackSaveData(void *self);
extern struct level_state *GetLevelState(void);

/* src/level/play_room.cpp */
extern s32 PlayRoom(struct level_progress *self);

/* src/level/room.c */
extern void ClearRoomExit(void);
extern void RequestRoomExit(void);
extern u8 IsRoomExitRequested(void);
extern void ResumeRoomAfterPause(struct level_progress *self);
extern void ResetObjBuffers(void);

/* src/level/room_entities.cpp */
extern void SpawnRoomEntities(struct entity_flags *self, const struct level_entity_list *list,
                              const struct level_link_list *links, s32 pos, s32 unused);

/* src/level/room_frame.cpp */
extern void UpdateRoomFrame(struct level_progress *self);
extern void SetupRoomBlend(struct level_progress *self);

/* src/level/run_room.cpp */
extern s32 RunRoom(struct level_progress *self);

/* src/level/spawn_bosses.cpp */
extern void SpawnRoomExit(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnDingodile(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTiny(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCortexBoss(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_crates.cpp */
extern void SpawnNitroSwitchCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnOutlineCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnArrowCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnIronSwitchCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnAkuAkuCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCheckpointCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBasicCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_enemies.cpp */
extern void SpawnLizard(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnVulture(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnVenusFlytrap(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPatrollingJungleEnemy(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBlowgunTribesman(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPenguin(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSeal(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPolarBear(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPufferfish(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnShark(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnMorayEel(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnElectricEel(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSquid(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnJellyfish(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnLaserBarrier(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnStationarySpaceEnemy(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPatrollingSpaceEnemy(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSaucerLabAssistant(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPistonCrusher(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnFlamethrowerLabAssistant(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnHomingSewerEnemy(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPatrollingSewerEnemy(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnRat(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnFrog(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSeaMine(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnWoodenCrusher(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_gem_platforms.cpp */
extern void SpawnRedGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnYellowGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnGreenGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnBlueGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);

/* src/level/spawn_gems.cpp */
extern void SpawnCrystal(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnCrateGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnGemPathGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnRedGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnGreenGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnYellowGem(u32 a, u16 a1, u16 a2, u16 a3);

/* src/level/spawn_objects.cpp */
extern void SpawnMegaMix(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSeaweed(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSeaweedNoAnimReset(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnFlame(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnRockPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnFlipPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBonusPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnMediumPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSmallPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnLargePlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnLaunchPadEntity(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnNoEntity(void);
extern void SpawnSealSpawner(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTimeCrate3(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTimeCrate2(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTimeCrate1(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSlotCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTntCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnReinforcedCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBouncyWumpaCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnMysteryCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnNitroCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnLifeCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnIronArrowCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnIronCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_pickups.cpp */
extern void SpawnBodySlamPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTornadoSpinPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnDoubleJumpPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTurboRunPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnStopwatch(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBlueGem(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCrateGemMarker(u32 arg, u32 arg1, u32 arg2, u16 arg3);
extern void *CreateTouchableSprite(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch);
extern void SpawnWumpa(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnHoverPlayerPosition(void);
extern void SpawnHoverStartMarker(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnUnderwaterPlayerPosition(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnUnderwaterStartMarker(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnPlayerPosition(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnStartMarkerStub(void);
extern void DestroyEntitySpawner(void);
extern void CreateEntitySpawner(void);

/* src/level/spawn_start_marker.cpp */
extern void SpawnStartMarker(u32 arg, u16 x, u16 y, u16 z);

/* src/level/terrain.c */
extern s32 GetTerrainFlagsAt(void *arg, s32 x, s32 y);
extern s32 ProbeFloorHeight(void *player, struct probe_pos *pos, s32 *outValue);
extern s32 ProbeSolidFloorHeight(void *player, struct probe_pos *pos, s32 *outValue);
extern s32 sub_8026C80(void *arg, s32 arg1, s32 *arg2);
extern s32 sub_8026C8C(void);

/* src/level/terrain_probe.c */
extern s32 ProbeTerrain(void *self, s32 mode, struct probe_pos *pos, s32 span, s32 *outValue);

/* src/level/terrain_probe_axes.c */
extern s32 ProbeTerrainY(struct level_layers *self, struct probe_pos *pos, s32 span, s32 *outValue,
                         s32 submode);
extern s32 ProbeTerrainX(struct level_layers *self, struct probe_pos *pos, s32 span, s32 *outValue,
                         s32 submode);

/* src/level/tile_cache.cpp (C linkage) */
extern u16 GetTerrainType(struct tile_cache *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut);

/* src/level/tile_slot_pool.cpp: layer 0's VRAM tile-slot pool */
extern void ResetTileSlotPool(struct tile_slot_pool *pool);
extern u16 AcquireTileSlot(struct tile_slot_pool *pool, u16 tile);
extern void ReleaseTileSlot(struct tile_slot_pool *pool, u32 tile);
extern void UploadTileSlot(struct tile_slot_pool *pool, s32 tileId, s32 slot);
extern void SetTileSlotPoolSource(struct tile_slot_pool *pool, s32 charBase, u32 src);

/* src/level/time_trial.cpp */
extern void StartTimeTrial(struct level_state *self);

/* sym_iwram.txt */
extern struct part_list *gDecorationList;
/* UpdateGameFrame's level state, stored once when the game starts and never
 * read. */
extern struct level_state *gGameFrameLevelState;
/* Updated and cleared, but never culled or drawn: the invisible objects,
 * the entity type 0x55 room-exit zones (spawn_bosses.cpp) and
 * SpawnSealSpawner's spawner. */
extern struct part_list *gUpdateOnlyPartList;

/* The enemies' anim maps (src/data/popup_tables_16b98c.c), stored in their
 * controllers by the spawners */
extern const s32 gBlowgunTribesmanAnimMap[8];
extern const s32 gCrusherAnimMap[8];
extern const s32 gElectricEelAnimMap[8];
extern const s32 gEnemyDefaultAnimMap[8];
extern const s32 gFlamethrowerLabAssistantAnimMap[8];
extern const s32 gPatrollingSewerEnemyAnimMap[8];
extern const s32 gPatrollingSpaceEnemyAnimMap[8];
extern const s32 gPenguinAnimMap[8];
extern const s32 gPufferfishAnimMap[8];
extern const s32 gSaucerLabAssistantAnimMap[8];
extern const s32 gSharkAnimMap[8];
extern const s32 gSquidAnimMap[8];
extern const s32 gStationarySpaceEnemyAnimMap[8];
extern const s32 gVenusFlytrapAnimMap[8];
extern const s32 gVultureAnimMap[8];

/* An entity spawner: the entity's type-specific id and its x/y and index
 * (SpawnEntity passes the entity record's four halfwords). */
typedef void (*entity_spawn_fn)(u32 id, u16 x, u16 y, u16 index);

/* The entity spawners, indexed by entity type (src/data/dispatch_table_16c6a4.c);
 * the few with other parameters are cast. */
extern const entity_spawn_fn gEntitySpawnFuncs[ENTITY_COUNT];

/* src/iwram/iwram_data.c */
#ifdef __cplusplus
extern class LevelLayers *gLevelLayersSingleton;
#else
extern struct level_layers *gLevelLayersSingleton;
#endif
extern struct level_state *gLevelStateSingleton;
extern u8 gRoomExitRequested;

/* The terrain height tables (src/data/terrain_1725a8.c, top-level asm) */
extern u8 gTerrainHeights0[];
extern u8 gTerrainHeights1[];
extern u8 gTerrainHeights2[];
extern u8 gTerrainHeights3[];

/* src/data/terrain_1725a8.c */
extern const struct terrain_type gTerrainTypes[51];

/* The level table (src/data/level_table_16c814.c, level_data.h). */
extern const struct level_info gLevelTable[LEVEL_COUNT];

/* The level themes' music cues and palette cycles (src/data/level_table_16c814.c) */
extern const u8 gThemeMusicCues[11];
extern const u16 gThemePaletteCycle1A[9];
extern const u16 gThemePaletteCycle1B[9];
extern const u16 gThemePaletteCycle2[5];
extern const u16 gThemePaletteCycle3[16];
extern const u16 gThemePaletteCycle5[5];

#endif /* __LEVEL_H__ */
