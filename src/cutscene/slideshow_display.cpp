#include "cutscene.hpp"
#include "audio.hpp"

extern "C" {
#include "core.h"
#include "cutscene.h"
#include "gfx.h"
#include "memory.h"
#include "globals.h"
}

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * level_query.cpp - see level_query.cpp's header comment and
 * docs/matching/archive/issue-38-medal-results-tally.md for the full write-up.
 * This single function sits between the BeginSlide..ShowSlidePicture run
 * (slideshow.cpp - BeginSlide NAKED-parked, the rest matched - see
 * docs/matching/archive/issue-38-sound-channel-family.md) and EndSlide
 * (matched, slideshow_display.cpp). */

/* Trivial setter: `gSlideshowDispcnt = value` - see slideshow.cpp's
 * ShowSlidePicture for the other (bitfield-level) writer of this same
 * global. */
void SetSlideshowDispcnt(u32 value)
{
    gSlideshowDispcnt = value;
}

/* GitHub issue #38: 0x08024790-0x080247EB (game_loop) - continuation of
 * the sound-channel-handle helper family (slideshow.cpppp). See
 * docs/matching/archive/issue-38-medal-results-tally.md for the original chunk
 * write-up and docs/matching/archive/issue-38-sound-channel-family.md for this
 * follow-up pass. */

/* Tail half of RunSlideshow's per-item body (slideshow.cpppp) - duck-out
 * (`duckMusic`), fade-start (`fadeAfter`), and re-arm (`rearmSfx`/
 * `sfx`) - reused standalone against a caller-supplied index. Each
 * of the three checks re-reads `slides[idx]` fresh rather than
 * sharing one cached pointer across all three, matching RunSlideshow's
 * own inlined copy of this same body. */
void Slideshow::EndSlide(s32 idx)
{
    if (slides[idx]->duckMusic != 0) {
        gAudioContext->FadeOutMusic(0);
    }

    {
        s32 v = slides[idx]->fadeAfter;

        if (v != -1) {
            FadeBrightness((u8)v, 1, 0);
        }
    }

    {
        const struct cutscene_slide *item = slides[idx];

        if (item->rearmSfx != 0 && item->sfx != SFX_NONE) {
            gAudioContext->StopSfx(item->sfx);
        }
    }
}

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * slideshow_display.cpp - see level_query.cpp's header comment and
 * docs/matching/archive/issue-38-medal-results-tally.md for the full write-up.
 * This is the tail of the chunk, right after EndSlide (parked/left
 * in asm/code_3_2_17_24790.s). */

/* DestroySlideshow: nothing to tear down; g++'s deleting destructor frees
 * `this` when bit 0 of its __in_chrg is set (the same shape as
 * level_query.cpp's DestroyUnusedLevelObject). */
Slideshow::~Slideshow()
{
}

/* ResetSlideshow: no slides, and the ShowPicture VRAM-bank toggle starts
 * at 1. The constructor (cutscene_player.cpp) calls it. */
void Slideshow::Reset()
{
    slides = 0;
    count = 0;
    toggle = 1;
}
