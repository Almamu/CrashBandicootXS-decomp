#ifndef GUARD_CUTSCENE_H
#define GUARD_CUTSCENE_H

#include "gba/types.h"
#include "aabb.h"

/*
 * The cutscene subsystem (src/cutscene/): the slideshow player that shows
 * the cutscenes (src/data/cutscenes_16d1c8.c). PlayCutscene
 * (level_cutscene.cpp) plays cutscene `idx` as a sequence of slides, each a
 * full-screen picture shown with its page of text in the current
 * language.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The slideshow and the cutscene player are C++ classes
 * (include/cutscene.hpp) with no C view. The background streamer and
 * layer classes in cutscene_player.cpp
 * (BgStreamer, BgLayerBase: include/bg_layer.hpp) only share its ROM
 * range.
 */

/* One slide. */
struct cutscene_slide {
    const u16 *picture; // 0x00 - a 256-colour palette, followed by the
                        //        LZ77 Mode 4 bitmap (ShowSlidePicture reads
                        //        it at +0x200)
    s32 wait;           // 0x04 - WaitForKeyPress's frame count
    s32 fade;           // 0x08 - FadeBrightness flags (| 0x80) as the
                        //        slide starts
    s32 fadeAfter;      // 0x0C - FadeBrightness flags after it, -1 = none
    u8 buttons;         // 0x10 - WaitForKeyPress's checkButtons; 1 = the
                        //        slide is skipped past (SkipSlides)
    u8 duckMusic;       // 0x11 - FadeOutMusic after the slide
    u8 rearmSfx;        // 0x12 - replay `sfx` after the slide
    u32 cue;            // 0x14 - music cue (PlaySong)
    u32 sfx;            // 0x18 - sound effect, 99 = none
};

/* A cutscene's slides. */
struct cutscene_slides {
    const struct cutscene_slide *const *slides;
    s32 count;
};

/* The text of one slide: strings shown one after the other. */
struct cutscene_page {
    const u8 *const *strings;
    s32 count;
};

/* The cutscenes, one {slides, count} header each
 * (src/data/cutscenes_16d1c8.c). */
extern const struct cutscene_slides gCutscenes[11];

/* Every cutscene's pages, per language (gLanguage), each indexed like
 * gCutscenes. Defined in src/iwram/iwram_data.cpp. */
extern const struct cutscene_page *const *gCutsceneTexts[6];
/* The tables gCutsceneTexts points at (src/data/cutscenes_16d1c8.c). */
extern const struct cutscene_page *const gCutsceneTextEnglish[11];
extern const struct cutscene_page *const gCutsceneTextFrench[11];
extern const struct cutscene_page *const gCutsceneTextGerman[11];
extern const struct cutscene_page *const gCutsceneTextSpanish[11];
extern const struct cutscene_page *const gCutsceneTextItalian[11];
extern const struct cutscene_page *const gCutsceneTextDutch[11];

/* The DISPCNT value the slideshow shows its pictures with:
 * SetSlideshowDispcnt sets it and ShowSlidePicture sets its bit 4 (the
 * Mode 4 frame) to the VRAM bank it streamed the picture into. */
extern u32 gSlideshowDispcnt;

/* src/cutscene/slideshow_display.cpp (C linkage) */
extern void SetSlideshowDispcnt(u32 value);

#endif // GUARD_CUTSCENE_H
