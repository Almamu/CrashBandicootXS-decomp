#include "core.h"
#include <libgcc.h>
#include "bosses.h"

/* Same boss-weapon subsystem as airship_fireball.c/airship_load_graphics.c - see
 * airship_fireball.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md.
 *
 * Per docs/rom_map.md ("A new mechanism: a procedurally-generated VRAM
 * fill-level meter"): a near-identical twin of
 * `ConvertHovercraftTiles` (outside this chunk, issue #62's range) that
 * procedurally generates a vertical meter/fill-level tile graphic.
 * Indexes `gAirshipPalette`'s per-level table via the
 * `gAirshipMapCols`/`gAirshipMapRows` fields (row/column counts,
 * both capped near 32), sums four rows' worth of heights into
 * `gAirshipMapTileBase`, computes `0xFF - sum` as a fill level, and DMAs
 * the result - packed two nibbles per byte from each row's raw byte
 * data - as 4-bit tile data into VRAM (`0x06008000`), a health-bar/
 * water-level-style meter built fresh per frame from up to 4 rows of
 * `gAirshipMapFrames`-indexed level data.
 *
 * Matched as plain C with the fixes that closed the one-row twin
 * `ConvertHovercraftTiles` (hovercraft.c, see
 * docs/matching/near-miss-polish-3.md). */

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

void ConvertAirshipTiles(void)
{
    s32 heights[4];
    u32 stride;
    s32 sum = 0;
    s32 off = 0x204;
    s32 k;
    s32 row_i;
    u32 *dst;
    u8 **rows = gAirshipMapFrames;
    u32 m;

    stride = (u32)(gAirshipMapCols * gAirshipMapRows + 1) >> 1 << 2;
    for (k = 0; k < 4; k++) {
        s32 x = *(const s32 *)((const u8 *)gAirshipPalette + off);
        heights[k] = x;
        sum += x;
        off += 4;
        rows[k] = (u8 *)gAirshipPalette + off;
        /* forces the height to be re-read (the ROM's `ldm r1!`) */
        asm("" : "+m"(heights[k]));
        off += stride;
        off += heights[k] << 5;
    }
    gAirshipMapTileBase = 0xFF - sum;
    dst = (u32 *)(((0xFF - sum) << 6) + BG_CHAR_ADDR(2));
    for (row_i = 0; row_i <= 3; row_i++) {
        u8 *src;
        u8 *row;
        s32 *hp;
        s32 n;
        s32 j;
        u32 *d;

        row = gAirshipMapFrames[row_i];
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

/* Same boss-weapon subsystem as airship_fireball.c - see that file's header
 * comment and docs/matching/issue-58-0x08030334-actor.md. */

/* Sets BG palette bank 1's last color (index 15) to either a near-white
 * flash color or a dim default, gated by bit 3 of `gAirshipStateTimer`
 * (a flags word driving this effect's per-frame look). */
void UpdateAirshipFlashColor(void)
{
    vu16 *bank1 = (vu16 *)(BG_PLTT + 0x20);

    if (gAirshipStateTimer & 8) {
        bank1[0xf] = 0x7fff;
    } else {
        bank1[0xf] = 0x1f;
    }
}

extern s32 QueueVramDmaTransfer(void *arg0, void *arg1, u16 arg2, u16 arg3);

/* While `gAirshipHitFlashTimer`'s DMA-refresh counter is armed, decrements
 * it and re-queues one "frame" of `gAirshipHitFlashPalettes`'s palette
 * animation strip into BG palette bank 1 - `__divsi3` picks a
 * triangle-wave frame index (0-2, mirrored back down for 3-4) so the
 * animation ping-pongs. */
void AnimateAirshipPalette(void)
{
    s32 v;

    if (gAirshipHitFlashTimer == 0) {
        return;
    }
    gAirshipHitFlashTimer--;

    v = __divsi3(gAirshipHitFlashTimer, 3);
    if (v > 2) {
        v = 5 - v;
    }

    QueueVramDmaTransfer((void *)gAirshipHitFlashPalettes[v], (void *)(BG_PLTT + 0x20), 0x20, 0x10);
}
