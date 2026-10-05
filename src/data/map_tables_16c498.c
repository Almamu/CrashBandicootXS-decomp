#include "core.h"

/*
 * ROM 0x0816C498-0x0816C58C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* `struct xy_pair` positions and animation ids of the level-select
 * screens: level_select.c (InitLevelSelect, DrawLevelSelectTime, LoadLevelSelectRecord,
 * InitLaunchPad) and level_select_pages.c (its level menu's item positions
 * and skins, RefreshLevelSelectPage). */
struct xy_pair {
    s32 x;
    s32 y;
};

const struct xy_pair gLevelSelectWorldPos = { 16, 32 };
const struct xy_pair gStaticData_0816C4A0 = { 16, 60 };
const struct xy_pair gLevelSelectCrystalPos = { 40, 33 };
const struct xy_pair gLevelSelectGemPos = { 50, 32 };
const struct xy_pair gLevelSelectTrialIconPos = { 66, 34 };
const struct xy_pair gLevelSelectTimePos = { 166, 34 };
const struct xy_pair gStaticData_0816C4C8 = { 201, 70 };
const struct xy_pair gStaticData_0816C4D0 = { 201, 87 };

const struct xy_pair gLevelSelectEntryPositions[6] = {
    { 30, 120 },
    { 70, 132 },
    { 120, 136 },
    { 170, 132 },
    { 210, 120 },
    { 0, 0 },
};
const struct xy_pair gLevelSelectEntryPositionsAllCleared[6] = {
    { 26, 118 },
    { 58, 130 },
    { 99, 136 },
    { 141, 136 },
    { 182, 130 },
    { 214, 118 },
};
const u32 gLevelSelectWorldEntryBoxAnims[4] = {
    0, 1, 2, 3,
};
const u32 gLevelSelectWorldAnims[4] = {
    9, 8, 6, 7,
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
