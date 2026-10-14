# The airship's, the hovercraft's and the yeti's states (#843)

`Airship`, `Hovercraft` and `Yeti` are all-static classes (their members
were separate IWRAM globals: docs/cplusplus.md), not ActorSelf objects.
Each has a `state` that indexes its plain `stateFuncs` table
(src/game/vehicles/jetpack/data/airship_state_table.cpp, hovercraft_state_tables.cpp,
yeti_state_table.cpp), which Update (the hovercraft: RunState)
calls as `stateFuncs[state]()`. So each name is its method's. The names
sit next to each class: `AIRSHIP_STATE_*` in include/game/vehicles/jetpack/airship.hpp,
`HOVERCRAFT_STATE_*` and `HOVERCRAFT_PHASE_*` in include/game/bosses/hovercraft.hpp,
`YETI_STATE_*` in include/yeti.hpp. The parts and fireballs of the
airship and hovercraft are ActorSelf objects with their own states:
[actor_states_jetpack.md](actor_states_jetpack.md).

## Airship

The airship's `SetState(st, idx)` sets `state`, zeroes `stateTimer` and
starts animation `idx` (left a number).

| Value | Name | Entered by |
|---|---|---|
| 0 | `INACTIVE` | Create; StateFall once the airship is past Y 0xBB80 (BG2 goes off) |
| 1 | `APPROACH` | Spawn, which starts the fight |
| 2 | `FIREBALLS` | StateApproach at distance 0x81FF; StateCannon once farther than 0x4300 |
| 3 | `CANNON` | StateFireballs at distance 0x31FF |
| 4 | `EXPLODE` | Damage, at 0 hit points |
| 5 | `FALL` | StateExplode, at `stateTimer` 0xAA |

What else reads it:
- Update animates and draws the picture only while it isn't `INACTIVE`,
  and LoadGraphics redraws it (BG2 on, the palette) only then.
- LoadGraphics' palette tails: `FALL` blacks out the four colours, and
  `EXPLODE` clears them one by one as `stateTimer` passes StateExplode's
  thresholds.
- GetHpPercent gives -1 (no gauge) while it is `INACTIVE`.
- IsTouching tests the boxes only in `FIREBALLS` or `CANNON`:
  `(u32)(state - AIRSHIP_STATE_FIREBALLS) <= AIRSHIP_STATE_CANNON -
  AIRSHIP_STATE_FIREBALLS`, the ROM's one unsigned range test.

## Hovercraft

The airship's twin, the same table layout. `EnterState(st, idx)` (and
its out-of-line copy SetState) is the airship's SetState without the
timer.

| Value | Name | Entered by |
|---|---|---|
| 0 | `INACTIVE` | Create |
| 1 | `APPROACH` | Spawn, which starts the fight |
| 2 | `CLOSE_IN` | StateApproach at distance 0x81FF; StateFallBack after its legs, farther than 0x8000 |
| 3 | `FALL_BACK` | StateCloseIn at distance 0x27FF |
| 4 | `EXPLODE` | nothing: StateExplodeStub is empty, the slot of the airship's StateExplode |
| 5 | `FALL` | LosePart, once the last of its four weapons is lost |

Update and LoadGraphics act only while it isn't `INACTIVE`, as the
airship's do. The launcher (hovercraft_launcher.cpp) reads it through
GetState: once two weapons or fewer are left, it starts launching while
the hovercraft is `CLOSE_IN`, or `FALL_BACK` up to depth 0x4AFF, and
past depth 0x4B00 in `FALL_BACK` it goes back to waiting.

### `phase`

`phase` means two things, by state:

| State | Value | Name | Meaning |
|---|---|---|---|
| `CLOSE_IN` | 0 | `STEER` | steers toward the player (Spawn's) |
| | 1 | `SWEEP` | sweeps from side to side; StateFallBack picks it while more than two weapons are left |
| | 2 | `CIRCLE` | circles with a growing radius; with two or fewer (StateCloseIn: any value but 0 and 1) |
| `FALL_BACK` | 0 | `FIRST_LEG` | StateCloseIn starts the count |
| | 3 | `LAST_LEG` | the legs swept side to side are counted up (`phase++`); after the last one (`phase > HOVERCRAFT_PHASE_LAST_LEG`) X goes back to 0, and far enough it closes in again |

## Yeti

| Value | Name | Entered by |
|---|---|---|
| 0 | `CHASE` | Create; StateCharge once its animation is done |
| 1 | `CHARGE` | StateChase, at random (chargeParams) |
| 2 | `CAUGHT` | Update, when its catch box is on the player (PolarPlayer::Catch) |
| 3 | `STOP` | Stop: the run is over (PolarPlayer::FinishRun, or Hurt/Shock with no mask left) |

Update follows the player's X except when `STOP`, and tries to catch
only while `CHASE` or `CHARGE`: `(u32)state <= YETI_STATE_CHARGE`.

## Not named here

- The `SetState`/`EnterState` animation index (the second argument).
- The distance and timer thresholds.
- `bg2Page ^= 1`, `hitFlashOn ^= 1` and the airship's `stateTimer & 8`
  flash: page flips and a blink, not flags.
