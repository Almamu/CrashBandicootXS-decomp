# GitHub issue #39: 0x08024810-0x08024E68 (25 functions, game_loop)

Issue #39 (`decomp-chunk`, category `game_loop`) listed 25 raw functions
in `asm/code_3_2_17_24810.s` - the remainder of the `UpdateGameFrame`-
`MainLoop` cluster sitting directly between the sound-channel-handle
family (`BeginSlide`-`ResetSlideshow`, GitHub issue #38,
`game_loop20.c`/`game_loop37.c`/`game_loop38.c`) and the terrain-tile
decode cache (`ScrollBgLayerBase`-`DecodeCollisionChunk`, GitHub issue #40,
`game_loop3.c`/`game_loop4.c`/`game_loop5.c`). All 25 turned out to
belong to two already-partially-documented systems from
`docs/rom_map.md`, closing both out.

## What this cluster turned out to be

### `struct SoundChannelList` gains four more functions

`ResetSlideshow` (already matched, `game_loop20.c`) constructs a
`struct SoundChannelList` (`game_loop37.c`) by zeroing `items`/`count`
and setting `toggle` to 1. This chunk adds:

- **`InitSlideshow`**: trivial wrapper - runs `ResetSlideshow`'s reset, then
  returns `self` unchanged.
- **`InitCutscenePlayer`**: extends that reset with two new fields this
  cluster introduces, `self+0x10`/`self+0x14` (a "text-paging" record
  array pointer and count - see `RunCutscenePlayer` below), both zeroed.
- **`DestroyCutscenePlayer`**: a plain two-argument forwarding trampoline to
  `DestroySlideshow` (`game_loop20.c`) - passes both its arguments through
  untouched (only `self` is ever loaded into a register; `flags` flows
  through in `r1` since the ROM never touches it).
