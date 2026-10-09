#include "bg_layer.hpp"

extern "C" {
#include "core.h"
#include "level_data.h"
#include <agb_syscall.h>
#include "level.h"
#include "globals.h"
}

/* TileCache::GetCell (include/bg_layer.hpp). Same lookup as
 * `GetTerrainHeights`/`GetTerrainType`, but returns the raw
 * decoded halfword unfiltered - no bounds check, no output params. */
u16 TileCache::GetCell(s32 x, s32 y)
{
    s32 tileX = x >> 4;
    s32 tileY = y >> 3;
    void *src = source;
    s32 tileIdx = tileY * width + tileX;
    u16 recordId = (*(u16 **)src)[tileIdx];
    u16 *cache = (u16 *)GetChunk(recordId);
    s32 my = y & 7;
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
void TileCache::SetSource(struct level_layer_desc *source)
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

    for (i = 0; i < 16; i++) {
        id[i] = -1;
    }
    nextSlot = 0;
}

/* Sets bit `n & 0x1f` of the (32-bit-word-per-block) bitmap array at
 * `self`, floor-dividing `n` by 32 to find the word (so it behaves
 * correctly for negative `n` too). Returns 1 if the bit was previously
 * clear (newly set), 0 if it was already set. */
s32 SetBitmapBit(void *self, s32 n)
{
    s32 t = n;
    s32 result = 0;
    s32 wordIndex, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;
    word = (s32 *)((u8 *)self + (wordIndex << 2));

    if (!(*word & mask)) {
        *word |= mask;
        result = 1;
    }
    return result;
}

/* Clears bit `n & 0x1f` of the same bitmap array `SetBitmapBit` sets. */
void ClearBitmapBit(void *self, s32 n)
{
    s32 t = n;
    s32 wordIndex, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;
    word = (s32 *)((u8 *)self + (wordIndex << 2));

    *word &= ~mask;
}


/* Zero-fills 32 words (128 bytes) at `dst` via the BIOS `CpuSet`
 * wrapper, 32-bit fixed-source mode. */
void ClearBitmap(void *dst)
{
    s32 zero = 0;

    CpuSet(&zero, dst, CPU_SET_32BIT | CPU_SET_SRC_FIXED | 0x20);
}

/* `ClearBitmap` wrapper that returns the same pointer it clears. */
void *InitBitmap(void *self)
{
    ClearBitmap(self);
    return self;
}
