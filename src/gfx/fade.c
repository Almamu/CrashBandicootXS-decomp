#include "core.h"
#include "match.h"
#include "system.h"
#include "gfx.h"

/* Sits right after StepBresenhamLine (ROM 0x08001254, in src/util/line_step.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */


/* A per-frame screen-brightness fade tick: every `gBrightnessFade`.
 * `period` frames, writes the next brightness step to `REG_BLDY`,
 * counting either up or down depending on `flags`' top bit (fading
 * in vs. out). After 17 steps (a full fade), resets both counters,
 * briefly disables interrupts (`REG_IME`) while resetting `period` to
 * `-1` and removing its own VBlank callback (`callbackId`), then re-enables interrupts.
 * `flag8` is pinned to r1 to match the ROM's register choice for the
 * `& 0x80` check (the constant goes to r0) - the natural (unpinned)
 * allocation puts the loaded byte in r0 and the constant in r1 instead,
 * one register off. `mask` is a separate local set after the timer
 * store: written as a literal in the test, or initialized at its
 * declaration, the constant load moves. */
void StepBrightnessFade(void)
{
    s32 counter;

    counter = gBrightnessFadeTimer + 1;
    gBrightnessFadeTimer = counter;
    if (counter == gBrightnessFade.period) {
        s32 val;
        MATCH_HOLD_REG(u8, flag8, r1);
        s32 mask;

        gBrightnessFadeTimer = 0;
        mask = FADE_FLAG_IN;
        flag8 = gBrightnessFade.flags;
        if (mask & flag8) {
            REG_BLDY = 16 - gBrightnessFadeStep;
        } else {
            REG_BLDY = gBrightnessFadeStep;
        }
        val = gBrightnessFadeStep + 1;
        gBrightnessFadeStep = val;
        if (val == 0x11) {
            gBrightnessFadeStep = 0;
            gBrightnessFadeTimer = 0;
            REG_IME = 0;
            gBrightnessFade.period = -1;
            RemoveVBlankCallback(gBrightnessFade.callbackId);
            REG_IME = 1;
        }
    }
}

/* Starts a screen-brightness fade: `flags` bit 0 selects the blend
 * target (`REG_BLDCNT`, `0xBF` vs `0xFF`), bit 7 selects
 * direction (fade in from `0x10` vs fade out from `0`); `frameDelay`
 * (clamped to at least 1) is how many frames each of the 17 steps
 * takes. Refuses to start (silently) if a fade is already running -
 * `gBrightnessFade.period` is the sentinel `-1` only when idle,
 * checked via the classic `(~x + 1) | ~x < 0` "x != -1" bit-trick
 * rather than a plain comparison (matching the ROM's exact `mvn; neg;
 * orr; cmp` sequence - a direct `!= -1` compiles to a shorter
 * load-constant-and-compare instead). If `sync` is nonzero, registers
 * `StepBrightnessFade` as a periodic callback (via `AddVBlankCallback`) to drive the
 * fade one step per call and returns immediately; otherwise it blocks
 * here, looping through all 17 steps itself and busy-waiting
 * `frameDelay` VBlanks between each via `WaitForVBlank`. */
void FadeBrightness(u8 flags, s32 frameDelay, u8 sync)
{
    {
        s32 f = gBrightnessFade.period;
        s32 notf = ~f;
        s32 t = -notf;
        t |= notf;
        if (t < 0) {
            return;
        }
    }

    if (frameDelay <= 0) {
        frameDelay = 1;
    }

    if (flags & FADE_FLAG_WHITE) {
        REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
    } else {
        REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    }

    if (sync != 0) {
        u8 dirBit = flags & FADE_FLAG_IN;
        if (dirBit != 0) {
            REG_BLDY = 0x10;
        } else {
            REG_BLDY = dirBit;
        }
        REG_IME = 0;
        gBrightnessFade.flags = flags;
        gBrightnessFade.period = frameDelay;
        gBrightnessFade.callbackId = AddVBlankCallback(StepBrightnessFade);
        REG_IME = 1;
    } else {
        s32 i = 0;
        s32 dirBit8 = flags & FADE_FLAG_IN;
        do {
            s32 next;
            if (dirBit8 != 0) {
                REG_BLDY = 0x10 - i;
            } else {
                REG_BLDY = i;
            }
            next = i + 1;
            if (frameDelay > 0) {
                s32 k = frameDelay;
                do {
                    WaitForVBlank();
                    k--;
                } while (k != 0);
            }
            i = next;
        } while (i <= 0x10);
    }
}

/* Sits right after FadeBrightness (ROM 0x0800132C, in src/gfx/fade.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

/* Blends the whole 512-entry palette at `gPaletteBackup` toward
 * black by `factor`/16 per channel (5 bits each, GBA BGR555), writing
 * the result to `gPaletteFadeBuffer`. Each channel is extracted via an
 * explicit shift-left-then-shift-right pair (not a plain `&`/`>>`) and
 * re-inserted via a "clear those bits, then OR the new value in"
 * sequence - matching the ROM's own instruction shapes, which use this
 * shape even for pulling the initial raw 16-bit pixel into the
 * (reused, never explicitly zeroed) `color` accumulator register.
 * The first channel's extraction goes through `tmp`, pinned to r0, and
 * the raw pixel through `raw`, pinned to r1; the other two extractions
 * are plain expressions. The post-subtract `(u16)` truncate before the
 * final 5-bit mask is redundant but is in the ROM, and the channel-2/3
 * insert's mask-then-shift vs
 * shift-then-mask ordering matters for exact instruction order even
 * though both compute the same value. */
void DarkenPalette(s32 factor)
{
    s32 i;

    for (i = 0; i <= 0x1FF; i++) {
        /* self-init: never zeroed (see above); silences -Wuninitialized (#577) */
        s32 color = color;
        s32 ch;
        s32 scaled;
        MATCH_HOLD_REG(s32, raw, r1);
        u16 *addr;

        addr = &gPaletteBackup[i];
        color &= ~0xFFFF;
        raw = *addr;
        color |= raw;

        {
            MATCH_HOLD_REG(s32, tmp, r0);
            tmp = color << 27;
            ch = (s32)((u32)tmp >> 27);
        }
        scaled = ch * factor;
        if (scaled < 0)
            scaled += 15;
        scaled >>= 4;
        {
            s32 diff = (u16)(ch - scaled);

            diff &= 0x1F;
            color = (color & ~0x1F) | diff;
        }

        ch = (s32)((u32)(color << 22) >> 27);
        scaled = ch * factor;
        if (scaled < 0)
            scaled += 15;
        scaled >>= 4;
        {
            s32 diff = (u16)(ch - scaled);

            diff &= 0x1F;
            diff <<= 5;
            color = (color & ~(0x1F << 5)) | diff;
        }

        ch = (s32)((u32)(color << 17) >> 27);
        scaled = ch * factor;
        if (scaled < 0)
            scaled += 15;
        scaled >>= 4;
        {
            s32 diff = (u16)(ch - scaled);

            diff &= 0x1F;
            diff <<= 10;
            color = (color & ~(0x1F << 10)) | diff;
        }

        gPaletteFadeBuffer[i] = color;
    }
}
