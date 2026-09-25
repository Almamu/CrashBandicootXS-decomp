# `graphics_loading` chunk `0x0801FA3C`-`0x08021668` (issue #31), second pass

Continues the first pass (PR #202, recorded in `docs/matching.md` under
"`graphics_loading` chunk `0x0801FA3C`-`0x08021668` (issue #31)"), which
matched/parked the 4-function "trigger effect type N" twin family
(`sub_8020E84`/`sub_8020F7C`/`sub_802107C`/`sub_802117C`,
`src/graphics/trigger_effect.c`) and left the other 21 functions raw.
This pass picks one of those 21 back up: `sub_801FDEC`.

## Semantics

`sub_801FDEC` is one instance of a large, near-identical family of
"two-line text popup" spawners that occupies most of the still-raw part
of this chunk (`docs/rom_map.md`'s "A family of 'trigger effect type N'
functions" section already names this shape via `sub_8020D4C` and
`sub_802062C`/`sub_8020788`, but had not carried any instance through to
C before this pass). Every sibling shares the same skeleton, varying
only in a handful of embedded constants:

1. Allocates a part-object via `sub_8009ED0(arg0, arg1, arg2)` - only 3
   of the caller's 4 `u16` arguments are actually consumed by the
   callee, but the ROM still marshals `arg3` into `r3` for the call
   (an artifact of the caller not narrowing its own argument list to
   match the callee, the same "pass everything, callee ignores the
   rest" convention seen elsewhere in this codebase).
2. Points the new part's `+0x20` field at a fixed offset (`0x54` for
   this instance) into the record reached through
   `gUnknown_030012D0`'s double pointer-to-pointer (one level deeper
   than the twin family's single dereference).
3. Sets the part's `+0x29` bitfield low nibble from `sub_800815C`'s
   result, via the same "compute address, then mask, then reload-AND-OR"
   idiom used throughout this ROM region.
4. Calls `sub_800CA74()` for a header/context pointer, then fires a
   `sub_803AD80` animation-table trampoline (`header + table->offset`,
   `part`, `table->fn`, where `table = header->0xc`) *twice*, tagging
   `header->0x6c = 7` and wiring `part->0x44 = header` in between - the
   table pointer is reloaded fresh from `header->0xc` for the second
   call rather than reused, matching this compiler's usual "no CSE
   across a store" behavior documented elsewhere in this file.
5. Marks itself active (`part->0xa = 1`), clears its own top flag bit
   (`part->0xc &= 0x7f`).
6. Looks up two "collected" bits via a `gUnknown_030012B4`-rooted
   `{u16 offsets[], u8 bytes[]}` pair, indexed by `arg3`, and packs them
   into `part->0x28`'s bits 4/5.
7. Registers itself into `gUnknown_030012F0`'s manager
   (`sub_8008E94`).
8. Overwrites `header->0x84` with a table pointer
   (`gStaticData_0816BA6C` for this instance) and calls
   `sub_800C6A8(header, 7)`.

Every one of the ~20 other still-raw functions in this chunk is a
variant of this same shape with different embedded offsets/constants
(and, for several, a different tail after step 8 - some do a
second `header->0x84` rewrite plus a struct-field copy, some do the
standard `sub_80087C0`/`sub_80087B4`/`sub_800872C` OAM trio instead of
steps 6-8, at least one variant reads a bit-27 test on `part->0x28`
first). Left untouched this pass - see "Left raw" below.

## Matched: `sub_801FDEC`

`src/graphics/graphics_loading_1fdec.c` - full clean `make compare`
verified (`La suma coincide`). `arg0` has to stay `u32` (not `u16` like
its siblings) for the same reason documented for the twin family in
`docs/matching.md`: the ROM only truncates it at its single call site
inside `sub_8009ED0`'s argument marshalling, not up front in the
prologue.

