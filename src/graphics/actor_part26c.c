#include "core.h"

/* Same boss-weapon subsystem as actor_part20.c/actor_part26b.c - see
 * actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Per docs/rom_map.md ("A new mechanism: a procedurally-generated VRAM
 * fill-level meter"): a near-identical twin of
 * `sub_80336CC` (outside this chunk, issue #62's range) that
 * procedurally generates a vertical meter/fill-level tile graphic.
 * Indexes `gStaticData_08167AD4`'s per-level table via the
 * `gUnknown_03001528`/`gUnknown_0300152C` fields (row/column counts,
 * both capped near 32), sums four rows' worth of heights into
 * `gUnknown_03001530`, computes `0xFF - sum` as a fill level, and DMAs
 * the result - packed two nibbles per byte from each row's raw byte
 * data - as 4-bit tile data into VRAM (`0x06008000`), a health-bar/
 * water-level-style meter built fresh per frame from up to 4 rows of
 * `gUnknown_03001580`-indexed level data.
 *
 * Matched as plain C with the fixes that closed the one-row twin
 * `sub_80336CC` (actor_part130.c, see
 * docs/matching/near-miss-polish-3.md). */
extern s32 gUnknown_03001528;
extern s32 gUnknown_0300152C;
extern s32 gUnknown_03001530;
extern u8 gStaticData_08167AD4[];
extern u8 *gUnknown_03001580[];

/* The height is re-read after the row-pointer store (the ROM's `ldm
 * r1!`), the second loop has its own counter, its header is written in
 * the ROM's order, and the 0xf mask comes from an `asm` so that it is
 * the AND's first operand (the ROM copies the mask, not the byte).
 * Matches under both compilers. */
static inline u32 MeterPx(u32 v)
{
    u32 r = 0;
    if (v != 0)
        r = 0x10 | v;
    return r;
}

void sub_8031604(void)
{
    s32 heights[4];
    u32 stride;
    s32 sum = 0;
    s32 off = 0x204;
    s32 k;
    s32 row_i;
    u32 *dst;
    u8 **rows = gUnknown_03001580;
    u32 m;

    stride = (u32)(gUnknown_03001528 * gUnknown_0300152C + 1) >> 1 << 2;
    for (k = 0; k < 4; k++) {
        s32 x = *(s32 *)(gStaticData_08167AD4 + off);
        heights[k] = x;
        sum += x;
        off += 4;
        rows[k] = gStaticData_08167AD4 + off;
        /* forces the height to be re-read (the ROM's `ldm r1!`) */
        asm("" : "+m"(heights[k]));
        off += stride;
        off += heights[k] << 5;
    }
    gUnknown_03001530 = 0xFF - sum;
    dst = (u32 *)(((0xFF - sum) << 6) + BG_CHAR_ADDR(2));
    for (row_i = 0; row_i <= 3; row_i++) {
        u8 *src;
        u8 *row;
        s32 *hp;
        s32 n;
        s32 j;
        u32 *d;

        row = gUnknown_03001580[row_i];
        hp = &heights[row_i];
        d = dst;
        src = row + stride;
        n = *hp;

        for (j = 0; j < n << 4; j++) {
            u32 b, c, p0, p1, p2, p3;

            /* the 0xf mask without a constant-set register: the mask is
             * the AND's first operand, as in the ROM */
            asm("" : "=r"(m) : "0"(0xf));
            b = *src;
            p0 = m & b;
            p0 = MeterPx(p0);
            p1 = (b >> 4) & m;
            src++;
            p1 = MeterPx(p1);
            c = *src;
            p2 = m & c;
            p2 = MeterPx(p2);
            p3 = (c >> 4) & m;
            src++;
            p3 = MeterPx(p3);
            *d++ = p0 | (p1 << 8) | (p2 << 16) | (p3 << 24);
        }
        dst = d;
    }
}
