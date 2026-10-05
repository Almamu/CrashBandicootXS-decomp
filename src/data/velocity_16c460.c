#include "core.h"

/*
 * ROM 0x0816C460-0x0816C484. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The 12-byte {x, y, z} velocity records (gobj_1a794.h's `struct vec3`)
 * of platform.c, indexed through gPlatformMoverMotionSet's entries
 * (entry_set_16c418.c). */
const s32 gPlatformMoverMotionRecords[3][3] = {
    { 0, 0, 0 },
    { 0, 8, 256 },
    { 0, 40, 1024 },
};
