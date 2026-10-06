#ifndef __LEVEL_H__
#define __LEVEL_H__

/* The level subsystem (src/level/): the level state and its accessors,
 * the level layers and BG scroll layers, rooms and their entities, the
 * entity spawners, the camera, terrain and collision maps, and the game
 * frame. Every function src/level/ defines, with the prototype of its
 * definition, plus the BG layer functions that cutscene_player.c holds
 * for ROM order, and the globals and tables the level files use
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
#include "bg_scroll_layer.h"
#include "vtable.h"

struct MedalListItem;
struct bg_streamer;
struct camera;
struct collider;
struct fx_part;
struct gl_self;
struct level_ctx;
struct level_progress;
struct lk_links;
struct lk_list;
struct lk_self;
struct orbit_part;
struct pooled_layer;
struct part_list;

/* The terrain types of the level collision maps (bg_layer_base.c): a
 * cell's low byte picks one (0x24 and above are solid, 0 is empty, and
 * GetTerrainHeights stops at 0x23), `modeValue` is a value per collision mode
 * (sub_8025228) and `heights` the surface height of each of the cell's
 * 8 pixel columns per mode, 0-7, 0xFF where there is none (GetTerrainHeights,
 * GetSolidTerrainHeights). */
struct terrain_type
{
    u8 modeValue[4];
    u8 heights[4][8];
};

/* The 16-slot decode/LRU tile-record cache used throughout this cluster
 * of files (`bg_layer_base.c`/`tile_cache.c`/`collision_map.c`, gLevelLayers->tiles; docs/rom_map.md's
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
    void *source;      /* 0x000 */
    void *decodeBase;  /* 0x004 - gLevelLayers's camera offset + source->4; DecodeCollisionChunk's decode-table base */
    s32 unk008;         /* 0x008 - source->0x1a << 3; not read anywhere in this cluster */
    s32 unk00c;          /* 0x00c - source->0x1c << 3; not read anywhere in this cluster */
    s32 unk010;           /* 0x010 - copy of source->0x1a; not read anywhere in this cluster */
    s32 unk014;            /* 0x014 - copy of source->0x1c; not read anywhere in this cluster */
    s32 width;               /* 0x018 - tiles, copy of source->0x16 */
    s32 height;                /* 0x01c - tiles, copy of source->0x18; not read anywhere in this cluster */
    u8 buf[16][0x100];           /* 0x020 - 0x1020, 16 decoded 256-byte chunks */
    s32 id[16];                    /* 0x1020 - 0x105c, record IDs resident in `buf` */
    s32 nextSlot;                    /* 0x1060 */
};

/* A terrain probe's position (ProbeTerrain and its helpers). */
struct probe_pos
{
    s32 x;
    s32 y;
};

/* The level-layers singleton (gLevelLayersSingleton, InitLevelLayers): the
 * level's scroll position and limits, BG layer 0 (a `struct
 * pooled_bg_layer`, 0x60 bytes, InitPooledBgLayer) and the three other BG
 * layers (0x5C bytes each, InitBgLayer), the collision tile cache and the
 * loaded level asset. */
struct level_layers
{
    s32 maxScrollX;                     // 0x00 - pixels
    s32 maxScrollY;                     // 0x04
    s32 scrollX;                        // 0x08 - pixels
    s32 scrollY;                        // 0x0C
    struct bg_scroll_layer *layer0;     // 0x10 - a struct pooled_bg_layer
    struct bg_scroll_layer *layers[3];  // 0x14
    struct tile_cache *tiles;           // 0x20 - 0x1064 bytes
    void *asset;                        // 0x24
    u8 assetOwned;                      // 0x28
    u8 unk_29;                          // 0x29
    u8 unk_2A;                          // 0x2A
    u8 unk_2B;                          // 0x2B
};

