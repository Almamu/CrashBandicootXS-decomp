#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0816C484-0x0816C498. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gMenuSkyBgPalette[];
extern const u8 gMenuSkyBgTiles[];
extern const u8 gMenuSkyBgMap[];

/* BG0 graphics package shared by LoadSaveMenuBg (settings_menu2.c),
 * LoadLanguageSelectBg (counter_selector_setup.c), RunLevelSelect
 * (actor_part_1b85c.c) and settings_menu13.c. */
const struct bg_package gMenuSkyBg = {
    0x20,
    0x14,
    (void *)gMenuSkyBgPalette,
    (void *)gMenuSkyBgTiles,
    (void *)gMenuSkyBgMap,
};
