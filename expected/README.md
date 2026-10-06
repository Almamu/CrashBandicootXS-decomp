# expected/

Frozen, never-edited copies of the original hand-disassembled ROM code, kept
purely so [decomp.dev](https://decomp.dev)'s progress report has a
byte-exact "target" (ground truth) object to diff the current, in-progress
source tree's "base" object against - see [`docs/decomp_dev.md`](../docs/decomp_dev.md)
for the full explanation and how the two sides are built. Three files:
two from different historical eras of this project, covering the ROM's
Thumb code, and one for the ARM code of the IWRAM image:

- **`code_3.s`** is `asm/code_3.s` as it existed at commit `710cc9a` (the
  last commit before any function was ever cut out of it for matching, and
  the commit right after every function it contains was given its final,
  descriptive name where one was known) - i.e. exactly what the ROM's code
  disassembles to for everything matched from September 2026 onward
  (then `src/graphics/`, `src/util/`, and part of `src/system/`), using the same
  symbol names the current `src/*.c` files use for anything since matched.
- **`legacy.s`** is `asm/code.s` (the ROM's *entire* code, in one file) as it
  existed at `8b090ca`, the very first commit - covering ROM addresses
  `0x08000170`-`0x080006A7`, i.e. `main.c`/`memory.c`/part of `irq.c`
  (`AgbMain` through `AddVBlankCallback`), matched away in 2025, well before
  `code_3.s` ever existed as its own file. `AgbMain` was already named at
  this commit (a standard-enough GBA convention that it carried over from
  the reference project this repo started from); everything else here is
  still `sub_XXXXXXXX`, renamed via `expected/corrections.txt` instead.

- **`iwram.s`** is different in origin: the IWRAM image's ARM code
  (IWRAM `0x03000000`-`0x030007CC`, stored in ROM at `0x087E55E4`) was
  never in any disassembly as code - it sat in `data/data.s` as one
  `.incbin` until it was decompiled (`asm/intr_main.s`, `src/iwram/`). So
  it was generated once from `baserom.gba` when that happened (ARM
  `objdump`, branch targets and literal pools as labels, pool words as
  the symbols they point at, function names as in `src/iwram/`), checked
  to reassemble to the same 0x7CC bytes, and frozen. Its labels carry
  IWRAM addresses. See docs/decomp_dev.md's "The IWRAM image".

`code_3.s` and `legacy.s` were both verified byte-identical to their source commit's blob via
`git hash-object` (for `legacy.s`, of the exact line range extracted -
diffed directly against `git show 8b090ca:asm/code.s`, not just hashed).

**Never edit any of the three `.s` files.** They aren't meant to reflect
current understanding of the code (that's what `src/*.c` and the remaining
`asm/*.s` files are for) - they exist solely as unchanging comparison
baselines. Why it matters: the progress report scores the build against
them, so a target edited to look like the current source would score
anything as matched, and the report would stop proving the code is
byte-exact. If one turns out to be wrong or incomplete somehow, regenerate
it from the same commit (or, for `iwram.s`, the same ROM bytes) rather
than hand-patching it; use `expected/corrections.txt` for the small,
known, expected discrepancies instead (see `docs/decomp_dev.md`).
`corrections.txt` is the one file here that changes: every rename of a
symbol the targets mention adds or updates its `rename` line (see
[`docs/naming.md`](../docs/naming.md), "What renaming touches").
