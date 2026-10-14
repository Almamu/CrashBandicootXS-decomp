#include "graphics.hpp"
#include "engine/gfx/graphics_package.hpp"

extern "C" {
#include "core.h"
#include "engine/gfx/gfx.h"
}

using Engine::Gfx::MenuSkyBg;

/*
 * ROM 0x0816C484-0x0816C498. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* BG0 graphics package shared by LoadSaveMenuBg (save_menu_draw.cpp),
 * LoadLanguageSelectBg (language_select_setup.cpp), LevelSelect::Run
 * (level_select.cpp) and power_dialog.cpp. */
const struct bg_package gMenuSkyBg = {
    0x20,
    0x14,
    (void *)MenuSkyBg::palette,
    (void *)MenuSkyBg::tiles,
    (void *)MenuSkyBg::map,
};
