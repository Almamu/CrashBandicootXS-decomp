# Status: util

`src/util/` - math, string/printf, RNG, line-drawing, time formatting
helpers.

## Matched

- `src/util/math_util.c`: `sub_80008B4`, `sub_80008CC`, `sub_80008F0`,
  `sub_80008FC`, `sub_800090C`, `sub_8000924`, `sub_800093C`
- `src/util/string_util.c`: `itoa`, `sub_80009F4`
- `src/util/printf_util.c`: `sub_8000AA8`, `sub_8000CA8`, `sub_8000CBC`
  (case-insensitive `strstr`, NAKED - see "Compiler-limitation NAKED
  transcriptions" below)
- `src/util/string_util2.c`: `CountNonSpaceChars`, `strcat`, `sub_8000DAC`,
  `strcpy`, `strlen`
- `src/util/rand_util.c`: `srand`, `sub_8000E1C`, `rand`
- `src/util/line_util.c`: `InitBresenhamLine`
- `src/util/time_util.c`: `FormatCentiseconds`
- `src/util/word_util.c`: `GetWordLength`, `sub_8001214`
- `src/util/line_util2.c`: `StepBresenhamLine`
- `src/util/math_div_util.c` (new file, GitHub issue #70, ROM
  `0x0803ADB4`-`0x0803AFDC`): `nullsub_8` - the shared divide-by-zero
  handler for the parked division/modulo trio below

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Compiler-limitation NAKED transcriptions (matched, not plain C)

- **`sub_8000CBC`** (`src/util/printf_util.c`, a case-insensitive
  `strstr`) - a full C reconstruction matched the ROM everywhere except
  a specific redundant-truncate branch shape in its lowercase-fold
  logic that conflicted with keeping `caseInsensitive` out of `r8`;
  converted to a byte-verified NAKED asm transcription instead (see
  `src/util/printf_util.c`'s own doc comment, and the general pattern
  established by `src/system/link_cable.c`'s `sub_8001CB8`).

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_803ADB4`/`sub_803AE4C`/`sub_803AF1C`** (`src/util/math_div_util.c`,
  GitHub issue #70) - signed division, signed modulo, and unsigned
  modulo software primitives (shift-and-subtract binary long division).
  `sub_803ADB4`'s C reconstruction matches the ROM instruction-for-
  instruction in its body; all three are blocked purely on a
  prologue/epilogue shape (real per-path register-save minimization the
  ROM has that agbcc's plain-C codegen doesn't reproduce) - see
  `docs/matching.md`'s issue #70 entry for the full gap analysis.

## Other notes

- `sub_80007EC` (ROM `0x080007EC`, not yet converted to C at all) is
  understood but not yet byte-matching - an instruction-scheduling detail.
  `asm/code_3_1.s` was split into itself (just this one function) plus
  `asm/code_3_1_2.s` (everything after it) so the functions around it
  could still be matched - see `docs/matching.md` for the pattern to reuse
  if this happens again.
