#include "cutscene.hpp"
#include "audio.hpp"

extern "C" {
#include "core.h"
#include "gba/dma_macros.h"
#include "cutscene.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* GitHub issue #38: 0x08024590-0x08024783 (game_loop), the sound-channel-
 * handle helper family - see docs/matching/archive/issue-38-medal-results-tally.md
 * and docs/matching/archive/issue-38-sound-channel-family.md. All four functions
 * are Slideshow's methods (include/cutscene.hpp; C++ since the #664
 * cleanup), built with old_agbcp as the C was with old_agbcc - see
 * docs/matching/archive/game-loop-old-agbcc.md. */

/* Starts sound cue `slides[idx]->cue` on the audio context. If the
 * channel already reports that cue, plays the item's secondary sfx
 * (unless it is the SFX_NONE sentinel, 0x63) and then starts the item's
 * fade; otherwise starts the fade first, then busy-waits for the cue
 * before playing the sfx. */
void Slideshow::BeginSlide(s32 idx)
{
    Slideshow *self = this;
    const struct cutscene_slide *item;

    gAudioContext->PlaySong(self->slides[idx]->cue);
    if (gAudioContext->GetCurrentSong() == (item = self->slides[idx])->cue) {
        if (item->sfx != SFX_NONE)
            gAudioContext->PlaySfx(item->sfx, 0x100);
        FadeBrightness(self->slides[idx]->fade | -0x80, 1, 0);
    } else {
        FadeBrightness(item->fade | -0x80, 1, 0);
        if (self->slides[idx]->sfx != SFX_NONE) {
            while (gAudioContext->GetCurrentSong() != self->slides[idx]->cue)
                ;
            gAudioContext->PlaySfx(self->slides[idx]->sfx, 0x100);
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
 * (cutscene_player.cpp), the same loop with the text pages added. */
void Slideshow::Run()
{
    Slideshow *self = this;
    s32 i;

    for (i = 0; i < self->count; i++) {
        u8 checkButtons;
        const struct cutscene_slide *item;

        self->ShowPicture(i);
        self->BeginSlide(i);

        item = self->slides[i];
        checkButtons = (u8)WaitForKeyPress(item->wait, item->buttons, 8);

        if (self->slides[i]->duckMusic != 0) {
            gAudioContext->FadeOutMusic(0);
        }

        {
            s32 v = self->slides[i]->fadeAfter;

            if (v != -1) {
                FadeBrightness((u8)v, 1, 0);
            }
        }

        {
            const struct cutscene_slide *item2 = self->slides[i];

            if (item2->rearmSfx != 0 && item2->sfx != SFX_NONE) {
                gAudioContext->StopSfx(item2->sfx);
            }
        }

        i = self->Skip(i, checkButtons);
    }
}

/* Scans forward from `startIdx + 1` for the next item whose `buttons`
 * isn't 1 ("busy"), returning the index just before it (or the last
 * index reached if every remaining item is busy). Returns `startIdx`
 * unchanged if `condFlag` is set, or if `startIdx + 1` is already past
 * the list. */
s32 Slideshow::Skip(s32 startIdx, u8 condFlag)
{
    Slideshow *self = this;
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
 * as `ShowSlidePicture`'s cousin in level_query.cpp, but for a different
 * global), DMA3-copies the asset's first half into `BG_PLTT` (a second,
 * independent palette-DMA-plus-DISPCNT-write path alongside the
 * already-documented `CommitDispcnt`/`gDispcnt` one - see
 * docs/rom_map.md), and finally commits `gSlideshowDispcnt`'s low
 * halfword straight to `REG_DISPCNT`.
 *
 * As C it needed `self` pinned to r5, the `asset + 0x200` computation
 * as an asm block and the DISPCNT shadow-byte rebuild pinned register by
 * register behind a memory barrier
 * (docs/matching/archive/issue-38-sound-channel-family.md); as a C++
 * method under old_agbcp none of that is needed: the rebuild only has to
 * keep the mask and the shadow byte in `s32` locals (so the AND is done
 * on the word, `movs #17; negs`, not folded to a byte `0xef`). */
void Slideshow::ShowPicture(s32 idx)
{
    Slideshow *self = this;
    const struct cutscene_slide *item = self->slides[idx];
    void *asset = (void *)item->picture;
    s32 toggle = self->toggle ^ 1;

    self->toggle = toggle;

    if (toggle == 0) {
        LoadTaggedAsset((u8 *)asset + 0x200, (void *)VRAM);
    } else {
        LoadTaggedAsset((u8 *)asset + 0x200, (void *)(VRAM + 0xA000));
    }

    {
        u8 *shadow = (u8 *)&gSlideshowDispcnt;
        {
            s32 bit4 = (1 & *(u8 *)&self->toggle) << 4;
            s32 mask = ~0x10;
            s32 byte = *shadow;

            *shadow = (mask & byte) | bit4;
        }
    }

    WaitForVBlank();

    DmaSet(
        3, asset, (void *)PLTT,
        (u32)((DMA_ENABLE | DMA_START_NOW | DMA_16BIT | DMA_SRC_INC | DMA_DEST_INC) << 16 | 0x100));
    REG_DISPCNT = *(u16 *)&gSlideshowDispcnt;
}
