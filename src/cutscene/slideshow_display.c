#include "core.h"
#include "match.h"
#include "cutscene.h"
#include "audio.h"
#include "gfx.h"
#include "memory.h"
#include "globals.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * level_query.c - see level_query.c's header comment and
 * docs/matching/archive/issue-38-medal-results-tally.md for the full write-up.
 * This single function sits between the BeginSlide..ShowSlidePicture run
 * (slideshow.c - BeginSlide NAKED-parked, the rest matched - see
 * docs/matching/archive/issue-38-sound-channel-family.md) and EndSlide
 * (matched, slideshow_display.c). */

/* Trivial setter: `gSlideshowDispcnt = value` - see slideshow.c's
 * ShowSlidePicture for the other (bitfield-level) writer of this same
 * global. */
void SetSlideshowDispcnt(u32 value)
{
    gSlideshowDispcnt = value;
}

/* GitHub issue #38: 0x08024790-0x080247EB (game_loop) - continuation of
 * the sound-channel-handle helper family (slideshow.c). See
 * docs/matching/archive/issue-38-medal-results-tally.md for the original chunk
 * write-up and docs/matching/archive/issue-38-sound-channel-family.md for this
 * follow-up pass. */

struct AudioContext;

/* Tail half of RunSlideshow's per-item body (slideshow.c) - duck-out
 * (`duckMusic`), fade-start (`fadeAfter`), and re-arm (`rearmSfx`/
 * `sfx`) - reused standalone against a caller-supplied index. Each
 * of the three checks re-reads `self->slides[idx]` fresh rather than
 * sharing one cached pointer across all three, matching RunSlideshow's
 * own inlined copy of this same body. */
void EndSlide(struct cutscene_player *self0, s32 idx)
{
    MATCH_HOLD_REG(struct cutscene_player *, self, r5) = self0;

    if (self->slides[idx]->duckMusic != 0) {
        FadeOutMusic(gAudioContext, 0);
    }

    {
        s32 v = self->slides[idx]->fadeAfter;

        if (v != -1) {
            FadeBrightness((u8)v, 1, 0);
        }
    }

    {
        const struct cutscene_slide *item = self->slides[idx];

        if (item->rearmSfx != 0 && item->sfx != 0x63) {
            StopSfx(gAudioContext, item->sfx);
        }
    }
}

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * slideshow_display.c - see level_query.c's header comment and
 * docs/matching/archive/issue-38-medal-results-tally.md for the full write-up.
 * This is the tail of the chunk, right after EndSlide (parked/left
 * in asm/code_3_2_17_24790.s). */

/* Same wrapper shape as sub_802425C (level_query.c): tears `self` down
 * via OperatorDelete if bit 0 of `flags` is set. */
void DestroySlideshow(struct cutscene_player *self, s32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Trivial constructor: no slides, and the ShowSlidePicture VRAM-bank
 * toggle starts at 1. */
void ResetSlideshow(struct cutscene_player *self)
{
    self->slides = NULL;
    self->count = 0;
    self->toggle = 1;
}
