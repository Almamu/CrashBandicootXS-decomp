#include "core.h"

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * game_loop18.c - see game_loop17.c's header comment and
 * docs/matching/issue-38-medal-results-tally.md for the full write-up.
 * This single function sits between the BeginSlide..ShowSlidePicture run
 * (game_loop37.c - BeginSlide NAKED-parked, the rest matched - see
 * docs/matching/issue-38-sound-channel-family.md) and EndSlide
 * (matched, game_loop38.c). */

extern void *gSlideshowDispcnt;

/* Trivial setter: `gSlideshowDispcnt = value` - see game_loop37.c's
 * ShowSlidePicture for the other (bitfield-level) writer of this same
 * global. */
void SetSlideshowDispcnt(void *value)
{
    gSlideshowDispcnt = value;
}

/* GitHub issue #38: 0x08024790-0x080247EB (game_loop) - continuation of
 * the sound-channel-handle helper family (game_loop37.c). See
 * docs/matching/issue-38-medal-results-tally.md for the original chunk
 * write-up and docs/matching/issue-38-sound-channel-family.md for this
 * follow-up pass. */

struct AudioContext;

/* Same `SoundChannelItem`/`SoundChannelList` shape game_loop37.c's
 * BeginSlide/RunSlideshow/ShowSlidePicture operate on - kept as this file's
 * own local copy (only the three fields this function reads), per this
 * project's established per-translation-unit convention for structs
 * shared across non-adjacent files. */
struct SoundChannelItem {
    u8 unused_00[0xc];
    s32 field_0c; /* +0x0c: sentinel -1 means "none" */
    u8 unused_10;
    u8 field_11;    /* +0x11: nonzero triggers a duck-out */
    u8 field_12;      /* +0x12: nonzero (and field_18 != 0x63) triggers a
                        * re-arm */
    u8 unused_13;
    u8 unused_14[4];
    u32 field_18; /* +0x18: sentinel 0x63 (99) means "no sfx" */
};

struct SoundChannelList {
    struct SoundChannelItem **items; /* +0x00 */
};

extern struct AudioContext *gAudioContext;
extern void FadeOutMusic(struct AudioContext *self, u32 value);
extern void FadeBrightness(u8 flags, s32 frameDelay, u8 sync);
extern void StopSfx(struct AudioContext *self, u32 id);

/* Tail half of RunSlideshow's per-item body (game_loop37.c) - duck-out
 * (`field_11`), fade-start (`field_0c`), and re-arm (`field_12`/
 * `field_18`) - reused standalone against a caller-supplied index. Each
 * of the three checks re-reads `self->items[idx]` fresh rather than
 * sharing one cached pointer across all three, matching RunSlideshow's
 * own inlined copy of this same body. */
void EndSlide(struct SoundChannelList *self0, s32 idx)
{
    register struct SoundChannelList *self asm("r5") = self0;

    if (self->items[idx]->field_11 != 0) {
        FadeOutMusic(gAudioContext, 0);
    }

    {
        s32 v = self->items[idx]->field_0c;

        if (v != -1) {
            FadeBrightness((u8)v, 1, 0);
        }
    }

    {
        struct SoundChannelItem *item = self->items[idx];

        if (item->field_12 != 0 && item->field_18 != 0x63) {
            StopSfx(gAudioContext, item->field_18);
        }
    }
}

/* GitHub issue #38: 0x0802425C-0x08024810 (game_loop), continued from
 * game_loop19.c - see game_loop17.c's header comment and
 * docs/matching/issue-38-medal-results-tally.md for the full write-up.
 * This is the tail of the chunk, right after EndSlide (parked/left
 * in asm/code_3_2_17_24790.s). */

extern void OperatorDelete(void *self);

/* Same wrapper shape as sub_802425C (game_loop17.c): tears `self` down
 * via OperatorDelete if bit 0 of `flags` is set. */
void DestroySlideshow(void *self, s32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Trivial constructor: zeroes `self->0`/`self->4`, sets `self->0xc` to
 * 1 (the ShowSlidePicture VRAM-bank toggle's initial state, see
 * asm/code_3_2_17_24590.s). */
void ResetSlideshow(void *self)
{
    u8 *s = (u8 *)self;

    *(s32 *)s = 0;
    *(s32 *)(s + 4) = 0;
    *(s32 *)(s + 0xc) = 1;
}
