# Issue #65 continued: 0x08035780-0x08037110 - the level-object subsystem

GitHub issue #65 (`decomp-chunk`, category `graphics_loading`) originally
listed 22 raw functions in `0x080354E0`-`0x08037110`. An earlier pass
(see [issue-65-graphics-loading.md](issue-65-graphics-loading.md))
covered the first three - `InitTitleScreen` (matched, real C),
`LoadTitleScreenBg` (NAKED), `LoadTitleScreenObjTiles` (matched, real C,
register-pinning + opaque asm islands) - all three now living in
`src/frontend/title_screen_init.c`. This is the write-up for the
remaining 19 functions, `UpdateTitleLogoPieces` through `DrawLogoActor`, the whole
former contents of `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s`
(confirmed by re-listing that file's functions with
`grep -n "^_*sub_\|^_*nullsub_"` before starting - the list matched the
issue's 19 remaining boxes exactly, and the file ended immediately
after `DrawLogoActor` with no extra trailing function).

## What this pass covered

Two distinct object shapes turned out to be involved, not one:

- **The 0x220-byte per-level scratch object** `InitTitleScreen`
  allocates and returns (still a raw `u32 *` there, per that file's own
  note). Most of this pass's functions operate on it:
  - A 9-slot, later 20-slot, stride-0x34 record array at `self+4`/
    `self+0x34+...`: each slot has a countdown word, a "delta record"
    walk pointer, and live position (Q16.16, `<<16`) / velocity (Q24.8,
    `<<8`) fields plus 4 raw accompanying words. `UpdateTitleLogoPieces` (9-slot
    variant) and its exact record-shape twins `ResetTitleLogoPieces` (init-time
    seed, `stm`-based store), `UpdateVvLogoPieces`/`DrawVvLogoPieces` (20-slot
    variant, split across two functions - one for the decay/reload
    pass, one for the OAM-queueing pass) all share this shape.
    `InitVvLogoPieces` seeds the 20-slot array's initial state from
    `gVvLogoPieceSeeds`.
  - A 7-slot rolling-hash "cheat code" detector at `self+0x210`
    (`TitleScreenCheatInput`, gated on `gKeys.held`'s bit 0x100 - R
    shoulder - folding one of 7 fixed hex "signature" constants selected
    by a `pressed` bitmask through a rotate-then-multiply-by-521 hash,
    checking the result against a fixed target `0x3034AF3B` to fire song
    `0xc`). `HashTitleCheatInput` is the same hash operation factored out as a
    standalone one-shot helper - see "Matched" below.
  - A BG2 affine-scroll position/scale block at `self+0x214..0x21c`,
    flushed to `REG_BG2X`/`REG_BG2Y`/`REG_BG2PA`/`REG_BG2PD` by
    `CommitTitleScreenFrame`.
  - `DrawTitleLogoPieces` builds the header record's own OAM affine-sprite
    entry and drives the same 9-slot array's per-frame OAM queueing.
  - `RunTitleScreen` is the sequencer: seeds the 9-slot array (via
    `ResetTitleLogoPieces`'s shape inline), loops `UpdateTitleLogoPieces` + `DrawTitleScreen` +
    `UpdateStarfield` + `CommitTitleScreenFrame` until the header's hold record drains,
    then runs a `TitleScreenCheatInput`-gated SFX/nudge phase followed by a
    17-frame BG2 fade-out.
  - `DrawTitleScreen` is the per-frame flush helper (`DrawTitleLogoPieces` plus
    conditional icon-manager positioning via `DrawTitleMenuItem`).
  - `DrawTitleMenuItem` positions one icon-manager slot's OAM entries (a pair,
    0xf0px apart) from its own record.
  - `DestroyTitleScreen` is a teardown/reset helper (stops the header's own
    play-counter, clears OAM attribute-2, resets DMA-adjacent
    registers).
  - `RunCompanyLogos` is the level-object subsystem's own init/run/teardown
    driver: sets up the OBJ-tile free list/OAM queue/sprite-frame cache,
    builds an actor-part header object (`InitLogoActor`), loads BG2's
    tileset/palette (`LoadVvLogoGraphics`) and the slot array (`InitVvLogoPieces`),
    loads the BG2 tilemap remap (`LoadUniversalLogoBg`), runs a fixed fade-in
    ramp, loops a per-frame body (input poll, a fade-scored counter,
    `UpdateVvLogoPieces`/`DrawVvLogoPieces`'s slot-array update), then tears the
    whole subsystem back down.
  - `LoadVvLogoGraphics` allocates 3 VRAM tile blocks and DMA-loads 3 palette
    banks plus their tile data (`LoadTaggedAssetBuffered`), plus a 4th block sized
    from a package header byte.
  - `LoadUniversalLogoBg` is BG2's tilemap-remap loader for
    `gUniversalLogoBg`'s package - the same "remap the tilemap's
    per-tile palette-select nibble while DMA-copying it to VRAM" shape
    `LoadTitleScreenBg`/`LoadTitleScreenObjTiles` already established,
    just for a second BG2 package and taking no arguments (this
    package's pointer lives entirely in the static table).

- **The unrelated `struct anim_part_instance`-shaped actor-part object**
  (`src/actor/actor_anim.c`/`polar_player_actions.c`'s already-documented
  "self", state at `+0x28`, frame accumulator at `+8`, table index at
  `+0xc`, frame halfword/byte at `+0x10`/`+0x12`, counter at `+0x44`):
  - `InitLogoActor` constructs one via `InitActorPart`, sets its vtable
    (`gLogoActorVtable`), and allocates/caches two VRAM tile blocks
    sized from its current animation frame.
  - `UpdateLogoActor` is its animation-state machine (5 states, each waiting
    on a specific frame id before transitioning, playing SFX at some
    transitions, decaying a position field in the penultimate state).
  - `DrawLogoActor` is its OAM builder (a no-op once the state machine
    reaches its terminal state): resolves the current frame's tile
    pointer, projects screen position via two `__divsi3` sine/cosine
    calls, clips against screen bounds, re-uploads to VRAM only when the
    frame pointer changed since last time, and queues the OAM entry.

## Matched (1 function, full clean `make compare` passing)

- **`HashTitleCheatInput`** (`src/frontend/title_screen_init.c`) - the
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

**Update:** `UpdateTitleLogoPieces` was later promoted to real C via the
static-inline anti-CSE technique (a separate `static inline` accessor
per field offset, plus materializing `self+CONST` and the `i*0x34`
stride as their own named locals before combining) - see
[issue-59-60-static-inline-cse-promotion.md](issue-59-60-static-inline-cse-promotion.md).
A later attempt at `TitleScreenCheatInput` with the same technique did *not*
close it (confirmed to be cross-jump/tail-merging of branch bodies, not
repeated address arithmetic - same doc). Both are still listed among
the 18 below since this section is this pass's own historical record;
see the two status docs (`docs/status/graphics_loading.md`) for the
current matched/parked state.

All 18 remaining functions - `UpdateTitleLogoPieces`, `DrawTitleLogoPieces`, `TitleScreenCheatInput`,
`RunTitleScreen`, `CommitTitleScreenFrame`, `DrawTitleMenuItem`, `DrawTitleScreen`, `ResetTitleLogoPieces`,
`DestroyTitleScreen`, `RunCompanyLogos`, `LoadVvLogoGraphics`, `InitVvLogoPieces`, `UpdateVvLogoPieces`,
`DrawVvLogoPieces`, `LoadUniversalLogoBg`, `InitLogoActor`, `UpdateLogoActor`, `DrawLogoActor`
- were each attempted as real C first, and each hit one of these gaps:

- **Cross-jump/tail-merging collapses the ROM's own duplicated address
  computation.** `UpdateTitleLogoPieces`'s field-copy block (10 stores across
  `self+0x18..0x3c+i*0x34`) is written by the ROM as 10 fully independent
  `ip`-plus-constant-plus-`r3` address computations, never hoisting a
  shared `self+i*0x34` base pointer even across a straight-line run with
  no intervening branch or call - but any plain-C phrasing that computes
  `self + offset + r3` repeatedly gets CSE'd into a shared base register
  by this compiler regardless of phrasing (tried: raw pointer casts,
  `asm volatile("" ::: "memory")` barriers between each store - neither
  stopped the fold, since the barrier invalidates memory contents, not a
  previously-computed pure-arithmetic register value). `TitleScreenCheatInput`'s
  7-branch dispatch hits the same family of gap from the opposite
  direction: 3 of its 7 branches inline an identical "compute the hash
  slot address, jump to the shared hash body" sequence that the ROM
  leaves duplicated 3 times (in a different scratch register, `r4`, than
  the other 4 branches' shared copy, which uses `r0`) but this compiler
  always merges into 1-2 physical copies via cross-jump elimination,
  even with an explicit `register ... asm("r4")` pin on the duplicated
  branches' local (the pin had no visible effect on the emitted code -
  the pinned register apparently gets coalesced away before the merge pass
  runs). `CommitTitleScreenFrame` hits a narrower version of the same idea: the ROM
  derives `REG_BG2PA`'s address from the just-computed `REG_BG2Y`
  address via a runtime `-0xc`, rather than loading a fresh literal,
  which no phrasing of four independent `REG_BG2*` register writes
  reproduces.
- **Heavy `sb`/`sl`/`r8`/`ip` register shuffling.** `DrawTitleLogoPieces`,
  `RunTitleScreen`, `ResetTitleLogoPieces`, `RunCompanyLogos`, `LoadVvLogoGraphics`,
  `UpdateVvLogoPieces`, `DrawVvLogoPieces`, `LoadUniversalLogoBg`, `DrawLogoActor` all lean on
  3-5 high registers simultaneously, several reused for genuinely
  different roles at different points in the same function (the same
  "save/reuse/restore shuttle around one physical register" shape
  documented for `LoadTitleScreenObjTiles`'s `pkgPtr`/`pkgPtrStash`) - closing
  these exactly, for 9 functions at once, was judged not to be a good
  use of a single pass's effort given every other technique in this
  project's catalog (register pins, opaque asm islands, declaration
  reordering) requires per-function, often per-register, iteration to
  land safely (see `LoadTitleScreenBg`'s and `LoadTitleScreenObjTiles`'s own
  multi-pass histories in
  [issue-65-graphics-loading.md](issue-65-graphics-loading.md) for how
  much iteration even one such function can take).
- The remaining functions (`DrawTitleScreen`, `DrawTitleMenuItem`, `DestroyTitleScreen`,
  `InitVvLogoPieces`, `InitLogoActor`, `UpdateLogoActor`) are simple enough that a
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
`DrawVvLogoPieces`'s `_08036AFC` loop preheader dropped a duplicate
`str r1, [r2]` the ROM's own compiler had emitted twice in that loop
body (once before the first `adds r1, #0xa0` and again after it) -
caught immediately by the per-function byte-diff (a resulting cascade
of "off by one instruction" mismatches for the rest of the function)
and fixed before integrating.

After integrating all 19 functions into
`src/frontend/title_screen_init.c` (replacing the now-fully-
consumed `asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s`, which is
deleted), a second, whole-file version of the same check was run:
compiling the finished `.c` file with the real `cpp`+`agbcc`+`as`
pipeline and byte-diffing the *entire* combined object's `.text`
(6544 bytes, `0x08035780`-`0x08037110`) against the matching
`baserom.gba` slice in one pass. This additionally confirmed every
internal cross-call between this file's own functions (`RunTitleScreen`
calling `UpdateTitleLogoPieces`/`DrawTitleScreen`/`CommitTitleScreenFrame`, etc. - now resolvable
to real relative offsets since both ends live in the same object)
lands at the exact byte offset the ROM's own layout does, with zero
gaps or padding between any two functions - every one of the 19
functions' individually-measured sizes summed to exactly 6544 bytes,
matching the chunk's own address range precisely.

