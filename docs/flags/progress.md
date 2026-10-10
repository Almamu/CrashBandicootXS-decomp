# `GameProgress::flags`: gems and powers (#843)

The progress block's byte at +0x02 (`class GameProgress`,
include/game_progress.hpp). The level state holds it as `progress.flags`, and
its snapshots, the save slots and the block PackSaveData returns to the menus
hold copies of it. The names are `PROGRESS_FLAG_*` (the masks) and
`PROGRESS_BIT_*` (the bit numbers) in include/constants/progress_flags.h.
The words of `levels[]`, one per level, have their own bits:
constants/level_flags.h, `LEVEL_FLAG_*`.

| Bit | Mask | Name | Set by | Read by |
|---|---|---|---|---|
| 0 | 0x01 | `RED_GEM` | Player::HandleEvent, `EVENT_RED_GEM` | SpawnRedGem, SpawnRedGemPlatform, CountGems, PauseMenu::DrawGemsPage, LevelSelect::LoadRecord |
| 1 | 0x02 | `YELLOW_GEM` | `EVENT_YELLOW_GEM` | SpawnYellowGem, SpawnYellowGemPlatform, the same counters and pages |
| 2 | 0x04 | `GREEN_GEM` | `EVENT_GREEN_GEM` | SpawnGreenGem, SpawnGreenGemPlatform, ... |
| 3 | 0x08 | `BLUE_GEM` | `EVENT_BLUE_GEM` | SpawnBlueGem, SpawnBlueGemPlatform, ... |
| 4 | 0x10 | `TURBO_RUN` | LevelState::GiveTurboRun | HasTurboRun (ACTION_STATE_TURBO_RUN), PauseMenu::DrawPowersPage, GetCompletionPercent |
| 5 | 0x20 | `SUPER_BODY_SLAM` | GiveSuperBodySlam | HasSuperBodySlam, DrawPowersPage, GetCompletionPercent; LevelSelect::IsNextWorldOpen on page 1 |
| 6 | 0x40 | `TORNADO_SPIN` | GiveTornadoSpin | HasTornadoSpin, DrawPowersPage, GetCompletionPercent; IsNextWorldOpen on page 3 |
| 7 | 0x80 | `DOUBLE_JUMP` | GiveDoubleJump | HasDoubleJump, DrawPowersPage, GetCompletionPercent; IsNextWorldOpen on page 2 |

LevelState::ClearPowers (UNUSED) clears bits 4-7.

## How the code tests the bits

Each site keeps the spelling the ROM's code needs (docs/naming.md, "Flag
bits"):

- **The mask:** `flags & PROGRESS_FLAG_RED_GEM` and `flags |=
  PROGRESS_FLAG_RED_GEM`. These are the spawners, the pause menu, the level
  select and Player::HandleEvent.
- **`TEST_BIT_SHR(flags, PROGRESS_BIT_X)`**, `(flags >> n) & 1`:
  LevelSelect::IsNextWorldOpen.
- **`TEST_BIT_SIGN(progress.flags, PROGRESS_BIT_X)`**, `(u32)(flags << (31
  - n)) >> 31`: HasTornadoSpin, HasSuperBodySlam and HasTurboRun. Written as
  `(x >> n) & 1`, they get an extra `and`.
- **`TEST_BIT_SIGN_U(bits, PROGRESS_BIT_X)`**, `((u32)bits << (31 - n)) >>
  31`: CountGems and GetCompletionPercent, on a `u8` copy of the byte.
- **`flags >> PROGRESS_BIT_DOUBLE_JUMP`:** bit 7 is the top bit, so
  HasDoubleJump and GetCompletionPercent need no mask.
- **Masks built in a register:** ClearPowers builds its masks as `s32`
  values (`~PROGRESS_FLAG_TURBO_RUN`, `~PROGRESS_FLAG_TORNADO_SPIN`). Two of
  them keep their literals: `+= 0x20` turns `~0x40` into `~0x20`, and `0x7f`
  is `~0x80` within the byte.
