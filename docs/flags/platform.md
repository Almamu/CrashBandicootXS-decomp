# `Platform::type`: what a platform does (#843)

A level platform (`class Platform`, engine/include/game/objects/platform.hpp) has a `type`
(+0x78). Platform::Create takes it from the platform's spawn record, unless
the spawn kind forces one. Every platform that gets a PlatformMover also
copies the type into the mover's `kind` (+0x10). The names,
`PLATFORM_TYPE_*`, sit next to the class.

| Value | Name | Spawn kinds that force it | Mover | What the code does with it |
|---|---|---|---|---|
| 0 | `STILL` | | none | ResolveCollision carries the player when it lands on it from above (`carried`, `hitAxes = 8`) |
| 1 | `MOVING` | | a PlatformMover over the record's `distX`/`distY`; the record's `flag` makes the platform always active (updated off screen too) | it moves back and forth; the mover carries the player while `active` |
| 2 | `WARP_EXIT` | 4 | none | standing within 7 pixels of its centre sends `EVENT_WARP_EXIT` |
| 3 | `WARP_BONUS` | 5 (SpawnBonusPlatform, until the bonus round is done; after it, kind 7 keeps the record's type) | none | `EVENT_WARP_BONUS_ROUND` until the bonus round is done (not in time trial); its exit facing comes from the record (SetExitMirror) |
| 4 | `WARP_GEM_PATH` | 3, 9-12 | none | `EVENT_WARP_GEM_PATH` until the gem path is done (not in time trial) |
| 5 | `FALLING` | | a still PlatformMover | when the player lands, the mover's `timer` starts; it wobbles for 5 frames in every 30, and 60 frames after the landing it falls (Y motion 3) |
| 6 | `FLIP` | 6 (SpawnFlipPlatform, entity 0x57) | a still PlatformMover; in the Neo Cortex fight a CortexBossPlatformMover (its constructor passes `PLATFORM_TYPE_FLIP`) | the green pad that turns red and tips over: past frame 0x12 it has no contact and drops the player, and 120 frames after the animation ends it comes back. The mover doesn't carry the player |
| 7 | `ROCK` | 8 (SpawnRockPlatform) | a still PlatformMover | ResolveCollision carries the player as on a still one. The mover holds its animation on the first frame while it isn't `active`, and marks the platform gone when the animation ends; but ResolveCollision sets `active` only for types 1, 5 and 6, so nothing starts it. The name comes from the spawner, since what the animation is for can't be seen |

When the player lands on a type 1, 5 or 6, ResolveCollision sets the
mover's `active` (MovePlayer then carries the player along, except on a
6) instead of the warp test, and clears it when he leaves. ResolveCollision's side
resolution skips them: `type != PLATFORM_TYPE_MOVING && type !=
PLATFORM_TYPE_FALLING && type != PLATFORM_TYPE_FLIP`.

The PlatformMover constructor gives types 6 and 7 no travel distance. Its
test is one unsigned range check, `(u32)(kind - PLATFORM_TYPE_FLIP) <=
PLATFORM_TYPE_ROCK - PLATFORM_TYPE_FLIP`, which is how the ROM has it.

## The spawn kind, `PLATFORM_KIND_*`

Platform::Create's last argument is the spawner's `kind`: the platform's
animation in sprite bank 0x1D4 (stored in its `tag`). The warp kinds also
force a type; the others keep the record's. The names sit next to
`PLATFORM_TYPE_*` in platform.hpp.

| Kind | Name | Spawner | Forced type |
|---|---|---|---|
| 0 | `LARGE` | SpawnLargePlatform | |
| 1 | `SMALL` | SpawnSmallPlatform | |
| 2 | `MEDIUM` | SpawnMediumPlatform | |
| 3 | `GEM_YELLOW` | SpawnYellowGemPlatform, once the yellow gem is collected | `WARP_GEM_PATH` |
| 4 | `EXIT_PAD` | SpawnRoomExit, with the player on foot | `WARP_EXIT` |
| 5 | `BONUS` | SpawnBonusPlatform | `WARP_BONUS` |
| 6 | `FLIP` | SpawnFlipPlatform (entity 0x57) | `FLIP` |
| 7 | `BONUS_DONE` | SpawnBonusPlatform once the bonus round is done, or in a time trial | |
| 8 | `ROCK` | SpawnRockPlatform | `ROCK` |
| 9 | `GEM_BLUE` | SpawnBlueGemPlatform | `WARP_GEM_PATH` |
| 0xA | `GEM_GREEN` | SpawnGreenGemPlatform | `WARP_GEM_PATH` |
| 0xB | `GEM_RED` | SpawnRedGemPlatform | `WARP_GEM_PATH` |
| 0xC | `GEM_PATH_DONE` | the four gem spawners once the gem path is done, or in a time trial | `WARP_GEM_PATH` |

The types in the room data (the parameter record's `type` word) stay
numbers: tools/levels.py writes the records as raw words.

## Not named here

- CortexBossPlatformMover reuses `kind` as a 0/1 flag
  (CortexTargetCtrl::SetPlatformsKind), so cortex.cpp's uses aren't
  platform types.
- ResolveCollision's `result`/`hdir`/`vdir` (1, 2, 4, 8) are direction
  bits, which a later #843 PR names.
