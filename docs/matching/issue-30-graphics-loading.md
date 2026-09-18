# Issue #30: 0x0801E578-0x0801FA3C - `LoadGraphicsPackage`'s scratch buffer accessors

GitHub issue #30 (`decomp-chunk`, category `graphics_loading`) listed 25
raw functions in `asm/code_3_2_17_188d0.s`, the front of the
`LoadGraphicsPackage` cluster `docs/rom_map.md` already anchored (see
"`graphics_loading` wasn't monolithic either"). This is the write-up for
the work done against that list.

## What this pass covered

`docs/rom_map.md` had already read `LoadGraphicsPackage` itself (a
palette/tiles/tilemap loader) and two of the cluster's other members
(`sub_801E788`'s background-centering math, `sub_801F8DC`'s text-label
constructor), without carrying either through to C. This pass instead
went after the small, self-contained functions immediately after
`LoadGraphicsPackage` - all six turned out to be accessors on the same
0x10-byte scratch buffer `sub_80374D0` (`counter_selector_setup.c`)
already shows a caller building up field-by-field before passing it to
`LoadGraphicsPackage`/`sub_801E640`:

```c
u8 buf[0x10];
sub_801E644(buf, 2, 0x1e, 1, 3);
LoadGraphicsPackage(buf, gStaticData_0816C484);
REG_BG0CNT = sub_801E640(buf);
```

- **`sub_801E640`**: reads back the packed halfword at `buf+0xc`.
- **`sub_801E644`**: a five-argument constructor - `buf+0x00`/`+0x04`/
  `+0x08` get `arg1`/`arg2`/`arg3` verbatim, `buf+0xc`'s low nibble
  packs `(arg5 & 3) | ((arg1 & 3) << 2)`, `buf+0xd` gets `arg2 & 0x1f`.
- **`sub_801E8F8`**: fills one 4bpp VRAM tile (`0x06017800`) with a
  solid color via DMA3, replicating `arg1`'s low nibble into all four
  nibbles of the transferred halfword, and also packs the same rounded-
  down nibble into `buf+0x15`'s low bits plus `buf+0x1c` verbatim.
- **`sub_801E950`**: packs `arg1 & 3` into bits 2-3 of `buf+0x15`.
- **`sub_801E964`**: a second, narrower constructor - `buf+0x00`/
  `+0x04` verbatim.
- **`sub_801E96C`**: clears `buf+0x11`'s bits 4-9 and `buf+0x15`'s
  bit 2 - a "reset before rebuild" pair.

`sub_801E688`/`sub_801E788` (the tile-cell-selection and viewport-
centering helpers `docs/rom_map.md` already read) and the rest of the
chunk - a sound-trigger dispatcher (`sub_801E990`) plus two more large
families (a "trigger effect type N" twin family shaped just like the
already-parked `sub_8020E84`-`sub_802117C` in `trigger_effect.c`, and
the "text label as sprite tiles" family `sub_801F8DC` anchors) - were
not attempted this pass; see "Left raw" below.

## Matched (4 functions, full clean `make compare` passing)

