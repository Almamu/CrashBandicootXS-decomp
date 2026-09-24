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

## Closed as NAKED - 5 functions

- **`sub_8024F24`** (the 16-slot lookup dispatcher): semantics,
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
  this session for `sub_801E688`/`LoadGraphicsPackage`/
  `LoadBg2Background` for the identical symptom - hand-transcribing
  the ROM disassembly instruction-for-instruction (including the
  `.pool` literal-pool splits at each of the ROM's own mid-function
  flush points). Verified byte-identical via isolated
  compile+`arm-none-eabi-as` assemble, a direct byte comparison against
  `baserom.gba` at `0x08024F24` (differing only in the
  as-yet-unresolved `bl sub_8025334` relocation bytes, as expected for
  an unlinked object), and a full clean `make compare`. Its raw bytes
  no longer live in `asm/code_3_2_17_24f24.s` - that file now starts
  directly at `sub_80250BC`.
- **`sub_80250BC`/`sub_8025130`/`sub_8025228`** (`sub_8024F24`'s three
  `(x, y)`-tile-lookup consumers - a terrain-property-table pointer
  lookup, its `mode`-selected/`flagsOut`-writing sibling with 4 table
  variants, and a `mode`-dispatched single-flag-byte variant reading
  4 adjacent bytes of one table): same register-allocation-permutation
  gap `sub_8024F24` had - this compiler never reproduces the ROM's own
  `self`/`x`/`y`/`mode` <-> `r5`/`r4`/`r6`/`r7` register packing no
  matter the C phrasing tried. Closed the same way as `sub_8024F24`:
  hand-transcribed instruction-for-instruction from the ROM
  disassembly, including every one of the ROM's own mid-function
  `.pool` splits (`sub_8025130`/`sub_8025228` each have three inline
  literal-pool flushes, one after each of the first three dispatch
  cases, plus a fourth literal shared with the function's own trailing
  pool - `sub_8025228` in particular re-flushes the *same*
  `gStaticData_081725A8` symbol four separate times rather than reusing
  one pool slot, since each `ldr` is in its own already-flushed pool
  region). Verified the same way as `sub_8024F24`: isolated
  compile+`arm-none-eabi-as` assemble with a direct byte comparison
  against `baserom.gba` (all three came back byte-identical modulo the
  expected unresolved `bl sub_8024F24`/`ldr =gStaticData_...`
  relocation bytes), confirmed again against the real
  cpp|agbcc-generated `.s` for the whole file, and a full clean `make
  compare`. Their raw bytes no longer live in
  `asm/code_3_2_17_24f24.s` - that file now starts directly at
  `sub_8025334`.
- **`sub_8025334`** (the RLE/delta decoder `sub_8024F24` calls on a
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
  held only this function after `sub_8024F24`/`sub_80250BC`/
  `sub_8025130`/`sub_8025228` were closed, so once this one closed too
  the file's contents were empty and it (plus its `ldscript.txt` entry)
  were removed rather than kept as a zero-function husk.

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
