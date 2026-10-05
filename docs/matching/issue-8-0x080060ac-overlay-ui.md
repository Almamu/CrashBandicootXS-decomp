# Issue #8: 0x080060AC-0x08006600 (overlay_ui, 9 functions)

The tail end of the composite pause/options screen's `asm/code_3_1_10.s`
region, continuing directly from GitHub issue #7's `PauseMenuCursorDown`/
`PauseMenuCursorUp` (`src/graphics/settings_menu5.o`) up to the pre-existing
parked `DrawPowerDialog` guard (`src/graphics/oam_count.c`) that already
marks this file's end. 3 of the 9 functions matched, 4 parked
(`NON_MATCHING`, semantics understood), 2 left completely untouched.

## Matched (3)

- **`FormatDecimal`** (`src/graphics/settings_menu9.c`) - decimal `itoa`:
  writes `value`'s digits (unsigned, most significant first) into
  `dest`, NUL-terminated, returning the digit count. Builds digits
  least-significant-first into a small stack buffer via the existing
  `__modsi3`/`__divsi3` div/mod primitives
  (`src/util/math_div_util.c`), then reverses them into `dest`. Shared
  by every settings-row/results-widget number label already matched in
  `src/graphics/settings_menu6.c`/`settings_menu7.c` (both already call
  it as an `extern`).

  The interesting gotcha: this compiler always copies argument
  registers to their home pseudo-registers in ascending source-register
  order (r0 before r1) for a plain, unpinned parameter list - but the
  ROM copies r1 (`dest` -> r7) *first*, r0 (`value` -> r5) *second*.
  Pinning `value`'s copy into a separate `register s32 val asm("r5") =
  value;` statement (rather than reading the plain `value` parameter
  directly) defers that r0->r5 copy until just before the loop that
  first needs it, which reproduces the ROM's order - while leaving
  `dest` alone for the natural allocator to find r7 on its own. An
  *explicit* pin on `dest` instead (`register u8 *d asm("r7") = dest`)
  looks tempting but is a genuine ABI hazard in this toolchain: the
  compiler stops including r7 in the function's own push/pop list once
  it's an explicit register variable, silently corrupting the caller's
  r7 (see `matching_decomp_register_pinning` memory) - the natural
  allocator gets the push/pop list right by itself once it reaches for
  r7 unprompted, which is what happens here once `value`'s copy is
  deferred.

- **`FormatVolumePercent`** (`src/graphics/settings_menu9.c`) - formats a
  `" <NN%>"`-shaped scratch string (space, `<`, decimal digits of
  `arg1 * 5` via `FormatDecimal` above, `%`, `>`, NUL) into `out`. Its
  first parameter is read by nothing in the ROM - a genuinely unused
  argument (confirmed: the very first real instruction overwrites r0
  before ever reading it).

- **`CommitPauseMenuFrame`** (`src/graphics/settings_menu12.c`) - the composite
  screen's own top-level object's "apply display registers" step:
  flushes VRAM DMA, clears `PLTT`, and writes `field_c8`/`field_cc`/
  `field_d0` to `REG_BLDCNT`/`REG_BLDY`/`REG_DISPCNT` - the same shape
  `src/graphics/oam_count.c`'s already-matched `CommitPowerDialogFrame` uses for a
  *different*, smaller per-widget object (`struct sub_8006700_actor`),
  confirming this is a second, larger "self" object still not fully
  reconciled (built by the still-raw `RunPauseMenu`/`InitPauseMenu`,
  GitHub issue #7). The final `field_d0` read+store needed an
  inline-asm-computed address pinned to `r0` rather than a plain field
  access: with plain `self->field_d0`, this compiler notices `self`
  (r4) is dead after that point and folds the address computation
  directly into r4 (one instruction shorter than the ROM), which the
  ROM's own codegen never does here (unlike the `field_c8`/`field_cc`
  accesses just above, which get the ROM's exact shape for free from
  plain field access).

  `FormatVolumePercent` also needed a trailing `asm(".align 2, 0")` - the
  classic alignment-padding gotcha (`matching_decomp_alignment_fix`
  memory): the ROM pads the gap before the next function with zero
  bytes (an explicit `.align 2, 0` in the original assembly, since the
  next function at the time was still raw), but this compiler's own
  default inter-function padding is a `mov r8, r8` NOP-equivalent
  instead - a real full-link mismatch that an isolated compile alone
  didn't surface until the full `make compare` pass.

## Parked (`NON_MATCHING`, 4)

All four hit the same class of gcc-2.9 register-allocation difficulty
already documented at length for `DrawPowerDialog` (`src/graphics/
oam_count.c`) and `InitPausePowersPage`/`InitPauseGemsPage`/`InitPauseRelicsPage`/`InitPauseTimeTrialPage`
(`src/graphics/settings_menu6.c`, GitHub issue #7): every load, store,
branch, and call is semantically confirmed, but the loop/self pointer
and the icon-manager position-store's scratch registers never land
exactly where the ROM's own allocator puts them, no matter how the
source is rephrased. Closing that gap would need the same kind of heavy
per-call-site register-pin macro work already invested in `DrawPowerDialog`
- more than this pass had budget for across four more functions on top
of it.

- **`DrawPauseTimeTrialPage`**, **`DrawPauseCrystalsPage`**, **`DrawPauseMenuPageTitle`**
  (`src/graphics/settings_menu11.c`, real bytes wrapped `.if
  NON_MATCHING == 0` in `asm/code_3_1_10_14.s`) - three icon-manager
  centered-label draws, each the companion "draw a number/label
  centered on an icon widget" step for one of GitHub issue #7's
  icon-widget constructors:
  - `DrawPauseTimeTrialPage` draws `self->timeBuf` (a `FormatCentiseconds`
    result) centered on the medal-icon widget `InitPauseTimeTrialPage` builds,
    using the same fixed `gStaticData_0816B27C` position pair that
    icon itself is positioned with. `self` is the same `struct
    pause_screen_results` `settings_menu6.c` already documents
    (`field_6c`/`field_bc`/`timeBuf` all line up at their existing
    offsets).
  - `DrawPauseCrystalsPage` positions the icon manager at `gStaticData_0816B1E4`
    (the same fixed point `InitPauseCrystalsPage`'s `field_88` icon uses) and
    hands off to the still-raw `DrawPauseFraction` (GitHub issue #7) to
    actually draw the two small result-count strings `InitPauseCrystalsPage`
    already formatted into `buf2c`/`buf46`.
  - `DrawPauseMenuPageTitle` draws a fixed-position (0xc2, 0x2c) category/header
    label, picking its source character from
    `gPauseMenuPageTitles[self->field_24]` fed through `GetUiText`
    (the same "char code -> something `_call_via_r2` can draw"
    conversion `DrawPowerDialog`/`InitPauseCrystalsPage` already use for fixed digits
    like `0x2e`/`0x14`). This `self` is a different, not-yet-reconciled
    object from the other two (only `field_24`, a plain `s32` category
    index, is touched here).

- **`PowerDialogLoop`** (`src/graphics/settings_menu10.c`, real bytes
  wrapped `.if NON_MATCHING == 0` in `asm/code_3_1_10_13.s`) - the
  settings-row confirm-cursor stepper on the small per-widget object
  `src/graphics/oam_count.c` already names `struct sub_8006700_actor`
  (redeclared locally here per this project's minimal-local-type
  convention for a type already anchored in another translation unit).
  Steps `field_24`'s low 5 bits down to 0 one at a time (redrawing/
  committing every step via `DrawPowerDialog`/`CommitPowerDialogFrame`/`AnimatePowerDialog`),
  then polls input (`UpdateKeys`/`gKeys.pressed`,
  redrawing every frame) until the confirm button is newly pressed,
  then steps `field_24` back up to `0x10` the same way, and finally
  forces `field_28` to `0x40` and re-applies.

## Left completely untouched (2)

- **`ShowPowerDialog`**, **`InitPowerDialog`** (`asm/code_3_1_10_12.s`) - a
  large pair of functions (the second taking 5 register arguments via
  `sb`/`sl`/`r8`) that allocate/construct another icon widget and wire
  it into the composite screen via `LoadGraphicsPackage`/
  `InitBgSetup`/`OperatorNew`. The overall shape is visible (another
  `settings_icon_actor`-style construction plus a
  `mem_free_bytes(MEM_HEAP_BOTH)` pair bracketing the whole thing,
  mirroring `ShowPowerDialog`'s own bracket), but several field offsets and
  the exact call graph through `InitPowerDialog`'s five-register argument
  list weren't traced to full confidence in the time available - left
  raw per docs/workflow.md's "leave it completely untouched" step
  rather than force a low-confidence reconstruction.

## File structure

`asm/code_3_1_10_11.s` (originally `FormatDecimal` through the
pre-existing parked `DrawPowerDialog` guard) split into:
`src/graphics/settings_menu9.c` (`FormatDecimal`/`FormatVolumePercent`, matched),
the new raw `asm/code_3_1_10_14.s` (`DrawPauseTimeTrialPage`/`DrawPauseCrystalsPage`/
`DrawPauseMenuPageTitle`'s real bytes, wrapped), `src/graphics/settings_menu11.c`
(their `NON_MATCHING` C reconstructions), `src/graphics/settings_menu12.c`
(`CommitPauseMenuFrame`, matched), the new raw `asm/code_3_1_10_12.s`
(`ShowPowerDialog`/`InitPowerDialog`, left untouched), `src/graphics/
settings_menu10.c` (`PowerDialogLoop`'s `NON_MATCHING` C reconstruction),
the new raw `asm/code_3_1_10_13.s` (its real bytes, wrapped), and
finally the trimmed `asm/code_3_1_10_11.s` (just the original file's
unchanged `DrawPowerDialog` guard). `ldscript.txt` and `tools/
report_units.py`'s `overlay_ui` entries both updated to match this
finer split, keeping every function's own address unchanged.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`). The first
attempt at this split briefly broke `make compare` two different ways,
both worth recording: (1) putting `CommitPauseMenuFrame` in the same object file
as `FormatDecimal`/`FormatVolumePercent` reordered it *before* the parked
`DrawPauseTimeTrialPage`/`DrawPauseCrystalsPage`/`DrawPauseMenuPageTitle` trio in the final link (since
whole object files link as contiguous units in `ldscript.txt` order,
regardless of which functions within them are matched vs. parked) -
fixed by giving `CommitPauseMenuFrame` its own object file
(`settings_menu12.o`) positioned after the trio's; (2) the
`FormatVolumePercent`/`DrawPauseTimeTrialPage` boundary needed the trailing
`asm(".align 2, 0")` described above. Neither was visible from an
isolated per-function compile - both are exactly the kind of
link-context-only bug docs/workflow.md's step 2/3 warning describes.

