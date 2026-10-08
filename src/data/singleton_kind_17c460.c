#include "core.h"
#include "actor_self.h"
#include "actor_anim.h"
#include "bosses.h"

/*
 * ROM 0x0817C460-0x0817C4C8. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The hovercraft's attack parameters (struct hovercraft_attack, bosses.h).
 * SpawnHovercraft picks one by gHovercraftLevel. */
const struct hovercraft_attack gHovercraftAttacks[2] = {
    { 60, { { 45, 6, 210 }, { 20, 5, 90 }, { 32, 3, 160 } } },
    { 60, { { 70, 4, 230 }, { 20, 3, 90 }, { 40, 2, 160 } } },
};

/* The camera-offset target box (struct anim_box) HovercraftStateCloseIn
 * (hovercraft.cpp) steers by. */
const struct anim_box gHovercraftBox = { -102, -12, -2, 51, 68, 4 };

/* The one keyframe CreateHovercraft (hovercraft.cpp) gives the singleton. */
const struct anim_frame_record gHovercraftKeyframes[1] = {
    { 64, 0, 1, 0, 0x0, { 0, 0 } },
};
