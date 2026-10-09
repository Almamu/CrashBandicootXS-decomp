#include "vehicle.hpp"

/*
 * ROM 0x0817C1C0-0x0817C200. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* The jetpack player's state methods, indexed by `state`
 * (gJetpackPlayerStateFuncs; JetpackPlayer::Update, jetpack_player_update.cpp, and
 * JetpackPlayer::RunState, jetpack_player.cpp, dispatch through it). Each
 * non-virtual `&JetpackPlayer::f` is g++'s {0, -1, f} record. */
const JetpackPlayer::StateFunc JetpackPlayer::stateFuncs[8] = {
    &JetpackPlayer::StateEnter,
    &JetpackPlayer::StateFly,
    &JetpackPlayer::StateRollLeft,
    &JetpackPlayer::StateRollRight,
    &JetpackPlayer::StateFall,
    &JetpackPlayer::StateFinish,
    &JetpackPlayer::StateBoost,
    &JetpackPlayer::StateResume,
};
