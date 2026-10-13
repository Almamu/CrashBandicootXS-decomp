#ifndef GUARD_SCREEN_H
#define GUARD_SCREEN_H

#include "gba/types.h"
#include "gba/defines.h"

/* The screen size and everything the game derives from it (#979,
 * docs/screen.md). gba/defines.h has DISPLAY_WIDTH x DISPLAY_HEIGHT
 * (240x160 on the GBA); a value that comes from the screen is spelled
 * with these, so a port with another screen size changes the game
 * consistently by changing the defines.
 *
 * Every macro is a constant expression, so a converted site compiles to
 * the same code as the literal it replaces, as long as the site keeps
 * its operation order: `x <= DISPLAY_WIDTH - 1` is the ROM's `x <= 239`,
 * `(DISPLAY_WIDTH - w) >> 1` its `(240 - w) >> 1`. */

/* Half the screen: its centre (120, 80 on the GBA). */
#define DISPLAY_CENTER_X (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_Y (DISPLAY_HEIGHT / 2)

/* The screen in 8x8 tiles (30x20 on the GBA). */
#define DISPLAY_WIDTH_TILES  (DISPLAY_WIDTH / 8)
#define DISPLAY_HEIGHT_TILES (DISPLAY_HEIGHT / 8)

/* The x (y) at which something `w` (`h`) pixels wide (high) touches the
 * right (bottom) edge: the right- and bottom-anchored layouts. */
#define DISPLAY_RIGHT(w)  (DISPLAY_WIDTH - (w))
#define DISPLAY_BOTTOM(h) (DISPLAY_HEIGHT - (h))

/* Their left and top counterparts: `x` (`y`) pixels from the left (top)
 * edge. They are the number itself; a layout anchored to the screen's
 * corners (the HUD) spells them, so that every coordinate names its
 * anchor and none is a bare position. */
#define DISPLAY_LEFT(x) (x)
#define DISPLAY_TOP(y)  (y)

/* The camera's scroll limits: a room (or a BG) `w` x `h` pixels can
 * scroll to `CAMERA_MAX_X(w)`, `CAMERA_MAX_Y(h)` before its right and
 * bottom edges come into view. */
#define CAMERA_MAX_X(w) ((w) - DISPLAY_WIDTH)
#define CAMERA_MAX_Y(h) ((h) - DISPLAY_HEIGHT)

/* The activation window: crates, parts and entities update while they
 * are inside the screen plus ACTIVE_AREA_MARGIN_X pixels left and right
 * of it and ACTIVE_AREA_MARGIN_Y above and below (440x280 on the GBA,
 * from the camera's scroll position minus the margins). The window
 * scales with the screen; the margins are tuning and stay. */
#define ACTIVE_AREA_MARGIN_X 100
#define ACTIVE_AREA_MARGIN_Y 60
#define ACTIVE_AREA_WIDTH    (DISPLAY_WIDTH + 2 * ACTIVE_AREA_MARGIN_X)
#define ACTIVE_AREA_HEIGHT   (DISPLAY_HEIGHT + 2 * ACTIVE_AREA_MARGIN_Y)

/* Fixed-size art: the cutscene pictures, the title screen, the menu
 * pictures and the other full-screen images are drawn for a 240x160
 * screen whatever DISPLAY_WIDTH and DISPLAY_HEIGHT are. DISPLAY_ART_X/Y
 * is where such a picture's top-left corner goes on the screen: centred
 * (letterboxed) on a bigger screen, 0 on the GBA. */
#define DISPLAY_ART_WIDTH  240
#define DISPLAY_ART_HEIGHT 160
#define DISPLAY_ART_X      ((DISPLAY_WIDTH - DISPLAY_ART_WIDTH) / 2)
#define DISPLAY_ART_Y      ((DISPLAY_HEIGHT - DISPLAY_ART_HEIGHT) / 2)

/* The centre of that art, in the art's own coordinates (120, 80): the
 * point an affine full-screen picture (a zoom) is scaled around. */
#define DISPLAY_ART_CENTER_X (DISPLAY_ART_WIDTH / 2)
#define DISPLAY_ART_CENTER_Y (DISPLAY_ART_HEIGHT / 2)

/* Screen-coordinate types: what a value that holds a screen position, a
 * screen-space offset or a size derived from the screen is stored in.
 * Each is the type the ROM uses for it, so the matching build is
 * unchanged; a port with a bigger screen can widen them here without
 * touching the game code.
 *
 * - screen_x_t / screen_y_t: a pixel position on the screen, signed
 *   (it can be off screen, left of or above it).
 * - screen_ux_t / screen_uy_t: the same, where the ROM keeps it unsigned
 *   (the font cursor, Font::posX/posY, and the code that reads it).
 * - screen_dx_t / screen_dy_t: a signed offset or delta in pixels (a
 *   sprite's half size from its anchor, a centring offset).
 * - screen_w_t / screen_h_t: a width or height in pixels, measured on
 *   the screen or derived from its size (MeasureText's result).
 * - screen_x_q8_t / screen_y_q8_t, screen_dx_q8_t / screen_dy_q8_t: the
 *   Q8 (24.8 fixed point, #916) forms of a position and of an offset.
 *
 * Where the ROM keeps a screen value narrower (a u8 or s16 field), the
 * field keeps its type and says so in a comment: it can't hold every
 * position of a bigger screen. */
typedef s32 screen_x_t;
typedef s32 screen_y_t;
typedef u32 screen_ux_t;
typedef u32 screen_uy_t;
typedef s32 screen_dx_t;
typedef s32 screen_dy_t;
typedef s32 screen_w_t;
typedef s32 screen_h_t;
typedef s32 screen_x_q8_t;
typedef s32 screen_y_q8_t;
typedef s32 screen_dx_q8_t;
typedef s32 screen_dy_q8_t;

#endif // GUARD_SCREEN_H
