#ifndef __LEVEL_H__
#define __LEVEL_H__

/* The level subsystem (src/level/): the level state and its accessors,
 * the level layers and BG scroll layers, rooms and their entities, the
 * entity spawners, the camera, terrain and collision maps, and the game
 * frame. Every function src/level/ defines, with the prototype of its
 * definition (the BG layers are C++ classes, include/bg_layer.hpp), and
 * the globals and tables the level files use
 * (docs/headers_plan.md). OperatorNew and the other new/delete operators
 * (operator_new.cpp) are in memory.h. A .c file that needs a different local
 * declaration for codegen keeps it as an asm-label alias with a
 * `codegen:` comment.
 *
 * Many functions take a file-local view of their object; this header
 * declares those by tag. */

#include "core.h"
#include "aabb.h"
#include "constants/level_flags.h"
#include "constants/mask_level.h"
#include "level_data.h"
#include "constants/bosses.h"
#include "bg_scroll_layer.h"
#include "constants/entities.h"
#include "constants/chunk_tokens.h"

/* The terrain types of the level collision maps (tile_cache.cpp): a
 * cell's low byte picks one (0x24 and above are solid, 0 is empty, and
 * GetTerrainHeights stops at 0x23), `modeValue` is a value per collision mode
 * (GetSolidTerrainModeValue) and `heights` the surface height of each of the cell's
 * 8 pixel columns per mode, 0-7, 0xFF where there is none (GetTerrainHeights,
 * GetSolidTerrainHeights). */
struct terrain_type {
    u8 modeValue[4];
    u8 heights[4][8];
};

/* The entity flags (`gEntityFlags`) are LevelEntityFlags, include/entity_flags.hpp. */

/* src/level/pooled_bg_layer.cpp (the BG layers' methods are BgLayer's and
 * PooledBgLayer's, include/bg_layer.hpp) */
extern void nullsub_26(void);

/* src/level/level_layers.cpp (LevelLayers' methods: bg_layer.hpp) */
extern s32 sub_8026A14(void);

/* src/level/level_query.cpp (C linkage) */
extern s32 CountLevelCrates(s32 idx);
extern s32 LevelHasEntityType(s32 idx, s32 flagIdx);
extern s32 LevelHasYellowGemEntity(s32 idx);
extern s32 LevelHasBlueGemEntity(s32 idx);
extern s32 LevelHasGreenGemEntity(s32 idx);
extern s32 LevelHasRedGemEntity(s32 idx);
extern s32 LevelHasGemPathGemEntity(s32 idx);
extern s32 CountRoomCrates(const struct level_room *item);

/* src/level/level_state.cpp (LevelState's methods: level_state.hpp) */
extern void nullsub_24(void);
#ifdef __cplusplus
extern class LevelState *GetLevelState(void);
#else
extern struct level_state *GetLevelState(void);
#endif

/* src/level/room.cpp */
extern void ClearRoomExit(void);
extern void RequestRoomExit(void);
extern u8 IsRoomExitRequested(void);
extern void ResetObjBuffers(void);

/* src/level/spawn_bosses.cpp */
extern void SpawnRoomExit(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnDingodile(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTiny(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCortexBoss(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnMegaMix(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_crates.cpp */
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

/* src/level/spawn_pickups.cpp */
extern void SpawnBodySlamPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTornadoSpinPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnDoubleJumpPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnTurboRunPower(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnStopwatch(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnBlueGem(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void SpawnCrateGemMarker(u32 arg, u16 arg1, u16 arg2, u16 arg3);
extern void *CreateTouchableSprite(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch);
extern void SpawnWumpa(u32 arg, u16 arg1, u16 arg2, u16 arg3);

/* src/level/spawn_markers.cpp */
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

/* src/level/terrain.cpp (the terrain probes are LevelLayers' methods:
 * bg_layer.hpp) */
extern s32 sub_8026C8C(void);

/* sym_iwram.txt */
#ifdef __cplusplus
/* The decoration parts (PartList, sprite_obj.hpp; C++ only). */
extern class PartList *gDecorationList;
#endif
/* UpdateGameFrame's level state, stored once when the game starts and never
 * read. */
#ifdef __cplusplus
extern class LevelState *gGameFrameLevelState;
#else
extern struct level_state *gGameFrameLevelState;
#endif
/* Updated and cleared, but never culled or drawn: the invisible objects,
 * the entity type 0x55 room-exit zones (spawn_bosses.cpp) and
 * SpawnSealSpawner's spawner. */
#ifdef __cplusplus
extern class PartList *gUpdateOnlyPartList;
#endif

/* The enemies' anim maps (src/data/popup_tables_16b98c.cpp), stored in their
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

/* The entity spawners, indexed by entity type (src/data/dispatch_table_16c6a4.cpp);
 * the few with other parameters are cast. */
extern const entity_spawn_fn gEntitySpawnFuncs[ENTITY_COUNT];

/* src/iwram/iwram_data.cpp */
#ifdef __cplusplus
extern class LevelLayers *gLevelLayersSingleton;
#endif
#ifdef __cplusplus
extern class LevelState *gLevelStateSingleton;
#else
extern struct level_state *gLevelStateSingleton;
#endif
extern u8 gRoomExitRequested;

/* The terrain height tables (src/data/terrain_1725a8.cpp, top-level asm) */
extern u8 gTerrainHeights0[];
extern u8 gTerrainHeights1[];
extern u8 gTerrainHeights2[];
extern u8 gTerrainHeights3[];

/* src/data/terrain_1725a8.cpp */
extern const struct terrain_type gTerrainTypes[51];

/* The level table (src/data/level_table_16c814.cpp, level_data.h). */
extern const struct level_info gLevelTable[LEVEL_COUNT];

/* The level themes' music cues and palette cycles (src/data/level_table_16c814.cpp) */
extern const u8 gThemeMusicCues[11];
extern const u16 gThemePaletteCycle1A[9];
extern const u16 gThemePaletteCycle1B[9];
extern const u16 gThemePaletteCycle2[5];
extern const u16 gThemePaletteCycle3[16];
extern const u16 gThemePaletteCycle5[5];

#endif /* __LEVEL_H__ */
