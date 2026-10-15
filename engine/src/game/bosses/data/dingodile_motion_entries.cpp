extern "C" {
#include "core.h"
#include "game/entities/objects.h"
#include "game/bosses/bosses.h"
}

/*
 * ROM 0x0816C418-0x0816C460: an entry table and the entry set that
 * points into it. Linked in ROM order between games/xs/data/data.s sections by
 * games/xs/ldscript.txt - see docs/data.md.
 */

/* {a, b} motion-record index pairs:
 * dingodile_create.cpp indexes gDingodileMotionRecords with entries 0-3;
 * entries 4-7 are gPlatformMoverMotionSet's, indexing gPlatformMoverMotionRecords
 * (platform_mover.cpp). */
const u32 gDingodileMotionEntries[8][2] = {
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 7, 6 },
    { 0, 0 },
    { 1, 0 },
    { 0, 1 },
    { 0, 2 },
};

/* The entry set CreatePlatformMover (platform_mover.cpp) stores in its
 * object's `set`. */
const struct entry_set gPlatformMoverMotionSet = {
    &gDingodileMotionEntries[4], 0x100,
};
