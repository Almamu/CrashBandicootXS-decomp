# `0x080372BC` - counter widget icon draw loop / tile-cache init (issue #67)

`DrawLanguageSelect`/`InitLanguageSelectGraphics` (`src/audio/counter_selector_icons.c`) are
the small on-screen "counter" widget's (`src/audio/counter_selector.c`,
`RunLanguageSelect`'s loop) per-frame icon draw pass and its one-time tile-cache
init helper. Both are fully understood - previously left raw
(`docs/status/audio.md`: "hit the same many-register gcc-2.9
allocation difficulty already documented for `DrawPowerDialog`"); this
pass converted them to byte-verified NAKED asm transcriptions, moving
them from "left raw" to "parked".

## What each function does

`DrawLanguageSelect` draws six digit slots (0-5), one per widget value: for
each slot index it either shows the shared "selected" overlay
(`gSmallFont`, a `struct bitmap_font *`, drawn with value 1 or
2 from `LanguageSelectBlink` depending on a bit of `self->field_0`, the
widget's free-running frame counter) when the loop index equals
`self->field_8` (the widget's current 0-5 value), or blanks it (value
0 via the same `FontSetPalette` call) otherwise. It then always draws
that slot's own fixed digit glyph - one of six `struct bitmap_font *`
looked up from `gLanguageNames[i]` - positioned via
`_call_via_r2` at a fixed X (centered from the rendered pixel width,
the same `(240-w)>>1` idiom `DrawPowerDialog`/`src/graphics/oam_count.c`
uses) and a Y that steps by `0xa` per slot from a `0x32` base, reading
and writing `gSmallFont->record->slots[0]`/`slots[2]` (see
`include/bitmap_font.h`) as its OAM-slot-record pair - the exact same
shape `DrawPowerDialog` uses on the same two globals.

`InitLanguageSelectGraphics` resets several OAM-manager globals
(`gOamBuffer`/`gPaletteCache`), then hand-fills
`gPaletteCache`'s (`struct palette_cache`, `include/vram_pool.h`)
`slots[0]`-`slots[3]` with 4 fixed 32-byte OBJ tiles copied from
`gLanguageSelectPalette0`/`_74C`/`_76C`/`_78C` (two tiles filled per loop
iteration via a doubled offset, 16 iterations), and finally threads
`gSmallFont`'s/`gLargeFont`'s `record->slots[6]`
trampoline (`_call_via_r1`) and VRAM-reserve (`ReserveObjVram`) pair -
copying `field_12c` from one `bitmap_font` into the other's
`field_108` via the ROM's own "subtract `0x24` from the already-loaded
`0x12c` offset constant" trick rather than a fresh `0x108` literal.

## Why this parked as NAKED instead of matching

Real C was attempted for both functions - an indexed loop and a
pointer-increment loop for the 4-tile copy, caching
`&gSmallFont`/`&gLargeFont`/`&gObjVramCursor` in a
local versus re-deriving them at each use, and the digit-slot loop's
centering-math rewritten several ways - but every attempt fell to the
same many-register gcc-2.9 allocation ceiling already documented at
length for two other functions in this codebase:

- `DrawPowerDialog` (`src/graphics/oam_count.c`) - the near-identical
  two-icon centering/draw loop this pair's `DrawLanguageSelect` extends to
  six slots, itself parked NAKED for exactly this reason.
- `ShowPowerDialog` (`src/graphics/settings_menu14.c`) - already NAKED,
  and its tail is *this same* `_call_via_r1`/`ReserveObjVram`/
  constant-reuse idiom `InitLanguageSelectGraphics`'s tail uses, hitting the identical
  register-choreography wall around the cached
  `gSmallFont`/`gLargeFont` addresses (there documented
  as needing `r5`/`r8`, with an explicit note that an `asm("r7")` pin
  for one of the parameters produced a genuine correctness bug rather
  than just a mismatch - the project's documented categorical r7-pin
  limitation).

In both of this pass's functions, the compiler kept reaching for
`r8`/`sb`/`sl` in different combinations than the ROM's plain r4-r7
reuse, regardless of how the source was restructured. Given this is
the third and fourth confirmed instance of the exact same ceiling
(after `DrawPowerDialog` and `ShowPowerDialog`), and the second one is
*already* the identical trailing idiom `InitLanguageSelectGraphics` ends with, further
per-call-site register-pin engineering was not attempted this pass -
consistent with how `DrawPowerDialog`/`ShowPowerDialog` were themselves
eventually parked.

## Verification

Every instruction in `src/audio/counter_selector_icons.c`'s two `NAKED`
bodies is transcribed directly from the ROM's own disassembly
(`asm/code_3_2_20a.s`, now removed) and checked against it - the only
edits versus the raw dump are stripping the flag-setting `S` suffix
from `adds`/`subs`/`lsls`/`lsrs`/`movs`/`asrs` (this project's other
`.c`-embedded NAKED bodies use the same bare mnemonics, since these
generated `.s` files don't carry a `.syntax unified` directive). Confirmed
via a full clean `rm -rf build && make NON_MATCHING=1 report` followed
by `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`sha1sum -c checksum.sha1`
prints "La suma coincide").

## Later pass: GAX toolchain retry

`DrawLanguageSelect` now matches as real C (calling the `_call_via_r2` method trampoline directly, glyph assigned inside the first call's arguments). `InitLanguageSelectGraphics` stays NAKED with a draft that matches through the tile-copy loop. See [gax-toolchain-retry.md](./gax-toolchain-retry.md).

## Later pass: late-ROM NAKED retry

`InitLanguageSelectGraphics` now matches as real C under both compilers: the two
icon-manager steps written as `static inline` helpers taking the manager
(the idiom `InitCredits` in actor_part131.c established - each expansion
rematerializes its own `0x108`/`0x12c`/`0x130` offsets), plus one `u32
zero` local shared by the `field_8`/`field_108` stores, which is the 0
the ROM keeps in r8. See [late-rom-naked-retry.md](./late-rom-naked-retry.md).