## Issue #6/#7 status (not addressed this pass)

This pass focused entirely on issue #8. Issue #6's two remaining
untouched functions (`DrawSaveSlotStats`, `InitSaveMenuIcons`) and its two
already-parked functions worth revisiting (`DrawEmptySlotLabel`,
`DrawSaveMenuTitle`) were reviewed but not changed - `DrawSaveSlotStats`/
`InitSaveMenuIcons` are each several hundred lines of raw disassembly (a
per-row multi-array numeric renderer and the screen's own OAM-buffer/
object-allocation init routine respectively) that weren't traced to
full confidence in the time available, and `DrawEmptySlotLabel`/`DrawSaveMenuTitle`
hit the exact same well-documented gcc-2.9 "last mile" register
nondeterminism this issue's own parked quartet does - no new technique
was found to close them this pass. Issue #7's 12 untouched functions
(the composite screen's own constructor pair, its settings-row cursor/
confirm/cancel driver, and their icon-manager-heavy sub-widgets) were
not attempted this pass either; docs/rom_map.md's "Correction:
`overlay_ui` is a small family of screens" section already gives high-
confidence semantics for several of them
(`RunPauseMenu`/`InitPauseMenu`/`DestroyPauseMenu`/`PauseMenuLoop`/`InitPauseMenuInfo`)
and would be a reasonable next chunk to pick up. Both issues are left
open with this comment.

