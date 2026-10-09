#ifndef GUARD_LEVEL_DATA_H
#define GUARD_LEVEL_DATA_H

#include "gba/types.h"
#include "constants/levels.h"
#include "constants/rooms.h"
#include "constants/categories.h"

/*
 * The rooms' level data (docs/levels.md), as src/data/level_rooms_*.c
 * define it. tools/levels.py generates those definitions from the
 * editable sources in data/levels/. The code reads the same records
 * through its own local views, named on each struct below.
 */

/* One layer (BG0-3 or the collision layer): the BG layers' and their
 * streamers' source (include/bg_layer.hpp), and the raw `source` of the
 * terrain cache (tile_cache.cpp/tile_cache_cell.cpp). */
struct level_layer_desc {
    const u16 *chunkGrid; // 0x00 - gridWidth x gridHeight chunk ids, row-major
    u32 assetOffset;      // 0x04 - this layer's section in the level asset
    const void *tileData; // 0x08 - BG tile set (NULL for the collision layer)
    s32 scaleX;           // 0x0C - Q8 parallax factor
    s32 scaleY;           // 0x10
    u16 cnt;              // 0x14 - BGnCNT bits (the priority is used)
    u16 gridWidth;        // 0x16 - in 16x8-cell chunks
    u16 gridHeight;       // 0x18
    u16 widthTiles;       // 0x1A
    u16 heightTiles;      // 0x1C
    u16 unk_1E;           // 0x1E - 2 in every room
};

/* One entity spawn record: `type` indexes the spawn-function table
 * (gEntitySpawner, dispatched by SpawnEntity), which gets the entity's
 * id, x, y and param; `param` indexes the room's parameter records
 * (`struct level_entity_list.paramOffsets`). */
struct level_entity {
    u16 type;
    u16 x; // pixels
    u16 y;
    u16 param;
};

/* The entities of one 256-px column (x >> 8). */
struct level_entity_group {
    u16 first; // entities in the columns before this one
    u16 count;
    const struct level_entity *entities;
};

/* A room's entities (`gEntityFlags->list`, level.h). The local views
 * were `struct lk_list` (room_entities.cpp), `struct collect_info`
 * (dingodile.c), `struct level_record_table` (the enemy spawners'
 * text_popup.h), `struct placement_level` (crate_create.cpp) and
 * CreatePlatform's level header. */
struct level_entity_list {
    u16 count;                               // 0x00 - all entities
    u16 groupCount;                          // 0x02
    const struct level_entity_group *groups; // 0x04
    const u16 *paramOffsets;                 // 0x08 - byte offset of each parameter record
    // 0x0C - the records: a flags word, then per-type words
    const u32 *params;
    const u16 *typeCounts; // 0x10 - entities per type
};

/* One entity's parameter record (was the enemy spawners' text_popup.h's
 * `struct level_record`), at `params + paramOffsets[id]` (bytes) in the
 * room's entity list (gEntityFlags->list). After the flags come
 * per-type parameter words; `p` has one view per layout, named after
 * the EnemyCtrl setters spawn_enemies.cpp hands them to (SetEnemyRangeX:
 * `rangeX`/`rangeY` are pixels either side of the spawn point;
 * SetEnemyRangeXSpeed/SetEnemyRangeYSpeed: `speed`/`accel`;
 * SetEnemyAttackCycle: `idleTime`/`attackTime`/`cycleOffset`;
 * SetEnemyWave: `period`/`phase`/`amplitude`). */
struct entity_params {
    u8 flags; // bit 1: clear = X-mirrored (the X mirror bit), bit 2: Y-mirrored
    union {
        // lizard, patrolling jungle enemy, polar bear, patrolling sewer enemy, rat
        struct {
            s32 rangeX; // 0x04
        } patrol;
        // blowgun tribesman, pufferfish (with the wave), piston crusher, wooden
        // crusher; the saucer lab assistant reads only `cycleOffset`
        struct {
            s32 idleTime;      // 0x04
            s32 attackTime;    // 0x08
            s32 cycleOffset;   // 0x0C
            s32 waveAmplitude; // 0x10
            s32 wavePeriod;    // 0x14
            s32 wavePhase;     // 0x18
        } cycle;
        // stationary space enemy, flamethrower lab assistant
        struct {
            s32 attackTime;  // 0x04
            s32 idleTime;    // 0x08
            s32 cycleOffset; // 0x0C
        } attackFirst;
        // electric eel, patrolling space enemy
        struct {
            s32 rangeX;      // 0x04
            s32 idleTime;    // 0x08
            s32 attackTime;  // 0x0C
            s32 cycleOffset; // 0x10
        } rangeCycle;
        // penguin
        struct {
            s32 rangeX;      // 0x04
            s32 attackTime;  // 0x08
            s32 idleTime;    // 0x0C
            s32 cycleOffset; // 0x10
        } rangeAttackFirst;
        // shark: X range, Y homing
        struct {
            s32 rangeX; // 0x04
            s32 rangeY; // 0x08
            s32 speedY; // 0x0C
            s32 accelY; // 0x10
        } rangeHomingY;
        // jellyfish
        struct {
            s32 amplitude; // 0x04
            s32 period;    // 0x08
            s32 phase;     // 0x0C
        } wave;
        // homing sewer enemy
        struct {
            s32 rangeX; // 0x04
            s32 speedX; // 0x08
            s32 accelX; // 0x0C
        } homingX;
        // sea mine
        struct {
            s32 rangeY; // 0x04
            s32 speedY; // 0x08
            s32 accelY; // 0x0C
            s32 rangeX; // 0x10
            s32 speedX; // 0x14
            s32 accelX; // 0x18
        } homingXY;
    } p;
};