## File structure

`asm/code_3_2_20_28568_c99c_31784_33ef4_355e0.s` is deleted (its entire
remaining content - all 19 functions - is now covered).
`src/frontend/title_screen_init.c` holds all 19 functions: 18
`NAKED`, 1 (`HashTitleCheatInput`) real C. `ldscript.txt`'s
`code_3_2_20_28568_c99c_31784_33ef4_355e0.o` line is replaced with
`title_screen_init.o` in the same link position.
`tools/report_units.py`'s single combined "left raw" entry for this
range is split into 19 per-function entries (18 `base_object = None`,
1 pointing at the new file). Verified via a full clean
`rm -rf build && make NON_MATCHING=1 report` (clean compile, no
warnings) and a full clean `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare`
(`La suma coincide`).

See [docs/status/graphics_loading.md](../status/graphics_loading.md)
for the running matched/parked list.

## Later pass: NAKED retry

The retry pass closed 9 of this file's NAKED functions as real C under
old_agbcc (`CommitTitleScreenFrame`, `DrawTitleMenuItem`, `DrawTitleScreen`, `DestroyTitleScreen`,
`RunCompanyLogos`, `LoadVvLogoGraphics`, `InitLogoActor`, `UpdateLogoActor`,
`DrawLogoActor`). The object is now on `OLD_AGBCC_OBJS`. `DrawTitleLogoPieces`,
`TitleScreenCheatInput`, `RunTitleScreen`, `ResetTitleLogoPieces`, `InitVvLogoPieces`, `UpdateVvLogoPieces`
and `LoadUniversalLogoBg` stay NAKED, each with a near-miss draft under
`#if NON_MATCHING`. `DrawVvLogoPieces` was not attempted. See
[issue-65-naked-retry.md](issue-65-naked-retry.md).

## Later pass (issues #64/#65 second NAKED retry)

`ResetTitleLogoPieces`, `RunTitleScreen` and `DrawTitleLogoPieces` are now real C. The file
was split at `TitleScreenCheatInput` into `title_screen.c`, because
`DrawTitleLogoPieces` needs strength reduction on and `InitVvLogoPieces` needs it
off. See [issue-64-65-naked-retry-2.md](issue-64-65-naked-retry-2.md).
