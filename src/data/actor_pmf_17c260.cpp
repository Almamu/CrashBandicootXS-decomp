#include "vehicle.hpp"

/*
 * ROM 0x0817C260-0x0817C2B8: two pointer-to-member tables (#664 part
 * 11f); the third, the airship fireball's, follows in
 * actor_pmf_17c2b8.cpp. Linked in ROM order between data/data.s sections
 * by ldscript.txt - see docs/data.md.
 */

/* The plane's state methods, indexed by `state` (gJetpackPlaneStateFuncs;
 * JetpackPlane::Update and RunState, jetpack_plane.cpp, dispatch through
 * it). Each non-virtual `&JetpackPlane::f` is g++'s {0, -1, f} record. */
const JetpackPlane::StateFunc JetpackPlane::stateFuncs[4] = {
    &JetpackPlane::StateFly,
    &JetpackPlane::StateFollow,
    &JetpackPlane::StateKnockedOut,
    &JetpackPlane::StateFall,
};

/* The bomber's (gJetpackBomberStateFuncs; JetpackBomber::Update and
 * RunState, jetpack_plane.cpp). */
const JetpackBomber::StateFunc JetpackBomber::stateFuncs[7] = {
    &JetpackBomber::StateIdle,
    &JetpackBomber::StateHome,
    &JetpackBomber::StateBobVertical,
    &JetpackBomber::StateSwingHorizontal,
    &JetpackBomber::StateCircle,
    &JetpackBomber::StateDrop,
    &JetpackBomber::StateDying,
};
