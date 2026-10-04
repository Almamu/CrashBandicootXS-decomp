# Issue #43: 0x08026418-0x08026A18, game_loop - the level-layers singleton

`sub_8026628`, `sub_8026A18`, `sub_8026AE8`, `sub_8026BC0`, `sub_8026BF8`
and `sub_8026C3C` were already matched before this pass. Of the remaining
19 functions, all 19 are byte-exact matched as C (three of them with
narrow inline-asm anchors, described below). Nothing parked, nothing left
raw.

- `src/system/level_layers.c` - 11 functions, the whole of the former
  `asm/code_3_2_17_266bc.s`.
- `src/system/tile_slot_pool.c` - 8 functions, the former tail of
  `asm/code_3_2_17_25fc8.s` (which now ends at `nullsub_26`, part of
  issue #42).

## The level-layers singleton (`level_layers.c`)

`gLevelLayersSingleton` is a 0x2C-byte singleton created on first use by
`GetLevelLayers` (`sub_8026EDC(0x2C)` then the constructor `InitLevelLayers`).
It's the same object `gLevelLayers` points at: the camera
(`gCamera`, issue #44) clamps into its scroll fields via
`SetLevelScroll(gLevelLayers, ...)`, and `sub_8026BF8`/`sub_8026C3C`
already reach its terrain tile cache at `+0x20`.

| offset | field |
|---|---|
| 0x00/0x04 | max scroll x/y (pixels) = layer 0 width/height - 240/160 |
| 0x08/0x0C | scroll x/y (pixels) |
| 0x10 | BG layer 0 (0x60 bytes, `InitPooledBgLayer`, see below) |
| 0x14/0x18/0x1C | BG1-3 scroll layers (0x5C bytes, `InitBgLayer(.., 1..3)`) |
| 0x20 | terrain tile cache (0x1064 bytes; `nullsub_4` is its do-nothing constructor, returning the pointer it was given) |
| 0x24 | level asset (either the ROM pointer or a heap copy) |
| 0x28 | set when `+0x24` is a heap copy |
| 0x29-0x2B | cleared by the constructor, not otherwise used here |

- **`LoadRoom(self, args)`** - level load. `args` is `{palette
  source, level descriptor}`. If the descriptor's `+0x18` flag is set,
  the asset at `+0x14` is unpacked (`LoadTaggedAsset`) into an EWRAM
  buffer sized from its header word `>> 8`; otherwise it's referenced in
  place. Layer 0, the tile cache and BG1-3 each get their data pointer
  from the descriptor (`LoadBgLayer`, `SetCollisionSource`); `ShowBg0` is
  called after layer 0, and `ShowBg1`/`B0`/`A0` after each of BG1-3
  that's enabled. Then
  `SpawnRoomEntities(gEntityFlags, ...)` and a DMA3 copy of 0x100 halfwords
  to BG palette RAM, clearing color 0.
- **`DestroyLevelLayers(self, flags)`** - destructor. Frees the asset if one is
  set, destroys each layer through its method table's `destroy` entry
  (called with 3), the tile cache via `sub_8025444(.., 3)`, clears
  `gLevelLayersSingleton`, and frees `self` when `flags & 1`.
- **`SetLevelScroll(self, x, y)`** - Q8 position to scroll: clamp each axis
  to `>= 0`, `>> 8`, cap at the max scroll.
- **`ScrollLevelLayers`/`ResetLevelLayers`** - call one method (table `+0x18` /
  `+0x10`) of layer 0 with `&self->scrollX`, then the same method of each
  enabled BG1-3 layer with layer 0's position. **`CommitLevelScroll`** calls
  `CommitBgLayerScroll` on layer 0 and each enabled layer.
- **`sub_80269DC`/`sub_80269F8`** - byte-identical predicates: `0` if
  `arg1`, `*arg2` and `arg3` are all nonzero, else `1`.
  **`sub_8026A14`** returns 0.

The layer method-table entries are `{s16 this-adjustment, pad, function
pointer}`, dispatched through `_call_via_r2(self + adjustment, arg, fn)`.
They're modelled as a local `struct layer_method` rather than reusing
`icon_slot`, since the objects are unrelated even though the shape is the same.

Only `SetLevelScroll` needed a change from the first attempt: using one
temp for both axes put the second axis's value in r3 instead of the
ROM's r1; separate `sx`/`sy` temps fix it.

## Layer 0 and the tile-slot pool (`tile_slot_pool.c`)

Layer 0 extends a BG-scroll layer with a pool pointer at `+0x5C`.
`InitPooledBgLayer` runs the base constructor, installs its own method table
(`gPooledBgLayerVtable`), sets `+0x34` bit 7, clears bits 2-3, and
allocates the pool; `DestroyPooledBgLayer` frees the pool, restores the base table
(`gBgLayerVtable`) and chains to `DestroyBgLayerBase` - a C++-style
destructor chain. `sub_8026480` reads `+0x34` bits 0-1.

The pool (0x480C bytes) maps up to 0x2000 source tiles onto 0x200
reference-counted VRAM tile slots: VRAM base and source base, `u16
refCount[0x200]`, `u16 slotForTile[0x2000]` (0x200 = not resident), a
`u16` free-slot stack and its top index. `ResetTileSlotPool` resets it,
`AcquireTileSlot` acquires (on a miss: pop a slot, record it, queue a
64-byte 8bpp tile DMA via `UploadTileSlot`; then bump the count and return
a BG map entry with the input's top two bits moved into bits 10-11, the
flip bits), `ReleaseTileSlot` releases, and `SetTileSlotPoolSource` points the pool at
character base block `n` and a source.

### Matching notes

- **Static-inline helpers.** The ROM recomputes `pool + 0x4808`
  (`freeTop`) inside `ResetTileSlotPool`'s first loop instead of reusing the
  register from the initializing store just above it, and similarly
  recomputes `pool + 0x408 + id*2` for later `slotForTile` accesses. No
  plain-C loop or statement shape tried (while / do-while / separate
  decrement / local pointer / raw-offset store / `u16` counter) does
  that; routing the push through a `static inline PushFreeSlot()` does,
  byte-exact, because the inlined copy of `pool` defeats CSE. The same
  idea (`PopFreeSlot`, `GetTileSlot`, `SetTileSlot`, `ClearTileSlot`)
  closes most of `ReleaseTileSlot`/`AcquireTileSlot`.
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
- **`+0x34` update in `InitPooledBgLayer`** - the ROM keeps a redundant
  `& 0x7f` before `| 0x80` and derives the `-0xd` mask as `0x80 - 0x8d`
  from the same register; this compiler always folds both. Closed with a
  narrow inline-asm block, same as `InitBgLayer`'s own `+0x34`/`+0x35`
  updates (`game_loop15.c`, docs/matching/naked-sub_8025d74-matched.md).
- **Trailing `asm(".align 2, 0")`** on `tile_slot_pool.c` (caught by the
  full build as a 2-byte `46C0` vs `0000` pad at `0x08026626`).

### `AcquireTileSlot` - two narrow inline-asm anchors

With all of the above, plain C gets within one instruction swap: the
ROM materializes `0x200` (`mov r1, #0x80; lsl r1, r1, #2`) *before* the
`ldrh r0, [r0]` of `slotForTile[id]` it's compared against, and this
compiler always loads a comparison's memory operand first (`-fforce-mem`
at `-O2`). Plain-C attempts that didn't work: comparison operand order and
constant type (`0x200 != cur`, `(u16)`, `0x200u`, a `sizeof`-derived
count), arithmetic/`switch`/`goto` forms of the test, unpinned / `s32` /
r0-pinned `cur`, a pinned constant before or after the load, `volatile`
(breaks the other functions), pointer locals for the entry (lets CSE drop
the hit path's reload, which the ROM keeps), and inline helpers taking
the entry address and constant as arguments (gets the constant order
right but re-associates the address as `(id*2 + 0x408) + pool`).

What closes it: an asm block containing just the constant and the load,
taking the entry as an `"m"` operand so the compiler still computes the
address itself (in the ROM's `(pool + 0x408) + id*2` order), with `cur`/
`none` pinned to r0/r1 for the following `cmp r0, r1`. That leaves the
refcount update: plain `pool->refCount[slot]++` swaps r0/r1 against the
ROM, and pinning the new count to r1 (the `ReleaseTileSlot` fix) swaps r4/r5
across the whole function once the first asm block is present. A second
three-instruction block for the update (`ldrh`/`add #1`/`strh` on a
`"+m"` operand, unpinned `"=&l"` temp) closes both.

Verified with a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`crashbandicootxs.gba: OK`).
