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

## Fourth pass: `sub_801E688`/`sub_801E788` - resolved the flagged OAM layout concern, parked (`NON_MATCHING`), not matched

Picked up the pair the third pass deliberately left alone. Both are now
real, semantically-confident C in the new
`src/graphics/graphics_package_1e688.c` (real bytes still guarded at
`asm/code_3_2_17_1e644.s` under `.if NON_MATCHING == 0`).

**`sub_801E688`** (`self`, `arg1`, `arg2`) is a best-fit box selector: it
stores `arg1`/`arg2` at `self+8`/`self+0xc`, then loops all 12 entries of
the shared `gStaticData_0816C644`/`674` "box preset" table (the same pair
`sub_801E788` indexes, confirmed by `docs/rom_map.md` to be 12
width/height entries), skipping any entry whose dimension is smaller than
half the requested `arg1`/`arg2` and tracking the smallest-area candidate
that still qualifies. The winning index is packed 2+2 bits split across
`self+0x13` bits 6-7 (low 2 bits) and `self+0x11` bits 6-7 (high 2 bits) -
this is the same `self+0x18`-stored index `sub_801E788` reads back and
re-splits when inserting into the affine table below. It also computes a
"tile index" (`0x400 - winningArea/32`, clamped to 10 bits) into
`self+0x14`, and two Q8.8 scale factors (`sub_803ADB4`-divided,
`self+0x20`/`self+0x24`) that classify into a 2-bit "scale mode" written
to `self+0x11` bits 0-1.

**`sub_801E788`** (`self`) computes `self`'s on-screen position from one
of 4 modes packed into `self+0x11` bits 0-1 (mode 2 is a no-op - no
position math), then unconditionally inserts `self`'s pre-built OAM
attribute template (`self+0x10..+0x17`) into the shadow OAM buffer via
`sub_8006AC8`. This resolves the third pass's flagged concern directly:
unless mode was 0 (no centering, which just clears the affine-enable bits
at `self+0x13` bits 4-5), it also allocates one affine-parameter group
from `gUnknown_03001300->field_08` (a separate counter from the normal
insertion counter at offset 0) and writes a pure-scale (no rotation) 2x2
matrix into it from `sub_801E688`'s `self+0x20`/`self+0x24` factors. The
four writes land at `field_08 * 0x20 + 0x12` and 8/16/24 bytes past it -
which looked like it didn't fit the shadow buffer's documented 8-byte
hardware-OAM-entry stride, but does: `field_08 * 0x20 + 0x12` is exactly
`(field_08 * 4 + 0) * 8 + 6`, i.e. the *filler* halfword (byte offset +6)
of the shadow entry at index `field_08 * 4`, and the other three writes
are the same field on the next three consecutive entries. Real GBA
hardware overlays the OBJ affine-parameter memory (PA/PB/PC/PD, one
`s16` each) on exactly that halfword of every 4th OAM entry - the four
writes here (scaleX, 0, 0, scaleY) are a diagonal, no-rotation affine
matrix, and `self+0x13` bits 1-5 (the shadow buffer's `field_08` value,
packed alongside the position bits `self+0x12`'s high byte already
holds) become that affine group's 5-bit selector index in the sprite's
own `attr1` field. A coherent, understood mechanism - not a layout
mismatch, just a side-table stride that isn't the same as the
already-documented plain-insertion one.

