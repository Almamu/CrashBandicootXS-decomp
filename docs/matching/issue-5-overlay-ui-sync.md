# Issue #5: 0x08002C84-0x08003B40 (overlay_ui)

25 functions right before the already-matched/parked
`0x08003B40-0x08004CB4` "connecting..." spinner-dialog cluster
(issue #7). 19 matched, 6 parked. All addresses confirmed via a full
clean `make compare` ("La suma coincide").

## The settings-sync protocol

The chunk's first cluster (`sub_8002C84`-`sub_8002EFC`) turned out to
be a multiplayer settings-sync protocol built on two small object
types, both now named and documented in `include/settings_sync.h`:

- **`struct settings_sync_record`** (0x200 bytes) - the actual
  checksummed settings payload. `self->field_8c`/`field_90`
  (`pause_options_screen.h`) are two instances of this. Per-row
  "selected" flags at +0x1f4, two fixed marker bytes at +0x1f8/+0x1f9
  (`sub_8002C84` stamps `'C'`/`0x12`; `sub_8002B94`, still raw, reads
  the `0x1f9` high nibble as a protocol-version-ish value elsewhere),
  a bitmask at +0x1fa (`sub_8002CF4`/`sub_8002D0C`/`sub_8002D28`), and
  a running additive checksum at +0x1fc (`sub_8002B70`/`sub_8002B44`,
  both still raw - a plain word-sum loop over the first 0x1f8 bytes).
- **`struct settings_sync_pump`** (0x220 bytes) - a transient SIO
  send/receive envelope wrapping a `settings_sync_record` copy.
  Allocated per "connecting..." spinner-dialog session
  (`sub_8003B40`, `src/graphics/settings_menu.c`, parked) via
  `sub_8002FCC`/`sub_8002FD8` and torn down with it.
  `tmpl`/`cursor` stream a record's bytes out to the SIO session's
  ring buffer (`sub_8002D44`); `data`/`writePtr` receive the remote
  side's copy from its own ring buffer (`sub_8002E20`).

`sub_8002D44`/`sub_8002E20` both drain/fill through a still
partially-uncharacterized SIO session object (`*gUnknown_03000804`) -
a fixed 0x80-byte ring per direction, wrapping at index 0x7f, with a
separate write-position/pending-count field pair per ring. The RX
side additionally indexes a per-player sub-record at
`session + playerIndex*0xc8` (matching `docs/rom_map.md`'s "resets 4
per-player communication slot buffers" note about `sub_8001DB4` in
the neighbouring SIO/link-cable cluster) - the exact field layout of
that per-player sub-record beyond the two offsets these two functions
touch (+0x10c data base, +0x18c avail count, +0x190 ring position)
isn't pinned down yet.

`sub_8002EFC` polls this pump once per frame: if the session isn't
"connected" (byte +7), it just tracks reset/completion of the pump
and returns 1/0; once connected, it picks a role from the session's
+0x3fc field, pumps RX/TX at most once each per call, and once both
sides report complete, waits ~30 extra polls before finally
settling at 0.

## The spinner dialog's shared struct shape

`sub_800306C` (constructor) and `sub_800312C` (destructor) turned out
to double as the constructor/destructor for **both** the composite
pause/options screen *and* the "connecting..." spinner dialog
(`sub_8004D4C`/`sub_8004D20`, `src/graphics/settings_menu4.c`,
already matched) - both allocate a `struct pause_options_screen`
(0xe4 bytes) and build the exact same `field_8c`/`field_90` pair, and
`sub_800300C`'s blocking modal input loop (used only by the spinner
dialog) drives the same `sub_80031E4` dispatcher/`sub_8004BD0` state
machine the real screen's per-frame update uses. `pause_options_screen.h`
picked up two more named fields from this chunk: `field_20` (a
"result ready" poll flag, read by `sub_800300C`) and `currentStats`
(a single scratch `settings_row_stats`, at 0x28-0x3b, right before the
`rowStats[4]` array).

## Parked (6, `NON_MATCHING`)

All six hit the same unresolved gcc-2.9 scratch-register
nondeterminism this project has documented at length already
(`sub_8006600`/`src/graphics/oam_count.c`,
`sub_80049CC`/`src/graphics/settings_menu.c`) - every load/store,
branch and call is semantically confirmed, real bytes stay in the
`asm/code_3_1_10_3_*.s` fragments listed below wrapped
`.if NON_MATCHING == 0`, C reconstructions stay in-tree under
`#if NON_MATCHING`:

- **`sub_8002D0C`** (`asm/code_3_1_10_3_2d0c.s`, C in
  `src/graphics/settings_menu8.c`) - the bitmask-clear accessor. The
  ROM keeps a redundant copy of the bit-cleared result through a
  second register (load -> `bics` -> copy -> store) that no variation
  tried here (separate result variable, register pins on the
  loaded/result values in every combination, a pointer-typed field
  access) reproduces - they all either collapse back to the
  single-register form or spill through an unrelated extra register.
  Its OR counterpart, `sub_8002D28`, matched cleanly with the same
  `register ... asm("r3")`/`asm("r1")` pins - only the AND-NOT shape
  resists.
