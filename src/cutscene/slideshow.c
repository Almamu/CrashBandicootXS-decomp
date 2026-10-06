#include "core.h"
#include "match.h"
#include "gba/dma_macros.h"
#include "cutscene.h"
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "globals.h"

/* GitHub issue #38: 0x08024590-0x08024783 (game_loop), the sound-channel-
 * handle helper family - see docs/matching/archive/issue-38-medal-results-tally.md
 * and docs/matching/archive/issue-38-sound-channel-family.md. All four functions
 * are real C, built with old_agbcc - see
 * docs/matching/archive/game-loop-old-agbcc.md. */

struct AudioContext;

/* Starts sound cue `slides[idx]->cue` on the audio context. If the
 * channel already reports that cue, plays the item's secondary sfx
 * (unless it is the 0x63 "none" sentinel) and then starts the item's
 * fade; otherwise starts the fade first, then busy-waits for the cue
 * before playing the sfx. */
void BeginSlide(struct cutscene_player *self, s32 idx)
{
    const struct cutscene_slide *item;

    PlaySong(gAudioContext, self->slides[idx]->cue);
    if (GetCurrentSong(gAudioContext) == (item = self->slides[idx])->cue) {
        if (item->sfx != 0x63)
            PlaySfx(gAudioContext, item->sfx, 0x100);
        FadeBrightness(self->slides[idx]->fade | -0x80, 1, 0);
    } else {
        FadeBrightness(item->fade | -0x80, 1, 0);
        if (self->slides[idx]->sfx != 0x63) {
            while (GetCurrentSong(gAudioContext) != self->slides[idx]->cue)
                ;
            PlaySfx(gAudioContext, self->slides[idx]->sfx, 0x100);
        }
    }
}

/* Per-frame driver loop over `self`'s item list: for each index, streams
 * the item's VRAM tile bank and refreshes its sound-channel handle
 * (`ShowSlidePicture`/`BeginSlide`), polls input (`WaitForKeyPress`) to get a
 * confirm/cancel result, applies the item's duck-out (`duckMusic`) and
 * fade-start (`fadeAfter`, sentinel -1) side effects, re-arms the item's
 * cue if needed (`rearmSfx`/`sfx`), then advances to the next
 * "still active" item via `SkipSlides`.
 *
 * UNUSED - no caller anywhere in the ROM (checked src/, asm/ and every
 * Thumb `bl` and aligned word of baserom.gba for its address). It plays
 * a slide list without text; the cutscenes use RunCutscenePlayer
 * (cutscene_player.c), the same loop with the text pages added. */
void RunSlideshow(struct cutscene_player *self0)
{
    struct cutscene_player *self = self0;
    s32 i;

    for (i = 0; i < self->count; i++) {
        u8 checkButtons;
        const struct cutscene_slide *item;

        ShowSlidePicture(self, i);
        BeginSlide(self, i);

        item = self->slides[i];
        checkButtons = (u8)WaitForKeyPress(item->wait, item->buttons, 8);

        if (self->slides[i]->duckMusic != 0) {
            FadeOutMusic(gAudioContext, 0);
        }

        {
            s32 v = self->slides[i]->fadeAfter;

            if (v != -1) {
                FadeBrightness((u8)v, 1, 0);
            }
        }

        {
            const struct cutscene_slide *item2 = self->slides[i];

            if (item2->rearmSfx != 0 && item2->sfx != 0x63) {
                StopSfx(gAudioContext, item2->sfx);
            }
        }

        i = SkipSlides(self, i, checkButtons);
    }
}

/* Scans forward from `startIdx + 1` for the next item whose `buttons`
 * isn't 1 ("busy"), returning the index just before it (or the last
 * index reached if every remaining item is busy). Returns `startIdx`
 * unchanged if `condFlag` is set, or if `startIdx + 1` is already past
 * the list. */
s32 SkipSlides(struct cutscene_player *self, s32 startIdx, u8 condFlag)
{
    s32 cur = startIdx;
    s32 next;
    s32 count;
    const struct cutscene_slide *const *items;

    if (condFlag) {
        return cur;
    }

    next = cur + 1;
    count = self->count;
    if (next >= count) {
        return cur;
    }
    items = self->slides;

    while (items[cur + 1]->buttons == 1) {
        cur = next;
        next = cur + 1;
        if (next >= count) {
            return cur;
        }
    }
    return cur;
}

