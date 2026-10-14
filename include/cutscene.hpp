#ifndef GUARD_CUTSCENE_HPP
#define GUARD_CUTSCENE_HPP

/* The cutscene player as C++ (#664 cleanup, docs/cplusplus.md): the
 * slideshow (src/cutscene/slideshow.cpp, slideshow_display.cpp) and the
 * cutscene player built on it (src/cutscene/cutscene_player.cpp).
 * PlayCutscene (level_cutscene.cpp) constructs a CutscenePlayer in place
 * in a stack aggregate (where the ROM calls the constructor, after the
 * display setup). Neither class has a vtable, and neither has a C view
 * (the C `struct cutscene_player` went in #754).
 *
 * The cutscenes themselves are data (src/data/cutscenes_16d1c8.cpp,
 * cutscene_slides_16e064.cpp): the slide and page tables below. The
 * cutscene text is C (src/data/cutscene_text_*.c) and includes nothing.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to
 * emit; the pragma keeps g++ from emitting out-of-line copies of inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

extern "C" {
#include "core.h"
#include "aabb.h"
}

/* One slide. */
struct cutscene_slide {
    const u16 *picture; // 0x00 - a 256-colour palette, followed by the
                        //        LZ77 Mode 4 bitmap (ShowSlidePicture reads
                        //        it at +0x200)
    s32 wait;           // 0x04 - WaitForKeyPress's frame count
    s32 fade;           // 0x08 - BrightnessFade::Start flags (| 0x80) as the
                        //        slide starts
    s32 fadeAfter;      // 0x0C - BrightnessFade::Start flags after it, -1 = none
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
 * (src/data/cutscenes_16d1c8.cpp). */
extern const struct cutscene_slides gCutscenes[11];

/* Every cutscene's pages, per language (gLanguage), each indexed like
 * gCutscenes. Defined in src/iwram/iwram_data.cpp. */
extern const struct cutscene_page *const *gCutsceneTexts[6];
/* The tables gCutsceneTexts points at (src/data/cutscenes_16d1c8.cpp). */
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

/* src/cutscene/slideshow_display.cpp, with C linkage (its C name). */
extern "C" void SetSlideshowDispcnt(u32 value);

namespace Engine
{
namespace Text
{
class Font;
} // namespace Text
} // namespace Engine

namespace Game
{
namespace Ui
{

/* A list of slides (pictures with a song, a fade and a sound effect), shown
 * one after the other. The constructor (InitSlideshow) calls Reset; only
 * the cutscene player is ever made, and RunSlideshow, the text-less loop,
 * is unused. */
class Slideshow
{
public:
    const struct cutscene_slide *const *slides; // 0x00
    s32 count;                                  // 0x04
    u8 unk_08[4];
    s32 toggle; // 0x0C - ShowPicture's VRAM-bank flip-flop, 1 after Reset

    Slideshow();                         // InitSlideshow
    ~Slideshow();                        // DestroySlideshow
    void BeginSlide(s32 idx);            // BeginSlide
    void Run();                          // RunSlideshow (UNUSED)
    s32 Skip(s32 startIdx, u8 condFlag); // SkipSlides
    void ShowPicture(s32 idx);           // ShowSlidePicture
    void EndSlide(s32 idx);              // EndSlide
    void Reset();                        // ResetSlideshow
};

COMPILE_TIME_ASSERT(cutscene_hpp, sizeof(Slideshow) == 0x10);

/* The cutscene player: the slideshow plus each slide's pages of text,
 * drawn with `font` into `box` (PlayCutscene, level_cutscene.cpp). */
class CutscenePlayer : public Slideshow
{
public:
    const struct cutscene_page *pages; // 0x10 - one per slide
    Engine::Text::Font *font;          // 0x14 (include/font.hpp)
    struct aabb box;                   // 0x18 - the text rectangle

    CutscenePlayer();  // InitCutscenePlayer
    ~CutscenePlayer(); // DestroyCutscenePlayer
    void Run();        // RunCutscenePlayer
};

COMPILE_TIME_ASSERT(cutscene_hpp, sizeof(CutscenePlayer) == 0x28);

} // namespace Ui
} // namespace Game

#endif /* !GUARD_CUTSCENE_HPP */
