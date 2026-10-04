# Issue #40: 0x08024E68-0x08025894 - the terrain tile-record decode cache

GitHub issue #40 (`decomp-chunk`, category `game_loop`) listed 25 raw
functions in `asm/code_3_2_17_231cc.s`. This is the write-up for the
work done against that list.

## What this cluster turned out to be

The whole chunk is one system: a **16-slot decode/LRU cache for
terrain-collision tile records**, plus a small viewport/parallax-layer
object that owns one. `docs/rom_map.md`'s "A new find: a custom
RLE/delta token-stream decoder" and "the frame-end flush hub" sections
had already characterized most of the pieces from a read-only pass; this
issue is where they got turned into (attempted) byte-exact C.

- **`struct tile_cache`** (defined identically, but not shared via a
  header, in `game_loop3.c`/`game_loop4.c`/`game_loop5.c`): 16
  256-byte decode buffers at `+0x20`, 16 resident record-IDs at
  `+0x1020`, and a ring-buffer eviction cursor at `+0x1060`.
  `SetCollisionSource` constructs one from a small level/room descriptor;
  `GetCollisionChunk` is the lookup/decode dispatcher (16 fixed id-checks,
  each either a fixed-offset hit or, on a miss on all 16, an eviction +
  `DecodeCollisionChunk` decode); `DecodeCollisionChunk` is the actual RLE/delta
  token-stream decoder (literal-fill run, signed-delta-accumulate run,
  raw-copy run, budget-limited to 0x7f halfwords per record).
- **`GetTerrainHeights`/`GetSolidTerrainHeights`/`sub_8025228`/`GetCollisionCell`/
  `GetTerrainType`** are five siblings of the same `(x>>4, y>>3)` tile
  lookup through that cache, differing only in what they do with the
  decoded cell (bounds-check + `gTerrainHeights0` terrain-property
  pointer; add a `mode`-selected table and output flag; add a `mode`
  dispatch returning a single flag byte from `gTerrainTypes`;
  return the raw byte + output params; return the raw decoded halfword
  directly plus a `hiOut` nibble).
- **`ScrollBgLayerBase`/`ResetBgLayerBase`/`SetBgLayerSource`** are a small
  viewport/parallax-scroll-layer object built around the cache (own
  fields not given a struct here - see the doc comment at the top of
  `game_loop3.c` for why: its full shape spans into still-raw neighbor
  functions `ScaleBgLayerScroll`/`sub_8024E24`/`FillBgStreamer`/`SetBgStreamerSource`,
  out of scope for this issue).
- **`sub_8025444`** is the exact same "`flags & 1` -> forward to
  `sub_8026ED0`" shape already matched as `sub_8006FC8` in
  `src/graphics/graphics.c`.
- **`sub_8025554`/`sub_8025588`** are a floor-divide-by-32 bitmap
  set/clear pair on an arbitrary `void *self` array - unrelated to the
  tile cache, just adjacent in ROM.
- **`sub_80255A8`/`sub_80255C4`** are a `CpuSet`-based 32-halfword
  (one palette bank) zero-fill wrapper and its return-self variant.
- **`SpawnRoomEntities`** - left completely untouched (see "Left raw" below).

## Matched (20 functions, full clean `make compare` passing)

`src/system/game_loop3.c`: `ScrollBgLayerBase`, `ResetBgLayerBase`, `SetBgLayerSource`,
`IsBgLayerEnabled`, `GetBgLayerY`, `GetBgLayerX`, `GetBgLayerHeightTiles`,
`GetBgLayerWidthTiles`, `GetBgLayerHeight`, `GetBgLayerWidth`.
`src/system/game_loop4.c`: `sub_8025444`, `nullsub_4`.
`src/system/game_loop5.c`: `GetCollisionCell`, `SetCollisionSource`, `sub_8025554`,
`sub_8025588`, `sub_80255A8`, `sub_80255C4`.

