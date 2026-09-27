#include "core.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

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

/* The decoded cell at pixel (x, y): 16x8-pixel tiles, one 256-byte cache
 * slot per tile record. */
static inline u16 GetCell(struct tile_cache *self, s32 x, s32 y)
{
    s32 tileX = x >> 4;
    s32 tileY = y >> 3;
    u16 *buf = sub_8024F24(self, (*(u16 **)self->source)[tileY * self->width + tileX]);
    return buf[(y & 7) * 16 + (x & 0xf)];
}

/* The low byte of the cell at pixel (x, y), or 0 when out of bounds. The
 * top nibble goes to hiOut and the flag nibble to flagsOut. */
u16 sub_8025460(struct tile_cache *self, s32 x, s32 y, u8 *flagsOut, s32 *hiOut)
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
