#include "irq.hpp"
#include "frame_limiter.hpp"
#include "fade.hpp"

extern "C" {
#include "core.h"
#include "system.h"
#include "gfx.h"
}

using Engine::Sys::FrameLimiter;
using Engine::Sys::Irq;

/* Sits right after StepBresenhamLine (ROM 0x08001254, in src/util/line_step.cpp)
 * and before whatever's still raw in asm/code_3_1_7.s. */

/* A per-frame screen-brightness fade tick: every `state.period`
 * frames, writes the next brightness step to `REG_BLDY`,
 * counting either up or down depending on `flags`' top bit (fading
 * in vs. out). After 17 steps (a full fade), resets both counters,
 * briefly disables interrupts (`REG_IME`) while resetting `period` to
 * `-1` and removing its own VBlank callback (`callbackId`), then re-enables interrupts. */
void BrightnessFade::Step()
{
    if (++timer == state.period) {
        timer = 0;
        if (state.flags & FADE_FLAG_IN) {
            REG_BLDY = 16 - step;
        } else {
            REG_BLDY = step;
        }
        if (++step == 0x11) {
            step = 0;
            timer = 0;
            REG_IME = 0;
            state.period = -1;
            Irq::RemoveVBlankCallback(state.callbackId);
            REG_IME = 1;
        }
    }
}

/* Starts a screen-brightness fade: `flags` bit 0 selects the blend
 * target (`REG_BLDCNT`, `0xBF` vs `0xFF`), bit 7 selects
 * direction (fade in from `0x10` vs fade out from `0`); `frameDelay`
 * (clamped to at least 1) is how many frames each of the 17 steps
 * takes. Refuses to start (silently) if a fade is already running -
 * `state.period` is the sentinel `-1` only when idle,
 * checked via the classic `(~x + 1) | ~x < 0` "x != -1" bit-trick
 * rather than a plain comparison (matching the ROM's exact `mvn; neg;
 * orr; cmp` sequence - a direct `!= -1` compiles to a shorter
 * load-constant-and-compare instead). If `sync` is nonzero, registers
 * `Step` as a periodic callback (via `Irq::AddVBlankCallback`) to drive the
 * fade one step per call and returns immediately; otherwise it blocks
 * here, looping through all 17 steps itself and busy-waiting
 * `frameDelay` VBlanks between each via `FrameLimiter::WaitForVBlank`. */
void BrightnessFade::Start(u8 flags, s32 frameDelay, u8 sync)
{
    s32 notf = ~state.period;
    s32 t = -notf;
    t |= notf;
    if (t < 0) {
        return;
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
        state.flags = flags;
        state.period = frameDelay;
        state.callbackId = Irq::AddVBlankCallback(Step);
        REG_IME = 1;
    } else {
        s32 i = 0;
        do {
            if ((flags & FADE_FLAG_IN) != 0) {
                REG_BLDY = 0x10 - i;
            } else {
                REG_BLDY = i;
            }
            if (frameDelay > 0) {
                s32 k = frameDelay;
                do {
                    FrameLimiter::WaitForVBlank();
                    k--;
                } while (k != 0);
            }
            i++;
        } while (i <= 0x10);
    }
}

/* Sits right after BrightnessFade::Start (FadeBrightness, ROM 0x0800132C)
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

/* Blends the whole 512-entry palette at `backup` toward black by
 * `factor`/16 per channel (5 bits each, GBA BGR555), writing the result
 * to `buffer`. */
void PaletteFade::Darken(s32 factor)
{
    s32 i;

    for (i = 0; i <= 0x1FF; i++) {
        union darken_color color;

        color.raw.value = backup[i];
        color.rgb.r -= color.rgb.r * factor / 16;
        color.rgb.g -= color.rgb.g * factor / 16;
        color.rgb.b -= color.rgb.b * factor / 16;
        buffer[i] = color.raw.value;
    }
}
