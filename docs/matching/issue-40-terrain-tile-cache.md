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
  `sub_80254F8` constructs one from a small level/room descriptor;
  `sub_8024F24` is the lookup/decode dispatcher (16 fixed id-checks,
  each either a fixed-offset hit or, on a miss on all 16, an eviction +
  `sub_8025334` decode); `sub_8025334` is the actual RLE/delta
  token-stream decoder (literal-fill run, signed-delta-accumulate run,
  raw-copy run, budget-limited to 0x7f halfwords per record).
- **`sub_80250BC`/`sub_8025130`/`sub_8025228`/`sub_80254C0`/
  `sub_8025460`** are five siblings of the same `(x>>4, y>>3)` tile
  lookup through that cache, differing only in what they do with the
  decoded cell (bounds-check + `gStaticData_081725AC` terrain-property
  pointer; add a `mode`-selected table and output flag; add a `mode`
  dispatch returning a single flag byte from `gStaticData_081725A8`;
  return the raw halfword; return the raw byte + output params).
- **`sub_8024E68`/`sub_8024E90`/`sub_8024EB4`** are a small
  viewport/parallax-scroll-layer object built around the cache (own
  fields not given a struct here - see the doc comment at the top of
  `game_loop3.c` for why: its full shape spans into still-raw neighbor
  functions `sub_8024DFC`/`sub_8024E24`/`sub_8024C64`/`sub_8024CF0`,
  out of scope for this issue).
- **`sub_8025444`** is the exact same "`flags & 1` -> forward to
  `sub_8026ED0`" shape already matched as `sub_8006FC8` in
  `src/graphics/graphics.c`.
- **`sub_8025554`/`sub_8025588`** are a floor-divide-by-32 bitmap
  set/clear pair on an arbitrary `void *self` array - unrelated to the
  tile cache, just adjacent in ROM.
- **`sub_80255A8`/`sub_80255C4`** are a `CpuSet`-based 32-halfword
  (one palette bank) zero-fill wrapper and its return-self variant.
- **`sub_80255D4`** - left completely untouched (see "Left raw" below).

## Matched (20 functions, full clean `make compare` passing)

`src/system/game_loop3.c`: `sub_8024E68`, `sub_8024E90`, `sub_8024EB4`,
`sub_8024F04`, `sub_8024F0C`, `sub_8024F10`, `sub_8024F14`,
`sub_8024F18`, `sub_8024F1C`, `sub_8024F20`.
`src/system/game_loop4.c`: `sub_8025444`, `nullsub_4`.
`src/system/game_loop5.c`: `sub_80254C0`, `sub_80254F8`, `sub_8025554`,
`sub_8025588`, `sub_80255A8`, `sub_80255C4`.

## Parked (`NON_MATCHING`) - 5 functions

All five are fully understood (semantics, field offsets, every call and
branch topology confirmed correct against the ROM disassembly) but
don't yet produce byte-identical output from `tools/agbcc`. Real bytes
for all five live in one raw block, `asm/code_3_2_17_24f24.s`
(0x08024F24-0x08025444, contiguous - `sub_8024F24`, `sub_80250BC`,
`sub_8025130`, `sub_8025228`, `sub_8025334` in ROM order), guarded
`.if NON_MATCHING == 0`; the `#if NON_MATCHING` C reconstructions live
in `src/system/game_loop3.c` right after the matched functions.

- **`sub_8024F24`** (the 16-slot lookup dispatcher) and
  **`sub_80250BC`** (its simplest consumer): control flow, the shared
  per-case tail (ROM computes the final `self + offset` pointer once,
  in a block shared by every matching case, rather than per-case - the
  C reconstruction reproduces this via a shared `offset`/`return`
  rather than 16 independent `if (...) return ...;` statements, which
  was necessary to even get the *total size* right, not just the
  bytes) and total size all match exactly. What's left is a pure
  register-allocation permutation: this compiler assigns `self`
  and `recordId` to the opposite of the ROM's `r7`/`r3` pair throughout
  the whole function (every comparison/address-add byte differs as a
  result, despite every instruction *shape* matching one-for-one).
  Tried: swapping the first comparison's operand order (no effect on
  the allocation, only cosmetic reordering elsewhere); explicit
  `register struct tile_cache *self asm("r7")` and
  `register s32 recordId asm("r3")` pins on either variable - both
  made things *worse* (the compiler stopped using the pinned register
  as a base pointer at all and fell back to `sp`-relative addressing
  mid-function, growing the function past its real size again).
  Parked with the naturally-allocated version.
- **`sub_8025130`/`sub_8025228`/`sub_8025460`**: same
  register-allocation-permutation gap as `sub_8024F24` above (all three
  share its `self`/`x`/`y`/... parameter shape and call it internally).
  Per-mode dispatch logic, the `type <= 0x23` vs `type > 0x23`
  early-exit sense (opposite between this family and
  `sub_80250BC`/`sub_80254C0` - confirmed by reading the raw branch
  senses directly, not assumed) and the `wide = (u32)cell << 16` "widen
  before extracting sub-fields" idiom the ROM uses for the nibble/hi
  extraction (needed to get `sub_8024F24`/`sub_80250BC` themselves to
  match the ROM's shared-register CSE shape for that extraction) are
  all confirmed correct.
- **`sub_8025334`** (the RLE/delta decoder): the decode loop mechanics
  (three run-mode branches, the literal/delta/raw-copy semantics, the
  0x7f-halfword budget) are reproduced and read byte-for-byte off the
  ROM disassembly, and control flow matches. The ROM keeps a
  "bytes-written" offset alive across the whole function in `r7`,
  using it *directly* as a write pointer in two of the three run modes
  (not just as a counter), and does not hoist the repeated
  `0x8000`/`0x4000` bit-test masks out of the outer loop; this
  compiler's natural allocation for the equivalent index-based
  reconstruction below picks a different register shape (a cached mask
  register, a plain incrementing index instead of a shared pointer).
  Not yet found a source phrasing that reproduces the ROM's exact
  register/instruction shape here.

## Left raw (untouched) - 1 function

- **`sub_80255D4`** (0x08025604-0x080255D4? real span
  0x080255D4-0x08025894, ~700 B) - `docs/rom_map.md` characterized this
  as a "per-frame visible-object/window list processor": DMA-writes to
  OBJ palette RAM and a BG window register, then walks a small
  count-prefixed array touching `gUnknown_030012E4`/`gUnknown_0300130C`,
  calling several still-unread functions (`sub_8025968`, `sub_8025D28`,
  `sub_8010714`, `sub_8010710`, `sub_8007398`, `sub_801070C`). Not
  understood precisely enough (which fields of the visited records mean
  what, why two lookups happen per entry) to commit a byte-exact-attempt
  reconstruction with confidence - left completely raw rather than guess.

## Real gotchas found along the way (useful beyond this issue)

1. **A per-statement C write of the same constant can silently cost 4
   bytes total ROM size**, not just mismatch locally. `sub_8024EB4`
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
   *shape*, not size** - `sub_8024F24`'s per-case chain
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
   adds. Affected `sub_80254C0` specifically (the only one of this
   family that stayed matched rather than parked) - a 2-instruction
   reorder, same total size, so this one didn't shift the ROM checksum,
   just failed the byte-exact check locally.

## Cross-references

- `docs/status/game_loop.md` - matched/parked lists updated.
- `docs/rom_map.md` - "A new find: a custom RLE/delta token-stream
  decoder" and "More of the cluster: the frame-end flush hub, a
  `CheckTerrainFlag`-style API, and collision response" sections are
  the read-only reconnaissance this issue's matching work is based on.
