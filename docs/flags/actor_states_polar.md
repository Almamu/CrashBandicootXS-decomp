# `ActorSelf::state`: the polar run's actors and the logo actor (#843)

`ActorSelf::state` (+0x28) is each actor subclass's own state number.
ActorSelf's constructor sets 0, and `SetState(st, idx)` sets it, zeroes
`stateTime` and restarts animation sequence `idx` (the second argument,
left a number: it is the record's sequence index). Some code stores
`state` directly, as the ROM does. The names sit next to each class:
`POLAR_PLAYER_STATE_*`, `POLAR_AKU_AKU_STATE_*`, `POLAR_LAUNCHER_STATE_*`,
`POLAR_PENGUIN_STATE_*` and `POLAR_ICICLE_STATE_*` in engine/include/game/vehicles/vehicle.hpp,
`LOGO_ACTOR_STATE_*` in engine/include/frontend.hpp. The jetpack and hovercraft
actors are a later PR.

## PolarPlayer

`state` indexes `stateFuncs` (engine/src/game/vehicles/polar/data/polar_player_state_table.cpp), and
PolarPlayer::Update calls `(this->*stateFuncs[state])()`, so each name is
its method's.

| Value | Name | Entered by |
|---|---|---|
| 0 | `MOUNT` | the constructor: drops onto the bear |
| 1 | `RUN` | StateJump, StateDash (B released), StateBoost, StateLaunched, StateLand, StateRecover |
| 2 | `DASH` | StateRun (B), StateBoost (B held) |
| 3 | `BOOST` | Boost, a boost pad |
| 4 | `JUMP` | StateRun, StateDash (A) |
| 5 | `LAUNCHED` | Launch, a launcher |
| 6 | `KNOCKED_OFF` | Hurt with no mask, StateShocked |
| 7 | `CAUGHT` | Catch, the yeti |
| 8 | `CARRIED_OFF` | StateCaught |
| 9 | `LAND` | StateMount |
| 10 | `FINISH` | Update, when the finish countdown (FinishRun) runs out |
| 11 | `FINISH_LEAP` | StateFinish |
| 12 | `SHOCKED` | Shock, the electric fence |
| 13 | `RECOVER` | nothing: no code sets it |

Boost and Launch act only while running, dashing or boosted:
`(u32)(state - POLAR_PLAYER_STATE_RUN) <= POLAR_PLAYER_STATE_BOOST -
POLAR_PLAYER_STATE_RUN`, the ROM's one unsigned range check.

## PolarAkuAku

| Value | Name | Meaning |
|---|---|---|
| 0 | `FOLLOW` | hovers round behind the player on a sine path (Move) |
| 1 | `INVINCIBLE` | the third mask: sits on the player, in front, while gPolarAkuAkuInvincibleTimer runs (Refresh) |
| 2 | `LOSE_MASK` | the last mask was lost (Refresh with `lost`): its animation plays, then Update refreshes it |

## PolarLauncher, PolarPenguin, PolarIcicle

| Class | Value | Name | Meaning |
|---|---|---|---|
| PolarLauncher | 0 | `READY` | launches the player on touch (PolarPlayer::Launch), or is set off by the yeti |
| | 1 | `EXPLODE` | SFX_EXPLOSION and animation 1; deleted when it is done |
| PolarPenguin | 0 | `FLY` | flies from spawn to spawn (Aim), hurting the player |
| | 1 | `KNOCKED_AWAY` | knocked away by the player (when Hurt says it hit) or the yeti; flies off |
| PolarIcicle | 0 | `HANGING` | until depth 0x5000 |
| | 1 | `DROP1` | until depth 0x5A00 |
| | 2 | `DROP2` | until depth 0x6400 |
| | 3 | `DROP3` | the last step |

The icicle's `state` counts its fall steps: at each depth it switches to
the next animation and does `state = state + 1`, which stays as written.

## LogoActor

Crash on the company logos (company_logos.cpp):

| Value | Name | Meaning |
|---|---|---|
| 0 | `ENTER` | the first animation plays through |
| 1 | `WALK` | sequence 1, until its frame 0x12 |
| 2 | `HIT` | sequence 7 (SFX_UNKNOWN_4F), until its frame 7, where the animation freezes (SFX_PLAYER_HURT) |
| 3 | `FLY_OFF` | moves towards the camera and up, for 16 frames |
| 4 | `GONE` | the end: Draw draws nothing |

## Not named here

- The `SetState` animation sequence (the second argument).
- The `stateTime` thresholds.
- Yeti's `state`: the yeti isn't an ActorSelf, and is a later field.
