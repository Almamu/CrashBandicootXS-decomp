#include "menus.hpp"

extern "C" {
#include "core.h"
}

/*
 * ROM 0x0816B298-0x0816B2E0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The rows of the pause/options screen: InitPauseMenu (pause_menu.cpp)
 * stores the table in its object's field_14. */
const struct pause_row gPauseMenuRows[5] = {
    { 0x32, PAUSE_ROW_TYPE_RESUME },
    { 0x30, PAUSE_ROW_TYPE_MUSIC },
    { 0x31, PAUSE_ROW_TYPE_SOUND },
    { 0x33, PAUSE_ROW_TYPE_QUIT },
    { 0x34, PAUSE_ROW_TYPE_RESTART },
};

/* 16 halfwords RunPauseMenu (pause_menu.cpp) copies into palette-cache
 * slot 0x83 (mostly 0xFFFF, like the other slot-2 halves). */
const u16 gPauseMenuPalette[16] = {
    0x0000, 0x9CC6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};