/* src/level/bg_layer.c */
extern void ScrollBgLayer(struct bg_scroll_layer *self, void *vec2);
extern void CommitBgLayerScroll(void *self);
extern void DrawBgLayerColumn(struct bg_scroll_layer *self, s32 col);
extern void DrawBgLayerRow(struct bg_scroll_layer *self, s32 row);
extern void RedrawBgLayer(struct bg_scroll_layer *self);
extern void ResetBgLayer(struct bg_scroll_layer *self, void *pos);
extern void LoadBgLayerTiles(struct bg_scroll_layer *self);
extern void LoadBgLayer(struct bg_scroll_layer *self, const struct level_layer_desc *desc);
extern s32 GetBgLayerScreenIndex(void *self, s32 col, s32 row);
extern s32 sub_802612C(void *self, s32 col);
extern s32 sub_802613C(void *self, s32 col);
extern void SetBgLayerScreenBase(struct bg_scroll_layer *self, u32 screenBase);
extern void SetBgLayerPriority(struct bg_scroll_layer *self, u32 priority);
extern void SetBgLayerColors256(struct bg_scroll_layer *self, u32 colors256);
extern u32 GetBgLayerCharBase(struct bg_scroll_layer *self);
extern void SetBgLayerCharBase(struct bg_scroll_layer *self, u32 charBase);
extern void WriteBgLayerOffsetRegs(struct bg_scroll_layer *self);
extern void WriteBgLayerCntReg(struct bg_scroll_layer *self);
extern void DestroyBgLayer(struct bg_scroll_layer *self, u32 flags);
extern void DrawPooledBgLayerColumn(struct pooled_bg_layer *self, s32 col);
extern s32 ClampPooledBgLayerScrollStep(struct pooled_bg_layer *self, s32 v);
extern void ReleasePooledBgLayerColumn(struct pooled_bg_layer *self, s32 col);
extern void ReleasePooledBgLayerRow(struct pooled_bg_layer *self, s32 row);
extern void ClipPooledBgLayerColumns(struct pooled_bg_layer *self, s32 lo, s32 hi);
extern void ClipPooledBgLayerRows(struct pooled_bg_layer *self, s32 lo, s32 hi);
extern void DrawPooledBgLayerRow(struct pooled_bg_layer *self, s32 row);
extern void ResetPooledBgLayer(struct pooled_bg_layer *self, void *pos);
extern void LoadPooledBgLayerTiles(struct pooled_bg_layer *self);
extern void nullsub_26(void);

/* src/level/bg_layer_base.c */
extern void ScrollBgLayerBase(struct bg_scroll_layer *self, s32 *vec2);
extern void ResetBgLayerBase(struct bg_scroll_layer *self, s32 *vec2);
extern void SetBgLayerSource(struct bg_scroll_layer *self, const struct level_layer_desc *source);
extern u8 IsBgLayerEnabled(struct bg_scroll_layer *self);
extern s32 GetBgLayerY(struct bg_scroll_layer *self);
extern s32 GetBgLayerX(struct bg_scroll_layer *self);
extern s32 GetBgLayerHeightTiles(struct bg_scroll_layer *self);
extern s32 GetBgLayerWidthTiles(struct bg_scroll_layer *self);
extern s32 GetBgLayerHeight(struct bg_scroll_layer *self);
extern s32 GetBgLayerWidth(struct bg_scroll_layer *self);
extern void *GetCollisionChunk(struct tile_cache *self, s32 recordId);
extern void *GetTerrainHeights(struct tile_cache *self, s32 x, s32 y);
extern void *GetSolidTerrainHeights(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut);
extern s8 sub_8025228(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut);
extern void DecodeCollisionChunk(struct tile_cache *self, s32 recordId, void *dest);

/* src/level/bg_layer_init.c */
extern void *InitBgLayer(void *self, s32 bgIndex);
extern void GrowBgLayerRows(struct bg_scroll_layer *self, s32 lo, s32 hi);
extern void GrowBgLayerColumns(struct bg_scroll_layer *self, s32 lo, s32 hi);
extern void ClipBgLayerColumns(struct bg_scroll_layer *self, s32 a, s32 b);
extern void ClipBgLayerRows(struct bg_scroll_layer *self, s32 a, s32 b);

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

/* src/level/drop_extra_life.c */
extern struct orbit_part *DropExtraLife(void *unused, u32 x, u32 y, u32 p3, u32 p5, u32 flag6);

