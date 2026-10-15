#include "game/level/bg_layer.hpp"

extern "C" {
#include "core.h"
#include "game/level/level_data.h"
#include "game/level/level.h"
#include "globals.h"
}

using Game::Level::TileCache;

/* The collision tile cache's cell lookup and source (TileCache,
 * engine/include/game/level/bg_layer.hpp), ROM 0x080254C0-0x08025554; the rest of the class
 * is in tile_cache.cpp, built with old_agbcp (this file is agbcp). This
 * file was collision_map.cpp until #770, which split its bitmap helpers
 * into entity_bitmap.cpp. */

/* TileCache::GetCell (engine/include/game/level/bg_layer.hpp). Same lookup as
 * `GetTerrainHeights`/`GetTerrainType`, but returns the raw
 * decoded halfword unfiltered - no bounds check, no output params. */
u16 TileCache::GetCell(s32 x, s32 y)
{
    /* `tileX` is needed: inlined, the instructions differ */
    s32 tileX = x >> 4;
    /* `tileY` is needed: inlined, the instructions are reordered */
    s32 tileY = y >> 3;
    /* `cache` is needed: inlined, the code is 2 instructions shorter */
    u16 *cache = (u16 *)GetChunk(source->chunkGrid[tileY * width + tileX]);
    s32 my = y & 7;
    /* `mx` is needed: inlined, the instructions are reordered */
    s32 mx = x & 0xf;

    my <<= 4;
    return cache[my + mx];
}

/* TileCache::SetSource: points the cache at `source`:
 * caches the tile-grid pointer, the decode-table base
 * (`gLevelLayers`'s camera offset + `source->assetOffset`), the tile-grid
 * dimensions, and the pixel-dimension fields nothing in this cluster
 * reads back - then resets every cache slot's resident id to -1 and the
 * eviction cursor to 0. Does nothing when `source` is NULL. */
void TileCache::SetSource(const struct level_layer_desc *source)
{
    s32 i;

    if (source == NULL) {
        return;
    }

    this->source = source;
    decodeBase = (u8 *)gLevelLayers->asset + (s32)source->assetOffset;
    widthTiles = source->widthTiles;
    heightTiles = source->heightTiles;
    widthPx = widthTiles << 3;
    heightPx = heightTiles << 3;
    width = source->gridWidth;
    height = source->gridHeight;

    for (i = 0; i < (s32)ARRAY_COUNT(id); i++) {
        id[i] = -1;
    }
    nextSlot = 0;
}
