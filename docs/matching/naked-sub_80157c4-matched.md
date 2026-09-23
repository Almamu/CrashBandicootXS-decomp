# `sub_80157C4` progress: 99.8% instruction match, still NAKED

`sub_80157C4` (`src/graphics/actor_part38d.c`, player mode-remapper
tail-calling `sub_800B86C`) is still a byte-correct NAKED asm
transcription for the default build - see
[issue-18-0x08014f8c-actor.md](./issue-18-0x08014f8c-actor.md)'s
"Parked, not matched: sub_80157C4" for the original parking rationale.
This doc records a near-complete C reconstruction (99.8% instruction
match, kept in the file under `#if NON_MATCHING`) so a future attempt
doesn't have to re-derive most of this.

## What's closed

Both gaps the original parking noted are closed:

- **The `0xd`/`0x18` case-pair merge**: the original parking found
  this compiler flattens an `if`/`else if` chain testing `mode==0xd ||
  mode==0x18` into two sequential compares sharing one target, rather
  than the ROM's own `cmp #0x12/beq`, `cmp #0x12/bgt`, `cmp #0xd/beq`,
  (fallthrough) `cmp #0x18/beq` decision triangle. Writing the control
  flow as explicit `goto`s matching the ROM's own block layout exactly
  (one label per ROM branch target, no `if`/`else` restructuring at
  all) reproduces the triangle byte-for-byte - this project's usual
  "translate the disassembly's control flow directly, don't re-infer
  it as structured C" convention applies to branch *shape* just as
  much as to instruction *choice*.
- **The `r8` register-allocation pull**: the original parking found
  gcc 2.9 wanted to spill the "running mode copies" value into `r8`
  where the ROM keeps everything in `r0`-`r7`. Not reproduced at all
  in this pass - plain `s32 mode` (no register pin) stayed in `r5`
  throughout without any `r8` pull, for reasons not fully understood
  (possibly a difference in exactly which values are simultaneously
  live at any point, following from the different control-flow
  shape above).
- **A prologue instruction-order gap found and fixed along the way**:
  explicitly pinning `arg0` to `register void *self asm("r6")` made
  this compiler materialize `r7` (the *second* argument) into its
  callee-saved home *before* `r6` (the first), rather than the ROM's
  own `r6`-then-`r7` order - the opposite of what the pin was for.
  Referencing `arg0` directly at its one use site (no local alias at
  all, register-pinned or otherwise) let the natural argument-shuffle
  order match the ROM's without any pin - the same class of "explicit
  pin changes an *unrelated* instruction's order" sensitivity
  documented elsewhere in this project (e.g. `sub_8039AA4`'s doc), but
  fixed here by removing a pin rather than adding one.

## What's still open (1 cosmetic residual, zero semantic impact)

The epilogue's `pop`-into-scratch-register-then-`bx` return sequence
(the standard Thumb pattern for restoring `lr` and returning via BX,
since Thumb1 has no `pop {pc}`) picks a different scratch register:
the ROM uses `r1` (`pop {r1}` / `bx r1`), this reconstruction gets
`r0` (`pop {r0}` / `bx r0`). Every register-pinning/statement-order
variation tried (pinning the call's own arguments, deferring or
reordering the final `sub_800B86C` call's own argument evaluation)
left this unchanged - it appears to be a low-level epilogue-generation
heuristic not exposed to C-level influence, not a phrasing gap. Purely
cosmetic: both sequences copy `lr` into a caller-clobbered scratch
register and branch through it identically.

## Verification

The `#if NON_MATCHING` reconstruction: `rm -rf build && make
NON_MATCHING=1 report` succeeds (including the `arm-none-eabi-as`
assemble-verification step, no warnings); instruction match against
`build/expected/units/raw_080157C4_target.o` via `objdiff-cli diff`
sits at 99.8% for `sub_80157C4`.

The default (NAKED) build: full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` - `crashbandicootxs.gba: La suma coincide`. Unchanged from
before this investigation; `tools/report_units.py`'s entry for
`0x080157C4` stays `base_object=None` (still parked, not matched).
