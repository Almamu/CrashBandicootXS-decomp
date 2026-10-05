# Issue #72: 0x0803B4EC-0x0803B884 (actor)

19 labelled functions, ~0.9 KB, `asm/code_3_3.s` - the whole remaining
file. Directly contiguous with `src/graphics/actor_anim.c`'s existing
coverage (issue #71 ended exactly at `0x0803B4EC`, where this chunk
starts), so all of it landed in that same file rather than a new one,
keeping ROM contiguity per `docs/workflow.md` step 4. `asm/code_3_3.s`
is now empty and has been deleted, with its one `ldscript.txt` line
removed.

## Seven more hidden functions with no label of their own

Before writing any C, a from-scratch `arm-none-eabi-as` assembly of the
original raw `asm/code_3_3.s` (recovered via `git show HEAD:asm/code_3_3.s`
before it was deleted) followed by `arm-none-eabi-objdump -d` on the
resulting `.o` gave an address for every real instruction and literal
pool word in the file, independent of the raw disassembly dump's own
(sometimes misleading) `.4byte` placeholders - several of those
placeholders turned out to be two ordinary Thumb instructions the
original naive disassembler failed to decode, not real data (confirmed
by disassembling the literal 4-byte value in isolation, e.g.
`0x64d06cd8` decodes cleanly to `ldr r0,[r3,#76]` / `str r0,[r2,#76]`).

Doing this full address walk (rather than trusting the visible
`thumb_func_start` labels alone) surfaced seven functions the original
disassembly never gave their own label - each one sits inside what
looks like trailing padding/literal-pool space after a labelled
function, exactly the `mem_walk_heaps`/`strlen` pattern documented in
`docs/decomp_dev.md`. Recorded via `split ADDRESS NAME` entries in
`expected/corrections.txt` so `report_units.py`/decomp.dev stop crediting
their bytes to the preceding labelled function:

- **`IsJetpackCheckpointTextUnshootable`** (0x0803B54C, 4 bytes) - between `UpdateJetpackCheckpointText`'s real
  return and `DestroyJetpackCheckpointText` below. `movs r0,#1; bx lr` - a trivial
  "return true" stub.
- **`DestroyJetpackCheckpointText`** (0x0803B550) - the standard `struct linked_node`
  teardown handler shape (see below), between `IsJetpackCheckpointTextUnshootable` and
  `UpdateJetpackExplosion`.
- **`UpdateJetpackExplosion`** (0x0803B57C) - `UpdatePolarCheckpointText`'s near-twin (see
  below), between `DestroyJetpackCheckpointText` and `IsJetpackExplosionUnshootable`.
- **`IsJetpackExplosionUnshootable`** (0x0803B5AC, 4 bytes) - byte-identical to
  `IsJetpackCheckpointTextUnshootable`, between `UpdateJetpackExplosion` and `DestroyJetpackExplosion`.
- **`GetActorHp`** (0x0803B5DC, 4 bytes) - between `DestroyJetpackExplosion`'s real
  return and `nullsub_44` below. `ldr r0,[r0,#0x54]; bx lr` - a plain
  `self->field_54` getter.
- **`nullsub_44`** (0x0803B5E0, 2 bytes, 2-byte aligned pad after) -
  `bx lr` alone, a genuinely empty stub (same shape as `ResetStopwatch` in
  `src/graphics/actor_part39.c`) - next available `nullsub_N`, since
  `_call_via_lr` was already taken.
- **`IsJetpackPlayerUnshootable`** (0x0803B5E4, 4 bytes) - between `nullsub_44`'s
  padding and `DestroyJetpackShot`. `movs r0,#0; bx lr` - a trivial "return
  false"/"return 0" stub.

None of these seven have a direct `bl`/`.4byte` reference anywhere in
`asm/*.s`/`expected/*.s`/`src/*.c` - same as every one of the 20 "kind"
teardown handlers already matched in this file for issue #71, which are
also grep-invisible (whatever installs one of these as an object's
"destroy" vtable slot does so from a still-unsymbolized raw pointer
table, not a readable `bl`). Not tagged `UNUSED` for the same reason
issue #71's handlers weren't - grep silence here doesn't mean confirmed
dead, just unconfirmed-live.

## Matched (all 19 labelled + 7 hidden = 26 functions)

All in `src/graphics/actor_anim.c`, in ROM order:

- **`UpdateJetpackCheckpointText`** - advances the animation frame accumulator, or
  fires the `+0x50` trampoline record instead when the "held" flag
  (`+0x12`) is set. The unconditional `+0x14` word write at the top
  looks like a per-call "ticked this frame" marker with no reader
  matched yet. In the frame-advance path,
  `self->frameTable[self->frameIndex]`'s bytes 4-7 (previously an
  opaque `unknown_04[4]` on `struct anim_frame_record`) turned out to
  be two real `s16` fields, now named `loopThreshold`/`loopBase`: once
  the frame base offset reaches `loopThreshold`, `field_08` steps back
  by `(loopThreshold - loopBase) << 8` and the `+0x12` flag gets set -
  a loop-back/wrap mechanism for the animation's playback position.
