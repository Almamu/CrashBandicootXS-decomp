# `EnemyCtrl::state` and `mode`: the enemy controller (#843)

The enemy controller (`class EnemyCtrl`, engine/include/game/enemies/enemy_ctrl.hpp) has two
small-number fields:

- `state` (+0x74) picks what Update does each frame. A spawner sets it
  once, through SetState, after it has set up the controller.
- `mode` (+0x68) is the animation mode. SetAnimMode plays `anims[mode]`, an
  entry of the spawner's 8-entry table (gEnemyDefaultAnimMap,
  gPenguinAnimMap, ...), and the per-state updaters step through the
  modes.

The names sit next to the class: `ENEMY_STATE_*`, `ENEMY_MODE_*` and
`ENEMY_ANIM_NONE`.

## `state`

Update switches on 1-18; 0 and anything above 18 do nothing. Each name
says which updaters the state runs. SetState starts each state with the
animation mode it begins in.

| Value | Name | Update runs | SetState starts it with | Spawners |
|---|---|---|---|---|
| 1 | `STILL` | nothing | mode 0 | moray eel, laser barrier |
| 2 | `PATROL` | UpdatePatrol | X motion 1, mode 0 | lizard, patrolling jungle enemy, polar bear, patrolling sewer enemy, rat |
| 3 | `TRIGGER` | UpdateTriggerBox | mode 0 | venus flytrap |
| 4 | `ATTACK_CYCLE` | UpdateAttackCycle | mode 4 if `cycleOffset >= idleTime`, else 0 | stationary space enemy, piston crusher, flamethrower lab assistant, wooden crusher |
| 5 | `FALL` | falls until it lands (`hitAxes == 8`); within 0x1E00 of the bottom of the level it stops colliding, and 0x1E00 below the bottom it is gone | a fixed fall ramp, `collides` set | seal |
| 6 | `BOB` | UpdateBob | mode 0 | jellyfish |
| 7 | `FLIP_CYCLE` | UpdateFlipCycle | X motion 2, mode 0, `counter` 0 | squid |
| 8 | `HOP` | UpdateHop | X and Y motion 3, mode 0 | frog |
| 9 | `HOME_X_OSCILLATE` | UpdateHomingX, UpdateOscillateX (and saves the position in gHomingEnemyX/Y) | mode 0 | homing sewer enemy |
| 10 | `HOME_Y` | UpdateHomingY | mode 0 | none |
| 11 | `HOME` | UpdateHomingX, UpdateHomingY; a sea mine that touches something hits itself (EVENT_HIT) | mode 0 | sea mine |
| 12 | `UNK12` | nothing | nothing | none |
| 13 | `PATROL_ATTACK` | UpdatePatrol, UpdateAttackCycle | X motion 1, then as 4 | penguin, electric eel, patrolling space enemy |
| 14 | `ATTACK_OSCILLATE` | UpdateAttackCycle, UpdateOscillateY | as 4 | pufferfish |
| 15 | `PATROL_HOME_Y` | UpdatePatrol, UpdateHomingY | X motion 1, mode 0 | shark |
| 16 | `ATTACK_SHOOT` | UpdateAttackCycle, UpdateShooter | as 4 | blowgun tribesman |
| 17 | `SWOOP` | the vulture's swoop: UpdateTriggerBox while it is at baseY, then it flies off and comes back from above | mode 0 | vulture |
| 18 | `SAUCER` | the saucer hum, UpdatePatrol, UpdateAttackCycle, the electric popup in modes 3-5 | X motion 1, then as 4 | saucer lab assistant |

State 12 is an empty case in both Update and SetState, and no spawner
sets it, so it is `_UNK12`. Nothing sets state 10 either, but its
behaviour is clear from Update.

## `mode`

| Value | Name | Set by | What it is |
|---|---|---|---|
| 0 | `NORMAL` | every state's start; the end of every other mode | idle, or walking (UpdatePatrol) |
| 1 | `TURN` | UpdatePatrol at a patrol bound, UpdateHop, UpdateFlipCycle | turning round: the X mirror bit flips when it ends (UpdateFlipCycle flips it before) |
| 2 | `STRIKE` | UpdateTriggerBox when the player touches the box, UpdateShooter on its period | the flytrap's snap or the vulture's swoop; the tribesman's dart at keyframe 10 |
| 3 | `ATTACK_START` | UpdateAttackCycle | going into the attack; skipped when the table has no animation for it. The crushers slam down and become solid at its end, and the saucer launches its popup |
| 4 | `ATTACK` | UpdateAttackCycle, SetState | the attack. UpdatePatrol walks in it too, and UpdateShooter shoots from it |
| 5 | `ATTACK_END` | UpdateAttackCycle | coming out of the attack, skipped like 3; the saucer's popup goes |
| 6 | `ATTACK_TURN` | UpdatePatrol from mode 4 | turning round in the attack pose; the squid's UpdateFlipCycle uses it for its vertical flip (the Y mirror bit) |
| 7 | `ATTACK_STRIKE` | UpdateShooter from mode 4 | the dart from the attack pose, at keyframe 8 |

`ENEMY_ANIM_NONE` (8) is the table entry for a mode the enemy has no
animation for. UpdateAttackCycle tests `anims[ENEMY_MODE_ATTACK_START]`
and `anims[ENEMY_MODE_ATTACK_END]` against it and skips that mode when it
matches. The tables themselves (engine/src/game/enemies/data/enemy_anim_maps.cpp) stay
numbers.

## Not named here

- `SetMotionX(n)` / `SetMotionY(n)`: the motion-set records (`modeB`,
  `modeA`).
- EnemyCtrl::HandleEvent's `gPlayer->ctrlMode == 1`: that is
  `Player::ctrlMode`, a later field.
