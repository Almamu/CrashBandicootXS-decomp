#include "vehicle.hpp"

/*
 * ROM 0x0817A6B8-0x0817A728. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The polar player's state methods, indexed by `state` (gPolarPlayerStateFuncs;
 * PolarPlayer::RunState, polar_player_states.cpp, and PolarPlayer::Update,
 * polar_player.cpp, dispatch through it). Each non-virtual `&PolarPlayer::f`
 * is g++'s {0, -1, f} record: {s16 this delta, s16 index, the code}. */
const PolarPlayer::StateFunc PolarPlayer::stateFuncs[14] = {
    &PolarPlayer::StateMount,
    &PolarPlayer::StateRun,
    &PolarPlayer::StateDash,
    &PolarPlayer::StateBoost,
    &PolarPlayer::StateJump,
    &PolarPlayer::StateLaunched,
    &PolarPlayer::StateKnockedOff,
    &PolarPlayer::StateCaught,
    &PolarPlayer::StateCarriedOff,
    &PolarPlayer::StateLand,
    &PolarPlayer::StateFinish,
    &PolarPlayer::StateFinishLeap,
    &PolarPlayer::StateShocked,
    &PolarPlayer::StateRecover,
};