/* src/level/entity_flags.c */
extern s32 CountCrateEntities(void *self, void *list);
extern void sub_8025944(void *self, s32 n);
extern s32 sub_8025968(void *self, s32 n);
extern s32 sub_802599C(void *self, s32 n);
extern void sub_80259D4(void *self, s32 n);
extern void sub_8025A0C(void *self, s32 n);
extern void sub_8025A3C(void *self, s32 val);
extern void DestroyEntityFlags(void *self, s32 flags);
extern void *InitEntityFlags(void *self);

/* src/level/entity_spawner.c */
extern void *LaunchEffectPart(void *pool, s32 arg1, s32 kind, s32 margin, s32 z, s32 speed, struct fx_part *src);
extern void *SpawnEffectPart(void *unused, s32 anim, s32 tag, s32 x, s32 y, s32 mirror);
extern struct orbit_part *DropWumpa(void *unused, u32 x, u32 y, u32 p3, u32 p4, u32 flag5);
extern void SpawnEntity(void **table, s32 id, u16 *rec);
extern void SetEntitySpawnerTable(void *self, const void *table, s32 count);
extern void sub_8025D54(void *self, s32 flags);
extern void InitEntitySpawner(void *self);

/* src/level/game_frame.c */
extern void UpdateGameFrame(struct level_state *self);

/* src/level/level_cutscene.c */
extern void DestroyLevelState(void *self, s32 flags);
extern void PlayCutscene(void *self, s32 idx);

/* src/level/level_layers.c */
extern void LoadRoom(struct level_layers *self, const struct level_room *args);
extern struct level_layers *InitLevelLayers(struct level_layers *self);
extern void DestroyLevelLayers(struct level_layers *self, u32 flags);
extern struct level_layers *GetLevelLayers(void);
extern void SetLevelScroll(struct level_layers *self, s32 x, s32 y);
extern void CommitLevelScroll(struct level_layers *self);
extern void ScrollLevelLayers(struct level_layers *self);
extern void ResetLevelLayers(struct level_layers *self);
extern s32 sub_80269DC(void *self, s32 arg1, s32 *arg2, s32 arg3);
extern s32 sub_80269F8(void *self, s32 arg1, s32 *arg2, s32 arg3);
extern s32 sub_8026A14(void);

/* src/level/level_query.c */
extern void sub_802425C(void *self, s32 flags);
extern void nullsub_25(void);
extern s32 CountLevelCrates(s32 idx);
extern s32 LevelHasEntityType(s32 idx, s32 flagIdx);
extern s32 IsInGemPathRoom(struct level_progress *self);
extern s32 IsInBonusRoom(struct level_progress *self);
extern s32 LevelHasYellowGemEntity(s32 idx);
extern s32 LevelHasBlueGemEntity(s32 idx);
extern s32 LevelHasGreenGemEntity(s32 idx);
extern s32 LevelHasRedGemEntity(s32 idx);
extern s32 LevelHasGemPathGemEntity(s32 idx);
extern s32 CountRoomCrates(struct MedalListItem *item);
extern void PlayRoomMusic(struct level_progress *self);
extern s32 NextRoom(struct level_progress *self);
extern void EnterGemPathRoom(struct level_progress *self);
extern void EnterBonusRoom(struct level_progress *self);
extern s32 SelectRoom(struct level_progress *self);

