# Smaller pointer and value types (#855)

Fields and parameters that were wider or vaguer than what they hold, and
that every read cast to the same type.

| thing | was | is | casts gone | notes |
|---|---|---|---:|---|
| `LevelState::gemPlatform` (level_state.hpp, 0x1BC) | `void *` | `Platform *` | 4 | StartTimeTrial's `(Platform *)gemPlatform`. `SetGemPlatform` takes a `Platform *` (its callers in spawn_gem_platforms.cpp pass one). Mangled name: `SetGemPlatform__10LevelStatePv` to `SetGemPlatform__10LevelStateP8Platform`. |
| `PauseMenu::DrawFraction(count, total)` (menus.hpp) | `void *` | `const u8 *` | 2 | the two `(u8 *)` casts to Font::DrawText. The callers pass the pause menu's `u8[3]` number texts. Mangled name: `DrawFraction__9PauseMenuPvT1` to `DrawFraction__9PauseMenuPCUcT1`. |
| `IsUnshootable()` (HpActor and its 18 overrides) | `s32` | `u8` | 1 | FindShotTarget's `(u8)n->IsUnshootable() == 0`. |
| `LevelState::bonusPlatform` (level_state.hpp, 0x1B8), `GetBonusPlatform()` | `void *` | `Platform *` | 3 | StartTimeTrial's `(Platform *)bonusPlatform`, and PlayRoom's two reads (below). `SetBonusPlatform` takes a `Platform *` (SpawnBonusPlatform passes Platform::Create's result). Mangled name: `SetBonusPlatform__10LevelStatePv` to `SetBonusPlatform__10LevelStateP8Platform`. |
| `anim_table_record::index` (actor_anim.h, 0x00) | `u32` | `u8` | 12 | the jetpack and polar actors' `switch ((u8)record->index)` and their tests. Every reader loads it with `ldrb`, as GetRecordIndex already returned a `u8`. |

cxx_symbols.txt maps the new mangled names onto the same C names.

## IsUnshootable is u8, not bool

As a `bool`, every override that returns a `u8` field (`dead`,
`exploding`, `dying`, `unshootable`, `triggered`, `done`) gets a `!= 0`
test that the ROM doesn't have: 11 objects change. As a `u8`, each
override returns its `ldrb` as before, and the caller extends the result
itself, as it did for the `(u8)` cast. This is the same as
LevelState::HasDoubleJump in predicates.md. There is a comment at
HpActor's declaration.

## anim_table_record::index and the tables

With `index` a `u8`, the three bytes after it are the compiler's padding
before the `table_A` pointer at 0x04. The ROM tables
(anim_family_178f80.cpp, anim_family_17aa6c.cpp, level_gfx_17cff4.cpp's
gLogoActorAnim) keep their positional initializers. Every index is below
256 and its upper bytes were 0, so the data objects are the same.

## bonusPlatform and PlayRoom's first word

LevelProgress::RunRoom (play_room.cpp), leaving the bonus round, read
`*(s32 *)gLevelState->GetBonusPlatform()`. The platform's first word is
`Entity::x` (entity.hpp, 0x00, Q8), so the checkpoint is 0x1E00 (30
pixels) left of the platform: `GetBonusPlatform()->x + -0x1E00`. The next
line's `((Platform *)state->GetBonusPlatform())->GetExitMirror()` is
`state->GetBonusPlatform()->GetExitMirror()`. play_room.o is the same.

RunRoom's `x = (s32)gLevelState; LevelState *state = (LevelState *)x;`
stays: the ROM has the level state in `x`'s register, and with
`LevelState *state = gLevelState` play_room.o changes. It has its comment.
