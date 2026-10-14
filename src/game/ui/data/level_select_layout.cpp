#include "game/ui/menus.hpp"
#include "level_select_worlds.h"

extern "C" {
#include "core.h"
}

/*
 * ROM 0x0816C498-0x0816C58C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* `struct vec2` (aabb.h) positions and animation ids of the level-select
 * screens: level_select.cpp (InitLevelSelect, DrawLevelSelectTime, LoadLevelSelectRecord,
 * InitLaunchPad, now launch_pad.cpp) and level_select_pages.cpp (its level menu's item positions
 * and skins, RefreshLevelSelectPage). The positions are on the 240x160
 * menu art, from its origin DISPLAY_ART_X/Y (screen.h, docs/screen.md). */
const struct vec2 gLevelSelectWorldPos = { DISPLAY_ART_X + 16, DISPLAY_ART_Y + 32 };
const struct vec2 gLevelSelectCrashIconPos = { DISPLAY_ART_X + 16, DISPLAY_ART_Y + 60 };
const struct vec2 gLevelSelectCrystalPos = { DISPLAY_ART_X + 40, DISPLAY_ART_Y + 33 };
const struct vec2 gLevelSelectGemPos = { DISPLAY_ART_X + 50, DISPLAY_ART_Y + 32 };
const struct vec2 gLevelSelectTrialIconPos = { DISPLAY_ART_X + 66, DISPLAY_ART_Y + 34 };
const struct vec2 gLevelSelectTimePos = { DISPLAY_ART_X + 166, DISPLAY_ART_Y + 34 };
const struct vec2 gLevelSelectNextWorldArrowPos = { DISPLAY_ART_X + 201, DISPLAY_ART_Y + 70 };
const struct vec2 gLevelSelectPrevWorldArrowPos = { DISPLAY_ART_X + 201, DISPLAY_ART_Y + 87 };

const struct vec2 gLevelSelectEntryPositions[LEVEL_SELECT_ENTRIES] = {
    { DISPLAY_ART_X + 30, DISPLAY_ART_Y + 120 },
    { DISPLAY_ART_X + 70, DISPLAY_ART_Y + 132 },
    { DISPLAY_ART_X + 120, DISPLAY_ART_Y + 136 },
    { DISPLAY_ART_X + 170, DISPLAY_ART_Y + 132 },
    { DISPLAY_ART_X + 210, DISPLAY_ART_Y + 120 },
    { 0, 0 }, /* the sixth entry, hidden until the five levels are cleared */
};
const struct vec2 gLevelSelectEntryPositionsAllCleared[LEVEL_SELECT_ENTRIES] = {
    { DISPLAY_ART_X + 26, DISPLAY_ART_Y + 118 },
    { DISPLAY_ART_X + 58, DISPLAY_ART_Y + 130 },
    { DISPLAY_ART_X + 99, DISPLAY_ART_Y + 136 },
    { DISPLAY_ART_X + 141, DISPLAY_ART_Y + 136 },
    { DISPLAY_ART_X + 182, DISPLAY_ART_Y + 130 },
    { DISPLAY_ART_X + 214, DISPLAY_ART_Y + 118 },
};
/* Per world (a page): the entries' box and the page title's animation,
 * from levels.json's "worlds" (level_select_worlds.h, docs/levels.md
 * "Level-select pages"). */
const u32 gLevelSelectWorldEntryBoxAnims[WORLD_COUNT] = { LEVEL_SELECT_WORLD_ENTRY_BOX_ANIMS };
const u32 gLevelSelectWorldAnims[WORLD_COUNT] = { LEVEL_SELECT_WORLD_ANIMS };
const u32 gLevelSelectRankAnims[5] = {
    1, 3, 2, 4, 0,
};

/* 16 halfwords InitLaunchPad copies into its palette cache (like the other
 * mostly-0xFFFF slot-2 halves). */
const u16 gLevelSelectPalette[16] = {
    0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* A partial page's tables (level_select_worlds.h, docs/levels.md "Partial
 * pages"): each world's first level, level count and boss, and the layouts
 * for fewer entries. Nothing with the ROM's pages, which are all full. */
LEVEL_SELECT_PAGE_TABLES
