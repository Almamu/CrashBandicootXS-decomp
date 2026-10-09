extern "C" {
#include "core.h"
#include "actor_anim.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
}

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/archive/issue-56-0x0802f0dc-actor.md.
 *
 * Two BG1 picture loaders (issue #56) sharing one repack loop: for each
 * of `rows` rows of `cols` map entries, add the tile base
 * (`GetCellAnimFreeTile() - 0x200`) to the entry, OR in a 4-bit palette bank
 * taken alternately from the low and high nibble of the next byte, and
 * store it at `BG_SCREEN_ADDR(26) + row * 0x40` (columns 0x20 and up go
 * to the second screen block, +0x7C0).
 *
 * - `LoadBgPicture(pic)`: DMA3-copies `pic`'s 0x200-byte palette to
 *   PLTT, reads cols/rows and the tile count (a `struct
 *   bg_picture_header`), runs the loop over the map at +0x208 with the
 *   nibbles after the tiles, then enables BG1 (priority 3, char block 1,
 *   screen block 26, 512x256) and DMA3-copies the tiles to VRAM +
 *   GetCellAnimFreeTile() * 32.
 * - `FillBgPictureMap(nibbles, map, cols, rows)`: the same loop on explicit
 *   arguments.
 *
 * Both are real C, built with old_agbcc (the whole object is).
 *
 * The ROM's 7B0 has the loop inlined: it has the separately
 * strength-reduced `dest[c]`/`dest[c + 0x3e0]` pointers an inlined copy
 * gets and a standalone compile doesn't (that one combines them into one
 * pointer plus 0x7c0, as in 8E8). So 7B0 calls the `static inline`
 * `MapFill` and 8E8 is the same loop written out as its own function.
 * The loop needs three spellings: the map entry read as `v = *map; v +=
 * base;` (the map pointer then outranks `v` for r5), the high nibble
 * masked, `(*nib >> 4) & 0xf` (without the mask the next-row pointer and
 * row+1 swap ip/r9), and the nibble toggle a `bool` (`odd = !odd`). With
 * an `s32` toggle (`odd ^= 1`), 7B0 needed two empty asm statements,
 * each adding a reference to a value to raise its register-allocation
 * priority: `dest` after the loop (or the nibble pointer outranked it
 * for r5) and `cols` before the call (or it tied with row+1 for r8/sl)
 * (#662 round 2; docs/matching/archive/late-rom-naked-retry.md). 8E8
 * compiles the same either way. */

static inline void MapFill(u8 *nib, u16 *map, s32 cols, s32 rows)
{
    u16 *dest;
    s32 base;
    bool odd;
    s32 r;
    s32 c;

    dest = (u16 *)BG_SCREEN_ADDR(26);
    base = GetCellAnimFreeTile() - 0x200;
    odd = false;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            s32 v, n;

            v = *map;
            v += base;
            map++;
            if (odd) {
                n = (*nib >> 4) & 0xf;
                nib++;
            } else {
                n = *nib & 0xf;
            }
            odd = !odd;
            n = (n << 12) | v;
            if (c < 0x20)
                dest[c] = n;
            else
                dest[c + 0x3e0] = n;
        }
        dest += 0x20;
    }
}

void LoadBgPicture(u8 *pic)
{
    s32 cols, rows;
    u32 tiles;
    u8 *tileData;

    DmaCopy16(3, pic, (void *)PLTT, BG_PLTT_SIZE);
    cols = ((struct bg_picture_header *)pic)->cols;
    rows = ((struct bg_picture_header *)pic)->rows;
    pic += 0x204;
    tiles = *(u32 *)pic;
    pic += 4;
    tileData = pic + ((cols * rows + 1) / 2) * 4;
    MapFill(tileData + tiles * 32, (u16 *)pic, cols, rows);
    REG_DISPCNT |= DISPCNT_BG1_ON;
    REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(26) | BGCNT_TXT512x256;
    {
        void *vd = (void *)(VRAM + GetCellAnimFreeTile() * 32);

        DmaCopy16(3, tileData, vd, tiles * 32);
    }
}

/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void FillBgPictureMap(u8 *nib, u16 *map, s32 cols, s32 rows)
{
    u16 *dest;
    s32 base;
    bool odd;
    s32 r;
    s32 c;

    dest = (u16 *)BG_SCREEN_ADDR(26);
    base = GetCellAnimFreeTile() - 0x200;
    odd = false;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            s32 v, n;

            v = *map;
            v += base;
            map++;
            if (odd) {
                n = (*nib >> 4) & 0xf;
                nib++;
            } else {
                n = *nib & 0xf;
            }
            odd = !odd;
            n = (n << 12) | v;
            if (c < 0x20)
                dest[c] = n;
            else
                dest[c + 0x3e0] = n;
        }
        dest += 0x20;
    }
}
