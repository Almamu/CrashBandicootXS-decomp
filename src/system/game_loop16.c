#include "core.h"
#include "bg_scroll_layer.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

extern void sub_8024E68(struct bg_scroll_layer *self, void *vec2);
extern void sub_803AD84(void *self, s32 lo, s32 hi, void *fn);
extern void sub_8024AA0(void *streamer, struct bg_scroll_layer *self);
extern void sub_8025E2C(struct bg_scroll_layer *self, s32 lo, s32 hi);
extern void sub_8025DE8(struct bg_scroll_layer *self, s32 lo, s32 hi);

static inline void CallClip(struct bg_scroll_layer *self, struct bg_layer_method *m, s32 lo, s32 hi)
{
    sub_803AD84((u8 *)self + m->thisOffset, lo, hi, m->fn);
}

extern u16 *sub_8024B18(void *streamer, s32 col, s32 row, s32 *rowOut);

static inline s32 Mod32(s32 v)
{
    return v % 32;
}

/* Moves the layer (sub_8024E68), then works out the tile columns and
 * rows the 240x160 screen covers at the new position, clips the
 * resident range to them through the clipRows/clipCols methods,
 * refreshes the streamer and streams the new range in
 * (sub_8025E2C/sub_8025DE8). */
void sub_8025E98(struct bg_scroll_layer *self, void *vec2)
{
    s32 colLo;
    s32 colHi;
    s32 rowLo;
    s32 rowHi;

    sub_8024E68(self, vec2);
    colLo = self->x / 8;
    colHi = (self->x + 0xef) / 8;
    rowLo = self->y / 8;
    rowHi = (self->y + 0x9f) / 8;
    CallClip(self, &self->vtable->clipRows, rowLo, rowHi);
    CallClip(self, &self->vtable->clipCols, colLo, colHi);
    sub_8024AA0(self->streamer, self);
    sub_8025E2C(self, colLo, colHi);
    sub_8025DE8(self, rowLo, rowHi);
}

/* Truncates the Q8 X/Y position (self+0/self+4) to plain tile-scroll
 * halfwords at self+0x54/self+0x56 (read back together as one 32-bit
 * word), then writes that packed pair through the pointer at
 * self+0x58 - the `BGnHOFS`/`BGnVOFS` register pair address
 * `sub_8025D74` (game_loop15.c) caches there. */
void sub_8025F24(void *self)
{
    s32 x = *(s32 *)self;
    u8 *dst1 = (u8 *)self + 0x54;

    *(s16 *)dst1 = x;
    {
        s32 y = *(s32 *)((u8 *)self + 4);
        u8 *dst2 = (u8 *)self + 0x56;

        *(s16 *)dst2 = y;
    }
    *(s32 *)(*(void **)((u8 *)self + 0x58)) = *(s32 *)((u8 *)self + 0x54);
}

/* Base `drawCol` (table +0x38): copies the resident rows of map column
 * `col` from the streamer into screen column `col % 32`. */
void sub_8025F3C(struct bg_scroll_layer *self, s32 col)
{
    s32 srcRow;
    u16 *src = sub_8024B18(self->streamer, col, self->rowLo, &srcRow);
    s32 i = Mod32(self->rowLo) * 32 + Mod32(col);
    s32 r;

    for (r = self->rowLo; r <= self->rowHi; r++)
    {
        self->screen[i] = src[srcRow * 64];
        srcRow = (srcRow + 1) & 0x1f;
        i = (i + 32) % 0x400;
    }
}
