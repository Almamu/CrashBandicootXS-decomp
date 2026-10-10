extern "C" {
#include "core.h"
#include "objects.h"
#include "player.h"
}

/*
 * ROM 0x0816B93C-0x0816B98C. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u32 gInputCtrlMotionEntries[9][2];

const struct entry_set gInputCtrlMotionSet = {
    gInputCtrlMotionEntries, 0x100,
};

const u32 gInputCtrlMotionEntries[9][2] = {
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 1, 5 },
    { 2, 6 },
    { 1, 3 },
    { 2, 4 },
    { 7, 0 },
    { 8, 0 },
};
