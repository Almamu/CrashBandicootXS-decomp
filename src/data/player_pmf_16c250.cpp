#include "swim_ctrl.hpp"
#include "input_ctrl.hpp"

extern "C" {
#include "bosses.h"
}

/*
 * ROM 0x0816C250-0x0816C2D8: two per-state member-function-pointer
 * tables of the player-controller objects, then an entry set. Linked in
 * ROM order between data/data.s sections by ldscript.txt - see
 * docs/data.md.
 */

/* The swim controller's state methods, indexed by `state`
 * (gSwimCtrlStateFuncs; SwimCtrl::Update, swim_ctrl.cpp, dispatches
 * through it; KillPlayer sets state 7). Each non-virtual `&SwimCtrl::f`
 * is g++'s {0, -1, f} record. */
const SwimCtrl::StateFunc SwimCtrl::stateFuncs[8] = {
    &SwimCtrl::StateIdle,      &SwimCtrl::StateSwim, &SwimCtrl::StateStroke,
    &SwimCtrl::StateSpin,      &SwimCtrl::StateTurn, &SwimCtrl::StateStop,
    &SwimCtrl::StateSwimStart, &SwimCtrl::StateDead,
};

/* The input controller's state methods (gInputCtrlStateFuncs;
 * InputCtrl::Update, input_ctrl.cpp). */
const InputCtrl::StateFunc InputCtrl::stateFuncs[4] = {
    &InputCtrl::StateStart,
    &InputCtrl::StateRide,
    &InputCtrl::StateUnusedRide,
    &InputCtrl::StateDead,
};

/* gMegaMixMotionSet's four entries. C linkage, as in the C table. */
extern "C" const u32 gMegaMixMotionEntries[4][2];
const u32 gMegaMixMotionEntries[4][2] = {
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
};

/* C linkage: bosses.h declares it (mega_mix.cpp). */
const struct entry_set gMegaMixMotionSet = {
    gMegaMixMotionEntries,
    0x100,
};