## Closed as NAKED - 6 functions

- **`GetCollisionChunk`** (the 16-slot lookup dispatcher): semantics,
  control flow, the shared per-case tail (ROM computes the final
  `self + offset` pointer once, in a block shared by every matching
  case, rather than per-case), and total size were all already
  confirmed correct as a real-C `#if NON_MATCHING` reconstruction (see
  git history for that version). The one remaining gap was a pure
  register-allocation permutation: this compiler always assigns `self`
  and `recordId` to the opposite of the ROM's `r7`/`r3` pair throughout
  the whole function (every comparison/address-add byte differs as a
  result, despite every instruction *shape* matching one-for-one).
  Tried: swapping the first comparison's operand order (no effect on
  the allocation, only cosmetic reordering elsewhere); explicit
  `register struct tile_cache *self asm("r7")` and
  `register s32 recordId asm("r3")` pins on either variable - both
  made things *worse* (the compiler stopped using the pinned register
  as a base pointer at all and fell back to `sp`-relative addressing
  mid-function, growing the function past its real size again). Closed
  instead by going fully `NAKED` - the same escape hatch already used
  this session for `FitScaledSprite`/`LoadGraphicsPackage`/
  `LoadTitleScreenBg` for the identical symptom - hand-transcribing
  the ROM disassembly instruction-for-instruction (including the
  `.pool` literal-pool splits at each of the ROM's own mid-function
  flush points). Verified byte-identical via isolated
  compile+`arm-none-eabi-as` assemble, a direct byte comparison against
  `baserom.gba` at `0x08024F24` (differing only in the
  as-yet-unresolved `bl DecodeCollisionChunk` relocation bytes, as expected for
  an unlinked object), and a full clean `make compare`. Its raw bytes
  no longer live in `asm/code_3_2_17_24f24.s` - that file now starts
  directly at `GetTerrainHeights`.
- **`GetTerrainHeights`/`GetSolidTerrainHeights`/`sub_8025228`** (`GetCollisionChunk`'s three
  `(x, y)`-tile-lookup consumers - a terrain-property-table pointer
  lookup, its `mode`-selected/`flagsOut`-writing sibling with 4 table
  variants, and a `mode`-dispatched single-flag-byte variant reading
  4 adjacent bytes of one table): same register-allocation-permutation
  gap `GetCollisionChunk` had - this compiler never reproduces the ROM's own
  `self`/`x`/`y`/`mode` <-> `r5`/`r4`/`r6`/`r7` register packing no
  matter the C phrasing tried. Closed the same way as `GetCollisionChunk`:
  hand-transcribed instruction-for-instruction from the ROM
  disassembly, including every one of the ROM's own mid-function
  `.pool` splits (`GetSolidTerrainHeights`/`sub_8025228` each have three inline
  literal-pool flushes, one after each of the first three dispatch
  cases, plus a fourth literal shared with the function's own trailing
  pool - `sub_8025228` in particular re-flushes the *same*
  `gTerrainTypes` symbol four separate times rather than reusing
  one pool slot, since each `ldr` is in its own already-flushed pool
  region). Verified the same way as `GetCollisionChunk`: isolated
  compile+`arm-none-eabi-as` assemble with a direct byte comparison
  against `baserom.gba` (all three came back byte-identical modulo the
  expected unresolved `bl GetCollisionChunk`/`ldr =gStaticData_...`
  relocation bytes), confirmed again against the real
  cpp|agbcc-generated `.s` for the whole file, and a full clean `make
  compare`. Their raw bytes no longer live in
  `asm/code_3_2_17_24f24.s` - that file now starts directly at
  `DecodeCollisionChunk`.
