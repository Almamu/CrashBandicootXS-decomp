#include "graphics.hpp"
#include "engine/gfx/graphics_package.hpp"
#include "game/ui/menus.hpp"

extern "C" {
#include "core.h"
}

using Engine::Gfx::PauseMenuBg;

/*
 * ROM 0x0816B284-0x0816B298. Linked in ROM order between data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md.
 */

/* BG graphics package loaded by RunPauseMenu (pause_menu.cpp). */
const struct bg_package gPauseMenuBg = {
    0x1e,
    0x14,
    (void *)PauseMenuBg::palette,
    (void *)PauseMenuBg::tiles,
    (void *)PauseMenuBg::map,
};
