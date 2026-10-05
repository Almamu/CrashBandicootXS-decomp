#include "core.h"
#include "objects.h"
#include "bosses.h"

/*
 * ROM 0x0816C418-0x0816C460: an entry table and the entry set that
 * points into it. Linked in ROM order between data/data.s sections by
 * ldscript.txt - see docs/data.md.
 */

/* {a, b} vec3-table index pairs (`struct vec_pair`, gobj_1a794.h):
 * dingodile_create.c indexes gDingodileMotionRecords with entries 0-3;
 * entries 4-7 are gPlatformMoverMotionSet's, indexing gPlatformMoverMotionRecords
 * (platform.c). */
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

/* The entry set CreatePlatformMover (platform.c) stores in its
 * object's `set`. */
const struct entry_set gPlatformMoverMotionSet = {
    &gDingodileMotionEntries[4], 0x100,
};
