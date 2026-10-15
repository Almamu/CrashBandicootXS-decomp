# `ActorSelf::state`: the jetpack and hovercraft actors (#843)

`ActorSelf::state` (+0x28) is each actor subclass's own state number.
ActorSelf's constructor sets 0, and `SetState(st, idx)` sets it, zeroes
`stateTime` and restarts animation sequence `idx` (the second argument,
left a number). Almost all of these actors have a `stateFuncs` table of
state methods (engine/src/game/vehicles/jetpack/data/*_state_table(s).cpp, hovercraft_state_tables.cpp) that
Update or RunState calls as `(this->*stateFuncs[state])()`, so `state`
is the table index and each name is its method's.

The names sit next to each class: the jetpack actors' in
engine/include/game/vehicles/vehicle.hpp, the airship's and hovercraft's parts in
engine/include/boss_actors.hpp. The polar run's actors are in
[actor_states_polar.md](actor_states_polar.md). The airship and the
hovercraft themselves (the all-static `Airship` and `Hovercraft`) aren't
ActorSelf objects, and their states are a later field.

## The jetpack actors

| Class | Value | Name | Entered by |
|---|---|---|---|
| JetpackPlayer | 0 | `ENTER` | the constructor at the course's start: flies in with the input off |
| | 1 | `FLY` | StateEnter, StateResume, StateBoost, the rolls' end |
| | 2 | `ROLL_LEFT` | StateFly and the rolls, L held |
| | 3 | `ROLL_RIGHT` | StateFly and the rolls, R held |
| | 4 | `FALL` | Damage, out of hit points |
| | 5 | `FINISH` | FinishRun, the course's end |
| | 6 | `BOOST` | PassRing, a ring (only from 1, 2, 3 or 6) |
| | 7 | `RESUME` | the constructor at a checkpoint (a nonzero start depth) |
| JetpackPlane | 0 | `FLY` | the constructor: hops from spawn point to spawn point |
| | 1 | `FOLLOW` | Aim, at the end of a slow hop chain: eases to the player |
| | 2 | `KNOCKED_OUT` | Damage |
| | 3 | `FALL` | StateKnockedOut; gone below y 0xE100 |
| JetpackBomber | 0-5 | `IDLE`, `HOME`, `BOB_VERTICAL`, `SWING_HORIZONTAL`, `CIRCLE`, `DROP` | the constructor, from the record's kind (4-9) |
| | 6 | `DYING` | touching the player, or Damage; gone when the animation is done |
| JetpackBalloon | 0 | `ATTACHED` | the constructor: its crate moves it |
| | 1 | `FLOAT_AWAY` | Release; gone above y -0xE100 |
| | 2 | `POP` | Damage; gone when the animation is done |
| JetpackBalloonCrate (and the question, health and time crates) | 0 | `HANG` | the constructor |
| | 1 | `FALL` | Break, its balloon popped; gone below y 0xE100 |
| | 2 | `DESTROYED` | touched or shot: it pays out; gone when the animation is done |

JetpackPlayer::PassRing tests `state` through a local copy; the four
tests name it the same way.

## The airship's and hovercraft's parts

| Class | Value | Name | Entered by |
|---|---|---|---|
| AirshipFireball | 0 | `ORBIT` | the constructor |
| | 1 | `SPIRAL_IN` | StateOrbit |
| | 2 | `EXPLODE` | StateOrbit, StateSpiralIn, Damage; gone when the animation is done |
| HovercraftFireball | 0 | `FLY` | the constructor |
| | 1 | `EXPLODE` | StateFly, Damage |
| HovercraftCannon | 0 | `WAIT` | the constructor, StateFire |
| | 1 | `FIRE` | StateWait |
| | 2 | `DESTROYED` | Damage; the common update stops when its animation is done |
| HovercraftLauncher | 0 | `WAIT` | the constructor, StateLaunch |
| | 1 | `LAUNCH` | StateWait; Damage hurts it only in this state |
| | 2 | `DESTROYED` | Damage |
| HovercraftSideGun (no state methods) | 0 | `ALIVE` | the constructor: follows the hovercraft and fires |
| | 1 | `DESTROYED` | Damage |

## Not named here

- The `SetState` animation sequence (the second argument).
- HovercraftCannonFlash's `SetState(0, 0)`: its only state.
- HovercraftLauncher::Damage's `dead = st`: it stores the state's 1 as
  the dead flag, which is how the ROM has it.
