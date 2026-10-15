# The direction bits: `Sprite::dir`, `hitAxes`, `hitMask` (#843)

Four bytes and words of the sprite objects hold the same four direction
bits. The names, `PART_DIR_*`, are in engine/include/game/entities/sprite_flags.h (sprite_obj.hpp
includes it). They replace player.hpp's `PLAYER_DIR_*` and
`PLAYER_HIT_X`/`PLAYER_HIT_Y`, which named the same bits on the player.

| Bit | Value | Name | In `dir` | In `hitAxes` / `hitMask` |
|---|---|---|---|---|
| 0 | 1 | `PART_DIR_RIGHT` | moving right (`speedX > 0`) | the probe going right hit a wall |
| 1 | 2 | `PART_DIR_LEFT` | moving left (`speedX < 0`) | the probe going left hit a wall |
| 2 | 4 | `PART_DIR_UP` | moving up (`speedY < 0`) | the probe going up hit the ceiling |
| 3 | 8 | `PART_DIR_DOWN` | moving down (`speedY > 0`) | on the floor |
| 0-1 | 3 | `PART_DIR_X_MASK` | the X motion | a wall |
| 2-3 | 0xC | `PART_DIR_Y_MASK` | the Y motion | the floor or the ceiling |

## `Sprite::dir` (+0x24)

MovingSprite::ApplyVelocity and Player::ApplyVelocity set it every frame
from the speeds: 0, then `RIGHT` or `LEFT` from `speedX`, then `DOWN` or
`UP` from `speedY` added. PlatformMover::MovePlayer adds the platform's
motion to the player's (a local copy, `dir`, stored back).

Readers:

- GroundSprite::ProbeTerrainAxes probes in the directions the sprite
  moves: `dir & PART_DIR_X_MASK` and `dir & PART_DIR_Y_MASK` are the
  probe modes it passes to `LevelLayers::Probe`. A sprite that doesn't
  move vertically and has the floor probe on is on the floor (`DOWN` in
  the result).
- The camera's StepDirectional steers its look-ahead with the target's
  (the player's) bits.
- Platform::ResolveCollision and Crate::ApplyCollision carry the player
  only when it isn't moving up (`!(dir & PART_DIR_UP)`).
- Crate::QueuePlayerCollision: a body slam while moving up (`dir ==
  PART_DIR_UP`) hits as a jump.

## `MovingSprite::hitAxes` (+0x68) and `hitMask` (+0x74)

ProbeTerrainAxes returns the directions whose probe hit the terrain, and
adds them to `hitMask` (cleared at its start). GroundSprite::
CheckPlayerContact ORs the result into `hitAxes`. Standing on something
(the floor, a platform, a crate, `carried`) sets `PART_DIR_DOWN`, and the
resets put the sprites down on the floor (`hitAxes = PART_DIR_DOWN`).
When the edge probe doesn't confirm a floor the sprite has just reached,
`hitAxes &= PART_DIR_X_MASK | PART_DIR_UP` takes the floor back (and sets
entity flag 5).

Readers:

- `hitAxes & PART_DIR_DOWN` / `== PART_DIR_DOWN`: on the floor. The
  action controller's states use it to tell standing from falling
  (CheckLeftGround, ApplyMotion, StateLeftGround, StateSpin,
  StateAirSpin, StateTornadoSpin, StateCrouchDown); the seal's `ENEMY_STATE_FALL` stops
  falling on it; GroundSprite::Update keeps the hitbox's bottom edge
  in place.
- `hitAxes & PART_DIR_UP` / `== PART_DIR_UP`: hit the ceiling. StateJump
  and StateAirborne fall; GroundSprite::Update keeps the hitbox's top
  edge in place.
- `hitAxes == PART_DIR_RIGHT || == PART_DIR_LEFT`: against a wall in the
  air (StateAirborne).
- `hitAxes & PART_DIR_X_MASK`: against a wall (SwimCtrl::StartSpin).
- `hitMask & PART_DIR_X_MASK` / `& PART_DIR_Y_MASK`: the swim controller
  stops the X or Y speed.

## EVENT_BUMP's side

The objects that push the player out sideways send `EVENT_BUMP` with the
direction the player was moving in when it hit them, and add it to the
player's `hitMask` (Crate::QueuePlayerCollision's `AddPlayerHit`). The
action controller ORs it into `hitAxes`. Platform::ResolveCollision works
it out the same way: `side` and `hdir` are `RIGHT` or `LEFT`, `vdir` is
`UP` (the player is below the platform) or `DOWN` (above), and `result`,
which side it resolves the overlap on, is one of the four.
Crate::QueuePlayerCollision's `dirX`/`dirY` are the same.

## The hitbox edges

OffsetToHitboxEdge, OffsetToHitboxEdgeStart and OffsetFromHitboxEdge
(sprite_obj.cpp; the last is UNUSED) take one of these directions as
`kind`: the edge of the hitbox that faces it (`RIGHT`/`LEFT`: x +/- w/2;
`UP`: y + offY; `DOWN`: y + offY + h). Their switches also take
`PART_DIR_Y_MASK` (12, both vertical bits) as `DOWN`. GroundSprite's
probes pass `PART_DIR_DOWN` for the point under its feet, and
`dir & PART_DIR_X_MASK` / `& PART_DIR_Y_MASK` for the scan's start.

## Not named here

- `GetDpadDirection()`'s results (the `dir` locals of the action and swim
  controllers, `dir == 3 || dir == 5 || dir == 7`): a d-pad direction
  number, not these bits.
- Crate::QueuePlayerCollision's `edge` and Crate::BreakInStack's `dir`
  (4 and 8): the crate side, left for the crate collision's own pass.
- `PlatformMover::dirX`/`dirY`: a 0/1 toggle (`^= 1`), the way the mover
  goes on its path.
