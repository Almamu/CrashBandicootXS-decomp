# `sub_803A03C` progress: 81.3% instruction match, still NAKED

`sub_803A03C` (`src/audio/gax_channel_pos_sweep.c`, per-tick ping-pong
position sweep) is still a byte-correct NAKED asm transcription for the
default build - see
[issue-68-0x08039818-audio.md](./issue-68-0x08039818-audio.md) for the
original parking rationale. This doc records a C reconstruction (81.3%
instruction match against `raw_0803A03C_target.o`, kept in the file
under `#if NON_MATCHING`) so a future attempt doesn't have to re-derive
the ground already covered.

## What's closed

Register-pinning `self` to `r2` and the columnar table pointer to `ip`
(both matching ROM's own choices exactly), plus the same `r8`-pin and
OR-fold techniques used elsewhere in this project, reproduce the whole
control-flow shape and semantics correctly, and get the function's
first ~15 instructions (the armed-gate check, the countdown
decrement/store/reload-free zero-test, and the `r8 = 0xff` load) to
byte-exact match:

- **The countdown decrement's "test the same register, don't reload"
  idiom**: `s[0x14] = s[0x14] - 1;` followed by testing `(u8)countdown
  != 0` against a *local* holding the sub result (not a fresh `ldrb`
  reload of `self+0x14`) reproduces ROM's exact `sub / strb / mov
  r1,#0xff / mov r8,r1 / lsl r0,#0x18 / cmp r0,#0` sequence with no
  extra truncation instructions - as long as the local stays a plain
  `s32` and the truncation is expressed as a cast at the *comparison*
  site (`(u8)countdown != 0`), not baked into the local's own
  declared type (declaring the local itself as `u8` makes gcc emit an
  extra `lsl/lsr` truncation pair before the store that ROM doesn't
  have).
- **The `self+0x13` OR-with-0xff idiom and the `r8` pin**: identical
  techniques to `sub_8039AA4` (`docs/matching/naked-sub_8039aa4-matched.md`)
  - `register s32 zero8 asm("r8") = 0xff;` set once early, and
    `s[0x13] = (u8)(zero8 | oldDir);` instead of a bare `= 0xff`, which
    this compiler would otherwise constant-fold away.
- **Not caching `self+0x10` (`idx`) or the table row index into a
  named local** - referencing `s[0x10]` directly inline at every
  `tablePtr + s[0x10] * 28 + OFFSET` access (rather than assigning
  `idx = s[0x10];` once and reusing the local) avoids a gcc scheduling
  quirk where the compiler otherwise hoists a *later* (else-branch-only)
  reload of `self+0x10` up above the earlier `ble`/`bge` branch,
  producing an instruction ROM doesn't have at that point at all.

## What's still open (~18 residual instructions, all register-choice/instruction-selection, zero semantic impact)

The remaining diffs are concentrated in the five-field columnar-table
addressing (`tablePtr + idx*28 + {0x14,0x18,0x1c,0x20,0x24}`) and
split into two related but distinct gaps, neither closed despite
several structurally different C phrasings:

1. **Register-choice-only diffs** (the bulk of the residual): which
   scratch register holds `posAccum` (ROM: `r6`; mine: `r7`), the
   cached `oldDir` byte (ROM: `r7`; mine: `r5`), and similar - these
   ripple through the rest of the function once set, but carry no
   semantic difference (e.g. `ldr r7, [r2, #0x48]` vs ROM's
   `ldr r6, [r2, #0x48]`).
2. **A genuine instruction-selection difference for the repeated
   `tablePtr + 0x20 + idx*28` address** (used twice, once per branch,
   for the "step toward the bound" and "bounce off the bound" reads):
   ROM computes the constant-offset base (`tablePtr + 0x20`) as its
   own three-instruction subexpression (`mov r5, ip` / `add r5, r5,
   #0x20` / then a *separate* `add r0, r5, r0` once `idx*28` is ready),
   and reuses that base across both reads in the same branch. Every C
   phrasing tried here - a raw `tablePtr + 0x20 + s[0x10]*28` (folds
   the `+0x20` into the load's own displacement instead, a 2-instruction
   `ldr [r,#0x20]` form) and an explicit `u8 *base20 = tablePtr +
   0x20;` local (which *does* reproduce the 3-instruction split, but
   also introduces an unwanted extra `idx`-copy instruction earlier
   and, worse, collapses the *two* separate-address-then-load
   instruction pairs into a single cached full-address reuse, net
   **regressing** match% from 81.3% to 75.9%) - couldn't reproduce
   ROM's exact middle ground (recompute the address components, reuse
   only the base, not the final address) without a worse tradeoff
   elsewhere. This looks like the same class of "many-register GAX2
   cluster" allocation-heuristic sensitivity the function's own doc
   comment already flagged, not a phrasing gap with an easy fix.

