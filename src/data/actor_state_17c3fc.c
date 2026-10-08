#include "core.h"
#include "actor_self.h"
#include "bosses.h"

/*
 * ROM 0x0817C3FC-0x0817C414. The two pointer-to-member tables that
 * followed, the balloon's and the balloon crate's, are split into
 * actor_pmf_17c414.cpp and actor_pmf_17c42c.cpp (#664 part 11f). Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md.
 */

/* Per-state step functions of the weapon-kind tracker, called through
 * _call_via_r0 as `gAirshipStateFuncs[gAirshipState]` by
 * UpdateAirship (airship.cpp). */
void (*const gAirshipStateFuncs[6])(void) = {
    AirshipStateInactive,
    AirshipStateApproach,
    AirshipStateFireballs,
    AirshipStateCannon,
    AirshipStateExplode,
    AirshipStateFall,
};
