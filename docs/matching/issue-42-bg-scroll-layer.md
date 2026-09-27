# Issue #42: 0x08025FC8-0x08026418, game_loop - the BG-scroll layer's methods

All of `asm/code_3_2_17_25fc8.s` is now `src/system/bg_scroll_layer_25fc8.c`:
26 functions, all byte-exact plain C. Nothing parked, nothing left raw, no
inline asm.

The issue listed 25 names. The old disassembly had two labelling errors in
the middle of the file:

- `0x0802612C` had no label. It followed `sub_8026108`'s `bx lr` and a
  padding halfword, so the listing folded it into `sub_8026108`. It's a real
  function (`col % 32`), now `sub_802612C`.
- `sub_802613E` really starts at `0x0802613C`. Its first instruction
  (`adds r0, r1, #0`) sat on the line above the label. Now `sub_802613C`, a
  byte-identical twin of `sub_802612C`.

No code or data references either address, so the names only appear in the
docs.

## Compiler: old_agbcc

This range was built with `tools/agbcc/bin/old_agbcc`. The object is on
the Makefile's `OLD_AGBCC_OBJS` list. The BGnCNT bitfield setters
(`sub_802614C`, `sub_8026160`, `sub_8026174`, `sub_8026190`) show the
tell: `movs r2, #-K` (`movs`/`negs`) comes *before* the `ldrb` it's
`and`ed with. With the final C, the current agbcc misses 7 of the 26:
those four, plus `sub_80260B4`, `sub_80260D4` and `sub_80263F8`.
old_agbcc matches all 26.

The #43 code right after (`tile_slot_pool.c`) was matched with the current
agbcc. That needed a constant-before-load inline-asm anchor in
`sub_80264F8`, plus one for the `+0x34` update in `sub_8026448`. Those are
the same old_agbcc symptoms, so that file may really be old_agbcc too. I
haven't tested it; it's out of scope here.

## The object

`include/bg_scroll_layer.h` (new) gives the layer a struct:
`struct bg_scroll_layer` (0x5C bytes, constructor `sub_8025D74`) and
`struct pooled_bg_layer` (0x60 bytes, adds the tile-slot pool at `+0x5C`,
constructor `sub_8026448`). It also has the method table, whose entries are
`{s16 this-adjustment, pad, fn}` and are called through
`sub_803AD7C`/`sub_803AD80`.

| offset | field |
|---|---|
| 0x00/0x04 | pixel position |
| 0x28 | enabled |
| 0x2C | tile-map ring-buffer streamer (game_loop57.c) |
| 0x30 | method table |
| 0x34 | BGnCNT shadow: priority:2, charBase:2, bits 4-6, colors256:1, screenBase:5, bits 13-15 (a `u16`/bitfield union, which pads to 4 bytes) |
| 0x38 | `&REG_BGnCNT` |
| 0x3C/0x40 | resident tile rows lo..hi |
| 0x44/0x48 | resident tile columns lo..hi |
| 0x4C | BG screen block (32x32 entries) |
| 0x50 | tile asset |
| 0x54/0x56 | HOFS/VOFS |
| 0x58 | `&REG_BGnHOFS` |

Method tables (`gStaticData_087E4C14` base / `gStaticData_087E4C64` layer 0):

| slot | base | layer 0 |
|---|---|---|
| 0x08 destroy | `sub_80261B8` | `sub_8026418` |
| 0x10 reset | `sub_802608C` | `sub_80263DC` |
| 0x18 | `sub_8025E98` | `sub_8025E98` |
| 0x20 | `sub_8024DCC` | `sub_8026250` (clamp to [-8, 8]) |
| 0x28 load tiles | `sub_80260B4` | `sub_80263F8` |
| 0x30 draw row | `sub_8025FC8` | `sub_8026368` |
| 0x38 draw column | `sub_8025F3C` | `sub_80261CC` |
| 0x40 clip columns | `sub_8025E70` | `sub_80262E8` |
| 0x48 clip rows | `sub_8025E84` | `sub_8026328` |

The base layer copies map entries into the screen block. Layer 0 acquires
a pool slot for each tile (`sub_80264F8`) and writes the returned map
entry. When the resident range shrinks, layer 0 also releases the rows and
columns that scroll out (`sub_8026264`/`sub_80262A4`, driven by
`sub_80262E8`/`sub_8026328`). Its `loadTiles` doesn't unpack anything. It
just points the pool at the character block and the asset's tile data.

`sub_80260D4` is the per-layer level-load hook that `level_layers.c` calls.
`sub_802602C` derives the resident ranges from the scroll position (a
240x160 screen) and draws every row through the table.

Unused (no caller, not in a method table): `sub_8026108`, `sub_802612C`,
`sub_802613C`, `sub_802614C`-`sub_8026190` (BGnCNT setters/getter),
`sub_80261A8`, `sub_80261B0` and `nullsub_26`.

## Matching notes

- **`Mod32` inline.** Writing `% 32` directly in `sub_8026108`, `sub_80261CC`
  and `sub_8025FC8` puts the wrong register or order on the first
  modulo. A `static inline s32 Mod32(s32)` fixes all three. (With
  `sub_802612C`/`sub_802613C` sitting right there, the original likely
  called a small inline helper.)
- **`sub_8026368`**: the ROM computes `c % 32` before the
  `sub_80264F8` call, which keeps `dst` in `r8`. Doing it in a separate
  `s32 i = Mod32(c);` statement gives that order.
- **`sub_80260D4`**: `self->cnt.bits.priority = desc->cnt` narrows the
  `u16` load to `ldrb`. A `u16` temp keeps the ROM's `ldrh`.
- **`sub_80263F8`**: loading `tileData` into a temp before the call gives
  the ROM's argument-evaluation order.
- **`sub_8025FC8` - goto loop.** The ROM loads and stores the
  stack-resident column cursor on every iteration. Every `for`/`while`/
  `do`-`while` form (volatile, arrays, unions, pointer temps,
  inline helpers) lets loop.c's `load_mems` keep the cursor in a register
  and store it once after the loop. load_mems only runs on loops with
  `NOTE_INSN_LOOP_BEG` notes, so a hand-rolled `if (...) return; loop:
  ... if (c <= end) goto loop;` avoids it. Explicit `end = colHi` and
  `mask = 0x3F` locals give the ROM's hoisted `r4`/`r7`. (The sibling
  `sub_8025F3C`, NAKED in game_loop16.c, does hoist its cursor, so the
  goto is specific to this function.)
- Trailing `asm(".align 2, 0")` for the `0000` pad after `nullsub_26`.

Verified with a full clean `rm -rf build && make NON_MATCHING=1 report`
(no warnings from the new file) and `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`
(`crashbandicootxs.gba: OK`).