/* Was `struct lk_link` in room_entities.cpp: chains entity `from` to entity
 * `to` (entity ids = spawn order). */
struct level_link {
    s32 from;
    s32 to;
};

struct level_link_list {
    s32 count;
    struct level_link links[1]; // `count` of them
};

/* One room: `struct level_desc` in level_layers.cpp. */
struct level_desc {
    const struct level_layer_desc *layers[3]; // 0x00 - BG1-3
    const struct level_layer_desc *layer0;    // 0x0C - BG0 (tile-slot pooled)
    const struct level_layer_desc *collision; // 0x10 - the terrain cache's
    const void *asset;                        // 0x14 - chunk streams
    u8 assetPacked;                           // 0x18 - asset is LZ77
    const struct level_entity_list *entities; // 0x1C
    const struct level_link_list *links;      // 0x20 - or NULL
    u8 unk_24[0xC];                           // 0x24 - zero in every room
};

/*
 * One room record of the level table (src/data/level_table_16c814.c):
 * the record RunRoom hands to LoadRoom, and the current room
 * (`LevelProgress::cat`, `LevelState::room.cat`). It merges level_layers.cpp's
 * `struct level_load_args` (batch 8b), level_query.cpp's `MedalListItem`
 * (`linkedObj`/`type` are `desc`/`kind`), level_state.h's `struct
 * level_category`, play_room.cpp's `gl_widget_kind` (#574, batch 9e) and
 * play_room.cpp's `level_blend` (`mode` was `kind`, `effect`/`eva`/`evb`
 * are `param.blend`; #656, batch 7).
 */

/* A room's colour-effect settings (SetupRoomBlend): the BLDCNT effect and
 * the BLDALPHA weights. Every room that sets them uses alpha blending
 * (effect 1) at EVA 4, EVB 16. */
struct level_room_blend {
    u16 effect; // 0x00 - 0: no blending (a fixed 16/16 alpha pattern)
    u8 eva;     // 0x02 - first-target weight
    u8 evb;     // 0x03 - second-target weight
};

struct level_room {
    const u16 *palette;            // 0x00 - BG palette, 256 colours
    const struct level_desc *desc; // 0x04 - NULL for a category stage
    // 0x08 - ROOM_KIND_*: 0-2 a room; 3 a stage played in actor category
    // `param.catIndex`. SetupRoomBlend raises the sprites' priority in
    // an underwater room (1)
    s32 kind;
    s32 unk_0C; // 0x0C - 0 in every record
    union {
        struct level_room_blend blend; // kinds 0-2
        // kind 3: the actor category (CountCategoryCrates, InitActorCategory)
        u16 catIndex;
    } param; // 0x10
};

/* A level's rooms (level_query.cpp's `MedalItemList` was a view: `items` is
 * `rooms`). */
struct level_room_list {
    s32 count;
    const struct level_room *const *rooms; // `count` rooms, in play order
    const struct level_room *extra1;       // or NULL
    const struct level_room *extra2;       // or NULL
};

/*
 * One level (gLevelTable, level.h). Every user reads the table through
 * this struct; the local views `threshold_table_entry`, `MedalTableEntry`,
 * `level_guard` and `gl_level_entry` were merged into it (#574, batch 8b).
 */
struct level_info {
    s32 nameText;          // 0x00 - text id of the level's name (GetUiText)
    u32 theme;             // 0x04 - picks the level-start colour cycle
                           //        (RunRoom) and indexes the music cues
                           //        gThemeMusicCues (PlayRoomMusic)
    u32 times[3];          // 0x08 - time-trial thresholds, centiseconds,
                           //        loosest first
    s32 maskAssistDeaths;  // 0x14 - level_state.maskAssistDeaths (SetMaskAssistDeaths)
    s32 crateAssistDeaths; // 0x18 - level_state.crateAssistDeaths (SetCrateAssistDeaths)
    u8 isBoss;             // 0x1C - 1 for the five boss levels (tiny, dingodile,
                           //        n. gin, neo cortex, mega-mix); RunRoom runs
                           //        CheckAllCratesBroken at level start only if 0
    const struct level_room_list *rooms; // 0x20
};

/* A link list with room for `n` links. */
#define LEVEL_LINKS(n) struct { s32 count; struct level_link links[n]; }

/* Byte offset of a parameter record in a room's parameter struct. */
#define LEVEL_PARAM_OFFSET(type, member) ((u16)(u32)&((type *)0)->member)

#endif // GUARD_LEVEL_DATA_H
