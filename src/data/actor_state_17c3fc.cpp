#include "game/vehicles/jetpack/airship.hpp"

using Game::Vehicles::Jetpack::Airship;

/*
 * ROM 0x0817C3FC-0x0817C414. The two pointer-to-member tables that
 * followed, the balloon's and the balloon crate's, are split into
 * actor_pmf_17c414.cpp and actor_pmf_17c42c.cpp (#664 part 11f). Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md.
 */

/* The airship's state functions (gAirshipStateFuncs), a plain function
 * table of Airship's static members, called through the pointer as
 * `stateFuncs[state]()` by Airship::Update (airship.cpp). */
void (*const Airship::stateFuncs[6])() = {
    &Airship::StateInactive,
    &Airship::StateApproach,
    &Airship::StateFireballs,
    &Airship::StateCannon,
    &Airship::StateExplode,
    &Airship::StateFall,
};
