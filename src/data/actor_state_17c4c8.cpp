#include "boss_actors.hpp"
#include "hovercraft.hpp"

/*
 * ROM 0x0817C4C8-0x0817C510. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The hovercraft's state functions (gHovercraftStateFuncs), a plain
 * function table of Hovercraft's static members, called as
 * `stateFuncs[state]()` by Hovercraft::RunState (hovercraft.cpp). */
void (*const Hovercraft::stateFuncs[6])() = {
    StateInactive,
    StateApproach,
    StateCloseIn,
    StateFallBack,
    StateExplodeStub,
    StateFall,
};

/* The cannon's state methods, indexed by `state`
 * (gHovercraftCannonStateFuncs; HovercraftCannon::Update and RunState,
 * hovercraft_cannon.cpp, dispatch through it). Each non-virtual
 * `&HovercraftCannon::f` is g++'s {0, -1, f} record. */
const HovercraftCannon::StateFunc HovercraftCannon::stateFuncs[3] = {
    &HovercraftCannon::StateWait,
    &HovercraftCannon::StateFire,
    &HovercraftCannon::StateDestroyed,
};

/* The launcher's state methods (gHovercraftLauncherStateFuncs;
 * HovercraftLauncher::Update and RunState, hovercraft_launcher.cpp). */
const HovercraftLauncher::StateFunc HovercraftLauncher::stateFuncs[3] = {
    &HovercraftLauncher::StateWait,
    &HovercraftLauncher::StateLaunch,
    &HovercraftLauncher::StateDestroyed,
};