Three spots needed `asm volatile` rather than plain C, all confirmed by
direct isolated-compile-vs-ROM instruction diff (and, since an isolated
compile is only a diagnostic tool, re-confirmed by a raw-byte diff of
the full clean `make compare` output against `baserom.gba` at this
function's address before it was accepted as matched - see
`docs/workflow.md`'s warning about exactly this mistake):

- **The `+0x29` bitfield update's negative-mask constant.** Plain C
  (`s32 acc = -0x10; acc &= *addr; ...`) lets this compiler synthesize
  `-0x10` from the still-live `r1 = 0xf` mask constant computed two
  lines earlier (`sub r1, r1, #0x1f`, a genuine one-instruction
  optimization since `0xf - 0x1f == -0x10`), but the ROM reloads a
  fresh `mov r1, #0x10` / `neg r1, r1` pair instead - a different,
  longer instruction sequence. Written as a 2-instruction `asm volatile`
  block to force the ROM's literal (non-optimized) form.
- **The `header + 0x6c = 7; part->0x44 = header;` write pair.** `r8`
  (where `header` lives for the whole function - see below) can't be
  the base register for an immediate-offset `strb`/`str` in Thumb, so
  the ROM copies it into a lo register once (`mov r3, r8`) and then uses
  two offset-form stores. A bare `(u8 *)header + 0x6c` cast instead
  computes each full address via a separate `add`, then stores with no
  offset - same result, different (and differently-sized) instructions.
  Fixed with paired `register ... asm("r0")`/`asm("r3")` locals for the
  tag value and the lo-register header copy, in that declaration order
  (order matters here too - swapping it changes which register gets
  which value).
- **The two-bit "collected" pack into `part->0x28`.** The ROM caches `1`
  into `r6` once (a second, independent `mov r6, #1` right next to the
  unrelated `part->0xa = 1` store, not reused from it) and then re-ANDs
  with that cached `1` twice per bit even when nothing in between could
  have clobbered the result - for the first bit this second AND
  straddles an unrelated `+0x28` address computation, but for the
  second bit both ANDs are directly adjacent, which this compiler's own
  peephole folds into a single instruction when phrased as plain C
  (`x &= 1; x &= 1;` collapses to one `and`). Two `asm volatile` blocks
  (split so the `gUnknown_030012B4` address load - itself letting the
  compiler manage its own literal-pool placement, keeping it in the
  function's single combined pool alongside the other 3 symbols instead
  of an inline `ldr r0, =symbol` splitting off its own mid-function
  pool - lands between them) spell out the exact ROM instructions
  instead of fighting the optimizer.

Also confirmed (not inline-asm, just a `register ... asm("r8")` pin):
the header pointer has to live in `r8` for the whole function. A plain
local lets gcc pick a lo register instead, which drops the
`mov r6, r8`/`push {r6}` prologue pair (and its epilogue counterpart)
and shortens the function by 4 bytes - an easy mistake to miss from an
isolated compile's text output alone, since the instruction-shape
diff still "looks right" until the actual byte count is checked.

**Assembler note:** this compiler's raw hex-asm output does not use
GAS's unified-syntax flag-setting mnemonics (`movs`/`ands`/`adds`/etc.)
- copying ROM disassembly text (which does use them, from `objdump`)
directly into an `asm volatile` block fails to assemble
("instruction not supported in Thumb16 mode"). The non-suffixed forms
(`mov`/`and`/`add`/...) match what this compiler's own output already
uses elsewhere in the same function.

File split: `sub_801FDEC` sat alone between still-raw neighbors on both
sides in `asm/code_3_2_17_1e990.s`, so it became three pieces in ROM
order - the trimmed `asm/code_3_2_17_1e990.s` (unchanged content, just
shorter), the new `src/graphics/graphics_loading_1fdec.c`, and the new
`asm/code_3_2_17_1feec.s` (everything from `sub_801FEEC` onward that
used to be in the same file) - see `ldscript.txt` and
`tools/report_units.py`'s `graphics_loading` category, both updated to
match.

## Left raw (~20, not attempted this pass)

The rest of the chunk - `sub_801FA3C`, `sub_801FB74`, `sub_801FCB4`,
`sub_801FEEC`, `sub_8020010`, `sub_8020138`, `sub_802026C`,
`sub_80203A8`, `sub_80204EC`, `sub_802062C`, `sub_8020788`,
`sub_80208C4`, `sub_80209EC`, `sub_8020B0C`, `sub_8020C18`,
`sub_8020D4C`, `sub_8021280`, `sub_8021388`, `sub_8021480`,
`sub_802155C` - are all confirmed instances of the same "two-line text
popup" family `sub_801FDEC` belongs to (semantics fully read for all of
them this pass), several with their own tail variant (a second
`header->0x84` rewrite plus a `{x, y, w}`-shaped struct-field copy into
`part->0x30`/`0x34`/`0x38`; the standard OAM-trio construction instead
of the "collected"-bits pack; a bit-27 test on `part->0x28` gating a
different tag value) not yet worked through the same register-level
verification `sub_801FDEC` needed. Given how much iteration a single
instance needed to close (three separate inline-asm workarounds for
three distinct compiler-scheduling/optimization quirks), and that at
least one of the tail variants introduces genuinely new register
pressure (`sub_802062C`/`sub_8020788` already spill to `sl`/`sb`/stack
in the raw disassembly, unlike `sub_801FDEC`'s plain `r8` pin), these
were left untouched rather than force a low-confidence match. The
`asm volatile` technique established here (drop the unified-syntax
suffix, match the ROM's exact idiom rather than the "obviously
equivalent" C phrasing) should transfer directly to whoever picks these
up next - the semantics above (numbered list) apply to all of them,
just with different embedded offsets/constants/tags.

Verified via a full clean `make compare` (`La suma coincide`) and
`make NON_MATCHING=1 report`.

## Third pass: the "trigger effect type N" twin family matched via NAKED transcription

`sub_8020E84`/`sub_8020F7C`/`sub_802107C`/`sub_802117C`
(`src/graphics/trigger_effect.c`), parked since the first pass
referenced above, are now all byte-exact matched, confirmed by a full
clean `make compare` ("La suma coincide"). Semantics were already fully
understood and confirmed instruction-for-instruction against the ROM;
the residual register-allocation gaps that first pass documented
(rotated parameter-home registers, an `sb`/`r9` reload-after-call
sequencing gcc 2.9 never reproduced) never responded to further plain-C
restructuring, so all four were converted to `NAKED` and their ROM
disassembly transcribed instruction-for-instruction - the same escape
hatch this project already established for `sub_8001CB8`/`sub_8001DB4`
(`src/system/link_cable.c`, see
`docs/matching/issue-4-sio-settings-sync.md`'s "The general strategy
for the rest" section). All four share the exact same shape (confirmed
by the transcription itself matching one-for-one once the twin family
was first identified) - only the bit-test mask, sound ids and tag value
differ between them, plus `sub_802117C` needing a third extra
callee-saved register (`sl`/r10) since its tag constant (`8`) doesn't
fit the same immediate-AND idiom the other three use.

**`ldscript.txt` gotcha:** `trigger_effect.c` compiled to an empty
object file while these four functions were `#if NON_MATCHING`-guarded
(no other code in that file), so it was never listed in `ldscript.txt`
at all - nothing needed it there. Once the functions became real,
always-compiled `NAKED` C, the file needed an actual `ldscript.txt`
entry at the exact point in `asm/code_3_2_17_1feec.s` where the raw
bytes used to sit. Since that raw block sat in the *middle* of a much
larger still-raw file (`sub_801FEEC`-`sub_8021BD8`, most of it still
raw per "Left raw" above), removing it left a single object with a gap
that needed filling, not just a line to delete - so
`code_3_2_17_1feec.s` was split into two files at that point (the
existing name keeps everything before `sub_8020E84`; the new
`code_3_2_17_21280.s` picks up at `sub_8021280` and keeps everything
after `sub_802117C`, unchanged), with `trigger_effect.o` inserted
between them in `ldscript.txt`. A first attempt that only deleted the
guarded block in place (without this split) still built and linked
without error, but silently shifted every ROM address from
`sub_8021280` onward by the guarded block's byte count - caught by the
post-build `arm-none-eabi-nm`/map-file address check against each
function's own `sub_XXXXXXXX` name before ever diffing bytes, per
`docs/workflow.md`'s warning about exactly this mistake.

## Fourth pass: the whole tail of `asm/code_3_2_17_21280.s` (`sub_8021668`-`sub_8021BD8`)

Picked back up the remaining ~20 raw "two-line text popup" siblings the
second pass identified but didn't attempt. Rather than working through
them from the top (`sub_8021280` onward, all full popup-family
instances needing the heavy `asm volatile` treatment `sub_801FDEC`
needed), this pass started from the *other* end of
`asm/code_3_2_17_21280.s` (`sub_8021668` onward), which turned out to
be a much easier mix: one more popup-family instance with a different
tail shape, a small `gStaticData_084A5600`-record spawner family
(registering into a manager global `sub_8021D80`'s family in
`graphics_loading_21d80.c` doesn't use), a run of plain
`sub_801A878`/`sub_801B984` trampolines, one `sub_800CB40`-based
constructor, and - closing out the file - 12 more plain `sub_800FF0C`
entity-constructor trampolines (types `0x12` down to `7`) continuing
the family `graphics_loading_21bfc.c` already covers for types `1`-`7`
at a different address. Every one of these 24 functions from
`sub_8021668` through `sub_8021BD8` (the literal last function in the
old `asm/code_3_2_17_21280.s`) is now real, matched C, plus one more
(`sub_802190C`) as a NAKED transcription - see below. This retires
`asm/code_3_2_17_21280.s` down to just its first 470 lines
(`sub_8021280`-`sub_802155C`, the 4 remaining full popup-family
instances - see "Left raw" below).

New file: `src/graphics/graphics_loading_21668.c`, inserted in
`ldscript.txt` right after `asm/code_3_2_17_21280.o` (which now ends
at `sub_802155C`'s literal pool) and before
`src/graphics/graphics_loading_21bfc.o`.

### `sub_8021668` - the popup family's OAM-trio tail variant

Same `sub_8009ED0` constructor and `+0x20` table-pointer setup as
`sub_801FDEC`, but a different tail: builds the part via the standard
`sub_80087C0`/`sub_80087B4`/`sub_800872C` OAM trio (like
`graphics_loading_21d80.c`'s family, not the twin family's lookup-table
pack), looks up a frame-nibble value through a *double* dereference of
its own just-stored `+0x20` table pointer (`*(*(part->0x20)) + 0x14`,
not `sub_800815C`) plus `gUnknown_030012B8`'s tile-asset cache via
`sub_8006DF8`, unconditionally clears bits 4/5 of `part->0x28` (no OR -
simpler than the twin family's lookup-table pack), fires a single
`sub_803AD80` trampoline (not twice), and finishes with a three-step
flags mask (`(((flags & 0x7f) & -5) & -0x41) | 0x10`). Two real bugs
surfaced and got fixed during this pass, both only visible after a full
clean `make compare` (an isolated compile alone hid both - see
`docs/workflow.md`'s warning about exactly this):

- **The record-id lookup was under-dereferenced at first.** `part->0x20`
  holds a pointer *to* the record (set moments earlier as
  `tableBase + 0x168`), and the ROM reads `*(part->0x20)` first (a
  second pointer) before indexing `+0x14` off *that* - an easy miss
  since the twin family's own `+0x20` field is used as a flat pointer
  everywhere else in this cluster. Missing the middle dereference still
  produced byte-plausible-looking (but wrong) code in isolation.
- **A `part->field_0x2d = 0;` write reordered relative to the ROM.**
  Plain C (`*((u8 *)part + 0x2d) = 0;`) let the compiler compute the
  destination address before materializing the `0` constant
  (`add r1,r6,#0; add r1,r1,#0x2d; mov r0,#0; strb r0,[r1]`), while the
  ROM computes the constant first (`mov r0,#0; add r1,r6,#0; ...`) -
  the same class of evaluation-order gap the twin family's `asm
  volatile` blocks work around elsewhere in this cluster, just for a
  plain store instead of a masked one this time. This one slipped past
  a by-hand ROM-listing comparison during development (the two
  instructions look interchangeable) and was only caught by the full
  clean `make compare`'s checksum failing by exactly 6 bytes at this
  address - fixed with a pair of `register ... asm("r0")`/`asm("r1")`
  locals (constant declared first, address second) forcing the same
  evaluation order as every other two-step store in this file.

Every mask/negative-constant step (record-id nibble pack, `0x28`
bit-4/5 clear, final 3-step flags mask) needed the same
`asm volatile`-anchored idiom the twin family established - plain C
folds two sequential AND-immediates into one, or reorders a call-result
reload, in ways the ROM's own codegen never does.

### The `gStaticData_084A5600`-record family: `sub_8021748`/`sub_80217D0`/`sub_802183C`

Same overall shape as `graphics_loading_21d80.c`'s `sub_8021D80` family
(`sub_8008434` constructor, `+0x20` table offset, `sub_800815C`
frame-nibble update), but two differences: they register into
`gUnknown_030012F8`'s manager instead of `EC`, and (except
`sub_80217D0`, which skips the OAM trio and the `+0x2d`/`+0xa` writes
entirely) they add a `part->flags = (flags & 0x7f) & -5;` step this
family didn't need before. That mask needed the same
two-`asm-volatile`-step treatment as everywhere else in this cluster -
plain C folds `(x & 0x7f) & -5` into a single `and`/`0x7b` immediate,
which the ROM's own codegen never does. `sub_8021748`/`sub_802183C`
also cache their shared `0` tag/`field_0A` value in `r5` across the
whole function (a genuine `register u8 zero asm("r5")`), matching the
ROM's own register reuse - assigning it only *after* the table-offset
store (not at declaration) is what keeps the truncation-prologue
instruction order matching the ROM's, the same declaration-vs-statement
timing gotcha the fourth pass's `sub_8021668` bug above is another
instance of.

### Plain trampolines: `sub_80218C4`/`sub_80218E8`/`sub_8021974`/`sub_8021998`/`sub_80219BC`/`sub_80219E0`/`sub_8021A4C`-`sub_8021BD8`

18 functions, no iteration needed beyond the established call-signature
patterns: 5 plain `sub_801A878(arg0, arg1, arg2, arg3, id)` calls (ids
`8`/`6`/`2`/`1`/`0` - same callee the twin family in `trigger_effect.c`
uses), one plain `sub_801B984(arg0, arg1, arg2, arg3)` tail call, and
12 plain `sub_800FF0C(arg0, arg1, arg2, arg3, type)` calls (types `0x12`
down to `7`) continuing the entity-constructor trampoline family
`graphics_loading_21bfc.c` already covers for types `1`-`7`. Every one
of these matched from the very first isolated compile - the 5-argument
call shape (4 register args plus a stack-passed 5th) reliably puts the
constant on the stack before the register args regardless of source
order, so there was nothing to fight here. `nullsub_21` (an empty
`bx lr` stub sitting between `sub_80219E0` and `sub_8021A00`) is also
in this file for the same reason - it has to be, to keep the file's ROM
range contiguous.

### `sub_8021A00` - a `sub_800CB40`-based constructor

The one function in this run using a *different* constructor
(`sub_800CB40`, no arguments) instead of `sub_8009ED0`/`sub_8008434`.
Calls `sub_8026EDC(0x28)` purely for a side effect first (return value
discarded, matching the "call purely for a side effect" idiom
`docs/naming.md` documents), then builds the real object, wiring a
fixed `sub_801F680` callback into `+0x1c`, `+0x20 = 0x78`, `+0x24 = 0`,
a Q8.8 `{x, y}` position, and a `flags |= 0x10`. Two small ordering
fixes were needed over the first plain-C draft:

- The `+0x24 = 0` write's `0` is cached in `r2` right after the object
  pointer is obtained (before the `+0x1c`/`+0x20` stores), then reused
  at the third store, not recomputed - a `register s32 zero asm("r2")`
  assigned at that point (not folded into a single-expression store)
  reproduces it.
- `obj->flags |= 0x10;` needed the mask (`0x10`) materialized in `r0`
  *before* the `ldrb` load of the current flags byte into `r2`, the
  same "constant first, then read" ordering
  `CLEAR_FLAGS_7F_AND_NEG5` uses elsewhere in this file - a plain
  `obj->flags |= 0x10;` statement evaluated the load first instead.

### `sub_802190C` - parked as NAKED

A gated `sub_801A878`/`sub_80234F4` dispatcher: picks id `7` if
`sub_80232A0(gUnknown_030012C0)` is true or `gUnknown_030012C0+0x8c` is
nonzero, else id `5` - the same OR-gated shape the twin family in
`trigger_effect.c` uses for its own sound-id choice, just feeding
`sub_80234F4` (`self->0x1b8` setter, `src/system/game_loop10.c`)
instead of `sub_80234E8`. Semantics are fully understood and every
instruction's operation matches the ROM, but the `arg0`-`arg3`
parameter-home registers (`r5`-`r8`, a mix of immediate and deferred
truncation) and the `id` register's exact scheduling relative to the
stack-argument store never converged through plain C or register pins
- the same class of gcc-2.9 register-allocation gap the twin family
hit (see the third pass above). One register-pinning attempt (pinning
`arg2`'s temporary to `r7` explicitly) produced outright *wrong* code
(a bogus `sp`-relative address computed into the register instead of
the intended value) rather than just a mismatched-but-correct
instruction sequence - a reminder that this compiler doesn't always fail
safe when a pin conflicts with its own internal register use (`r7` as
an implicit frame-adjacent register in this Thumb ABI). Transcribed
instruction-for-instruction from the ROM disassembly instead, the same
escape hatch used throughout this project. Tracked as parked in
`tools/report_units.py` with its own `base_object = None` entry,
interleaved between two matched ranges of the same
`graphics_loading_21668.o` (`tools/report_units.py`'s existing
`actor_anim.o` entries already establish that the same `base_object`
path can appear in more than one `UNITS` row for non-contiguous address
ranges within one real object file).

### Left raw (4, not fully attempted this pass)

The remaining raw stretch, `asm/code_3_2_17_21280.s` (470 lines, ending
right at `sub_8021668`'s start), is **not** all the same "two-line text
popup" family - checked this pass, correcting an assumption the second
pass's writeup carried forward:

- **`sub_8021280`** is a *different* function entirely - not part of
  the popup family. It dispatches on `sub_8023290`/`sub_80232B8`/
  `sub_8023324`/`sub_802332C` (a `gStaticData_0816C86C`-indexed guard
  check) into one of three arms: two calls to `sub_80071E4` +
  `sub_80070EC` (a differently-sized spawn, tag `0x12`, registering into
  `gUnknown_030012E8`), or a `sub_801A878` position-probe feeding
  `sub_8023500` with an offset `{x, y}` pair. Not attempted this pass -
  semantics read far enough to know it's not a popup-family sibling, but
  not worked through to a full C reconstruction.
- **`sub_8021388`**, **`sub_8021480`**, **`sub_802155C`** genuinely
  *are* 3 more popup-family instances (same `sub_8009ED0` constructor,
  `+0x20` table offset, `sub_800815C`/`UPDATE_PART_FRAME_NIBBLE` nibble
  update, `gUnknown_030012B4` two-bit collected pack, `sub_803AD80`
  trampoline via an allocated header, tag/manager-register tail -
  `sub_8021480`/`sub_802155C` skip the flags-mask step `sub_8021388`
  has and use a plain `flags |= 0x10` instead, and `sub_802155C` adds
  the OAM trio like `sub_8021668`). All three additionally call a
  header-construction helper (`sub_801A838(block, arg1, arg2)` for
  `sub_8021388`, `sub_80189EC()` for `sub_8021480`, `sub_80197DC()` for
  `sub_802155C`) and a closing `sub_8023318(gUnknown_030012C0, hdr)`
  neither `sub_801FDEC` nor `sub_8021668` have. `sub_8021388` got the
  furthest this pass: every instruction's *operation* matches the ROM
  (confirmed via isolated compile, using the same collected-bits-pack
  `asm volatile` block as `sub_801FDEC`/`graphics_loading_21668.c`), but
  the prologue's `arg1`/`arg2` truncation-into-`r8`/`sb` sequence has an
  extra ROM instruction pair (`mov r8, r1` / `mov sb, r2` computed from
  the *raw, untruncated* incoming values, immediately followed by a
  second `mov r8, r1` / `mov sb, r2` pair from the *truncated* values -
  i.e. the ROM spills the parameter twice) that no plain-C phrasing or
  register-pin tried this pass reproduced. Given `sub_8021480`/
  `sub_802155C` likely share a close variant of the same gap
  (unconfirmed - not attempted), this looks like the same class of
  gcc-2.9 parameter-lowering quirk documented elsewhere in this cluster,
  not a semantics problem - a good NAKED-transcription candidate for
  whoever picks these three up next, or worth one more plain-C attempt
  with a different technique (e.g. an explicit `asm volatile` spelling
  out the double-store prologue directly, the same escape hatch used
  for the collected-bits pack).

Verified via a full clean `make compare` (`La suma coincide`) and
`make NON_MATCHING=1 report`.

## Fifth pass: the final raw region (`sub_8021280`-`sub_802155C`) - 2 of 4 real C, 2 NAKED

Picked up the "Left raw (4)" list the fourth pass left behind - the last
still-raw stretch of `asm/code_3_2_17_21280.s`. All four are now real,
always-compiled code (2 matched, 2 NAKED), retiring the raw file entirely.
New file: `src/graphics/graphics_loading_21280.c`, replacing
`asm/code_3_2_17_21280.o` in `ldscript.txt` at the same point.

### Matched: `sub_8021388`, `sub_802155C`

Both are "two-line text popup" siblings, matched byte-exact (full clean
`make compare`, `La suma coincide`). Confirms the fourth pass's guess that
`sub_8021388`'s prologue double-store (`mov r8, r1` / `mov sb, r2` from
the raw incoming values, immediately followed by a second pair from the
truncated ones) really is a pure gcc-2.9 codegen quirk, not a semantics
gap - but it turned out reachable from plain C after all, via a technique
the fourth pass hadn't tried: writing the *entire* prologue-through-call
(argument truncation, the double `r8`/`sb` store, and the `bl sub_8009ED0`
itself) as one hand-spelled `asm volatile` block with the raw incoming
registers (`r0`-`r3`) pinned as inputs and `part`/`a1`/`a2`/the truncated
`arg3` pinned as outputs - rather than trying to coax the compiler's own
scheduler into the ROM's exact instruction order through plain-C
statement ordering (which this pass confirmed, again, gets silently
reordered/CSE'd away; see "Two more compiler-codegen gotchas" below for
the two extra spots this same class of gap turned up in `sub_8021388`
itself, past the point the fourth pass had already diagnosed).

`sub_802155C` (the OAM-trio tail variant, same shape as `sub_8021668`)
needed the same "hand-spelled asm block covering the whole
prologue-through-call" treatment for its own single-truncation prologue
(`arg3`'s home is `r5` for the whole function, `part` is `r4`), plus a
similar explicit block for its `+0x2d`/cached-constants store (`part->
field_2d = 1`, with a `0` cached into `sb` for a `part->field_2c = 0`
write far later and a `1` cached into `r6` for the collected-bits pack -
all materialized before the store itself, not after, the same "constant
before store" ordering this cluster's other functions already needed).
One more real gotcha specific to this instance: `hdr = sub_80197DC()`'s
result is used for its own `+0xc` table dereference *before* getting
aliased into `r8` (`hdr`'s durable home for later) - `r8` can't be an
immediate-offset load's base register in Thumb (the same restriction
`sub_801FDEC` hit for its `+0x6c`/`+0x44` store pair), but here the ROM
sidesteps it entirely by using the fresh, still-low-register return value
in `r0` for the *first* access, only recovering `hdr` from `r8` after `r0`
gets clobbered by an unrelated `ldrsh` - reproduced with one more
`asm volatile` block spelling out the exact `bl`/`mov r8, r0`/dereference/
`add r0, r8` sequence, rather than the `sub_801FDEC`-style plain-C
`hdr`-pinned-in-`r8` access that works everywhere else in this cluster
but doesn't here (a plain-C attempt produced an extra `add r0, r0, #0xc`
address computation instead of folding the offset into the `ldr`'s
immediate, since Thumb can't fold an immediate offset onto a *high*-register
base and this compiler has to compute the address separately when
starting from the `r8`-pinned variable instead of the call's fresh `r0`
return value).

#### Two more compiler-codegen gotchas found finishing `sub_8021388`

Isolated-compile "confirmed matching" from the fourth pass turned out to
still have two real mismatches, only caught by this pass's full clean
`make compare` (per docs/workflow.md's standing warning about exactly
this) - both fixed with small `asm volatile` blocks:

- **The `+0x20` table-offset constant (`0xa2 * 4`).** A plain
  `register s32 off asm("r3") = 0xa2 * 4;` pin is silently ignored for a
  bare constant initializer - this compiler still picks its own register
  (`r1`) for the two-step `mov`/`lsl` synthesis regardless of the pin,
  unlike every other case in this cluster where pinning a *computed* or
  *parameter-derived* value works fine. Spelled out as a 3-instruction
  `asm volatile` block instead, forcing `r3` directly.
- **The `part->field_0A = 1;` / collected-bits-pack `1` write pair.**
  Same "two independent constant writes, ROM materializes both before
  the store" idiom this cluster has hit repeatedly (`sub_8021280`'s
  argument prologue, `sub_802155C`'s `+0x2d` store above) - plain C
  (even with the register-pinned `one` declared and assigned *before*
  the store) still let the compiler schedule the store between the two
  writes rather than after both. Fixed with the same 3-instruction
  `asm volatile` idiom `sub_801FDEC`'s own version of this pack already
  established (`mov r0, #1` / `mov r5, #1` / `strb r0, [r6, #0xa]`).

### Parked as NAKED: `sub_8021280`, `sub_8021480`

Both fully understood, every instruction's *content* confirmed matching
via isolated compile, but both hit the confirmed `r7`-pin gap documented
for `sub_8007114` (src/graphics/graphics.c) and `sub_802190C` above -
this compiler only adds a hard-pinned register to a function's callee-saved
push/pop set when it tracks that register as holding a value live across
a *wider* span than a single inline-asm block, and `r7` in both of these
functions is only ever used as scratch inside one `asm volatile` block
(the position-probe offset marshalling for `sub_8021280`'s middle arm; the
collected-bits pack's mask-byte reload for `sub_8021480`). Every plain-C
technique tried to force `r7`'s inclusion - an unused pinned local, capturing
it as the asm's own output, a trailing "keep it alive" read spanning from
the asm block to the end of the function - failed to get this compiler to
push/pop `r7`, confirming (for two more functions) that this is a genuine,
unconditional toolchain limitation for `r7` specifically, not something
that responds to more C-level effort. Transcribed instruction-for-instruction
from the ROM disassembly instead:

- **`sub_8021280`** - a three-way dispatcher (not part of the "two-line
  text popup" family): if `sub_8023290`/`sub_80232B8`/`sub_8023324`
  (`gUnknown_030012C0`) all say "no" and the current level's
  `gStaticData_0816C86C`-indexed threshold-table entry's guard field
  (offset `+4`, meaning not otherwise understood) is zero, spawns a
  `sub_80071E4`-built part sized `0x64`x`0x64` tagged `0x12`, registering
  into `gUnknown_030012E8`. Otherwise, if the byte at
  `gUnknown_030012D8 + 0x88` is zero, probes a position via
  `sub_801A878(..., id=4)` (returning a pointer whose first two Q8.8
  fields line up with `struct actor`'s own `x`/`y`) and feeds
  `sub_8023500` an `{x - 2, y - 0x1e}` offset pair; otherwise falls
  through to the same `sub_80071E4` spawn as the first arm, sized
  `0x28`x`0x28` instead. Every `sub_80071E4`/`sub_801A878` call still
  marshals `arg3` into `r3` even though neither function's real body
  reads a 4th argument - the same "pass everything, callee ignores the
  rest" convention this whole ROM region's `sub_8009ED0` callers
  establish.
- **`sub_8021480`** - one more "two-line text popup" sibling (a bare
  `sub_80189EC()` header call, no OAM trio, `flags |= 0x10` at the very
  end instead of right after the `+0x29` nibble update).

### Verification

Full clean `make compare` (`La suma coincide`) and `make NON_MATCHING=1
report`, both passing. Issue #31 stays open in the PR text (not every
function across the whole issue's original scope is a real C match -
`sub_8021280`/`sub_8021480` here, plus every other NAKED/`NON_MATCHING`
entry this issue accumulated across all five passes, don't count) but this
retires the last raw bytes this issue's own scope covers - what's left
open against #31 from here is exclusively already-parked functions
(NAKED or `NON_MATCHING`), tracked in docs/status/graphics_loading.md.

## Sixth pass: `sub_801EF0C`-`sub_801FCB4` (12 more, the range before `sub_801FDEC`)

The fifth pass's "last raw bytes this issue's own scope covers" claim
above turned out to miss one stretch: `sub_801EF0C`-`sub_801FCB4`, 12
more "two-line text popup" siblings sitting in the *original*
`asm/code_3_2_17_1e990.s` (the file `sub_801FDEC` itself was extracted
from, back in the second pass) rather than the
`asm/code_3_2_17_1feec.s`/`asm/code_3_2_17_21280.s` files the third
through fifth passes worked through. `tools/report_units.py`'s
`0x0801EA5C` entry flagged this exact range as "a promising real-C
target for whoever picks this up next" and "out of scope" for the pass
that wrote that note - this pass is that follow-up. Semantics for all
12 are the numbered list at the top of this document, unchanged - every
one allocates via `sub_8009ED0` (or, for `sub_801F680`, the bigger
`sub_800A604` constructor), hooks its own fixed offset into the
`gUnknown_030012D0`-rooted table at `+0x20`, updates its `+0x29` frame
nibble via `sub_800815C`, packs the `gUnknown_030012B4` "collected" bits
into `+0x28`, registers into `gUnknown_030012F0`, and closes with one of
several tail shapes this cluster's earlier passes already catalogued
(a single header write, a "second `header->0x84` rewrite plus a
struct-field or record-field copy", an OAM trio, a bit-27 re-test, or -
for `sub_801F680` alone - a `PlaySfx` call). New file:
`src/graphics/graphics_loading_1ef0c.c`, replacing
`asm/code_3_2_17_1e990.o` at that point in `ldscript.txt` (the raw file
itself shrinks to just `sub_801EA5C`-`sub_801EE3C`, the still-raw
"trigger effect type N" twin-family shape noted at the top of this
document - genuinely out of scope for this pass, a separate already-
parked wall per issue #31's own scope note).

### Matched: `sub_801F050`, `sub_801F170`, `sub_801F680`

Real C, confirmed by a full clean `make compare`. These three are the
only ones in the range whose ROM disassembly doesn't need `r7` in its
callee-saved push/pop set - `sub_801F050`/`sub_801F170` shadow only
`sb`/`r8` (two extra high registers) through `r5`/`r6`, and
`sub_801F680` shadows only `r8` (one extra) through `r6`, all comfortably
inside this compiler's own natural register choices at `O2` without
needing to reach for `r7` anywhere. Each needed the same category of
fix, confirmed by isolated-compile diff against the ROM disassembly:

- **Value-before-address (or address-before-value) ordering.** A single
  offset write like `part->0x20 = value` or `hdr->0x44 = header` compiles
  fine either way semantically, but this compiler schedules whichever
  sub-expression is declared/computed first into its own register move
  *first* - the ROM's own order isn't always "compute the address, then
  the value" (see `sub_801F050`'s `part->0x20` write, which needs the
  value materialized into `r0` before `part`'s `r8`→`r1` copy) or always
  the reverse (its `hdr->0x44` write wants `part`'s copy computed
  *after* the store to `hdr->0x6c`, not before). Fixed by reordering the
  C statements/nested-block declarations to match, the same technique
  `sub_801FDEC` already established.
- **A bare-constant register pin is silently ignored.** Exactly the
  `sub_8021388` gotcha this document's fourth/fifth passes already
  flagged (`register s32 off asm("r3") = 0x18;` lands the two-step
  mov/lsl synthesis in whatever register this compiler likes, not the
  pinned one) recurred for `sub_801F170`'s second `sub_803AD80`
  trampoline call, whose `+0x18` offset constant needs `r3` specifically
  (the *first* trampoline call in the same function reuses `r2` instead -
  the classic "no CSE across a call" scheduling gap this whole cluster's
  earlier passes already documented, just for a register choice this
  time instead of a reload). Fixed by hand-spelling the whole trampoline
  call - argument marshalling, offset constant, and `bl` - as one
  `asm volatile` block, the same escape hatch `sub_8021388` used for its
  own `+0x20` table-offset constant. The identical fix was needed for
  both of `sub_801F680`'s two trampoline calls (`hdr` lives in `r8`
  there, so the "avoid an immediate-offset load off a high-register
  base" idiom `sub_802155C` established layers on top of the same
  constant-pin gotcha).
- **A hard-pinned register still "reserved" after its C-level scope
  ends can't be reused for an unrelated later value in the same
  function - unless the reuse is spelled out as raw asm text.** Plain-C
  re-declaration of a *second*, differently-scoped `register T x
  asm("r5")` local later in `sub_801F170` (after the first `r5`-pinned
  local's block had already closed) silently landed in `r3` instead,
  even though the exact same "reuse a hard register across sibling
  blocks" technique works everywhere else in this cluster for r0-r3.
  Only `r5` specifically hit this in this pass; reusing `r0`/`r1` for
  unrelated locals in later blocks of the same functions worked with no
  issue. Fixed by writing the final struct-field-write block as one
  `asm volatile` island instead of separately-pinned C locals.
- **`sub_801F050`'s `part` lives in `r8`, `sub_801F170`'s `part` lives in
  `r4`.** Same `sub_801FDEC`-style "pin whichever register the ROM
  actually used" technique, just for `part` instead of `hdr` this time -
  confirms the technique generalizes to any of this family's live
  pointers, not just the header.

### Parked as NAKED: the other nine

`sub_801EF0C`, `sub_801F2BC`, `sub_801F3DC`, `sub_801F528`,
`sub_801F7B8`, `sub_801F8DC`, `sub_801FA3C`, `sub_801FB74`,
`sub_801FCB4` all hit the confirmed `r7`-in-the-callee-saved-set gap
`sub_8021280`/`sub_8021480`/`sub_802190C` already established for this
project: each one's ROM disassembly needs `r7` in its
`push {..., r7, lr}`/`pop {..., r7}` prologue/epilogue, shadowing a
third extra high register (`sl`, alongside `sb`/`r8`) through `r7`
itself, or `r7` gets used as pure scratch inside one or two *disjoint*
single-instruction-island spots (the `+0x29` nibble reload, or the
collected-bits pack's own mask-byte reload) that never asks the
compiler to treat `r7` as live across a wider span. Every one of these
nine had every instruction's *operation* already confirmed matching via
isolated compile before being transcribed - this is purely the
categorical toolchain gap, not a semantics gap. No new register-pinning
technique was found for this pass (the same techniques that worked for
`sub_801F050`/`sub_801F170`/`sub_801F680` above, and every real-C match
elsewhere in this cluster, were tried first and consistently failed to
get `r7` into the push/pop list here, exactly as documented for
`sub_8021280`/`sub_8021480` in the fifth pass above) - transcribed
instruction-for-instruction from the ROM disassembly instead, the same
escape hatch used throughout this project. Two additional tail-shape
variants get their first real writeup here (the rest reuse shapes
already catalogued by earlier passes in this document):

- **`sub_801F3DC`** computes a genuine average-then-quarter: `s32 half =
  (record.f4 + record.f8) / 2; s32 quarter = half / 4;`, written into
  `hdr->0x48`/`hdr->0x4c` - this compiler's own signed-division-by-a-
  power-of-2 idiom (the branchless `(x + ((unsigned)x >> 31)) >> 1` trick
  for `/2`, but the branching `cmp`/`bge`/`add #3`/`asr #2` form for the
  second `/4` on an already-computed value) reproduced exactly in the
  transcription.
- **`sub_801FB74`** is the "bit-27 test on `part->field_28` gating a
  different tag value" variant the second pass's writeup flagged but
  never worked through: after the OAM trio, it re-reads the same
  `+0x28` byte the collected-bits pack just wrote, tests bit 4 via
  `lsl r0, r2, #0x1b` (putting that bit at the sign position for a
  `blt`), and re-toggles it before the closing `sub_800C6A8` call - no
  `header->0x84` rewrite or record relookup in this one's tail at all.

### Verification

Full clean `make compare` (`La suma coincide`) and `make NON_MATCHING=1
report`, both passing. This retires `asm/code_3_2_17_1e990.s` down to
just `sub_801EA5C`-`sub_801EE3C` (the still-raw "trigger effect type N"
twin-family shape, a separate already-parked wall per this issue's own
scope note at the top of this document) - the whole `sub_801EF0C`-
`sub_801FCB4` stretch is now real, always-compiled code (3 matched, 9
NAKED), tracked in `docs/status/graphics_loading.md`.

## Seventh pass: `asm/code_3_2_17_1feec.s` (`sub_801FEEC`-`sub_8020D4C`, 13 functions)

Picked up the raw file `sub_801FDEC` (second pass, above) was split out
of - the whole `asm/code_3_2_17_1feec.s` that survived intact since the
third pass carved `trigger_effect.c`'s twin family out of the *middle*
of the original larger raw file, leaving this 13-function stretch as its
own still-raw remainder (`tools/report_units.py`'s own comment on this
range called it "most of the rest of the chunk 31 range"). All 13 are
one more set of "two-line text popup" family instances, same numbered
skeleton as documented at the top of this file: a `sub_8009ED0`-built
part object, a `gUnknown_030012D0`-rooted `+0x20` table offset, a
`sub_800815C` frame-nibble update, a `gUnknown_030012B4` two-bit
"collected" pack into `+0x28`, a `gUnknown_030012F0` manager
registration, and a header (`sub_800CA74`) with one or two `sub_803AD80`
trampoline calls - varying only the embedded offsets/constants and tail
shape (a header->0x84 double-rewrite plus a record-field/struct-field
copy; the standard OAM trio; a plain record-relookup feeding
`sub_800C6A8`/`sub_800C898`; a bit-27 re-test). New file:
`src/graphics/graphics_loading_1feec.c`, replacing
`asm/code_3_2_17_1feec.o` at the same point in `ldscript.txt` - this
retires that raw file entirely.

### Matched: `sub_8020B0C`

The one function in this stretch whose ROM disassembly avoids the r7
gap: `push {r4, r5, r6, lr}` plus a single lo-register copy of `r8` (no
`sb`, no `r7` at all), same overall register footprint as
`sub_801F050`/`sub_801F680` (`graphics_loading_1ef0c.c`) rather than the
4-low-register-plus-extra-high-registers shape every other function in
this file needs. Reconstructed as real C using exactly those two
functions' established idioms: `part` pinned in `r5`, `hdr` pinned in
`r8` and dereferenced through its own fresh `r0` return value before
being aliased into `r8` (the same "avoid an immediate-offset load off a
high-register base" trick `sub_801F680`/`sub_802155C` already
established), a hand-spelled `asm volatile` island for the whole
constructor-call prologue (raw-register truncation, the `arg3` stash
into `r4`, and the `bl sub_8009ED0` itself), and the same
`register s32 one asm("r6")`-cached collected-bits-pack block
`sub_801F050` uses. Tail is a "second header->0x84 rewrite" variant
that reuses one already-computed address (`hdr + 0x84`, pinned in `r1`)
for both stores rather than recomputing it, with a `part->field_0A`
overwrite (`1` then `7`) sandwiched in between - the same
"value-before-address" ordering and address-reuse-across-statements
technique established throughout this cluster.

One genuine transcription slip surfaced only by the full clean
`make compare` (an isolated compile alone would have hidden it, since
both spellings are instruction-plausible - see docs/workflow.md's
standing warning about exactly this): the collected-bits-pack's own
byte-load scratch register. `sub_801F050`'s version of this same block
uses `r3` for `ldrb r3, [r2]` because that function's `hdr` lives in
`r4` (so `r4` isn't free to reuse as scratch there); `sub_8020B0C` has
`idx` (not `hdr`) living in `r4`, and `idx`'s register is dead by this
point in the block (fully consumed by the address computation just
before), so the ROM reuses `r4` itself for the byte load instead -
matching `sub_801FDEC`'s own version of this exact block, not
`sub_801F050`'s. A first draft copied `sub_801F050`'s `r3` spelling
verbatim without re-deriving which register was actually free in this
function's own layout, producing a checksum failure isolated to exactly
2 bytes (`0x08020ba6`-`0x08020ba7`) by a raw byte `cmp` against
`baserom.gba` - fixed by using `r4` for both the `ldrb` and the
following `lsr`, matching the ROM exactly. The lesson generalizes: which
scratch register a "reuse the dead index/pointer register" idiom picks
depends on what's *actually* live in the surrounding function, not on
which sibling function's version of the same block happens to look most
similar - each instance needs its own liveness check against its own
register assignment, not a blind copy from the nearest matched sibling.

### Parked as NAKED: the other twelve

`sub_801FEEC`, `sub_8020010`, `sub_8020138`, `sub_802026C`,
`sub_80203A8`, `sub_80204EC`, `sub_802062C`, `sub_8020788`,
`sub_80208C4`, `sub_80209EC`, `sub_8020C18`, `sub_8020D4C` all hit the
confirmed r7-callee-saved-set gap this issue has now documented many
times over: each one's ROM disassembly needs r7 in its
`push {r4,r5,r6,r7,lr}`/`pop {...,r7}` prologue/epilogue (shadowing
`sb`/`r8`, or `sl`/`sb`/`r8`, or, for `sub_8020010`, just `r8` alone
through `r7`), while r7 itself is used only as scratch inside disjoint
single-instruction islands (the `+0x29` nibble reload, the
collected-bits pack's mask-byte reload) - never a value this compiler's
own allocator tracks as live across a wider span, which is the
precondition every technique in this project's toolbox needs to get a
register into the callee-saved set at all. No new technique was tried
this pass beyond what `sub_8021280`/`sub_8021480`/`sub_802190C` and the
nine `graphics_loading_1ef0c.c` functions already exhausted for this
exact wall - transcribed instruction-for-instruction from the ROM
disassembly instead, per this project's established NAKED escape hatch.
Every instruction's *operation* was confirmed matching via isolated
compile before transcription for all twelve. Two tail-shape variants
worth noting that hadn't appeared in quite this form before:

- **`sub_802062C`/`sub_8020788`** need all three extra high registers
  (`sl`/`sb`/`r8`) simultaneously, the widest register footprint of any
  function in this file - `sub_802062C` additionally spills its `+0x28`
  record address to a 4-byte stack slot (`sub sp, #4`) across the
  `sub_8008E94` manager-registration call, reloading it from `sp`
  afterward rather than keeping it in a register the call might
  clobber.
- **`sub_80204EC`/`sub_8020D4C`** share a "dependent `sub r0, #0x4b`"
  mask idiom distinct from this cluster's usual `mov #N`/`neg` idiom:
  `part->field_0A` is set to `0xa` (leaving that value live in `r0`),
  then `r0` is directly decremented by `0x4b` (`0xa - 0x4b = -0x41`) and
  ANDed into the flags byte - reusing the just-stored constant rather
  than materializing a fresh negated mask from zero.

### Verification

Full clean `make compare` (`La suma coincide`) and `make NON_MATCHING=1
report`, both passing. This retires `asm/code_3_2_17_1feec.s` entirely -
the file no longer exists, replaced by `src/graphics/graphics_loading_1feec.c`
at the same point in `ldscript.txt`. 1 of the 13 functions in this file is
real C, the other 12 are NAKED, tracked in `docs/status/graphics_loading.md`
and `tools/report_units.py`. Issue #31 stays open - the remaining raw/parked
scope (the "trigger effect type N" twin-family shape at
`sub_801EA5C`-`sub_801EE3C`, plus every NAKED/`NON_MATCHING` entry this
issue has accumulated across all seven passes) is unchanged by this pass
beyond adding twelve more already-parked NAKED entries.