- **`RunCutscenePlayer`**: a *second* per-frame driver loop over the same
  `self+0/self+4` items/count pair `RunSlideshow` (`game_loop37.c`)
  already drives, but interleaved with an explicit OAM-shadow-buffer
  flush (`sub_8006A90`/`sub_8006A48`/`WaitForVBlank`/`sub_8006AAC` on
  `gUnknown_03001300` - the same "HUD-icon-plus-number renderer" OAM
  pacing pattern `docs/matching.md` documents elsewhere) and a nested
  text-paging walk through the new `self+0x10` record array (each
  record `{void **strings; s32 count;}`), rendering each string via
  `sub_8000EE4` (`text_layout.c`) against an `icon_manager *` at
  `self+0x14` and a 2-word "box" at `self+0x18`/`self+0x1c`, continuing
  to the next string in the current record while a held-input mask (9,
  versus `RunSlideshow`'s 8) stays set. `self+0x24` feeds `__udivsi3`
  (value/divisor) to compute the per-call text-wrap `limit`.

### The "visual scrolling background streamer" (`DecodeLayerChunk`-`sub_8024E24`)

`docs/rom_map.md`'s "A new find: a custom RLE/delta token-stream
decoder" section and its two follow-ups ("Follow-up: resolved the
semantics by tracing callers", "The background streamer's missing
'level load' half") had already characterized this system from a
read-only pass; this issue is where it got turned into (attempted)
byte-exact C.

The object is a small viewport/parallax layer built around a **circular
4x4-block ring-buffer tilemap** (64 halfword columns x 32 rows, 0x1000
bytes total at `self+8`), fed by the same custom RLE/delta token-stream
decoder the terrain-tile cache's `DecodeCollisionChunk` (`game_loop3.c`) uses,
just writing into a 2D buffer (row stride 64 halfwords) instead of a
flat one:

- **`DecodeLayerChunk`**: the decoder itself - looks up a base pointer via
  `self+4` indexed by a halfword table, then decodes a token stream
  (budget-limited to 0x7f halfwords) with the same three run modes
  (literal-fill, signed-delta-accumulate, raw-copy) as `DecodeCollisionChunk`.
- **`ScrollBgStreamer`**: the per-frame driver. Right-shifts the world
  position by 7/6 (128/64px tile granularity) and, on each axis the
  camera has crossed a tile boundary since last call, streams in
  exactly the newly-exposed column (`StreamBgColumn`) or row
  (`StreamBgRow`) - the tile 4 ahead of the old edge when scrolling
  forward, or the tile directly behind when scrolling back - while
  advancing a mod-4 "sub-block" accumulator (`self+0x14`/`self+0x15`).
- **`StreamBgRow`/`StreamBgColumn`**: the Y-axis/X-axis incremental
  streamers - for the newly-exposed row/column, decode up to 4 tiles via
  `DecodeLayerChunk` into the ring buffer's block-addressed slot.
- **`FillBgStreamer`**: the "level load" constructor half - same tile-coord
  math as `ScrollBgStreamer`, but loops the *entire* 4x4 block grid to seed
  the ring buffer's full initial contents (rather than one row/column).
- **`GetBgStreamerColumn`/`GetBgStreamerRow`/`GetBgStreamerCell`**: convert a world pixel
  `(x, y)` into the ring buffer's wrapped `(col, row)` address -
  `GetBgStreamerColumn` returns the column-wrapped pointer and writes the row
  out by pointer, `GetBgStreamerRow` the reverse, `GetBgStreamerCell` combines both
  and returns the decoded halfword value directly (no out-param).
- **`SetBgStreamerSource`/`DestroyBgStreamer`/`InitBgStreamer`/`sub_8024D58`/
  `sub_8024D5C`/`sub_8024D60`/`sub_8024D6C`/`DestroyBgLayerBase`/`InitBgLayerBase`/
  `sub_8024DCC`/`sub_8024DE0`/`ScaleBgLayerScroll`/`sub_8024E24`** round out
  construction/accessors: `SetBgStreamerSource` stores the room/level descriptor
  and derives a "decode base" from `gLevelLayers`'s own `+0x24`
  field, the same "camera offset + source field" shape as the
  terrain-tile cache's `decodeBase`; `DestroyBgStreamer`/`InitBgStreamer`/
  `DestroyBgLayerBase`/`InitBgLayerBase` wire up two different
  `_call_via_r2`-style interworking-trampoline tables
  (`gBgStreamerVtable`/`gBgLayerBaseVtable`) for notifying a parent
  object of size/position changes, following the exact
  `self + *(s16 *)(mgr + N)`/`*(void **)(mgr + N + 4)` idiom
  `src/graphics/actor_anim.c`'s `sub_803B0F0` already established;
  `sub_8024D58`/`sub_8024D5C`/`sub_8024D60`/`sub_8024D6C` are plain
  position accessors; `sub_8024DCC` clamps to `[-0x10, 0x10]`;
  `sub_8024DE0` clamps a position pair against an upper-bound pair;
  `ScaleBgLayerScroll` applies the layer's per-axis scale (floor-dividing the
  Q8 product, matching `matching.md`'s established `+0xff`-before-`>>8`
  floor-division idiom for negative products); `sub_8024E24` is a
  two-line position-delta forward through the trampoline table,
  accumulating both axes' results back into the layer's own position.

None of this cluster's fields are given a named struct here, for the
same reason `game_loop3.c`'s own viewport/parallax-layer object isn't:
several fields are only ever written/read by functions on both sides of
this ROM range (this file, `game_loop3.c`, and still-raw neighbors like
`ScaleBgLayerScroll`/`sub_8024E24`/`FillBgStreamer`/`SetBgStreamerSource` that `game_loop3.c`'s
own header comment named before this issue closed them), so committing
a struct now risks getting a field wrong that only surfaces once the
rest is matched.

## Result: 20 real C, 5 NAKED, all 25 closed