**Parked (`NON_MATCHING`), not matched.** `sub_801E688` is a
~110-instruction, register-starved search loop in the same category as
`LoadGraphicsPackage`/`sub_801E644` above (every general-purpose register
committed simultaneously) - not attempted byte-exact this pass beyond
confirming the C's semantics compile cleanly. `sub_801E788` got much
closer (heavy iteration: the negative-constant clear-mask idiom for all
three `self+0x13` bit-pack masks including the ROM's own `subs r2,#0x10`
delta-derivation of the third mask from the second, a
`struct oam_shadow_buffer **addr = &gUnknown_03001300` address cache
matching `graphics_loading_21d80.c`'s established pattern so the final
`sub_8006AC8` call's address load is shared across both branches like the
ROM's own `r6` reuse, and an explicit register pin for the loop-scoped
`slot`/`field_08` value) but hit one gap that resisted every technique
tried: the ROM keeps `self` in `r7` for the whole function, matching its
4-register `push {r4-r7}` list, and reaching that register is only
possible through an explicit `register u8 *self asm("r7")` pin - but
pinning `self` this way makes this compiler stop folding
`self[constant offset]` into a single `ldrb/ldrh/ldr rX,[r7,#imm]`
instruction, emitting a separate `add rX, rX, #imm` plus a zero-offset
dereference instead, for *every* access to `self`, not just the ones
near the register-pressure edge. Confirmed with a minimal one-line
repro (`register u8 *self asm("r7") = selfArg; return self[0x11];`
still expands to `add r7,r0,#0; add r0,r0,#0x11; ldrb r0,[r0]` rather
than `ldrb r0,[r7,#0x11]`) - a genuine, reproducible gcc-2.9/agbcc
limitation for asm-register-pinned pointer locals, not something any
C-level restructuring tried routes around. This is the same
first-pass-vs-second-pass register-pressure artifact category as
`LoadGraphicsPackage`/`sub_801E644`/`LoadBg2Background` elsewhere in
this cluster, just manifesting through address-mode folding instead of
a dropped push/pop pair - worth recording as a new flavor of the gotcha
for whoever hits it next.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings for the new file) and `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`La suma coincide` - the guarded real bytes still assemble
unchanged).

`sub_801E990` (the sound-trigger dispatcher docs/rom_map.md already
partially read - 3 Q8.8-shifted x/y/z args, a `gUnknown_030012C0`-gated
position/flag write into `gUnknown_030012D8+0x28`, then a conditional
`PlaySfx`) was read in full this pass too, but not attempted: its second
half calls into several still-unread helpers
(`sub_80232E0`/`sub_8023130`/`sub_803AFEC`/`sub_80232B8`/`sub_803AD88`)
whose own signatures and the `gUnknown_030012D8+0x18+0x68`-rooted
sub-struct they read from aren't pinned down yet - a confident
reconstruction would mean chasing all of those first, which this pass's
remaining time didn't cover. Left raw, along with the rest of this
issue's "sound-trigger dispatch plus the trigger-effect/text-popup-
spawner families" remainder (`sub_801EA5C` onward, unchanged from the
third pass's characterization), for whoever picks this up next.

## Fifth pass: `sub_801E990` - matched semantics, parked (NAKED transcription)

Picked up `sub_801E990` (the sound-trigger dispatcher the third pass
flagged its unresolved helper calls for). All five previously-unread
helpers turned out to already be matched elsewhere in the tree as
plain one-line field accessors on the same `gUnknown_030012C0`-rooted
player struct: `sub_80232F4`/`sub_80232E0`/`sub_8023130`/`sub_803AFEC`
(`+0xa8`/`+0x7c`/`+0x84`/`+0x74` respectively - the first three in
`asm/code_3_2_17_231cc.s`'s still-raw accessor cluster, the fourth
already matched in `actor_aabb_setup.c`) and `sub_80232B8` (`+0xa4`,
matched in `game_loop10.c`/`game_loop2.c`). `sub_803AD88` itself is not
a normal function at all - it's the `bx r4` register-trampoline from
`reg_trampolines.c` (`src/system/reg_trampolines.c`'s
`sub_803AD78`-`sub_803AD94` "call through register" family) - the ROM
loads the real callee's address into `r4` right before the `bl`, and
this project's established convention (`sub_8009FD4` in
`actor_part9.c`, `sub_8007DBC`) is to model that load as a genuine
"dead read" (`register void *x asm("r4") = ...; (void)x;`) immediately
before an ordinary-looking `sub_803AD88(addr, arg1, arg2, arg3)` call.

With every operand pinned down, the function's full semantics are:

1. If the player's `+0xa8` flag (`sub_80232F4`) is set: looks up a
   per-`z` flags byte via the `gUnknown_030012B4 -> *rec -> {+8
   offsets[], +0xc base}` table - the exact same table
   `sub_8021D04` (`graphics_loading_21bfc.c`, issue #33) already reads,
   indexed the same way (`offsets[z]`, then `base[offsets[z]]`) - folds
   bit 1 of that byte into the player's `+0x28` bitfield's bit 4, then
   unconditionally writes the incoming `x`/`y` args (shifted to Q8.8)
   into the player's own `x`/`y` fields, the same unconditional write
   `sub_80221A4`/`sub_80221D4` (`graphics_loading_21d80.c`) already do
   elsewhere in this cluster.
2. Unless the player's `+0x8c` "paused" flag is set: fires the
   player's `table+0x68` trampoline (`sub_803AD88`, action `0x1a`) and
   plays SFX `0x100` through `gUnknown_030012BC`, gated by a
   budget/reentrancy check - either the player's spawn counter
   (`+0x7c`) has room against its cap (`+0x84`), or, when it doesn't,
   `sub_803AFEC` (`+0x74`), `sub_80232B8` (`+0xa4`) and the player's
   `+0x78` mode field all agree it's still safe to fire.

A plain-C reconstruction with this exact meaning compiles cleanly and
was confirmed instruction-for-instruction correct against the ROM in
isolation for every operation, field offset and call - except one
section: the `gUnknown_030012B4` table-resolution plus `+0x28`
bitfield-pack block never converged on the ROM's own register choices
(`byte` staying in r0 across the shift, the shifted bit landing in r2,
the `-0x11` clear mask materializing via a `movs r0,#1`/`subs
r0,#0x12` derivation that reuses the register still holding an earlier
`1` rather than a fresh literal), no matter the statement order,
explicit intermediate variables, or the negative-constant idiom used
elsewhere in this project - every restructuring tried shuffled the
register assignment without landing on the ROM's exact one. This is
the identical `gUnknown_030012B4 -> *rec -> {+8, +0xc}` resolution
shape already documented as unmatchable via plain C for `sub_8021D04`
(issue #33) for the same underlying reason, strongly suggesting this
specific table-lookup-into-bitfield-pack shape is a recurring gcc-2.9
register-allocation dead end for this codebase rather than something
this pass's C phrasing missed.

Converted to `NAKED` and transcribed instruction-for-instruction from
the ROM disassembly instead - confirmed byte-identical against the ROM
bytes (compared via `arm-none-eabi-objdump` on both the isolated
compile and the raw ROM disassembly reassembled standalone) before
integrating. Cut out of `asm/code_3_2_17_1e990.s` (was the first
function in that file) into the new `src/graphics/graphics_loading_1e990.c`,
with `ldscript.txt` updated to place the new object immediately before
the now-trimmed raw file (which starts at `sub_801EA5C` instead).

**The rest of this issue's raw region** (`sub_801EA5C` through
`sub_801FCB4`, ending at the already-matched `sub_801FDEC`) splits into
two families, both worth flagging precisely for whoever picks this up
next:

