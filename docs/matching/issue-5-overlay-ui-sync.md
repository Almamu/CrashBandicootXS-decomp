# Issue #5: 0x08002C84-0x08003B40 (overlay_ui)

25 functions right before the already-matched/parked
`0x08003B40-0x08004CB4` "connecting..." spinner-dialog cluster
(issue #7). 19 matched, 6 parked. All addresses confirmed via a full
clean `make compare` ("La suma coincide").

## The settings-sync protocol

The chunk's first cluster (`ResetSaveData`-`PollSaveTransfer`) turned out to
be a multiplayer settings-sync protocol built on two small object
types, both now named and documented in `include/settings_sync.h`:

- **`struct save_data`** (0x200 bytes) - the actual
  checksummed settings payload. `self->field_8c`/`field_90`
  (`save_menu.h`) are two instances of this. Per-row
  "selected" flags at +0x1f4, two fixed marker bytes at +0x1f8/+0x1f9
  (`ResetSaveData` stamps `'C'`/`0x12`; `GetSaveGameId`, still raw, reads
  the `0x1f9` high nibble as a protocol-version-ish value elsewhere),
  a bitmask at +0x1fa (`TestSaveFlags`/`ClearSaveFlags`/`SetSaveFlags`), and
  a running additive checksum at +0x1fc (`UpdateSaveChecksum`/`CheckSaveChecksum`,
  both still raw - a plain word-sum loop over the first 0x1f8 bytes).
- **`struct settings_sync_pump`** (0x220 bytes) - a transient SIO
  send/receive envelope wrapping a `save_data` copy.
  Allocated per "connecting..." spinner-dialog session
  (`LinkExchangeSaveData`, `src/save/save_menu_draw.c`, parked) via
  `SetSaveTransferRecord`/`ResetSaveTransfer` and torn down with it.
  `tmpl`/`cursor` stream a record's bytes out to the SIO session's
  ring buffer (`SendSaveTransferChunk`); `data`/`writePtr` receive the remote
  side's copy from its own ring buffer (`ReceiveSaveTransferChunk`).

`SendSaveTransferChunk`/`ReceiveSaveTransferChunk` both drain/fill through a still
partially-uncharacterized SIO session object (`*gLinkSession`) -
a fixed 0x80-byte ring per direction, wrapping at index 0x7f, with a
separate write-position/pending-count field pair per ring. The RX
side additionally indexes a per-player sub-record at
`session + playerIndex*0xc8` (matching `docs/rom_map.md`'s "resets 4
per-player communication slot buffers" note about `ResetLinkSessionState` in
the neighbouring SIO/link-cable cluster) - the exact field layout of
that per-player sub-record beyond the two offsets these two functions
touch (+0x10c data base, +0x18c avail count, +0x190 ring position)
isn't pinned down yet.

`PollSaveTransfer` polls this pump once per frame: if the session isn't
"connected" (byte +7), it just tracks reset/completion of the pump
and returns 1/0; once connected, it picks a role from the session's
+0x3fc field, pumps RX/TX at most once each per call, and once both
sides report complete, waits ~30 extra polls before finally
settling at 0.

## The spinner dialog's shared struct shape

`InitSaveMenu` (constructor) and `DestroySaveMenu` (destructor) turned out
to double as the constructor/destructor for **both** the composite
pause/options screen *and* the "connecting..." spinner dialog
(`OpenSaveMenu`/`CloseSaveMenu`, `src/save/save_menu.c`,
already matched) - both allocate a `struct save_menu`
(0xe4 bytes) and build the exact same `field_8c`/`field_90` pair, and
`RunSaveMenu`'s blocking modal input loop (used only by the spinner
dialog) drives the same `SaveMenuInput` dispatcher/`DrawSaveMenu` state
machine the real screen's per-frame update uses. `save_menu.h`
picked up two more named fields from this chunk: `field_20` (a
"result ready" poll flag, read by `RunSaveMenu`) and `currentStats`
(a single scratch `settings_row_stats`, at 0x28-0x3b, right before the
`rowStats[4]` array).

## Parked (6, `NON_MATCHING`)

All six hit the same unresolved gcc-2.9 scratch-register
nondeterminism this project has documented at length already
(`DrawPowerDialog`/`src/menus/power_dialog_draw.c`,
`DrawSaveMenuTitle`/`src/save/save_menu_draw.c`) - every load/store,
branch and call is semantically confirmed, real bytes stay in the
`asm/code_3_1_10_3_*.s` fragments listed below wrapped
`.if NON_MATCHING == 0`, C reconstructions stay in-tree under
`#if NON_MATCHING`:

