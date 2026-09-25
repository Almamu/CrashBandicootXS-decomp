# GitHub issue #39: 0x08024810-0x08024E68 (25 functions, game_loop)

Issue #39 (`decomp-chunk`, category `game_loop`) listed 25 raw functions
in `asm/code_3_2_17_24810.s` - the remainder of the `UpdateGameFrame`-
`MainLoop` cluster sitting directly between the sound-channel-handle
family (`sub_8024590`-`sub_8024804`, GitHub issue #38,
`game_loop20.c`/`game_loop37.c`/`game_loop38.c`) and the terrain-tile
decode cache (`sub_8024E68`-`sub_8025334`, GitHub issue #40,
`game_loop3.c`/`game_loop4.c`/`game_loop5.c`). All 25 turned out to
belong to two already-partially-documented systems from
`docs/rom_map.md`, closing both out.

## What this cluster turned out to be

### `struct SoundChannelList` gains four more functions

`sub_8024804` (already matched, `game_loop20.c`) constructs a
`struct SoundChannelList` (`game_loop37.c`) by zeroing `items`/`count`
and setting `toggle` to 1. This chunk adds:

- **`sub_8024810`**: trivial wrapper - runs `sub_8024804`'s reset, then
  returns `self` unchanged.
- **`sub_8024948`**: extends that reset with two new fields this
  cluster introduces, `self+0x10`/`self+0x14` (a "text-paging" record
  array pointer and count - see `sub_8024820` below), both zeroed.
- **`sub_802493C`**: a plain two-argument forwarding trampoline to
  `sub_80247EC` (`game_loop20.c`) - passes both its arguments through
  untouched (only `self` is ever loaded into a register; `flags` flows
  through in `r1` since the ROM never touches it).
- **`sub_8024820`**: a *second* per-frame driver loop over the same
  `self+0/self+4` items/count pair `sub_8024640` (`game_loop37.c`)
  already drives, but interleaved with an explicit OAM-shadow-buffer
  flush (`sub_8006A90`/`sub_8006A48`/`sub_80006A8`/`sub_8006AAC` on
  `gUnknown_03001300` - the same "HUD-icon-plus-number renderer" OAM
  pacing pattern `docs/matching.md` documents elsewhere) and a nested
  text-paging walk through the new `self+0x10` record array (each
  record `{void **strings; s32 count;}`), rendering each string via
  `sub_8000EE4` (`text_layout.c`) against an `icon_manager *` at
  `self+0x14` and a 2-word "box" at `self+0x18`/`self+0x1c`, continuing
  to the next string in the current record while a held-input mask (9,
  versus `sub_8024640`'s 8) stays set. `self+0x24` feeds `sub_8037E54`
  (value/divisor) to compute the per-call text-wrap `limit`.

### The "visual scrolling background streamer" (`sub_8024960`-`sub_8024E24`)

`docs/rom_map.md`'s "A new find: a custom RLE/delta token-stream
decoder" section and its two follow-ups ("Follow-up: resolved the
semantics by tracing callers", "The background streamer's missing
'level load' half") had already characterized this system from a
read-only pass; this issue is where it got turned into (attempted)
byte-exact C.

The object is a small viewport/parallax layer built around a **circular
4x4-block ring-buffer tilemap** (64 halfword columns x 32 rows, 0x1000
bytes total at `self+8`), fed by the same custom RLE/delta token-stream
decoder the terrain-tile cache's `sub_8025334` (`game_loop3.c`) uses,
just writing into a 2D buffer (row stride 64 halfwords) instead of a
flat one:

- **`sub_8024960`**: the decoder itself - looks up a base pointer via
  `self+4` indexed by a halfword table, then decodes a token stream
  (budget-limited to 0x7f halfwords) with the same three run modes
  (literal-fill, signed-delta-accumulate, raw-copy) as `sub_8025334`.
- **`sub_8024AA0`**: the per-frame driver. Right-shifts the world
  position by 7/6 (128/64px tile granularity) and, on each axis the
  camera has crossed a tile boundary since last call, streams in
  exactly the newly-exposed column (`sub_8024C08`) or row
  (`sub_8024BAC`) - the tile 4 ahead of the old edge when scrolling
  forward, or the tile directly behind when scrolling back - while
  advancing a mod-4 "sub-block" accumulator (`self+0x14`/`self+0x15`).
- **`sub_8024BAC`/`sub_8024C08`**: the Y-axis/X-axis incremental
  streamers - for the newly-exposed row/column, decode up to 4 tiles via
  `sub_8024960` into the ring buffer's block-addressed slot.
- **`sub_8024C64`**: the "level load" constructor half - same tile-coord
  math as `sub_8024AA0`, but loops the *entire* 4x4 block grid to seed
  the ring buffer's full initial contents (rather than one row/column).
- **`sub_8024B18`/`sub_8024B48`/`sub_8024B78`**: convert a world pixel
  `(x, y)` into the ring buffer's wrapped `(col, row)` address -
  `sub_8024B18` returns the column-wrapped pointer and writes the row
  out by pointer, `sub_8024B48` the reverse, `sub_8024B78` combines both
  and returns the decoded halfword value directly (no out-param).
- **`sub_8024CF0`/`sub_8024D0C`/`sub_8024D38`/`sub_8024D58`/
  `sub_8024D5C`/`sub_8024D60`/`sub_8024D6C`/`sub_8024D74`/`sub_8024DAC`/
  `sub_8024DCC`/`sub_8024DE0`/`sub_8024DFC`/`sub_8024E24`** round out
  construction/accessors: `sub_8024CF0` stores the room/level descriptor
  and derives a "decode base" from `gUnknown_03001308`'s own `+0x24`
  field, the same "camera offset + source field" shape as the
  terrain-tile cache's `decodeBase`; `sub_8024D0C`/`sub_8024D38`/
  `sub_8024D74`/`sub_8024DAC` wire up two different
  `sub_803AD80`-style interworking-trampoline tables
  (`gStaticData_087E4BDC`/`gStaticData_087E4BEC`) for notifying a parent
  object of size/position changes, following the exact
  `self + *(s16 *)(mgr + N)`/`*(void **)(mgr + N + 4)` idiom
  `src/graphics/actor_anim.c`'s `sub_803B0F0` already established;
  `sub_8024D58`/`sub_8024D5C`/`sub_8024D60`/`sub_8024D6C` are plain
  position accessors; `sub_8024DCC` clamps to `[-0x10, 0x10]`;
  `sub_8024DE0` clamps a position pair against an upper-bound pair;
  `sub_8024DFC` applies the layer's per-axis scale (floor-dividing the
  Q8 product, matching `matching.md`'s established `+0xff`-before-`>>8`
  floor-division idiom for negative products); `sub_8024E24` is a
  two-line position-delta forward through the trampoline table,
  accumulating both axes' results back into the layer's own position.