- **`DecodeCollisionChunk`** (the RLE/delta decoder `GetCollisionChunk` calls on a
  cache miss): decode-loop mechanics (the three run-mode branches, the
  literal/delta/raw-copy semantics, the 0x7f-halfword budget) and
  control flow were already fully confirmed correct as a real-C
  `#if NON_MATCHING` reconstruction. The remaining gap, once fully
  traced register-by-register against the ROM disassembly: the ROM
  keeps a "bytes-written" *byte-offset write pointer* alive across the
  whole function in `r7` (seeded from `dest` at entry, incremented in
  lockstep with the `written` counter in every mode), but it is only
  ever *dereferenced* directly in the delta-run mode's first write (the
  initial `accum` halfword) and its last write (the trailing odd delta,
  when `count` is even) - the delta-run mode's own steady-state
  two-at-a-time loop, and *every* write in both the literal-fill and
  raw-copy modes, instead recompute a fresh `dest + written*2` pointer
  from `r8`/`ip` right before storing, even though `r7` holds the exact
  identical address at that point. This is a genuine
  redundant-shadow-register artifact of whatever compiler produced the
  original ROM - not a shape a straightforward index-based C
  reconstruction can be phrased into reproducing, since C has no way to
  say "maintain this pointer in a specific register but only read it
  back at these two specific points, elsewhere always recomputing from
  a different pair of registers that happen to agree." Closed as a
  NAKED transcription - the same escape hatch as the four functions
  above - hand-transcribing the ROM disassembly instruction-for-
  instruction, including its own trailing 2-byte zero pad
  (`asm(".align 2, 0")` after the function, per the
  `matching_decomp_alignment_fix` precedent - without it,
  `arm-none-eabi-as`'s default NOP-fill alignment (`0x46c0`) produces 2
  bytes that differ from the ROM's zero-fill). Verified byte-identical
  via isolated compile + `arm-none-eabi-as` assemble, a direct byte
  comparison against `baserom.gba` at `0x08025334`, and a full clean
  `make compare`. `asm/code_3_2_17_24f24.s` is now gone entirely - it
  held only this function after `GetCollisionChunk`/`GetTerrainHeights`/
  `GetSolidTerrainHeights`/`sub_8025228` were closed, so once this one closed too
  the file's contents were empty and it (plus its `ldscript.txt` entry)
  were removed rather than kept as a zero-function husk.
- **`GetTerrainType`** (`src/system/game_loop4.c`) - the last of
  `GetCollisionChunk`'s `(x, y)`-tile-lookup consumers: returns the raw
  decoded halfword directly (no bounds check, no terrain-table lookup),
  while also writing the cell's top nibble out through `hiOut`. Same
  register-allocation-permutation gap as `GetSolidTerrainHeights`/`sub_8025228`
  above (this compiler never reproduces the ROM's own `self`/`x`/`y`/
  `flagsOut` <-> `r5`/`r4`/`r6`/`r7` register packing, no matter how the
  C is phrased). Closed the same way: hand-transcribed
  instruction-for-instruction from the ROM disassembly (formerly
  `asm/code_3_2_17_25460.s`, now deleted - confirmed via
  `arm-none-eabi-objdump -t` that it held only this one function).
  Unlike its four siblings above, this function needed no mid-function
  `.pool` directive at all - it has zero literal-pool references (no
  `ldr =symbol`), only a single `bl GetCollisionChunk` relocation, so the
  whole 96-byte body is one contiguous block with no split points.
  Verified byte-identical via isolated `arm-none-eabi-as` assembly, a
  direct byte comparison against a from-scratch assemble of
  `asm/code_3_2_17_25460.s` itself (identical except for the expected
  unresolved `bl GetCollisionChunk` relocation bytes, which matched anyway
  since both objects reference the same undefined external symbol), and
  a full clean `make compare`. **This was the last remaining unclosed
  member of the issue #40 terrain-tile-cache cluster - the whole issue
  is now closed.**

## Left raw (untouched) - 1 function

- **`SpawnRoomEntities`** (0x08025604-0x080255D4? real span
  0x080255D4-0x08025894, ~700 B) - `docs/rom_map.md` characterized this
  as a "per-frame visible-object/window list processor": DMA-writes to
  OBJ palette RAM and a BG window register, then walks a small
  count-prefixed array touching `gEntitySpawner`/`gCrateList`,
  calling several still-unread functions (`sub_8025968`, `SpawnEntity`,
  `sub_8010714`, `sub_8010710`, `sub_8007398`, `sub_801070C`). Not
  understood precisely enough (which fields of the visited records mean
  what, why two lookups happen per entry) to commit a byte-exact-attempt
  reconstruction with confidence - left completely raw rather than guess.

## Real gotchas found along the way (useful beyond this issue)

1. **A per-statement C write of the same constant can silently cost 4
   bytes total ROM size**, not just mismatch locally. `SetBgLayerSource`
   zeroes three separate fields (`self+0x28` as a byte, `self+0/+4` as
   words); writing each as a fresh `0` literal let the compiler
   re-materialize the constant into a *second* register instead of
   reusing the one from the first write (the ROM keeps it in `r3` the
   whole time) - one extra 2-byte `movs` instruction. Fixed by
   declaring one `s32 zero = 0;`/`u8 *readyFlag` pair up front and
   reusing them for all three writes, in the same statement order the
   ROM's instructions appear in (declaring-with-initializer computes
   the value immediately in program order here, so *where* the
   assignment statement sits textually matters, not just that the
   variable exists).
