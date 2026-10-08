#include "boss_actors.hpp"

/*
 * ROM 0x0817C4C8-0x0817C510. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The hovercraft's state functions, a plain function table called as
 * `gHovercraftStateFuncs[gHovercraftState]()` by RunHovercraftState
 * (hovercraft.cpp). C linkage: bosses.h declares it. */
void (*const gHovercraftStateFuncs[6])(void) = {
    HovercraftStateInactive,
    HovercraftStateApproach,
    HovercraftStateCloseIn,
    HovercraftStateFallBack,
    HovercraftStateExplodeStub,
    HovercraftStateFall,
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