## Second pass: `InitPowerDialog` matched, `ShowPowerDialog` parked

A follow-up pass tackled this issue's final two "left completely
untouched" functions, `ShowPowerDialog`/`InitPowerDialog` (previously raw in
`asm/code_3_1_10_12.s`).

- **`InitPowerDialog`** (`src/graphics/settings_menu13.c`) - the two-string
  dialog/message-box object constructor docs/rom_map.md's "Correction"
  section already traced: builds a small `struct sub_8006700_actor`
  (the same object `src/graphics/oam_count.c`/`settings_menu10.c`
  already name, allocated by the caller at exactly its own `0x2c`-byte
  size) plus one `struct settings_icon_actor`-shaped background icon
  owned via `field_18`, built the same way `settings_menu6.c`'s icon
  constructors are (`InitUiSpriteObj(OperatorNew(0x40))`, `field_20`
  pointed at the shared `gSpriteBankSet` header table at a new
  `0xe4<<1` offset). **Matched byte-exact**, but only after the same
  class of heavy register pinning `oam_count.c`'s `SUB_8006600_*`
  macros and `settings_menu6.c`'s `UPDATE_ICON_FRAME_NIBBLE` already
  use - see the full account of gotchas (post-increment-store fusion,
  address-register reuse across adjacent-but-distinct offsets, and a
  plain `register T v asm("rN") = expr` initializer not actually
  forcing the copy into `rN`) in that file's own header comment.

  The first attempt at this function looked byte-identical in an
  **isolated** compile but still failed a full clean `make compare` -
  a real instance of docs/workflow.md's warning that an isolated
  compile is diagnostic only: the compiled function came out 4 bytes
  *shorter* than the ROM's own `0x140`-byte span (ending at
  `0x08006514` instead of `0x08006518`, silently shifting every
  address after it and cascading into a checksum mismatch that looked,
  from `cmp`'s output alone, like the whole ROM had come apart). The
  missing 4 bytes turned out to be a single `field_29`-address
  computation the natural allocator "optimized" into `addr + 1`
  (reusing the still-live `field_28` address register) instead of the
  ROM's fresh `self + 0x29` recomputation - found by objdumping the
  *actual linked* `crashbandicootxs.elf` at the real ROM address and
  diffing it directly against the ROM's own disassembly, not by trusting
  the isolated `.s` output a second time.