- **`sub_8002D44`**, **`sub_8002E20`**, **`sub_8002EFC`**
  (`asm/code_3_1_10_3_2d44.s`, C in `src/graphics/settings_menu8a2.c`)
  - the SIO send/receive pump trio. Every technique this project
  documents was tried (down-counting `for`/`do-while` loops matching
  the ROM's `n != -1` sentinel idiom, swapping the wrap/non-wrap
  branch order to match the ROM's fallthrough side, explicit
  register-variable pins on `self`/`remaining`/`session` and a `p`
  alias for the final field-store pair) - `sub_8002D44` in particular
  landed at the ROM's exact byte *size* (220 bytes) after all that,
  but never byte-for-byte content; `sub_8002E20`/`sub_8002EFC` didn't
  converge on size either.
- **`sub_8003698`** (`asm/code_3_1_10_3_3698.s`, C in
  `src/graphics/settings_menu8b.c`) - the shared "commit or refresh
  row" step nine of this chunk's input handlers call into. The
  `handleAddr`-cached-pointer pattern that fixed the same class of gap
  in `sub_800306C`/`sub_800312C`/`sub_80031E4` below got this one to
  the ROM's exact byte size too, but not exact content.
- **`sub_8003A60`** (`asm/code_3_1_10_3_3a60.s`, C in
  `src/graphics/settings_menu8c.c`) - the state-select label list
  draw. Same measure-then-draw icon shape as `sub_80049CC`
  (`src/graphics/settings_menu.c`, already parked) - a cached
  `&gUnknown_030012DC` address pin (the same technique that worked for
  the constructor/destructor/dispatcher below) collided with a
  compiler-hoisted constant landing in the same register as the
  pinned loop counter, corrupting it; further pin juggling didn't
  converge before this was parked instead of risking a subtly wrong
  "matched" function.

## Matched (19)

- `sub_8002C84`, `sub_8002CE8`, `sub_8002CF4` - record init (DMA16
  zero-fill + marker stamp + checksum refresh) and two flag-test
  accessors (`src/graphics/settings_menu8.c`).
- `sub_8002D28` - the bitmask-set accessor, needed an explicit
  `asm(".align 2, 0")` after it: GAS's default Thumb padding filler is
  the `mov r8, r8` NOP (`0x46c0`), but the ROM pads this function's
  tail with a zero halfword instead (`src/graphics/settings_menu8a2.c`).
- `sub_8002FCC`, `sub_8002FD4`, `sub_8002FD8` - the pump's
  template-attach/data-pointer/reset accessors.
- `sub_800300C` - the spinner dialog's blocking modal input loop.
  Needed `gUnknown_030007E0` modelled as a `{u16 held; u16 pressed;}`
  pair (reading `.pressed` directly) rather than
  `*(u16*)((u8*)&gUnknown_030007E0 + 2)`, which the compiler folds
  into the linker-relocated constant instead of the ROM's runtime
  `ldrh r1, [r0, #2]`; also needed the loop's `self = *selfAddr`
  dereference inlined at each call site (`sub_8004BD0(*selfAddr)` etc.
  instead of assigning to a local first) to match the ROM's direct
  `ldr r0, [r4]` reload pattern, and a call site for the real
  `sub_8004A64(void)` (matched elsewhere, in
  `src/graphics/settings_menu3.c`) that still passes `self` in r0 -
  the ROM's caller sets it up even though the callee never reads it.
- `sub_800306C`, `sub_800312C`, `sub_80031E4` - the shared
  constructor/destructor/per-frame-dispatcher for both the composite
  screen and the spinner dialog. All three needed a cached
  `void **fieldAddr`/`c`/`b`/`a`-style pointer (matching the ROM's own
  "load the address once, dereference fresh each time" idiom) instead
  of repeatedly re-reading `self->field`, plus - for `sub_800312C`
  specifically - leaving `self`/`flags` as plain, unrenamed function
  parameters (any intermediate local copy, however declared, produced
  a redundant register-to-register copy the ROM doesn't have).
  `sub_80031E4`'s state-9/state-7 dispatch table also had the two
  numerically-adjacent-looking handlers backwards in the very first
  reconstruction attempt (`sub_800376C`/`sub_800397C` swapped) - the
  ROM's `switch` case bodies are laid out in a source order that
  doesn't match ascending case-label order (case 9's body physically
  precedes case 7's), which the C reconstruction now mirrors.
- `sub_80032E8`, `sub_80033E8`, `sub_80034BC`, `sub_80035C0`,
  `sub_800376C`, `sub_8003824`, `sub_80038D0`, `sub_800397C` - the
  per-`state` input handlers `sub_80031E4` dispatches to. Several
  needed their confirm-bit test written as two separate `if`
  statements sharing a `goto confirm` label rather than a single
  `(flags & 1) || (flags & 8)` - the ROM never merges those two bit
  tests into one mask-and-compare the way gcc's optimizer does when
  given the `||` form directly.

## Struct/header changes

- `include/settings_sync.h` (new) - `struct settings_sync_record`,
  `struct settings_sync_pump`, shared across
  `settings_menu8.c`/`settings_menu8a2.c`/`settings_menu8b.c`.
- `include/pause_options_screen.h` - added `field_8` (u8, "input loop
  should exit" flag), `field_20` (u8, "result ready" flag),
  `currentStats` (a `settings_row_stats` at 0x28-0x3b, replacing
  `unused_28`), and split `unused_1e[6]` to carve out `field_20`;
  `field_10` changed from `u32` to `s32` (several handlers do signed
  wrap-inc/decrement on it, matching the ROM's `bge`/`ble` branches).

See `docs/status/overlay_ui.md` for the per-function matched/parked
lists.
