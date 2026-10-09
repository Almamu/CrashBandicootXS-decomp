#include "boss_actors.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "actor.h"
#include "gfx.h"
#include "globals.h"
}

/* Same boss-weapon subsystem as airship_fireball.cpp/airship_load_graphics.cpp - see
 * airship_fireball.cpp's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
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
 * `ConvertHovercraftTiles` (hovercraft.cpp, see
 * docs/matching/archive/near-miss-polish-3.md). */

/* The height is re-read after the row-pointer store (the ROM's `ldm
 * r1!`) because gAirshipMapFrames is a `u32` table (AnimPart's frame
 * offsets): the store has the int alias set, which may alias heights[],
 * so cse doesn't reuse the stored value (#662 round 3; as `u8 *` it did,
 * and an asm forced the re-read). The second loop has its own counter,
 * its header is written in the ROM's order, and the 0xf mask comes from
 * an `asm` so that it is the AND's first operand (the ROM copies the
 * mask, not the byte). Matches under both compilers.
 *
 * Why the mask needs it (#662 round 3, from the dumps): which operand
 * regmove copies into the result is the AND's first one, and cse1's
 * fold_rtx puts an operand whose value it knows to be constant second.
 * So a mask set anywhere cse can see it (in the pixel loop, `0xf & b`,
 * an inline's parameter) comes out `b & m` and the byte is copied. Set
 * before the pixel loop, where cse doesn't see it, the order stays, but
 * loop.c then hoists the set out of the row loop as well (into ip, where
 * the ROM sets it in the pixel loop's preheader); with the row loop as a
 * `goto` loop the set stays put, but the row loop then has no loop notes
 * and the reference weighting puts `stride` and `row_i` in each other's
 * homes (r10 and the stack; tried on the hovercraft twin). No -f flag,
 * alone or in pairs, gets the plain `b & 0xf` there. */
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
    u32 *rows = gAirshipMapFrames;
    u32 m;

    stride = (u32)(gAirshipMapCols * gAirshipMapRows + 1) >> 1 << 2;
    for (k = 0; k < 4; k++) {
        s32 x = *(const s32 *)((const u8 *)gAirshipPalette + off);
        heights[k] = x;
        sum += x;
        off += 4;
        rows[k] = (u32)((u8 *)gAirshipPalette + off);
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

        row = (u8 *)gAirshipMapFrames[row_i];
        hp = &heights[row_i];
        d = dst;
        src = row + stride;
        n = *hp;

        for (j = 0; j < n << 4; j++) {
            u32 b, c, p0, p1, p2, p3;

            /* the 0xf mask without a constant-set register: the mask is
             * the AND's first operand, as in the ROM */
            MATCH_CONST(m, 0xf);
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

/* Same boss-weapon subsystem as airship_fireball.cpp - see that file's header
 * comment and docs/matching/archive/issue-58-0x08030334-actor.md. */

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

/* The end of the airship's code (#664 part 11f): its HP gauge, its
 * teardown and its idle state (C linkage), in jetpack_balloon.cpp until
 * #769. See docs/matching/archive/issue-58-0x08030334-actor.md and
 * issue-62-0x08033804-actor.md. */

/* The airship's hit points as a percentage of its attack's (the HUD's
 * gauge, UpdateHudPercentCounters), at least 1 while it has any; -1 while
 * no airship is active (gAirshipState 0). */
s32 GetAirshipHpPercent(void)
{
    s32 hp;
    s32 result;

    if (gAirshipState == 0) {
        return -1;
    }

    hp = gAirshipHp;
    result = hp * 100 / gAirshipAttack->hp;
    if (result == 0 && hp > 0) {
        result = 1;
    }
    return result;
}

/* Frees the airship (CreateAirship's `new AnimPart`, airship.cpp). */
void DestroyAirship(void)
{
    delete gAirship;
}

/* UNUSED - no caller anywhere in the ROM (checked every src/ .c file, the
 * category vtables and every word-aligned Thumb pointer in baserom.gba).
 * Empty; it has no table slot to name it after, so it keeps the nullsub_N
 * name (docs/naming.md). */
void nullsub_30(void)
{
}

/* gAirshipStateFuncs[0]: no airship is active (AirshipStateFall goes back
 * to state 0). Empty. */
void AirshipStateInactive(void)
{
}