**Matched as real C (20):** `InitSlideshow`, `DestroyCutscenePlayer`, `InitCutscenePlayer`,
`ScrollBgStreamer`, `GetBgStreamerColumn`, `GetBgStreamerRow`, `GetBgStreamerCell`, `SetBgStreamerSource`,
`DestroyBgStreamer`, `InitBgStreamer`, `sub_8024D58`, `sub_8024D5C`, `sub_8024D60`,
`sub_8024D6C`, `DestroyBgLayerBase`, `InitBgLayerBase`, `sub_8024DCC`, `sub_8024DE0`,
`ScaleBgLayerScroll`, `sub_8024E24`.

`ScrollBgStreamer` (the per-frame axis-crossing dispatcher) matched on the
first isolated-compile attempt with no register pins needed at all -
the same "confirmed the same as the just-closed sibling cluster" result
`sub_8026628` got in the terrain-streamer's own neighborhood.

`GetBgStreamerColumn`/`GetBgStreamerRow`/`GetBgStreamerCell` (the wrapped-address helpers)
each needed the `register u8 subX/subY asm("r5")`/`register s32 shifted
asm("r4")` pin recipe (load the raw sub-block byte into one fixed
register, its `<<4`/`<<3`-shifted product into a *different* fixed
register) to reproduce the ROM's own two-register relay - reusing one
register for both the load and the shift (the natural, unpinned
compilation) produces the right *value* but the wrong *register*, a
byte-for-byte mismatch despite identical semantics. `GetBgStreamerCell`
additionally needed its col/row temporaries to land in `r3` (not `r4`)
since it has one fewer live argument (`self`/`x`/`y` only, no
out-parameter) than its two siblings, freeing `r3` as scratch - the
same register got reused for a different purpose purely because of a
different argument count, not any code shape difference.

Three functions needed simple statement-order fixes once the two-word
struct convention was in the wrong order - a recurring gotcha, not a
register problem:

- `SetBgStreamerSource` needed its two `u16` loads (`source+0x1a`/`source+0x1c`)
  hoisted into separate locals *before* either store, matching the
  ROM's own "load both, then store both" grouping (writing each load
  immediately followed by its store made the compiler reuse one
  register for both, losing the ROM's two-register shape).
- `DestroyBgLayerBase` needed its `gBgLayerBaseVtable` table-pointer store
  moved *before* the `self+0x2c` child-pointer load (the reverse of the
  most natural C statement order, but matching the ROM's own
  instruction sequence).
- `sub_8024DE0` needed its second clamp's `self+0xc` load deferred into
  a nested scope (`{ s32 b = ...; ... }`) rather than declared alongside
  the first at the top of the function, so the load actually happens
  after the first clamp/store pair completes, matching the ROM.
- `GetBgStreamerColumn`/`GetBgStreamerRow` additionally needed the final `self+8`
  buffer-base load to happen *before* the column/row shift-by-2 (the
  reverse of the initially-tried order) - caught only by the direct
  byte comparison against `baserom.gba`, not by eyeballing instruction
  shapes, since both orders produce the same instruction *count* and
  *mnemonics*, just swapped.

**Trailing alignment gotcha:** `sub_8024E24` (the last function in the
file) sits at a ROM address that isn't a multiple of 4 bytes past its
own end, and the ROM's own raw block has a bare `.align 2, 0` after it
(the `matching_decomp_alignment_fix` precedent). Unlike mid-file
functions - where `agbcc` always emits its own `.align 2, 0` immediately
before the *next* function's label, incidentally padding the current
one - the *last* function in a `.c` file gets no such automatic padding,
so `arm-none-eabi-as`'s default NOP-fill (`0x46c0`) applies instead of
the ROM's zero-fill. Fixed with an explicit trailing
`asm(".align 2, 0");` after the function, same as this project's other
trailing-pad cases.