- **`ShowPowerDialog`** (`src/graphics/settings_menu14.c`) - the higher-
  level dialog spawner docs/rom_map.md already traced (palette/DISPCNT
  reset, re-init the two icon managers, reset the VRAM upload cursor,
  fire each manager's `record->slots[6]` trampoline, allocate and build
  the dialog via `InitPowerDialog` above, then run it via `PowerDialogLoop`).
  Every load/store/call is confirmed against the ROM. **Parked**
  (`NON_MATCHING`) - hits the same "last mile" gcc-2.9 register/
  constant-reuse nondeterminism this issue's other parked functions
  document, specifically around the two cached-global-address locals
  (`gSmallFont`/`030012E0`, `r5`/`r8` in the ROM) and the exact
  constant-reuse trick the ROM uses to derive `gLargeFont`'s
  `field_108` offset by subtracting `0x24` from the already-loaded
  `field_12c` offset constant rather than loading a fresh literal. An
  explicit `asm("r7")` pin for the `type` parameter was tried and
  discarded - it produced a genuine **correctness bug**, not just a
  mismatch: the natural allocator reused r7 for an unrelated cached
  address partway through the function, silently clobbering the pinned
  value before its one remaining use at the `InitPowerDialog` call site.
  This is the project's documented categorical r7-pin limitation
  (`docs/matching/naked-sub_8007dbc.md`) showing up in a new function;
  not worth continuing to push on for a single call site. Real bytes
  wrapped `.if NON_MATCHING == 0` in the new `asm/code_3_1_10_15.s`.

Since `ShowPowerDialog` is still parked, **issue #8 is not fully closed by
this pass** - one function's worth of register-allocation work remains.

## File structure (second pass)

