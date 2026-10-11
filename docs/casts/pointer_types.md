# Smaller pointer and value types (#855)

Fields and parameters that were wider or vaguer than what they hold, and
that every read cast to the same type.

| thing | was | is | casts gone | notes |
|---|---|---|---:|---|
| `LevelState::gemPlatform` (level_state.hpp, 0x1BC) | `void *` | `Platform *` | 4 | StartTimeTrial's `(Platform *)gemPlatform`. `SetGemPlatform` takes a `Platform *` (its callers in spawn_gem_platforms.cpp pass one). Mangled name: `SetGemPlatform__10LevelStatePv` to `SetGemPlatform__10LevelStateP8Platform`. |
| `PauseMenu::DrawFraction(count, total)` (menus.hpp) | `void *` | `const u8 *` | 2 | the two `(u8 *)` casts to Font::DrawText. The callers pass the pause menu's `u8[3]` number texts. Mangled name: `DrawFraction__9PauseMenuPvT1` to `DrawFraction__9PauseMenuPCUcT1`. |
| `IsUnshootable()` (HpActor and its 18 overrides) | `s32` | `u8` | 1 | FindShotTarget's `(u8)n->IsUnshootable() == 0`. |
| `anim_table_record::index` (actor_anim.h, 0x00) | `u32` | `u8` | 12 | the jetpack and polar actors' `switch ((u8)record->index)` and their tests. Every reader loads it with `ldrb`, as GetRecordIndex already returned a `u8`. |

cxx_symbols.txt maps the two new mangled names onto the same C names.

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

## Left

- `LevelState::bonusPlatform` (`void *`): StartTimeTrial casts it to
  `Platform *`, but PlayRoom reads its first word through
  `*(s32 *)GetBonusPlatform()`, which needs a look first.