**Closed as NAKED (5):** `RunCutscenePlayer`, `DecodeLayerChunk`, `StreamBgRow`,
`StreamBgColumn`, `FillBgStreamer` - all hit the same gcc-2.9
register-allocation-permutation/redundant-shadow-register class of gap
already exhaustively documented for the neighboring terrain-tile-cache
cluster (`GetCollisionChunk`/`GetTerrainHeights`/`GetSolidTerrainHeights`/`sub_8025228`/
`DecodeCollisionChunk`, `game_loop3.c`, GitHub issue #40): each packs several
persistent values (a running linear tile/byte index, a loop counter, a
budget counter, fixed mask constants) into a specific fixed register
(`r6`/`r7`/`r8`/`ip`/`sb`/`sl`) simultaneously live across one or more
calls to `DecodeLayerChunk`, and this compiler's allocator never reproduces
that exact packing no matter how the C is phrased - the same symptom,
not a new one. Hand-transcribed instruction-for-instruction from the
ROM disassembly instead, following the exact same NAKED-transcription
convention (numbered local labels, register names spelled `sb`/`r8`/`ip`
verbatim, explicit trailing `.align 2, 0` where the ROM's own raw block
has one).

`RunCutscenePlayer` in particular is the largest NAKED transcription in this
pass (~284 B) - its control flow (an outer per-item loop, an OAM-shadow
flush, a two-level nested text-paging loop with two different exit
conditions) was fully confirmed correct via an isolated plain-C
reconstruction first (every branch and call matched the ROM
one-for-one), but `self`/the running item index/the held-input flag
byte never landed in the ROM's own `r4`/`r8`/`r7` triple simultaneously
across the seven distinct calls this loop makes.

## Verification

Every function's plain-C or NAKED translation was first checked in
isolation (`arm-none-eabi-cpp` + `agbcc`, or for NAKED functions,
`arm-none-eabi-as` + a direct byte comparison against `baserom.gba`),
per `docs/workflow.md`'s standing warning that this is a diagnostic
tool only. The isolated pass caught most of the statement-order issues
above; the direct-byte-comparison technique against the fully-linked
`crashbandicootxs.gba` (not just an isolated `.o`) caught two more real
bugs an isolated compile could not have: `GetBgStreamerColumn`/`GetBgStreamerRow`'s
buffer-base-load-before-shift ordering (both orders produce identical
instruction counts and shapes, only a byte-level diff against the real
linked ROM shows the swap) and `sub_8024E24`'s trailing alignment gap
(invisible in an isolated per-function check, since it only manifests
as the file's very last two bytes once fully linked).

Confirmed via a full clean `rm -rf build && make NON_MATCHING=1 report`
(no warnings for `game_loop57.c`) and a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`crashbandicootxs.gba: La suma coincide`).
`asm/code_3_2_17_24810.s` is now fully retired - all 25 functions moved
to the new `src/system/game_loop57.c`, and `ldscript.txt`'s entry for it
now points there directly.

## Cross-references

- `docs/status/game_loop.md` - matched list updated.
- `docs/rom_map.md` - "A new find: a custom RLE/delta token-stream
  decoder" and its two follow-up sections are the read-only
  reconnaissance this issue's matching work is based on.
- `docs/matching/issue-40-terrain-tile-cache.md` - the neighboring
  cluster this issue's NAKED functions share their register-allocation-
  permutation gap with.
- `docs/matching/issue-35-36-0x080231cc-game-loop.md` - the cluster
  immediately preceding this one in ROM order.

## Later pass (strag1)

`RunCutscenePlayer` and `DecodeLayerChunk` are now real C (old_agbcc), so the whole
`0x08024810`-`0x08024E68` range is decompiled. `RunCutscenePlayer`'s
`&gUnknown_03001300` reloads come from a function-scope pointer local
that global-alloc leaves unallocated; `DecodeLayerChunk` is `DecodeCollisionChunk`'s
matched shape with a 2D cell index. See
[strag1-naked-retry.md](strag1-naked-retry.md).
