#include "game/bosses/boss_actors.hpp"

using Game::Vehicles::Jetpack::AirshipFireball;

/*
 * ROM 0x0817C2B8-0x0817C2D0, split from jetpack_plane_bomber_state_tables.cpp (#664 part
 * 11i). Linked in ROM order between games/xs/data/data.s sections by games/xs/ldscript.txt
 * - see docs/data.md.
 */

/* The airship fireball's state methods, indexed by `state`
 * (gAirshipFireballStateFuncs; AirshipFireball::Update and RunState,
 * airship_fireball.cpp, dispatch through it). Each non-virtual
 * `&AirshipFireball::f` is g++'s {0, -1, f} record. */
const AirshipFireball::StateFunc AirshipFireball::stateFuncs[3] = {
    &AirshipFireball::StateOrbit,
    &AirshipFireball::StateSpiralIn,
    &AirshipFireball::StateExplode,
};
