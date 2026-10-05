#ifndef GUARD_CUTSCENE_H
#define GUARD_CUTSCENE_H

#include "gba/types.h"
#include "aabb.h"

/*
 * The cutscene subsystem (src/cutscene/): the slideshow player that shows
 * the cutscenes (src/data/cutscenes_16d1c8.c). PlayCutscene
 * (level_cutscene.c) plays cutscene `idx` as a sequence of slides, each a
 * full-screen picture shown with its page of text in the current
 * language.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The background streamer and layer functions in cutscene_player.c
 * (DecodeLayerChunk..StepBgLayerScroll) only share its ROM range. They
 * go in the level subsystem's header (docs/headers_plan.md, "Who owns a
 * symbol").
 */

struct bitmap_font;

/* One slide. */
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

/* A cutscene's slides. */
struct cutscene_slides
{
    const struct cutscene_slide *const *slides;
    s32 count;
};

/* The text of one slide: strings shown one after the other. */
struct cutscene_page
{
    const u8 *const *strings;
    s32 count;
};

/* The slideshow player. ResetSlideshow/InitCutscenePlayer construct one
 * (PlayCutscene keeps it on the stack), RunCutscenePlayer shows its
 * slides with their pages of text, and RunSlideshow (unused) shows the
 * slides alone. */
struct cutscene_player
{
    const struct cutscene_slide *const *slides; // 0x00
    s32 count;                  // 0x04
    u8 unk_08[4];
    s32 toggle;                 // 0x0C - ShowSlidePicture's VRAM-bank flip-flop,
                                //        1 after ResetSlideshow
    const struct cutscene_page *pages; // 0x10 - one per slide
    struct bitmap_font *font;   // 0x14
    struct aabb box;            // 0x18 - the text rectangle
};

/* The cutscenes, one {slides, count} header each
 * (src/data/cutscenes_16d1c8.c). */
extern const struct cutscene_slides gCutscenes[11];

/* Every cutscene's pages, per language (gLanguage), each indexed like
 * gCutscenes. Defined in src/iwram/iwram_data.c. */
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

/* src/cutscene/slideshow.c */
extern void BeginSlide(struct cutscene_player *self, s32 idx);
extern void RunSlideshow(struct cutscene_player *self);
extern s32 SkipSlides(struct cutscene_player *self, s32 startIdx, u8 condFlag);
extern void ShowSlidePicture(struct cutscene_player *self, s32 idx);

/* src/cutscene/slideshow_display.c */
extern void SetSlideshowDispcnt(u32 value);
extern void EndSlide(struct cutscene_player *self, s32 idx);
extern void DestroySlideshow(struct cutscene_player *self, s32 flags);
extern void ResetSlideshow(struct cutscene_player *self);

/* src/cutscene/cutscene_player.c */
extern struct cutscene_player *InitSlideshow(struct cutscene_player *self);
extern void RunCutscenePlayer(struct cutscene_player *self);
extern void DestroyCutscenePlayer(struct cutscene_player *self, s32 flags);
extern struct cutscene_player *InitCutscenePlayer(struct cutscene_player *self);

#endif // GUARD_CUTSCENE_H
