# `sub_803B46C` progress: 88% instruction match, still NAKED

`sub_803B46C` (`src/graphics/actor_anim.c`, screen-space visibility
test and OAM setup for a sprite frame drawn at the fixed screen
position (120, 106)) is still a byte-correct NAKED asm transcription
for the default build - see
[issue-71-0x0803b060-actor.md](./issue-71-0x0803b060-actor.md) for the
original parking rationale (shared with its near-identical twin
`sub_802C2FC`, `src/graphics/actor_part19b.c`). This doc records a
closer C reconstruction (88% instruction match, kept in the file under
`#if NON_MATCHING`) so a future attempt doesn't have to re-derive this
ground.

## What's closed

- **The `| 0`-with-a-zero-valued-local dead store**: the ROM keeps a
  real `mov r0, #0` / `orr r3, r0` pair the compiler always eliminates
  from a plain `a0 |= 0;` (it can prove the OR is a no-op) - closed via
  the established `asm volatile("orr %0, %0, %1" : "+r"(a0) :
  "r"(zero))` opaque-asm idiom.
- **Not pinning `frame` to r7**: an explicit `register u8 *frame
  asm("r7")` (needed since `frame` survives both calls) made this
  compiler drop r7 from the push/pop list entirely - the categorical
  r7-pin hazard documented throughout this project. Leaving `frame` as
  a plain unpinned local lets the natural allocator land it in r7 (and
  correctly preserve it) on its own, the same fix already used for
  `sub_80157C4` (`docs/matching/naked-sub_80157c4-matched.md`).
- **A genuine formula bug caught along the way**: the right-edge bounds
  check reuses the *already-shifted* `wShift` value (`wShift << 1`,
  i.e. `w << 3`) rather than recomputing from the raw `w` (`w << 1`,
  a different and wrong value) - the ROM's `lsl r0, r2, #1` operates on
  r2 (still holding `wShift` from the earlier `w << 2`), not on a
  fresh `w` load. An early draft wrote `x + (w << 1)`, which compiled
  without error but was a real semantic bug, not just a register
  mismatch - caught by comparing instruction *operands* against the
  ROM, not just mnemonics.

## What's still open (~12% residual, all register-choice, zero semantic impact)

The remaining diffs are scattered register-choice differences in the
`self+0x18` priority/palette-nibble unpack and the final
`SetupSpriteFrameOam` argument shuffle (e.g. the ROM computes `lsl r1,
r4, #16` into r1 and `orr`s from there, this reconstruction keeps the
shift in r4 in place) - the same class of small, individually-
unpredictable register-allocation choices this project's other
near-miss docs describe. Several iterations closed most of the
function; the remainder didn't converge with the effort budget
available this pass - left for a future attempt, possibly starting
from `sub_802C2FC`'s own twin gap once one of the pair closes fully.

## Verification

The `#if NON_MATCHING` reconstruction: `rm -rf build && make
NON_MATCHING=1 report` succeeds (including the `arm-none-eabi-as`
assemble-verification step, no warnings); instruction match against
`build/expected/units/raw_0803B46C_target.o` via `objdiff-cli diff`
sits at 88% for `sub_803B46C`.

The default (NAKED) build: full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`. Unchanged from
before this investigation; `tools/report_units.py`'s entry for
`0x0803B46C` stays `base_object=None` (still parked, not matched).
