#include "core.h"
#include "memory.h"
#include "level.h"

/* Built with old_agbcc - see docs/matching/archive/game-loop-old-agbcc.md. */

/* If bit 0 of `flags` is set, forwards to `OperatorDelete` - identical
 * shape to `DestroySpriteBankSet` (src/gfx/graphics.cpp). */
void DestroyTileCache(void *self, u32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* The tile cache's empty constructor (InitLevelLayers). */
struct tile_cache *InitTileCache(struct tile_cache *self)
{
    return self;
}

/* The decoded cell at pixel (x, y): 16x8-pixel tiles, one 256-byte cache
 * slot per tile record. */
static inline u16 GetCell(struct tile_cache *self, s32 x, s32 y)
{
    s32 tileX = x >> 4;
    s32 tileY = y >> 3;
    u16 *buf = GetCollisionChunk(self, (*(u16 **)self->source)[tileY * self->width + tileX]);
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
