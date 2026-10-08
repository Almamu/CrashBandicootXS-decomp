#include "boss_actors.hpp"

/*
 * ROM 0x0817C450-0x0817C460. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The hovercraft fireball's state methods, indexed by `state`
 * (gHovercraftFireballStateFuncs; HovercraftFireball::Update and RunState,
 * hovercraft.cpp, dispatch through it). Each non-virtual
 * `&HovercraftFireball::f` is g++'s {0, -1, f} record. */
const HovercraftFireball::StateFunc HovercraftFireball::stateFuncs[2] = {
    &HovercraftFireball::StateFly,
    &HovercraftFireball::StateExplode,
};
