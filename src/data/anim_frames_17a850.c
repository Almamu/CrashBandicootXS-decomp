#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817A850-0x0817A880. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The keyframes of the sub_802DFDC singleton (actor_part60.c, the yeti
 * of compressed frame set B), with frame_table_17a880.c's
 * gStaticData_0817A880 as their frame table. */
const struct anim_frame_record gStaticData_0817A850[4] = {
    { 128, 0, 30, 0, 0x0, { 0, 0 } },
    { 128, 30, 31, 0, 0x0, { 0, 0 } },
    { 128, 61, 62, 46, 0x0, { 0, 0 } },
    { 128, 61, 62, 46, 0x0, { 0, 0 } },
};