- **`sub_801EA5C`-`sub_801EE3C`** (5 functions): the same "trigger
  effect type N" bit-test (`sub_8023404`)/`sub_8008434`-spawn shape
  already parked as `NAKED` in `trigger_effect.c`
  (`sub_8020E84`-`sub_802117C`, issue #31/#33) - the same register-
  rotation gap that resisted plain C there is likely to resist here
  too, so NAKED transcription is the expected outcome, not another
  fresh matching attempt.
- **`sub_801EF0C`-`sub_801F8DC`** (through `sub_801FA3C`/`sub_801FB74`/
  `sub_801FCB4`, ~10 functions): further instances of the "text label
  as sprite tiles" spawner family whose shape `sub_801FDEC`
  (`graphics_loading_1fdec.c`, issue #31) already matched as **real,
  byte-exact C** - `sub_8009ED0` allocation, `sub_800CA74` style
  lookup, two `sub_803AD80` trampoline calls, and the same
  `gUnknown_030012B4`-rooted "collected bits" pack this pass's
  `sub_801E990` write-up above also resolves the table shape for. This
  is the more promising real-C target of the two remaining families -
  `sub_801FDEC`'s own matched C is the template to start from.

Left raw for whoever picks this up next; `report_units.py`'s entry for
this address range now points at `sub_801EA5C` (the new start of
`asm/code_3_2_17_1e990.s`) instead of `sub_801E990` and calls out both
families explicitly.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings for the new file) and `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`La suma coincide`).

## Sixth pass: `sub_801E950` matched as real C

The second pass above (`## Second pass`) had converted `sub_801E950`
to a `NAKED` transcription on the theory that the ROM rematerializes
its `-0xd` mask via `sub r2,r2,#0x10` reusing the register that still
held the earlier `#3` constant - checking the real target object
(`build/expected/units/raw_0801E950_target.o`) shows this theory was
wrong: the ROM's actual bytes are a plain `movs r2,#0xd` / `rsbs
r2,r2,#0` (i.e. `neg`), a completely fresh reload, not a reuse. The
straightforward plain-C phrasing (two separate register-pinned masks,
the second one materialized via the same `mov #N; neg` opaque-asm
negative-mask idiom `UPDATE_ICON_FRAME_NIBBLE` already uses) matches
100% on the first isolated-compile attempt. `tools/report_units.py`'s
entry for `0x0801E950` now points at
`src/graphics/graphics_package_1e8f8.o`. Verified via `rm -rf build &&
make NON_MATCHING=1 report` + `objdiff-cli diff` (100%) and a full
clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`).
