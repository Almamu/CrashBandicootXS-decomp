# expected/

Frozen, never-edited copies of the original hand-disassembled ROM code, kept
purely so [decomp.dev](https://decomp.dev)'s progress report has a
byte-exact "target" (ground truth) object to diff the current, in-progress
source tree's "base" object against - see [`docs/decomp_dev.md`](../docs/decomp_dev.md)
for the full explanation and how the two sides are built. Two files, two
different historical eras of this project, together covering the whole
game's code:

- **`code_3.s`** is `asm/code_3.s` as it existed at commit `710cc9a` (the
  last commit before any function was ever cut out of it for matching, and
  the commit right after every function it contains was given its final,
  descriptive name where one was known) - i.e. exactly what the ROM's code
  disassembles to for everything matched from September 2026 onward
  (`src/graphics/`, `src/util/`, and part of `src/system/`), using the same
  symbol names the current `src/*.c` files use for anything since matched.
- **`legacy.s`** is `asm/code.s` (the ROM's *entire* code, in one file) as it
  existed at `8b090ca`, the very first commit - covering ROM addresses
  `0x08000170`-`0x080006A7`, i.e. `main.c`/`memory.c`/part of `irq.c`
  (`AgbMain` through `sub_8000680`), matched away in 2025, well before
  `code_3.s` ever existed as its own file. `AgbMain` was already named at
  this commit (a standard-enough GBA convention that it carried over from
  the reference project this repo started from); everything else here is
  still `sub_XXXXXXXX`, renamed via `expected/corrections.txt` instead.

Both verified byte-identical to their source commit's blob via
`git hash-object` (for `legacy.s`, of the exact line range extracted -
diffed directly against `git show 8b090ca:asm/code.s`, not just hashed).

**Never edit either file.** They aren't meant to reflect current
understanding of the code (that's what `src/*.c` and the remaining
`asm/*.s` files are for) - they exist solely as unchanging comparison
baselines. If one turns out to be wrong or incomplete somehow, regenerate
it from the same commit rather than hand-patching it; use
`expected/corrections.txt` for the small, known, expected discrepancies
instead (see `docs/decomp_dev.md`).
