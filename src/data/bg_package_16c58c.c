#include "core.h"
#include "graphics_package.h"

/*
 * ROM 0x0816C58C-0x0816C5A0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gLevelSelectPageBgPalette[];
extern const u8 gLevelSelectPageBgTiles[];
extern const u8 gLevelSelectPageBgMap[];

/* BG graphics package loaded by DestroyLevelSelectPageBg (level_select_pages.c). */
const struct bg_package gLevelSelectPageBg = {
    0x20,
    0x20,
    (void *)gLevelSelectPageBgPalette,
    (void *)gLevelSelectPageBgTiles,
    (void *)gLevelSelectPageBgMap,
};
