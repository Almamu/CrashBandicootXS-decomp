# Yes/no tests: bool, not `(u8)` of an s32 (#855)

Many of the game's yes/no functions were declared returning `s32`, and
every caller cast the result: `if ((u8)gLevelState->HasTurboRun())`. That
came from the C headers, whose callers declared a `u8` return the
definitions didn't have (docs/headers_plan.md, "Callers"). On this
compiler the caller extends a narrow return value itself (`lsl`/`lsr #24`,
or `lsl #24` and a `cmp` for a test). So a `bool` (one byte in g++ 2.9)
or a `u8` return gives the callers the same code as the cast of an `s32`,
and the definition only changes when converting its value to `bool`
needs code.

## Retyped to bool, casts gone (64)

| functions | file | casts |
|---|---|---:|
| LevelState::HasTornadoSpin, HasSuperBodySlam, HasTurboRun | level_state.cpp | 18 |
| LevelProgress::IsInBonusRoom, IsInGemPathRoom | level_query.cpp | 9 |
| LevelProgress::NextRoom, SelectRoom | room_select.cpp | 2 |
| LevelState::IsCrystalSaved | level_state.cpp | 2 |
| LevelState::LevelHas{Yellow,Blue,Green,Red,GemPath}Gem; LevelHas*GemEntity, LevelHasEntityType | level_state.cpp, level_query.cpp | 5 |
| LevelEntityFlags::IsActivated, IsGone | entity_flags.cpp | 9 |
| IsSpawnCollected | actor_spawn_collected.cpp | 3 |
| IsActorMaskAssistDue | actor_category_stats.cpp | 2 |
| IsBrightnessFadeActive | fade_to_black.cpp | 1 |
| LevelLayers::Probe, ProbeX, ProbeY; MovingSprite::ProbeEdgeTerrain | terrain_probe.cpp, terrain_probe_axes.cpp, moving_sprite_probe.cpp | 11 |

LevelState::HasDoubleJump returns `u8` (2 casts gone). It is `return
progress.flags >> 7`, and as a bool that gets a `!= 0` test the ROM
doesn't have. The other three power tests are the sign-bit extraction
`(u32)(flags << n) >> 31`, which converts to bool with no code.

## Kept s32, with a comment

- **IsTouchingPlayer** (31 casts, the 3D stages' actors). It calls the
  category's `isTouchingPlayer` hook, PolarIsTouchingPlayer or
  JetpackIsTouchingPlayer. As bools, the callers and IsTouchingPlayer are
  the same, but the two hooks get a second return path: they return the
  inline `BoxOverlap`'s `u8`, and with the `bool` conversion the early
  `return 0` gets its own exit.
- **Entity::OverlapsRect and IsInsideRect** (the virtuals, 8 casts in the
  part and crate lists). As bools, Entity's, Sprite's and Crate's
  definitions change.

`HpActor::IsUnshootable` and its overrides (1 cast) were not tried.
