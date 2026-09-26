# Issue #43: 0x08026418-0x08026A18, game_loop - the level-layers singleton

`sub_8026628`, `sub_8026A18`, `sub_8026AE8`, `sub_8026BC0`, `sub_8026BF8`
and `sub_8026C3C` were already matched before this pass. Of the remaining
19 functions, 18 are byte-exact matched as C and one (`sub_80264F8`) is
parked under `NON_MATCHING` with a NAKED transcription in the matching
build. Nothing left raw.

- `src/system/level_layers.c` - 11 functions, the whole of the former
  `asm/code_3_2_17_266bc.s`.
- `src/system/tile_slot_pool.c` - 8 functions, the former tail of
  `asm/code_3_2_17_25fc8.s` (which now ends at `nullsub_26`, part of
  issue #42).

## The level-layers singleton (`level_layers.c`)

`gUnknown_0300084C` is a 0x2C-byte singleton created on first use by
`sub_80268AC` (`sub_8026EDC(0x2C)` then the constructor `sub_80267A0`).
It's the same object `gUnknown_03001308` points at: the camera
(`gUnknown_030012D4`, issue #44) clamps into its scroll fields via
`sub_80268D0(gUnknown_03001308, ...)`, and `sub_8026BF8`/`sub_8026C3C`
already reach its terrain tile cache at `+0x20`.

| offset | field |
|---|---|
| 0x00/0x04 | max scroll x/y (pixels) = layer 0 width/height - 240/160 |
| 0x08/0x0C | scroll x/y (pixels) |
| 0x10 | BG layer 0 (0x60 bytes, `sub_8026448`, see below) |
| 0x14/0x18/0x1C | BG1-3 scroll layers (0x5C bytes, `sub_8025D74(.., 1..3)`) |
| 0x20 | terrain tile cache (0x1064 bytes; `nullsub_4` is its do-nothing constructor, returning the pointer it was given) |
| 0x24 | level asset (either the ROM pointer or a heap copy) |
| 0x28 | set when `+0x24` is a heap copy |
| 0x29-0x2B | cleared by the constructor, not otherwise used here |

- **`sub_80266BC(self, args)`** - level load. `args` is `{palette
  source, level descriptor}`. If the descriptor's `+0x18` flag is set,
  the asset at `+0x14` is unpacked (`LoadTaggedAsset`) into an EWRAM
  buffer sized from its header word `>> 8`; otherwise it's referenced in
  place. Layer 0, the tile cache and BG1-3 each get their data pointer
  from the descriptor (`sub_80260D4`, `sub_80254F8`); `sub_80015D0` is
  called after layer 0, and `sub_80015C0`/`B0`/`A0` after each of BG1-3
  that's enabled. Then
  `sub_80255D4(gUnknown_030012B4, ...)` and a DMA3 copy of 0x100 halfwords
  to BG palette RAM, clearing color 0.
- **`sub_802680C(self, flags)`** - destructor. Frees the asset if one is
  set, destroys each layer through its method table's `destroy` entry
  (called with 3), the tile cache via `sub_8025444(.., 3)`, clears
  `gUnknown_0300084C`, and frees `self` when `flags & 1`.
- **`sub_80268D0(self, x, y)`** - Q8 position to scroll: clamp each axis
  to `>= 0`, `>> 8`, cap at the max scroll.
- **`sub_802692C`/`sub_8026984`** - call one method (table `+0x18` /
  `+0x10`) of layer 0 with `&self->scrollX`, then the same method of each
  enabled BG1-3 layer with layer 0's position. **`sub_80268F8`** calls
  `sub_8025F24` on layer 0 and each enabled layer.
- **`sub_80269DC`/`sub_80269F8`** - byte-identical predicates: `0` if
  `arg1`, `*arg2` and `arg3` are all nonzero, else `1`.
  **`sub_8026A14`** returns 0.

The layer method-table entries are `{s16 this-adjustment, pad, function
pointer}`, dispatched through `sub_803AD80(self + adjustment, arg, fn)`.
They're modelled as a local `struct layer_method` rather than reusing
`icon_slot`, since the objects are unrelated even though the shape is the same.

Only `sub_80268D0` needed a change from the first attempt: using one
temp for both axes put the second axis's value in r3 instead of the
ROM's r1; separate `sx`/`sy` temps fix it.

## Layer 0 and the tile-slot pool (`tile_slot_pool.c`)

Layer 0 extends a BG-scroll layer with a pool pointer at `+0x5C`.
`sub_8026448` runs the base constructor, installs its own method table
(`gStaticData_087E4C64`), sets `+0x34` bit 7, clears bits 2-3, and
allocates the pool; `sub_8026418` frees the pool, restores the base table
(`gStaticData_087E4C14`) and chains to `sub_8024D74` - a C++-style
destructor chain. `sub_8026480` reads `+0x34` bits 0-1.

The pool (0x480C bytes) maps up to 0x2000 source tiles onto 0x200
reference-counted VRAM tile slots: VRAM base and source base, `u16
refCount[0x200]`, `u16 slotForTile[0x2000]` (0x200 = not resident), a
`u16` free-slot stack and its top index. `sub_802648C` resets it,
`sub_80264F8` acquires (on a miss: pop a slot, record it, queue a
64-byte 8bpp tile DMA via `sub_80265FC`; then bump the count and return
a BG map entry with the input's top two bits moved into bits 10-11, the
flip bits), `sub_80265A0` releases, and `sub_8026618` points the pool at
character base block `n` and a source.

### Matching notes

- **Static-inline helpers.** The ROM recomputes `pool + 0x4808`
  (`freeTop`) inside `sub_802648C`'s first loop instead of reusing the
  register from the initializing store just above it, and similarly
  recomputes `pool + 0x408 + id*2` for later `slotForTile` accesses. No
  plain-C loop or statement shape tried (while / do-while / separate
  decrement / local pointer / raw-offset store / `u16` counter) does
  that; routing the push through a `static inline PushFreeSlot()` does,
  byte-exact, because the inlined copy of `pool` defeats CSE. The same
  idea (`PopFreeSlot`, `GetTileSlot`, `SetTileSlot`, `ClearTileSlot`)
  closes most of `sub_80265A0`/`sub_80264F8`.
- **Refcount updates** needed the new count in an r1-pinned temp (the
  ROM puts the element address in r0 and the value in r1; every unpinned
  form swaps them).
- **Unions for the tile reference/map entry.** The ROM holds the `u16`
  argument in `r8` as `(r8 & 0xFFFF0000) | tile` and extracts
  `lsl #18/lsr #18` (bits 0-13) and `lsl #16/lsr #30` (bits 14-15); the
  result is built the same way in `r6` (`& 0xFFFF0000 | slot`, then
  `& ~0xC00 | flip << 10`). That's gcc's code for a 4-byte union of a
  `u16` and a `u32` bitfield struct living in a register - a
  `u16`-bitfield struct gives different code.
- **`+0x34` update in `sub_8026448`** - the ROM keeps a redundant
  `& 0x7f` before `| 0x80` and derives the `-0xd` mask as `0x80 - 0x8d`
  from the same register; this compiler always folds both. Closed with a
  narrow inline-asm block, same as `sub_8025D74`'s own `+0x34`/`+0x35`
  updates (`game_loop15.c`, docs/matching/naked-sub_8025d74-matched.md).
- **Trailing `asm(".align 2, 0")`** on `tile_slot_pool.c` (caught by the
  full build as a 2-byte `46C0` vs `0000` pad at `0x08026626`).

### `sub_80264F8` - parked

With all of the above, the C differs from the ROM only in the order of
two independent instructions ahead of the residency test: the ROM
materializes `0x200` (`mov r1, #0x80; lsl r1, r1, #2`) before `ldrh r0,
[r0]` of `slotForTile[id]`, this compiler emits the load first. Tried:
comparison operand order and constant type (`0x200 != cur`, `(u16)`,
`0x200u`, a `sizeof`-derived count), unpinned / `s32` / r0-pinned
`cur`, `==` with swapped branches (different block layout), predicate
inline helpers (loses the reload in the hit path), pinning the constant
to r1 before the load (hoists it above unrelated setup), pinning the
entry address to r0 (reshuffles every callee-saved register), a
combined inline-asm constant+load (adds a zero-extend and reshuffles),
and an empty asm barrier. Parked under `#if NON_MATCHING` with the
3-instruction-diff C; the matching build uses a NAKED transcription
generated directly from the original `asm/code_3_2_17_25fc8.s` block.

Verified with a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: OK`).