`asm/code_3_1_10_12.s` (previously holding both `ShowPowerDialog` and
`InitPowerDialog` raw) is gone - split into the new `src/graphics/
settings_menu14.c` (`ShowPowerDialog`'s `NON_MATCHING` C reconstruction),
the new `asm/code_3_1_10_15.s` (`ShowPowerDialog`'s real bytes, wrapped),
and the new `src/graphics/settings_menu13.c` (`InitPowerDialog`, matched,
unconditional). `ldscript.txt` updated to link them in that order
(`settings_menu12.o`, `settings_menu14.o`, `code_3_1_10_15.o`,
`settings_menu13.o`, `settings_menu10.o`, ...), keeping every
function's own address unchanged. `tools/report_units.py`'s
`overlay_ui` entries updated to match.

Verified via a full clean `rm -rf build && make NON_MATCHING=1 report`
and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`).

## Third pass: the remaining parked quartet + `ShowPowerDialog` matched via NAKED transcription

Closed out this issue's last 4 parked functions (`DrawPauseTimeTrialPage`,
`DrawPauseCrystalsPage`, `DrawPauseMenuPageTitle` in `src/graphics/settings_menu11.c`,
`PowerDialogLoop` in `src/graphics/settings_menu10.c`) plus `ShowPowerDialog`
(`src/graphics/settings_menu14.c`), the second pass's remaining parked
function - all 5 now byte-exact matched, confirmed by a full clean
`make compare` (`La suma coincide`).

Same treatment as `docs/matching/issue-7-0x08004d74-overlay-ui.md`'s
second pass (done in the same session, as one combined batch across
both issues): each was fully understood already (their own doc comments
walk every field/branch/call), just blocked by this project's
well-documented gcc-2.9 register-allocation nondeterminism, so each was
converted to `NAKED` asm - a mechanical, byte-verified transcription of
the ROM's own instructions (translated to divided syntax, GNU numeric
local labels) rather than continuing to fight the compiler over
individual register choices. `ShowPowerDialog` in particular is the same
function whose doc comment above records a genuine correctness bug from
an `asm("r7")` pin attempt in the second pass - the NAKED rewrite
sidesteps that whole class of bug by not pinning anything, just
reproducing the ROM's own register choices verbatim.

`DrawPauseTimeTrialPage`/`DrawPauseCrystalsPage`/`DrawPauseMenuPageTitle` and `PowerDialogLoop` needed no
`ldscript.txt`/file restructuring - each already had its own dedicated
object file position reserved (`settings_menu11.o`/`settings_menu10.o`),
just contributing zero bytes to the real build while entirely parked;
removing their `#if NON_MATCHING` guards (and the corresponding raw
`.if NON_MATCHING == 0` bytes from `asm/code_3_1_10_14.s`/
`code_3_1_10_13.s`, now-empty files deleted outright) was a pure
in-place change. `ShowPowerDialog` likewise needed no restructuring beyond
deleting its now-empty `asm/code_3_1_10_15.s`.

Verified via the same full clean `rm -rf build && make NON_MATCHING=1
report` and `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare` (`La suma coincide`) this whole
session's combined batch (across both issue #7 and issue #8) required.

### Closing this issue

Every function in GitHub issue #8's original 9-function range
(`FormatDecimal`-`PowerDialogLoop`, including `ShowPowerDialog`/`InitPowerDialog`
which that issue's own checklist hadn't been marked complete for) is
now byte-exact matched. This PR closes issue #8.

## Later pass: the NAKED five are real C

The issue #4/#6/#8 retry ([issue-4-6-8-naked-retry.md](issue-4-6-8-naked-retry.md)) replaced all five NAKED transcriptions
from the third pass with plain C:

- `DrawPauseTimeTrialPage`, `DrawPauseCrystalsPage` and `DrawPauseMenuPageTitle` match under both
  compilers once the draws are written as `record->slots[n]` virtual
  calls.
- `ShowPowerDialog` matches under both with the `IconSetup`/`IconReserve`
  helpers and a one-argument `FontResetPalette`.
- `PowerDialogLoop` matches under old_agbcc with a packed `level:5`
  bitfield. `settings_menu10.c` is now on `OLD_AGBCC_OBJS`.

The issue's range now has no NAKED, raw or NON_MATCHING functions.
