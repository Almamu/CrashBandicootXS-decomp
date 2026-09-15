# decomp.dev integration

This project reports its matching progress to [decomp.dev](https://decomp.dev),
which reads a JSON report (in [objdiff](https://github.com/encounter/objdiff)'s
`Report` schema) uploaded as a GitHub Actions artifact on every push to `main`
(and on PRs, for its PR-comment feature) - see `.github/workflows/build.yml`'s
"Build progress report objects"/"Install objdiff-cli"/"Generate progress
report"/"Upload progress report artifact" steps. `objdiff.json` (objdiff-cli's
own config, read by `report generate`) is generated fresh by `make report`
every time (see "One unit per matched file" below) and gitignored - never
hand-edit or commit it.

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
applied wherever they were known at that point. `expected/legacy.s` is the
same idea for the ROM's very first region (`main.c`/`memory.c`/`irq.c`'s
functions): a frozen copy of `asm/code.s` as it existed at the very first
commit (`8b090ca`), before *any* splitting or renaming happened at all -
that region was fully matched away by late 2025, well before `code_3.s`
even existed as a separate file, so it needed its own, earlier frozen
source (see `expected/README.md` for the exact address range and how it
was verified). Both are committed once and **must never be edited again**.
This is what "pristine baseline" from the earlier discussion of this
integration turned into in practice: rather than repeatedly re-extracting
bytes from `baserom.gba` (which would lose per-symbol relocations and
produce false mismatches for anything containing a function call), git
history already had the right frozen sources sitting in it, for both
regions.

## One unit per matched file, not one merged blob

`make report` (via `tools/report_units.py`) builds **one objdiff unit per
matched `src/*.c` file**, each tagged with a category (`graphics`/`util`/
`system`), plus one untagged unit per still-fully-raw stretch of ROM - not
a single unit covering the whole game. This is what makes decomp.dev's
per-system progress bars possible: each unit's `metadata.progress_categories`
tags it, and objdiff-cli's report aggregates matched/total *per category*
across whichever units carry that tag, then again as one overall total.

This has to be per-file, not per-category-merged, because of the exact
same problem `mem_collect` had (see below): objdiff infers a symbol's size
from the distance to the *next* symbol in the same object when there's no
explicit `.size`, and `arm-none-eabi-ld -r`-merging two functions that
aren't really adjacent in the ROM (which is what merging every `Util`
function into one blob would do - `math_util.c` and `time_util.c` aren't
next to each other) reintroduces exactly that bug at the merge seam. A
matched *file*, on the other hand, really is one contiguous ROM region
(the "one `.c` file per contiguous ROM region" rule in
[`docs/workflow.md`](./workflow.md) guarantees it), so slicing and pairing
one target/base object per file keeps every function's inferred size
correct, and category totals just fall out of aggregating those units.

For each entry in `tools/report_units.py`'s address table:

- **base**: that file's own compiled object under `build/crashbandicootxs/`
  (already built by the normal `NON_MATCHING=1` pass - nothing extra to do).
- **target**: `tools/slice_expected.py` pulls the matching address range's
  *text* out of `expected/code_3.s` or `expected/legacy.s` (whichever one
  covers it - see below) - the same technique `expected/legacy.s` itself
  was carved out with, so relocations and literal pools survive intact -
  then `tools/patch_expected_target.py` applies `expected/corrections.txt`
  to the assembled slice (see the next section). Four files
  (`printf_util.c`, `text_layout.c`, `input_util.c`, `oam_count.c`) contain
  a still-parked function; their range is wider than their own
  `NON_MATCHING=0` object shows, since the parked function's real ROM
  bytes currently live in the *neighboring* still-raw `asm/*.s` chunk
  instead - `expected/code_3.s` already has it labelled at its true
  address regardless (it was only ever parked, never extracted), so
  slicing still works unmodified once the address table accounts for it.
