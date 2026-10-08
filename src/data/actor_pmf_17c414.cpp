#include "vehicle.hpp"

/*
 * ROM 0x0817C414-0x0817C42C, split from actor_state_17c3fc.c (#664 part
 * 11f). Linked in ROM order between data/data.s sections by ldscript.txt
 * - see docs/data.md.
 */

/* The balloon's state methods, indexed by `state`
 * (gJetpackBalloonStateFuncs; JetpackBalloon::RunState,
 * jetpack_balloon.cpp, dispatches through it). Each non-virtual
 * `&JetpackBalloon::f` is g++'s {0, -1, f} record. */
const JetpackBalloon::StateFunc JetpackBalloon::stateFuncs[3] = {
    &JetpackBalloon::StateAttached,
    &JetpackBalloon::StateFloatAway,
    &JetpackBalloon::StatePop,
};
