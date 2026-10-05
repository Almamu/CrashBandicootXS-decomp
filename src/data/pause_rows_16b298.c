#include "core.h"

/*
 * ROM 0x0816B298-0x0816B2E0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* pause_menu_draw.c's `struct pause_screen_row_record`. */
struct pause_screen_row_record {
    s32 labelId;
    s32 typeTag;
};

/* The rows of the pause/options screen: InitPauseMenu (pause_menu.c)
 * stores the table in its object's field_14. */
const struct pause_screen_row_record gPauseMenuRows[5] = {
    { 0x32, 0x0 },
    { 0x30, 0x4 },
    { 0x31, 0x5 },
    { 0x33, 0x2 },
    { 0x34, 0x1 },
};

/* 16 halfwords RunPauseMenu (pause_menu.c) copies into palette-cache
 * slot 0x83 (mostly 0xFFFF, like the other slot-2 halves). */
const u16 gPauseMenuPalette[16] = {
    0x0000, 0x9CC6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};
