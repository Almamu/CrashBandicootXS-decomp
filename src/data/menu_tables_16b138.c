#include "core.h"
#include "save.h"
#include "menus.h"

/*
 * ROM 0x0816B138-0x0816B284. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The text DrawYesNoPrompt (save_menu_draw.cpp) draws in an icon's text slot. */
const u8 gMenuCursorText[] = ">";

/* InitSaveMenuIcons (save_menu_draw.cpp) fills palette-cache slot 0 from the
 * first two arrays and slot 2 from the last two, 16 halfwords of each
 * (the same four as continue_prompt.cpp's gContinuePromptPalette0 ... 0817C572). */
const u16 gSaveMenuPalette0[16] = {
    0x83E0, 0x9CC6, 0x107F, 0x0D04, 0x0F9F, 0x894C, 0x0864, 0x05D4,
    0x0ABE, 0xA27F, 0x09BE, 0x1D5F, 0x886B, 0x94DF, 0x0C9B, 0x0873,
};

const u16 gSaveMenuPalette1[16] = {
    0x03E0, 0x1CC6, 0x359E, 0x0D04, 0x4FDE, 0x094C, 0x0864, 0x05D4,
    0x3AFE, 0x36BE, 0x223E, 0x35FE, 0x086B, 0x35DE, 0x35DB, 0x0873,
};

const u16 gSaveMenuPalette2[16] = {
    0x0000, 0x9CC6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

const u16 gSaveMenuPalette3[16] = {
    0x0000, 0x001F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/* The text ids of the five labels DrawSaveMenuMain (save_menu_input.cpp) draws. */
const s32 gSaveMenuOptions[5] = {
    0x1B, 0x1C, 0x1E, 0x1D, 0x20,
};

/* The label text ids DrawPauseMenuPageTitle (pause_menu_pages_draw.cpp) picks from. */
const s32 gPauseMenuPageTitles[5] = {
    0x36, 0x35, 0x37, 0x38, 0x39,
};

/* Icon positions and frame indices of the menu screens in
 * pause_menu_pages_init.cpp (InitPauseCrystalsPage, InitPausePowersPage, InitPauseGemsPage, InitPauseRelicsPage,
 * InitPauseTimeTrialPage), pause_menu_pages_draw.cpp and pause_menu_gems.cpp. */
const struct icon_pos gPauseCrystalIconPos = { 212, 112 };
const struct icon_pos gPausePowerIconPos[4] = {
    { 180, 96 },
    { 212, 96 },
    { 180, 128 },
    { 212, 128 },
};

const s32 gPausePowerIconFrames[4] = {
    3, 2, 0, 1,
};

const struct icon_pos gPauseGemIconPos[5] = {
    { 200, 90 },
    { 168, 110 },
    { 184, 110 },
    { 200, 110 },
    { 216, 110 },
};

const s32 gPauseGemIconFrames[5] = {
    1, 3, 2, 4, 0,
};

const struct icon_pos gPauseRelicIconPos[3] = {
    { 172, 100 },
    { 194, 100 },
    { 216, 100 },
};

const s32 gPauseRelicIconFrames[3] = {
    1, 2, 0,
};

const struct icon_pos gPauseTimeTrialIconPos = { 196, 120 };
