#include "core.h"
#include "level_data.h"
#include <agb_syscall.h>

extern void *gLevelLayers;

/* See game_loop3.c for the full `tile_cache` doc comment - duplicated
 * here (not shared via a header) since it's only ever accessed through
 * a raw pointer parameter in this cluster of files. */
struct tile_cache {
    void *source;      /* 0x000 */
    void *decodeBase;  /* 0x004 */
    s32 unk008;         /* 0x008 */
    s32 unk00c;          /* 0x00c */
    s32 unk010;           /* 0x010 */
    s32 unk014;            /* 0x014 */
    s32 width;               /* 0x018 */
    s32 height;                /* 0x01c */
    u8 buf[16][0x100];           /* 0x020 - 0x1020 */
    s32 id[16];                    /* 0x1020 - 0x105c */
    s32 nextSlot;                    /* 0x1060 */
};

extern void *GetCollisionChunk(struct tile_cache *self, s32 recordId);

/* Same lookup as `GetTerrainHeights`/`GetTerrainType`, but returns the raw
 * decoded halfword unfiltered - no bounds check, no output params. */
u16 GetCollisionCell(struct tile_cache *self, s32 x, s32 y)
{
    s32 tileX = x >> 4;
    s32 tileY = y >> 3;
    void *src = self->source;
    s32 tileIdx = tileY * self->width + tileX;
    u16 recordId = (*(u16 **)src)[tileIdx];
    u16 *cache = GetCollisionChunk(self, recordId);
    s32 my = y & 7;
    s32 mx = x & 0xf;

    my <<= 4;
    return cache[my + mx];
}

/* Constructs `self` from `source` (see the `tile_cache` comment above):
 * caches the tile-grid pointer, the decode-table base
 * (`gLevelLayers`'s camera offset + `source->assetOffset`), the tile-grid
 * dimensions, and the pixel-dimension fields nothing in this cluster
 * reads back - then resets every cache slot's resident id to -1 and the
 * eviction cursor to 0. Does nothing when `source` is NULL. */
void SetCollisionSource(struct tile_cache *self, struct level_layer_desc *source)
{
    s32 i;

    if (source == NULL) {
        return;
    }

    self->source = source;
    self->decodeBase = (u8 *)*(void **)((u8 *)gLevelLayers + 0x24) + (s32)source->assetOffset;
    self->unk010 = source->widthTiles;
    self->unk014 = source->heightTiles;
    self->unk008 = self->unk010 << 3;
    self->unk00c = self->unk014 << 3;
    self->width = source->gridWidth;
    self->height = source->gridHeight;

    for (i = 0; i < 16; i++) {
        self->id[i] = -1;
    }
    self->nextSlot = 0;
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
