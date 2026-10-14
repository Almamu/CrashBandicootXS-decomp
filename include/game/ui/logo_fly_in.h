#ifndef GUARD_LOGO_FLY_IN_H
#define GUARD_LOGO_FLY_IN_H

#include "screen.h"
#include "engine/util/math_util.h"

/* The logo pieces' fly-ins (#979, docs/screen.md, "The logo fly-ins").
 *
 * A logo piece moves through its motion (struct delta_record,
 * frontend.hpp) a step at a time. A step sets the piece's x and y, then
 * adds its deltas (Q16.16 pixels a frame) for the step's hold - 1 frames,
 * and the next step sets the position it has reached: the ROM's deltas
 * are (next x - x) * 0x10000 / hold. The positions are on the 240x160
 * logo art, from its origin DISPLAY_ART_X/Y.
 *
 * Twenty-one pieces fly in from outside the art: the Vicarious Visions
 * logo's pieces 1-19 from its right (their first step starts at x 248,
 * 240 for piece 19) and the title logo's pieces 0 and 1 from its bottom
 * and bottom-left (y 160, and x -96). That first step is the only one
 * outside the art. On the GBA the art fills the screen, and it starts
 * just off screen; on a bigger screen it would start on it.
 *
 * So the first step is lengthened backwards, through the screen's margin
 * around the art (DISPLAY_ART_MARGIN_*, screen.h): it starts
 * LOGO_FLY_IN_FRAMES frames earlier, as far past the screen's edge as the
 * ROM's start is past the art's border (or up to a frame's move further),
 * and moves at the ROM's speed: the same deltas, so the piece passes the
 * ROM's start on the frame the ROM's step would have set it, and reaches
 * the next step where the ROM's does. The piece's seed (its delay before
 * the first step, gVvLogoPieceSeeds, gTitleLogoPieceSeeds) is as many
 * frames shorter, so every piece lands, and the logo plays, on the ROM's
 * frames. The tables keep their length and the code that walks them is
 * the same.
 *
 * On the GBA every margin is 0, so every fly-in is 0 frames: each step,
 * hold and seed folds to the ROM's number. The seeds' delays (0x54 at
 * least, 84 frames) bound a fly-in, and so how big a margin can be. */

/* The frames a piece moving `speed` (Q16.16 pixels a frame, either sign)
 * takes to cross `margin` pixels, rounded up: 0 for no margin. */
#define LOGO_FLY_IN_FRAMES(margin, speed) ((INT_TO_Q16(margin) + ABS(speed) - 1) / ABS(speed))

/* A first step's x or y `frames` frames earlier: `pos`, less what `speed`
 * covers in that time, as the u16 field stores it (a negative one wraps,
 * see struct delta_record). */
#define LOGO_FLY_IN_POS(pos, frames, speed) ((u16)((pos) - Q16_TO_INT((frames) * (speed))))

/* A fly-in's first step: the ROM's `hold` frames from (`x`, `y`) at `dx`,
 * `dy` a frame, lengthened by `frames` frames before it. The rest of the
 * record (no depth, a 1:1 scale) is the same in every one. */
#define LOGO_FLY_IN_STEP(hold, x, y, frames, dx, dy)                                                                   \
    {                                                                                                                  \
        (hold) + (frames), LOGO_FLY_IN_POS(x, frames, dx), LOGO_FLY_IN_POS(y, frames, dy), 0, 256, 256, dx, dy, 0, 0, \
            0                                                                                                          \
    }

/* The fly-ins' speeds (their first step's deltas, the ROM's) and lengths.
 * The Vicarious Visions logo's pieces come in from the right, along a
 * row. */
#define VV_LOGO_FLY_IN_FRAMES(dx) LOGO_FLY_IN_FRAMES(DISPLAY_ART_MARGIN_RIGHT, dx)

#define VV_LOGO_PIECE01_DX     (-575260)
#define VV_LOGO_PIECE02_DX     (-711533)
#define VV_LOGO_PIECE03_DX     (-720896)
#define VV_LOGO_PIECE04_DX     (-547693)
#define VV_LOGO_PIECE05_DX     (-514205)
#define VV_LOGO_PIECE06_DX     (-458752)
#define VV_LOGO_PIECE07_DX     (-425984)
#define VV_LOGO_PIECE08_DX     (-338602)
#define VV_LOGO_PIECE09_DX     (-274059)
#define VV_LOGO_PIECE10_DX     (-190650)
#define VV_LOGO_PIECE11_DX     (-517389)
#define VV_LOGO_PIECE12_DX     (-495160)
#define VV_LOGO_PIECE13_DX     (-470317)
#define VV_LOGO_PIECE14_DX     (-458752)
#define VV_LOGO_PIECE15_DX     (-458752)
#define VV_LOGO_PIECE16_DX     (-447829)
#define VV_LOGO_PIECE17_DX     (-432537)
#define VV_LOGO_PIECE18_DX     (-364088)
#define VV_LOGO_PIECE19_DX     (-1212416)

#define VV_LOGO_PIECE01_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE01_DX)
#define VV_LOGO_PIECE02_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE02_DX)
#define VV_LOGO_PIECE03_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE03_DX)
#define VV_LOGO_PIECE04_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE04_DX)
#define VV_LOGO_PIECE05_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE05_DX)
#define VV_LOGO_PIECE06_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE06_DX)
#define VV_LOGO_PIECE07_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE07_DX)
#define VV_LOGO_PIECE08_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE08_DX)
#define VV_LOGO_PIECE09_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE09_DX)
#define VV_LOGO_PIECE10_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE10_DX)
#define VV_LOGO_PIECE11_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE11_DX)
#define VV_LOGO_PIECE12_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE12_DX)
#define VV_LOGO_PIECE13_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE13_DX)
#define VV_LOGO_PIECE14_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE14_DX)
#define VV_LOGO_PIECE15_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE15_DX)
#define VV_LOGO_PIECE16_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE16_DX)
#define VV_LOGO_PIECE17_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE17_DX)
#define VV_LOGO_PIECE18_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE18_DX)
#define VV_LOGO_PIECE19_FLY_IN VV_LOGO_FLY_IN_FRAMES(VV_LOGO_PIECE19_DX)

/* The title logo's piece 0 comes up from below the art (its x is on the
 * art), piece 1 up and right from below its bottom-left corner: it
 * crosses both margins, and takes the longer. */
#define TITLE_LOGO_PIECE0_DX     (-494498)
#define TITLE_LOGO_PIECE0_DY     (-959208)
#define TITLE_LOGO_PIECE1_DX     1153761
#define TITLE_LOGO_PIECE1_DY     (-956825)

#define TITLE_LOGO_PIECE0_FLY_IN LOGO_FLY_IN_FRAMES(DISPLAY_ART_MARGIN_BOTTOM, TITLE_LOGO_PIECE0_DY)
#define TITLE_LOGO_PIECE1_FLY_IN                                                                                       \
    MAX(LOGO_FLY_IN_FRAMES(DISPLAY_ART_MARGIN_LEFT, TITLE_LOGO_PIECE1_DX),                                             \
        LOGO_FLY_IN_FRAMES(DISPLAY_ART_MARGIN_BOTTOM, TITLE_LOGO_PIECE1_DY))

#endif // GUARD_LOGO_FLY_IN_H
