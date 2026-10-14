#ifndef __LEVEL_H__
#define __LEVEL_H__

/* The level subsystem (src/level/): the level state and its accessors,
 * the level layers and BG scroll layers, rooms and their entities, the
 * entity spawners, the camera, terrain and collision maps, and the game
 * frame. Every function src/level/ defines, with the prototype of its
 * definition (the BG layers are C++ classes, include/bg_layer.hpp), and
 * the globals and tables the level files use
 * (docs/headers.md). OperatorNew and the other new/delete operators
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
#include "constants/themes.h"
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

/* The terrain names (#843, docs/flags/terrain.md). A cell's low byte is
 * its terrain type: 0 is empty, 1-0x23 have per-column heights
 * (GetTerrainHeights), 0x24 and above are solid (GetSolidTerrainHeights,
 * GetSolidTerrainModeValue). */
#define TERRAIN_TYPE_NONE 0
#define TERRAIN_TYPE_LAST_HEIGHTS 0x23 // the last type GetTerrainHeights reads

/* The collision mode, the `modeValue`/`heights` index: the direction the
 * probe moves in (LevelLayers::ProbeX's and ProbeY's `submode`, picked by
 * Probe from its PART_DIR_* direction). */
#define TERRAIN_MODE_DOWN 0  // a floor: ProbeY pushes the position up to the cell's top
#define TERRAIN_MODE_RIGHT 1 // ProbeX pushes it left of the cell
#define TERRAIN_MODE_UP 2    // a ceiling: ProbeY pushes it down below the cell
#define TERRAIN_MODE_LEFT 3  // ProbeX pushes it right of the cell

/* A cell's top nibble (bits 12-15): set, a solid cell is open to the
 * probes of one collision mode, which pass through it. */
#define TERRAIN_FLAG_OPEN_RIGHT 1 // TERRAIN_MODE_RIGHT
#define TERRAIN_FLAG_OPEN_LEFT 2  // TERRAIN_MODE_LEFT
#define TERRAIN_FLAG_OPEN_DOWN 4  // TERRAIN_MODE_DOWN
#define TERRAIN_FLAG_OPEN_UP 8    // TERRAIN_MODE_UP

/* The entity flags (`gEntityFlags`) are LevelEntityFlags, include/entity_flags.hpp. */

/* src/level/pooled_bg_layer.cpp (the BG layers' methods are BgLayer's and
 * PooledBgLayer's, include/bg_layer.hpp) */
extern void nullsub_26(void);

/* src/level/level_layers.cpp (LevelLayers' methods: bg_layer.hpp) */
extern s32 sub_8026A14(void);

/* src/level/level_state.cpp (LevelState's methods: level_state.hpp) */
extern void nullsub_24(void);

/* src/level/room.cpp */
extern void ResetObjBuffers(void);

/* src/level/spawn_bosses.cpp */
extern void SpawnRoomExit(u32 id, u16 x, u16 y, u16 param);

/* src/level/spawn_gems.cpp: the gems and the crystal are static members
 * of Pickups (include/pickups.hpp, #915) */

/* src/level/spawn_objects.cpp (the decorations', the platforms', the
 * launch pad's and the seal spawner's slots are static members of
 * Pickups and of the classes they create, #915) */
extern void SpawnNoEntity(void);

/* src/level/spawn_pickups.cpp (the powers and the blue gem are static
 * members of Pickups, #915) */
extern void SpawnCrateGemMarker(u32 id, u16 x, u16 y, u16 param);
extern void *CreateTouchableSprite(u32 index, u32 tag, u32 field0A, u32 cx, u16 cy, u16 cw, u16 ch);

/* src/level/spawn_markers.cpp */
extern void SpawnHoverPlayerPosition(void);
extern void SpawnHoverStartMarker(u32 id, u16 x, u16 y, u16 param);
extern void SpawnUnderwaterPlayerPosition(u32 id, u16 x, u16 y, u16 param);
extern void SpawnUnderwaterStartMarker(u32 id, u16 x, u16 y, u16 param);
extern void SpawnPlayerPosition(u32 id, u16 x, u16 y, u16 param);
extern void SpawnStartMarkerStub(void);

/* src/level/spawn_start_marker.cpp */
extern void SpawnStartMarker(u32 id, u16 x, u16 y, u16 param);

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
/* In their namespace (docs/cplusplus.md, "Namespaces"). */
namespace Game
{
namespace Level
{
class LevelState;
class LevelLayers;
} // namespace Level
} // namespace Game
extern Game::Level::LevelState *gGameFrameLevelState;
#else
extern struct level_state *gGameFrameLevelState;
#endif
/* Updated and cleared, but never culled or drawn: the invisible objects,
 * the entity type 0x55 room-exit zones (spawn_bosses.cpp) and
 * PeriodicSpawner::Spawn's spawner. */
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

/* An entity spawner: the entity's id (its index in the room's entity list,
 * which numbers its gone bit), its x/y and its parameter record's index
 * (SpawnEntity passes the id and the entity record's x, y and param; every
 * spawner below takes the same four, as `id, x, y, param`). */
typedef void (*entity_spawn_fn)(u32 id, u16 x, u16 y, u16 param);

/* The entity spawners, indexed by entity type (src/data/dispatch_table_16c6a4.cpp);
 * the few with other parameters are cast. */
extern const entity_spawn_fn gEntitySpawnFuncs[ENTITY_COUNT];

/* src/iwram/iwram_data.cpp */
#ifdef __cplusplus
extern Game::Level::LevelLayers *gLevelLayersSingleton;
#endif

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
extern const u8 gThemeMusicCues[THEME_COUNT];
extern const u16 gThemePaletteCycle1A[9];
extern const u16 gThemePaletteCycle1B[9];
extern const u16 gThemePaletteCycle2[5];
extern const u16 gThemePaletteCycle3[16];
extern const u16 gThemePaletteCycle5[5];

#endif /* __LEVEL_H__ */
