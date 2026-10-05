# Issue #42: 0x08025FC8-0x08026418, game_loop - the BG-scroll layer's methods

All of `asm/code_3_2_17_25fc8.s` is now `src/system/bg_scroll_layer_25fc8.c`:
26 functions, all byte-exact plain C. Nothing parked, nothing left raw, no
inline asm.

The issue listed 25 names. The old disassembly had two labelling errors in
the middle of the file:

- `0x0802612C` had no label. It followed `GetBgLayerScreenIndex`'s `bx lr` and a
  padding halfword, so the listing folded it into `GetBgLayerScreenIndex`. It's a real
  function (`col % 32`), now `sub_802612C`.
- `sub_802613E` really starts at `0x0802613C`. Its first instruction
  (`adds r0, r1, #0`) sat on the line above the label. Now `sub_802613C`, a
  byte-identical twin of `sub_802612C`.

No code or data references either address, so the names only appear in the
docs.

## Compiler: old_agbcc

This range was built with `tools/agbcc/bin/old_agbcc`. The object is on
the Makefile's `OLD_AGBCC_OBJS` list. The BGnCNT bitfield setters
(`SetBgLayerScreenBase`, `SetBgLayerPriority`, `SetBgLayerColors256`, `SetBgLayerCharBase`) show the
tell: `movs r2, #-K` (`movs`/`negs`) comes *before* the `ldrb` it's
`and`ed with. With the final C, the current agbcc misses 7 of the 26:
those four, plus `LoadBgLayerTiles`, `LoadBgLayer` and `LoadPooledBgLayerTiles`.
old_agbcc matches all 26.

The #43 code right after (`tile_slot_pool.c`) was matched with the current
agbcc. That needed a constant-before-load inline-asm anchor in
`AcquireTileSlot`, plus one for the `+0x34` update in `InitPooledBgLayer`. Those are
the same old_agbcc symptoms, so that file may really be old_agbcc too. I
haven't tested it; it's out of scope here.

## The object

`include/bg_scroll_layer.h` (new) gives the layer a struct:
`struct bg_scroll_layer` (0x5C bytes, constructor `InitBgLayer`) and
`struct pooled_bg_layer` (0x60 bytes, adds the tile-slot pool at `+0x5C`,
constructor `InitPooledBgLayer`). It also has the method table, whose entries are
`{s16 this-adjustment, pad, fn}` and are called through
`_call_via_r1`/`_call_via_r2`.

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

Method tables (`gBgLayerVtable` base / `gPooledBgLayerVtable` layer 0):

| slot | base | layer 0 |
|---|---|---|
| 0x08 destroy | `DestroyBgLayer` | `DestroyPooledBgLayer` |
| 0x10 reset | `ResetBgLayer` | `ResetPooledBgLayer` |
| 0x18 | `ScrollBgLayer` | `ScrollBgLayer` |
| 0x20 | `ClampBgLayerScrollStep` | `ClampPooledBgLayerScrollStep` (clamp to [-8, 8]) |
| 0x28 load tiles | `LoadBgLayerTiles` | `LoadPooledBgLayerTiles` |
| 0x30 draw row | `DrawBgLayerRow` | `DrawPooledBgLayerRow` |
| 0x38 draw column | `DrawBgLayerColumn` | `DrawPooledBgLayerColumn` |
| 0x40 clip columns | `ClipBgLayerColumns` | `ClipPooledBgLayerColumns` |
| 0x48 clip rows | `ClipBgLayerRows` | `ClipPooledBgLayerRows` |

The base layer copies map entries into the screen block. Layer 0 acquires
a pool slot for each tile (`AcquireTileSlot`) and writes the returned map
entry. When the resident range shrinks, layer 0 also releases the rows and
columns that scroll out (`ReleasePooledBgLayerColumn`/`ReleasePooledBgLayerRow`, driven by
`ClipPooledBgLayerColumns`/`ClipPooledBgLayerRows`). Its `loadTiles` doesn't unpack anything. It
just points the pool at the character block and the asset's tile data.

`LoadBgLayer` is the per-layer level-load hook that `level_layers.c` calls.
`RedrawBgLayer` derives the resident ranges from the scroll position (a
240x160 screen) and draws every row through the table.

Unused (no caller, not in a method table): `GetBgLayerScreenIndex`, `sub_802612C`,
`sub_802613C`, `SetBgLayerScreenBase`-`SetBgLayerCharBase` (BGnCNT setters/getter),
`WriteBgLayerOffsetRegs`, `WriteBgLayerCntReg` and `nullsub_26`.

## Matching notes

- **`Mod32` inline.** Writing `% 32` directly in `GetBgLayerScreenIndex`, `DrawPooledBgLayerColumn`
  and `DrawBgLayerRow` puts the wrong register or order on the first
  modulo. A `static inline s32 Mod32(s32)` fixes all three. (With
  `sub_802612C`/`sub_802613C` sitting right there, the original likely
  called a small inline helper.)
- **`DrawPooledBgLayerRow`**: the ROM computes `c % 32` before the
  `AcquireTileSlot` call, which keeps `dst` in `r8`. Doing it in a separate
  `s32 i = Mod32(c);` statement gives that order.
- **`LoadBgLayer`**: `self->cnt.bits.priority = desc->cnt` narrows the
  `u16` load to `ldrb`. A `u16` temp keeps the ROM's `ldrh`.
- **`LoadPooledBgLayerTiles`**: loading `tileData` into a temp before the call gives
  the ROM's argument-evaluation order.
- **`DrawBgLayerRow` - goto loop.** The ROM loads and stores the
  stack-resident column cursor on every iteration. Every `for`/`while`/
  `do`-`while` form (volatile, arrays, unions, pointer temps,
  inline helpers) lets loop.c's `load_mems` keep the cursor in a register
  and store it once after the loop. load_mems only runs on loops with
  `NOTE_INSN_LOOP_BEG` notes, so a hand-rolled `if (...) return; loop:
  ... if (c <= end) goto loop;` avoids it. Explicit `end = colHi` and
  `mask = 0x3F` locals give the ROM's hoisted `r4`/`r7`. (The sibling
  `DrawBgLayerColumn`, NAKED in game_loop16.c, does hoist its cursor, so the
  goto is specific to this function.)
- Trailing `asm(".align 2, 0")` for the `0000` pad after `nullsub_26`.

Verified with a full clean `rm -rf build && make NON_MATCHING=1 report`
(no warnings from the new file) and `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`
(`crashbandicootxs.gba: OK`).
