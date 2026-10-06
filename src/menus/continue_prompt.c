#include "core.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "text.h"
#include "system.h"
#include "audio.h"
#include "menus.h"
#include "gfx.h"
#include "globals.h"

extern void *_call_via_r1(void *arg0, void *arg1);

/* The other half of the continue prompt's setup, called from
 * `InitContinuePrompt` (continue_prompt_init.c): flushes the shared VRAM upload cursor
 * twice, hooks `self->icons` up to the global text icon manager
 * (`gSmallFont`), fires its 7th OAM trampoline slot, clears its
 * `marginX` and re-derives the cursor's limit from `tileCount`, resets
 * the shared tile cache and pins its first four slots, seeds those four
 * slots with fixed 32-byte tile patterns from ROM data, flushes the
 * cache, sets the overlay's DISPCNT "OBJ enable" bit, and flushes the
 * OAM shadow buffer.
 *
 * Built with old_agbcc. The seeding loop is indexed through `i`: gcc
 * strength-reduces every access into its own pointer but keeps `i` as
 * the up-counting trip counter (r7) the ROM has. Written with explicit
 * pointer increments, `i` has nothing left to do but count, and gcc
 * reverses it into a down-counter. */
void InitContinuePromptGraphics(struct continue_prompt *self)
{
    struct bitmap_font *icons;
    struct palette_cache *cache;
    s32 i;

    gObjVramCursor->baseTile = 0;
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);

    icons = gSmallFont;
    self->icons = icons;
    icons->tileBase = 0;
    {
        u8 *rec = (u8 *)icons->record + 0x40;
        _call_via_r1((u8 *)icons + *(s16 *)rec, *(void **)(rec + 4));
    }
    self->icons->marginX = 0;
    ReserveObjVram(gObjVramCursor, self->icons->tileCount << 5);
    MarkObjVram(gObjVramCursor);

    FreeUnlockedPaletteSlots(gPaletteCache);
    ClaimPaletteSlot(gPaletteCache, 0);
    ClaimPaletteSlot(gPaletteCache, 1);
    ClaimPaletteSlot(gPaletteCache, 2);
    ClaimPaletteSlot(gPaletteCache, 3);

    cache = gPaletteCache;
    {
        u16 *destA = (u16 *)cache->slots[0];
        u16 *destB = (u16 *)cache->slots[2];

        for (i = 0; i < 16; i++) {
            destA[i] = gContinuePromptPalette0[i];
            destA[i + 0x10] = gContinuePromptPalette1[i];
            destB[i] = gContinuePromptPalette2[i];
            destB[i + 0x10] = gContinuePromptPalette3[i];
        }
    }
    UploadPaletteCache(gPaletteCache);

    ((u8 *)&self->dispcnt)[1] |= 0x10;

    ResetOamBuffer(gOamBuffer);
    HideUnusedOamEntries(gOamBuffer);
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
}

/* The continue prompt's (`InitContinuePrompt`/`InitContinuePromptGraphics`, continue_prompt_init.c/
 * continue_prompt.c) per-frame driver, called once per frame while the
 * effect is running (caller not yet identified in this pass - out of
 * scope, see docs/matching/issue-63-0x08033ef4-actor.md). Busy-loops
 * (yielding via `DrawContinuePrompt`/`CommitContinuePromptFrame` each iteration - graphics-
 * loading/particle-update helpers just past this file's own raw-asm
 * boundary, `asm/..._34aa4.s`) polling input twice per outer iteration:
 * a confirm press (bit 0) or D-pad-down-with-L (bit 3) of
 * `gKeys.pressed` immediately exits with a "confirm" SFX
 * cue (0x49); otherwise L alone (bit 6, only once `self+0x20`'s one-shot
 * flag is already set) or R alone (bit 7, only once it's clear) plays a
 * "step" cue (0x46) and flips that flag. Every two inner iterations, a
 * 0-15 counter (`self+0x12`'s low 5 bits) ping-pongs a screen-space
 * blend-alpha value up/down by re-applying `self->blend` (already built
 * by `InitContinuePrompt`) to REG_BLDCNT/BLDALPHA, driving the overlay's
 * flicker/pulse animation. Returns 1 if `self+0x20`'s flag is still
 * clear when the loop exits via the confirm branch, else 0.
 *
 * Matched (old_agbcc) in the late-ROM retry passes
 * (docs/matching/late-rom-naked-retry.md). It takes: `k` as a struct
 * copy of the input word (the ROM's word load + `lsrs #16` per test,
 * then a fresh `ldrh` for the 0x80 test after the calls); the loop as
 * `while (dir >= 0)` (with `while (1)` jump.c moves the return
 * computation to the `break`); and two empty asm statements, which emit
 * no code:
 *  - `asm("" : : "r"(audio))` at the top of the loop adds one reference
 *    to `audio`, so it outranks the pair counter for r7 (the counter
 *    lands in r8 without a pin);
 *  - `asm("" : "+r"(k))` between the `& 1` and `& 8` tests makes the
 *    second test's shift a fresh value, so CSE doesn't share it with the
 *    first, and the first test keeps the ROM's `movs r0, #1; ands r0, r1`
 *    register choice. */
s32 ContinuePromptLoop(struct continue_prompt *self)
{
    s32 dir = 1;
    s32 i = 0;
    s32 level = self->blend.bits.eva;
    struct held_pressed_pair *input = &gKeys.half;
    struct AudioContext **audio = &gAudioContext;

    while (dir >= 0) {
        asm("" : : "r"(audio)); /* extra reference: audio outranks i for r7 */
        UpdateKeys(gInput);
        {
            struct held_pressed_pair k = *input;

            /* The "+r" asm keeps the & 8 test's shift separate from the
             * & 1 test's. */
            if ((k.pressed & 1) || ({ asm("" : "+r"(k)); (u16)(k.pressed & 8); })) {
                PlaySfx(*audio, 0x49, 0x100);
                break;
            }
            if ((k.pressed & 0x40) && self->selection == 1) {
                PlaySfx(*audio, 0x46, 0x100);
                self->selection = 0;
            }
        }
        if ((input->pressed & 0x80) && self->selection == 0) {
            PlaySfx(*audio, 0x46, 0x100);
            self->selection = 1;
        }
        DrawContinuePrompt(self);
        CommitContinuePromptFrame(self);
        if (++i > 1) {
            i = 0;
            if (dir) {
                if (--level <= 0)
                    dir = 0;
            } else {
                if (++level > 15)
                    dir = 1;
            }
            self->blend.bits.eva = level;
            *(vu32 *)0x04000050 = self->blend.word;
        }
    }
    return self->selection == 0;
}

asm(".align 2, 0");