- **`sub_801E640`** (`src/graphics/graphics_package_1e640.c`)
- **`sub_801E8F8`** (`src/graphics/graphics_package_1e8f8.c`) - the DMA3
  tile-fill. Getting this one byte-exact needed two real fixes beyond
  the arithmetic itself: the DAD constant (`0x06017800`) has to be
  loaded into its own local *before* the stack scratch halfword's
  address is computed (matching the ROM's load-early order, not the
  more natural "compute address, then constants" C ordering), and the
  scratch halfword's address has to be taken *twice* - once implicitly
  by the `buf = pattern;` store, once explicitly by `&buf` for the DMA
  register write - rather than cached in one pointer, matching the
  ROM's own two separate `mov rN, sp` computations into different
  registers. The first isolated compile "matched" on inspection but
  actually reordered/CSE'd these away; only the full linked `make
  compare` caught it - see `docs/workflow.md`'s standing warning about
  isolated compiles not being proof.
- **`sub_801E964`**, **`sub_801E96C`**
  (`src/graphics/graphics_package_1e964.c`) - the last function in this
  object needed an explicit trailing `asm(".align 2, 0")` to zero-pad
  the 2-byte gap up to `sub_801E990`'s 4-aligned start, instead of this
  compiler's default Thumb NOP-fill (`0x46C0`) for an implicit end-of-
  object pad - the `matching_decomp_alignment_fix` technique.

**A recurring compiler-codegen gotcha found and fixed across several of
these**: gcc 2.9/agbcc's register allocator frequently *skips* an
apparently-redundant copy instruction the ROM's own compile kept (e.g.
copying a mask constant into a second register before combining it with
a freshly-loaded byte, rather than combining the freshly-loaded byte
directly with the constant's own register). Two matching techniques
fixed this: (1) explicit `register T x asm("rN")` pins forcing specific
hard registers for each intermediate value, following the ROM's own
register choices exactly; (2) an empty `asm("" : "+r"(x))` compiler
barrier immediately after the constant is materialized, which stops
this compiler from folding the "materialize, then copy" pair into a
single "materialize directly into the destination" instruction. Both
are visible in `sub_801E8F8`/`sub_801E96C`'s final C.

## Parked (`NON_MATCHING`) - 2 functions

- **`sub_801E644`** (`src/graphics/graphics_package_1e640.c`, real bytes
  in `asm/code_3_2_17_1e644.s` under `.if NON_MATCHING == 0`): every
  instruction's operation matches the ROM and every technique above was
  tried (explicit local copies for each mask, matching statement order,
  register pins), but this compiler still collapses two of the ROM's
  register copies away, and pushes/pops one extra callee-saved register
  (`r7`) in every phrasing tried that got the copies back. Parked rather
  than keep guessing.
- **`sub_801E950`** (`src/graphics/graphics_package_1e8f8.c`, real bytes
  in `asm/code_3_2_17_1e950.s` under `.if NON_MATCHING == 0`): matches
  in full shape except one instruction - the ROM reloads the `-0xd` mask
  constant fresh (`movs r2,#0xd; rsbs r2,r2,#0`), while this compiler
  always rematerializes it cheaply from the register still holding the
  earlier `#3` constant (`sub r2,r2,#0x10`) instead, regardless of
  literal spelling (`-0xd` vs. `~0xc`) or an `asm("":"+r"(...))` barrier
  placed between the two constant loads.

## Left raw (19 functions, not attempted this pass)

