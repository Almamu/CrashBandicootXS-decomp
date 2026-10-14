# The text modes: wrapped text and the credits (#843)

## DrawWrappedText's `mode`, `WRAPPED_TEXT_MODE_*`

DrawWrappedText (src/engine/text/wrapped_text.cpp) and DrawWrappedTextInBox
(text_box.cpp) take a `mode` that says when the drawn words reach the
screen. The names are in include/engine/text/text.h, next to the prototypes.

| Value | Name | What DrawWrappedText does | Callers |
|---|---|---|---|
| 0 | `BUFFERED` | draws into the OAM buffer and leaves it: no Reset/HideUnused before, no WaitForVBlank/Commit after | PowerDialog::Draw (power_dialog_draw.cpp), which builds the whole dialog in the buffer |
| 1 | `PER_WORD` | resets the OAM buffer, then commits after every word (the text types out) and once at the end | CutscenePlayer::Run (cutscene_player.cpp), the cutscene captions |
| 2 | `PER_LINE` | the same, but commits only after a line wrap, and at the end | none in the ROM |

## The credits, `CREDITS_OP_*` and `CREDITS_POPUP_MODE_*`

Credits::UpdateText (src/game/ui/credits.cpp) reads gCreditsText
(src/data/credits_17c5d0.cpp) a line at a time. Three bytes are opcodes;
the data spells them with its own string macros. The names are in
include/game/ui/frontend.hpp, before `struct CreditsPopup`.

| Byte | Name | Data macro | Meaning |
|---|---|---|---|
| 1 | `CREDITS_OP_PICTURE` | `CREDITS_PICTURE_0`..`_4` | the next byte is a logo's index: a logo popup |
| 2 | `CREDITS_OP_SMALL` | `CREDITS_SMALL` | the text that follows is in gSmallFont |
| 3 | `CREDITS_OP_LARGE` | `CREDITS_LARGE` | ... in gLargeFont |

Each character or logo becomes a CreditsPopup, whose `mode` Credits::DrawText
switches on:

| Value | Name | Drawn as |
|---|---|---|
| 0 | `CREDITS_POPUP_MODE_SMALL_CHAR` | a glyph of gSmallFont |
| 1 | `CREDITS_POPUP_MODE_LARGE_CHAR` | a glyph of gLargeFont |
| 2 | `CREDITS_POPUP_MODE_LOGO` | the logo's 32x32 OAM cells |

A character popup's `mode` is copied from Credits' `largeFont` (0 small,
1 large), a 0/1 toggle that stays a number. The small-font opcode stores
`height`, still 0 at that point, as the ROM does; it has a comment.
