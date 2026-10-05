# Issue #27: 0x0801CEE0-0x0801DA38, graphics - level-select page turns and background layers

All 25 functions of the former `asm/code_3_2_17_188d0_1cee0.s` are now
plain C in `src/graphics/actor_part_1cee0.c`. The `.s` file is deleted.
There are no NAKED or `NON_MATCHING` functions. The shared structs are in
the new `include/level_menu.h`. Verified with a clean
`make NON_MATCHING=1 report` and a clean `make compare`
(`crashbandicootxs.gba: OK`).

**The file is compiled with `old_agbcc`.** It is on the Makefile's
`OLD_AGBCC_OBJS` list, next to `actor_part_1967c.o`. In the first draft
compiled with both compilers, the current `agbcc` got 12 functions wrong
and `old_agbcc` got 9 wrong. Every byte read-modify-write here (the
blend-register shadows, BG2CNT, the sprites' mirror/mode/palette bits)
came out constant-first under `old_agbcc` as plain bitfield C. So the
`Opaque()`/pin workarounds from #23/#25/#26 were not needed. What was
left was loop shape and evaluation order, described below.

## What the code is

This is the rest of issue #26's level-select screen (`struct level_menu`,
0xAC bytes, built by `InitLevelSelect`), plus its two background objects.

- **Page turns.** `LevelSelectPrevWorld` (Down) and `LevelSelectNextWorld` (Up) are the
  handlers `LevelSelectLoop` dispatches. If the move is allowed
  (`LevelSelectHasPrevWorld`: `world != 0`; `LevelSelectIsNextWorldOpen`: bit 5/7/6 of save byte 2
  for pages 0/1/2), each one settles the cursor (`SettleLevelSelectPage`) and
  plays 0x56/0x55. Then it loops while the key stays held: step `world`,
  move BG1's scroll target a page (`sub_801D790` +0x100 /
  `sub_801D79C` -0x100), run `LevelSelectTurnPage`, wait a frame. After the loop,
  `RefreshLevelSelectPage` switches the page-title sprite's animation
  (`gStaticData_0816C548[world]`) and puts the cursor panel back on the
  clamped cursor. A blocked move plays 0x48.
- **`LevelSelectTurnPage`** runs frames until BG1's scroll reaches its target
  (`ScrollLevelSelectPageBg` eases it 8 per frame). When the low byte of the scroll
  reaches 0xA0 (the halfway point), it reloads the page entries. This
  is `sub_801D638` (item method +0x10 `(world, slot)`),
  `sub_801D5CC` (choose the cursor layout
  `gStaticData_0816C508`/`4D8` and `lastIndex` 5/4, depending on whether
  all five levels of the page are cleared, then item method +0x18
  `(&positions[i])`) and `sub_801D668`
  (`SetLevelSelectEntryBox(item, gStaticData_0816C538[world])`), all three inlined.
- **Exits.** `LevelSelectConfirm` handles A on an open entry: sound 0x52, panel
  to (0x78, 0x35), wait for the panel and the icon layer, then fade
  (`BLDCNT` effect 3 on all first targets, `evy = t / 2`).
  `LevelSelectExit` handles Start: sound 0x49, the same fade, and
  `result = 1`. `WaitLevelSelectCursor` is the "wait for the panel" loop.
  `ReloadLevelSelectPalette` reloads palette 0xF and re-applies it to the eight
  sprites.
- **`struct page_bg`** (BG1, `CreateLevelSelectPageBg`): the `InitBgSetup`
  background descriptor, then `scroll`/`target` (Q8, starting at
  0x300) and the BG1HOFS/VOFS pair (`sub_801D7D0` returns both as one
  word, `sub_801D7D4` resets them). `DestroyLevelSelectPageBg` is its destructor
  body.
- **`struct icon_bg`** (BG2, `InitZoomBg`, 0x8C bytes): the BG2CNT
  shadow (priority 1, 256 colours), a cleared screen block with an 8x4
  block of tile entries, and four corner sprites (bank `+0x258`,
  positions `gStaticData_0816C5F0` around (0x78, 0x35), mirrored X/Y
  per corner) registered through `RandomizeZoomBgTwinkle`. The rest of this class
  is in issue #28's range.
- `SetNewWorldOpened` sets `gNewWorldOpened` (called from `game_loop55.c`).

**UNUSED:** `sub_801D698` (one frame of the screen without the menu's
update). It has no `bl`/`.4byte` reference and no Thumb pointer anywhere
in the ROM. It is matched anyway.

`actor_part_1b85c.c` still has its own copies of these structs. It was
left alone because another pass is revising that file. It can switch to
`include/level_menu.h`. The two layouts agree, except that the header
types the save block (`struct menu_save`) and the two background layers.

## Matching notes

- **Loops written with `goto`** (`LevelSelectPrevWorld`/`LevelSelectNextWorld`). The ROM
  jumps into the loop test and keeps the key test's exit inside the
  body. A `while` with `break` gives gcc's rotated loop with a duplicated
  test instead.
- **`LevelSelectHasPrevWorld`**: `world != 0` compiles to a compare and a branch.
  The ROM's `neg/orr/lsr #31` is `(-w | w) >> 31` on a `u32` copy.
- **`LevelSelectIsNextWorldOpen`**: explicit shifts (`(b >> 5) & 1`). A bitfield read
  gives `lsl/lsr`, and the ROM drops the `& 1` only for bit 7, which
  gcc does on its own.
- **`RefreshLevelSelectPage`**: an inline `SetAnim(sprite, u32 idx)`. Passing the
  table word (not a `u8`) keeps the word load, and the load happens
  after the sprite pointer, as in the ROM.
- **`sub_801D5CC`'s cleared count**: `levels[k].b.cleared`, with
  `k = world * 5 + j` in its own local and `j` a counter separate from
  the second loop's `i`. This gives the ROM's order (world*5, then load
  `save`, then *4) and its registers. A byte-wide view of the save word
  (`union level_record`) gives the `ldrb`.
- **`ReloadLevelSelectPalette`**: `self->sprites[i]` indexed twice. A pointer local
  gets strength-reduced and the loop reversed.
- **`LevelSelectTurnPage`**: `(x & 0xFF) == 0xA0` for the ROM's
  `movs #0xff; ands` (a `(u8)` cast gives `lsl/lsr`).
- **`InitZoomBg`** needed five things:
  - The tile block is written `dst[j] = v` in the inner loop with
    `dst += 8` after it. With that form, gcc's loop pass hoists the
    later field addresses into the same `sl`/`r8`/`ip`/`sb` registers
    as the ROM.
  - The slot loop indexes `self->slots[i]` directly. It passes
    `&self->slots[i]` to `RandomizeZoomBgTwinkle` instead of using a slot pointer
    local. This fixes the order of the three induction-variable
    increments and the two stack spills.
  - Field stores go through inline setters (`SetMode`/`SetFlipX`/
    `SetFlipY`/`SetPalette`/`SetPos`), as the C++ member functions would
    have. A parameter keeps the full clear-then-or for a 1-bit field
    (a literal `= 1` compiles to a bare `orr`). `SetMode`'s `s32`
    parameter makes the loop hoist the `1` into `sb`.
  - The stores run in the order `unk_28 = unk_2C = unk_30 = 0` and
    `unk_3C = unk_38 = 0x2000`, which gives the ROM's store order.
  - The `bgcnt` union and `struct sprite_f28` need
    `__attribute__((packed))`; otherwise ARM's 4-byte struct rounding
    moves every later field.