Given the size of this function (104 ROM instructions across two
branches and five distinct table-column accesses), and that every
attempted fix for gap 2 traded one instruction-selection match for a
different one elsewhere without net improvement, this is being parked
at 81.3% rather than chased further right now.

## Further attempts (2026-09-24 session): still 81.3%, no regression

A later session re-opened this function specifically to try to close
gap 2, using a faster isolated-compile/`objdiff-cli diff` loop (no
full rebuild per iteration) to try more phrasings quickly. None beat
the existing 81.3% baseline; all are recorded here so a future attempt
doesn't re-spend time on the same dead ends:

- **Register-pinning `posAccum`/`oldDir` to ROM's own registers** (`register
  s32 posAccum asm("r6")`, `register u8 oldDir asm("r7")`, matching
  ROM's choices instead of the compiler's own `r7`/`r5`): both pins
  individually made the match *worse* (measured 78.2%/78.3% vs
  baseline's 78.7% in this session's isolated-compile harness - the
  same relative ordering as the file's own 81.3% figure, just a
  different absolute baseline since this measurement used a direct
  `objdiff-cli diff` against a `tools/slice_expected.py` slice rather
  than the full project report). Pinning one register perturbs the
  compiler's other allocation choices enough to cost more instructions
  elsewhere than it fixes - consistent with this being a whole-function
  allocation cascade, not an independently-fixable single choice.
- **`s32 *col20 = (s32 *)(tablePtr + 0x20);` array-indexed as
  `col20[s[0x10] * 7]`** (instead of the byte-pointer-cast `base20`
  form already tried and documented above): produces the *identical*
  regression as the byte-pointer form (measured 73.1% in this
  session's harness, matching the byte-pointer form's own 73.1%
  exactly) - array-subscript syntax doesn't reach a different RTL
  shape than pointer-arithmetic-plus-cast for this compiler, so it's
  not a route around the CSE-collapse problem.
- **`*(volatile s32 *)(base20 + s[0x10] * 28)` on just the second
  `col20` read**, trying to force a genuine reload without also
  forcing full address recomputation: this *does* force a second
  `ldr`, but the register allocator keeps the *entire computed
  address* (not just `base20`) live in one register across the
  intervening code and reuses it whole for the second load - the
  opposite of what's needed (ROM keeps `base20` and `idx*28` cached
  *separately* and recombines them with a fresh `add` each time, one
  extra instruction ROM has that no volatile-based phrasing reproduced).
- **Modeling the table as a real struct indexed by row**, per this
  project's usual "prefer structs" cleanup step: ruled out as
  structurally invalid before even compiling it. The five columns this
  function touches sit at byte offsets 0x14/0x18/0x1c/0x20/0x24
  *within* each 28-byte-strided row, but 0x24 + a 2-byte read = 0x26
  (38 decimal) - past the 28-byte stride itself. A `PosSweepRow`
  struct sized to hold a field at offset 0x24 would necessarily be
  larger than the actual 28-byte stride between rows, which C's type
  system can't express as an array element (the field would land
  inside the *next* row's data at that point) - confirming the raw
  `tablePtr + idx*28 + OFFSET` pointer arithmetic already in the file
  isn't a stand-in for an easier struct form; it's the only C shape
  that can express this table's actual (overlapping/non-packed) layout
  at all.

Conclusion unchanged: parked at 81.3%, real C, under `#if
NON_MATCHING`; the shipped default build stays the byte-verified NAKED
transcription. No regression was introduced by this session - the
`#if NON_MATCHING` block in the file is unchanged from before this
investigation.

## Verification

The `#if NON_MATCHING` reconstruction: `rm -rf build && make
NON_MATCHING=1 report` succeeds (including the `arm-none-eabi-as`
assemble step); instruction match against
`build/expected/units/raw_0803A03C_target.o` via `objdiff-cli diff`
sits at 81.3% for `sub_803A03C`.

The default (NAKED) build: full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`. Unchanged from
before this investigation; `tools/report_units.py`'s entry for
`0x0803A03C` stays `base_object=None` (parked, not matched).
