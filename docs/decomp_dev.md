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

The ROM's data (everything after the code) is reported too, as a
separate data measure: see "Data progress" further down.

## One unit per matched file, not one merged blob

`make report` (via `tools/report_units.py`) builds **one objdiff unit per
matched `src/*.c` or `lib/*/src/*.c` file**, each tagged with a category
(`graphics`/`util`/`system`/..., and one per library: `gax`, `agb_eeprom`,
`libgcc` - see [libraries.md](./libraries.md)), plus one untagged unit per
still-fully-raw stretch of ROM - not
a single unit covering the whole game. This is what makes decomp.dev's
per-system progress bars possible: each unit's `metadata.progress_categories`
tags it, and objdiff-cli's report aggregates matched/total *per category*
across whichever units carry that tag, then again as one overall total.

This has to be per-file, not per-category-merged, because of the exact
same problem `mem_collect` had (see below): objdiff infers a symbol's size
from the distance to the *next* symbol in the same object when there's no
explicit `.size`, and `arm-none-eabi-ld -r`-merging two functions that
aren't really adjacent in the ROM (which is what merging every `Util`
function into one blob would do - `fixed_math.c` and `time_format.c` aren't
next to each other) reintroduces exactly that bug at the merge seam. A
matched *file*, on the other hand, really is one contiguous ROM region
(the "one `.c` file per contiguous ROM region" rule in
[`docs/workflow.md`](./workflow.md) guarantees it), so slicing and pairing
one target/base object per file keeps every function's inferred size
correct, and category totals just fall out of aggregating those units.

For each entry in `tools/report_units.py`'s address table:

- **base**: that file's own compiled object under `build/crashbandicootxs/`
  (already built by the normal `NON_MATCHING=1` pass), copied to
  `build/expected/units/` with its function sizes fixed up - see "Byte-exact
  functions must score exactly 100%" below.
- **target**: `tools/slice_expected.py` pulls the matching address range's
  *text* out of `expected/code_3.s` or `expected/legacy.s` (whichever one
  covers it - see below) - the same technique `expected/legacy.s` itself
  was carved out with, so relocations and literal pools survive intact -
  then `tools/patch_expected_target.py` applies `expected/corrections.txt`
  to the assembled slice (see the next section). Four files
  (`printf.c`, `wrapped_text.c`, `input.c`, `power_dialog_draw.cpp`) contain
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
`corrections.txt` is passed to every slice unfiltered. Besides `rename`
and `split` there are `unlabel`, `code`/`data` and `resolve` entries for
the less common cases described under "Byte-exact functions must score
exactly 100%" below. See the comment at the top of
`expected/corrections.txt` for the exact line format.

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

## Hand-written assembly is excluded from the totals

Some of the ROM was never C: crt0.s's `start`, libgcc's `lib1funcs.asm`
routines (`__udivsi3`, `__divsi3`, `__modsi3`, `__umodsi3`, `__div0` and
the `_call_via_rN`/`_call_via_lr` trampolines, `lib/libgcc/lib1funcs.s`),
the BIOS SWI wrappers (each is just `svc #N; bx lr`,
`lib/libagbsyscall/libagbsyscall.s`) and `IntrMain`, the IWRAM image's
interrupt dispatcher (`asm/intr_main.s`, first entry of `IWRAM_UNITS`,
see "The IWRAM image" below). There is nothing to decompile there, so
counting that code as unmatched (or as "matched", which is what happened
while a NAKED transcription sat inside a C unit) would misstate progress.
`tools/report_units.py` marks these ranges with `HANDWRITTEN` and emits no
unit for them, so they drop out of both the matched and the total counts.
crt0 is excluded the same way: its range comes before the first unit. The
bytes are still verified by `make compare`. Only mark a range
`HANDWRITTEN` once it's confirmed to be hand-written, not just hard to
match.

## The IWRAM image

crt0 copies `0x9E8` bytes from ROM `0x087E55E4` to IWRAM `0x03000000` at
boot. That image is built from source and linked the usual GBA way,
with a separate run address (VMA) and load address (LMA):
`ldscript.txt`'s `iwram` output section is placed at `0x03000000` with
`AT(LOADADDR(ROM) + SIZEOF(ROM))`, so its bytes follow the ROM data and
every symbol in it has its IWRAM address. `__iwram_lma` (its load
address) is crt0's copy source, the length is still
`gIntrTable - IntrMain_Buffer`. The `rom_fill` section after it
fills the rest of the 8 MB with `0xFF`. `sym_iwram.txt` only names the
uninitialised IWRAM from `0x030009E8` on (its `IWRAM (NOLOAD)` section
still starts at `0x03000000` and overlaps `iwram`; ld allows that for a
NOLOAD section). The image holds, in order:

