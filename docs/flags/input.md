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
