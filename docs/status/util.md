# Status: util

`src/util/` - math, string/printf, RNG, line-drawing, time formatting
helpers.

## Matched

- `src/util/math_util.c`: `FixedDistSq`, `FixedDist`, `FixedDiv`,
  `FixedMul`, `FixedInverse16`, `FixedDiv16`, `FixedMul16`
- `src/util/string_util.c`: `itoa`, `FormatPaddedNumber`
- `src/util/printf_util.c`: `vsprintf`, `sprintf`, `FindSubstring`
  (case-insensitive `strstr` - was previously NAKED, now matched as
  real C via opaque inline-asm-materialized lowercase folds plus
  deferring the inner loop's match-found computation to a label placed
  after the whole scan/verify loop so gcc's block linearizer places it
  right before the shared epilogue, matching the ROM's own layout - see
  [naked-sub_8000cbc-matched.md](../matching/naked-sub_8000cbc-matched.md))
- `src/util/string_util2.c`: `CountNonSpaceChars`, `strcat`, `strncpy`,
  `strcpy`, `strlen`
- `src/util/rand_util.c`: `srand`, `RandRange`, `rand`
- `src/util/line_util.c`: `InitBresenhamLine`
- `src/util/time_util.c`: `FormatCentiseconds`
- `src/util/word_util.c`: `GetWordLength`, `sub_8001214`
- `src/util/line_util2.c`: `StepBresenhamLine`
- `src/util/math_div_util.c` (new file, GitHub issue #70, ROM
  `0x0803AE48`-`0x0803AE4C`): `__div0` (shared divide-by-zero
  handler) - a genuinely trivial no-op stub with no real C logic to
  express. (This file's `__divsi3`/`__modsi3`/`__umodsi3` are
  NAKED transcriptions tracked as parked - see below.)
- `src/util/math_div64_util.c` (GitHub issue #66, ROM
  `0x08037648`-`0x08037F3C`): `__divdi3` (UNUSED),
  `__udivdi3`, `__muldi3` - GAX2's
  bundled libgcc2.c code. This object is built without
  `-mthumb-interwork` (the Makefile's `NO_INTERWORK_OBJS`): the ROM's
  combined `pop {r4-r7, pc}` returns are just agbcc's non-interworking
  epilogue, and with the flag dropped these are libgcc2.c's own source,
  verbatim (`include/libgcc2_udivmoddi4.h` holds `__udivmoddi4`). See
  `docs/matching/gax-toolchain-retry.md`. (`GaxZeroFill`, the zero-fill
  helper that used to share this file, moved to
  `src/audio/gax_zero_fill.c` - it's GAX2 engine code with a normal
  interworking return.)

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

### NAKED transcription (byte-exact, but not real decompiled C)

These functions produce byte-exact ROM output, but only because the
entire function body is hand-transcribed disassembly wrapped in inline
`asm()` - the C-level matching attempt failed and the raw bytes got
embedded as asm instead. They're tracked as parked, not matched.

- **`__divsi3`** (`src/util/math_div_util.c`, signed division),
  **`__modsi3`** (signed modulo), **`__umodsi3`** (unsigned
  modulo) - the ROM's per-path prologue/epilogue register-save
  minimization, and `__modsi3`/`__umodsi3`'s `ror` codegen, that
  agbcc's plain-C codegen can't reproduce. GitHub issue #70, see
  `docs/matching/issue-69-eeprom-timer.md`'s "NAKED transcription pass"
  section.
- **`__udivsi3`** (`src/util/math_div64_util.c`) -
  lib1funcs.asm's hand-written Thumb routine (per-path `push {r4}` /
  `push {lr}; bl __div0` shapes, `mov pc, lr` return), not compiler
  output, so NAKED is its legitimate final state - the same situation
  as `math_div_util.c`'s trio above. GitHub issue #66, see
  `docs/matching/gax-toolchain-retry.md`.

## Other notes

- `sub_80007EC` (ROM `0x080007EC`, not yet converted to C at all) is
  understood but not yet byte-matching - an instruction-scheduling detail.
  `asm/code_3_1.s` was split into itself (just this one function) plus
  `asm/code_3_1_2.s` (everything after it) so the functions around it
  could still be matched - see `docs/matching.md` for the pattern to reuse
  if this happens again.
