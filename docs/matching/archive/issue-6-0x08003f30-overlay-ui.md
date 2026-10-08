# Issue #6: 0x08003F30-0x08004D74 (overlay_ui, 2 functions)

Issue #6's only two remaining functions on the composite pause/options
screen (`src/save/save_menu_draw.c`): `DrawSaveSlotStats` and
`InitSaveMenuIcons`, both previously "left completely untouched" (raw in
`asm/code_3_1_10_4.s`, no `#if NON_MATCHING` reconstruction at all) per
`docs/matching.md`'s "Issue #6/#7 status" note and the follow-up
`docs/matching/archive/issue-8-0x080060ac-overlay-ui.md` write-up, which both
record that these two were reviewed but not attempted.

## Parked (`NON_MATCHING`, 1)

- **`DrawSaveSlotStats`** (`src/save/save_menu_draw.c`, real bytes wrapped
  `.if NON_MATCHING == 0` in `asm/code_3_1_10_4.s`) - a per-row numeric
  display: draws three of the row's `struct settings_row_stats` fields
  (`field_4`/`field_10`/`field_8` - `statPtr` is
  `(&self->currentStats)[rowIdx]`, i.e. `currentStats` and `rowStats
  [0..3]` read as one contiguous 5-element array, the same shape
  `RefreshSaveSlotSummaries`/`SummarizeProgress` in `src/save/save_menu_ui.c`
  already establish for `rowStats`) as plain decimal strings via
  `itoa`, one each into `self->rowObjA[rowIdx]`/`rowObjC[rowIdx]`/
  `rowObjB[rowIdx]` (small position objects `InitSaveMenuIcons` below
  allocates), each drawn through `gSmallFont`'s `record->
  slots[2]` trampoline and preceded by the same highlight/dim
  `FontSetPalette` call `DrawSaveSlots` (this file, already parked) uses for
  its own selected-row highlight. A fourth value (`statPtr->field_0`)
  is formatted as `"NN%"` by `itoa`-ing then manually scanning for the
  NUL terminator and overwriting it with a literal `%` byte
  (re-terminating one byte later) - measured once via
  `gLargeFont`'s `slots[0]` trampoline to get its pixel width,
  then drawn a second time via that same manager's `slots[2]`
  trampoline, right-aligned against the caller-supplied `label1`
  x-coordinate using the measured width (`posX = label1 - width +
  0x1f`) - the standard "measure, then right-align" idiom this ROM
  region uses throughout (see `DrawSaveMenuCancel`'s own write-up in this same
  file for another instance).

  Every load, store, and call is confirmed against the ROM (all four
  positioned-draw blocks follow the exact same
  position/reset/format/highlight/draw shape, just at different fixed
  offsets and through different `rowObj*` arrays/`icon_record` slot
  indices). **Not yet byte-matching** - this is the same "several
  near-identical unrolled blocks, each wanting the loop-carried
  registers in slightly different places" difficulty class this file's
  other parked functions (`DrawSaveMenuMessageLines`/`DrawYesNoPrompt`/`DrawSaveSlots`/
  `DrawEmptySlotLabel`/`DrawSaveMenuTitle`) already document at length, compounded
  here by four blocks instead of two-to-three and by several
  cross-block-live locals (`highlight`, the running `buf[]` contents,
  the row object pointers) that would need the same kind of
  `SUB_8006600_*`/`UPDATE_ICON_FRAME_NIBBLE`-style per-call-site
  register-pin macro work `src/menus/power_dialog.c`'s
  `InitPowerDialog` needed (see that file's header comment for the concrete
  gotchas that technique runs into) - not attempted here given the size
  of the function and the number of near-identical blocks it would need
  repeating across.

## Left completely untouched (1)

- **`InitSaveMenuIcons`** (`asm/code_3_1_10_4.s`) - the screen's own init
  routine: resets the OAM shadow buffer and two tile caches
  (`ResetOamBuffer`/`HideUnusedOamEntries`/`WaitForVBlank`/`CommitOamBuffer` on
  `gOamBuffer`, `FreeUnlockedPaletteSlots`/four `ClaimPaletteSlot` calls on
  `gPaletteCache`), copies the first four per-level
  `gStaticData_0816Bxxx` tables (`gSaveMenuPalette0`/`15A`/`17A`/
  `19A` - the same tables `docs/rom_map.md`'s settings-menu
  investigation already links to this screen) into a 16-row loop
  writing halfwords at `self+0x2c`/`+0x4c` and `self+0x6c`/`+0x8c`
  (four parallel arrays, 0x20 bytes apart, 16 entries each - not yet
  reconciled against `struct save_menu`'s existing
  `rowStats`/`currentStats` layout, which only covers up to offset
  `0x8c`), then runs the **exact same 9-statement two-icon-manager init
  block** `src/menus/power_dialog.c`'s parked `ShowPowerDialog`
  already transcribes byte-for-byte identically (zero `gSmallFont`/
  `030012E0`'s posX/posY, fire each one's `record->slots[6]` trampoline,
  reserve `field_12c<<5` bytes of VRAM via `ReserveObjVram`, copying
  `gSmallFont`'s `field_12c` into `gLargeFont`'s
  `field_108` in between), and finally allocates 15 objects (5 each
  across `rowObjA`/`rowObjB`/`rowObjC`, the exact arrays `DrawSaveSlotStats`
  above reads) in a `sl`/`sb`/`r8`-heavy loop, each one built via the
  standard `OperatorNew(0x40)`/`InitUiSpriteObj` alloc, a `gSpriteBankSet`
  header-table pointer at three new offsets (`0xc0<<1`/`0xc6<<1`/
  `0xde<<1`, extending `docs/rom_map.md`'s "five confirmed
  header-relative offsets" note to eight), a fixed `type` byte (`1`/
  `2`/`0` respectively) at `+0x2d`, the standard `ResetSpriteFrameTimer`/
  `ResetSpriteFrameIndex`/`SetSpriteAnimDone` OAM trio, and a `GetSpriteAnimPaletteSlot`-driven
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
     map onto `struct save_menu`'s existing fields at those
     offsets (`flags` at `0x04`, `state` at `0x0c`, ..., `field_24` at
     `0x24`, `currentStats` starting at `0x28`) - `self+0x2c` lands
     4 bytes into `currentStats`, `self+0x4c` and `self+0x6c` land
     inside `rowStats[0]`/`rowStats[1]`, and `self+0x8c` is exactly
     `field_8c` (already named, a different object entirely per
     `docs/matching/archive/issue-5-overlay-ui-sync.md`) - a real, unresolved
     conflict between two different chunks' independently-derived
     field layouts for the same struct, not just an unnamed gap.
  2. The three new `0xc0<<1`/`0xc6<<1`/`0xde<<1` header-table offsets
     this function's 15 object allocations use would need independent
     confirmation the same way the existing five were each confirmed
     one function at a time - not done here given the size of the
     function already at risk from point 1.

  Since `InitSaveMenuIcons` already contains the exact same two-icon-manager
  block `ShowPowerDialog` does (confirmed byte-for-byte identical in the
  raw disassembly), it would very likely hit that block's own
  demonstrated register-allocation resistance too (see
  `docs/matching/archive/issue-8-0x080060ac-overlay-ui.md`'s "Second pass"
  section) even setting the two open questions above aside - so a
  parked `NON_MATCHING` attempt wasn't started this pass either.

Since neither function is fully matched, **issue #6 is not closed by
this pass**.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`) after
`DrawSaveSlotStats`'s parked reconstruction landed.

