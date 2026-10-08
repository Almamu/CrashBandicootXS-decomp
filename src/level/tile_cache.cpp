#include "bg_layer.hpp"

extern "C" {
#include "core.h"
#include "memory.h"
#include "level.h"
}

/* The collision tile cache (TileCache, include/bg_layer.hpp): its
 * destructor and constructor, which LevelLayers' (level_layers.cpp) run,
 * and the terrain-type lookup (C linkage, for terrain.cpp). C++ since the
 * #664 cleanup; built with old_agbcp, as the C was with old_agbcc (see
 * docs/matching/archive/game-loop-old-agbcc.md). */

/* DestroyTileCache: nothing to tear down; g++'s deleting destructor
 * frees the cache when bit 0 of its __in_chrg is set. */
TileCache::~TileCache()
{
}

/* InitTileCache: the empty constructor. */
TileCache::TileCache()
{
}

/* The decoded cell at pixel (x, y): 16x8-pixel tiles, one 256-byte cache
 * slot per tile record. */
static inline u16 GetCell(struct tile_cache *self, s32 x, s32 y)
{
    s32 tileX = x >> 4;
    s32 tileY = y >> 3;
    u16 *buf = (u16 *)GetCollisionChunk(self, (*(u16 **)self->source)[tileY * self->width + tileX]);
    return buf[(y & 7) * 16 + (x & 0xf)];
}

/* The low byte of the cell at pixel (x, y), or 0 when out of bounds. The
 * top nibble goes to hiOut and the flag nibble to flagsOut. */
u16 GetTerrainType(struct tile_cache *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut)
{
    u16 cell;
    u8 nibble;

    if (x < 0 || y < 0)
        return 0;
    cell = GetCell(self, x, y);
    *hiOut = cell >> 12;
    nibble = (cell >> 8) & 0xf;
    if (nibble)
        *flagsOut = nibble;
    return cell & 0xff;
}
