# Input bits: the KEYINPUT buttons (#843)

The game reads the buttons as KEYINPUT bits, and include/gba/io_reg.h already
names them: `A_BUTTON` (1), `B_BUTTON` (2), `SELECT_BUTTON` (4),
`START_BUTTON` (8), `DPAD_*` (0x10-0x80), `R_BUTTON` (0x100) and `L_BUTTON`
(0x200). Each test now uses these names instead of the bit's value. The
buttons are hardware facts, but the code also shows what each one does:
A jumps (and fires), B spins or slides, START confirms or pauses, and
`A_BUTTON | START_BUTTON` skips a screen.

## Where the input word comes from

| Word | Layout | Read by |
|---|---|---|
| `gKeys` (`union key_state`, globals.h) | `half.held` / `half.pressed`, or `all` with held in the low half and newly pressed in the high half | the title, credits and starfield (`half.pressed & (A_BUTTON \| START_BUTTON)`), the polar vehicle, InitActorCategory (`all & SELECT_BUTTON`, `(all >> 16) & START_BUTTON`), LevelProgress::RunRoom (`half.pressed & START_BUTTON` pauses, a held `all & SELECT_BUTTON` shows the HUD counters), the save menu's link exchange (`all & B_BUTTON`) |
| ActionCtrl's `in` (action_obj.h) | a copy of `gKeys`: `INPUT_HELD(in)` and `INPUT_PRESSED(in)` | ActionCtrl's states: `INPUT_PRESSED(in) & A_BUTTON` jumps, `& B_BUTTON` spins or slides |
| WaitForKeyPress's `keys`, the jetpack's `keys.held` | the pressed or held bits | `A_BUTTON`, `START_BUTTON` |

The `(u16)(gKeys.all & B_BUTTON)` casts are part of the matching spelling
(save_menu_draw.cpp, polar_player.cpp), so they stay.

## The D-pad as one direction, `KEY_DIR_*`

KeyInput::GetDpadDirection (src/system/key_input.cpp) turns the held
D-pad bits into an index (`KEY_DIR_INDEX_UP` 1, `_DOWN` 2, `_LEFT` 4,
`_RIGHT` 8) and looks it up in gDpadDirectionTable. The result is one
direction; with opposite buttons held, down beats up and right beats
left. The names are in include/key_input.hpp.

| Value | Name | Used as |
|---|---|---|
| 0 | `KEY_DIR_NONE` | no D-pad: StateIdle releases the D-pad lock and lets a slippery floor slide on, StateRun goes idle, StateCrawl stops; SwimCtrl::StateSwim slows down, and StateSpin picks what follows the spin |
| 1 | `KEY_DIR_UP` | StateCrawl stands up when there is room |
| 2 | `KEY_DIR_DOWN` | crouch: StateIdle, StateRun and StateBodySlamLand (the last two also on `DOWN_LEFT`/`DOWN_RIGHT`); StateCrawl stops on it |
| 3, 4 | `KEY_DIR_LEFT`, `KEY_DIR_RIGHT` | StateSlide and EndSpin start the run, SteerSpin steers; StateIdle runs on any of 3-8 |
| 5, 6 | `KEY_DIR_UP_LEFT`, `KEY_DIR_UP_RIGHT` | only in the turn tests and StateIdle's 3-8 |
| 7, 8 | `KEY_DIR_DOWN_LEFT`, `KEY_DIR_DOWN_RIGHT` | crouch (above); StateCrouch starts the crawl on `LEFT`, `RIGHT`, `DOWN_LEFT` or `DOWN_RIGHT` |

`<= KEY_DIR_DOWN` is "nothing sideways" (StateJump, HandleAirInput,
SteerSpin). The turn tests (ActionCtrl::UpdateFacing and StateCrouch,
SwimCtrl::CheckTurn) compare with the three right or the three left
directions.

ActionCtrl::SteerSpin's and EndSpin's `mode` is this direction (StateSpin
passes it).

A few sites use the 0 or 1 in `dir` as a plain value after testing it
(StateIdle's `dpadLockTimer = dir`, StateRun's `QueueX(dir, 1, dir)`,
StateCrawl's `QueueX(busy, dir, busy)`, SwimCtrl::StateSwim's
`mode = dir`), as the ROM reuses the register; they keep that, with a
comment where it isn't obvious.
