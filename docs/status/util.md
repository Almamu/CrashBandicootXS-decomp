# Status: util

`src/util/` - math, string/printf, RNG, line-drawing, time formatting
helpers.

## Matched

- `src/util/fixed_math.cpp`: `FixedDistSq`, `FixedDist`, `FixedDiv`,
  `FixedMul`, `FixedInverse16`, `FixedDiv16`, `FixedMul16`
- `src/util/number_format.cpp`: `itoa`, `FormatPaddedNumber`
- `src/util/printf.cpp`: `vsprintf`, `sprintf`, `FindSubstring`
  (case-insensitive `strstr` - was previously NAKED, now matched as
  real C via opaque inline-asm-materialized lowercase folds plus
  deferring the inner loop's match-found computation to a label placed
  after the whole scan/verify loop so gcc's block linearizer places it
  right before the shared epilogue, matching the ROM's own layout - see
  [naked-sub_8000cbc-matched.md](../matching/archive/naked-sub_8000cbc-matched.md))
- `src/util/string.cpp`: `CountNonSpaceChars`, `strcat`, `strncpy`,
  `strcpy`, `strlen`
- `src/util/rand.cpp`: `srand`, `RandRange`, `rand`
- `src/util/line.cpp`: `InitBresenhamLine`
- `src/util/time_format.cpp`: `FormatCentiseconds`
- `src/text/text_box.cpp`: `GetWordLength`, `DrawWrappedTextInBox`
- `src/util/line_step.cpp`: `StepBresenhamLine`
### libgcc (`lib/libgcc/`, a library - see [docs/libraries.md](../libraries.md))

- `lib/libgcc/libgcc2.c` (GitHub issue #66, ROM
  `0x08037648`-`0x08037E54` and `0x08037ECC`-`0x08037F3C`), compiled once
  per function (`_divdi3.o`, `_udivdi3.o`, `_muldi3.o`): `__divdi3`
  (UNUSED), `__udivdi3`, `__muldi3` - linked in with GAX2. Built without
  `-mthumb-interwork` (the Makefile's `NO_INTERWORK_OBJS`): the ROM's
  combined `pop {r4-r7, pc}` returns are just agbcc's non-interworking
  epilogue, and with the flag dropped these are libgcc2.c's own source,
  verbatim (`lib/libgcc/libgcc2_udivmoddi4.h` holds `__udivmoddi4`). See
  `docs/matching/archive/gax-toolchain-retry.md`. (`GaxZeroFill`, the zero-fill
  helper that used to share this file, is `lib/gax/src/gax_zero_fill.c` -
  it's GAX2 engine code with a normal interworking return.)
- `lib/libgcc/lib1funcs.s`: lib1funcs.asm's hand-written routines,
  assembled once per routine - `__udivsi3` (ROM `0x08037E54`, between
  `__udivdi3` and `__muldi3`), and `_call_via_r0`-`_call_via_r7`/
  `_call_via_lr`, `__divsi3`, `__div0`, `__modsi3`, `__umodsi3` (ROM
  `0x0803AD78`-`0x0803AFDC`). Never C (per-path register saves, `ror`,
  `mov pc, lr` returns), so they are byte-verified transcriptions,
  excluded from progress (`HANDWRITTEN`). They were NAKED functions in C
  files before (GitHub issues #66/#69/#70, see
  `docs/matching/archive/issue-69-eeprom-timer.md`'s "NAKED transcription pass").

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

### NAKED transcription (byte-exact, but not real decompiled C)

These functions produce byte-exact ROM output, but only because the
entire function body is hand-transcribed disassembly wrapped in inline
`asm()` - the C-level matching attempt failed and the raw bytes got
embedded as asm instead. They're tracked as parked, not matched.

None in `src/util/` (libgcc's hand-written routines are `lib/libgcc/lib1funcs.s`, see above).

## Other notes

- `ShowBitmapScreen` (ROM `0x080007EC`, not yet converted to C at all) is
  understood but not yet byte-matching - an instruction-scheduling detail.
  `asm/code_3_1.s` was split into itself (just this one function) plus
  `asm/code_3_1_2.s` (everything after it) so the functions around it
  could still be matched - see `docs/matching.md` for the pattern to reuse
  if this happens again.
