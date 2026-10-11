# `Ctrl::state`: the boss controllers (#843)

`Ctrl::state` (+0x08) is each controller subclass's own state number.
The constructor leaves it 0, and `SetMode(n)` (Ctrl's vtable slot 4) sets
it. Tiny, Dingodile and the Neo Cortex fight's target have a
`SetState(part, next)` that does the state's set-up (animation, motion,
spawns) and calls SetMode; the others call SetMode directly, and some
controllers set another's state through its `mover` (the projectile
stuns Dingodile, Dingodile blinks his shield, a shot hits Neo Cortex,
Neo Cortex stops the target). Each class's Update is one `switch` on
`state`. Tiny's and Dingodile's Updates switch on a local copy, which is
where the inventory's `local state (Update, ...)` rows come from.

The names sit next to each class in include/boss_ctrl.hpp. Values no
code enters are `_UNK<n>`, except where the case's code says what it does.

## Dingodile (`DINGODILE_STATE_*`)

`counter` is the hits he has taken; `passes` the level edges reached on
this walk.

| Value | Name | Entered by / meaning |
|---|---|---|
| 0 | `START` | the constructor: contact and vulnerability off, then `WALK` |
| 1 | `WALK` | `START`, `TURN`, `FIRE`, `RECOVER`, `SHARK`, `WAIT`'s nextState: walks to the approach table's next stop (`FIRE` when the player is near) or the level's edge (`FIRE_TURN` after the pass's last edge, else `TURN`) |
| 2 | `WAIT` | `TURN`: `timer` frames, then `nextState` (`WALK`, or `SHARK` from the third round) |
| 3 | `TURN` | `WALK`, `WALK_BACK`, `FIRE_TURN`: the turn animation (6), then `WALK` and `WAIT` |
| 4 | `UNK4` | nothing: animation 1, then `WALK` |
| 5 | `FIRE` | `WALK` near the player (Approach): a rocket at frame 0x14, then `WALK` on |
| 6 | `FIRE_TURN` | `WALK` at the pass's last edge: a rocket, then vulnerable and `TURN` |
| 7 | `STUNNED` | DingodileProjectileCtrl, when the stalactite lands on him while he is vulnerable: motion stopped, animation 5 |
| 8 | `DAZED` | `STUNNED`: the shield blinks, 0xD2 frames; a spin attack is a `HIT`, else `RECOVER` |
| 9 | `RECOVER` | `DAZED`'s time out: animation 2, the shield blinks, then `WALK` |
| 10 | `RECOVER_HIT` | `HIT`: animation 2, the shield blinks, then `RETREAT` |
| 11 | `HIT` | a spin attack in `DAZED`: SFX_BOSS_HIT, the hit counted; the third: `DEFEATED` |
| 12 | `RETREAT` | `RECOVER_HIT`: three sharks, then walks to near the level's edge, then `TURN_BACK` |
| 13 | `TURN_BACK` | `RETREAT`: the turn animation, then `WALK_BACK` |
| 14 | `WALK_BACK` | `TURN_BACK`: walks to the edge without stopping, then `TURN` |
| 15 | `SHARK` | `WAIT`'s nextState from the third round: one shark from behind him, then `WALK` |
| 16 | `DEFEATED` | `HIT`, the third: SpawnTurboRunPower unless HasSuperBodySlam; falls out of the level |
| 17 | `GONE` | `DEFEATED`, below the level: RequestRoomExit; nothing runs |

## Dingodile's shield (`DINGODILE_SHIELD_STATE_*`)

| Value | Name | Entered by / meaning |
|---|---|---|
| 0 | `START` | the constructor: the alpha blending on, then `STEADY` |
| 1 | `BLINK_START` | Dingodile's `RECOVER`/`RECOVER_HIT`: arms two blinks, then `BLINK` |
| 2 | `BLINK_START_DAZED` | Dingodile's `DAZED`: arms two blinks, then `BLINK_DAZED` |
| 3 | `BLINK` | toggles the blink bit every 20 frames, then `STEADY` |
| 4 | `BLINK_DAZED` | the same code as `BLINK` |
| 5 | `STEADY` | nothing more |

1/3 and 2/4 run the same code; only who sets them differs.

## Dingodile's rocket and stalactite (`DINGODILE_PROJECTILE_STATE_*`)

| Value | Name | Entered by / meaning |
|---|---|---|
| 0 | `LAUNCH` | the rocket's constructor: the rise motion, then `RISE` |
| 1 | `RISE` | at the top: spawns the stalactite (SpawnStalactite), then `SPENT` |
| 2 | `SPENT` | the rocket's animation plays out, then it is gone |
| 3 | `FALL` | `DROP`: breaks on the player, on a vulnerable Dingodile (`DINGODILE_STATE_STUNNED`) or on the floor |
| 4 | `UNK4` | nothing: falls as `FALL`, but breaks only on the floor |
| 5 | `DROP` | SpawnStalactite: animation 9 and the fall motion, then `FALL` |
| 6 | `BREAK` | stops; the animation plays out, then it is gone |

## Tiny (`TINY_STATE_*`)

A hop is two states: the rise to its top (`x`, `y`, StartHop), then the
drop onto a pad (a gTouchableList anchor). `counter` is the round.