- **`ClearSaveFlags`** (`asm/code_3_1_10_3_2d0c.s`, C in
  `src/save/save_data.c`) - the bitmask-clear accessor. The
  ROM keeps a redundant copy of the bit-cleared result through a
  second register (load -> `bics` -> copy -> store) that no variation
  tried here (separate result variable, register pins on the
  loaded/result values in every combination, a pointer-typed field
  access) reproduces - they all either collapse back to the
  single-register form or spill through an unrelated extra register.
  Its OR counterpart, `SetSaveFlags`, matched cleanly with the same
  `register ... asm("r3")`/`asm("r1")` pins - only the AND-NOT shape
  resists.
- **`SendSaveTransferChunk`**, **`ReceiveSaveTransferChunk`**, **`PollSaveTransfer`**
  (`asm/code_3_1_10_3_2d44.s`, C in `src/save/save_transfer.c`)
  - the SIO send/receive pump trio. Every technique this project
  documents was tried (down-counting `for`/`do-while` loops matching
  the ROM's `n != -1` sentinel idiom, swapping the wrap/non-wrap
  branch order to match the ROM's fallthrough side, explicit
  register-variable pins on `self`/`remaining`/`session` and a `p`
  alias for the final field-store pair) - `SendSaveTransferChunk` in particular
  landed at the ROM's exact byte *size* (220 bytes) after all that,
  but never byte-for-byte content; `ReceiveSaveTransferChunk`/`PollSaveTransfer` didn't
  converge on size either.
- **`SaveGameToSlot`** (`asm/code_3_1_10_3_3698.s`, C in
  `src/save/save_menu_input.c`) - the shared "commit or refresh
  row" step nine of this chunk's input handlers call into. The
  `handleAddr`-cached-pointer pattern that fixed the same class of gap
  in `InitSaveMenu`/`DestroySaveMenu`/`SaveMenuInput` below got this one to
  the ROM's exact byte size too, but not exact content.
- **`DrawSaveMenuMain`** (`asm/code_3_1_10_3_3a60.s`, C in
  `src/save/save_menu_input.c`) - the state-select label list
  draw. Same measure-then-draw icon shape as `DrawSaveMenuTitle`
  (`src/save/save_menu_draw.c`, already parked) - a cached
  `&gSmallFont` address pin (the same technique that worked for
  the constructor/destructor/dispatcher below) collided with a
  compiler-hoisted constant landing in the same register as the
  pinned loop counter, corrupting it; further pin juggling didn't
  converge before this was parked instead of risking a subtly wrong
  "matched" function.

## Matched (19)

- `ResetSaveData`, `IsSaveSlotEmpty`, `TestSaveFlags` - record init (DMA16
  zero-fill + marker stamp + checksum refresh) and two flag-test
  accessors (`src/save/save_data.c`).
- `SetSaveFlags` - the bitmask-set accessor, needed an explicit
  `asm(".align 2, 0")` after it: GAS's default Thumb padding filler is
  the `mov r8, r8` NOP (`0x46c0`), but the ROM pads this function's
  tail with a zero halfword instead (`src/save/save_transfer.c`).
- `SetSaveTransferRecord`, `GetSaveTransferData`, `ResetSaveTransfer` - the pump's
  template-attach/data-pointer/reset accessors.
- `RunSaveMenu` - the spinner dialog's blocking modal input loop.
  Needed `gKeys` modelled as a `{u16 held; u16 pressed;}`
  pair (reading `.pressed` directly) rather than
  `*(u16*)((u8*)&gKeys + 2)`, which the compiler folds
  into the linker-relocated constant instead of the ROM's runtime
  `ldrh r1, [r0, #2]`; also needed the loop's `self = *selfAddr`
  dereference inlined at each call site (`DrawSaveMenu(*selfAddr)` etc.
  instead of assigning to a local first) to match the ROM's direct
  `ldr r0, [r4]` reload pattern, and a call site for the real
  `EndLinkSaveTransfer(void)` (matched elsewhere, in
  `src/save/save_menu_ui.c`) that still passes `self` in r0 -
  the ROM's caller sets it up even though the callee never reads it.
