# expected/

Frozen, never-edited copy of the original hand-disassembled ROM code, kept
purely so [decomp.dev](https://decomp.dev)'s progress report has a
byte-exact "target" (ground truth) object to diff the current, in-progress
source tree's "base" object against - see [`docs/decomp_dev.md`](../docs/decomp_dev.md)
for the full explanation and how the two sides are built.

`code_3.s` is `asm/code_3.s` as it existed at commit `710cc9a` (the last
commit before any function was ever cut out of it for matching, and the
commit right after every function it contains was given its final,
descriptive name where one was known) - i.e. exactly what the ROM's code
disassembles to, using the same symbol names the current `src/*.c` files
use for anything since matched. Verified byte-identical to that commit's
blob via `git hash-object`.

**Never edit this file.** It isn't meant to reflect current understanding
of the code (that's what `src/*.c` and the remaining `asm/*.s` files are
for) - it exists solely as an unchanging comparison baseline. If it turns
out to be wrong or incomplete somehow, regenerate it from the same commit
rather than hand-patching it.

Does **not** currently cover `src/system/main.c`/`memory.c`/`irq.c`: those were
matched even earlier, before this repo settled on the `code_3.s` naming/
splitting convention, and reconstructing their original pristine
disassembly would need walking further back through several more splits
of the very first `asm/code.s`. They're excluded from the progress report
for now rather than guessed at - see `docs/decomp_dev.md`.