/* Toggles `self`'s VRAM-bank flip-flop (`self->toggle`) and streams
 * `self->slides[idx]`'s tile asset (its `+0x200` byte offset - the
 * asset's second half) to whichever of the two OBJ tile VRAM banks the
 * new toggle state selects (`0x06000000`/`0x0600A000`), via
 * `LoadTaggedAsset`. Then rebuilds `gSlideshowDispcnt`'s bit 4 from the
 * toggle's low bit (same `& ~0x10 | bit`-idiom byte-shadow-update shape
 * as `ShowSlidePicture`'s cousin in level_query.c, but for a different
 * global), DMA3-copies the asset's first half into `BG_PLTT` (a second,
 * independent palette-DMA-plus-DISPCNT-write path alongside the
 * already-documented `CommitDispcnt`/`gDispcnt` one - see
 * docs/rom_map.md), and finally commits `gSlideshowDispcnt`'s low
 * halfword straight to `REG_DISPCNT`.
 *
 * Matched, but only after two more register-pinning/ordering gotchas on
 * top of the `self`/`asset` pins the first pass already found:
 * - The `toggle != 0` branch's `asset + 0x200` scratch-offset computation
 *   needs to land in r2 (not gcc's own natural choice of r1, which
 *   happens to already match the *other*, `toggle == 0` branch's
 *   identical-shaped computation) - an `asm volatile("mov r2, #0x80\n\t
 *   lsl r2, r2, #2\n\tadd %0, %1, r2" : "=r"(addr) : "r"(asset) : "r2")`
 *   anchor (the same "hardcode the scratch register, let the output land
 *   wherever" idiom `font.c` uses) forces it.
 * - The `gSlideshowDispcnt` shadow-byte rebuild needed its own two-part
 *   fix: gcc's front end always schedules the `& ~0x10` mask/byte-read
 *   pair *before* the toggle-bit `& 1 << 4` shift-and-mask when both are
 *   written as independent statements (this reconstruction's first
 *   attempt), where the ROM computes the shifted toggle bit first; a
 *   `MATCH_MEMORY_BARRIER()` right after it fixes that. But the barrier
 *   alone widens `bit4`'s tracked value range just enough that the final `orr` gets an extra defensive
 *   `lsl #24; lsr #24` truncation pair the ROM doesn't have - avoided by
 *   writing the AND as `bit4 & toggleByte` (not `toggleByte & bit4`),
 *   which happens to pick the same destination register (r1, not r5) the
 *   ROM's own `ands r1, r5` uses. See
 * docs/matching/archive/issue-38-sound-channel-family.md. */
void ShowSlidePicture(struct cutscene_player *self0, s32 idx)
{
    MATCH_HOLD_REG(struct cutscene_player *, self, r5) = self0;
    const struct cutscene_slide *item = self->slides[idx];
    void *asset = (void *)item->picture;
    s32 toggle = self->toggle ^ 1;

    self->toggle = toggle;

    if (toggle == 0) {
        LoadTaggedAsset((u8 *)asset + 0x200, (void *)VRAM);
    } else {
        MATCH_HOLD_REG(u8 *, addr, r0);

        // clang-format off
        asm volatile("mov r2, #0x80\n\tlsl r2, r2, #2\n\tadd %0, %1, r2"
                     : "=r"(addr) : "r"(asset) : "r2");
        // clang-format on
        LoadTaggedAsset(addr, (void *)(VRAM + 0xA000));
    }

    {
        u8 *shadow = (u8 *)&gSlideshowDispcnt;
        {
            MATCH_HOLD_REG(s32, bit4, r1) = 1;
            MATCH_HOLD_REG(s32, toggleByte, r5);
            MATCH_HOLD_REG(s32, mask, r0);
            MATCH_HOLD_REG(s32, byte, r2);
            MATCH_HOLD_REG(s32, result, r0);

            toggleByte = *(u8 *)&self->toggle;
            bit4 = (bit4 & toggleByte) << 4;
            MATCH_MEMORY_BARRIER();
            mask = ~0x10;
            byte = *shadow;
            result = mask & byte;
            result = result | bit4;
            *shadow = result;
        }
    }

    WaitForVBlank();

    DmaSet(
        3, asset, (void *)PLTT,
        (u32)((DMA_ENABLE | DMA_START_NOW | DMA_16BIT | DMA_SRC_INC | DMA_DEST_INC) << 16 | 0x100));
    REG_DISPCNT = *(u16 *)&gSlideshowDispcnt;
}
