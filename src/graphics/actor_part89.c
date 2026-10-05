#include "core.h"

/* The continue prompt's (`InitContinuePrompt`/`InitContinuePromptGraphics`, actor_part87.c/
 * actor_part88.c) per-frame driver, called once per frame while the
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
struct continue_prompt {
    u8 unused_00[0x10];
    u8 bldcntLo;
    u8 bldcntHi;
    u8 eva : 5;
    u8 evaHi : 3;
    u8 bldalphaHi;
    u8 unused_14[0xc];
    s32 selection;
};

struct keys89 {
    u16 held;
    u16 pressed;
};

extern void UpdateKeys(void *arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *gInput;
extern u32 gKeys;
extern void *gAudioContext;
extern void DrawContinuePrompt(struct continue_prompt *self);
extern void CommitContinuePromptFrame(struct continue_prompt *self);

s32 ContinuePromptLoop(struct continue_prompt *self)
{
    s32 dir = 1;
    s32 i = 0;
    s32 level = self->eva;
    struct keys89 *input = (struct keys89 *)&gKeys;
    void **audio = &gAudioContext;

    while (dir >= 0) {
        asm("" : : "r"(audio)); /* extra reference: audio outranks i for r7 */
        UpdateKeys(gInput);
        {
            struct keys89 k = *input;

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
            self->eva = level;
            *(vu32 *)0x04000050 = *(u32 *)&self->bldcntLo;
        }
    }
    return self->selection == 0;
}

asm(".align 2, 0");
