#include "bg_layer.hpp"

extern "C" {
#include "core.h"
#include "memory.h"
#include "level.h"
}

/* The collision tile cache (TileCache, include/bg_layer.hpp): its
 * destructor and constructor, which LevelLayers' (level_layers.cpp) run,
 * and the terrain-type lookup (GetTerrainType, for terrain.cpp; a method
 * since #752). C++ since the #664 cleanup; built with old_agbcp, as the C was with old_agbcc (see
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

/* The low byte of the cell at pixel (x, y), or 0 when out of bounds. The
 * top nibble goes to hiOut and the flag nibble to flagsOut. */
u16 TileCache::GetTerrainType(s32 x, s32 y, u8 *flagsOut, s32 *hiOut)
{
    u16 cell;
    u8 nibble;

    if (x < 0 || y < 0)
        return 0;
    cell = CellAt(x, y);
    *hiOut = cell >> 12;
    nibble = (cell >> 8) & 0xf;
    if (nibble)
        *flagsOut = nibble;
    return cell & 0xff;
}