- `asm/intr_main.s`: `IntrMain`, hand-written ARM - `HANDWRITTEN`.
- `src/iwram/string_arm.c` and `src/iwram/sprite_arm.c`: compiled ARM C,
  built with `tools/agbcc/bin/agbcc_arm_patched` (the `ARM_OBJS` and
  `PATCHED_ARM_OBJS` in the Makefile: `-O2 -fomit-frame-pointer
  -mthumb-interwork` plus one option each; agbcc_arm_patched is
  SAT-R/agbcc's agbcc_arm with `tools/agbcc_patches/`, built by
  `tools/build_patched_agbcc_arm.sh`). Their `.text` goes into `iwram`.
- `src/iwram/iwram_data.c`: the initialised globals from `0x030007CC`,
  its `.data`.

**Code units.** The report can't slice these from `expected/code_3.s`:
the image was never disassembled as code there. `expected/iwram.s` is
its frozen target, generated once from `baserom.gba` (ARM disassembly,
branch targets and pool words as labels and symbols, IWRAM addresses in
the `name: @ 0x030000D4` labels `slice_expected.py` looks for) and never
edited again, like the other two. `tools/report_units.py` has a second
address table, `IWRAM_UNITS`, in IWRAM addresses, whose targets
`build_target()` slices from that file. objdiff reads ARM or Thumb from
the objects' `$a`/`$t` mapping symbols, so ARM units need nothing else.
The two C files are one unit each (`util` and `graphics`), and their
functions count toward the code totals like any other.

**Data unit.** `iwram_data.c`'s `.data` is the last data unit
(`IWRAM_DATA` in the script, at ROM `IWRAM_LMA + 0x7CC`, target bytes
from the ROM like any `src/data` table). The image's code bytes are not
data any more, so `total_data` dropped by `0x7CC`.

## Byte-exact functions must score exactly 100%

objdiff counts a function toward `matched_code`/`matched_functions` only
at exactly 100%, so a byte-exact function that objdiff scores at 99.9% is
not a rounding error: it counts as entirely unmatched. Before this was
fixed, such near-misses cost the report about 7.7 points (92.3% matched
code with every compiled function in the ROM byte-exact). The causes, and
where each is handled:

- **Inferred sizes.** Neither frozen source uses `thumb_func_end`, so no
  target symbol had an ELF `.size`. objdiff then infers the size from the
  next symbol and trims trailing zero bytes as padding. That also trims
  the zero upper half of a final literal-pool word like
  `.4byte 0x000001FF`: the target loses two bytes, the word decodes as a
  `.hword`, and the function scores 99.9%. `patch_expected_target.py`
  now gives every target function an explicit size: up to the next
  function, minus a trailing zero halfword only when the assembler's
  mapping symbols show it is alignment padding and not half of a pool
  word. This was the largest class (about 70 functions).
- **Base sizes that stop short.** agbcc's `.size` ends before anything
  the assembler emits after the function, such as the literal pool of an
  inline-asm `ldr rN, =sym`, and a NAKED function's own labels have no
  `.size` at all. `report_units.py` points each unit's `base_path` at a
  copy of the compiled object (`build/expected/units/<unit>_base.o`)
  whose function sizes are widened (never shrunk) the same way. The
  build's own objects are never modified.
- **Disassembly artifacts.** The frozen disassembly sometimes wrote
  padding as `movs r0, r0` (which decodes as code, while agbcc's padding
  is data) and in one region wrote real instructions as `.4byte` data
  (`.4byte 0x1c03b500` for `push {lr}; adds r3, r0, #0`).
  `slice_expected.py` rewrites both while slicing, without changing any
  byte: the padding becomes `.align 2, 0`, and a numeric `.4byte` that
  no `ldr` loads (so it can't be a pool word) becomes two `.inst.n`
  halfwords.
- **Stale labels and boundaries.** Handled per function in
  `expected/corrections.txt`: `split` for functions the disassembly never
  labelled, `unlabel` for labels it took for function starts that the C
  doesn't have (the `GAX_CALL_ARM` return points `sub_8039E50`,
  `sub_803A318`, `sub_803A608`; `sub_802613E`, which starts
  mid-instruction; the padding stub `sub_8016046`), `code`/`data` for
  `mem_walk_heaps` and `EepromTimerIntr`, which the frozen sources only have as
  `.byte` blobs, and `resolve` for a base object that calls a function
  through a local `.set` alias and so has no relocation on those `bl`s
  (libgcc2.c's calls to `__udivsi3` were the one case, until the
  function got its libgcc name and the alias went away).

After these fixes, every function in a matched unit scores 100%, and
code progress is 100%: every compiled function in the ROM is matched.
The last two, ARM functions in the IWRAM image (`itoa_arm`,
`LookupSpriteFrameCache`), match with a locally patched agbcc_arm
(docs/matching/iwram-image.md, seventh pass). Any function below 100%
in a future report is either a regression or a new case of one of the
causes above.

