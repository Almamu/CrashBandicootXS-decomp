# InputCtrl: `state` and `dirState` (#843)

InputCtrl (include/game/crash/input_ctrl.hpp) is the player's controller in the hover
room (`ROOM_KIND_HOVER`, `PLAYER_CTRL_MODE_INPUT`). The names sit next to
the class.

## `state` (`INPUT_CTRL_STATE_*`)

`state` is Ctrl's (+0x08) and indexes `stateFuncs`
(gInputCtrlStateFuncs, src/data/player_pmf_16c250.cpp). Update calls the
state's method every frame, after the D-pad steering, which it skips when
the player is dead.

| Value | Name | Method | Entered by / meaning |
|---|---|---|---|
| 0 | `START` | StateStart | Restart, which Player::ResetForRoom calls for each room: animation 0, X entry 1 and Y entry 0 queued, `dirState` cleared, the camera lead created (once) and reset; then `RIDE` |
| 1 | `RIDE` | StateRide | StateStart; also Reset (the constructor): the whole ride. It would go to `UNUSED_RIDE` when animation 0 ends, but that animation loops, so it never does |
| 2 | `UNUSED_RIDE` | StateUnusedRide | StateRide only, so never: when the animation ends, `RIDE` with X entry 2 |
| 3 | `DEAD` | StateDead | KillPlayer (HandleEvent's hits, `EVENT_HIT`..`EVENT_HIT_EXPLOSION`): SFX_PLAYER_HURT, a life lost; the player is gone when the animation ends. Update skips the steering |

## `dirState` (`INPUT_CTRL_DIR_STATE_*`)

The D-pad's last vertical direction (+0x16). Update queues a Y motion
entry only when the direction changes.

| Value | Name | Meaning |
|---|---|---|
| 0 | `NONE` | neither up nor down held: Y entry 0. StateStart clears it too |
| 1 | `UP` | up held: Y entry 3 |
| 2 | `DOWN` | down held (and not up): Y entry 5 |

## Not named here

- The motion entry numbers (`QueueNowX`/`QueueNowY`, `motionX`/`motionY`)
  and the animation numbers.
