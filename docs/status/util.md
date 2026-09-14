# Status: util

`src/util/` - math, string/printf, RNG, line-drawing, time formatting
helpers.

## Matched

- `src/util/math_util.c`: `sub_80008B4`, `sub_80008CC`, `sub_80008F0`,
  `sub_80008FC`, `sub_800090C`, `sub_8000924`, `sub_800093C`
- `src/util/string_util.c`: `sub_800094C`, `sub_80009F4`
- `src/util/printf_util.c`: `sub_8000AA8`, `sub_8000CA8`
- `src/util/string_util2.c`: `sub_8000D68`, `sub_8000D80`, `sub_8000DAC`,
  `sub_8000DE0`, `sub_8000DF8`
- `src/util/rand_util.c`: `sub_8000E10`, `sub_8000E1C`, `sub_8000E4C`
- `src/util/line_util.c`: `sub_8000E6C`
- `src/util/time_util.c`: `sub_800106C`
- `src/util/word_util.c`: `sub_80011F4`, `sub_8001214`
- `src/util/line_util2.c`: `sub_8001254`

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8000CBC`** (`src/util/printf_util.c`, a case-insensitive
  `strstr`) - matching a specific redundant-truncate branch shape in its
  lowercase-fold logic conflicts with keeping `caseInsensitive` out of
  `r8` - see `docs/matching.md`, "Parked, not matched: `sub_8000CBC`".

## Other notes

- `sub_80007EC` (ROM `0x080007EC`, not yet converted to C at all) is
  understood but not yet byte-matching - an instruction-scheduling detail.
  `asm/code_3_1.s` was split into itself (just this one function) plus
  `asm/code_3_1_2.s` (everything after it) so the functions around it
  could still be matched - see `docs/matching.md` for the pattern to reuse
  if this happens again.
