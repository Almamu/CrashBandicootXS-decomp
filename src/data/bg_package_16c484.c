#include "core.h"
#include "graphics_package.h"
#include "gfx.h"

/*
 * ROM 0x0816C484-0x0816C498. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gMenuSkyBgPalette[];
extern const u8 gMenuSkyBgTiles[];
extern const u8 gMenuSkyBgMap[];

/* BG0 graphics package shared by LoadSaveMenuBg (save_menu_ui.c),
 * LoadLanguageSelectBg (language_select_setup.c), RunLevelSelect
 * (level_select.c) and power_dialog.c. */
const struct bg_package gMenuSkyBg = {
    0x20,
    0x14,
    (void *)gMenuSkyBgPalette,
    (void *)gMenuSkyBgTiles,
    (void *)gMenuSkyBgMap,
};