- Still-fully-raw stretches (nothing matched there yet) get a unit with
  only a target (the frozen slice) and no base - they count toward the
  *overall* total, correctly showing as unmatched. Most now also carry
  a `category` despite having no `base_path`/match percentage of their
  own, reflecting `docs/rom_map.md`'s reconnaissance pass - see
  "Categorizing the still-raw majority of the ROM" below for which ones
  and why a couple of regions are deliberately left uncategorized still.

**The address table is the fragile part.** It's hand-written in
`tools/report_units.py` from a **clean `make compare` (`NON_MATCHING=0`)
build's** `crashbandicootxs.map` - deliberately *not* the `NON_MATCHING=1`
build, because a parked function's imperfect reconstruction is a different
byte count than the real ROM, which shifts every address *after* it in a
`NON_MATCHING=1` map. `NON_MATCHING=0`'s addresses are the only ones
guaranteed correct, since `make compare`'s checksum verifies them against
the real ROM directly. Whenever a function moves between files, a new file
is added, or another parked function gets fixed, re-derive this table from
a fresh `make compare` map rather than hand-adjusting it.

Run `make NON_MATCHING=1 report` after a clean build (`rm -rf build`) to
produce `objdiff.json` and every unit's objects, matching
[`docs/workflow.md`](./workflow.md)'s convention for `NON_MATCHING` builds
generally.

## `expected/corrections.txt`: patching targets without editing the source

Since `expected/code_3.s`/`expected/legacy.s` are frozen, they can't be
corrected in place when later matching work finds something the original
disassembly got wrong - namely:

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

`tools/patch_expected_target.py` applies `expected/corrections.txt` to each
*assembled* target slice via `objcopy --redefine-sym`/`--add-symbol` - never
to either `.s` source. It's run once per unit now (see above), so it's
built to tolerate corrections that don't apply to a given slice (a rename
whose old name isn't present, or a split address outside the slice's own
range) by skipping them silently rather than erroring - the same
`corrections.txt` is passed to every slice unfiltered. See the comment at
the top of `expected/corrections.txt` for the exact line format.

**When to add one**: whenever a newly-matched function doesn't show up in a
locally-generated `report.json` at all, or reports an unexpectedly low match
percentage that direct byte comparison against `baserom.gba` (the project's
own established verification method - see `docs/workflow.md`) says shouldn't
be there.

## Categorizing the still-raw majority of the ROM

