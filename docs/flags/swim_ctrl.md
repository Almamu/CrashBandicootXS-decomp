# `SwimCtrl::state` and `mode`: the swim controller (#843)

The swim controller (`class SwimCtrl`, include/swim_ctrl.hpp) drives the
diving Crash in the underwater rooms. It has two small-number fields:

- `state` (Ctrl's, +0x08) is the state method Update runs, the
  `stateFuncs` index (gSwimCtrlStateFuncs).
- `mode` (+0x22) is the row of gSwimCtrlModeAnimRows, the animation set
  the state plays; `tilt` picks the column.

SetState (and its inline copy SetStateNow) sets both: `SetState(state,
mode, timer, timerMax)`. The state methods also set `state` alone through
Ctrl::SetMode. The names, `SWIM_STATE_*` and `SWIM_MODE_*`, sit next to the
class.

## `state`

| Value | Name | Method | Entered from |
|---|---|---|---|
| 0 | `IDLE` | StateIdle: floating, a slow bob (Y motion 1 at 30 frames, 2 at 60) | Reset, Restart, StateStop at the end of its animation |
| 1 | `SWIM` | StateSwim | ResetMode (the end of a stroke, spin or turn, or a D-pad press out of SWIM_START) |
| 2 | `STROKE` | StateStroke | StartStroke (A) |
| 3 | `SPIN` | StateSpin | StartSpin (B or R), and a spin turn's end |
| 4 | `TURN` | StateTurn | CheckTurn, when the D-pad points the other way |
| 5 | `STOP` | StateStop | StateSwim, StateStroke and StateSwimStart with the D-pad released at level tilt |
| 6 | `SWIM_START` | StateSwimStart | StateIdle when the D-pad is pressed |
| 7 | `DEAD` | StateDead: the player is gone at the end of the death animation | KillPlayer (HandleEvent's hits) |

- Update runs only the state method while `DEAD`. While `STROKE` it
  doesn't level `tilt` out, and steps it only while a direction is held.
- CheckTurn turns the player in every state but `TURN` and `DEAD`: `case
  SWIM_STATE_IDLE ... SWIM_STATE_SPIN: case SWIM_STATE_STOP ...
  SWIM_STATE_SWIM_START:`.
- ApplySwimDrift picks its drift values for `STROKE`, `SPIN` and the rest.

## `mode`

| Value | Name | Set with |
|---|---|---|
| 0 | `IDLE` | state 0 |
| 1 | `SWIM` | state 1 |
| 2 | `STROKE` | state 2 |
| 3 | `SPIN` | state 3 |
| 4 | `TURN_RIGHT` | state 4, from CheckTurn: the player faced left (X-mirrored) and the D-pad points right; the mirror bit is cleared |
| 5 | `SPIN_TURN_RIGHT` | the same turn started in a spin, or B/R during a `TURN_RIGHT` |
| 6 | `TURN_LEFT` | state 4: the player faced right and the D-pad points left; SetFlipX(1) at its end |
| 7 | `SPIN_TURN_LEFT` | the same turn started in a spin, or B/R during a `TURN_LEFT` |

GetDpadDirection gives 4, 6 and 8 for right, up-right and down-right, and
3, 5 and 7 for the left ones (gDpadDirectionTable). Those are what
CheckTurn tests.

- A spin turn goes back to the plain turn when its timer runs out
  (StateTurn).
- At its end a turn goes back to `SWIM`, and a spin turn to `SPIN`.
- While `SPIN` or a spin turn, the player's `kind` is `EVENT_ATTACK_SPIN`
  (Update's last test). Otherwise it is `EVENT_HIT`.

## Left as numbers

ApplyLevel (the out-of-line copy is ApplyTilt) switches on `mode` with
the labels 7, 14-18, 23, 33-37 and 41. Only 7 can be a swim mode, and it
is spelled `SWIM_MODE_SPIN_TURN_LEFT`. What the others were meant to be
isn't known: read as `ACTION_STATE_*` they are no set the code uses
together (AIR_SPIN to STAND_UP, LAND, HANG_SPIN to HANG_MOVE, WARP_IN),
and the ROM's jump table has them, so they stay numbers, with a comment.
