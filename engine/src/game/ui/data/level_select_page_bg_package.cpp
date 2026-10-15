#include "graphics.hpp"
#include "engine/gfx/graphics_package.hpp"
#include "game/ui/menus.hpp"

extern "C" {
#include "core.h"
}

using Engine::Gfx::LevelSelectPlatformBg;

/*
 * ROM 0x0816C58C-0x0816C5A0. Linked in ROM order between games/xs/data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md.
 */

/* BG graphics package loaded by DestroyLevelSelectPageBg (level_select_page_bg.cpp). */
const struct bg_package gLevelSelectPageBg = {
    0x20,
    0x20,
    (void *)LevelSelectPlatformBg::palette,
    (void *)LevelSelectPlatformBg::tiles,
    (void *)LevelSelectPlatformBg::map,
};
