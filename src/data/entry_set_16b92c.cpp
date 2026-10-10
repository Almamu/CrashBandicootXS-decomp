extern "C" {
#include "core.h"
#include "objects.h"
#include "player.h"
}

/*
 * ROM 0x0816B92C-0x0816B93C: two entry sets. Linked in ROM order between
 * data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* Their entries, in motion_records_16b304.cpp. */
extern const u32 gActionCtrlMotionEntries[][2];
extern const u32 gSwimCtrlMotionEntries[][2];

/* The sets PlayRoom (play_room.cpp) gives the two HUD widgets it
 * builds, through SetCtrlAnimSet (which stores them at +0x04). */
const struct entry_set gActionCtrlMotionSet = {
    gActionCtrlMotionEntries, 0x100,
};

const struct entry_set gSwimCtrlMotionSet = {
    gSwimCtrlMotionEntries, 0x100,
};