2. **This project's isolated per-function compile only proves
   *shape*, not size** - `GetCollisionChunk`'s per-case chain
   (`if (...) return self->buf[N];` × 16) looked byte-identical
   instruction-by-instruction against the ROM in isolation, but totaled
   48 bytes more than the real function once linked, because the ROM
   computes its final pointer once in a block shared by every matching
   case while 16 independent `return` statements each get their own
   copy of that computation. This is exactly the workflow.md warning
   about isolated compiles never being proof - here it wasn't even a
   register-content bug, it was a whole-program **size** regression
   that only showed up as a shifted checksum across the *entire* ROM
   after `make compare`, not as a local diff. Diagnosed via the
   map-file address-shift method (`crashbandicootxs.map`, per-object
   `.text` start/size compared against a from-scratch `arm-none-eabi-as`
   of the untouched raw bytes for the same span - see
   `docs/workflow.md`'s "diagnose via the map-file address-shift
   method").
3. **`((y & 7) << 4) + (x & 0xf)` as one expression vs. as two
   separately-masked locals changes codegen** even though the value is
   identical: writing the shift-then-add as one expression computes
   `y & 7` and shifts it before touching `x`, while the ROM (and the
   fix - `s32 my = y & 7; s32 mx = x & 0xf; my <<= 4; return
   cache[my + mx];`) computes *both* masks first, then shifts, then
   adds. Affected `GetCollisionCell` specifically (the only one of this
   family that stayed matched rather than parked) - a 2-instruction
   reorder, same total size, so this one didn't shift the ROM checksum,
   just failed the byte-exact check locally.

## Cross-references

- `docs/status/game_loop.md` - matched/parked lists updated.
- `docs/rom_map.md` - "A new find: a custom RLE/delta token-stream
  decoder" and "More of the cluster: the frame-end flush hub, a
  `CheckTerrainFlag`-style API, and collision response" sections are
  the read-only reconnaissance this issue's matching work is based on.

## Later pass: second near-miss sweep

`DecodeCollisionChunk` is real C (old_agbcc). `src` starts as `decodeBase`
itself, then gets advanced, so the base lives in r6 as in the ROM. The
delta run's sign extensions are written as `<< 24` / `<< 16` shifts into
an `s32`, with `acc` copied into an `s32` before the `>> 24`. That
interleaves the shift pairs the way the ROM does. An empty
`asm("" : : "r"(n))` after `n -= 2` fixes the old r4/r5 swap between
`acc` and the run counter. See [near-miss-polish-2.md](near-miss-polish-2.md).
