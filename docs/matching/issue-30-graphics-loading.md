# Issue #30: 0x0801E578-0x0801FA3C - `LoadGraphicsPackage`'s scratch buffer accessors

> **Superseded for the NAKED functions below:** this region was built
> with old_agbcc. `LoadGraphicsPackage`, `InitBgSetup`, `FitScaledSprite`
> and `DrawScaledSprite` are now plain C - see [issue-30-old-agbcc.md](issue-30-old-agbcc.md).

GitHub issue #30 (`decomp-chunk`, category `graphics_loading`) listed 25
raw functions in `asm/code_3_2_17_188d0.s`, the front of the
`LoadGraphicsPackage` cluster `docs/rom_map.md` already anchored (see
"`graphics_loading` wasn't monolithic either"). This is the write-up for
the work done against that list.

## What this pass covered

`docs/rom_map.md` had already read `LoadGraphicsPackage` itself (a
palette/tiles/tilemap loader) and two of the cluster's other members
(`DrawScaledSprite`'s background-centering math, `SpawnPufferfish`'s text-label
constructor), without carrying either through to C. This pass instead
went after the small, self-contained functions immediately after
`LoadGraphicsPackage` - all six turned out to be accessors on the same
0x10-byte scratch buffer `LoadLanguageSelectBg` (`counter_selector_setup.c`)
already shows a caller building up field-by-field before passing it to
`LoadGraphicsPackage`/`GetBgSetupControl`:

```c
u8 buf[0x10];
InitBgSetup(buf, 2, 0x1e, 1, 3);
LoadGraphicsPackage(buf, gMenuSkyBg);
REG_BG0CNT = GetBgSetupControl(buf);
```

- **`GetBgSetupControl`**: reads back the packed halfword at `buf+0xc`.
- **`InitBgSetup`**: a five-argument constructor - `buf+0x00`/`+0x04`/
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

`FitScaledSprite`/`DrawScaledSprite` (the tile-cell-selection and viewport-
centering helpers `docs/rom_map.md` already read) and the rest of the
chunk - a sound-trigger dispatcher (`SpawnStartMarker`) plus two more large
families (a "trigger effect type N" twin family shaped just like the
already-parked `SpawnRedGemPlatform`-`SpawnBlueGemPlatform` in `trigger_effect.c`, and
the "text label as sprite tiles" family `SpawnPufferfish` anchors) - were
not attempted this pass; see "Left raw" below.

## Matched (4 functions, full clean `make compare` passing)

- **`GetBgSetupControl`** (`src/graphics/graphics_package_1e640.c`)
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
  the 2-byte gap up to `SpawnStartMarker`'s 4-aligned start, instead of this
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

