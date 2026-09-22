# `0x080372BC` - counter widget icon draw loop / tile-cache init (issue #67)

`sub_80372BC`/`sub_8037388` (`src/audio/counter_selector_icons.c`) are
the small on-screen "counter" widget's (`src/audio/counter_selector.c`,
`sub_80371B4`'s loop) per-frame icon draw pass and its one-time tile-cache
init helper. Both are fully understood - previously left raw
(`docs/status/audio.md`: "hit the same many-register gcc-2.9
allocation difficulty already documented for `sub_8006600`"); this
pass converted them to byte-verified NAKED asm transcriptions, moving
them from "left raw" to "parked".

## What each function does

`sub_80372BC` draws six digit slots (0-5), one per widget value: for
each slot index it either shows the shared "selected" overlay
(`gUnknown_030012DC`, a `struct icon_manager *`, drawn with value 1 or
2 from `sub_8037534` depending on a bit of `self->field_0`, the
widget's free-running frame counter) when the loop index equals
`self->field_8` (the widget's current 0-5 value), or blanks it (value
0 via the same `sub_8028A30` call) otherwise. It then always draws
that slot's own fixed digit glyph - one of six `struct icon_manager *`
looked up from `gStaticData_0817E714[i]` - positioned via
`sub_803AD80` at a fixed X (centered from the rendered pixel width,
the same `(240-w)>>1` idiom `sub_8006600`/`src/graphics/oam_count.c`
uses) and a Y that steps by `0xa` per slot from a `0x32` base, reading
and writing `gUnknown_030012DC->record->slots[0]`/`slots[2]` (see
`include/icon_manager.h`) as its OAM-slot-record pair - the exact same
shape `sub_8006600` uses on the same two globals.

`sub_8037388` resets several OAM-manager globals
(`gUnknown_03001300`/`gUnknown_030012B8`), then hand-fills
`gUnknown_030012B8`'s (`struct tile_asset_cache`, `include/vram_pool.h`)
`slots[0]`-`slots[3]` with 4 fixed 32-byte OBJ tiles copied from
`gStaticData_0817E72C`/`_74C`/`_76C`/`_78C` (two tiles filled per loop
iteration via a doubled offset, 16 iterations), and finally threads
`gUnknown_030012DC`'s/`gUnknown_030012E0`'s `record->slots[6]`
trampoline (`sub_803AD7C`) and VRAM-reserve (`sub_8006C58`) pair -
copying `field_12c` from one `icon_manager` into the other's
`field_108` via the ROM's own "subtract `0x24` from the already-loaded
`0x12c` offset constant" trick rather than a fresh `0x108` literal.

## Why this parked as NAKED instead of matching

Real C was attempted for both functions - an indexed loop and a
pointer-increment loop for the 4-tile copy, caching
`&gUnknown_030012DC`/`&gUnknown_030012E0`/`&gUnknown_030012FC` in a
local versus re-deriving them at each use, and the digit-slot loop's
centering-math rewritten several ways - but every attempt fell to the
same many-register gcc-2.9 allocation ceiling already documented at
length for two other functions in this codebase:

- `sub_8006600` (`src/graphics/oam_count.c`) - the near-identical
  two-icon centering/draw loop this pair's `sub_80372BC` extends to
  six slots, itself parked NAKED for exactly this reason.
- `sub_80062A8` (`src/graphics/settings_menu14.c`) - already NAKED,
  and its tail is *this same* `sub_803AD7C`/`sub_8006C58`/
  constant-reuse idiom `sub_8037388`'s tail uses, hitting the identical
  register-choreography wall around the cached
  `gUnknown_030012DC`/`gUnknown_030012E0` addresses (there documented
  as needing `r5`/`r8`, with an explicit note that an `asm("r7")` pin
  for one of the parameters produced a genuine correctness bug rather
  than just a mismatch - the project's documented categorical r7-pin
  limitation).

In both of this pass's functions, the compiler kept reaching for
`r8`/`sb`/`sl` in different combinations than the ROM's plain r4-r7
reuse, regardless of how the source was restructured. Given this is
the third and fourth confirmed instance of the exact same ceiling
(after `sub_8006600` and `sub_80062A8`), and the second one is
*already* the identical trailing idiom `sub_8037388` ends with, further
per-call-site register-pin engineering was not attempted this pass -
consistent with how `sub_8006600`/`sub_80062A8` were themselves
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
