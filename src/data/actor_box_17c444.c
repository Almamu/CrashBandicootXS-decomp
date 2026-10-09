#include "core.h"
#include "actor_anim.h"
#include "vehicle.h"

/*
 * ROM 0x0817C444-0x0817C450. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The box UpdateJetpackRocket (jetpack_rocket.cpp) copies into a part's +0x38
 * box. */
const struct anim_box gJetpackRocketBox = { -21, -21, -2, 42, 42, 4 };
