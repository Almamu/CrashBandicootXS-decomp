#ifndef GUARD_FADE_HPP
#define GUARD_FADE_HPP

/* The screen fades (src/engine/gfx/fade.cpp, fade_to_black.cpp, #915), two
 * all-static classes:
 * - BrightnessFade, the hardware brightness fade: 17 steps of REG_BLDY
 *   toward white or black, run in a loop or one step a frame from a
 *   VBlank callback;
 * - PaletteFade, the palette fade to black the rooms end with: the
 *   palette darkened in steps through an IWRAM copy of it.
 *
 * All-static like the frame limiter (docs/cplusplus.md, "The brightness
 * and palette fades as all-static classes"): their variables are IWRAM
 * globals that no other object reads, and static data members compile
 * like those globals, static member functions like plain functions.
 * cxx_symbols.txt (block "#915 BrightnessFade and PaletteFade") maps each
 * member to its C name (`_Q36Engine3Gfx14BrightnessFade.state` is gBrightnessFade,
 * ...): games/xs/sym_iwram.txt places PaletteFade's, iwram_data.cpp defines
 * BrightnessFade's with the other initialised IWRAM globals. The
 * functions keep their C names the same way (`Step__Q36Engine3Gfx14BrightnessFade` is
 * StepBrightnessFade).
 *
 * In the namespace Engine::Gfx (#1033, docs/cplusplus.md "Namespaces").
 *
 * `#pragma interface`: no vtable to emit; it keeps g++ from emitting
 * out-of-line copies of inline methods (docs/cplusplus.md). */
#pragma interface

extern "C" {
#include "core.h"
}

struct brightness_fade; // gfx.h

namespace Engine
{
namespace Gfx
{

class BrightnessFade
{
public:
    static void Step(); // StepBrightnessFade: the VBlank callback Start adds
    /* FadeBrightness: `flags` FADE_FLAG_*, `frameDelay` frames a step;
     * with `sync`, a step a frame from the VBlank callback (Step) */
    static void Start(u8 flags, s32 frameDelay, u8 sync);
    static bool IsActive(); // IsBrightnessFadeActive

private:
    /* iwram_data.cpp */
    static struct brightness_fade state; // gBrightnessFade - `period` -1 when idle
    static s32 step;                     // gBrightnessFadeStep - 0-16, the next BLDY step
    static s32 timer;                    // gBrightnessFadeTimer - the frames into this step
};

class PaletteFade
{
public:
    static void Darken(s32 factor); // DarkenPalette: `backup` by factor/16 into `buffer`
    static void FadeToBlack();      // FadePaletteToBlack

private:
    /* IWRAM (games/xs/sym_iwram.txt) */
    static u16 backup[512]; // gPaletteBackup - the palette RAM when the fade started
    static u16 buffer[512]; // gPaletteFadeBuffer - the darkened copy DMA'd to palette RAM
};

} // namespace Gfx
} // namespace Engine

#endif // GUARD_FADE_HPP
