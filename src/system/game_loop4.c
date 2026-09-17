#include "core.h"

extern void sub_8026ED0(void *self);

/* If bit 0 of `flags` is set, forwards to `sub_8026ED0` - identical
 * shape to `sub_8006FC8` (src/graphics/graphics.c). */
void sub_8025444(void *self, u32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

void nullsub_4(void)
{
}
asm(".align 2, 0");

#if NON_MATCHING
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

extern void *sub_8024F24(struct tile_cache *self, s32 recordId);

/* Same lookup as `sub_80250BC`, but returns the raw decoded halfword
 * directly (no bounds check, no terrain-table lookup) and also writes
 * the cell's top nibble out through `hiOut`.
 *
 * NOT YET BYTE-MATCHING: same register-allocation-permutation gap as
 * `sub_8025130`/`sub_8025228` in game_loop3.c - see
 * docs/matching/issue-40-terrain-tile-cache.md. */
u16 sub_8025460(struct tile_cache *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut)
{
    s32 tileX, tileY, tileIdx;
    void *src;
    u16 recordId;
    u16 *cache;
    u32 wide;
    u16 cell;
    u8 nibble;

    if (x < 0 || y < 0) {
        return 0;
    }

    tileX = x >> 4;
    tileY = y >> 3;
    src = self->source;
    tileIdx = tileY * self->width + tileX;
    recordId = (*(u16 **)src)[tileIdx];
    cache = sub_8024F24(self, recordId);
    wide = cache[((y & 7) << 4) + (x & 0xf)];
    wide <<= 16;
    cell = wide >> 16;

    *hiOut = wide >> 0x1c;
    nibble = (wide >> 0x18) & 0xf;
    if (nibble != 0) {
        *flagsOut = nibble;
    }

    return cell & 0xff;
}
#endif
