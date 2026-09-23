# `sub_8039AA4` progress: 99.7% instruction match, still NAKED

`sub_8039AA4` (`src/audio/gax_channel_envelope_tick.c`, per-tick
envelope/portamento-pitch update) is still a byte-correct NAKED asm
transcription for the default build - see
[issue-68-channel-bind-envelope-note.md](./issue-68-channel-bind-envelope-note.md)
for the original parking rationale. This doc records a much closer C
reconstruction (99.7% instruction match, kept in the file under `#if
NON_MATCHING`) and independently reconfirms the exact residual the
original parking finding described, so a future attempt doesn't have
to re-derive any of this.

## What's closed

A full C reconstruction (register-pinning `self` to r4, and each
struct-field pair's load registers to match the ROM's own choices)
gets every instruction's operation and operand right except two, using
techniques already established elsewhere in this project:

- **The ROM's `r6=0`, computed once early and reused for both trailing
  zero-stores** (`strh r6, [r4, #0x32]` / `strh r6, [r4, #0x30]`),
  reproduced with a single `register s32 zero asm("r6");` set once
  right after the first clamp block (matching the ROM's own
  `movs r6, #0` position) instead of two fresh `mov r1, #0` immediates.
- **The portamento-apply add's stubborn 3-register operand-order
  canonicalization** (`add r0, r5, r1`, not `add r1, r1, r5`) - no
  rewriting of the C addition's operand order changed which register
  this compiler picked as the destination; the same "single opaque
  `asm volatile` add" fix documented in the register-pinning memory
  notes (point 6) closes it:
  ```c
  asm volatile("add %0, %1, %2" : "=r"(newVal) : "r"(sum26), "r"(slideRate));
  ```
- **Struct-field-pair load ordering and registers** for the `0x28`/
  `0x26` and `0x2c`/`0x2a` accumulator adds - matched by declaring each
  pair as two register-pinned locals in the ROM's own load order,
  e.g. `register s32 v2c asm("r0") = ...; register s32 v2a asm("r2")
  = ...;` (note: declared in the *opposite* order from the struct's
  own field order, matching the ROM's own `0x2c`-before-`0x2a` load
  sequence).
- **A genuine semantic subtlety**: the ROM's portamento-apply step
  (`add r0, r5, r1`) reuses the raw, not-yet-truncated 32-bit sum from
  the earlier `self->0x26 += self->0x28` step (still live in r5) as
  one of the add's inputs, while the "don't overshoot" sign-check reads
  (`ldrsh ... [r4, #0x26]`) go through fresh memory reloads instead of
  reusing that same cached value - both correct (the memory always
  holds what the cached register holds, since it was just stored
  there), but only one specific C-level phrasing reproduces the exact
  ROM instruction for each: a local (`sum26`) reused directly for the
  add, alongside separate `*(s16 *)(s + 0x26)` pointer dereferences
  (not references to `sum26`) for the sign checks.

## What's still open (2 residuals)

Two `ldrsh`-with-register-offset reads in the portamento tail (the
`self+0x32` sign-check reload, and the `self+0x30` target reload)
don't land in the ROM's exact scratch registers from plain C. Pinning
either one's offset register individually **independently reconfirms
the exact finding the original NAKED parking already recorded**:
forcing either read's register perturbs the *earlier*, already-correct
clamp blocks' own register choices (`ldrb r2, [r4, #0x15]` /
`ldrb r5, [r4, #0x17]` drift to `r3`/`r1` once either later pin is
added), even though nothing about those two reads and the clamp blocks
overlaps in the C source's own data flow. This is consistent with
gcc-2.9's hard-register variable reservations not being scoped as
tightly as their C block - a pin's effect on register availability
apparently isn't purely local to its own lexical scope. Every
combination tried (pinning one read at a time, both together, via a
raw materialized `ldrsh` instruction with an explicit offset-register
operand) reproduced the same ripple, matching the original attempt's
own conclusion almost exactly.

The 2 remaining instructions:

```
ROM:  mov r3, #0x32       MINE: mov r2, #0x32
      ldrsh r0, [r4, r3]        ldrsh r0, [r4, r2]
...
ROM:  mov r0, #0x30       MINE: mov r3, #0x30
      ldrsh r2, [r4, r0]        ldrsh r2, [r4, r3]
```

Both are pure register-choice differences with **zero** semantic
impact. A future attempt could try isolating the whole tail (from the
`self->0x32` check onward) into its own small helper function, giving
the register allocator a fresh, smaller scope independent of the two
clamp blocks earlier in the function - the same idea floated for
`sub_8000EE4`'s own residual (`naked-sub_8000ee4-progress.md`).

## Verification

The `#if NON_MATCHING` reconstruction: `rm -rf build && make
NON_MATCHING=1 report` succeeds; instruction match against
`build/expected/units/gax_channel_envelope_tick_target.o` via
`objdiff-cli diff` sits at 99.7% for `sub_8039AA4`.

The default (NAKED) build: full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`. Unchanged from
before this investigation; `tools/report_units.py`'s entry for
`0x08039AA4` stays `base_object=None` (parked, not matched).
