#include "airship.hpp"
#include "audio.hpp"

extern "C" {
#include "globals.h"
#include "math_util.h"
#include "match.h"
#include <libgcc.h>
#include "actor.h"
#include "gfx.h"
}

/* The airship's damage, graphics, HP gauge and teardown (#664 parts
 * 11f and 11i, Airship since #772, include/airship.hpp): DamageAirship, its graphics
 * (LoadAirshipGraphics, ConvertAirshipTiles and the flash/palette
 * updates) and the end of its code. DamageAirship and
 * LoadAirshipGraphics were airship_damage.cpp and
 * airship_load_graphics.cpp until #771. Same boss-weapon subsystem as
 * airship_fireball.cpp - see that file's header comment and
 * docs/matching/archive/issue-58-0x08030334-actor.md.
 *
 * DamageAirship, hit by the player's shots: takes `delta` off the
 * airship's hit points and starts the hit flash
 * (gAirshipHitFlashTimer). While it has hit points left, a hit sound;
 * at zero, it stops and enters state 4 (exploding) with animation 1. */
void Airship::Damage(s32 delta)
{
    s32 remaining = hp - delta;

    hp = remaining;
    hitFlashTimer = 0x12;

    if (remaining <= 0) {
        hp = 0;
        velX = 0;
        velY = 0;
        velZ = 0xaa;
        SetState(4, 1);
    } else {
        gAudioContext->PlaySfx(SFX_AIRSHIP_HIT, 0x100);
    }
}

/* LoadAirshipGraphics: confirmed by docs/rom_map.md as a
 * `category_vtable` slot (`gActorCategoryVtables`, type 1, slot 6) - part of this actor's per-frame dispatch table.
 *
 * Zero-fills tile 0xff of BG char block 2 (one 0x40-byte 8bpp tile,
 * right before `BG_CHAR_ADDR(3)`, the blank tile the map's 0xff entries
 * use), then DMA3-fills 0xffff halfwords into BG2's two affine map pages
 * (screen blocks 24 and 25, which `UpdateAirshipBg2` flips between) and
 * runs `ConvertAirshipTiles`'s VRAM fill-level meter generator (see
 * docs/rom_map.md's "procedurally-generated VRAM fill-level meter"
 * finding). While the airship's state (`gAirshipState`) is non-zero:
 * forces a BG2CNT preset toggle (via `gAirshipBg2PageFlip`/
 * `gAirshipBg2Page` and `UpdateAirshipBg2`), looks up the current
 * frame's tilemap through its AnimPart (`gAirship`'s `animIndex` and
 * `animTime`) and blits it via `DrawAirshipMap` (docs/rom_map.md's
 * confirmed "rectangular BG-tilemap blit routine"), sets DISPCNT's
 * `DISPCNT_BG2_ON` (the bit `AirshipStateFall` clears), and
 * DMAs a 0x10-halfword palette strip from `gAirshipPalette` into
 * BG palette bank 1 (`0x05000020`). Once there, one of two mutually
 * exclusive tails run based on the airship's state: state 5 mirrors
 * palette index 8/0/0xf (slots `+0x10`/`+8`/`+2`/`+0x1e`) all down to
 * black; state 4 clears individual palette slots (`+0x1e`/`+2`/`+8`/
 * `+0x10`) as `gAirshipStateTimer` (a frame/flags counter) crosses four
 * successive thresholds (9, 0x31, 0x4f, 0x6d) - a fade/flash-out
 * sequence for the effect's palette strip.
 *
 * Matching notes (no register pins needed): the filler-tile clear walks
 * a plain `s32` address downward (so its loop test is the ROM's signed
 * `bge`) with the zero hoisted into a local first, the fill/copy are the
 * standard `DmaFill16`/`DmaCopy16` macros, and the palette strip is
 * `vu16` - the state-5 blackout is one chained assignment, whose
 * volatile read-backs are the ROM's `ldrh`/`strh` ladder. */