## Later pass: `DrawSaveSlotStats` converted to NAKED transcription (still tracked as parked)

The "several near-identical unrolled blocks, each wanting the
loop-carried registers in slightly different places" gap described
above never had a plain-C fix - same class as `DrawPowerDialog`
(`docs/status/graphics.md`). Since the semantics were already fully
confirmed (this document's own derivation above), `DrawSaveSlotStats` was
converted to a byte-verified NAKED asm transcription instead, the same
pass that also converted this file's five sibling functions
(`LinkExchangeSaveData`, `DrawSaveMenuMessageLines`, `DrawSaveMenuCancel`, `DrawYesNoPrompt`,
`DrawSaveSlots` - all built on the same centered-label/positioned-glyph
primitive, all hitting the identical difficulty class) - see
`src/save/save_menu_draw.c`'s header comment and
`src/util/printf.cpp`'s `FindSubstring` for the established NAKED-
transcription pattern. Every instruction in all six now matches the
ROM exactly; verified via a full clean `make compare` (`La suma
coincide`). Per this project's tracking policy, byte-exact NAKED asm
doesn't count as "matched" - only real decompiled C does - so all six
are tracked as **parked** in `tools/report_units.py`/
`docs/status/overlay_ui.md`, not matched, even though their bytes are
provably correct.

`InitSaveMenuIcons` (this file's own init routine, still genuinely not
understood with confidence - see the two open questions above) was not
attempted this pass and stays fully raw in `asm/code_3_1_10_4.s`
(trimmed to just this one function once its five siblings graduated
out of the file). `InitSaveMenuIcons` and issue #6 both stay open.

## Later pass: 7 of 9 are real C

The issue #4/#6/#8 retry ([issue-4-6-8-naked-retry.md](issue-4-6-8-naked-retry.md)) matched these as plain C:

- `LinkExchangeSaveData`, `DrawSaveMenuMessageLines`, `DrawSaveMenuCancel`, `DrawSaveSlotStats` and
  `DrawSaveSlots` in `save_menu_draw.c`, which is now on `OLD_AGBCC_OBJS`
  (the matched functions compile the same under both compilers).
- `DrawEmptySlotLabel` and `DrawSaveMenuTitle` in `save_menu_ui.c`.

`DrawSaveSlotStats`'s byte argument is a packed one-byte struct.
`DrawSaveSlots` inlines `DrawEmptySlotLabel`.

Two are left:

- `DrawYesNoPrompt` is still NAKED. Its draft is 9 halfwords off because
  two constants' registers are swapped.
- `InitSaveMenuIcons` now has a C reconstruction under `NON_MATCHING`. It is
  5 halfwords off under old_agbcc, in the loop pre-header only. Its raw
  bytes in `asm/code_3_1_10_4.s` are now guarded with
  `.if NON_MATCHING == 0`.

## Later pass: hard-register hold

`InitSaveMenuIcons` is now real C in `src/save/save_menu_draw.c` (old_agbcc)
and `asm/code_3_1_10_4.s` is gone. The loop pre-header was already fixed
by plain `u8 *`/`u16 *` stores (early-rom-naked-retry-2.md). The last 6
halfwords were the third icon: the frame-0 store takes its address in r0
through a pinned pointer, and an r1 hold at the nibble mask makes reload
pick r3 as in the ROM. The hold's asm statements shift the stack-slot
order, and three bare `asm("")` at the top restore it. See
[hard-register-hold-retry.md](hard-register-hold-retry.md).