- `InitSaveMenu`, `DestroySaveMenu`, `SaveMenuInput` - the shared
  constructor/destructor/per-frame-dispatcher for both the composite
  screen and the spinner dialog. All three needed a cached
  `void **fieldAddr`/`c`/`b`/`a`-style pointer (matching the ROM's own
  "load the address once, dereference fresh each time" idiom) instead
  of repeatedly re-reading `self->field`, plus - for `DestroySaveMenu`
  specifically - leaving `self`/`flags` as plain, unrenamed function
  parameters (any intermediate local copy, however declared, produced
  a redundant register-to-register copy the ROM doesn't have).
  `SaveMenuInput`'s state-9/state-7 dispatch table also had the two
  numerically-adjacent-looking handlers backwards in the very first
  reconstruction attempt (`SaveMenuOverwriteInput`/`SaveMenuConfirmDeleteInput` swapped) - the
  ROM's `switch` case bodies are laid out in a source order that
  doesn't match ascending case-label order (case 9's body physically
  precedes case 7's), which the C reconstruction now mirrors.
- `SaveMenuMainInput`, `SaveMenuMoveCursor`, `SaveMenuLoadInput`, `SaveMenuLinkInput`,
  `SaveMenuOverwriteInput`, `SaveMenuSaveInput`, `SaveMenuDeleteInput`, `SaveMenuConfirmDeleteInput` - the
  per-`state` input handlers `SaveMenuInput` dispatches to. Several
  needed their confirm-bit test written as two separate `if`
  statements sharing a `goto confirm` label rather than a single
  `(flags & 1) || (flags & 8)` - the ROM never merges those two bit
  tests into one mask-and-compare the way gcc's optimizer does when
  given the `||` form directly.

## Struct/header changes

- `include/settings_sync.h` (new) - `struct save_data`,
  `struct settings_sync_pump`, shared across
  `save_data.c`/`save_transfer.c`/`save_menu_input.c`.
- `include/save_menu.h` - added `field_8` (u8, "input loop
  should exit" flag), `field_20` (u8, "result ready" flag),
  `currentStats` (a `settings_row_stats` at 0x28-0x3b, replacing
  `unused_28`), and split `unused_1e[6]` to carve out `field_20`;
  `field_10` changed from `u32` to `s32` (several handlers do signed
  wrap-inc/decrement on it, matching the ROM's `bge`/`ble` branches).

See `docs/status/overlay_ui.md` for the per-function matched/parked
lists.

## Second pass

Issue #5 was reopened after an earlier PR closed it with 6 functions
still parked. This pass matched 4 of those 6 (`ClearSaveFlags`,
`SaveGameToSlot`, `DrawSaveMenuMain`, `PollSaveTransfer`); `SendSaveTransferChunk`/
`ReceiveSaveTransferChunk` stay parked for a reason explained below that no C-level
technique gets around.

- **`ClearSaveFlags`** (bitmask-clear accessor, `src/graphics/
  save_data.c`) - the redundant register-to-register copy the
  first pass's every plain-C attempt collapsed away turned out to be
  forceable with a single `asm volatile("add %0, %1, #0" : "=r"(v) :
  "r"(loaded))` between the `bics`-equivalent computation and the
  store - the same "force the ROM's own extra copy with an inline-asm
  no-op move" trick `matching_decomp_register_pinning` already
  documents for other functions, just not tried here yet. Also
  surfaced a small toolchain quirk worth remembering: this exact
  `arm-none-eabi-as` build rejects the differing-register 3-operand
  immediate-`#0` form (`adds r1, r3, #0`) outright ("instruction not
  supported in Thumb16 mode"), even though it's a valid Thumb1
  encoding and openly appears in the ROM's own objdump - the fix is
  always spelling these as `add` (no `s` suffix) in this project's own
  hand-written asm text, matching what agbcc's own `-fhex-asm` output
  already does for compiler-generated code. The same applies to
  `lsls`/`lsrs`/`subs`/`asrs`/`rsbs` (the last needs `neg`, not
  `rsb`/`rsbs`, for the zero-minus-register idiom) throughout this
  pass's other inline-asm blocks.

- **`SaveGameToSlot`** (shared "commit or refresh row" step, `src/
  save/save_menu_input.c`) - the previous pass got this to the
  ROM's exact byte *size* with a single cached `handleAddr`, but the
  real gap was argument-evaluation order: `MemCopy32(buf + 0x70,
  PackSaveData(*c0Addr), 0x68)` lets this compiler compute `buf + 0x70`
  (the first argument) before calling `PackSaveData` for the second,
  where the ROM's own build evaluates the call first and only computes
  the pointer argument afterward, as part of the call's own register
  setup. Forcing that order just needs the call's result captured into
  a named local first (`void *result = PackSaveData(*c0Addr);
  MemCopy32(buf + 0x70, result, 0x68);`) rather than nesting the call
  directly in the outer call's argument list. Separately, `handleAddr`
  genuinely does need recomputing a second time (`&self->field_8c`
  taken again, matching the ROM's second `adds r4, r7, #0`/`adds r4,
  #0x8c` pair) rather than reusing the first one - a second named local
  (`handleAddr2`) rather than reusing `handleAddr` reproduces that.

- **`DrawSaveMenuMain`** (state-select label list draw, `src/graphics/
  save_menu_input.c`) - this one resisted every plain-C register-pin
  combination tried across both passes: a loop-invariant constant
  (`mgr->record`'s `0x130` field offset, and separately
  `&gSaveMenuOptions[0]`) kept getting hoisted out of the loop into
  whichever register looked free at that point, landing on `r7` (the
  pinned loop counter) and silently corrupting it, because this
  compiler's constant-hoisting pass is blind to a raw-asm loop body's
  internal control flow - and even sidestepping that, the ROM's exact
  choice of scratch register per access still differed (e.g. reusing
  `r1`'s already-computed `0x114` via a plain `+0x1c` for the second
  `mgr->record` fetch, instead of resynthesizing `0x98<<1` from
  scratch). Matched in the end by transcribing the ROM's own
  instruction sequence directly into two `asm volatile` blocks per
  iteration (the highlight-branch dispatch, and the
  measure-then-draw pair), leaving only the loop's own `i`/`y`
  compare-and-increment in plain C - this compiler only includes a
  register in a function's automatic callee-save push/pop when *it*
  allocated that register for a real (non-asm) value, so `i`/`y`
  specifically have to stay genuine C locals (not register-pinned
  operand-only variables) for the prologue to come out right. Two
  smaller gaps besides: `GetSaveMenuBlinkPalette`'s return value needs its
  ROM-visible `u8` truncation (`lsls`/`lsrs #0x18`) spelled out
  explicitly, since this compiler doesn't reproduce it from the C
  return type alone; and the final call's stack-passed `u8 flag`
  argument needs its own asm too, because Thumb1 has no
  stack-pointer-relative byte store (`strb`) - the ROM computes `mov
  r1, sp` first - while this compiler always emits a direct word-sized
  `str r0, [sp]` for a stack-passed byte argument. Getting the
  compiler to still reserve that argument's 4-byte stack slot needs
  taking its address as a dummy, otherwise-unused asm input operand.
  Also needed a manual `.pool` directive right after the highlight
  branch's own unconditional jump, since `&gSmallFont`'s
  literal-pool placement is controlled by this same compiler pass that
  can't see inside the raw-asm loop body - without it, the literal
  gets deferred all the way to the function's tail instead of landing
  in the ROM's early slot.

- **`PollSaveTransfer`** (SIO pump per-frame poll, `src/graphics/
  save_transfer_poll.c`, new file) - matched the same way as
  `DrawSaveMenuMain` above, as one big `asm volatile` transcription of the
  ROM's instructions (this one genuinely doesn't need any real C
  control flow at all, since it has no loop). The output register
  matters here since the function returns a value: an unconstrained
  `"=r"(result)` let this compiler pick any register (it picked `r8`
  in one attempt, producing an outright illegal `movs r8, #1`), so
  `result` has to be a `register s32 result asm("r0")` pin instead,
  with each of the function's four return-value-setting points writing
  through `%0` (which resolves to that same `r0`) instead of a literal
  `r0`, and no separate closing move needed since the ROM's own
  `pop {r4, r5, r6}; pop {r1}; bx r1` never has one either. Needed its
  own new file (`save_transfer_poll.c`) rather than joining
  `save_menu_input.c`/`save_transfer.c` because its real address
  (`0x08002EFC`) sits between the still-parked `SendSaveTransferChunk`/
  `ReceiveSaveTransferChunk` (staying in `asm/code_3_1_10_3_2d44.s`) and
  `save_menu_input.c`'s first function - the usual "one `.c` file per
  contiguous ROM region" rule from `docs/workflow.md`.

- **`SendSaveTransferChunk`/`ReceiveSaveTransferChunk`** (SIO pump TX/RX drain-fill,
  `src/save/save_transfer.c`) - still parked. Both need `r7` as
  a genuinely allocated scratch register (matching the ROM's own
  `sendLen`/sentinel usage there), and this exact agbcc build *never*
  includes `r7` in a function's automatic callee-save push/pop,
  regardless of how that register gets used - confirmed by direct,
  minimal reproduction outside this file entirely: a function that
  only ever touches `r7` via a plain `asm` clobber never gets it
  pushed; neither does one with an explicit `register s32 x
  asm("r7")` pin used as a real cross-statement value (even read back
  through a genuine function call afterward); nor does a fully
  unregistered local forced into `r7` by exhausting every other
  register under heavy pinning pressure - every other clobbered/pinned
  register (`r4`-`r6`, `r8`, `r9`) gets saved and restored correctly
  in the exact same setup, just never `r7`. This is the same
  categorical limitation already documented project-wide for
  `CheckSpritePickup` (`actor_part2.c`), `MovePolarAkuAku` (`actor_part62.c`,
  `docs/matching/issue-54-actor-d3a8.md`) and others (see
  `matching_decomp_register_pinning` memory point 10) - parked rather
  than keep chasing a compiler bug with no known workaround. The
  third function of the trio, `PollSaveTransfer`, doesn't touch `r7` at
  all and has been matched (see above).

## NAKED-transcription pass

`SendSaveTransferChunk`/`ReceiveSaveTransferChunk` above were the only two functions left
parked anywhere in this issue's scope, both blocked on the exact same
categorical gcc-2.9 limitation described above: neither can get `r7`
back into the automatic callee-save push/pop list from plain C, no
matter how it's pinned. Rather than keep chasing that dead end, both
were converted to `NAKED` and their bodies rewritten as a single
`asm(...)` block transcribing the ROM's own disassembly
instruction-for-instruction - the same escape hatch already
established and proven in this project (see
`docs/matching/issue-4-sio-settings-sync.md`'s "NAKED transcription,
byte-verified" section, and the smaller worked examples in
`lib/libgcc/lib1funcs.s`'s `__div0` and `lib/gax/src/gax_swi.c`'s
`GaxHuffUnComp`). Both functions were already fully understood
semantically - the parked C reconstruction that used to sit in
`src/save/save_transfer.c` (now replaced) and the walkthrough
above are that derivation - so this was a pure transcription pass, not
a fresh reverse-engineering one.

Mechanically this followed the same recipe as issue #4's NAKED pass:
the disassembler's unified-syntax mnemonics were translated to the
divided syntax `arm-none-eabi-as`'s default mode expects for
hand-written text (`adds`→`add`, `movs`→`mov`, `muls`→`mul`,
`lsls`→`lsl`, `subs`→`sub`, `rsbs rX, rX, #0`→`neg rX, rX`), and every
real `_08XXXXXX:` ROM address label was renumbered to a GNU-as local
numeric label (`1:`...`11:` for `SendSaveTransferChunk`, `1:`...`8:` for
`ReceiveSaveTransferChunk`, referenced `Nf`/`Nb`), since a `NAKED` function's asm
block can't reference the real ROM address as a label. Both functions
keep their original prologue/epilogue written out literally -
`SendSaveTransferChunk`'s `push {r4, r5, r6, r7, lr}` / `mov r7, sb` / `mov r6,
r8` / `push {r6, r7}` pair (and the matching `pop {r3, r4}` / `mov r8,
r3` / `mov sb, r4` / `pop {r4, r5, r6, r7}` / `pop {r0}` / `bx r0`
epilogue) and `ReceiveSaveTransferChunk`'s smaller `mov r7, r8` / `push {r7}` pair -
exactly the callee-save sequence gcc could never reproduce from C for
these two. `SendSaveTransferChunk` also keeps both of the ROM's separate
`gLinkSession` literal-pool copies (labels `3:`/`11:`) rather than
merging them into one, matching the ROM's own pool placement byte for
byte.

Verified via the standard loop: an isolated compile+assemble of
`src/save/save_transfer.c` was first disassembled and eyeballed
instruction-by-instruction against the original ROM disassembly (still
just a diagnostic, per `docs/workflow.md` step 3 - not proof), then the
whole `asm/code_3_1_10_3_2d44.s` file (which held nothing but these two
functions, now fully extracted) was deleted and `ldscript.txt`'s
`code_3_1_10_3_2d44.o` line removed, and a full clean `rm -rf build &&
make NON_MATCHING=1 report` followed by clean `rm -rf build
crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make
compare` both passed (`sha1sum`'s "La suma coincide"). Both functions
are now matched, closing out the r7 limitation for this issue's scope.

## Later pass: `ReceiveSaveTransferChunk` matched

The last-eight NAKED retry closed `ReceiveSaveTransferChunk` as real C (see
[last-eight-naked-retry.md](last-eight-naked-retry.md)). The 14-halfword
register permutation left by the last-seven pass came from the channel
pointer's copy preference for r2 (it was an `"+r"` escape of `c + 0x108`
with `c` pinned to r2): the wrap loop's ring pointer inherited that
preference, which pushed `old` out of r2. Building the channel pointer
with `asm volatile("" : "=r"(ch) : "r"(c + 0x108))` drops the
preference, and pinning the wrap loop's count pointer to r1 (set after
the zero-trip test, with the loop written as `if` + `do`/`while`)
leaves r2 for `old`. `n` no longer needs extra references.
