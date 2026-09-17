# Issue #46: 0x08028568-0x08028B7C - the icon/text-widget renderer

GitHub issue #46 (`decomp-chunk`, category `hud`) listed 25 raw functions
in `asm/code_3_2_20_28568.s`. This is the write-up for the work done
against that list.

## What this cluster turned out to be

Two unrelated things, plus one bigger, previously-only-partially-known
object family:

- **`sub_8028568`/`sub_802856C`** are a trivial setter/increment pair on
  the same central blink-timer object `src/graphics/hud_blink.c` already
  documents (`gUnknown_03001318`), extending its known field range to
  `+0x28`. Contiguous with `hud_blink.c`'s existing functions, so they
  were appended there rather than getting a new file.
- **`sub_8028574`** is a `struct hud_counter`'s `parts`-array destructor
  (see `include/hud.h`) - unrelated to everything else in this chunk,
  just adjacent in ROM.
- **Everything else** (`sub_80285C4` through `sub_8028B58`) operates on
  `struct icon_manager` (`include/icon_manager.h`), an object
  `src/graphics/oam_count.c`/`src/graphics/text_layout.c` had already
  partially characterized (an OAM icon positioner with `posX`/`posY`/
  `record` fields) but left most of its leading `unused_00`/`unused_10c`/
  `unused_118` byte ranges opaque. This chunk's functions read and write
  those ranges directly, so `include/icon_manager.h` now documents the
  real shape:
  - `oam_scratch[8]` (was `unused_00`'s first 8 bytes) - a 6-byte
    OAM-shaped draw-request scratch buffer `sub_80285C4` rebuilds fresh
    per glyph and hands to `sub_8006AC8`.
  - `charLookup[0x100]` (the rest of `unused_00`) - a reverse
    char-byte -> glyph-index lookup table, built once by
    `InitHudIconWidgetA`/`InitHudIconWidgetB` from a small font-glyph
    order table (`gStaticData_08174D84`/`gStaticData_08175188`).
  - `glyphRecords` (was `unused_10c`) - pointer to a
    `struct icon_glyph_metrics` array (width/shape/tile-row per glyph),
    12 bytes/entry, indexed by `charLookup`.
  - `field_118`/`field_11c`/`spaceWidth`/`field_124`/`field_128` (was
    `unused_118`) - left margin X, line height (confirmed against
    `src/util/word_util.c`'s existing `sub_8001214`, which already used
    this same field as a divisor), space-character advance width, a
    per-glyph OAM-attribute stride, and a pointer to the widget's own
    upload asset table respectively.

  The family is a small text/icon renderer: `InitHudIconWidgetA`/
  `InitHudIconWidgetB`/`sub_8028A78` are three constructor variants
  (different font/glyph tables); `sub_80285C4` draws one glyph and
  advances `posX`; `sub_8028808`/`sub_8028890` are per-character
  dispatchers (newline/space/else-draw); `sub_8028860` draws a
  fixed-count run of characters; `sub_8028900`/`sub_8028968`/
  `MeasureText` are text-measurement helpers (line width, block height,
  widest line); `UploadHudTile` uploads the glyph sheet to OBJ VRAM; the
  rest of `sub_8028AC4`-`sub_8028B58` are trivial field
  getters/setters/trampoline-forwarders.

## Matched (17 functions, full clean `make compare` passing)

`src/graphics/hud_blink.c` (appended): `sub_8028568`, `sub_802856C`.
`src/graphics/hud_icon_widget.c`: `sub_8028574`.
`src/graphics/hud_icon_widget2.c`: `sub_8028860`.
`src/graphics/hud_icon_widget3.c`: `sub_8028968`.
`src/graphics/hud_icon_widget4.c`: `UploadHudTile`, `sub_8028A30`,
`sub_8028A40`.
`src/graphics/hud_icon_widget5.c`: `sub_8028AC4`, `sub_8028ADC`,
`sub_8028AE8`, `sub_8028B04`, `sub_8028B28`, `sub_8028B34`,
`sub_8028B40`, `sub_8028B4C`, `sub_8028B58`.

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
C reconstructions in four matching `hud_icon_widget_*.c` files:

- **`asm/code_3_2_20_85c4.s`** (`sub_80285C4`, `InitHudIconWidgetA`,
  `InitHudIconWidgetB`, `sub_8028808`) - reconstructions in
  `src/graphics/hud_icon_widget_85c4.c`.
- **`asm/code_3_2_20_8890.s`** (`sub_8028890`, `sub_8028900`) -
  reconstructions in `src/graphics/hud_icon_widget_8890.c`.
- **`asm/code_3_2_20_8994.s`** (`MeasureText`) - reconstruction in
  `src/graphics/hud_icon_widget_8994.c`.
- **`asm/code_3_2_20_8a78.s`** (`sub_8028A78`) - reconstruction in
  `src/graphics/hud_icon_widget_8a78.c`.

Two distinct residual gaps, both already-known classes of gcc-2.9
difficulty in this codebase:

1. **`InitHudIconWidgetA`/`InitHudIconWidgetB`/`sub_8028A78`** (all
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
2. **`sub_80285C4`**: `self` sits in `ip`/`r12` here instead of the
   ROM's `r3` - too many simultaneously-live values (the glyph index,
   three re-derived `rec` pointers reloaded around the `sub_8006AC8`
   call, `self` itself) for this compiler to fit into `r4`-`r7` the way
   the ROM does. Tried explicit `&self->posX`/`&self->glyphRecords`
   locals matching the ROM's own address-caching shape, and plain
   repeated field access - neither changed the register choice.
3. **`sub_8028808`/`sub_8028890`**: the ROM lowers the 3-way
   `if (newline) {...} else if (space) {...} else {dispatch}` so the
   *middle* arm (space) ends up inline and the other two become
   jumped-to blocks in test order, with a shared two-instruction tail
   ("`dest += self->offset`") folded out of the newline/space arms.
   Tried hoisting that tail into explicit `destAddr`/`offset` locals
   (does trigger the compiler's own tail-merge, confirmed) and a
   `switch` (produces a different, also-wrong block order) - neither
   reproduced the exact block layout.
4. **`sub_8028900`/`MeasureText`**: the ROM pins `str`/`&glyphRecords`
   (or `self`/`&spaceWidth` for `MeasureText`) into `r8`/`sb`/`ip`,
   spilling them across the loop's own `bl sub_803AD84`/`sub_8006DF8`-
   style calls - the same class of gap already documented for
   `sub_8006600`/`sub_8037388` elsewhere in this codebase (see
   `src/audio/counter_selector_setup.c`'s comment on the latter).

## Real gotchas found along the way (useful beyond this issue)

1. **Trailing function-alignment padding is compiler-fill, not
   zero-fill, unless forced.** `sub_8028574` (`hud_icon_widget.c`) and
   `sub_8028968` (`hud_icon_widget3.c`) both end 2 bytes short of a
   4-byte boundary, immediately followed by a raw `.s` fragment whose
   `.align 2, 0` directive (correctly) zero-fills the gap. Left alone,
   `tools/agbcc` fills the same gap with its own trailing alignment
   `nop` (`0x46c0`, "mov r8, r8") instead of zero bytes - byte-identical
   in isolation, but a real mismatch once linked next to the ROM's own
   zero-padding. Fixed with a trailing `asm(".align 2, 0");` statement
   right after each function, per the existing
   `matching_decomp_alignment_fix` convention (see `graphics.c`'s
   `nullsub_1` for the same pattern already in this codebase). Caught
   via the map-file address-shift method from `docs/workflow.md`: `cmp`
   the built ROM against `baserom.gba`, convert the first differing
   byte offset to a ROM address, and look it up in
   `crashbandicootxs.map`.
2. **Statement order controls register *order*, not just presence,
   for immediate-vs-load combine pairs.** `sub_8028A30`'s
   `self->oam_scratch[5] = (self->oam_scratch[5] & 0xF) | (val << 4)`
   needed the constant `0xF` materialized into a lower-numbered register
   than the loaded byte, matching the ROM's `movs r2,#0xf` before
   `ldrb r3,[r0,#5]` - plain C (any operand order, any statement
   split) kept picking the opposite register assignment. Fixed with
   `register u8 mask asm("r2")`/`register u8 field asm("r3")` pins plus
   an `asm volatile("" : "+r"(mask))` barrier between materializing the
   mask and loading the field, forcing both the pin and the order.
   `sub_8028A40` hit the exact same shape (verified against
   `baserom.gba` directly, not just an isolated re-transcription of the
   ROM disassembly - the isolated comparison the first pass used had
   mistakenly matched only the surrounding instructions and missed this
   swap) and got the identical fix.

## Cross-references

- `docs/status/hud.md` - matched/parked lists updated.
- `include/icon_manager.h` - `struct icon_manager`'s `unused_00`/
  `unused_10c`/`unused_118` byte ranges filled in with real named
  fields, and the new `struct icon_glyph_metrics` type added.
- `src/audio/counter_selector_setup.c` - `sub_8037388`'s own comment
  documents the same r8/r9-register-pressure class of gap hit by
  `sub_8028900`/`MeasureText` here.
- `src/graphics/actor_aabb_setup.c` - `sub_803AFF0`/`sub_803B024`
  document the identical "two `self+0x130` stores in a row, the first
  genuinely dead" pattern reused by `InitHudIconWidgetA`/
  `InitHudIconWidgetB`/`sub_8028A78` here.
