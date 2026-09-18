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
