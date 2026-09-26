# Issue #65 continued: 0x08035780-0x08037110 - the level-object subsystem

GitHub issue #65 (`decomp-chunk`, category `graphics_loading`) originally
listed 22 raw functions in `0x080354E0`-`0x08037110`. An earlier pass
(see [issue-65-graphics-loading.md](issue-65-graphics-loading.md))
covered the first three - `LoadLevelGraphics` (matched, real C),
`LoadBg2Background` (NAKED), `LoadObjSpriteTiles` (matched, real C,
register-pinning + opaque asm islands) - all three now living in
`src/graphics/level_graphics.c`. This is the write-up for the
remaining 19 functions, `sub_8035780` through `sub_8036FBC`, the whole
former contents of `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s`
(confirmed by re-listing that file's functions with
`grep -n "^_*sub_\|^_*nullsub_"` before starting - the list matched the
issue's 19 remaining boxes exactly, and the file ended immediately
after `sub_8036FBC` with no extra trailing function).

## What this pass covered

Two distinct object shapes turned out to be involved, not one:

- **The 0x220-byte per-level scratch object** `LoadLevelGraphics`
  allocates and returns (still a raw `u32 *` there, per that file's own
  note). Most of this pass's functions operate on it:
  - A 9-slot, later 20-slot, stride-0x34 record array at `self+4`/
    `self+0x34+...`: each slot has a countdown word, a "delta record"
    walk pointer, and live position (Q16.16, `<<16`) / velocity (Q24.8,
    `<<8`) fields plus 4 raw accompanying words. `sub_8035780` (9-slot
    variant) and its exact record-shape twins `sub_80360DC` (init-time
    seed, `stm`-based store), `sub_8036668`/`sub_803686C` (20-slot
    variant, split across two functions - one for the decay/reload
    pass, one for the OAM-queueing pass) all share this shape.
    `sub_8036600` seeds the 20-slot array's initial state from
    `gStaticData_0817D6C0`.
  - A 7-slot rolling-hash "cheat code" detector at `self+0x210`
    (`sub_8035D1C`, gated on `gUnknown_030007E0.held`'s bit 0x100 - R
    shoulder - folding one of 7 fixed hex "signature" constants selected
    by a `pressed` bitmask through a rotate-then-multiply-by-521 hash,
    checking the result against a fixed target `0x3034AF3B` to fire song
    `0xc`). `sub_80360C0` is the same hash operation factored out as a
    standalone one-shot helper - see "Matched" below.
  - A BG2 affine-scroll position/scale block at `self+0x214..0x21c`,
    flushed to `REG_BG2X`/`REG_BG2Y`/`REG_BG2PA`/`REG_BG2PD` by
    `sub_8035F9C`.
  - `sub_80358A8` builds the header record's own OAM affine-sprite
    entry and drives the same 9-slot array's per-frame OAM queueing.
  - `sub_8035E14` is the sequencer: seeds the 9-slot array (via
    `sub_80360DC`'s shape inline), loops `sub_8035780` + `sub_8036068` +
    `sub_8034688` + `sub_8035F9C` until the header's hold record drains,
    then runs a `sub_8035D1C`-gated SFX/nudge phase followed by a
    17-frame BG2 fade-out.
  - `sub_8036068` is the per-frame flush helper (`sub_80358A8` plus
    conditional icon-manager positioning via `sub_8035FEC`).
  - `sub_8035FEC` positions one icon-manager slot's OAM entries (a pair,
    0xf0px apart) from its own record.
  - `sub_8036154` is a teardown/reset helper (stops the header's own
    play-counter, clears OAM attribute-2, resets DMA-adjacent
    registers).
  - `sub_80361B0` is the level-object subsystem's own init/run/teardown
    driver: sets up the OBJ-tile free list/OAM queue/sprite-frame cache,
    builds an actor-part header object (`sub_8036E20`), loads BG2's
    tileset/palette (`sub_8036528`) and the slot array (`sub_8036600`),
    loads the BG2 tilemap remap (`sub_8036CF4`), runs a fixed fade-in
    ramp, loops a per-frame body (input poll, a fade-scored counter,
    `sub_8036668`/`sub_803686C`'s slot-array update), then tears the
    whole subsystem back down.
  - `sub_8036528` allocates 3 VRAM tile blocks and DMA-loads 3 palette
    banks plus their tile data (`sub_8037110`), plus a 4th block sized
    from a package header byte.
  - `sub_8036CF4` is BG2's tilemap-remap loader for
    `gStaticData_0817D7A4`'s package - the same "remap the tilemap's
    per-tile palette-select nibble while DMA-copying it to VRAM" shape
    `LoadBg2Background`/`LoadObjSpriteTiles` already established,
    just for a second BG2 package and taking no arguments (this
    package's pointer lives entirely in the static table).

- **The unrelated `struct anim_part_instance`-shaped actor-part object**
  (`src/graphics/actor_anim.c`/`actor_part19.c`'s already-documented
  "self", state at `+0x28`, frame accumulator at `+8`, table index at
  `+0xc`, frame halfword/byte at `+0x10`/`+0x12`, counter at `+0x44`):
  - `sub_8036E20` constructs one via `InitActorPart`, sets its vtable
    (`gStaticData_087E55C4`), and allocates/caches two VRAM tile blocks
    sized from its current animation frame.
  - `sub_8036EC4` is its animation-state machine (5 states, each waiting
    on a specific frame id before transitioning, playing SFX at some
    transitions, decaying a position field in the penultimate state).
  - `sub_8036FBC` is its OAM builder (a no-op once the state machine
    reaches its terminal state): resolves the current frame's tile
    pointer, projects screen position via two `sub_803ADB4` sine/cosine
    calls, clips against screen bounds, re-uploads to VRAM only when the
    frame pointer changed since last time, and queues the OAM entry.

## Matched (1 function, full clean `make compare` passing)

- **`sub_80360C0`** (`src/graphics/graphics_loading_35780.c`) - the
  standalone rotate-then-multiply-by-521 hash update. Every operation
  matched a plain-C reconstruction immediately except the rotate itself:
  the natural `(v << 1) | (v >> 31)` idiom gets folded by agbcc's
  optimizer into a single Thumb `ROR rD, rD, rN` instruction, which this
  ROM's own compiled output never emits (it always expands a rotate into
  the explicit `lsl`/`lsr`/`orr` sequence). Splitting the shift into two
  separate, explicitly register-pinned locals (`hi`/`lo` to `r3`/`r2`,
  matching the ROM's own choice) and adding an empty `asm("" : "+r"(hi))`
  barrier between them stopped the fold and produced byte-identical
  output on the first try after that fix.

## Parked (`NAKED`, 18 functions)

All 18 remaining functions - `sub_8035780`, `sub_80358A8`, `sub_8035D1C`,
`sub_8035E14`, `sub_8035F9C`, `sub_8035FEC`, `sub_8036068`, `sub_80360DC`,
`sub_8036154`, `sub_80361B0`, `sub_8036528`, `sub_8036600`, `sub_8036668`,
`sub_803686C`, `sub_8036CF4`, `sub_8036E20`, `sub_8036EC4`, `sub_8036FBC`
- were each attempted as real C first, and each hit one of these gaps:

- **Cross-jump/tail-merging collapses the ROM's own duplicated address
  computation.** `sub_8035780`'s field-copy block (10 stores across
  `self+0x18..0x3c+i*0x34`) is written by the ROM as 10 fully independent
  `ip`-plus-constant-plus-`r3` address computations, never hoisting a
  shared `self+i*0x34` base pointer even across a straight-line run with
  no intervening branch or call - but any plain-C phrasing that computes
  `self + offset + r3` repeatedly gets CSE'd into a shared base register
  by this compiler regardless of phrasing (tried: raw pointer casts,
  `asm volatile("" ::: "memory")` barriers between each store - neither
  stopped the fold, since the barrier invalidates memory contents, not a
  previously-computed pure-arithmetic register value). `sub_8035D1C`'s
  7-branch dispatch hits the same family of gap from the opposite
  direction: 3 of its 7 branches inline an identical "compute the hash
  slot address, jump to the shared hash body" sequence that the ROM
  leaves duplicated 3 times (in a different scratch register, `r4`, than
  the other 4 branches' shared copy, which uses `r0`) but this compiler
  always merges into 1-2 physical copies via cross-jump elimination,
  even with an explicit `register ... asm("r4")` pin on the duplicated
  branches' local (the pin had no visible effect on the emitted code -
  the pinned register apparently gets coalesced away before the merge pass
  runs). `sub_8035F9C` hits a narrower version of the same idea: the ROM
  derives `REG_BG2PA`'s address from the just-computed `REG_BG2Y`
  address via a runtime `-0xc`, rather than loading a fresh literal,
  which no phrasing of four independent `REG_BG2*` register writes
  reproduces.
- **Heavy `sb`/`sl`/`r8`/`ip` register shuffling.** `sub_80358A8`,
  `sub_8035E14`, `sub_80360DC`, `sub_80361B0`, `sub_8036528`,
  `sub_8036668`, `sub_803686C`, `sub_8036CF4`, `sub_8036FBC` all lean on
  3-5 high registers simultaneously, several reused for genuinely
  different roles at different points in the same function (the same
  "save/reuse/restore shuttle around one physical register" shape
  documented for `LoadObjSpriteTiles`'s `pkgPtr`/`pkgPtrStash`) - closing
  these exactly, for 9 functions at once, was judged not to be a good
  use of a single pass's effort given every other technique in this
  project's catalog (register pins, opaque asm islands, declaration
  reordering) requires per-function, often per-register, iteration to
  land safely (see `LoadBg2Background`'s and `LoadObjSpriteTiles`'s own
  multi-pass histories in
  [issue-65-graphics-loading.md](issue-65-graphics-loading.md) for how
  much iteration even one such function can take).
- The remaining functions (`sub_8036068`, `sub_8035FEC`, `sub_8036154`,
  `sub_8036600`, `sub_8036E20`, `sub_8036EC4`) are simple enough that a
  first real-C attempt looked plausible, but were transcribed as NAKED
  alongside their siblings for this pass rather than spending further
  isolated-compile iteration on each individually - none are ruled out
  as eventually matchable, they just weren't pushed through this time.

Every one of the 18 was verified byte-correct *before* integrating, via
a lighter-weight variant of the project's usual isolated-compile check:
since a NAKED body's bytes are supposed to be the ROM's own bytes
verbatim (just reformatted from unified to divided Thumb syntax), each
was assembled standalone with `arm-none-eabi-as` (no `cpp`/`agbcc`
needed - NAKED bypasses C codegen entirely), extracted with
`arm-none-eabi-objcopy -O binary --only-section=.text`, and compared
byte-for-byte against the matching slice of `baserom.gba`. Every
mismatch found this way was confirmed to be one of the two expected,
inherent relocation artifacts of an unlinked standalone object - an
unresolved `bl` call-site offset, or an unresolved external symbol's
literal-pool address word (which disassembles as garbage-looking
"instructions" when its 4 zero/placeholder bytes are read back as
Thumb opcodes) - never a real transcription error. One genuine
transcription bug *was* caught this way: an early hand-transcription of
`sub_803686C`'s `_08036AFC` loop preheader dropped a duplicate
`str r1, [r2]` the ROM's own compiler had emitted twice in that loop
body (once before the first `adds r1, #0xa0` and again after it) -
caught immediately by the per-function byte-diff (a resulting cascade
of "off by one instruction" mismatches for the rest of the function)
and fixed before integrating.

After integrating all 19 functions into
`src/graphics/graphics_loading_35780.c` (replacing the now-fully-
consumed `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s`, which is
deleted), a second, whole-file version of the same check was run:
compiling the finished `.c` file with the real `cpp`+`agbcc`+`as`
pipeline and byte-diffing the *entire* combined object's `.text`
(6544 bytes, `0x08035780`-`0x08037110`) against the matching
`baserom.gba` slice in one pass. This additionally confirmed every
internal cross-call between this file's own functions (`sub_8035E14`
calling `sub_8035780`/`sub_8036068`/`sub_8035F9C`, etc. - now resolvable
to real relative offsets since both ends live in the same object)
lands at the exact byte offset the ROM's own layout does, with zero
gaps or padding between any two functions - every one of the 19
functions' individually-measured sizes summed to exactly 6544 bytes,
matching the chunk's own address range precisely.

## File structure

`asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s` is deleted (its entire
remaining content - all 19 functions - is now covered).
`src/graphics/graphics_loading_35780.c` holds all 19 functions: 18
`NAKED`, 1 (`sub_80360C0`) real C. `ldscript.txt`'s
`code_3_2_20_28568_c99c_31784_33ef4_355e0.o` line is replaced with
`graphics_loading_35780.o` in the same link position.
`tools/report_units.py`'s single combined "left raw" entry for this
range is split into 19 per-function entries (18 `base_object = None`,
1 pointing at the new file). Verified via a full clean
`rm -rf build && make NON_MATCHING=1 report` (clean compile, no
warnings) and a full clean `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`
(`La suma coincide`).

See [docs/status/graphics_loading.md](../status/graphics_loading.md)
for the running matched/parked list.