- **`InitBgSetup`** (`src/graphics/graphics_package_1e640.c`, real bytes
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

`LoadGraphicsPackage`, `FitScaledSprite`, `DrawScaledSprite` (all three already
characterized in `docs/rom_map.md`, real bytes now in
`asm/code_3_2_17_188d0.s` and the new `asm/code_3_2_17_1e644.s`) plus
`SpawnStartMarker`, `SpawnCrystal`, `SpawnCrateGem`, `SpawnGemPathGem`,
`SpawnRedGem`, `SpawnGreenGem`, `SpawnYellowGem`, `SpawnLizard`,
`SpawnVulture`, `SpawnVenusFlytrap`, `sub_801F2BC`, `SpawnBlowgunTribesman`,
`SpawnPenguin`, `SpawnSeal`, `SpawnPolarBear`, `SpawnPufferfish` (real bytes
now in the new `asm/code_3_2_17_1e990.s`) - not attempted this pass,
left untouched rather than force a low-confidence match. Two families
worth flagging for whoever picks this up next: `SpawnCrystal` through
`SpawnYellowGem` share the exact bit-test/`CreateSpriteObj`-spawn shape already
parked as `SpawnRedGemPlatform`-`SpawnBlueGemPlatform` in `trigger_effect.c` (issue #31)
- the same register-rotation gap that resisted parking there is likely
to resist here too; `SpawnLizard` through `SpawnPufferfish` are all "text
label as sprite tiles" constructors sharing `SpawnPufferfish`'s already-read
shape (`CreateMovingSprite` allocation, `CreateEnemyCtrl` style lookup, two
`_call_via_r2` calls).

Verified via a full clean `rm -rf build && make compare` (`La suma
coincide`) and `make NON_MATCHING=1 report`.

## Second pass: `InitBgSetup`/`sub_801E950` matched via NAKED transcription

Both functions parked above are now byte-exact matched, confirmed by a
full clean `make compare` ("La suma coincide"). Every instruction's
operation was already confirmed correct against the ROM; the residual
register-allocation gaps (an extra callee-saved `r7` for `InitBgSetup`,
a one-instruction-shorter mask rematerialization for `sub_801E950`)
never responded to further plain-C restructuring, so both were
converted to `NAKED` and their ROM disassembly transcribed
instruction-for-instruction - the same escape hatch this project
already established for `MakeLinkHandshakeId`/`ResetLinkSessionState`
(`src/system/link_cable.c`, see
`docs/matching/issue-4-sio-settings-sync.md`'s "The general strategy
for the rest" section).

`asm/code_3_2_17_1e950.s` held nothing but its one guarded function, so
it's now empty and was deleted, with its `ldscript.txt` line dropped.
`asm/code_3_2_17_1e644.s` still holds two other raw functions
(`FitScaledSprite`/`DrawScaledSprite`, see "Left raw" above) after the removed
guarded block, so only that block's lines were removed from the file -
no split was needed since the guarded function sat at the very start of
the file, not in the middle.

See [docs/status/graphics_loading.md](../status/graphics_loading.md) for
the running matched/parked list.

## Third pass: `LoadGraphicsPackage` itself - parked (`NON_MATCHING`), not matched

`LoadGraphicsPackage` (the cluster's own namesake, `0x0801E578`-`0x0801E640`,
real bytes now guarded at the tail of `asm/code_3_2_17_188d0.s`) is fully
understood: it uses the same 5-field `struct bg_package` descriptor
`LoadTitleScreenBg`/`LoadTitleScreenObjTiles` (`src/graphics/level_graphics.c`,
issue #65) already established for package loading - moved to a shared
`include/graphics_package.h` header per docs/workflow.md step 7's "check
whether a struct for the same object already exists elsewhere first" rule,
rather than re-declaring it a third time. It loads a palette
(`LoadTaggedAsset` into `0x05000000` + a bank offset from the `self+8`
scratch-buffer field), a tileset (`LoadTaggedAsset` into `0x06000000` + a
char-block offset from `self+0`), and a tilemap into a `OperatorNewArray`
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
`InitBgSetup`/`sub_801E8F8` above) closed every gap but one: this function
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
`LoadTitleScreenBg` (`src/graphics/level_graphics.c`, issue #65) and
`InitBgSetup` above - parked under `NON_MATCHING` (the ABI-correct
variant) rather than force either a broken function or a fake match.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings) and `rm -rf build crashbandicootxs.elf
crashbandicootxs.gba crashbandicootxs.map && make compare` (`La suma
coincide` - the guarded real bytes still assemble unchanged).

`FitScaledSprite`/`DrawScaledSprite` (the tile-cell-selection and
viewport-centering helpers, real bytes in `asm/code_3_2_17_1e644.s`) were
read again this pass but not attempted: `DrawScaledSprite` in particular
branches into a 4-way switch that writes through `gOamBuffer` (the
OAM shadow buffer) at offsets (`+0x12`, `+0x1A`, `+0x22`, `+0x2A` relative
to a packed slot index) that don't fit that struct's already-documented
8-byte hardware-OAM-entry stride cleanly - understanding that side-table
layout correctly is a prerequisite for a confident C reconstruction and
wasn't rushed this pass. Left raw for whoever picks this up next.

## Fourth pass: `FitScaledSprite`/`DrawScaledSprite` - resolved the flagged OAM layout concern, parked (`NON_MATCHING`), not matched

Picked up the pair the third pass deliberately left alone. Both are now
real, semantically-confident C in the new
`src/graphics/graphics_package_1e688.c` (real bytes still guarded at
`asm/code_3_2_17_1e644.s` under `.if NON_MATCHING == 0`).

**`FitScaledSprite`** (`self`, `arg1`, `arg2`) is a best-fit box selector: it
stores `arg1`/`arg2` at `self+8`/`self+0xc`, then loops all 12 entries of
the shared `gObjSizeWidths`/`674` "box preset" table (the same pair
`DrawScaledSprite` indexes, confirmed by `docs/rom_map.md` to be 12
width/height entries), skipping any entry whose dimension is smaller than
half the requested `arg1`/`arg2` and tracking the smallest-area candidate
that still qualifies. The winning index is packed 2+2 bits split across
`self+0x13` bits 6-7 (low 2 bits) and `self+0x11` bits 6-7 (high 2 bits) -
this is the same `self+0x18`-stored index `DrawScaledSprite` reads back and
re-splits when inserting into the affine table below. It also computes a
"tile index" (`0x400 - winningArea/32`, clamped to 10 bits) into
`self+0x14`, and two Q8.8 scale factors (`__divsi3`-divided,
`self+0x20`/`self+0x24`) that classify into a 2-bit "scale mode" written
to `self+0x11` bits 0-1.

**`DrawScaledSprite`** (`self`) computes `self`'s on-screen position from one
of 4 modes packed into `self+0x11` bits 0-1 (mode 2 is a no-op - no
position math), then unconditionally inserts `self`'s pre-built OAM
attribute template (`self+0x10..+0x17`) into the shadow OAM buffer via
`AddOamEntry`. This resolves the third pass's flagged concern directly:
unless mode was 0 (no centering, which just clears the affine-enable bits
at `self+0x13` bits 4-5), it also allocates one affine-parameter group
from `gOamBuffer->field_08` (a separate counter from the normal
insertion counter at offset 0) and writes a pure-scale (no rotation) 2x2
matrix into it from `FitScaledSprite`'s `self+0x20`/`self+0x24` factors. The
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

**Parked (`NON_MATCHING`), not matched.** `FitScaledSprite` is a
~110-instruction, register-starved search loop in the same category as
`LoadGraphicsPackage`/`InitBgSetup` above (every general-purpose register
committed simultaneously) - not attempted byte-exact this pass beyond
confirming the C's semantics compile cleanly. `DrawScaledSprite` got much
closer (heavy iteration: the negative-constant clear-mask idiom for all
three `self+0x13` bit-pack masks including the ROM's own `subs r2,#0x10`
delta-derivation of the third mask from the second, a
`struct oam_shadow_buffer **addr = &gOamBuffer` address cache
matching `graphics_loading_21d80.c`'s established pattern so the final
`AddOamEntry` call's address load is shared across both branches like the
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
`LoadGraphicsPackage`/`InitBgSetup`/`LoadTitleScreenBg` elsewhere in
this cluster, just manifesting through address-mode folding instead of
a dropped push/pop pair - worth recording as a new flavor of the gotcha
for whoever hits it next.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings for the new file) and `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`La suma coincide` - the guarded real bytes still assemble
unchanged).

`SpawnStartMarker` (the sound-trigger dispatcher docs/rom_map.md already
partially read - 3 Q8.8-shifted x/y/z args, a `gLevelState`-gated
position/flag write into `gPlayer+0x28`, then a conditional
`PlaySfx`) was read in full this pass too, but not attempted: its second
half calls into several still-unread helpers
(`GetDeaths`/`GetMaskAssistDeaths`/`GetLives`/`IsInBonusRound`/`_call_via_r4`)
whose own signatures and the `gPlayer+0x18+0x68`-rooted
sub-struct they read from aren't pinned down yet - a confident
reconstruction would mean chasing all of those first, which this pass's
remaining time didn't cover. Left raw, along with the rest of this
issue's "sound-trigger dispatch plus the trigger-effect/text-popup-
spawner families" remainder (`SpawnCrystal` onward, unchanged from the
third pass's characterization), for whoever picks this up next.

## Fifth pass: `SpawnStartMarker` - matched semantics, parked (NAKED transcription)

Picked up `SpawnStartMarker` (the sound-trigger dispatcher the third pass
flagged its unresolved helper calls for). All five previously-unread
helpers turned out to already be matched elsewhere in the tree as
plain one-line field accessors on the same `gLevelState`-rooted
player struct: `GetSpawnAtStart`/`GetDeaths`/`GetMaskAssistDeaths`/`GetLives`
(`+0xa8`/`+0x7c`/`+0x84`/`+0x74` respectively - the first three in
`asm/code_3_2_17_231cc.s`'s still-raw accessor cluster, the fourth
already matched in `actor_aabb_setup.c`) and `IsInBonusRound` (`+0xa4`,
matched in `game_loop10.c`/`game_loop2.c`). `_call_via_r4` itself is not
a normal function at all - it's the `bx r4` register-trampoline from
`reg_trampolines.c` (`src/system/reg_trampolines.c`'s
`_call_via_r0`-`_call_via_r7` "call through register" family) - the ROM
loads the real callee's address into `r4` right before the `bl`, and
this project's established convention (`HitMovingSprite` in
`actor_part9.c`, `CheckSpritePickup`) is to model that load as a genuine
"dead read" (`register void *x asm("r4") = ...; (void)x;`) immediately
before an ordinary-looking `_call_via_r4(addr, arg1, arg2, arg3)` call.

With every operand pinned down, the function's full semantics are:

1. If the player's `+0xa8` flag (`GetSpawnAtStart`) is set: looks up a
   per-`z` flags byte via the `gEntityFlags -> *rec -> {+8
   offsets[], +0xc base}` table - the exact same table
   `SpawnBasicCrate` (`graphics_loading_21bfc.c`, issue #33) already reads,
   indexed the same way (`offsets[z]`, then `base[offsets[z]]`) - folds
   bit 1 of that byte into the player's `+0x28` bitfield's bit 4, then
   unconditionally writes the incoming `x`/`y` args (shifted to Q8.8)
   into the player's own `x`/`y` fields, the same unconditional write
   `sub_80221A4`/`sub_80221D4` (`graphics_loading_21d80.c`) already do
   elsewhere in this cluster.
2. Unless the player's `+0x8c` "paused" flag is set: fires the
   player's `table+0x68` trampoline (`_call_via_r4`, action `0x1a`) and
   plays SFX `0x100` through `gAudioContext`, gated by a
   budget/reentrancy check - either the player's spawn counter
   (`+0x7c`) has room against its cap (`+0x84`), or, when it doesn't,
   `GetLives` (`+0x74`), `IsInBonusRound` (`+0xa4`) and the player's
   `+0x78` mode field all agree it's still safe to fire.

A plain-C reconstruction with this exact meaning compiles cleanly and
was confirmed instruction-for-instruction correct against the ROM in
isolation for every operation, field offset and call - except one
section: the `gEntityFlags` table-resolution plus `+0x28`
bitfield-pack block never converged on the ROM's own register choices
(`byte` staying in r0 across the shift, the shifted bit landing in r2,
the `-0x11` clear mask materializing via a `movs r0,#1`/`subs
r0,#0x12` derivation that reuses the register still holding an earlier
`1` rather than a fresh literal), no matter the statement order,
explicit intermediate variables, or the negative-constant idiom used
elsewhere in this project - every restructuring tried shuffled the
register assignment without landing on the ROM's exact one. This is
the identical `gEntityFlags -> *rec -> {+8, +0xc}` resolution
shape already documented as unmatchable via plain C for `SpawnBasicCrate`
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
the now-trimmed raw file (which starts at `SpawnCrystal` instead).

**Update (later session):** the `+0x28` bitfield-pack block's
remaining register-choice gap closed - see
[naked-sub_801e990-matched.md](./naked-sub_801e990-matched.md) for the
fix (modeling the r3-pinned local as the *address of*
`gPlayer` rather than its dereferenced value). `SpawnStartMarker`
is now real, fully matched C; the `NAKED` wrapper and `#if
NON_MATCHING` toggle described above have been removed from
`graphics_loading_1e990.c`.

**The rest of this issue's raw region** (`SpawnCrystal` through
`SpawnElectricEel`, ending at the already-matched `SpawnSquid`) splits into
two families, both worth flagging precisely for whoever picks this up
next:

- **`SpawnCrystal`-`SpawnYellowGem`** (5 functions): the same "trigger
  effect type N" bit-test (`GetCurrentLevelFlags`)/`CreateSpriteObj`-spawn shape
  already parked as `NAKED` in `trigger_effect.c`
  (`SpawnRedGemPlatform`-`SpawnBlueGemPlatform`, issue #31/#33) - the same register-
  rotation gap that resisted plain C there is likely to resist here
  too, so NAKED transcription is the expected outcome, not another
  fresh matching attempt.
- **`SpawnLizard`-`SpawnPufferfish`** (through `SpawnShark`/`SpawnMorayEel`/
  `SpawnElectricEel`, ~10 functions): further instances of the "text label
  as sprite tiles" spawner family whose shape `SpawnSquid`
  (`graphics_loading_1fdec.c`, issue #31) already matched as **real,
  byte-exact C** - `CreateMovingSprite` allocation, `CreateEnemyCtrl` style
  lookup, two `_call_via_r2` trampoline calls, and the same
  `gEntityFlags`-rooted "collected bits" pack this pass's
  `SpawnStartMarker` write-up above also resolves the table shape for. This
  is the more promising real-C target of the two remaining families -
  `SpawnSquid`'s own matched C is the template to start from.

Left raw for whoever picks this up next; `report_units.py`'s entry for
this address range now points at `SpawnCrystal` (the new start of
`asm/code_3_2_17_1e990.s`) instead of `SpawnStartMarker` and calls out both
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

## Seventh pass: `FitScaledSprite` - NAKED transcription

Picked up `FitScaledSprite` (the tile-cell-selection helper the fourth pass
left as "fully understood, not yet byte-exact"). Its plain-C
reconstruction was already known-correct instruction-for-instruction;
this pass tried to close the byte-exact gap with the same techniques
that worked for `AllocVramTileBlock` (issue #47): one continuous
`asm volatile` island covering the entire ~110-instruction body,
register-pinning only `self`/`arg1`/`arg2` as plain `r0`/`r1`/`r2`
inputs and leaving every other register (`r3`-`r8`, `sb`, `sl`, `ip`)
as bare clobbers, with the loop's back-edge and the mid-function
literal-pool split (`.pool`, matching the ROM's own
`gObjSizeWidths`/`gObjSizeHeights`/`0x3FF`/`0xFFFFFC00`
pool placement right after the "mode 3" early-out branch) handled the
same way as `AllocVramTileBlock`'s own island.

This got remarkably close on the very first isolated-compile attempt:
every single instruction in the function *body* came out byte-
identical to the ROM (confirmed via a direct `arm-none-eabi-objcopy
--only-section=.text` + `cmp` comparison against
`asm/code_3_2_17_1e644.s`'s bytes for this function's address range) -
but the compiler-synthesized prologue/epilogue didn't: the ROM's own
`push {r4,r5,r6,r7,lr}` / `push {r5,r6,r7}` (saving `r7` alongside
`r4`-`r6`/`r8`/`sb`/`sl`) came out as `push {r4,r5,r6,lr}` /
`push {r4,r5,r6}` - `r7` silently missing from *both* push lists (and
the mirrored pop lists), even though the asm body plainly uses `r7`
throughout (`arg1`, later reused for `scaleX`). This is the identical
symptom `DrawScaledSprite` below already documented at length, just reached
by a different route: that writeup found pinning `self` to
`register u8 *self asm("r7")` breaks address-mode folding; this pass
found that `r7` never enters the callee-save push/pop list *at all*
via inline asm, regardless of whether it's referenced as a bare
clobber string or via a dummy `register s32 r7dummy asm("r7")` output
operand (tried both, both reproduced the exact same missing-`r7`
prologue/epilogue with an otherwise-perfect body) - confirming this is
a genuine, reproducible gcc-2.9/agbcc limit on this specific register
for prologue-list inclusion, not something reachable through more
inline-asm phrasing.

Converted to `NAKED` and transcribed instruction-for-instruction from
the ROM disassembly instead - trivial once the asm-island version's
body text was already confirmed byte-identical, since the NAKED
version reuses the exact same instruction sequence, just with a
hand-written prologue/epilogue (matching the ROM's own
`push`/`mov`-dance/`push` and `pop`/`mov`-dance/`pop`/`bx` shape used
elsewhere in this cluster) instead of relying on the compiler to
synthesize one. Also needed a trailing `asm(".align 2, 0")` after the
function (the `matching_decomp_alignment_fix` precedent - the
function's real instruction stream is 254 bytes, 2 short of the next
4-byte boundary, and this compiler's default NOP-fill pad doesn't
match the ROM's zero-fill).

Cut `FitScaledSprite`'s block out of `asm/code_3_2_17_1e644.s` (it sat at
the very start of the file, so - like the second pass's
`InitBgSetup` cut - no mid-file split was needed, just dropping the
leading block and re-opening the `.if NON_MATCHING == 0` guard right
before `DrawScaledSprite`, which is now the file's only function).
`ldscript.txt`'s two entries for this pair had to swap order:
`FitScaledSprite` is now a real (always-compiled) object in
`graphics_package_1e688.o`, so it must link *before*
`code_3_2_17_1e644.o` (which now holds only `DrawScaledSprite`) to land at
its correct, lower ROM address - the opposite of the pre-existing
order, which had the asm file first back when it supplied both
functions' bytes. `tools/report_units.py`'s single combined entry for
this pair was split into two: `(0x0801E688, None, ...)` (matching the
`InitBgSetup`/`SpawnStartMarker` precedent - a NAKED transcription doesn't
count as "matched" for this project's per-file tracking, even though
it's byte-correct) and `(0x0801E788, "src/graphics/graphics_package_1e688.o", ...)`
(unchanged treatment, still pointing at the `.c` file's `#if
NON_MATCHING` reconstruction for its NON_MATCHING=1 diffable
percentage, the same convention `LoadGraphicsPackage` above uses).

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings for the new file) + `objdiff-cli report
generate` (`DrawScaledSprite`'s own entry unaffected, still ~41% fuzzy;
`FitScaledSprite` correctly excluded from the diffable-percentage report,
same as `InitBgSetup`) and a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`La suma coincide`).

## Eighth pass: `LoadGraphicsPackage` itself - NAKED transcription

Picked up the cluster's own namesake, left parked (`NON_MATCHING`) by
the third pass above with a precisely-characterized single gap: every
operation and register choice matched the ROM after heavy pinning
(`self`->r5, `pkg`->r6, `mapBuf`->r8, `src`->r7, the packed
palette-bank mask->ip, `width`->r4, `height`->sl, the row stride->sb,
the per-row dest pointer->r0, the inner-loop src/dest/count triple->
r2/r1/r3), except that reusing r6 for one more scratch temp (the loaded
tilemap halfword, right before it's ORed with the palette-bank mask -
matching the ROM's own `ldrh r6,...`) made this compiler's allocator
stop treating `src`'s r7 as needing a callee-save push/pop at all, even
though the function body still writes and later reads it through the
whole outer loop. This is the exact same "compiler drops a genuinely
live register from its own auto-generated prologue/epilogue list under
register pressure" limitation already closed this session for
`SetupRoomBlend` (`src/system/game_loop8.c`, PR #334) and `FitScaledSprite`
(`src/graphics/graphics_package_1e688.c`, PR #336, "Seventh pass"
above), and documented as still-open for `LoadTitleScreenBg`
(`src/graphics/level_graphics.c`, issue #65) - given the extensive
prior iteration already recorded in the third pass's write-up (every
plausible C-level restructuring already tried and exhausted), this pass
skipped straight to the NAKED escape hatch rather than re-attempt
plain-C phrasings.

The existing register-pinned C reconstruction's instruction content was
already fully correct per the third pass, so the ROM disassembly
(`asm/code_3_2_17_188d0.s`'s guarded tail, 94 Thumb instructions/188
bytes) was transcribed directly into a `NAKED void LoadGraphicsPackage`
function, reusing the exact hand-written `push`/`mov`-dance/`push`/
`sub sp` prologue and `add sp`/`pop`/`mov`-dance/`pop`/`pop`/`bx`
epilogue shape already established for `FitScaledSprite`/`SpawnStartMarker` in
this cluster. Verified via an isolated `cpp`+`agbcc` compile,
`arm-none-eabi-as` assemble, and a direct `arm-none-eabi-objcopy
--only-section=.text` byte comparison against the ROM bytes extracted
from `baserom.gba` at `0x0801E578`: every halfword matched except the
5 `bl` call sites (`LoadTaggedAsset` x3, `OperatorNewArray`, `OperatorDeleteArray`),
which differ only because the isolated object is unlinked - the exact
expected relocation-placeholder pattern, not a real mismatch.

Cut the guarded `LoadGraphicsPackage` block out of the tail of
`asm/code_3_2_17_188d0.s` entirely (it was the last thing in the file,
so this was a pure truncation, no mid-file split needed) - the file now
ends at `DestroyLevelSelectCursor`'s trailing literal pool. No `ldscript.txt` changes
were needed: `graphics_package_1e578.o` already linked immediately
after `code_3_2_17_188d0.o` and before `graphics_package_1e688.o`, which
is still the correct order now that the `.c` file unconditionally
provides the real function (matching `FitScaledSprite`'s precedent, just
without a reorder since this function was already the C file's sole
export at that link position). `tools/report_units.py`'s entry for
`0x0801E578` now points at `None` (NAKED, not "matched" - the
`InitBgSetup`/`FitScaledSprite`/`SpawnStartMarker` convention) instead of the
`.o` file.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings) + `objdiff-cli report generate` and a full
clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`).

## Ninth pass: `DrawScaledSprite` - NAKED transcription

Picked up `DrawScaledSprite` (the viewport-centering helper the fourth pass
left parked under `NON_MATCHING`, fully semantically confirmed but one
gap short of byte-exact). That earlier write-up characterized the gap
carefully: the ROM keeps `self` in `r7` for the whole function
(matching its `push {r4-r7,lr}`/`push {r5,r6,r7}` prologue), and this
compiler only compiles `self[offset]` into a single
`ldrb/ldrh/ldr rX,[r7,#imm]` instruction when `self` is an *ordinary*
local - pinning it to `r7` via `register u8 *self asm("r7")` makes
every dereference lower into a separate `add rX,rX,#imm` plus a
zero-offset load/store instead (confirmed with a minimal one-line
repro), a genuine, reproducible gcc-2.9/agbcc limitation. The fourth
pass's write-up framed this as "unreachable from portable C under this
compiler" - true, but specifically about plain-C register-pin
semantics. It doesn't apply to a `NAKED` hand transcription, which
sidesteps this compiler's addressing-mode-folding pass entirely: there
is no C-level codegen left to fight, since every instruction is
written literally with its own explicit register and immediate-offset
encoding. This is the exact same escape hatch this cluster's sibling
`FitScaledSprite` (`src/graphics/graphics_package_1e688.c`, "Seventh pass"
above) was just closed with earlier today, for the same underlying
`r7`-addressing-mode-folding bug class (just reached via a different
mechanism there - `r7` never entering the compiler's own synthesized
push/pop list at all, rather than losing offset-folding once pinned).

Transcribed the ROM's own Thumb disassembly
(`asm/code_3_2_17_1e644.s`'s guarded `DrawScaledSprite` block, 368 bytes/
0x170) instruction-for-instruction into a `NAKED void DrawScaledSprite`
function, following `FitScaledSprite`'s established style immediately
above it in the same file (suffix-less Thumb mnemonics - `add`/`mov`/
`lsl`/`lsr`/`asr`/`and`/`orr`/`sub` instead of the ROM disassembly's
unified-syntax `adds`/`movs`/`lsls`/... spellings, `neg rX, rX` in
place of the ROM's `movs rX,#N`/`rsbs rX,rX,#0` pairs, and `.pool`
markers placed to reproduce the ROM's own four literal-pool split
points exactly - after the mode-0 block, after the mode-1 block, after
the "mode == 0" clear-bits block just before the affine-allocation
branch point, and a trailing one after the epilogue for the
`gOamBuffer` reload the affine-allocation block does). Verified
by an isolated `cpp`+`agbcc`+`arm-none-eabi-as` compile of just this
function and a direct byte comparison against
`asm/code_3_2_17_1e644.s`'s own bytes (reassembled standalone) - both
came out to exactly 368 bytes, byte-identical, on the very first
attempt; no iteration was needed since the pool placement fell out
naturally from writing the `ldr rX, =literal`s in the ROM's own order
and marking `.pool` at the ROM's own split points.

`asm/code_3_2_17_1e644.s` held nothing but this one guarded function at
this point (its other two functions, `InitBgSetup` and `FitScaledSprite`,
were already cut out by earlier passes), so the file is now deleted
entirely, with its `ldscript.txt` line dropped - `DrawScaledSprite`'s bytes
now come from `graphics_package_1e688.o`, which already links at the
correct position (immediately after `graphics_package_1e640.o`, before
`graphics_package_1e8f8.o`) since it already supplied `FitScaledSprite`'s
real bytes at that same link position. `tools/report_units.py`'s entry
for `0x0801E788` now points at `None` (NAKED, not "matched" - the
`InitBgSetup`/`FitScaledSprite`/`SpawnStartMarker`/`LoadGraphicsPackage`
convention) instead of the `.o` file.

This closes out every function this issue's original 25-function list
named in `asm/code_3_2_17_188d0.s`/`asm/code_3_2_17_1e644.s` except the
two families already flagged as left raw for a future pass
(`SpawnCrystal`-`SpawnYellowGem`'s "trigger effect type N" siblings and
`SpawnLizard`-`SpawnPufferfish`'s "text label as sprite tiles" siblings, per
the fifth pass above) - `asm/code_3_2_17_1e644.s` no longer exists, and
the only object left un-real-C'd immediately around this cluster is
`asm/code_3_2_17_1e990.s`, starting at `SpawnCrystal`.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(clean compile, no warnings) + `objdiff-cli report generate` and a full
clean `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`).

## Tenth pass: `SpawnCrystal`-`SpawnYellowGem` matched as real C under old_agbcc

The last raw stretch of this issue, `asm/code_3_2_17_1e990.s`
(`0x0801EA5C`-`0x0801EF0C`, six functions), is now real C in
`src/graphics/graphics_loading_1ea5c.c`, and the raw file is retired.

These are the "trigger effect type N" spawners the fifth pass flagged
as likely to hit the same register-rotation gap that parked
`SpawnRedGemPlatform`-`SpawnBlueGemPlatform` (`trigger_effect.c`) as NAKED. That gap was
the compiler. The ROM's bit tests read `movs rA, #mask; ldrb rB, [..];
ands rA, rB` - the constant is materialized before the byte it is ANDed
with - which is old_agbcc's tell (docs/matching/issue-24-boss-actor.md).
Built with `tools/agbcc/bin/old_agbcc` (the object is on the Makefile's
`OLD_AGBCC_OBJS`), all six match as plain C with no register pins, no
inline asm and no `goto`s. Under the current agbcc the same source
differs in all six (20-34 changed instructions each).

What the six do (all four-argument `(u32 a0, u16 a1, u16 a2, u16 a3)`
spawners reached through the trigger dispatch table at
`gStaticData_0816C6C0`; `SpawnCrateGem` is also called directly from
`game_loop2.c`):

- `SpawnCrystal`/`SpawnCrateGem`/`SpawnGemPathGem` test bit 0/1/2 of the byte
  `GetCurrentLevelFlags(gLevelState)` returns a pointer to. If it is clear
  they spawn a `CreateSpriteObj` part, point its animation bank at
  `**gSpriteBankSet + 0x1BC` (`SpawnCrystal`) or `+ 0x180`, set its
  tag (+0x2D) and type byte (+0x0A: 0x1B/0x1D/0x1E), run the
  `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` trio, store `GetSpriteAnimPaletteSlot`'s
  frame nibble and register the part with the `gUnknown_030012EC`
  manager. `SpawnCrateGem` additionally spawns effect 0x2B through
  `SpawnEffectPart(gEntitySpawner, ...)` and sets bits 0-1 of its +0x28
  to 1 and clears its "hidden" flag bit.
- `SpawnRedGem`/`SpawnGreenGem`/`SpawnYellowGem` first call
  `GetBossIndex(gLevelState)`; if that returns 1 they hand the
  spawn to `SpawnCortexBossGem` (`actor_part_188d0.c`) with kind 0/1/2.
  Otherwise they test bit 0/2/1 of `gLevelState+2` and spawn the
  same way (tags 3/2/0, types 0x1F/0x20/0x22).

The only things the C has to get right:

- **`tag`/`type` as locals.** The ROM loads both constants into
  callee-saved registers (`r8`/`sb`) before the `CreateSpriteObj` call and
  stores them from there afterwards. That is how this compiler treats a
  variable assigned before the call; a literal at the store site is
  loaded at the store instead.
- **The bit's type.** `SpawnCrateGem`/`SpawnYellowGem` keep the tested bit
  (it is stored as the tag, or passed on to `SpawnEffectPart`) and the ROM
  narrows it with `lsls #24; lsrs #24`, so it is `u8`; `SpawnCrystal`
  keeps it unnarrowed, so it is `s32` there.
- **Branch layout of the mode-1 hand-off.** The ROM tests
  `GetBossIndex(...) == 1` with a `beq` to the `SpawnCortexBossGem` call placed
  after the spawn body, which is `if (... != 1) { spawn } else {
  SpawnCortexBossGem(...); }` - the other nesting puts the hand-off first.

The part object is `struct gfx_part`, moved out of
`actor_part_188d0.c` into the new shared `include/gfx_part.h` (bits 0-3
of +0x28 split into two 2-bit fields for `SpawnCrateGem`'s write; nothing
in `actor_part_188d0.c` used them, and it still matches).

The four `trigger_effect.c` siblings (`SpawnRedGemPlatform`, `SpawnYellowGemPlatform`,
`SpawnGreenGemPlatform`, `SpawnBlueGemPlatform`, NAKED with `#if NON_MATCHING` near-misses
written against the current agbcc) have the same mask-first tell and
very likely fall to the same switch. They were outside this pass's
scope and are unchanged.

Issue #30 stays open: `LoadGraphicsPackage`, `InitBgSetup`,
`FitScaledSprite`, `DrawScaledSprite` and the other NAKED parks listed above are
still NAKED.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
(no warnings from the new file) and a full clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` (`crashbandicootxs.gba: OK`).