`LoadGraphicsPackage`, `sub_801E688`, `sub_801E788` (all three already
characterized in `docs/rom_map.md`, real bytes now in
`asm/code_3_2_17_188d0.s` and the new `asm/code_3_2_17_1e644.s`) plus
`sub_801E990`, `sub_801EA5C`, `sub_801EB04`, `sub_801EBF0`,
`sub_801EC9C`, `sub_801ED6C`, `sub_801EE3C`, `sub_801EF0C`,
`sub_801F050`, `sub_801F170`, `sub_801F2BC`, `sub_801F3DC`,
`sub_801F528`, `sub_801F680`, `sub_801F7B8`, `sub_801F8DC` (real bytes
now in the new `asm/code_3_2_17_1e990.s`) - not attempted this pass,
left untouched rather than force a low-confidence match. Two families
worth flagging for whoever picks this up next: `sub_801EA5C` through
`sub_801EE3C` share the exact bit-test/`sub_8008434`-spawn shape already
parked as `sub_8020E84`-`sub_802117C` in `trigger_effect.c` (issue #31)
- the same register-rotation gap that resisted parking there is likely
to resist here too; `sub_801EF0C` through `sub_801F8DC` are all "text
label as sprite tiles" constructors sharing `sub_801F8DC`'s already-read
shape (`sub_8009ED0` allocation, `sub_800CA74` style lookup, two
`sub_803AD80` calls).

Verified via a full clean `rm -rf build && make compare` (`La suma
coincide`) and `make NON_MATCHING=1 report`.

## Second pass: `sub_801E644`/`sub_801E950` matched via NAKED transcription

Both functions parked above are now byte-exact matched, confirmed by a
full clean `make compare` ("La suma coincide"). Every instruction's
operation was already confirmed correct against the ROM; the residual
register-allocation gaps (an extra callee-saved `r7` for `sub_801E644`,
a one-instruction-shorter mask rematerialization for `sub_801E950`)
never responded to further plain-C restructuring, so both were
converted to `NAKED` and their ROM disassembly transcribed
instruction-for-instruction - the same escape hatch this project
already established for `sub_8001CB8`/`sub_8001DB4`
(`src/system/link_cable.c`, see
`docs/matching/issue-4-sio-settings-sync.md`'s "The general strategy
for the rest" section).

`asm/code_3_2_17_1e950.s` held nothing but its one guarded function, so
it's now empty and was deleted, with its `ldscript.txt` line dropped.
`asm/code_3_2_17_1e644.s` still holds two other raw functions
(`sub_801E688`/`sub_801E788`, see "Left raw" above) after the removed
guarded block, so only that block's lines were removed from the file -
no split was needed since the guarded function sat at the very start of
the file, not in the middle.

See [docs/status/graphics_loading.md](../status/graphics_loading.md) for
the running matched/parked list.

## Third pass: `LoadGraphicsPackage` itself - parked (`NON_MATCHING`), not matched

`LoadGraphicsPackage` (the cluster's own namesake, `0x0801E578`-`0x0801E640`,
real bytes now guarded at the tail of `asm/code_3_2_17_188d0.s`) is fully
understood: it uses the same 5-field `struct bg_package` descriptor
`LoadBg2Background`/`LoadObjSpriteTiles` (`src/graphics/level_graphics.c`,
issue #65) already established for package loading - moved to a shared
`include/graphics_package.h` header per docs/workflow.md step 7's "check
whether a struct for the same object already exists elsewhere first" rule,
rather than re-declaring it a third time. It loads a palette
(`LoadTaggedAsset` into `0x05000000` + a bank offset from the `self+8`
scratch-buffer field), a tileset (`LoadTaggedAsset` into `0x06000000` + a
char-block offset from `self+0`), and a tilemap into a `sub_8026EC0`
scratch buffer, then remaps the tilemap's per-tile entries - OR-ing in a
palette-bank nibble derived from `self+8` - into `0x06000000` + a
screen-block offset from `self+4`, one fixed 0x40-byte-wide (32-tile) row
at a time regardless of the source package's own (possibly narrower)
`width`. It also flips `self+0xc`'s top bit (the BG control byte's
256-color/16-color mode select) based on whether the palette asset's own
declared size exceeds `0x20`.

This is a large (~110-instruction), register-starved function - the ROM's
own compile uses every one of the 12 available general-purpose registers
simultaneously in the main copy loop, spilling one address to a real stack
slot. Heavy register pinning (`self`->r5, `pkg`->r6, `mapBuf`->r8,
`src`->r7, the packed palette-bank mask->`ip`, `width`->r4, `height`->`sl`,
the row stride->`sb`, the per-row dest pointer->r0, the inner-loop
src/dest/count triple->r2/r1/r3) plus several `asm("":"+r"(...))` barriers
(to force this compiler's "skip an apparently-redundant copy" habit back
into the ROM's own instruction order - the same techniques documented for
`sub_801E644`/`sub_801E8F8` above) closed every gap but one: this function
needs r6 free for one more scratch temp (the loaded tilemap halfword,
right before it's ORed with the palette-bank mask) *inside* the same
window `pkg`'s own r6 binding is technically still in scope for. Pinning
that temp to r6 (matching the ROM's own `ldrh r6,...`) makes gcc's
allocator stop treating `src`'s r7 as needing a callee-save push/pop at
all, even though the function body still writes and later reads it, for
reasons that didn't yield to further restructuring (statement reordering,
a dummy trailing `asm` read of `pkg` to keep r6 "reserved" longer, moving
the pin to an outer scope) - so the choice was between byte-exact bar one
dropped push/pop pair (semantically self-consistent within the function,
but ABI-incorrect towards the caller - a real bug, not a cosmetic
mismatch) or ABI-correct with r6 landing on a different scratch register
than the ROM picked. This is the same first-pass-vs-second-pass
register-pressure artifact category already documented for
`LoadBg2Background` (`src/graphics/level_graphics.c`, issue #65) and
`sub_801E644` above - parked under `NON_MATCHING` (the ABI-correct
variant) rather than force either a broken function or a fake match.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings) and `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` (`La suma
coincide` - the guarded real bytes still assemble unchanged).

`sub_801E688`/`sub_801E788` (the tile-cell-selection and
viewport-centering helpers, real bytes in `asm/code_3_2_17_1e644.s`) were
read again this pass but not attempted: `sub_801E788` in particular
branches into a 4-way switch that writes through `gUnknown_03001300` (the
OAM shadow buffer) at offsets (`+0x12`, `+0x1A`, `+0x22`, `+0x2A` relative
to a packed slot index) that don't fit that struct's already-documented
8-byte hardware-OAM-entry stride cleanly - understanding that side-table
layout correctly is a prerequisite for a confident C reconstruction and
wasn't rushed this pass. Left raw for whoever picks this up next.