void Airship::LoadGraphics()
{
    s32 i;
    s32 base = BG_CHAR_ADDR(2) + 0xff * TILE_SIZE_8BPP;
    u32 zero = 0;

    for (i = base + TILE_SIZE_8BPP - 4; i >= base; i -= 4)
        *(u32 *)i = zero;
    DmaFill16(3, 0xFFFF, (void *)BG_SCREEN_ADDR(24), 2 * BG_SCREEN_SIZE);
    ConvertTiles();
    if (state != 0) {
        AnimPart *self;
        vu16 *pal;

        bg2PageFlip = 1;
        bg2Page = 0;
        self = anim;
        {
            s32 t = Q8_TO_INT(self->animTime);
            DrawMap((u16 *)self->frameOffsets[self->anims[self->animIndex].frameIndex + t]);
        }
        REG_DISPCNT |= DISPCNT_BG2_ON;
        UpdateBg2();
        pal = (vu16 *)(BG_PLTT + PALETTE_SIZE_16);
        DmaCopy16(3, palette, pal, PALETTE_SIZE_16);
        if (state == 5) {
            pal[15] = pal[1] = pal[4] = pal[8] = 0;
        } else if (state == 4) {
            if ((u32)stateTimer > 9)
                pal[15] = 0;
            if ((u32)stateTimer > 0x31)
                pal[1] = 0;
            if ((u32)stateTimer > 0x4f)
                pal[4] = 0;
            if ((u32)stateTimer > 0x6d)
                pal[8] = 0;
        }
    }
}

/* ConvertAirshipTiles, per docs/rom_map.md ("A new mechanism: a
 * procedurally-generated VRAM fill-level meter"): a near-identical twin of
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
 * alone or in pairs, gets the plain `b & 0xf` there.
 *
 * The exact condition (#662 round 4, from cse.c, regmove.c and loop.c):
 * regmove copies the first operand of the AND that doesn't die there
 * (neither does: the byte is shifted next, the mask is loop-invariant),
 * so the ROM's RTL had `(and mask byte)`. cse1's fold_rtx swaps a
 * commutative operation whose first operand has a known constant value
 * and whose second doesn't, so at cse1 the mask's 0xf must be unknown:
 * set outside the pixel loop's extended basic block, whose top label
 * ends it. loop.c must then leave that set in the pixel loop's
 * preheader instead of moving it out of the row loop; scan_loop keeps
 * it only when a conditional jump precedes it in the row loop
 * (maybe_never) and the register is used in other blocks. The only
 * such jump is the pixel loop's own entry test, which jump.c copies in
 * front of the loop. A source guard in its place (`j = 0; if (j < n <<
 * 4) { m = 0xf; for (; j < n << 4; j++) ... }`, and the do/while and
 * `for (;;)` forms) gives the ROM's four mask copies, but cse1 then
 * shares the guard's `n << 4` with the loop test, which the ROM
 * recomputes from `n` (r8) every iteration (28-34 lines off; a guard on
 * `n` adds a compare, 42-48), and it is a redundant test anyway. A u8
 * (QImode) AND is 128 lines off. #662 round 5: the ROM's other nibble
 * expanders don't share an idiom that gets there. sprite_arm.cpp's ARM
 * UnpackNibbleTiles (`ExpandNibble(v & 15)`, a ternary helper, one
 * halfword per word) and bg_picture.cpp's MapFill (`*nib & 0xf`, the
 * constant set at each use and dying in the AND, so the result takes
 * its register) put this loop 100-118 lines off in every combination
 * of helper (if, ternary, early return) and body (two byte locals,
 * indexing, a halfword, `% 16`/`/ 16`, the high nibble unmasked), where
 * the plain `m = 0xf` in the loop is 8 lines off: the two mask copies
 * become byte copies. #662 round 6 (rewritten from the ROM, on the
 * hovercraft twin): regmove's backward pass copies the first of the
 * AND's operands that doesn't die; a byte dying at the AND would be
 * retargeted by its load with no copy at all, so the ROM's mask copy
 * does need `(and mask byte)` (the 0x10 copy comes from `v` dying at
 * the `ior` with its setter before the `if`). The mask set in the pixel loop's `for` init or at the top
 * of the row body is 78 lines off (hoisted out of the row loop), at
 * function scope 88 (one set before both loops, in r9); a ternary or
 * if/else MeterPx is the plain 8; reading the byte twice (`src[0] &
 * 0xf`, `src[0] >> 4`) is 118-120 (cse1 merges the loads).
 *
 * #662 round 7 (private agbcp/old_agbcp builds, then every object
 * rebuilt with them and compared function by function): the plain
 * `b & 0xf` matches, in both twins, if regmove copies a non-dying
 * operand that is a remote constant in preference to the first one,
 * but 7 matched functions then change (number_format.cpp's itoa,
 * UpdateZoomBg, HandleLinkSerial, ResetLinkSessionState,
 * DecodeLayerChunk, LoadCreditsLogos, UpdateSlotCrate: each an `x &
 * MASK` whose ROM copies `x`, not the mask); copying the last non-dying
 * operand changes 27. `m = 0xf; ... m & b` in the pixel loop matches if
 * cse1 doesn't swap an AND whose first operand is a register with a
 * known constant, but GetDpadDirection, ProbeGroundSpriteTerrain and
 * the two DrawSpritePieces change (5 with IOR and XOR too, 143 for every
 * commutative code). So both compilers had both rules as they are. In
 * the matched corpus the mask copy is Yeti::UpdatePalette's `mask2 &
 * c`, a mask set in the block before a single loop, where cse1 can't
 * see it and loop.c has no outer loop to move it to; here that is the
 * set at the top of the row body (78 lines). u8/u16 masks or pixel
 * values (the halfword AND of round 7's player code) are 8 lines off or
 * worse (45-89). */
