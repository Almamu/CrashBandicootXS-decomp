# decomp.dev integration

This project reports its matching progress to [decomp.dev](https://decomp.dev),
which reads a JSON report (in [objdiff](https://github.com/encounter/objdiff)'s
`Report` schema) uploaded as a GitHub Actions artifact on every push to `main`
(and on PRs, for its PR-comment feature) - see `.github/workflows/build.yml`'s
"Build progress report objects"/"Install objdiff-cli"/"Generate progress
report"/"Upload progress report artifact" steps.

## Why this needed more than just running objdiff-cli

`objdiff-cli report generate` diffs a **target** object (ground truth - what
the bytes should be) against a **base** object (what the current source
actually compiles to), symbol-by-symbol, and needs both sides to have
consistent, *unlinked* relocations so it can tell "calls a different function"
apart from "calls the same function, just at a different final address."

Most objdiff-integrated projects get their target objects by assembling an
unmodified copy of the original disassembly that they *never edit* as they
convert it to C - the C rewrite happens in a separate, editable tree. This
project didn't start that way: matched functions were **cut out of**
`asm/code_3_*.s` as they were matched, so there's no standing pristine copy
left to assemble a target object from directly.

**The fix**: `expected/code_3.s` is a frozen copy of `asm/code_3.s` as it
existed at commit `710cc9a`, the last point before any function was ever cut
out of it - i.e. exactly the original disassembly, with real names already
applied wherever they were known at that point. It is committed once and
**must never be edited again** (see `expected/README.md`). This is what
"pristine baseline" from the earlier discussion of this integration turned
into in practice: rather than repeatedly re-extracting bytes from
`baserom.gba` (which would lose per-symbol relocations and produce false
mismatches for anything containing a function call), git history already had
exactly the right frozen source sitting in it.

## What `make report` builds

Two objects, both under `build/expected/`:

- **`code_3.o`** (the target) - `expected/code_3.s` assembled as-is, then
  patched (see below) via `tools/patch_expected_target.py`. Contains every
  function in scope, whether it's been matched yet or not.
- **`base_combined.o`** (the base) - every currently-matched/parked
  `src/*.c` object (built under `NON_MATCHING=1`, so parked functions are
  included as their real - possibly imperfect - C reconstruction, not
  swapped out for raw asm) merged into one object via `arm-none-eabi-ld -r`.
  Deliberately **excludes** anything still living purely in `asm/*.s` -
  those functions simply don't appear in `base_combined.o`, so objdiff
  correctly reports them as "not yet attempted" (their bytes still count
  toward the total, but not toward the matched total) instead of trivially
  "100% matched" (which raw, unconverted asm would otherwise show, since
  by construction it still reproduces the ROM bytes exactly).

`objdiff.json` at the repo root points a single unit at these two objects.
Run `make NON_MATCHING=1 report` after a clean build (`rm -rf build`) to
produce them, matching [`docs/workflow.md`](./workflow.md)'s convention for
`NON_MATCHING` builds generally.

## `expected/corrections.txt`: patching the target without editing it

Since `expected/code_3.s` is frozen, it can't be corrected in place when
later matching work finds something the original disassembly got wrong -
namely:

- **A function got a real name only when (or after) it was matched**, not
  in the one dedicated renaming pass commit `710cc9a` itself already
  captured. Without a correction, it doesn't show up in the report at all,
  since objdiff pairs target/base symbols by name within a unit, and the
  frozen target still has the old `sub_XXXXXXXX` name.
- **A function boundary the original disassembly never split out** - it
  labelled a bigger span as one function, and later matching work found
  it's actually two. Without a correction, *both* functions report a wrong
  match percentage: the one the original disassembly did label appears
  too large (compared against extra bytes that belong to the other one),
  and the other doesn't appear at all.

`tools/patch_expected_target.py` applies `expected/corrections.txt` to the
*assembled* `code_3.o` via `objcopy --redefine-sym`/`--add-symbol` as part of
building it - never to the `.s` source. See the comment at the top of
`expected/corrections.txt` for the exact line format.

**When to add one**: whenever a newly-matched function doesn't show up in a
locally-generated `report.json` at all, or reports an unexpectedly low match
percentage that direct byte comparison against `baserom.gba` (the project's
own established verification method - see `docs/workflow.md`) says shouldn't
be there.

## A separate, known limitation: small residual percentages on real matches

`expected/code_3.s` never uses the `thumb_func_end` macro (see
`asm/macros/function.inc`), so none of its ~2045 symbols carry an explicit
ELF `.size` - objdiff infers each one's size from the distance to the next
label instead. For most functions this infers correctly, but a handful show
99-99.9% instead of 100% even though a direct byte comparison confirms
they're genuinely byte-exact: objdiff's inferred size includes a trailing
literal-pool constant or padding halfword that belongs to neither function
cleanly (there's no label marking exactly where one function's own literal
pool ends and the gap before the next function's code begins). This is
cosmetic - it doesn't affect whether a function is truly matched, only the
last fractional percentage point objdiff reports for it - and isn't worth
chasing down function-by-function; `expected/corrections.txt` is for the
two real problems above (missing name, missing boundary), not this one.

## Known gap: `main.c`/`memory.c`/`irq.c` aren't in the report yet

These three were matched even earlier than `asm/code_3.s` existed as a named
file (back when the whole ROM was still one `asm/code.s`), through several
more splits than `code_3.s` went through. Reconstructing their pristine
disassembly would mean walking further back through that history. They're
excluded from `objdiff.json`'s scope for now (neither counted as matched nor
as outstanding) rather than guessed at - a reasonable follow-up if someone
wants a fully complete percentage, but a small one relative to the whole ROM.

Graphics/audio/data extraction is out of scope for this report entirely -
`objdiff`/decomp.dev's progress model is about code, and this project's
asset extraction progress is tracked separately in
[`docs/graphics.md`](./graphics.md) and [`docs/audio.md`](./audio.md).

## Registering the project on decomp.dev

This part can't be scripted - it needs an interactive GitHub login:

1. Go to [decomp.dev/manage/new](https://decomp.dev/manage/new), sign in
   with GitHub, and pick this repository (you need admin access to it,
   which the repo owner has).
2. Fill in the game name, an optional short name, and platform
   **Game Boy Advance**.
3. Optionally install the decomp.dev GitHub App on the repo, so it's
   notified the moment a workflow run finishes instead of polling every
   5 minutes, and can post PR comments showing progress deltas.

Everything else - generating and uploading `report.json` in the shape
decomp.dev expects - is already handled by the CI workflow described above.
