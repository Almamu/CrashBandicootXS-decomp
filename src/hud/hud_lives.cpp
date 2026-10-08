#include "hud.hpp"

extern "C" {
#include "core.h"
#include "util.h"
#include <libgcc.h>
#include "globals.h"
}

/* Hud::UpdateLives (UpdateHudLives, #664 cleanup, include/hud.hpp): the
 * lives counter, two digits (parts 0 and 1; a single-digit count hides
 * the second) and the icon (part 2), redrawn every frame while the
 * counter is on screen.
 *
 * Built with old_agbcp (the C was agbcc, held to the ROM by 34 register
 * pins, five instruction asm statements and a volatile hold): its clamp
 * sequence is the ROM's. The second digit's value is computed before its
 * part's address, as the ROM does. */
void Hud::UpdateLives()
{
    HudPart *cur;

    if (livesSlide == 0)
        return;

    if (GetLives(gLevelState) > 0)
        lives = GetLives(gLevelState);
    else
        lives = 0;

    if (livesSlide == 1 || livesSlide == 3)
        gHudSlideOffset = livesSlideTimer * 2 - 0x28;
    else
        gHudSlideOffset = 0;

    {
        s32 v = lives;
        s32 w = shownLives;

        cur = parts;
        if (v != w) {
            if (v > 9) {
                s32 f = __divsi3(v, 10);

                HUDPART_CLAMP_FRAME(&cur[0], cur[0].tag, f);
                f = __modsi3(lives, 10);
                HUDPART_CLAMP_FRAME(&cur[1], cur[1].tag, f);
            } else {
                HUDPART_CLAMP_FRAME(&cur[0], cur[0].tag, v);
                /* The ROM keeps this clamp's dead `tag` load: -1 is
                 * never past the end, so only the store is left. */
                HUDPART_CLAMP_FRAME(&cur[1], cur[1].tag, -1);
            }
        }
    }

    cur[2].Draw(0, 0);
    parts[0].Draw(0, 0);
    parts[1].Draw(0, 0);
    shownLives = lives;
}