**When a new function scores below 100%** even though `make compare`
passes, run `objdiff-cli diff -1 <target.o> -2 <base.o> <symbol>` (both
paths are in `objdiff.json`; unit names aren't unique) and read the
mismatching rows. A `.word`/`.hword` against instructions, or rows
missing at one end, point at sizes or mapping symbols. A function
missing from one side points at a missing `rename`/`split`/`unlabel`.
Note that `objdiff-cli diff` can report differences that `report
generate` ignores (a raw pool constant against a relocation to the same
address, for example). The report's own `fuzzy_match_percent` is what
decides "matched".

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
  (`WaitForVBlank` onward) were actually matched much later, from `code_3.s`
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
  Thumb instructions (`mem_walk_heaps`) instead, with `expected/corrections.txt`
  giving the frozen target a matching `split` entry. Same bytes, same
  `make compare` result, just inspectable instead of opaque.

Asset extraction also shows up in the report now, as its own data measure;
see "Data progress" below. The per-asset details are still tracked in
[`docs/graphics.md`](./graphics.md) and [`docs/audio.md`](./audio.md).

## Data progress (decomp.dev's separate data bar)

decomp.dev draws a second progress bar next to the code one from objdiff's
data measures (`total_data`/`matched_data`/`matched_data_percent`). Those
count bytes in data sections (`.rodata`) instead of code bytes, so they
never change any code measure.

**What counts as matched data.** This is the usual definition for GBA
decomps. A byte of ROM data is *matched* when the build produces it from a
source in the repo: a PNG/`.pal`/`.bin` under `graphics/` compressed by
gbagfx, the GAX2 audio rebuilt from `sound/` by `tools/gax_audio.py`, the
sfx table built from `sound/sfx_table.json`, a C table in `src/data/`
(see [data.md](./data.md)), and so on. It is *unmatched*
while `data/data.s` still copies it with `.incbin "baserom.gba", ...`.
Both kinds come out byte-identical in the ROM, and `make compare` checks
that, so matching bytes can't be the test. The source is what decides.

**Scope.** Everything after the code, from `0x0803B8B0` to the end of the
ROM (`0x08800000`), is linked from the `/* Data */` block of
`ldscript.txt`: the sections of `data/data.s` (one label per blob and one
`.incbin` per label) interleaved with the `.rodata` of the `src/data/*.c`
tables, in ROM order, then the IWRAM image (`0x087E55E4`-`0x087E5FCC`,
see "The IWRAM image": only its initialised data, `0x087E5DB0` on, is
data; the rest is code). The last 106,548 bytes, from `0x087E5FCC` on
(the linker's `rom_fill` section), are all `0xFF`: that's empty cartridge
space, not data. The report ends the data range at
`DATA_END = 0x087E5FCC`, which leaves 8,036,176 bytes of data. The
constant is hard-coded so the no-ROM path gets the same totals. When
`baserom.gba` is present, the script checks that the trailing `0xFF` run
really starts there. Smaller all-`0x00` runs inside still-baserom blobs
(151 runs of 256+ bytes, 64,716 bytes in total, the largest 2,664 bytes,
nearly all inside the big `gLanguageSelectPalette3`/`gSpriteBankTable`
blobs) still count as data: they sit inside real data, and there are no
other `0xFF` runs. Apart from `src/data/`, no C file puts data in the
ROM: agbcc emits no `.rodata`/`.data` for the code files, and
`ldscript.txt` only places their `.text`, discarding everything else.
Jump tables and literal pools inside code are counted as code.

**How `tools/report_units.py` builds the units** (`data_units()`):

1. It walks the `/* Data */` block of `ldscript.txt` (`data_layout()`)
   and turns each piece into blobs (label, address, size, source): a
   `data/data.s` section into one blob per label, a `src/data/*.o` object
   into one blob per global object symbol in its `.rodata` (a blob runs to
   the next symbol, read with `objdump` from the object `make report` has
   just built). Addresses are computed by adding blob sizes from
   `0x0803B8B0`, the code table's end sentinel, so no map file is needed.
   A `data.s` blob's size is the incbin's length argument, or the built
   file's size when there is none. Each baserom blob's own incbin offset
   has to agree with the computed address, or the script stops, which
   also catches a C table of the wrong size. So does a `data.s` section
   the linker script doesn't place, or a C object that would start
   misaligned.
2. It groups adjacent blobs into units: each run of blobs built from the
   same asset directory (`data_graphics_intro_XXXXXXXX`,
   `data_sound_XXXXXXXX`, ...), each `src/data` file
   (`data_src_data_<file>_XXXXXXXX`) and each run of baserom blobs
   (`data_raw_XXXXXXXX`). That currently comes to 240 units.
3. For every unit it assembles a **target** object with each blob as a
   global, `.type %object`, `.size`d symbol in `.rodata`, its bytes taken
   from `baserom.gba`. A built unit also gets a **base** object with the
   same symbols, whose bytes come from the same `.incbin` of the built file
   that `data/data.s` uses. A `src/data` table's bytes only exist after
   linking (its pointers are relocations) and `make report` doesn't link,
   so its base takes the ROM's bytes too: `make compare` is what proves
   the C produces them. A baserom unit gets **no base**, like the
   still-raw code ranges, so it counts toward `total_data` and never toward
   `matched_data`.
4. Every data unit is tagged with the `data` progress category. Built units
   are also marked `complete`, since there's nothing left to clean up in a
   blob that comes from an editable source.

**Why units never mix built and baserom blobs.** objdiff's report measures
data per section, all or nothing: a section counts toward `matched_data`
only if it matches 100%. It also merges `.rodata.*` sections back into one
`.rodata` per object. One baserom blob inside a built unit would therefore
make that whole unit count as unmatched. With units kept pure, each one is
simply 0% or 100%, and the total is exactly the byte count of the built
blobs. A built unit whose bytes really differ from the ROM (a broken asset
in a PR) drops to 0%. objdiff compares the actual bytes, so that shows up.

**Moving a blob from baserom to built** needs nothing in the report
script. Replace its `.incbin "baserom.gba", ...` in `data/data.s` with the
built file, keeping the label, and the next `make report` counts it. The
same goes for a table moved into `src/data/` (the steps are in
[data.md](./data.md)): once `ldscript.txt` places the object, the report
picks it up.

**Build requirements.** `make report` depends on the built graphics and
sound files, though not on `data.o`, which would need `baserom.gba`. Target
objects need `baserom.gba`. Without it (fork-PR CI), baserom units are
zero-filled to the right size and built units use their own bytes as the
target (a `src/data` unit is zero-filled on both sides). That keeps the
totals correct but doesn't verify the assets, which
only matters for fork PRs: `main`'s run has the ROM, and `make compare`
checks every byte anyway.

As of the first `src/data` conversion: 1,650,143 of 8,038,172 bytes
matched (20.53%). That is the two LZ77 blobs in `graphics/unknown/`, the
intro and tileset1 graphics, the GAX2 audio data, the sfx table, and
9,716 bytes of pointer tables in `src/data/` (up from 1,640,427, 20.41%).
[`docs/data_map.md`](./data_map.md) triages the remaining raw blobs and
suggests an order for converting them.

## "Matched" vs "complete": tracking the cleanup pass separately

Byte-exact and *clean* aren't the same thing here: matching a function is
often done first as raw `asm(...)`/`NAKED` or with raw pointer-arithmetic
offset casts and literal hardware addresses, with the
[`docs/workflow.md`](./workflow.md) step 7 cleanup pass (named struct
fields, `REG_*`/`OAM`/`PLTT`/`DMA_*` macros) coming later, sometimes much
later, as its own follow-up. Tracking only "matched" would make every
`Cleanup: raw pointer arithmetic in ...` GitHub issue (see
`tools/chunk_remaining_work.py --cleanup-scan`, `CONTRIBUTING.md`'s
cleanup-task conventions) invisible to decomp.dev - a file could be 100%
byte-matched and still be full of the exact opaque casts this project is
trying to get rid of.

objdiff's `Unit.metadata.complete` field (see
[objdiff's config schema](https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json))
exists for exactly this: a per-unit status independent of match
percentage. `tools/report_units.py` sets it for every matched unit by
re-running `chunk_remaining_work.py`'s `scan_cleanup_candidates()` (the
same scan that generates the per-file cleanup issues) and marking a unit
`complete: true` only if its source file has zero raw offset casts and
zero raw hardware addresses outside `#if NON_MATCHING` blocks; anything
else gets `complete: false`. (The advisory `offset_addr` kind, a byte
pointer plus a constant left as an address, is listed by
`--cleanup-report` but doesn't count: it is as often a payload past a
header as a field.) Still-raw units (no `base_path`) never get
a `complete` key at all - it isn't applicable until something's matched.

`objdiff-cli report generate` aggregates this automatically into the
report's top-level `measures.complete_units`/`measures.complete_code`/
`measures.complete_code_percent`, alongside the existing
`matched_units`/`matched_code_percent` - decomp.dev reads both without
any further CI changes. Re-running `make NON_MATCHING=1 report` after any
matching *or* cleanup work keeps this in sync automatically; there's
nothing to hand-maintain here the way the `UNITS` address table is.

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