static inline u32 MeterPx(u32 v)
{
    u32 r = 0;
    if (v != 0)
        r = 0x10 | v;
    return r;
}

void Airship::ConvertTiles()
{
    s32 heights[4];
    u32 stride;
    s32 sum = 0;
    s32 off = 0x204;
    s32 k;
    s32 row_i;
    u32 *dst;
    u32 *rows = mapFrames;
    u32 m;

    stride = (u32)(mapCols * mapRows + 1) >> 1 << 2;
    for (k = 0; k < 4; k++) {
        s32 x = *(const s32 *)((const u8 *)palette + off);
        heights[k] = x;
        sum += x;
        off += 4;
        rows[k] = (u32)((u8 *)palette + off);
        off += stride;
        off += heights[k] << 5;
    }
    mapTileBase = 0xFF - sum;
    dst = (u32 *)(((0xFF - sum) << 6) + BG_CHAR_ADDR(2));
    for (row_i = 0; row_i <= 3; row_i++) {
        u8 *src;
        u8 *row;
        s32 *hp;
        s32 n;
        s32 j;
        u32 *d;

        row = (u8 *)mapFrames[row_i];
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

/* Sets BG palette bank 1's last color (index 15) to either a near-white
 * flash color or a dim default, gated by bit 3 of `gAirshipStateTimer`
 * (a flags word driving this effect's per-frame look). */
void Airship::UpdateFlashColor()
{
    vu16 *bank1 = (vu16 *)(BG_PLTT + PALETTE_SIZE_16);

    if (stateTimer & 8) {
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
void Airship::AnimatePalette()
{
    s32 v;

    if (hitFlashTimer == 0) {
        return;
    }
    hitFlashTimer--;

    v = __divsi3(hitFlashTimer, 3);
    if (v > 2) {
        v = 5 - v;
    }

    QueueVramDmaTransfer((void *)hitFlashPalettes[v], (void *)(BG_PLTT + PALETTE_SIZE_16),
                         PALETTE_SIZE_16, 0x10);
}

/* The end of the airship's code (#664 part 11f): its HP gauge, its
 * teardown and its idle state, in jetpack_balloon.cpp until
 * #769. See docs/matching/archive/issue-58-0x08030334-actor.md and
 * issue-62-0x08033804-actor.md. */

/* The airship's hit points as a percentage of its attack's (the HUD's
 * gauge, UpdateHudPercentCounters), at least 1 while it has any; -1 while
 * no airship is active (gAirshipState 0). */
s32 Airship::GetHpPercent()
{
    s32 left;
    s32 result;

    if (state == 0) {
        return -1;
    }

    left = hp;
    result = left * 100 / attack->hp;
    if (result == 0 && left > 0) {
        result = 1;
    }
    return result;
}

/* Frees the airship (CreateAirship's `new AnimPart`, airship.cpp). */
void Airship::Destroy()
{
    delete anim;
}

/* UNUSED - no caller anywhere in the ROM (checked every src/ .c file, the
 * category vtables and every word-aligned Thumb pointer in baserom.gba).
 * Empty; it has no table slot to name it after, so it keeps the nullsub_N
 * name (docs/naming.md). */
void Airship::nullsub_30()
{
}

/* gAirshipStateFuncs[0]: no airship is active (AirshipStateFall goes back
 * to state 0). Empty. */
void Airship::StateInactive()
{
}