/* src/level/level_state.c */
extern void FreezeLevelClock(struct level_state *self, s32 seconds);
extern void TickLevelClock(struct level_state *self);
extern void AddBrokenCrate(struct level_state *self);
extern void PressSwitchCrate(struct level_state *self);
extern void *GetBonusPlatform(struct level_state *self);
extern void SetCrateAssistDeaths(struct level_state *self, s32 value);
extern void SetMaskAssistDeaths(struct level_state *self, s32 value);
extern void sub_8023120(struct level_state *self, s32 value);
extern s32 GetCrateAssistDeaths(struct level_state *self);
extern s32 GetMaskAssistDeaths(struct level_state *self);
extern s32 sub_8023138(struct level_state *self);
extern void AddPendingSwitchCrates(struct level_state *self, s32 delta);
extern void sub_802314C(struct level_state *self, s32 mask);
extern s32 sub_8023158(struct level_state *self, s32 mask);
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
extern void SetLevelBoss(struct level_state *self, struct level_state_1c8 *value);
extern s32 sub_8023324(struct level_state *self);
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
extern void SetGemPlatform(struct level_state *self, s32 value);
extern void SetBonusPlatform(struct level_state *self, s32 value);
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
extern void UnpackSaveData(struct level_state *self, void *src);
extern void *PackSaveData(void *self);
extern void *GetLevelState(void);

/* src/level/play_room.c */
extern s32 PlayRoom(void *self);

/* src/level/room.c */
extern void ClearRoomExit(void);
extern void RequestRoomExit(void);
extern u8 IsRoomExitRequested(void);
extern void ResumeRoomAfterPause(void *self);
extern void ResetObjBuffers(void);

/* src/level/room_entities.c */
extern void SpawnRoomEntities(struct lk_self *self, struct lk_list *list, struct lk_links *links, s32 pos, s32 unused);

/* src/level/room_frame.c */
extern void UpdateRoomFrame(void *self);
extern void SetupRoomBlend(struct level_ctx *self);

/* src/level/run_room.c */
extern s32 RunRoom(struct gl_self *self);

