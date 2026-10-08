#ifndef GUARD_CUTSCENE_HPP
#define GUARD_CUTSCENE_HPP

/* The cutscene player as C++ (#664 cleanup, docs/cplusplus.md): the
 * slideshow (src/cutscene/slideshow.cpp, slideshow_display.cpp) and the
 * cutscene player built on it (src/cutscene/cutscene_player.cpp).
 * cutscene.h's struct cutscene_player is the C view, for PlayCutscene
 * (level_cutscene.cpp), which keeps one inside a stack aggregate (so it is
 * constructed where the ROM calls the constructor, after the display
 * setup) and calls the constructor, RunCutscenePlayer and the destructor
 * by their C names (cxx_symbols.txt). Neither class has a vtable.
 *
 * `#pragma interface`: no class here has a vtable, so there is none to
 * emit; the pragma keeps g++ from emitting out-of-line copies of inline
 * methods (docs/cplusplus.md, "Emitting the vtables"). */
#pragma interface

extern "C" {
#include "core.h"
#include "aabb.h"
#include "cutscene.h"
}

class Font;

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
    Font *font;                        // 0x14 (include/font.hpp)
    struct aabb box;                   // 0x18 - the text rectangle

    CutscenePlayer();  // InitCutscenePlayer
    ~CutscenePlayer(); // DestroyCutscenePlayer
    void Run();        // RunCutscenePlayer
};

COMPILE_TIME_ASSERT(cutscene_hpp, sizeof(CutscenePlayer) == sizeof(struct cutscene_player));

#endif /* !GUARD_CUTSCENE_HPP */
