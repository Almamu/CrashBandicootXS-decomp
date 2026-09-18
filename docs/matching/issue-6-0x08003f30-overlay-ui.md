# Issue #6: 0x08003F30-0x08004D74 (overlay_ui, 2 functions)

Issue #6's only two remaining functions on the composite pause/options
screen (`src/graphics/settings_menu.c`): `sub_8003F30` and
`sub_800450C`, both previously "left completely untouched" (raw in
`asm/code_3_1_10_4.s`, no `#if NON_MATCHING` reconstruction at all) per
`docs/matching.md`'s "Issue #6/#7 status" note and the follow-up
`docs/matching/issue-8-0x080060ac-overlay-ui.md` write-up, which both
record that these two were reviewed but not attempted.

## Parked (`NON_MATCHING`, 1)

- **`sub_8003F30`** (`src/graphics/settings_menu.c`, real bytes wrapped
  `.if NON_MATCHING == 0` in `asm/code_3_1_10_4.s`) - a per-row numeric
  display: draws three of the row's `struct settings_row_stats` fields
  (`field_4`/`field_10`/`field_8` - `statPtr` is
  `(&self->currentStats)[rowIdx]`, i.e. `currentStats` and `rowStats
  [0..3]` read as one contiguous 5-element array, the same shape
  `sub_8004860`/`sub_80048E0` in `src/graphics/settings_menu2.c`
  already establish for `rowStats`) as plain decimal strings via
  `itoa`, one each into `self->rowObjA[rowIdx]`/`rowObjC[rowIdx]`/
  `rowObjB[rowIdx]` (small position objects `sub_800450C` below
  allocates), each drawn through `gUnknown_030012DC`'s `record->
  slots[2]` trampoline and preceded by the same highlight/dim
  `sub_8028A30` call `sub_80041BC` (this file, already parked) uses for
  its own selected-row highlight. A fourth value (`statPtr->field_0`)
  is formatted as `"NN%"` by `itoa`-ing then manually scanning for the
  NUL terminator and overwriting it with a literal `%` byte
  (re-terminating one byte later) - measured once via
  `gUnknown_030012E0`'s `slots[0]` trampoline to get its pixel width,
  then drawn a second time via that same manager's `slots[2]`
  trampoline, right-aligned against the caller-supplied `label1`
  x-coordinate using the measured width (`posX = label1 - width +
  0x1f`) - the standard "measure, then right-align" idiom this ROM
  region uses throughout (see `sub_8003C90`'s own write-up in this same
  file for another instance).

  Every load, store, and call is confirmed against the ROM (all four
  positioned-draw blocks follow the exact same
  position/reset/format/highlight/draw shape, just at different fixed
  offsets and through different `rowObj*` arrays/`icon_record` slot
  indices). **Not yet byte-matching** - this is the same "several
  near-identical unrolled blocks, each wanting the loop-carried
  registers in slightly different places" difficulty class this file's
  other parked functions (`sub_8003BDC`/`sub_8003D3C`/`sub_80041BC`/
  `sub_8004914`/`sub_80049CC`) already document at length, compounded
  here by four blocks instead of two-to-three and by several
  cross-block-live locals (`highlight`, the running `buf[]` contents,
  the row object pointers) that would need the same kind of
  `SUB_8006600_*`/`UPDATE_ICON_FRAME_NIBBLE`-style per-call-site
  register-pin macro work `src/graphics/settings_menu13.c`'s
  `sub_80063D8` needed (see that file's header comment for the concrete
  gotchas that technique runs into) - not attempted here given the size
  of the function and the number of near-identical blocks it would need
  repeating across.

## Left completely untouched (1)

- **`sub_800450C`** (`asm/code_3_1_10_4.s`) - the screen's own init
  routine: resets the OAM shadow buffer and two tile caches
  (`sub_8006A90`/`sub_8006A48`/`sub_80006A8`/`sub_8006AAC` on
  `gUnknown_03001300`, `sub_8006EA8`/four `sub_8006D50` calls on
  `gUnknown_030012B8`), copies the first four per-level
  `gStaticData_0816Bxxx` tables (`gStaticData_0816B13A`/`15A`/`17A`/
  `19A` - the same tables `docs/rom_map.md`'s settings-menu
  investigation already links to this screen) into a 16-row loop
  writing halfwords at `self+0x2c`/`+0x4c` and `self+0x6c`/`+0x8c`
  (four parallel arrays, 0x20 bytes apart, 16 entries each - not yet
  reconciled against `struct pause_options_screen`'s existing
  `rowStats`/`currentStats` layout, which only covers up to offset
  `0x8c`), then runs the **exact same 9-statement two-icon-manager init
  block** `src/graphics/settings_menu14.c`'s parked `sub_80062A8`
  already transcribes byte-for-byte identically (zero `gUnknown_030012DC`/
  `030012E0`'s posX/posY, fire each one's `record->slots[6]` trampoline,
  reserve `field_12c<<5` bytes of VRAM via `sub_8006C58`, copying
  `gUnknown_030012DC`'s `field_12c` into `gUnknown_030012E0`'s
  `field_108` in between), and finally allocates 15 objects (5 each
  across `rowObjA`/`rowObjB`/`rowObjC`, the exact arrays `sub_8003F30`
  above reads) in a `sl`/`sb`/`r8`-heavy loop, each one built via the
  standard `sub_8026EDC(0x40)`/`sub_8008904` alloc, a `gUnknown_030012D0`
  header-table pointer at three new offsets (`0xc0<<1`/`0xc6<<1`/
  `0xde<<1`, extending `docs/rom_map.md`'s "five confirmed
  header-relative offsets" note to eight), a fixed `type` byte (`1`/
  `2`/`0` respectively) at `+0x2d`, the standard `sub_80087C0`/
  `sub_80087B4`/`sub_800872C` OAM trio, and a `sub_800815C`-driven
  `field_29` nibble update - then closes with five fixed Q8 width/
  height rects written directly through the `sp`-cached object
  pointers.

  **Left fully raw rather than force a low-confidence reconstruction.**
  The overall shape is fully traced (every helper call above is
  cross-referenced against an already-matched/parked sibling with the
  identical signature), but two things keep this from being confidently
  written up as C:
  1. The 16-row copy loop's four destination arrays
     (`self+0x2c`/`self+0x4c`/`self+0x6c`/`self+0x8c`) don't cleanly
     map onto `struct pause_options_screen`'s existing fields at those
     offsets (`flags` at `0x04`, `state` at `0x0c`, ..., `field_24` at
     `0x24`, `currentStats` starting at `0x28`) - `self+0x2c` lands
     4 bytes into `currentStats`, `self+0x4c` and `self+0x6c` land
     inside `rowStats[0]`/`rowStats[1]`, and `self+0x8c` is exactly
     `field_8c` (already named, a different object entirely per
     `docs/matching/issue-5-overlay-ui-sync.md`) - a real, unresolved
     conflict between two different chunks' independently-derived
     field layouts for the same struct, not just an unnamed gap.
  2. The three new `0xc0<<1`/`0xc6<<1`/`0xde<<1` header-table offsets
     this function's 15 object allocations use would need independent
     confirmation the same way the existing five were each confirmed
     one function at a time - not done here given the size of the
     function already at risk from point 1.

  Since `sub_800450C` already contains the exact same two-icon-manager
  block `sub_80062A8` does (confirmed byte-for-byte identical in the
  raw disassembly), it would very likely hit that block's own
  demonstrated register-allocation resistance too (see
  `docs/matching/issue-8-0x080060ac-overlay-ui.md`'s "Second pass"
  section) even setting the two open questions above aside - so a
  parked `NON_MATCHING` attempt wasn't started this pass either.

Since neither function is fully matched, **issue #6 is not closed by
this pass**.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`) after
`sub_8003F30`'s parked reconstruction landed.
