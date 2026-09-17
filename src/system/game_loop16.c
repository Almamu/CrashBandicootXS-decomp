#include "core.h"

#if NON_MATCHING
extern void sub_8024E68(void *self, void *vec2);
extern void sub_803AD84(void *addr, s32 a1, s32 a2, void *fn);
extern void sub_8024AA0(void *arg0, void *self);
extern void sub_8025E2C(void *self, s32 lo, s32 hi);
extern void sub_8025DE8(void *self, s32 lo, s32 hi);

/* Floor-divide-by-8 helper matching the ROM's "add 7 before the
 * arithmetic shift when negative" idiom, used four times in a row
 * below. */
static s32 Div8Floor(s32 v)
{
    if (v >= 0) {
        return v >> 3;
    }
    return (v + 7) >> 3;
}

/* Computes the four screen-edge tile coordinates from self's Q8
 * position (self+0/self+4) against the 240x160 GBA screen (0xef/0x9f
 * are one pixel short of the full extent, keeping the edge tile
 * inclusive), re-derives the streaming layer via `sub_8024E68`/
 * `sub_8024AA0`, fires the two `self->0x30`-table-driven trampolines
 * (`+0x48`/`+0x40` records) with the tile bounds via `sub_803AD84`
 * (whose function-pointer argument naturally lands in `r3`, the 4th
 * AAPCS register, so - unlike `sub_8025D28`'s `sub_803AD8C`/"bx r5"
 * case - no register pin is needed here), then clamps the streamed
 * range via `sub_8025E2C`/`sub_8025DE8` (game_loop15.c).
 *
 * PARKED, NOT BYTE-MATCHING: every field/offset/call confirmed against
 * the ROM, including which axis's tile pair goes to which trampoline
 * record and which axis's pair goes to which clamp call (the ROM
 * pairs `self->0x30+0x48`/`sub_8025E2C` with the Y-derived tiles and
 * `self->0x30+0x40`/`sub_8025DE8` with the X-derived ones). Not
 * iterated to an exact register allocation within this issue's chunk -
 * a large function with `r8` spanning most of it. See
 * docs/matching/issue-41-game-loop-25894.md. */
void sub_8025E98(void *self, void *vec2)
{
    u8 *s = (u8 *)self;
    s32 x = *(s32 *)s;
    s32 y = *(s32 *)(s + 4);
    s32 xTileMin = Div8Floor(x);
    s32 xTileMax = Div8Floor(x + 0xef);
    s32 yTileMin = Div8Floor(y);
    s32 yTileMax = Div8Floor(y + 0x9f);

    sub_8024E68(self, vec2);

    {
        u8 *rec = *(u8 **)(s + 0x30) + 0x48;
        s32 off = *(s16 *)rec;
        void *addr = s + off;
        void *fn = *(void **)(rec + 4);

        sub_803AD84(addr, yTileMin, yTileMax, fn);
    }
    {
        u8 *rec = *(u8 **)(s + 0x30) + 0x40;
        s32 off = *(s16 *)rec;
        void *addr = s + off;
        void *fn = *(void **)(rec + 4);

        sub_803AD84(addr, xTileMin, xTileMax, fn);
    }

    sub_8024AA0(*(void **)(s + 0x2c), self);

    sub_8025E2C(self, xTileMin, xTileMax);
    sub_8025DE8(self, yTileMin, yTileMax);
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

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

#if NON_MATCHING
extern void *sub_8024B18(void *arg0, s32 arg1, s32 *outCol);

/* Floor-divide-by-32 (rows) helper, same idiom as the bitmap-grid
 * family in game_loop12.c/game_loop13.c. */
static s32 FloorDiv32(s32 v)
{
    if (v < 0) {
        v += 0x1f;
    }
    return v >> 5;
}

/* Streams decoded tile data into the circular row buffer at self+0x4c,
 * for rows self->0x3c..self->0x40 inclusive: `sub_8024B18` resolves
 * the decode table (self->0x2c, self->0x3c, and a column-cursor output
 * slot on the stack) once, then each row copies a halfword from the
 * decode table (indexed by the column cursor, wrapping every 32
 * columns via a 128-byte-stride column bank) into the circular buffer
 * at a position derived from `self`'s/`arg1`'s combined 32x32-tile
 * offset, advancing that combined offset by 1 row each time with
 * wraparound at 0x400 (32*32) tiles.
 *
 * PARKED, NOT BYTE-MATCHING: the control flow, the two floor-mod
 * computations, the 128-byte column stride and the 1024-tile wraparound
 * are all confirmed against the ROM's raw operations, but this is a
 * dense, register-heavy loop (the ROM keeps `r8` live across the whole
 * function too) that was not iterated to an exact register allocation
 * within this issue's chunk. See docs/matching/issue-41-game-loop-25894.md. */
void sub_8025F3C(void *self, s32 arg1)
{
    u8 *s = (u8 *)self;
    s32 rowStart = *(s32 *)(s + 0x3c);
    s32 rowEnd = *(s32 *)(s + 0x40);
    s32 col;
    void *table = sub_8024B18(*(void **)(s + 0x2c), rowStart, &col);
    s32 rowMod = rowStart - (FloorDiv32(rowStart) << 5);
    s32 colMod = arg1 - (FloorDiv32(arg1) << 5);
    s32 tileIdx = (rowMod << 5) + colMod;
    s32 row;
    u16 *buf;

    if (rowStart > rowEnd) {
        return;
    }

    buf = *(u16 **)(s + 0x4c);
    for (row = rowStart; row <= rowEnd; row++) {
        buf[tileIdx] = *(u16 *)((u8 *)table + (col << 7));

        col = (col + 1) & 0x1f;

        tileIdx += 0x20;
        if (tileIdx >= 0x400) {
            tileIdx -= 0x400;
        }
    }
}
#endif /* NON_MATCHING */
asm(".align 2, 0");
