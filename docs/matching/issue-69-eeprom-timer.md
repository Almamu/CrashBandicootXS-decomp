# Issue #69: `0x0803A944`-`0x0803ADB0` (system) - timer arm/disarm + EEPROM cluster

This issue's remaining 7 functions were a timer arm/disarm/DMA-transfer
trio (3 already parked from an earlier pass) plus a 4-function DMA3
bit-serial GBA EEPROM read/write/verify/retry cluster (left completely
untouched, only a working theory on record). This pass matched 3 of the
7 (`sub_803AA90`, `sub_803ACE0`, `sub_803AD38`), meaningfully narrowed
the register-allocation gap on a 4th (`sub_803AA08`), and turned the
EEPROM cluster's working theory into a fully-confirmed, committed
`NON_MATCHING` reconstruction for 2 more (`sub_803AB54`/`sub_803AC04`).
`sub_803AAD4` is unchanged from the previous pass. **Not closing #69** -
4 of the 7 are still parked, not byte-exact.

## Matched this pass

**`sub_803AA90`** (`src/system/timer_util_aa90.c` - split into its own
file since its real ROM address, `0x0803AA90`, sits between the
still-parked `sub_803AA08` and `sub_803AAD4`, so it isn't adjacent to
`timer_util.c`'s own matched functions; see docs/workflow.md step 4).
The exact inverse of `sub_803AA08`: stops the timer, disables its IRQ
line, restores IME. Two real gaps, both fixed:

1. The ROM briefly repoints `gUnknown_03001628` (the module's timer-
   register-pointer global) at the timer's CNT_H half - storing the
   incremented pointer back to the global - before restoring it
   (`*ptr = 0; ptr++; gUnknown_03001628 = ptr; *ptr = 0; ptr--;
   gUnknown_03001628 = ptr;`). A plain `ptr[1] = 0;` (no intermediate
   global writes) let the compiler dead-store-eliminate the temporary
   repoint, since nothing reads `gUnknown_03001628` back before it's
   overwritten again three lines later. Fixed by declaring
   `gUnknown_03001628` itself `vu16 * volatile` (a volatile *pointer*,
   not just a pointer to volatile data - it's plausibly genuinely
   volatile anyway, since it's read/written from the timer's own IRQ
   handler context too) so the compiler can no longer prove the store
   is dead.
2. That same `volatile` fix, applied naively (`gUnknown_03001628++;`
   directly), then re-reads the global from memory on *every* access
   instead of keeping it in a register, which doesn't match the ROM's
   read-once/write-back-twice shape. Fixed by routing the increment/
   decrement through an explicit local `vu16 *ptr` variable instead.
3. The final `REG_IE &= ~(8 << gUnknown_03001620);` line: plain C
   always materializes the shift's constant operand (`8`) before its
   variable operand (confirmed via an isolated probe - this compiler's
   `x << y` codegen unconditionally loads `x` first), where the ROM
   loads the index first. Fixed by pinning the REG_IE address, the
   index, and the shift result each to their own `register ... asm`
   variable and writing them as their own separate statements (`riePtr
   = ...; idx = gUnknown_03001620; bit = 8; bit <<= idx; *riePtr &=
   ~bit;`) - this reverses the operand-load order to match, and pinning
   the REG_IE address itself to `r2` (assigned before either operand)
   moves its literal load to the very top of the sequence too, exactly
   matching the ROM.

**`sub_803ACE0`** (`src/system/eeprom_verify.c` - see below). Reads an
EEPROM block back via `sub_803AB54` and compares it against the
caller's expected data. One real surprise: the ROM does its **own**
`addr >= maxCount` range check up front and **never looks at
`sub_803AB54`'s own return value at all** - it always proceeds to the
compare loop even if the read bailed out early with an error, using
whatever ended up in the (potentially stale/garbage) local buffer. This
reconstruction replicates that faithfully rather than "fixing" it.
Needed the load-then-increment-then-compare statement order spelled out
explicitly (`u16 a = *expected; u16 b = *p; p++; expected++; if (a !=
b) { ... }`) instead of a more natural `if (*p != *expected++) ...`,
to get the pointer increments positioned inside the comparison block
the way the ROM has them, rather than deferred to the `for` loop's own
increment step.

**`sub_803AD38`** (same file). Writes+verifies with up to 3 retries.
Matched on the first attempt once its callees' return values were typed
correctly - see the `u16 result` note below.

## Narrowed this pass (still parked)

**`sub_803AA08`**: previously "every field/register access matches but
extended-register choice and literal-pool order were both wrong,
everywhere". Fixed via the same folded-assignment trick as `sub_803AA90`
(`gUnknown_0300162C = *(imeAddr = (vu16 *)0x04000208);` makes the
compiler evaluate the *destination* global's address before the source
pointer's, matching the ROM) plus a `vu16 * volatile *tmpAddr` local
assigned *before* the r8-pinned `timerPtrAddr`, then dereferenced
through *itself* rather than through the pinned copy - this reproduces
the ROM's "compute the address once into a plain register, copy it to
r8 for later reuse, but still deref the *original* plain-register copy
for the immediate read" shape (Thumb can't dereference r8-r15 directly,
so routing the first deref through the r8-pinned variable forces an
extra `mov` the ROM doesn't have). What's left is narrow: the `REG_IF =
8 << gUnknown_03001620` shift's operand-order gap (same root cause as
`sub_803AA90`'s point 3 above, but not yet resolved here - several
pinning attempts changed *which* registers were used without matching
both the order *and* the ROM's extra scratch-register round-trip
simultaneously), the following `REG_IE |=` store lands its OR result in
r2 instead of the ROM's r1, and the final `*timerPtrAddr = timerPtr`
restore re-fetches r8 into a fresh register instead of reusing the one
still live from the previous store three instructions earlier.

**`sub_803AB54`/`sub_803AC04`**: see the EEPROM section below.

## Unchanged (still parked)

**`sub_803AAD4`**: no change from the previous pass - still the
busy-wait tail's loop-rotation/literal-pool-placement shape (see the
function's own doc comment in `src/system/timer_util.c`). A hand-written
`asm volatile` anchor for just the tail *can* reproduce the ROM's
doubled check, but the ROM keeps every constant this function uses
(including the loop's `0x040000DE` address) in one shared trailing
literal pool that only the compiler's own pool management can
reproduce - a hand-embedded `.word` inside an inline-asm block
necessarily lands mid-function instead, trading this gap for a
pool-placement one rather than closing it. Also newly discovered while
investigating: even the *pre-tail* portion doesn't byte-match either -
this compiler promotes the twice-used `REG_IME` address into a cached
extended register across the whole function, where the ROM just
re-loads the same literal twice. Left for whoever revisits this
function next.

## The EEPROM cluster: confirmed protocol

`sub_803AB54`/`sub_803AC04`/`sub_803ACE0`/`sub_803AD38` (ROM
`0x0803AB54`-`0x0803AD78`) build/verify a bit-serial transmission for
the GBA EEPROM save chip. Reading every instruction against the real
GBA EEPROM bit-serial protocol confirms this exactly:

- **Read** (`sub_803AB54`, `src/system/eeprom_util.c`): sends 2 start
  bits ("11"), the chip's `addrBitCount` address bits (6 or 14,
  MSB-first), and one more bit - left as uninitialized stack data, the
  ROM never explicitly sets it and this reconstruction doesn't either,
  to match byte-for-byte (`addrBitCount + 3` halfwords total: 2 start +
  N address + 1 unset). Then reads back `0x44` (68) halfwords: the
  first few are the chip's "busy" dummy bits (never unpacked), followed
  by 64 data bits, unpacked MSB-first into 4 words landing in
  `dest[3]`, `dest[2]`, `dest[1]`, `dest[0]` (reverse order - the real
  GBA EEPROM convention).
- **Write** (`sub_803AC04`): sends 2 start bits ("10" - the chip
  distinguishes read from write by this pair), the address bits, 64
  data bits (MSB-first per 16-bit word), and a trailing stop bit ("0")
  explicitly written this time (`addrBitCount + 0x43` halfwords: 2
  start + N address + 64 data + 1 stop). Then arms a watchdog timer
  (`sub_803AA08` with `gStaticData_085A9F10` as the reload/config) and
  busy-waits on the EEPROM data port's ready bit (`0x0D000000` bit 0)
  until either it goes ready or the timer's IRQ handler flags a timeout
  (`gUnknown_03001624`), stopping the timer either way before
  returning. Returns `0xC001` on a watchdog timeout.
- Only bit 0 of each 16-bit DMA slot matters to the real hardware -
  neither the ROM nor this reconstruction masks the values written into
  the bit buffer beyond the address/data value itself (e.g. `buf[i] =
  addr >> k;`, not `buf[i] = (addr >> k) & 1;`), relying on the EEPROM
  interface only latching bit 0 of each transferred halfword.

This is committed as a `NON_MATCHING` reconstruction in
`src/system/eeprom_util.c` (real bytes still in
`asm/code_3_2_20e_ab54.s`) rather than left raw - a real upgrade from
the previous pass's "working theory, not confirmed enough to commit
even a parked reconstruction". What resists byte-matching is the same
family of register-allocation/loop-rotation gaps documented above for
`sub_803AA08`/`sub_803AAD4`:

- `sub_803AB54`'s bit-unpack tail: `word <<= 1; word |= *p & 1;` (two
  separate statements, not one combined expression - confirmed via an
  isolated probe that only the two-statement form produces the ROM's
  fused `lsl r/, #0x11` / `lsr r/, #0x10` truncate-and-shift idiom)
  fixed the inner loop's shape exactly, but the outer loop still can't
  keep the "mask = 1" constant and outer-loop counter in the same low
  registers the ROM reuses from the address-packing phase above (this
  compiler's allocator spills the mask into `r12` instead once it runs
  out of low registers at that point), and it hoists the read-back/
  dest-pointer setup earlier than the ROM does (both loop-invariant, so
  the scheduler moves them up across the two `sub_803AAD4` calls even
  though they're real function calls).
- `sub_803AC04`'s busy-wait tail hits the identical loop-rotation gap
  `sub_803AAD4` has: this compiler restructures the `if (cond) break;
  if (flag) { if (cond) break; result = ...; break; }` shape into extra
  basic blocks the ROM doesn't have, rather than the ROM's simpler
  linear flow.

`sub_803ACE0`/`sub_803AD38` (the read-verify and write-retry pair) are
split into their own file, `src/system/eeprom_verify.c`, for the same
ROM-contiguity reason `sub_803AA90` needed its own file - their real
addresses sit between the still-parked `sub_803AC04` and
`reg_trampolines.c`'s matched functions, so they can't share a
translation unit with either. `sub_803AD38`'s `result` local is typed
`u16` (not `s32`, even though the callees it stores both return `s32`)
specifically to match the ROM's `lsls r0,r0,0x10 / lsrs r2,r0,0x10`
truncation after each call - confirming those calls' return values only
ever need to be compared against zero, truncated to 16 bits, by this
caller.

## Files touched

- `src/system/timer_util.c` - `sub_803AA08` improved (still parked),
  `sub_803AA90` cut out to its own file, `sub_803AAD4` unchanged.
  `gUnknown_03001628`'s declaration is now `vu16 * volatile`.
- `src/system/timer_util_aa90.c` (new) - `sub_803AA90`, matched.
- `src/system/eeprom_util.c` (new) - `sub_803AB54`/`sub_803AC04`,
  `NON_MATCHING` reconstructions (previously fully raw).
- `src/system/eeprom_verify.c` (new) - `sub_803ACE0`/`sub_803AD38`,
  matched.
- `asm/code_3_2_20e_aa08.s` - trimmed to just `sub_803AA08`.
- `asm/code_3_2_20e_aa90.s` (new, split from the above) - just
  `sub_803AAD4`.
- `asm/code_3_2_20e_ab54.s` - trimmed to just `sub_803AB54`/
  `sub_803AC04`.
- `ldscript.txt` - `timer_util_aa90.o`/`code_3_2_20e_aa90.o` inserted
  between the two halves of the old `code_3_2_20e_aa08.o`;
  `eeprom_util.o`/`eeprom_verify.o` inserted around
  `code_3_2_20e_ab54.o`.
- `tools/report_units.py` - `UNITS` table split to match the new file
  boundaries.
- `docs/status/system.md` - matched/parked lists updated.
