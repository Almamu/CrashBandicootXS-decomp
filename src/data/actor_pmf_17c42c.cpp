#include "vehicle.hpp"

/*
 * ROM 0x0817C42C-0x0817C444, split from actor_state_17c3fc.c (#664 part
 * 11f). Linked in ROM order between data/data.s sections by ldscript.txt
 * - see docs/data.md.
 */

/* The balloon crate's state methods, indexed by `state`
 * (gJetpackBalloonCrateStateFuncs; JetpackBalloonCrate::Update and
 * RunState, jetpack_crates.cpp, dispatch through it). Each non-virtual
 * `&JetpackBalloonCrate::f` is g++'s {0, -1, f} record. */
const JetpackBalloonCrate::StateFunc JetpackBalloonCrate::stateFuncs[3] = {
    &JetpackBalloonCrate::StateHang,
    &JetpackBalloonCrate::StateFall,
    &JetpackBalloonCrate::StateDestroyed,
};
