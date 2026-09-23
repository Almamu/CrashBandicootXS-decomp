# Status: util

`src/util/` - math, string/printf, RNG, line-drawing, time formatting
helpers.

## Matched

- `src/util/math_util.c`: `sub_80008B4`, `sub_80008CC`, `sub_80008F0`,
  `sub_80008FC`, `sub_800090C`, `sub_8000924`, `sub_800093C`
- `src/util/string_util.c`: `itoa`, `sub_80009F4`
- `src/util/printf_util.c`: `sub_8000AA8`, `sub_8000CA8`, `sub_8000CBC`
  (case-insensitive `strstr` - was previously NAKED, now matched as
  real C via opaque inline-asm-materialized lowercase folds plus
  deferring the inner loop's match-found computation to a label placed
  after the whole scan/verify loop so gcc's block linearizer places it
  right before the shared epilogue, matching the ROM's own layout - see
  [naked-sub_8000cbc-matched.md](../matching/naked-sub_8000cbc-matched.md))
- `src/util/string_util2.c`: `CountNonSpaceChars`, `strcat`, `sub_8000DAC`,
  `strcpy`, `strlen`
- `src/util/rand_util.c`: `srand`, `sub_8000E1C`, `rand`
- `src/util/line_util.c`: `InitBresenhamLine`
- `src/util/time_util.c`: `FormatCentiseconds`
- `src/util/word_util.c`: `GetWordLength`, `sub_8001214`
- `src/util/line_util2.c`: `StepBresenhamLine`
- `src/util/math_div_util.c` (new file, GitHub issue #70, ROM
  `0x0803AE48`-`0x0803AE4C`): `nullsub_8` (shared divide-by-zero
  handler) - a genuinely trivial no-op stub with no real C logic to
  express. (This file's `sub_803ADB4`/`sub_803AE4C`/`sub_803AF1C` are
  NAKED transcriptions tracked as parked - see below.)
- `src/util/math_div64_util.c` (new file, GitHub issue #66, ROM
  `0x08037648`-`0x08037FA0`): `sub_8037F3C` (zero-fill memset helper via
  the BIOS `CpuFastSet` SWI) - the 64-bit division/multiply family's
  neighbor in this contiguous ROM slice, not part of the family itself
  (see below for the other four). See
  `docs/matching/issue-66-67-math-div64-util.md`.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED asm transcription (byte-correct, not decompiled C)

### NAKED transcription (byte-exact, but not real decompiled C)

These functions produce byte-exact ROM output, but only because the
entire function body is hand-transcribed disassembly wrapped in inline
`asm()` - the C-level matching attempt failed and the raw bytes got
embedded as asm instead. They're tracked as parked, not matched.

- **`sub_803ADB4`** (`src/util/math_div_util.c`, signed division),
  **`sub_803AE4C`** (signed modulo), **`sub_803AF1C`** (unsigned
  modulo) - the ROM's per-path prologue/epilogue register-save
  minimization, and `sub_803AE4C`/`sub_803AF1C`'s `ror` codegen, that
  agbcc's plain-C codegen can't reproduce. GitHub issue #70, see
  `docs/matching/issue-69-eeprom-timer.md`'s "NAKED transcription pass"
  section.
- **`sub_8037648`** (`src/util/math_div64_util.c`, signed 64-bit
  division - UNUSED, no caller anywhere in the ROM), **`sub_8037A7C`**
  (unsigned 64-bit division), **`sub_8037E54`** (unsigned 32-bit
  division, quotient only), **`sub_8037ECC`** (64x64->64 truncating
  multiply, this ROM's compiled `__muldi3`/`__umulsidi3`) - a
  non-interworking `pop {r4-r7, pc}` / bare `mov pc, lr` return
  convention this project's `-mthumb-interwork` build can't reproduce
  from any C phrasing (confirmed by direct isolated-compile experiment,
  not just precedent). GitHub issue #66, see
  `docs/matching/issue-66-67-math-div64-util.md`.

## Other notes

- `sub_80007EC` (ROM `0x080007EC`, not yet converted to C at all) is
  understood but not yet byte-matching - an instruction-scheduling detail.
  `asm/code_3_1.s` was split into itself (just this one function) plus
  `asm/code_3_1_2.s` (everything after it) so the functions around it
  could still be matched - see `docs/matching.md` for the pattern to reuse
  if this happens again.
