#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0816B284-0x0816B298. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gPauseMenuBgPalette[];
extern const u8 gPauseMenuBgTiles[];
extern const u8 gPauseMenuBgMap[];

/* BG graphics package loaded by RunPauseMenu (pause_menu.c). */
const struct bg_package gPauseMenuBg = {
    0x1e,
    0x14,
    (void *)gPauseMenuBgPalette,
    (void *)gPauseMenuBgTiles,
    (void *)gPauseMenuBgMap,
};
