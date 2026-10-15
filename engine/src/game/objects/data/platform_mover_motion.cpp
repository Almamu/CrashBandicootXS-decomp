extern "C" {
#include "core.h"
#include "game/entities/objects.h"
#include "engine/util/math_util.h"
}

/*
 * ROM 0x0816C460-0x0816C484. Linked in ROM order between games/xs/data/data.s
 * sections by games/xs/ldscript.txt - see docs/data.md.
 */

/* The motion records (`struct speed_ramp`, objects.h) of platform.c, indexed through gPlatformMoverMotionSet's entries
 * (dingodile_motion_entries.cpp). */
const struct speed_ramp gPlatformMoverMotionRecords[3] = {
    { 0, 0, 0 },
    { 0, PLATFORM_SLOW_MOVE_ACCEL, INT_TO_Q8(1) },
    { 0, PLATFORM_FALL_ACCEL, INT_TO_Q8(4) },
};
