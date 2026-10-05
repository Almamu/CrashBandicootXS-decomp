#include "core.h"

/* Sits right after StepBresenhamLine (ROM 0x08001254, in src/util/line_util2.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

extern void RemoveVBlankCallback(s32 arg0);
extern s32 gBrightnessFadeTimer;
extern s32 gBrightnessFadeStep;

struct unk_030007E8 {
    s32 field_0;
    s32 field_4;
    u8 field_8;
};

extern struct unk_030007E8 gBrightnessFade;

/* A per-frame screen-brightness fade tick: every `gBrightnessFade`.
 * `field_0` frames, writes the next brightness step to `REG_BLDY`,
 * counting either up or down depending on `field_8`'s top bit (fading
 * in vs. out). After 17 steps (a full fade), resets both counters,
 * briefly disables interrupts (`REG_IME`) while resetting `field_0` to
 * `-1` and calling `RemoveVBlankCallback` with `field_4` (presumably to kick off
 * whatever comes after the fade), then re-enables interrupts.
 * `mask`/`flag8` are pinned to r0/r1 to match the ROM's exact register
 * choice for the `& 0x80` check - the natural (unpinned) allocation
 * puts the loaded byte in r0 and the constant in r1 instead, one
 * register off. */
void StepBrightnessFade(void)
{
    s32 counter;

    counter = gBrightnessFadeTimer + 1;
    gBrightnessFadeTimer = counter;
    if (counter == gBrightnessFade.field_0) {
        s32 val;
        register u8 flag8 asm("r1");
        register s32 mask asm("r0");

        gBrightnessFadeTimer = 0;
        mask = 0x80;
        flag8 = gBrightnessFade.field_8;
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
            gBrightnessFade.field_0 = -1;
            RemoveVBlankCallback(gBrightnessFade.field_4);
            REG_IME = 1;
        }
    }
}

extern s32 AddVBlankCallback(void *callback);
extern void WaitForVBlank(void);

/* Starts a screen-brightness fade: `flags` bit 0 selects the blend
 * target (`REG_BLDCNT`, `0xBF` vs `0xFF`), bit 7 selects
 * direction (fade in from `0x10` vs fade out from `0`); `frameDelay`
 * (clamped to at least 1) is how many frames each of the 17 steps
 * takes. Refuses to start (silently) if a fade is already running -
 * `gBrightnessFade.field_0` is the sentinel `-1` only when idle,
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
        s32 f = gBrightnessFade.field_0;
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

    if (flags & 1) {
        REG_BLDCNT = 0xBF;
    } else {
        REG_BLDCNT = 0xFF;
    }

    if (sync != 0) {
        u8 dirBit = flags & 0x80;
        if (dirBit != 0) {
            REG_BLDY = 0x10;
        } else {
            REG_BLDY = dirBit;
        }
        REG_IME = 0;
        gBrightnessFade.field_8 = flags;
        gBrightnessFade.field_0 = frameDelay;
        gBrightnessFade.field_4 = AddVBlankCallback(StepBrightnessFade);
        REG_IME = 1;
    } else {
        s32 i = 0;
        register s32 dirBit8 asm("r8");
        dirBit8 = flags & 0x80;
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

/* Sits right after FadeBrightness (ROM 0x0800132C, in src/graphics/fade_util.c)
 * and before whatever's still raw in asm/code_3_1_7.s. */

extern u16 gPaletteBackup[512];
extern u16 gPaletteFadeBuffer[512];

/* Blends the whole 512-entry palette at `gPaletteBackup` toward
 * black by `factor`/16 per channel (5 bits each, GBA BGR555), writing
 * the result to `gPaletteFadeBuffer`. Each channel is extracted via an
 * explicit shift-left-then-shift-right pair (not a plain `&`/`>>`) and
 * re-inserted via a "clear those bits, then OR the new value in"
 * sequence - matching the ROM's own instruction shapes, which use this
 * shape even for pulling the initial raw 16-bit pixel into the
 * (reused, never explicitly zeroed) `color` accumulator register.
 * Several inline-asm-anchored temporaries (`tmp`/`diff`, both pinned to
 * r0) are needed to reproduce exact ROM register/instruction choices
 * that gcc's own optimizer would otherwise collapse into shorter but
 * differently-shaped code: the extraction's two shifts naturally
 * collapse into one register when written as a single C expression;
 * the post-subtract `(u16)` truncate before the final 5-bit mask gets
 * optimized away entirely (correct result, but ROM has the redundant
 * 16-bit truncate first); and the channel-2/3 insert's mask-then-shift
 * vs shift-then-mask ordering matters for exact instruction order even
 * though both compute the same value. */
void DarkenPalette(s32 factor)
{
    s32 i;

    for (i = 0; i <= 0x1FF; i++) {
        s32 color;
        register s32 ch asm("r1");
        s32 scaled;
        register s32 raw asm("r1");
        register u16 *addr asm("r1");

        addr = &gPaletteBackup[i];
        color &= ~0xFFFF;
        raw = *addr;
        color |= raw;

        {
            register s32 tmp asm("r0");
            tmp = color << 27;
            ch = (s32)((u32)tmp >> 27);
        }
        scaled = ch * factor;
        if (scaled < 0) scaled += 15;
        scaled >>= 4;
        {
            register s32 diff asm("r0");
            diff = ch - scaled;
            asm("lsl %0, %0, #0x10\n\tlsr %0, %0, #0x10" : "+r"(diff));
            diff &= 0x1F;
            color = (color & ~0x1F) | diff;
        }

        {
            register s32 tmp asm("r0");
            tmp = color << 22;
            ch = (s32)((u32)tmp >> 27);
        }
        scaled = ch * factor;
        if (scaled < 0) scaled += 15;
        scaled >>= 4;
        {
            register s32 diff asm("r0");
            diff = ch - scaled;
            asm("lsl %0, %0, #0x10\n\tlsr %0, %0, #0x10" : "+r"(diff));
            diff &= 0x1F;
            diff <<= 5;
            color = (color & ~(0x1F << 5)) | diff;
        }

        {
            register s32 tmp asm("r0");
            tmp = color << 17;
            ch = (s32)((u32)tmp >> 27);
        }
        scaled = ch * factor;
        if (scaled < 0) scaled += 15;
        scaled >>= 4;
        {
            register s32 diff asm("r0");
            diff = ch - scaled;
            asm("lsl %0, %0, #0x10\n\tlsr %0, %0, #0x10" : "+r"(diff));
            diff &= 0x1F;
            diff <<= 10;
            color = (color & ~(0x1F << 10)) | diff;
        }

        gPaletteFadeBuffer[i] = color;
    }
}
