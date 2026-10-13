#include "menus.hpp"

extern "C" {
#include "core.h"
#include "save.h"
}

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
 * InitPauseTimeTrialPage), pause_menu_pages_draw.cpp and pause_menu_collectibles.cpp. The
 * positions are on the pause menu's 240x160 art, from its origin
 * DISPLAY_ART_X/Y (screen.h, docs/screen.md). */
const struct vec2 gPauseCrystalIconPos = { DISPLAY_ART_X + 212, DISPLAY_ART_Y + 112 };
const struct vec2 gPausePowerIconPos[4] = {
    { DISPLAY_ART_X + 180, DISPLAY_ART_Y + 96 },
    { DISPLAY_ART_X + 212, DISPLAY_ART_Y + 96 },
    { DISPLAY_ART_X + 180, DISPLAY_ART_Y + 128 },
    { DISPLAY_ART_X + 212, DISPLAY_ART_Y + 128 },
};

const s32 gPausePowerIconFrames[4] = {
    3, 2, 0, 1,
};

const struct vec2 gPauseGemIconPos[5] = {
    { DISPLAY_ART_X + 200, DISPLAY_ART_Y + 90 },
    { DISPLAY_ART_X + 168, DISPLAY_ART_Y + 110 },
    { DISPLAY_ART_X + 184, DISPLAY_ART_Y + 110 },
    { DISPLAY_ART_X + 200, DISPLAY_ART_Y + 110 },
    { DISPLAY_ART_X + 216, DISPLAY_ART_Y + 110 },
};

const s32 gPauseGemIconFrames[5] = {
    1, 3, 2, 4, 0,
};

const struct vec2 gPauseRelicIconPos[3] = {
    { DISPLAY_ART_X + 172, DISPLAY_ART_Y + 100 },
    { DISPLAY_ART_X + 194, DISPLAY_ART_Y + 100 },
    { DISPLAY_ART_X + 216, DISPLAY_ART_Y + 100 },
};

const s32 gPauseRelicIconFrames[3] = {
    1, 2, 0,
};

const struct vec2 gPauseTimeTrialIconPos = { DISPLAY_ART_X + 196, DISPLAY_ART_Y + 120 };
