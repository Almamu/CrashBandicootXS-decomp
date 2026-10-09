#include "sprite_obj.hpp"
#include "bg_layer.hpp"
#include "font.hpp"
#include "cutscene.hpp"

extern "C" {
#include "math_util.h"
#include "text.h"
#include "cutscene.h"
#include <libgcc.h>
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* GitHub issue #39: 0x08024810-0x08024960 (game_loop) - the start of
 * the remainder of the UpdateGameFrame-MainLoop cluster after the
 * sound-channel-handle family (slideshow.cpp/slideshow_display.cpp); the
 * background streamer (src/level/bg_streamer.cpp) and BgLayerBase
 * (src/level/bg_layer_base.cpp) follow it (#770).
 *
 * `InitSlideshow`/`RunCutscenePlayer`/`DestroyCutscenePlayer`/`InitCutscenePlayer`
 * are the cutscene player (class CutscenePlayer, include/cutscene.hpp)
 * that slideshow.cpp/slideshow_display.cpp also drive: `ResetSlideshow`
 * sets the VRAM-bank toggle to 1, and `InitCutscenePlayer` also clears
 * `pages`/`font`. `RunCutscenePlayer` is `RunSlideshow`'s (slideshow.cpp)
 * loop over the slides, interleaved with an explicit OAM-shadow-buffer
 * flush (`ResetOamBuffer`/`HideUnusedOamEntries`/`WaitForVBlank`/
 * `CommitOamBuffer` on `gOamBuffer`) and a nested text-paging loop
 * through each slide's `struct cutscene_page`, rendering each string
 * via `DrawWrappedText` (wrapped_text.cpp) with `font` into `box`,
 * continuing to the next string while a held-input mask (9, versus
 * `RunSlideshow`'s 8) stays set. `box.h` divided by the font's line
 * height gives the per-call text-wrap `limit`.
 * `DestroyCutscenePlayer` is a plain two-argument forwarding trampoline to
 * `DestroySlideshow` (slideshow_display.cpp).
 *
 * Built with old_agbcp (the Makefile's OLD_AGBCC_OBJS) - see
 * docs/matching/archive/game-loop-old-agbcc.md. */

/* InitSlideshow, Slideshow's constructor (include/cutscene.hpp): Reset. */
Slideshow::Slideshow()
{
    Reset();
}

/* Per-frame driver loop over `self`'s slides (the loop `RunSlideshow`
 * (slideshow.cpp) also runs), interleaved with an explicit OAM-shadow-buffer
 * flush and a nested text-paging walk through each slide's page. See
 * this file's header comment for the full shape.
 *
 * Matched (old_agbcc). The ROM reloads `&gOamBuffer` from the
 * literal pool at each of the three OAM flushes (rotating r1/r2/r3):
 * that is a function-scope local `oamp` set to the address before the
 * loop, which global-alloc leaves without a register, so reload
 * rematerializes it at each use and r4 stays free for `self`. The
 * prologue reads the box word and `font` into locals before the
 * store, and the page loop is a plain `for` with `j++`. */

void CutscenePlayer::Run()
{
    CutscenePlayer *self = this;
    OamBuffer **oamp = &gOamBuffer;
    s32 limit;
    s32 i;

    {
        s32 b = self->box.x;
        Font *t = self->font;

        t->SetMargin(b);
        limit = t->HeightToLines(self->box.h);
    }
    for (i = 0; i < self->count; i++) {
        u8 res = 1;

        self->ShowPicture(i);
        (*oamp)->Reset();
        (*oamp)->HideUnused();
        WaitForVBlank();
        (*oamp)->Commit();
        self->BeginSlide(i);
        if (self->pages[i].count == 0) {
            res = WaitForKeyPress(self->slides[i]->wait, self->slides[i]->buttons, 9);
        } else {
            s32 j;

            for (j = 0; j < self->pages[i].count && res == 1; j++) {
                u8 *str = (u8 *)self->pages[i].strings[j];
                s32 pos = 0;

                while (str[pos] != 0 && res == 1) {
                    pos += DrawWrappedText(str + pos, self->font, &self->box, limit, 1);
                    res = WaitForKeyPress(self->slides[i]->wait, self->slides[i]->buttons, 9);
                }
            }
        }
        self->EndSlide(i);
        i = self->Skip(i, res);
    }
}

/* DestroyCutscenePlayer: g++'s destructor of a derived class with nothing
 * of its own to tear down passes its __in_chrg on to the base's
 * (DestroySlideshow), which frees `this` if asked. */
CutscenePlayer::~CutscenePlayer()
{
}

/* InitCutscenePlayer: the slideshow's constructor, then no pages and no
 * font yet. */
CutscenePlayer::CutscenePlayer()
{
    pages = 0;
    font = 0;
}
