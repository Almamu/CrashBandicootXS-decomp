# Issue #46, and single functions from issues #5 and #63

## Issue #46: the last five HUD functions (old_agbcc)

`FontDrawGlyph`, `InitSmallFont`, `InitLargeFont` (hud_icon_widget_85c4.c),
`FontMeasureChars` (hud_icon_widget_8890.c) and `FontMeasureText` (hud_icon_widget_8994.c)
are plain C. Four of the five only match under `old_agbcc` (agbcc is 2-80
halfwords off), and every function in the three files matches under it, so
all three move into `OLD_AGBCC_OBJS`. This is the same region as
`hud_digit_array.c` (docs/matching/game-loop-old-agbcc.md). What mattered:

- **`FontDrawGlyph`:**
  - The glyph's OAM scratch is a local `struct glyph_oam` with y, shape, x, size and tile bitfields.
  - X goes through a `SetGlyphX(oam, s32)` inline, so the `0x1ff` mask comes from the literal pool.
  - The y byte is read through a block-local pointer.
- **`InitSmallFont`/`B`:** the shared preamble is a `static inline InitIconManager`. Written in place, the ROM's recomputation of `&self->record` after the CpuSet fill is lost.
- **`FontMeasureChars`/`FontMeasureText`:**
  - Both use `switch (c)` with `case ' '` first, which gives the ROM's test and block order.
  - `FontMeasureChars` needs `u32 c`.
  - `FontMeasureText` returns `cur < maxWidth ? maxWidth : cur`.

## Issue #5: `sub_8002D44`

The SIO pump's TX fill step is plain C (under either compiler). It needs a
`struct sio_channel *ch = &s->tx` local; written as `s->tx.` throughout, gcc
keeps the first `&count` alive instead of recomputing it as the ROM does. The
old "r7 can never be pushed" note was wrong. `sub_8002E20` stays NAKED (see
its comment).

## Issue #63: `InitContinuePromptGraphics`

Previously raw asm with a `NON_MATCHING` draft; now plain C in actor_part88.c
under `old_agbcc`, and `asm/code_3_2_20_28568_c99c_31784_33ef4_3487c.s` is
gone.

- **Struct fix:** `struct fade_overlay`'s `icons` field is at +0x18, not +0x1C.
- **Seeding loop:** it indexes the four `gStaticData_0817C5xx` arrays directly with an up-counting `i`. With explicit pointer increments, old_agbcc's loop pass reverses `i` into a down-counter; that reversal, not an r7 bug, is what parked it.

`ContinuePromptLoop` stays NAKED.

## Comment-only updates

- **Issue #24:** the NAKED `sub_801A03C` and `sub_801A114` (actor_part_1967c.c) now describe their remaining gaps under old_agbcc (26 and 139 halfwords, register allocation).
- **Issue #56:** so do `LoadBgPicture` and `FillBgPictureMap` (actor_part45d.c).
