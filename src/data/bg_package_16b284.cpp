#include "graphics.hpp"
#include "graphics_package.hpp"
#include "menus.hpp"

extern "C" {
#include "core.h"
}

/*
 * ROM 0x0816B284-0x0816B298. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* BG graphics package loaded by RunPauseMenu (pause_menu.cpp). */
const struct bg_package gPauseMenuBg = {
    0x1e,
    0x14,
    (void *)PauseMenuBg::palette,
    (void *)PauseMenuBg::tiles,
    (void *)PauseMenuBg::map,
};
