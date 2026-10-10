extern "C" {
#include "core.h"
#include "menus.h"
}

/*
 * ROM 0x0816C498-0x0816C58C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* `struct vec2` (menus.h) positions and animation ids of the level-select
 * screens: level_select.cpp (InitLevelSelect, DrawLevelSelectTime, LoadLevelSelectRecord,
 * InitLaunchPad, now launch_pad.cpp) and level_select_pages.cpp (its level menu's item positions
 * and skins, RefreshLevelSelectPage). */
const struct vec2 gLevelSelectWorldPos = { 16, 32 };
const struct vec2 gLevelSelectCrashIconPos = { 16, 60 };
const struct vec2 gLevelSelectCrystalPos = { 40, 33 };
const struct vec2 gLevelSelectGemPos = { 50, 32 };
const struct vec2 gLevelSelectTrialIconPos = { 66, 34 };
const struct vec2 gLevelSelectTimePos = { 166, 34 };
const struct vec2 gLevelSelectNextWorldArrowPos = { 201, 70 };
const struct vec2 gLevelSelectPrevWorldArrowPos = { 201, 87 };

const struct vec2 gLevelSelectEntryPositions[6] = {
    { 30, 120 }, { 70, 132 }, { 120, 136 }, { 170, 132 }, { 210, 120 }, { 0, 0 },
};
const struct vec2 gLevelSelectEntryPositionsAllCleared[6] = {
    { 26, 118 }, { 58, 130 }, { 99, 136 }, { 141, 136 }, { 182, 130 }, { 214, 118 },
};
const u32 gLevelSelectWorldEntryBoxAnims[4] = {
    0,
    1,
    2,
    3,
};
const u32 gLevelSelectWorldAnims[4] = {
    9,
    8,
    6,
    7,
};
const u32 gLevelSelectRankAnims[5] = {
    1, 3, 2, 4, 0,
};

/* 16 halfwords InitLaunchPad copies into its palette cache (like the other
 * mostly-0xFFFF slot-2 halves). */
const u16 gLevelSelectPalette[16] = {
    0x0000, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};
