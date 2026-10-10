# `SaveMenu::state` and the blink bit (#843)

The save menu (`class SaveMenu`, include/save_menu.hpp) has two fields
named here.

## `state` (+0x0C)

`state` is the screen the menu shows. SaveMenu::Input and SaveMenu::Draw
each switch on it, after a `(u32)state <= SAVE_MENU_STATE_UNK10` bound
check. The names are `SAVE_MENU_STATE_*` in include/save.h, next to
RunSaveMenu, which takes the first state as an argument.

| Value | Name | Input | Draw | Entered from |
|---|---|---|---|---|
| 0 | `MAIN` | MainInput | DrawMain | RunSaveMenu (UpdateGameFrame, on leaving the level select); the back paths of every other state |
| 1 | `LOAD` | LoadInput (cartridge) | DrawLoad | MainInput's "load game"; RunSaveMenu (UpdateGameFrame, the title screen's load) |
| 2 | `LOAD_LINK` | LoadInput (link save) | DrawLoadLink | LinkInput, when both games agree |
| 3 | `LINK_TRANSFER` | LinkInput | DrawMessage | MainInput's "load link game" |
| 4 | `MESSAGE` | MessageInput | DrawMessage | LinkInput (failed, or the other game's text) |
| 5 | `SAVE` | SaveInput | DrawSave | MainInput's "save game"; back from `CONFIRM_OVERWRITE` |
| 6 | `DELETE` | DeleteInput | DrawDelete | MainInput's "delete game"; back from `CONFIRM_DELETE` |
| 7 | `CONFIRM_DELETE` | ConfirmDeleteInput | DrawConfirmDelete | DeleteInput on a used slot |
| 8 | `UNK8` | (none) | (none) | nothing sets it |
| 9 | `CONFIRM_OVERWRITE` | OverwriteInput | DrawOverwrite | SaveInput on a used slot |
| 10 | `UNK10` | (none) | (none) | nothing sets it; it is the switches' upper bound |

The ROM gives states 8 and 10 their own (empty) cases, but nothing enters
them, so they are `_UNK<n>` (docs/naming.md).

## `flags` (+0x04): the blink

`flags` isn't a flags byte. It is a frame counter that Input advances as
`(flags + 1) & 0xff`. Its bit 2, `SAVE_MENU_BLINK_BIT`, picks the
highlighted row's palette: 1 while the bit is set, 2 while it is clear.
That is four frames on, four off.

`SaveMenu::IsBlinkOn()` returns `(flags >> SAVE_MENU_BLINK_BIT) & 1`, the
spelling every draw uses. These sites call it:

- DrawCancel;
- the `SET_HIGHLIGHT` macro;
- draw_row_mark;
- DrawEmptySlotLabel;
- GetBlinkPalette.

DrawYesNoPrompt keeps the test spelled out, with a comment: the inline call
swaps r8 and sb in the rest of that function.
