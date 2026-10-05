#ifndef GUARD_CUTSCENE_H
#define GUARD_CUTSCENE_H

#include "gba/types.h"

/*
 * The cutscenes (src/data/cutscenes_16d1c8.c): PlayCutscene
 * (level_cutscene.c) plays cutscene `idx` as a sequence of
 * slides, each a full-screen picture shown with its page of text in the
 * current language. The code reads the same records through its own
 * local views, named on each struct below.
 */

/* One slide: `struct SoundChannelItem` (slideshow.c), `struct
 * pager_item` (cutscene_player.c). */
struct cutscene_slide
{
    const u16 *picture;     // 0x00 - a 256-colour palette, followed by the
                            //        LZ77 Mode 4 bitmap (ShowSlidePicture reads
                            //        it at +0x200)
    s32 wait;               // 0x04 - WaitForKeyPress's frame count
    s32 fade;               // 0x08 - FadeBrightness flags (| 0x80) as the
                            //        slide starts
    s32 fadeAfter;          // 0x0C - FadeBrightness flags after it, -1 = none
    u8 buttons;             // 0x10 - WaitForKeyPress's checkButtons; 1 = the
                            //        slide is skipped past (SkipSlides)
    u8 duckMusic;           // 0x11 - FadeOutMusic after the slide
    u8 rearmSfx;            // 0x12 - replay `sfx` after the slide
    u32 cue;                // 0x14 - music cue (PlaySong)
    u32 sfx;                // 0x18 - sound effect, 99 = none
};

/* A cutscene's slides: `struct text_list` (level_cutscene.c). */
struct cutscene_slides
{
    const struct cutscene_slide *const *slides;
    s32 count;
};

/* The text of one slide: `struct pager_text` (cutscene_player.c), strings
 * shown one after the other. */
struct cutscene_page
{
    const u8 *const *strings;
    s32 count;
};

#endif // GUARD_CUTSCENE_H
