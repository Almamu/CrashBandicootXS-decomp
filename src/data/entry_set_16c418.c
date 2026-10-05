#include "core.h"

/*
 * ROM 0x0816C418-0x0816C460: an entry table and the entry set that
 * points into it. Linked in ROM order between data/data.s sections by
 * ldscript.txt - see docs/data.md.
 */

/* An entry set as gobj_1a794.h's `struct mover` points at it (`set`,
 * +0x04): the {a, b} entries (`struct vec_pair`) and a word the code
 * doesn't read, 0x100 in every set in the ROM. */
struct entry_set
{
    const u32 (*entries)[2];
    u32 unk_04;
};

/* {a, b} vec3-table index pairs (`struct vec_pair`, gobj_1a794.h):
 * actor_part_1a794.c indexes gDingodileMotionRecords with entries 0-3;
 * entries 4-7 are gPlatformMoverMotionSet's, indexing gPlatformMoverMotionRecords
 * (actor_part_1b208.c). */
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

/* The entry set CreatePlatformMover (actor_part_1b208.c) stores in its
 * object's `set`. */
const struct entry_set gPlatformMoverMotionSet = {
    &gDingodileMotionEntries[4], 0x100,
};
