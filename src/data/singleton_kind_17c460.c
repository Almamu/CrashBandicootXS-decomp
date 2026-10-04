#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"

/*
 * ROM 0x0817C460-0x0817C4C8. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* actor_part130.c's `struct singleton_kind` (0x28 bytes, fields not
 * named yet), written as ten words. SpawnHovercraft picks one by
 * gUnknown_030015D8. */
struct singleton_kind {
    s32 words[10];
};

const struct singleton_kind gStaticData_0817C460[2] = {
    { { 60, 45, 6, 210, 20, 5, 90, 32, 3, 160 } },
    { { 60, 70, 4, 230, 20, 3, 90, 40, 2, 160 } },
};

/* The camera-offset target box (struct anim_box) sub_8032C0C
 * (actor_part130.c) steers by. */
const struct anim_box gStaticData_0817C4B0 = { -102, -12, -2, 51, 68, 4 };

/* The one keyframe CreateHovercraft (actor_part130.c) gives the singleton. */
const struct anim_frame_record gStaticData_0817C4BC[1] = {
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
};
