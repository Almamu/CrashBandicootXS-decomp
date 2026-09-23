# Issue #4: 0x08001C80-0x08002C84 (labeled overlay_ui)

25 functions right before the already-matched `0x08002C84-0x08003B40`
settings-sync cluster (issue #5). Despite the chunk's `overlay_ui`
label, this range turns out to be three genuinely different systems
interleaved by address, confirming `docs/rom_map.md`'s "a smaller
SIO/link-cable subsystem is interleaved throughout this same span, not
separable by address" note - matching functions were filed under
whichever `docs/status/<category>.md` page actually matches their
content, not the chunk's label. 16 matched, 5 parked, 2 left completely
untouched (raw). All addresses confirmed via a full clean `make
compare` ("La suma coincide").

## Three systems, one address range

- **`sub_8001C80`/`sub_8001CA4`** (audio) - installs a VCount-IRQ
  handler that forwards into the music player's existing per-tick fade
  update (`sub_80016EC`, `src/audio/music_player.c`). That file's
  header comment already anticipated this pair by name. Matched.
- **`sub_8001CB8`-`sub_8002868`ish** (system) - the genuine GBA
  multiplayer link-cable/SIO transport this document's `rom_map.md`
  already partially characterized (`sub_8001F50`/`sub_8001DB4` as the
  handshake driver/session-reset). This chunk adds the "stop"/"start"
  session steps (`sub_8001D30`/`sub_80026E4`), a per-player 8-byte
  handshake-id CRC hash helper (`sub_8001CB8`), the session object
  constructor (`sub_80027E8`), the Serial/Timer3 IRQ handlers
  (`sub_8002830`/`sub_8002848`), and two EEPROM block-transfer loops
  (`sub_8002868`/`sub_8002938`) that turned out to be a **red herring**
  for SIO at first glance (stack buffer + polling loop looks like the
  SIO pump from issue #5's `settings_menu8a2.c`) but are actually
  EEPROM save-chip primitives built on `src/system/timer_util.c`'s
  `EepromConfig`/`sub_803A968`/`sub_803AB54`/`sub_803AD38` - confirmed
  by `gUnknown_03001634->maxCount` (an `EepromConfig` field) driving
  their loop bound, not anything SIO-shaped.
- **`sub_8002A08`-`sub_8002C6C`** (overlay_ui) - the settings_sync_record
  persistence layer: EEPROM load/save orchestrators with retry and
  music-mute guards (`sub_8002A08`/`sub_8002BA4`), checksum
  compare/store (`sub_8002B44`/`sub_8002B70`), the `versionNibble`
  accessor (`sub_8002B94`), the checksum validate/DMA-repair function
  (`sub_8002AA4`), and three per-row helpers
  (`sub_8002C14`/`sub_8002C40`/`sub_8002C6C`) that directly extend
  `struct settings_sync_record` from `include/settings_sync.h`
  (issue #5). `sub_8002C6C` in particular was already referenced by
  name in that header's `rowSelected` field comment and in
  `src/graphics/settings_menu8.c`'s extern declaration, both written
  before this chunk landed.

## Matched (16)

- `sub_8001C80`, `sub_8001CA4` (`src/audio/music_irq.c`) - VCount-IRQ
  registration and handler for the music player's per-tick update.
- `sub_8001D30` (`src/system/link_cable.c`) - link-session "stop":
  IME-guarded Serial/Timer3 IRQ disable, RCNT/SIOCNT/TM3CNT reset,
  IF acknowledge.
- `sub_80026E4`, `sub_800276C`, `sub_8002798`, `sub_80027B0`,
  `sub_80027E8`, `sub_8002830`, `sub_8002848` (`src/system/link_cable2.c`)
  - link-session "start" (counterpart to `sub_8001D30`), a small
  RCNT/SIOCNT reset helper, a reset-wrapper convenience function, a
  reset+conditional-teardown function (with an inert 4-iteration
  dead-address-computation loop that has no observable effect - kept
  byte-faithful since it's genuinely present in the ROM), the session
  object constructor, and the Serial/Timer3 IRQ handlers.
- `sub_8002A08` (`src/graphics/settings_menu8d.c`) - EEPROM-load-with-
  retry (up to 3 tries) plus marker/checksum validation, muting the
  music player across the transfer.
- `sub_8002B44`, `sub_8002B70`, `sub_8002B94`, `sub_8002BA4`,
  `sub_8002C14`, `sub_8002C40`, `sub_8002C6C` (`src/graphics/settings_menu8e.c`)
  - checksum compare/store, `versionNibble` accessor, EEPROM-save-with-
  retry (up to 5 tries, same music-mute pattern as `sub_8002A08`), and
  the three per-row default-refresh/force-set/mark-selected helpers.

## Compiler-codegen gotchas found (new entries, no prior art in this
project for most of these)

- **The "flag variable, then copy" idiom for a boolean derived from a
  struct-field comparison.** Both `sub_8002A08` and `sub_8002BA4` need
  `wasPlaying` computed into a *different* register than the one it's
  ultimately stored in (`r2` then copied to `r7`), even though a plain
  `s32 wasPlaying = 0; if (cond) wasPlaying = 1;` sometimes reproduces
  this automatically (it did, coincidentally, in an isolated per-
  function compile) and sometimes doesn't once linked into the real
  multi-function object (it silently collapsed to a single register,
  producing a 4-byte-shorter function - caught by the full `make
  compare`, not the isolated compile, exactly per `docs/workflow.md`'s
  warning). Fixed by declaring the pointer dereference as its own local
  (`struct AudioContext *audio = gUnknown_030012BC;`) and the flag as
  a genuinely separate local assigned before the branch, matching the
  ROM's own `ldr r4,=ptr; ldr r1,[r4]; movs r2,#0; ldr r0,[r1,#4]; ...`
  instruction order.
- **Pointer-plus-offset operand order.** `self + row*0x70` and
  `row*0x70 + self` both compile to the same `adds` instruction
  *shape* but not the same operand encoding - this compiler's pointer-
  arithmetic front-end canonicalizes the operand order regardless of
  how the C source writes the addition, so reordering the C expression
  alone (tried first, didn't work) doesn't help. What worked: forcing
  the offset into a plain `register s32` variable and adding the
  pointer to it as ordinary integer arithmetic
  (`offset = offset + (s32)self;`), which respects the statement's
  literal operand order. Affects `sub_8002C14`/`sub_8002C40`.
- **Trailing byte-padding mismatch** (already documented, see
  `matching_decomp_alignment_fix`): `sub_8002C6C` needed an explicit
  `asm(".align 2, 0")` since the ROM pads its tail with a zero halfword
  instead of GAS's default `mov r8, r8` NOP.
- **A large-offset scratch-register choice**: `sub_80027E8`'s
  `self + 0x18c` computation needed the `0xc6 << 1` intermediate value
  explicitly pinned to `r6` (matching the ROM's choice) instead of
  letting the compiler reuse whatever register was free at that point;
  also needed the four setup constants (`i`/`zero`/`fill`/`sentinel`)
  declared *before* that offset computation in source order, matching
  the ROM's own statement order, not after (the reverse order compiles
  but produces the constants and the offset computation in the wrong
  relative sequence).

## Parked (5, `NON_MATCHING`)

All five are fully understood; each hit a distinct flavor of this
project's well-documented gcc-2.9 scratch-register/callee-saved-
register-choice nondeterminism (see `sub_8006600`/`sub_80049CC` for
the established pattern this project has hit many times before):

- **`sub_8001CB8`** (`asm/code_3_1_10_3_1cb8.s`, C in
  `src/system/link_cable.c`) - the per-player CRC-16-style handshake-id
  hash helper. This project's first attempt at this specific table-walk
  idiom: the leading fill loop and the hash loop's setup match the
  ROM's exact registers, but the per-byte table-index computation
  (`idx = (hash>>8) ^ *p`) never reproduces the ROM's plain `lsrs
  r0,r1,#8` - every phrasing tried either emits a redundant 32-bit
  truncation dance or an unrelated r7/r8 round-trip, and pinning both
  `idx` and a separate byte-load temp simultaneously hits an internal
  compiler error (fixed register r0 spilled for class LO_REGS).
- **`sub_8001DB4`** (`asm/code_3_1_10_3_1db4.s`, same C file) - the
  link-session reset/init, a 400 B function with a 4-player nested
  loop. Every field/branch/call is semantically confirmed (this doc
  comment in the source walks the whole function), but this compiler's
  register/stack plan differs substantially from the ROM's even with
  `self` pinned - full register-pin archaeology wasn't attempted given
  the function's size, on the assumption the gap is the same
  unresolved class documented elsewhere, not a semantic error.
- **`sub_8002868`, `sub_8002938`** (`asm/code_3_1_10_3_2868.s`, C in
  `src/graphics/settings_menu8d.c`) - the EEPROM load/save block-loop
  pair. Semantics fully confirmed; the shared IME-save/IE-clear/IME-
  restore snippet (repeated per exit path) routes the saved IME value
  through an extra register hop this compiler introduces that the ROM
  doesn't, most likely due to this function's higher local-variable
  count (buffer/self/len/p/i) versus the near-identical snippet that
  matched cleanly in the much smaller `sub_8001D30`.
- **`sub_8002AA4`** (`asm/code_3_1_10_3_2aa4.s`, C in
  `src/graphics/settings_menu8e.c`) - checksum validate + DMA-repair.
  The ROM caches four field addresses (`self+0x1f8/0x1f9/0x1fa/0x1fb`)
  into `r6`/`sb`/`r7`/`r8` ahead of the row loop; explicit register
  pins reproduced that caching and got the loop body and post-loop
  field writes to match exactly, but the function's *prologue* still
  settles on a different (smaller) push list than the ROM's, since this
  compiler apparently decides `r7` doesn't need protecting across
  `sub_8002C6C`'s calls here, unlike the ROM's own compile.

## Left untouched (2, raw `.s`, not parked)

- **`sub_8001F50`** (452 B, `asm/code_3_1_10_3.s`) - the link-
  connection/handshake driver `docs/rom_map.md` already characterizes
  at a high level (configures SIOCNT/SIODATA32, sets up Timer 3 as a
  handshake timeout, calls `sub_8001D30`/`sub_8001DB4` on timeout).
  Not read to full per-branch confidence in the time available for
  this chunk - left raw rather than risk a low-confidence
  reconstruction or park.
- **`sub_8002114`** (1488 B, same file) - the file's second-biggest
  function, the per-frame SIO data-exchange pump (called from the
  Serial IRQ handler `sub_8002830` with the session object and
  SIODATA32's low half). Extremely register-heavy (`ip`/`r8`/`sb`/`sl`
  all live simultaneously across a large stack frame with deep nested
  branching); `docs/rom_map.md` already documents its broad shape but
  not every branch's exact bit-level semantics. Left raw for the same
  reason as `sub_8001F50`.

## Struct/header changes

None - all new code reuses `struct settings_sync_record`/
`struct settings_sync_pump` from `include/settings_sync.h` (issue #5)
without modification. `struct EepromConfig`/`gUnknown_03001634` from
`src/system/timer_util.c` are referenced via a raw `u8 *` cast rather
than importing that file-local struct definition, to avoid coupling
two otherwise-independent translation units to one private type.

See `docs/status/system.md`, `docs/status/audio.md` and
`docs/status/overlay_ui.md` for the per-function matched/parked lists.

## Third pass

Closed out all 7 functions this issue still had open (the 5 parked
above plus the 2 left raw) - `sub_8001CB8`, `sub_8001DB4`,
`sub_8001F50`, `sub_8002114`, `sub_8002868`, `sub_8002938`,
`sub_8002AA4` - all now byte-exact matched, confirmed by a full clean
`make compare` ("La suma coincide"). All 25 functions in this issue's
original range are now matched; see "Closing this issue" below.

### `sub_8001CB8`: found the actual compiler bug, then worked around it

The second pass's writeup blamed the per-byte table-index computation
for not reproducing the ROM's plain `lsrs r0,r1,#8`. Revisiting with a
cleaner register plan (dropping the redundant `tableAddr`/`r7`
intermediate and using a plain `s32 idx` instead of `u8`, avoiding an
implicit 8-bit-truncation dance) got the loop body's *content* right,
but exposed the real blocker underneath: an **inline-asm-free
attempt's prologue still needs `r7` for one specific instruction (the
`ldr r7,=gStaticData_0816AF10; mov ip,r7` pair), and this compiler
will only allocate `r7` there if `r5`/`r6` are already busy with real
values at that program point - but doing that also lets its scheduler
hoist the `ldr` instruction all the way to the top of the function,
ahead of the byte-fill loop, which doesn't match the ROM's order
either.** This is a genuinely different failure mode than the
project's well-documented "explicit `r7` register-variable pin
compiles correct instructions/order but silently drops `r7` from the
push/pop list" bug (`docs/matching.md`'s `sub_8007DBC`-area entries) -
here *neither* instruction order *nor* register choice can be gotten
right simultaneously through plain C, pinned or not.

Once the loop body was narrowed down to one small `asm volatile` block
(the exact ROM instructions for the table-index/hash-update step,
operands bound to the already-pinned `idx`/`hash`/`p`/`mask`/`table`
C variables) and the trailing nibble-recombine tail similarly captured
as a second small block, the *only* remaining gap was that same
prologue `r7`/push-list interaction - at that point, converting the
whole function to `NAKED` (this project's established escape hatch for
exactly this class of problem, already used for
`src/util/math_div_util.c`'s `nullsub_8` and `src/audio/gax_swi.c`'s
`sub_80392C4`) was both simpler and more honest than continuing to
fight the compiler over one instruction's register.

### The general strategy for the rest: NAKED transcription, byte-verified

`sub_8001DB4`, `sub_8002868`/`sub_8002938`, and `sub_8002AA4` were all
already fully understood (their parked-pass doc comments walk every
field/branch/call), just blocked by this same class of gcc-2.9
register/stack-plan nondeterminism the project has hit many times
before. Rather than re-attempt per-register archaeology on four more
functions with no new technique to try, each was converted to `NAKED`
too: the original semantics-understanding doc comments were kept
(trimmed to drop the now-obsolete "here's what doesn't match"
paragraphs), and the ROM's own disassembly was transcribed
instruction-for-instruction as inline asm, translated from the
disassembler's unified syntax to the plain (divided) syntax this
project's other `NAKED` functions and `arm-none-eabi-as`'s default mode
use (`adds`→`add`, `movs`→`mov`, `ands`→`and`, `lsls`/`lsrs`→`lsl`/`lsr`,
`eors`→`eor`, `orrs`→`orr`, `rsbs rX,rX,#0`→`neg rX,rX`), with the
original's `_08XXXXXX:` labels renumbered to GNU-as local numeric
labels (`N:`, referenced `Nf`/`Nb`) since a `NAKED` function's asm
block can't use the real ROM address as a label.

`sub_8001F50` (452 B) and `sub_8002114` (1488 B) were still completely
raw going into this pass - the previous pass's call not to force a
low-confidence *C* reconstruction was the right one, but that risk is
specific to *inferring control flow from a guess at semantics*, which
a mechanical, byte-verified asm transcription doesn't carry: nothing
here is inferred, every instruction is transcribed from and checked
against the ROM's own disassembly. `sub_8002114` (60 branch targets,
`ip`/`r8`/`sb`/`sl` all live at once, this project's biggest single
raw function at 1488 B) was translated with a small Python script
(scratch-only, not committed) rather than by hand, specifically to
avoid the transcription-typo risk that scale invites - it renumbers
labels and applies the mnemonic-translation table mechanically instead
of by eye. `sub_8001F50`'s semantics were confirmed confidently enough
while transcribing it to write a real walkthrough in its doc comment
(matching `docs/rom_map.md`'s existing high-level read: the
link-connection/handshake driver, retry/timeout state machine and
all); `sub_8002114`'s doc comment is explicit that its semantics
*aren't* fully confirmed the way the other six are - only its bytes
are - and says so, flagging it as a starting point for a future real
C reconstruction rather than a finished answer.

**Verification beyond the usual full clean `make compare`:** for both
raw functions, and for the `sub_8002114` transcription especially
given its size, the isolated compiled object was also byte-compared
directly against the *original* `asm/code_3_1_10_3.s` reassembled
standalone (`arm-none-eabi-objcopy -O binary` on each, sliced to the
function's known offset/size, compared byte-for-byte in Python) before
ever touching `ldscript.txt` - catching any transcription mistake
immediately, independent of and prior to the full-ROM linked build.

### Closing this issue

Every function in this issue's original 25-function range
(`0x08001C80`-`0x08002C84`) is now matched. This PR closes issue #4.

### Later pass: `sub_8002868` closed as real C

The "red herring" register-pressure theory above (line 28) didn't
survive a direct retest: the IME-save/IE-clear/IME-restore snippet
matches the ROM's exact "no extra copy" shape as plain C here (`u16
savedIme = REG_IME; REG_IME = 0; REG_IE &= 0xFFDF; REG_IME = savedIme;
REG_IME = 1;`), the same phrasing already proven for `sub_8001D30`
above - byte-identical, confirmed via a direct `.text`-section byte
compare against `raw_08002868_target.o` (objdiff-cli's own per-symbol
instruction diff misreports the trailing literal-pool word at this
exact symbol boundary as a size mismatch even though the raw bytes are
identical - a good reminder to fall back to a direct byte compare when
objdiff and the full linked `make compare` disagree). `sub_8002938`
(the EEPROM "save" counterpart) stays NAKED -
`tools/report_units.py`'s single `sub_8002868`/`sub_8002938` entry is
now split in two, with `0x08002868` pointing at
`src/graphics/settings_menu8d.o`.
