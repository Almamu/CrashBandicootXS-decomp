#include "core.h"
#include "bosses.h"

/* Same boss-weapon "self"/tracker object family as airship_fireball.c/
 * airship_fall.c - see airship_fireball.c's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * Per docs/rom_map.md ("a rectangular BG-tilemap blit routine"):
 * streams 16-bit tile-index-pair values from `self`'s own data
 * (`self+0x1c`/`0x1e`, `0x20`, ...) two at a time, adds a per-call bias
 * byte (`gAirshipMapTileBase`) to each, and packs each pair into one
 * 16-bit VRAM write. Row stride is `0x20` halfwords - a standard
 * 32-tile-wide GBA BG tilemap row; row/column counts (both capped near
 * 32) come from `gAirshipMapCols`/`gAirshipMapRows`, write base
 * from those plus `gAirshipBg2Page`.
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS). Two details matter:
 * the bias is read as a plain `u8` narrowing of the `s32` global (an
 * `ldrb` hoisted out of the inner loop into a copy register), and `row`
 * is declared before `i`, so the loop optimizer creates the `row + 0x20`
 * pseudo after `i + 1` and `i + 1` wins the r7/ip tie as in the ROM
 * (docs/matching/archive/issue-58-61-naked-retry.md). */

void DrawAirshipMap(u16 *src)
{
    u8 *row = (u8 *)((gAirshipBg2Page + 0x18) << 11) + (VRAM + (0x20 - gAirshipMapCols) / 4 * 2) + ((0x20 - gAirshipMapRows) / 2 * 32 + 2);
    s32 i, j;

    for (i = 0; i < gAirshipMapRows; i++) {
        for (j = 0; j < gAirshipMapCols / 2; j++) {
            u8 bias = gAirshipMapTileBase;
            u16 lo = *src++ + bias;
            u16 hi = *src++ + bias;
            ((u16 *)row)[j] = lo | (hi << 8);
        }
        row += 0x20;
    }
}