None of this cluster's fields are given a named struct here, for the
same reason `game_loop3.c`'s own viewport/parallax-layer object isn't:
several fields are only ever written/read by functions on both sides of
this ROM range (this file, `game_loop3.c`, and still-raw neighbors like
`sub_8024DFC`/`sub_8024E24`/`sub_8024C64`/`sub_8024CF0` that `game_loop3.c`'s
own header comment named before this issue closed them), so committing
a struct now risks getting a field wrong that only surfaces once the
rest is matched.

## Result: 20 real C, 5 NAKED, all 25 closed

**Matched as real C (20):** `sub_8024810`, `sub_802493C`, `sub_8024948`,
`sub_8024AA0`, `sub_8024B18`, `sub_8024B48`, `sub_8024B78`, `sub_8024CF0`,
`sub_8024D0C`, `sub_8024D38`, `sub_8024D58`, `sub_8024D5C`, `sub_8024D60`,
`sub_8024D6C`, `sub_8024D74`, `sub_8024DAC`, `sub_8024DCC`, `sub_8024DE0`,
`sub_8024DFC`, `sub_8024E24`.

`sub_8024AA0` (the per-frame axis-crossing dispatcher) matched on the
first isolated-compile attempt with no register pins needed at all -
the same "confirmed the same as the just-closed sibling cluster" result
`sub_8026628` got in the terrain-streamer's own neighborhood.

`sub_8024B18`/`sub_8024B48`/`sub_8024B78` (the wrapped-address helpers)
each needed the `register u8 subX/subY asm("r5")`/`register s32 shifted
asm("r4")` pin recipe (load the raw sub-block byte into one fixed
register, its `<<4`/`<<3`-shifted product into a *different* fixed
register) to reproduce the ROM's own two-register relay - reusing one
register for both the load and the shift (the natural, unpinned
compilation) produces the right *value* but the wrong *register*, a
byte-for-byte mismatch despite identical semantics. `sub_8024B78`
additionally needed its col/row temporaries to land in `r3` (not `r4`)
since it has one fewer live argument (`self`/`x`/`y` only, no
out-parameter) than its two siblings, freeing `r3` as scratch - the
same register got reused for a different purpose purely because of a
different argument count, not any code shape difference.

Three functions needed simple statement-order fixes once the two-word
struct convention was in the wrong order - a recurring gotcha, not a
register problem:

- `sub_8024CF0` needed its two `u16` loads (`source+0x1a`/`source+0x1c`)
  hoisted into separate locals *before* either store, matching the
  ROM's own "load both, then store both" grouping (writing each load
  immediately followed by its store made the compiler reuse one
  register for both, losing the ROM's two-register shape).
- `sub_8024D74` needed its `gStaticData_087E4BEC` table-pointer store
  moved *before* the `self+0x2c` child-pointer load (the reverse of the
  most natural C statement order, but matching the ROM's own
  instruction sequence).
- `sub_8024DE0` needed its second clamp's `self+0xc` load deferred into
  a nested scope (`{ s32 b = ...; ... }`) rather than declared alongside
  the first at the top of the function, so the load actually happens
  after the first clamp/store pair completes, matching the ROM.
- `sub_8024B18`/`sub_8024B48` additionally needed the final `self+8`
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

**Closed as NAKED (5):** `sub_8024820`, `sub_8024960`, `sub_8024BAC`,
`sub_8024C08`, `sub_8024C64` - all hit the same gcc-2.9
register-allocation-permutation/redundant-shadow-register class of gap
already exhaustively documented for the neighboring terrain-tile-cache
cluster (`sub_8024F24`/`sub_80250BC`/`sub_8025130`/`sub_8025228`/
`sub_8025334`, `game_loop3.c`, GitHub issue #40): each packs several
persistent values (a running linear tile/byte index, a loop counter, a
budget counter, fixed mask constants) into a specific fixed register
(`r6`/`r7`/`r8`/`ip`/`sb`/`sl`) simultaneously live across one or more
calls to `sub_8024960`, and this compiler's allocator never reproduces
that exact packing no matter how the C is phrased - the same symptom,
not a new one. Hand-transcribed instruction-for-instruction from the
ROM disassembly instead, following the exact same NAKED-transcription
convention (numbered local labels, register names spelled `sb`/`r8`/`ip`
verbatim, explicit trailing `.align 2, 0` where the ROM's own raw block
has one).

`sub_8024820` in particular is the largest NAKED transcription in this
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
bugs an isolated compile could not have: `sub_8024B18`/`sub_8024B48`'s
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
