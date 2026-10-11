# Text pointers: `const u8 *` (#855)

GetUiText returned the string's address as an `s32`. The font methods took
`u8 *`, and the strings live in ROM (`gUiTextTables` is `const u8 *const
*`). So almost every text use was cast: `DrawText((u8 *)GetUiText(0x28))`,
`(u8 *)nameText` on an `s32` field, `messageLine1 = (u32)gCrash2LinkTextPtr`,
and `(u8 *)gMenuCursorText` on a `const` string. A pointer's type and its
`const` don't change the code, so every object stays the same.

## Retyped (64 casts gone)

| what | was | now |
|---|---|---|
| GetUiText | `s32` | `const u8 *` |
| Font::MeasureText, MeasureChars, DrawText, DrawChars, TextHeight | `u8 *` text | `const u8 *` |
| DrawWrappedText, DrawWrappedTextInBox, GetWordLength | `u8 *` text | `const u8 *` |
| PauseMenu::levelName, levelLabel | `void *` | `const u8 *` |
| LevelSelect::nameText | `s32` | `const u8 *` |
| PowerDialog::titleText, descText, and its constructor's parameters | `s32` | `const u8 *` |
| SaveMenu::messageLine1, messageLine2; SaveMenu::DrawMessageLines | `u32`, `s32` | `const u8 *` |
| TitleScreen::DrawMenuItem's `text` | `s32` | `const u8 *` |
| the locals that hold a GetUiText string (level_select.cpp, power_dialog_draw.cpp, the pause menu pages) | `s32`, `u8 *` | `const u8 *` |

The casts on the other strings passed to the font go too: the cursor texts,
the level select's time texts, the save menu's number buffers, the credits'
characters and the language names.

A parameter's type is part of a method's g++ 2.9 mangled name, so
cxx_symbols.txt has the new names: `MeasureText__4FontPCUc`,
`__11PowerDialogPCUcT1i`, `DrawMessageLines__8SaveMenuPCUcT1`,
`DrawMenuItem__11TitleScreenPCUci` and the others.

## Left

- PauseMenu::DrawFraction takes its two number strings as `void *` and
  casts them for DrawText. Its signature is next.
- PauseMenu::InitInfo reads the level's name id as `*(u32 *)((u8
  *)gLevelTable + levelIdx * sizeof(struct level_info))`. Its comment
  says that spelling it as the member moves the load of gLevelTable's
  address.
