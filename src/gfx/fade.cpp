extern "C" {
#include "core.h"
#include "system.h"
#include "gfx.h"
}

/* Sits right after StepBresenhamLine (ROM 0x08001254, in src/util/line_step.cpp)
 * and before whatever's still raw in asm/code_3_1_7.s. */


/* A per-frame screen-brightness fade tick: every `gBrightnessFade`.
 * `period` frames, writes the next brightness step to `REG_BLDY`,
 * counting either up or down depending on `flags`' top bit (fading
 * in vs. out). After 17 steps (a full fade), resets both counters,
 * briefly disables interrupts (`REG_IME`) while resetting `period` to
 * `-1` and removing its own VBlank callback (`callbackId`), then re-enables interrupts. */
void StepBrightnessFade(void)
{
    if (++gBrightnessFadeTimer == gBrightnessFade.period) {
        gBrightnessFadeTimer = 0;
        if (gBrightnessFade.flags & FADE_FLAG_IN) {
            REG_BLDY = 16 - gBrightnessFadeStep;
        } else {
            REG_BLDY = gBrightnessFadeStep;
        }
        if (++gBrightnessFadeStep == 0x11) {
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

/* Sits right after FadeBrightness (ROM 0x0800132C, in src/gfx/fade.cpp)
 * and before whatever's still raw in asm/code_3_1_7.s. */

/* A BGR555 palette entry as its channels, in a word-sized union: the
 * ROM keeps it in one register, inserting the loaded halfword into it
 * (`and #0xFFFF0000; orr`) and each darkened channel back into its bits.
 * The u16 channel fields make each insert truncate to 16 bits first. */
union darken_color {
    struct {
        u32 value:16;
    } raw;
    struct {
        u16 r:5;
        u16 g:5;
        u16 b:5;
    } rgb;
};

/* Blends the whole 512-entry palette at `gPaletteBackup` toward
 * black by `factor`/16 per channel (5 bits each, GBA BGR555), writing
 * the result to `gPaletteFadeBuffer`. */
void DarkenPalette(s32 factor)
{
    s32 i;

    for (i = 0; i <= 0x1FF; i++) {
        union darken_color color;

        color.raw.value = gPaletteBackup[i];
        color.rgb.r -= color.rgb.r * factor / 16;
        color.rgb.g -= color.rgb.g * factor / 16;
        color.rgb.b -= color.rgb.b * factor / 16;
        gPaletteFadeBuffer[i] = color.raw.value;
    }
}