- **`DestroyJetpackCheckpointText`**/`DestroyJetpackExplosion`/`DestroyJetpackShot`/`DestroyJetpackPlane`/
  `DestroyJetpackBomber`/`DestroyJetpackCannonball`/`DestroyAirshipFireball`/`DestroyJetpackBalloon`/
  `DestroyJetpackParachuteNitro`/`DestroyJetpackRocket`/`DestroyJetpackRing`/`DestroyHovercraftFireball`/
  `DestroyHovercraftCannon`/`DestroyHovercraftLauncher`/`DestroyHovercraftSideGun`/`DestroyHovercraftCannonFlash` - 16 more
  byte-identical `struct linked_node` teardown handlers, the exact same
  shape as issue #71's 20: set `self->field_50` to
  `gActorVtable`, unlink `self` from its `+0x48`/`+0x4c`
  circular list, free `self` when `flags & 1`.
- **`UpdateJetpackExplosion`** - `UpdatePolarCheckpointText`'s near-twin: advances `self+0x24`
  (a Q8 fixed-point accumulator, `+170`/256 per call instead of
  `UpdatePolarCheckpointText`'s `-0x180`/256) and either fires the `+0x50` trampoline
  record if `self+0x12` is set, or tail-calls `UpdateActor(self)`
  otherwise.
- **`DestroyJetpackHealthCrate`/`DestroyJetpackTimeCrate`/`DestroyJetpackQuestionCrate`** - a third teardown
  shape: tear down via `DestroyJetpackBalloonCrate(self, 0)` (itself still unmatched)
  instead of the inline list-unlink, then free `self` when `flags & 1`
  - same as every other handler here.
- **`GetActorHp`**/`nullsub_44`/`IsJetpackPlayerUnshootable` - see "Seven more hidden
  functions" above.

### Compiler-codegen notes

- **The same `ptr + int` add-operand-order canonicalization as issue
  #71's `GetAnimFrameData`**, hit again in `UpdateJetpackCheckpointText`'s
  `rec = &table[idx]`: this compiler always canonicalizes pointer
  arithmetic so the pointer operand ends up as the final `ADD`'s first
  source, encoding `add r3,r3,r1` (table's register stays destination)
  where the ROM has `add r1,r1,r3` (the multiply accumulator's register
  stays destination, table added in). No amount of reordering the
  `+`'s C-level operands, introducing a `register`-pinned intermediate,
  or restructuring as compound assignment on a plain `int` changed it -
  only fully leaving the pointer type behind fixed it: compute the
  byte offset as `s32 off`, `off += (s32)table;` (self-referential
  compound assignment onto the *offset* variable, not the pointer),
  then cast `off` to the record pointer type at the end. Same
  root cause and same integer-cast fix as issue #71's entry for this
  exact gap - written up again here since the earlier isolated-compile
  note didn't carry over as an obvious search hit while writing this
  chunk from scratch.
- **A scratch-register choice for a small immediate, fixed with an
  inline-asm anchor.** `UpdateJetpackCheckpointText`'s `self->field_08 +=
  *(s16*)(self+0x10)` needs a register to hold the immediate `16`
  offset for `ldrsh` (no immediate encoding exists for that op); every
  plain-C phrasing tried (compound assignment, a separate `s16 delta`
  local declared before/after the `s32` accumulator, a `register s32
  off asm("r3") = 0x10` pin - discarded by constant propagation exactly
  like the precedent in `docs/matching.md`'s `GetSpriteHitbox` entry) landed
  the constant in `r0` where the ROM has `r3`. A two-instruction inline
  `asm("mov r3, #0x10\n\tldrsh r1, [r4, r3]" : "=r"(delta) : : "r3")`
  anchor, with `delta` declared `s32` (not `s16`, to avoid a redundant
  sign-extension pair gcc can't see through its own `ldrsh`'s implicit
  extension) and `register ... asm("r1")`-pinned to receive it, closes
  the gap exactly - an established technique
  (`matching_decomp_register_pinning`), just needed at instruction
  granularity here rather than for a whole local.
- **`self` pinned to `r4` for the whole function.** Without the pin,
  this compiler puts `self` in a scratch register for the early-return
  (trampoline-dispatch) branch, since its last use there is right
  before a tail call and nothing forces a callee-saved home; but it
  still needs `r4` for the other branch (used again after the
  `GetAnimFrameBaseOffset()` call crosses a call boundary). The ROM
  picks `r4` for both branches uniformly - `register u8 *self
  asm("r4") = selfArg;` reproduces that. A `struct anim_part_instance *`
  view of the same pointer was *not* kept as a second local alongside
  the raw `u8 *self` (unlike issue #71's approach, which extended that
  struct for `field_08`) - keeping both alive at once here costs an
  extra register and a spurious `mov` the ROM doesn't have; the struct
  cast is only needed transiently, for the `GetAnimFrameBaseOffset()`
  call argument itself.

### A real bug caught by the full clean rebuild

`UpdateJetpackExplosion`'s `self+0x12` "held"-flag check was first transcribed as
`self[0x18]` - a plain misread of the raw disassembly's `ldrb
r0,[r2,#18]` (decimal `18` = hex `0x12`, not `0x18`; this file's
disassembler only adds a `@ 0x..` hex comment for offsets roughly
`>= 0x40`, so the smaller ones are easy to misread as already being
hex). An isolated per-function compile of `UpdateJetpackExplosion` alone still
"matched" against a naively-reread expected snippet, since both sides
of that comparison shared the same misreading - only step 6's full
clean `make compare` against the real `baserom.gba`-derived checksum
caught the two differing bytes at `0x0803B586`, exactly the kind of
step-6-only bug `docs/workflow.md` step 3 warns about. Fixed by
rereading the original offset as decimal.

## Verification

Full clean rebuild, twice (once before, once after the `self[0x18]`
fix):

```
rm -rf build && make NON_MATCHING=1 report
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
```

`make compare` prints `La suma coincide` on the current tree.