| Value | Name | Entered by / meaning |
|---|---|---|
| 0 | `START` | the constructor: contact off, then `HOP_DOWN` onto pad 1 |
| 1 | `HOP_DOWN` | `START`, `HOP_UP`: drops onto the pad, then `STOMP` |
| 2 | `HOP_DOWN_REST` | `HOP_UP_REST`: drops onto the pad (SFX_UNKNOWN_3D), then `REST` |
| 3 | `HOP_UP` | `STOMP`, `STOMP_PAD`, `REST` via `ANIM`: rises towards the next pad (PickHopTarget), then `HOP_DOWN` |
| 4 | `WAIT` | nothing directly; `ANIM` falls into its case: `timer` frames, then `nextState` |
| 5 | `ANIM` | `REST`, `DROP_IN`: the animation plays out (or `timer` runs out), then `nextState` |
| 6 | `STOMP` | `HOP_DOWN`: the stomp animation; the fourth: `HOP_UP_REST`, else `HOP_UP` |
| 7 | `HOP_UP_REST` | `STOMP`, the fourth: rises, then `HOP_DOWN_REST` |
| 8 | `REST` | `HOP_DOWN_REST`: 0xB4 frames, when a spin attack is a `HIT`; then `HOP_UP` after `ANIM` |
| 9 | `HIT` | a spin attack in `REST`: SFX_BOSS_HIT, the hit counted; the third: `DEFEATED`, else `LEAP_OFF` |
| 10 | `LEAP_OFF` | `HIT`: rises off the top of the screen, then `OFFSCREEN` |
| 11 | `DROP_IN` | `OFFSCREEN`: drops onto the round's pad, then `STOMP_PAD` after `ANIM` |
| 12 | `STOMP_PAD` | `DROP_IN`: the round's pad sinks (`stomped`, StompedHopPadCtrl), then `HOP_UP` |
| 13 | `OFFSCREEN` | `LEAP_OFF`: three falling leaves (SpawnFallingLeaves), 0x46 frames apart, then `DROP_IN` |
| 14 | `DEFEATED` | `HIT`, the third: rises; SpawnTornadoSpinPower unless HasTornadoSpin; then `FALL_OUT` |
| 15 | `FALL_OUT` | `DEFEATED`: drops out of the level, RequestRoomExit, then `GONE` |
| 16 | `GONE` | `FALL_OUT`: nothing runs |

In `OFFSCREEN`, `nextState` is a frame countdown for the leaves, not a
state: SetState's `nextState = 0` and Update's `nextState = 0x46`,
`nextState == 0` and `nextState--` stay numbers.

## The Neo Cortex fight

### CortexBossCtrl (`CORTEX_BOSS_STATE_*`)

| Value | Name | Entered by / meaning |
|---|---|---|
| 0 | `START` | the constructor: spawns the cannon and the target, then `AIM` |
| 1 | `AIM` | `START`, `HIT`: turns Neo Cortex and the cannon to the target |
| 2 | `HIT` | CortexShotCtrl, a fast shot hitting a gem: the round counted; the third: `DEFEATED`, else `AIM` |
| 3 | `DEFEATED` | `HIT`: the target stops (`CORTEX_TARGET_STATE_STOP`), SpawnBodySlamPower unless HasTurboRun; sinks out of the level |
| 4 | `GONE` | `DEFEATED`, below the level: RequestRoomExit; nothing runs |

### CortexTargetCtrl (`CORTEX_TARGET_STATE_*`)

The hops (`ENTER`, `HOP`, `HOP_OFF`) and `ANIM` go on to `nextState`.

| Value | Name | Entered by / meaning |
|---|---|---|
| 0 | `START` | the constructor: contact off, then `ENTER` |
| 1 | `ENTER` | `START`, `CHASE`: the platforms' kind cleared; hops to the level's top right, then `HOP` |
| 2 | `HOP` | `ENTER`, `FIRE`: a hop across the level (the round's height pattern), then `AIM`, or `CHASE` at the far edge |
| 3 | `AIM` | `HOP`: the red crosshair, then `FIRE` after `ANIM` |
| 4 | `FIRE` | `AIM` via `ANIM`: a slow shot, then `HOP` |
| 5 | `CHASE` | `HOP` at the edge: the platforms' kind set; hops after the player, blinking as `timer` runs out; then a fast shot and `ENTER` |
| 6 | `HOP_OFF` | `LEAVE`: hops below the level, then `GONE` |
| 7 | `ANIM` | `AIM`: the animation plays out, then `nextState` |
| 8 | `LEAVE` | `STOP`: aims below the level, then `HOP_OFF` |
| 9 | `STOP` | Neo Cortex's `DEFEATED`, through the target's mover: `LEAVE` |
| 10 | `GONE` | `HOP_OFF`: nothing runs |

### CortexBossGemCtrl, CortexCannonCtrl

| Class | Value | Name | Meaning |
|---|---|---|---|
| CortexBossGemCtrl | 0 | `CORTEX_BOSS_GEM_STATE_WAIT` | until a fast shot hits it (the part's `kind`) |
| | 1 | `CORTEX_BOSS_GEM_STATE_SHRINK` | the shrink animation, then gone |
| CortexCannonCtrl | 0 | `CORTEX_CANNON_STATE_IDLE` | its only state: contact with the player off |

## Not named here

- Tiny's `nextState` as the leaves' countdown (above).
- The SetTargetAnim / StartMotion indices and the timers.
- `SpawnShieldOrRocket`'s `mode` (0 the shield, 1 the rocket), the
  gems' and shots' `kind`.
- The Mega Mix controller, StompedHopPadCtrl, CortexBossPlatformMover
  and DingodileSharkCtrl: later fields.
