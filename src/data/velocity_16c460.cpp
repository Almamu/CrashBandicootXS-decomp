extern "C" {
#include "core.h"
#include "objects.h"
#include "math_util.h"
}

/*
 * ROM 0x0816C460-0x0816C484. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The motion records (`struct speed_ramp`, objects.h) of platform.c, indexed through gPlatformMoverMotionSet's entries
 * (entry_set_16c418.cpp). */
const struct speed_ramp gPlatformMoverMotionRecords[3] = {
    { 0, 0, 0 },
    { 0, PLATFORM_SLOW_MOVE_ACCEL, INT_TO_Q8(1) },
    { 0, PLATFORM_FALL_ACCEL, INT_TO_Q8(4) },
};
