# Issue #46: 0x08028568-0x08028B7C - the icon/text-widget renderer

GitHub issue #46 (`decomp-chunk`, category `hud`) listed 25 raw functions
in `asm/code_3_2_20_28568.s`. This is the write-up for the work done
against that list.

## What this cluster turned out to be

Two unrelated things, plus one bigger, previously-only-partially-known
object family:

- **`SetHudCrateTotal`/`IncHudCrateTotal`** are a trivial setter/increment pair on
  the same central blink-timer object `src/hud/hud_slide.c` already
  documents (`gHud`), extending its known field range to
  `+0x28`. Contiguous with `hud_slide.c`'s existing functions, so they
  were appended there rather than getting a new file.
- **`DestroyHud`** is a `struct hud_counter`'s `parts`-array destructor
  (see `include/hud.h`) - unrelated to everything else in this chunk,
  just adjacent in ROM.
- **Everything else** (`FontDrawGlyph` through `FontSetTileBase`) operates on
  `struct bitmap_font` (`include/bitmap_font.h`), an object
  `src/menus/power_dialog_draw.c`/`src/text/wrapped_text.c` had already
  partially characterized (an OAM icon positioner with `posX`/`posY`/
  `record` fields) but left most of its leading `unused_00`/`unused_10c`/
  `unused_118` byte ranges opaque. This chunk's functions read and write
  those ranges directly, so `include/bitmap_font.h` now documents the
  real shape:
  - `oam_scratch[8]` (was `unused_00`'s first 8 bytes) - a 6-byte
    OAM-shaped draw-request scratch buffer `FontDrawGlyph` rebuilds fresh
    per glyph and hands to `AddOamEntry`.
  - `charLookup[0x100]` (the rest of `unused_00`) - a reverse
    char-byte -> glyph-index lookup table, built once by
    `InitSmallFont`/`InitLargeFont` from a small font-glyph
    order table (`gSmallFontChars`/`gLargeFontChars`).
  - `glyphRecords` (was `unused_10c`) - pointer to a
    `struct icon_glyph_metrics` array (width/shape/tile-row per glyph),
    12 bytes/entry, indexed by `charLookup`.
  - `field_118`/`field_11c`/`spaceWidth`/`field_124`/`field_128` (was
    `unused_118`) - left margin X, line height (confirmed against
    `src/text/text_box.c`'s existing `DrawWrappedTextInBox`, which already used
    this same field as a divisor), space-character advance width, a
    per-glyph OAM-attribute stride, and a pointer to the widget's own
    upload asset table respectively.

  The family is a small text/icon renderer: `InitSmallFont`/
  `InitLargeFont`/`InitFont` are three constructor variants
  (different font/glyph tables); `FontDrawGlyph` draws one glyph and
  advances `posX`; `FontPutChar`/`FontDrawText` are per-character
  dispatchers (newline/space/else-draw); `FontDrawChars` draws a
  fixed-count run of characters; `FontMeasureChars`/`FontTextHeight`/
  `FontMeasureText` are text-measurement helpers (line width, block height,
  widest line); `FontUploadTiles` uploads the glyph sheet to OBJ VRAM; the
  rest of `FontHeightToLines`-`FontSetTileBase` are trivial field
  getters/setters/trampoline-forwarders.

## Matched (17 functions, full clean `make compare` passing)

`src/hud/hud_slide.c` (appended): `SetHudCrateTotal`, `IncHudCrateTotal`.
`src/hud/hud_slide.c`: `DestroyHud`.
`src/text/font_draw_chars.c`: `FontDrawChars`.
`src/text/font_height.c`: `FontTextHeight`.
`src/text/font.c`: `FontUploadTiles`, `FontSetPalette`,
`FontResetPalette`.
`src/text/font.c`: `FontHeightToLines`, `FontGetTileCount`,
`FontSetPos`, `FontNewLineAt`, `FontGetMargin`, `FontSetMargin`,
`FontGetY`, `FontGetX`, `FontSetTileBase`.

Because matched and parked functions interleave in ROM order, the
matched runs above sit in **five** separate small `.c` files rather than
one - each is exactly one contiguous matched span, alternating in
`ldscript.txt` with the raw `.s` fragments below so every function keeps
its real ROM address.

## Parked (`NON_MATCHING`) - 8 functions

All eight are fully understood (semantics, field offsets, and call/branch
topology confirmed against the ROM disassembly) but don't yet produce
byte-identical output from `tools/agbcc`. Real bytes live in four raw
fragments, each guarded `.if NON_MATCHING == 0`, with `#if NON_MATCHING`
C reconstructions in four matching files (then `hud_icon_widget_*.c`, now
`src/text/font_glyph.c`, `font_draw_text.c`, `font_measure.c` and `font.c`):

- **`asm/code_3_2_20_85c4.s`** (`FontDrawGlyph`, `InitSmallFont`,
  `InitLargeFont`, `FontPutChar`) - reconstructions in
  `src/text/font_glyph.c`.
- **`asm/code_3_2_20_8890.s`** (`FontDrawText`, `FontMeasureChars`) -
  reconstructions in `src/text/font_draw_text.c`.
- **`asm/code_3_2_20_8994.s`** (`FontMeasureText`) - reconstruction in
  `src/text/font_measure.c`.
- **`asm/code_3_2_20_8a78.s`** (`InitFont`) - reconstruction in
  `src/text/font.c`.

Two distinct residual gaps, both already-known classes of gcc-2.9
difficulty in this codebase:

1. **`InitSmallFont`/`InitLargeFont`/`InitFont`** (all
   three share the same `posX`/`posY` zero-init preamble): the ROM
   computes both field addresses in ascending-offset order (sharing the
   `+4` constant delta between `+0x110` and `+0x114`) but *stores*
   through them in the opposite order (`posY` first, then `posX`).
   Tried: plain struct-field assignment both orderings, raw `(u8 *)self
   + N` casts both orderings, and a shared base-pointer local
   (`u8 *base = self + 0x110; *(u32*)(base+4) = 0; *(u32*)base = 0;`,
   which does get the store order right but collapses the two address
   computations into a single base+immediate-offset store instead of
   the ROM's two independently-computed zero-offset addresses). None
   reproduced both the address-computation order and the store order at
   once.
2. **`FontDrawGlyph`**: `self` sits in `ip`/`r12` here instead of the
   ROM's `r3` - too many simultaneously-live values (the glyph index,
   three re-derived `rec` pointers reloaded around the `AddOamEntry`
   call, `self` itself) for this compiler to fit into `r4`-`r7` the way
   the ROM does. Tried explicit `&self->posX`/`&self->glyphRecords`
   locals matching the ROM's own address-caching shape, and plain
   repeated field access - neither changed the register choice.
3. **`FontPutChar`/`FontDrawText`**: the ROM lowers the 3-way
   `if (newline) {...} else if (space) {...} else {dispatch}` so the
   *middle* arm (space) ends up inline and the other two become
   jumped-to blocks in test order, with a shared two-instruction tail
   ("`dest += self->offset`") folded out of the newline/space arms.
   Tried hoisting that tail into explicit `destAddr`/`offset` locals
   (does trigger the compiler's own tail-merge, confirmed) and a
   `switch` (produces a different, also-wrong block order) - neither
   reproduced the exact block layout.
4. **`FontMeasureChars`/`FontMeasureText`**: the ROM pins `str`/`&glyphRecords`
   (or `self`/`&spaceWidth` for `FontMeasureText`) into `r8`/`sb`/`ip`,
   spilling them across the loop's own `bl _call_via_r3`/`GetPaletteSlot`-
   style calls - the same class of gap already documented for
   `DrawPowerDialog`/`InitLanguageSelectGraphics` elsewhere in this codebase (see
   `src/frontend/language_select_setup.c`'s comment on the latter).

## Real gotchas found along the way (useful beyond this issue)

1. **Trailing function-alignment padding is compiler-fill, not
   zero-fill, unless forced.** `DestroyHud` (`hud_slide.c`) and
   `FontTextHeight` (`font_height.c`) both end 2 bytes short of a
   4-byte boundary, immediately followed by a raw `.s` fragment whose
   `.align 2, 0` directive (correctly) zero-fills the gap. Left alone,
   `tools/agbcc` fills the same gap with its own trailing alignment
   `nop` (`0x46c0`, "mov r8, r8") instead of zero bytes - byte-identical
   in isolation, but a real mismatch once linked next to the ROM's own
   zero-padding. Fixed with a trailing `asm(".align 2, 0");` statement
   right after each function, per the existing
   `matching_decomp_alignment_fix` convention (see `graphics.c`'s
   `InitSpriteBankSet` for the same pattern already in this codebase). Caught
   via the map-file address-shift method from `docs/workflow.md`: `cmp`
   the built ROM against `baserom.gba`, convert the first differing
   byte offset to a ROM address, and look it up in
   `crashbandicootxs.map`.
2. **Statement order controls register *order*, not just presence,
   for immediate-vs-load combine pairs.** `FontSetPalette`'s
   `self->oam_scratch[5] = (self->oam_scratch[5] & 0xF) | (val << 4)`
   needed the constant `0xF` materialized into a lower-numbered register
   than the loaded byte, matching the ROM's `movs r2,#0xf` before
   `ldrb r3,[r0,#5]` - plain C (any operand order, any statement
   split) kept picking the opposite register assignment. Fixed with
   `register u8 mask asm("r2")`/`register u8 field asm("r3")` pins plus
   an `asm volatile("" : "+r"(mask))` barrier between materializing the
   mask and loading the field, forcing both the pin and the order.
   `FontResetPalette` hit the exact same shape (verified against
   `baserom.gba` directly, not just an isolated re-transcription of the
   ROM disassembly - the isolated comparison the first pass used had
   mistakenly matched only the surrounding instructions and missed this
   swap) and got the identical fix.

## Cross-references

- `docs/status/hud.md` - matched/parked lists updated.
- `include/bitmap_font.h` - `struct bitmap_font`'s `unused_00`/
  `unused_10c`/`unused_118` byte ranges filled in with real named
  fields, and the new `struct icon_glyph_metrics` type added.
- `src/frontend/language_select_setup.c` - `InitLanguageSelectGraphics`'s own comment
  documents the same r8/r9-register-pressure class of gap hit by
  `FontMeasureChars`/`FontMeasureText` here.
- `src/util/aabb_setup.c` - `DestroyLargeFont`/`DestroySmallFont`
  document the identical "two `self+0x130` stores in a row, the first
  genuinely dead" pattern reused by `InitSmallFont`/
  `InitLargeFont`/`InitFont` here.

## Second pass

Follow-up pass over the 8 functions this issue's first pass left
parked, working the remaining GitHub issue #46 scope.

### Matched (3 of 8) - full clean `make compare` passing

- **`FontPutChar`** (`src/text/font_glyph.c`, now split
  out of the still-parked `FontDrawGlyph`/`InitSmallFont`/
  `InitLargeFont` at the end of the same file - `#if NON_MATCHING`
  now only wraps those three). The documented `if`/`else if`/`else`
  block-layout gap was fixed with explicit `goto`s to force the ROM's
  exact physical block order (space arm inline as the fallthrough,
  newline and dispatch arms as jumped-to blocks in test order, tail
  reached both ways). The `self`/`charByte` prologue-order gap
  (previously undocumented as fully understood) turned out to be this
  compiler unconditionally widening a `u8` parameter's zero-extension
  before anything else, regardless of source order - worked around by
  taking `charByte` as a raw `u32` (sidestepping the byte-promotion
  invariant that forces the early widen) and writing the three-
  instruction prologue as one literal `asm volatile` block.
- **`FontDrawText`** (`src/text/font_draw_text.c`, split out of
  the still-parked `FontMeasureChars` at the top of the same file). Same
  `goto`-based block-order fix as `FontPutChar`, plus explicit register
  pins for `self`/`str`/the cached `&posX` (`r4`/`r5`/`r6`) - `&posY`
  deliberately left *unpinned* (see the r7 finding below). Also hit the
  project's known trailing-alignment-padding gotcha (function ends 2
  bytes short of a 4-byte boundary; this compiler's own padding `nop`
  isn't the ROM's zero-fill) - fixed with the standard
  `asm(".align 2, 0")` following statement.
- **`InitFont`** (`src/text/font.c`) - no
  charLookup-building loop like its `InitSmallFont`/`B` siblings,
  so once the shared preamble's inline-asm address anchors were right,
  this one reached a full match with nothing left over. The file no
  longer has an `#if NON_MATCHING` guard at all.

### A genuinely new technique: literal inline-asm address anchors for the shared preamble

`FontDrawGlyph`/`InitSmallFont`/`InitLargeFont`/`InitFont`
all share a `record`/`posX`/`posY`/`field_118`/`field_12c` zero-init
preamble. Plain C (any statement order, any struct-field-vs-raw-offset
phrasing) never reproduced two things at once: the ROM *recomputes*
`&record` fresh for each of its two stores instead of caching the
address across the `CpuSet` call in between (a CSE this compiler
applies unconditionally to repeated `self->record = X` assignments),
and the `posX`/`posY`/`field_118`/`field_12c` zero stores compute their
addresses in ascending-offset order but store through them in a
different order than they were computed. Fixed by writing the ROM's
literal instruction sequence as `asm volatile` blocks with generic
`"=r"` outputs (letting the register allocator still pick freely,
avoiding the r7 hazard below) - the same address-anchor idiom
`aabb_setup.c`'s `DestroyLargeFont`/`DestroySmallFont` already established,
just scaled up to a longer shared sequence. `InitFont` applies this
whole; `InitSmallFont`/`B` apply it too but the loop past it (next
section) still blocks a full match.

### The r7-pinning toolchain bug, and why it blocks a full match on 4 of the remaining 5

Chasing `InitSmallFont`/`InitLargeFont`'s charLookup-building
loop and `FontMeasureChars`/`FontMeasureText` surfaced (and got independently
confirmed by a minimal standalone repro) a real, silent ABI-violation
bug in this specific agbcc/gcc-2.9-arm toolchain: `register T x
asm("r7")` - or, it turns out, *any* inline-asm use of r7, even a bare
clobber or an `"=r"`-constrained output bound to r7 via a register
variable - compiles with **no push/pop of r7 at all**, silently
corrupting the caller's r7 across the call. Values can only safely live
in r7 here via natural, unforced allocation (gcc's own allocator picks
it correctly, with proper save/restore, when nothing forces it there).
Worse, this session found the bug is *contagious*: pinning enough
*other* hard registers (not r7 itself) can starve whatever's left for
r7, making the unforced allocator drop it from the callee-saved set too
- hit for both `InitSmallFont`'s charLookup loop (pinning `count`
to r7 outright broke codegen outright - see the function's own comment)
and `FontMeasureChars` (pinning *either* `&spaceWidth` or `&charLookup` to
their ROM registers, `ip`/`r6`, made r7 drop out even though r7 itself
was left alone). The base bug (never pin r7 explicitly) was already
documented in `docs/matching.md`'s "Why not just pin r7" and
`matching_decomp_register_pinning` memory point 10 from earlier
sessions; this "contagious" refinement - enough *other* pins can starve
r7 even without touching it - is new and recorded here, not in
`docs/matching.md` itself (frozen, never gains new entries - see that
file's own header).

Net effect: `InitSmallFont`/`InitLargeFont`'s preambles are
now byte-exact (see above), but their charLookup-building loops -
which the ROM allocates with the table's leading count byte
permanently in r7 - could not be forced to match; `FontMeasureChars`/
`FontMeasureText` hit the identical class of gap for a different r7-held
value (a glyph-index byte inside the `else` arm). All four got real,
verified-not-just-eyeballed improvements (see the isolated-compile
lesson below) via safe pins (`r4`/`r5`/`r8`/`sb`/`r3` etc.) and the
address-anchor technique, and are left parked with the exact remaining
gap documented in each function's own comment, rather than continuing
to chase a confirmed toolchain limitation.

`FontDrawGlyph` itself (the fourth already-parked function in this
issue's original scope) was looked at again but not usefully improved
this pass - it needs the same address-anchor treatment applied to a
much larger set of bitfield-masking stores (`0xFE00`/`0xFC00`-style
16-bit masks built from a 32-bit literal-pool load in the ROM, vs. this
compiler's own shift-construction of the same mask), a distinct problem
from anything solved above; left with its original documented gap
rather than risk a low-confidence partial change.

### A second isolated-compile-vs-full-build lesson

Early in this pass, "matches" was provisionally claimed for functions
based on manually eyeballing an isolated-compile disassembly next to
the ROM listing - `FontPutChar` in particular looked identical this
way. A normalized, scripted instruction-by-instruction diff (stripping
comment text, canonicalizing hex-vs-decimal immediates and 2-operand-
vs-3-operand `add`/`sub` forms, but *not* register numbers) caught real
remaining mismatches manual reading had missed, in a function already
mentally filed as "done." This is the same category of mistake
`docs/workflow.md` step 3 already warns about (an isolated compile is
diagnostic, never proof) - the lesson refined here is that even a
*careful manual read* of an isolated compile is not reliable enough on
its own; only the step-6 full clean `make compare` against the real ROM
is proof, and a scripted diff is a much better *intermediate* check
than eyeballing before paying for that full rebuild. The alignment-
padding gotcha on `FontDrawText` (previous section) was caught exactly
this way too - the scripted diff was clean, but the full `make compare`
still failed, tracked down via the map-file address-shift method
`docs/workflow.md` describes.

### Remaining scope

`FontDrawGlyph`, `InitSmallFont`, `InitLargeFont`, `FontMeasureChars`,
`FontMeasureText` are still parked - genuinely resistant to this toolchain,
not unattempted. Issue #46 stays open; a future pass could revisit
`FontDrawGlyph`'s bitfield-mask gap (a distinct, more tractable-looking
problem than the r7 wall the other four hit), or wait for/investigate a
workaround to the r7 toolchain bug itself given how many parked
functions across this codebase cite it.

`DestroyFont` (`0x08028B7C`, immediately after this issue's
range) remains fully raw and out of scope - not attempted this pass.

## Third pass: NAKED-transcription - all 25 functions now matched

Follow-up pass over the five functions the second pass left parked:
`FontDrawGlyph`, `InitSmallFont`, `InitLargeFont`,
`FontMeasureChars`, `FontMeasureText`. All five are confirmed blocked by the same
r7-pinning toolchain bug documented in the second pass above (`register
T x asm("r7")`, or any inline-asm use of r7 at all - even indirectly, by
pinning enough *other* hard registers to starve the unforced allocator's
own r7 choice - compiles with no push/pop of r7, silently corrupting the
caller's r7 across the call). Rather than keep chasing plain-C
workarounds around a confirmed compiler bug, this pass transcribed all
five directly as `NAKED` asm functions instead - the same technique this
project already uses elsewhere for this exact class of problem
(`lib/libgcc/lib1funcs.s`'s `__div0`, `lib/gax/src/gax_swi.c`'s
`GaxHuffUnComp`, `src/link/link_handshake.c`'s `MakeLinkHandshakeId`/`UpdateLinkSession`).
A NAKED function has no compiler-generated prologue/epilogue or
register allocation at all, so the r7 bug (and any other codegen
mismatch) is moot - the instructions are typed in verbatim, checked
byte-by-byte against the ROM disassembly's unified-syntax mnemonics
translated to this project's divided-syntax convention (`adds`->`add`,
`ands`->`and`, `lsls`/`lsrs`->`lsl`/`lsr`, `orrs`->`orr`, `subs`->`sub`,
`muls`->`mul`, `movs`->`mov`; `cmp`/`ldrb`/`strb`/`ldrh`/`strh`/`b`/
conditional branches/`bl`/`bx` unchanged).

All five real-bytes raw `.s` fragments this issue's scope depended on
are now gone entirely, and the ldscript entries removed with them:

- `asm/code_3_2_20_85c4.s` deleted; `FontDrawGlyph`, `InitSmallFont`,
  `InitLargeFont` now live as `NAKED` functions in
  `src/text/font_glyph.c`, ahead of the already-matched
  plain-C `FontPutChar`. No `#if NON_MATCHING` guard anywhere in the
  file any more.
- `asm/code_3_2_20_8890.s` deleted; `FontMeasureChars` now lives as a
  `NAKED` function in `src/text/font_draw_text.c`, after the
  already-matched plain-C `FontDrawText`.
- `asm/code_3_2_20_8994.s` deleted; `FontMeasureText` now lives as a
  `NAKED` function, alone, in `src/text/font_measure.c`.

This brings issue #46 to full completion: all 25 functions in the
original chunk are matched, closing the issue.

Two small file-organization notes for future reference:

- `FontMeasureChars`'s trailing `.align 2, 0` (the function ends 2 bytes
  short of a 4-byte boundary in the ROM, same gotcha documented in the
  first pass above) is now written directly inside the `NAKED` asm
  string's own literal text, rather than as a separate following
  `asm(".align 2, 0");` statement (that idiom only applies to
  compiler-generated functions, where it works around the compiler's
  *own* padding choice - a `NAKED` function's raw asm text controls its
  own trailing bytes directly).
- `FontMeasureText`'s ROM bytes happen to already end on a 4-byte boundary
  (no trailing `.align` was present in the original raw `.s` fragment
  either), so its `NAKED` transcription needs no alignment directive at
  all.