/* src/level/spawn_bosses.c */
extern void SpawnRoomExit(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnDingodile(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTiny(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCortexBoss(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_crates.c */
extern void SpawnNitroSwitchCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnOutlineCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnArrowCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnIronSwitchCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnAkuAkuCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCheckpointCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBasicCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_enemies.c */
extern void SpawnLizard(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnVulture(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnVenusFlytrap(u32 arg, u32 arg1, u32 arg2, u32 arg3);
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

/* src/level/spawn_gem_platforms.c */
extern void SpawnRedGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnYellowGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnGreenGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnBlueGemPlatform(u32 a, u16 a1, u16 a2, u16 a3);

/* src/level/spawn_gems.c */
extern void SpawnCrystal(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnCrateGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnGemPathGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnRedGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnGreenGem(u32 a, u16 a1, u16 a2, u16 a3);
extern void SpawnYellowGem(u32 a, u16 a1, u16 a2, u16 a3);

/* src/level/spawn_objects.c */
extern void SpawnMegaMix(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSeaweed(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80217D0(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnFlame(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnRockPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnFlipPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBonusPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnMediumPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSmallPlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnLargePlatform(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnLaunchPadEntity(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void nullsub_21(void);
extern void SpawnSealSpawner(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTimeCrate3(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTimeCrate2(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTimeCrate1(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnSlotCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTntCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void sub_8021B00(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBouncyWumpaCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnMysteryCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnNitroCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnLifeCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnIronArrowCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnIronCrate(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_pickups.c */
extern void SpawnBodySlamPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTornadoSpinPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnDoubleJumpPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTurboRunPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnStopwatch(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBlueGem(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCrateGemMarker(u32 arg, u32 arg1, u32 arg2, u16 arg3);
extern void *sub_80220C4(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch);
extern void SpawnWumpa(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void nullsub_22(void);
extern void SpawnHoverStartMarker(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80221A4(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnUnderwaterStartMarker(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80221D4(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void nullsub_23(void);
extern void DestroyEntitySpawner(void);
extern void CreateEntitySpawner(void);
extern void *InitLevelState(void *self);

/* src/level/spawn_start_marker.c */
extern void SpawnStartMarker(u32 arg, u16 x, u16 y, u16 z);

/* src/level/terrain.c */
extern s32 GetTerrainFlagsAt(void *arg, s32 x, s32 y);
extern s32 sub_8026BF8(void *player, struct probe_pos *pos, s32 *outValue);
extern s32 sub_8026C3C(void *player, struct probe_pos *pos, s32 *outValue);
extern s32 sub_8026C80(void *arg, s32 arg1, s32 *arg2);
extern s32 sub_8026C8C(void);

/* src/level/terrain_probe.c */
extern s32 ProbeTerrain(void *self, s32 mode, struct probe_pos *pos, s32 span, s32 *outValue);

/* src/level/terrain_probe_axes.c */
extern s32 ProbeTerrainY(struct collider *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode);
extern s32 ProbeTerrainX(struct collider *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode);

/* src/level/tile_cache.c */
extern void DestroyTileCache(void *self, u32 flags);
extern struct tile_cache *nullsub_4(struct tile_cache *self);
extern u16 GetTerrainType(struct tile_cache *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut);

/* src/level/tile_slot_pool.c */
extern void DestroyPooledBgLayer(struct pooled_layer *self, u32 flags);
extern struct pooled_layer *InitPooledBgLayer(struct pooled_layer *self, s32 bgIndex);
extern u32 sub_8026480(struct pooled_layer *self);
extern void ResetTileSlotPool(struct tile_slot_pool *pool);
extern u16 AcquireTileSlot(struct tile_slot_pool *pool, u16 tile);
extern void ReleaseTileSlot(struct tile_slot_pool *pool, u32 tile);
extern void UploadTileSlot(struct tile_slot_pool *pool, s32 tileId, s32 slot);
extern void SetTileSlotPoolSource(struct tile_slot_pool *pool, s32 charBase, u32 src);

/* src/level/time_trial.c */
extern void StartTimeTrial(struct level_state *self);

/* src/cutscene/cutscene_player.c (for ROM order): the tile-map streamer of
 * the BG layers (`struct bg_streamer`) and the BG layer base methods */
extern void DecodeLayerChunk(struct bg_streamer *self, s32 recordId, void *dest);
extern void ScrollBgStreamer(void *self, void *worldpos);
extern void *GetBgStreamerColumn(void *self, s32 x, s32 y, s32 *rowOut);
extern void *GetBgStreamerRow(void *self, s32 x, s32 y, s32 *colOut);
extern u16 GetBgStreamerCell(void *self, s32 x, s32 y);
extern void StreamBgRow(struct bg_streamer *self, s32 row);
extern void StreamBgColumn(struct bg_streamer *self, s32 col);
extern void FillBgStreamer(struct bg_streamer *self, s32 *pos);
extern void SetBgStreamerSource(void *self, void *source);
extern void DestroyBgStreamer(void *self, s32 flags);
extern void *InitBgStreamer(void *self);
extern s32 GetBgStreamerHeight(void *self);
extern s32 GetBgStreamerWidth(void *self);
extern void SetBgStreamerSizeVec(void *self, void *vec);
extern void SetBgStreamerSize(void *self, s32 x, s32 y);
extern void DestroyBgLayerBase(void *self, s32 flags);
extern void *InitBgLayerBase(void *self, s32 unused);
extern s32 ClampBgLayerScrollStep(void *self, s32 value);
extern void ClampBgLayerScrollMax(void *self, s32 *out);
extern void ScaleBgLayerScroll(void *self, void *vec2);
extern void StepBgLayerScroll(void *self, void *delta);

/* The BG layer method tables (src/data/entity_vtables_7e3bec.c) */
extern const struct vtable_slot gBgLayerVtable[10];
extern const struct vtable_slot gPooledBgLayerVtable[10];

/* sym_iwram.txt */
extern struct part_list *gDecorationList;
extern struct level_state *gUnknown_030012C4;
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
extern const entity_spawn_fn gEntitySpawnFuncs[92];

/* src/iwram/iwram_data.c */
extern struct level_layers *gLevelLayersSingleton;
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
extern const struct level_info gLevelTable[25];

/* The level themes' music cues and palette cycles (src/data/level_table_16c814.c) */
extern const u8 gThemeMusicCues[11];
extern const u16 gThemePaletteCycle1A[9];
extern const u16 gThemePaletteCycle1B[9];
extern const u16 gThemePaletteCycle2[5];
extern const u16 gThemePaletteCycle3[16];
extern const u16 gThemePaletteCycle5[5];

#endif /* __LEVEL_H__ */
