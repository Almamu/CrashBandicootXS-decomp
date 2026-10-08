#include "core.h"
#include "actor_self.h"
#include "vehicle.h"

/*
 * ROM 0x0817A850-0x0817A880. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The keyframes of the CreateYeti singleton (yeti.cpp, the yeti
 * of compressed frame set B), with frame_table_17a880.c's
 * gYetiFrames as their frame table. */
const struct anim_frame_record gYetiKeyframes[4] = {
    { 128, 0, 30, 0, 0x0, { 0, 0 } },
    { 128, 30, 31, 0, 0x0, { 0, 0 } },
    { 128, 61, 62, 46, 0x0, { 0, 0 } },
    { 128, 61, 62, 46, 0x0, { 0, 0 } },
};