Beyond `graphics`/`util`/`system` (mirroring `src/`'s layout, tagging
actually-matched files), `tools/report_units.py`'s `UNITS` list also
tags several **still-fully-raw** stretches with a category - `game_loop`,
`actor`, `graphics_loading`, `audio` (the Shin'en GAX2 engine), `hud`,
and `overlay_ui` - even though none of that code has a `base_object` or
counts as matched yet. This is possible because [`docs/rom_map.md`](./rom_map.md)'s
whole-ROM reconnaissance pass (originally "reconnaissance, not ground
truth") has, over many rounds of direct function reads, reached high
confidence on where most of these regions actually start and end -
confirmed landmark addresses, individually-read functions, or a
dominant connected component - enough to be worth a decomp.dev progress
bucket even at 0% matched, the same way an unmatched `src/*.c` file
would show 0% under its own category rather than not appearing at all.

Two categories `rom_map.md` also identified - **`menu_ui`** and **`fx`** -
deliberately don't get their own address range: both are individual
functions scattered *inside* another category's contiguous span rather
than a separate block (`menu_ui`'s dispatch-table functions sit inside
`graphics_loading`'s `LoadGraphicsPackage` cluster; `fx`'s two-function
particle/trajectory-queue pair sits inside the `hud` gap after
`MainLoop`) - carving them out would need a boundary that doesn't
exist, so they're folded into their containing category instead. A few
other spots have a smaller-scale version of the same problem, folded
into the dominant category rather than left out or guessed at:

- `overlay_ui`'s span (`0x080015E0`-`0x08006600`) also contains a
  confirmed SIO/link-cable multiplayer subsystem, not separable by
  address from the pause-menu/dialog code around it.
- The `audio` span (`0x08037110`-`0x0803A944`, narrowed this session -
  see `docs/audio.md`) has a couple of confirmed generic compiler-
  runtime helpers (64-bit division routines) interleaved with genuine
  GAX2 code, the same false-positive pattern `docs/audio.md` warns
  about.
- The `hud` gap right after `MainLoop` (`0x08026EEC`-`0x0802866C`)
  overstates `rom_map.md`'s own ~4.8 KB hud figure by the ~1.1 KB that's
  actually `fx`/genuinely unlabeled, folded in for the same reason.

`tools/report_units.py`'s own comments note each of these inline, next
to the exact `UNITS` entry it applies to - check there before treating
any of `game_loop`/`actor`/`graphics_loading`/`audio`/`hud`/`overlay_ui`'s
decomp.dev percentage as more precise than "the dominant category in
this address range." Genuinely untouched regions (nothing in
`rom_map.md`, or too small to matter - like the ~200 B gap right after
`irq.c`) stay uncategorized, same as before this pass.

## A separate, known limitation: small residual percentages on real matches

Neither `expected/code_3.s` nor `expected/legacy.s` uses the
`thumb_func_end` macro (see `asm/macros/function.inc`), so none of their
~2060 combined symbols carry an explicit ELF `.size` - objdiff infers each
one's size from the distance to the next label instead. For most functions
this infers correctly, but a handful show 99-99.9% instead of 100% even
though a direct byte comparison confirms they're genuinely byte-exact:
objdiff's inferred size includes a trailing literal-pool constant or padding
halfword that belongs to neither function cleanly (there's no label marking
exactly where one function's own literal pool ends and the gap before the
next function's code begins). This is cosmetic - it doesn't affect whether a
function is truly matched, only the last fractional percentage point objdiff
reports for it - and isn't worth chasing down function-by-function;
`expected/corrections.txt` is for the two real problems above (missing name,
missing boundary), not this one. (`mem_collect`, in the section below, is
*not* an example of this - a 64-byte hidden function is not a rounding
error, which is exactly how that one was told apart from this category.)

## Resolved: `main.c`/`memory.c`/`irq.c`, and a genuine hidden function

These three were matched even earlier than `asm/code_3.s` existed as a named
file (back when the whole ROM was still one `asm/code.s`), through several
more splits than `code_3.s` went through, and were originally excluded from
the report entirely for lack of a pristine source. `expected/legacy.s` (see
above) closed that gap.

Closing it also surfaced two real, distinct problems, worth knowing about
since both patterns will likely recur:

- **`irq.c` turned out to be a mixed file**: some of its functions came from
  the old `code_1.s`/`code_2.s` lineage (2025), but seven others
  (`sub_80006A8` onward) were actually matched much later, from `code_3.s`
  (Sept 2026) - and were being silently dropped from the report because an
  earlier version of this integration excluded `irq.o` from
  `base_combined.o` *entirely* on the assumption the whole file predated
  `code_3.s`. There's no such thing as "this file is legacy" in general -
  only "this function's frozen source is legacy.s or code_3.s" - which is
  why the base side has no exclusions at all now; every `src/*.c` object
  goes in, and pairing is purely by symbol name against whichever frozen
  source actually has that name.
- **A genuinely unreachable function was hiding as a raw byte blob**:
  `mem_collect` used to report ~89% for no visible reason - direct
  disassembly of the 64 bytes right after it (still correctly compiled,
  since `make compare` never lies) showed real, coherent Thumb code with no
  caller anywhere in the matched source: two near-identical
  `mem_i/ewram_heap_pointer`-chasing loops, most likely an
  identical-code-folding artifact from agbcc's optimizer rather than
  anything reachable from a real call site. `src/system/memory.c` already
  had this embedded as a raw `.byte` blob (`// this is ugly AF`) from
  whoever matched `mem_collect` originally, precisely because leaving it
  out breaks the ROM's byte layout - it's now written as real, labelled
  Thumb instructions (`sub_800039C`) instead, with `expected/corrections.txt`
  giving the frozen target a matching `split` entry. Same bytes, same
  `make compare` result, just inspectable instead of opaque.

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
