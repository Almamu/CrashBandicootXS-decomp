# EEPROM SDK library at -O1 (issue #69 retry)

The three EEPROM helpers left NAKED in issue #69's range are now real C:

| address | SDK name | file | before | now |
|---|---|---|---|---|
| `0x0803AAD4` | `DMA3Transfer` | `lib/agb_eeprom/src/eeprom_timer_stop.c` | NAKED | real C, -O1 |
| `0x0803AB54` | `EEPROMRead` | `lib/agb_eeprom/src/eeprom_read_write.c` | NAKED | real C, -O1 |
| `0x0803AC04` | `EEPROMWrite` (timer-watchdog version) | `lib/agb_eeprom/src/eeprom_read_write.c` | NAKED | real C, -O1 |

`StopEepromTimer` (already matched with three register
pins and a `vu16 * volatile` global) was rewritten as the SDK's plain C
too, because it shares the -O1 object.

## Why: this is Nintendo SDK C built at -O1

The ROM has the string `EEPROM_V122` at `0x085A9EEC`: Nintendo's
AgbEeprom library, linked from the SDK as C compiled with the SDK's
own settings. zeldaret/tmc reconstructed the V124 version of the same
library (`src/eeprom.c`). V122 differs mainly in the write's timeout:
it uses a timer IRQ (`StartEepromTimer`/`StopEepromTimer`, the same
start/stop pair as pokeemerald's `agb_flash` flash timer) where V124
counts VCOUNT scanlines.

That source shape, dropped in unchanged, is off at the game's -O2
with both compilers. With `-O1` (agbcc, not old_agbcc) it is
byte-identical:

| function | agbcc -O2 | agbcc -O1 | old_agbcc -O1 |
|---|---|---|---|
| `StopEepromTimer` (plain SDK C) | 2 | **MATCH** | MATCH |
| `DMA3Transfer` | 43 (size 120 vs 128) | **MATCH** | 2 |
| `EEPROMRead` | 79 | **MATCH** | 44 (size 184 vs 176) |
| `EEPROMWrite` | 107 (size 248 vs 220) | **MATCH** | 35 (size 228 vs 220) |

(halfwords differing, relocations masked, whole file compiled.)

For `DMA3Transfer`, a flag sweep over every -O2 sub-flag the compilers
accept (`-fno-gcse`, `-fno-rerun-cse-after-loop`, `-fno-regmove`,
`-fno-thread-jumps`, ...) left it at 40-48 halfwords. Only `-O1` works,
and `-O1` plus `-fexpensive-optimizations` or `-frerun-cse-after-loop`
breaks it again (39/34). So -O1 itself is the setting, not a
coincidence of one sub-flag.

### What -O1 explains

- **The DMA wait "tail evaluated twice"** that the old notes called
  unreproducible is gcc's duplicated `while` exit test
  (`jump.c: duplicate_loop_exit_test`). At -O2 the duplicated test and
  the in-loop test end up with the same registers and cross-jumping
  merges them into a `b` into the loop. At -O1 they keep different
  registers and both stay. The `adds r1, #2` that derives DMA3CNT_H
  from the DMA3CNT address is reload's `move2add`, since r1 still holds
  0x040000DC. The source is just `while (REG_DMA3CNT_H & 0x8000) ;`.
- **The "twice-loaded REG_IME literal"** is -O1 not CSE'ing the
  constant address across the function.
- **The read's bit-unpack register plan** (the "r12 spill" note) and the
  write's busy-wait tail both come out as in the ROM without any
  pins.

### Source-shape details that matter

- `EEPROMRead`: the address-bit pointer has to be written as a byte
  offset, `(u16 *)((u8 *)buffer + ((width << 1) + 1) + 1)` (TMC's
  `(u8 *)ptr += ...; ((u8 *)ptr)++` in standard C). `&buffer[width + 1]`
  is 3 halfwords off, `(u8 *)buffer + (width << 1) + 1 + 1` is 1 off.
- `EEPROMWrite`: `ptr = (u16 *)(0x42 + (u32)buffer + (u32)(width * 2) +
  0x42)` (TMC's spelling) gives the ROM's `lsl; add sp; add #0x84`
  order. `ret = 0` goes *after* the `StartEepromTimer`
  call.
- `DMA3Transfer`: WAITCNT is one expression,
  `REG_WAITCNT = (REG_WAITCNT & 0xF8FF) | cfg->waitcntBits`. TMC's
  three-statement `temp` version swaps the `ands` operands.

## Build change

`Makefile` gets an `O1_OBJS` list (`eeprom_timer_stop.o`,
`eeprom_read_write.o`) that swaps `-O2` for `-O1`. Both files match as a
whole with it.

The rest of the library (`eeprom_timer.c`, `eeprom_verify.c`) moved to
-O1 in a second pass, below.

## Issue #69 status

Every compiler-generated function in `0x0803A944`-`0x0803ADB0` is now
real C. What's left NAKED there is hand-written library code: the BIOS
SWI wrappers (then in `timer_util.c`, now
`lib/libagbsyscall/libagbsyscall.s`) and the libgcc `_call_via_rN`
trampolines (now `lib/libgcc/lib1funcs.s`).

## Second pass: the rest of the library

The five other AgbEeprom functions still matched at -O2 with pins, a
volatile global and a barrier, and one compiled SDK function was still a
raw `.byte` blob. All six are now the SDK's plain C, and the two objects
that hold them joined `O1_OBJS`, so the whole library
(`0x0803A968`-`0x0803AD7C`, four objects) builds at -O1:

| address | SDK name | object | new C, agbcc -O2 | new C, agbcc -O1 | new C, old_agbcc -O1 |
|---|---|---|---|---|---|
| `0x0803A968` | `EEPROMConfigure` | `eeprom_timer.o` | MATCH | **MATCH** | MATCH |
| `0x0803A9AC` | `EepromTimerIntr`, the timer IRQ handler (`FlashTimerIntr` in agb_flash) | `eeprom_timer.o` | 11 (size 32 vs 36) | **MATCH** | MATCH |
| `0x0803A9D0` | `SetEepromTimerIntr` | `eeprom_timer.o` | MATCH | **MATCH** | MATCH |
| `0x0803AA08` | `StartEepromTimer` | `eeprom_timer.o` | 47 (size 132 vs 136) | **MATCH** | MATCH |
| `0x0803ACE0` | `EEPROMCompare` | `eeprom_verify.o` | MATCH | **MATCH** | MATCH |
| `0x0803AD38` | `EEPROMWrite1_check` | `eeprom_verify.o` | MATCH | **MATCH** | MATCH |

(halfwords differing, relocations masked, whole file compiled; the
whole `.text` of both objects is also byte-identical to the ROM at
-O1, including the NAKED BIOS SWI wrappers that object still held
then, which are hand-written and unaffected by the flag; they are
`lib/libagbsyscall/libagbsyscall.s` now.)

`eeprom_timer.c` did not need splitting: its only non-SDK code is the
BIOS SWI wrappers (libagbsyscall-style, also Nintendo library code),
which come out the same under any flag. `eeprom_verify.c` matches at
both levels with the new C; it moved to -O1 so that the whole library
is built with the setting it was compiled with.

### What each rewrite removed

- **`StartEepromTimer`**: six `register ... asm("rN")`
  pins, an `asm volatile("" : "+r")` barrier, nested scratch scopes and
  the `vu16 * volatile gEepromTimerReg` declaration. The source is now
  pokeemerald's `StartFlashTimer` with a `const u16 *maxTime` argument
  and the IF acknowledge written before the IE enable:

  ```c
  gEepromSavedIme = REG_IME;
  REG_IME = 0;
  gEepromTimerReg[1] = 0;
  REG_IF = INTR_FLAG_TIMER0 << gEepromTimerNum;
  REG_IE |= INTR_FLAG_TIMER0 << gEepromTimerNum;
  gEepromTimeoutFlag = 0;
  gEepromTimerCount = *maxTime++;
  *gEepromTimerReg++ = *maxTime++;
  *gEepromTimerReg-- = *maxTime++;
  REG_IME = 1;
  ```

  The ROM's r8/r9 copies of the IME address and of `&gTimerReg`, which
  the pins forced at -O2, are what -O1's register allocation does on
  its own. At -O2 this C is 47 halfwords off. The old pinned C was 2 off
  at -O1, which is what PR #516 measured.
- **`EepromTimerIntr`** (timer IRQ handler, pokeemerald's `FlashTimerIntr`):
  `if (count != 0 && --count == 0) flag = 1;`. It was kept as a
  `.byte` blob labelled `gStaticData_0803A9AD` (the Thumb address
  `SetEepromTimerIntr` hands out). At -O2 CSE reuses the first `ldrh` of the
  counter; the ROM loads it twice, which -O1 does.
- **`SetEepromTimerIntr`**: `&REG_TMCNT(gEepromTimerNum)`
  with the stored timer number re-read, as in agb_flash. It returns the
  handler through a `void (**)(void)` instead of a data label.
- **`EEPROMConfigure`**: TMC's version with a `u16`
  result. It already matched; the only change is the local's type.
- **`EEPROMCompare`**: TMC's loop
  (`if (*data++ != *ptr++) { result = 0x8000; break; }`) with the
  out-of-range case as an early `return 0x80FF`. TMC's `if/else` form
  puts 0x80FF into `result`'s register, 2 halfwords off at -O1. The old
  C (two locals per iteration, `i <= 3`) was 19 off at -O1.
- **`EEPROMWrite1_check`**: TMC's nested
  `if (result == 0) { ...; if (result == 0) break; }` instead of the
  old `continue` chain.

The functions keep the project's `s32` return types, which the callers
in `src/graphics/settings_menu8d.c` and the siblings in
`eeprom_read_write.c` declare; the SDK returns `u16`, and the code is the
same either way because every result is a `u16` local.

## Other SDK code in the ROM

Searched for, and nothing else found:

- **Version strings.** The only Nintendo `NAME_Vnnn` tag in the ROM is
  `EEPROM_V122`. There is no `FLASH_V`/`FLASH1M_V`/`SRAM_V`/`SIIRTC_V`
  tag, no `m4a`/`MusicPlayer`/`MPlay` string, and no `AGB_*` or
  `LIBGCC` string. The other `_V` hit (`fhe_V700` at `0x0856C332`) is
  inside binary data. The only other library ID strings are GAX's
  ("GAX Sound Engine 2.01D (Sep 28 2001) (c) Shin'en Multimedia"), a
  third-party library whose flags are covered by
  `docs/matching/gax-toolchain-retry.md`.
- **Nintendo library code that isn't compiled C.** The BIOS SWI
  wrappers (`lib/libagbsyscall/libagbsyscall.s`, `BgAffineSet`-`VBlankIntrWait`), the libgcc
  `_call_via_rN` table (`lib1funcs.s`), `__divsi3`/`__modsi3`/
  `__umodsi3`/`__div0` and `crt0.s` are all hand-written asm. Compiler
  flags don't apply to them.
- **Game code that looks like library code.** `irq.c` (IRQ table
  setup, VBlank handler, key reading), `memory.c` (heap), `rand.c`,
  `number_format.c`/`string.c`/`printf.c` (`itoa`, `strcat` etc. on `u8 *`),
  `boot.c` and the link-cable/SIO files (`src/link/*.c`, a custom
  CRC-16 handshake) sit in the game's own address range with the
  game's own data structures, and none follows an SDK source. As a
  check, the pinned `irq.c` functions `DisableVBlankHandler`, `UpdateKeys`
  and `ClearKeys` written as plain C are 11/26/5 halfwords off at
  **both** -O1 and -O2, so -O1 does not explain their pins.
- **Whole-tree sweep.** Every `src/**/*.c` was compiled at -O1 (agbcc
  and old_agbcc) and compared per function. About 200 objects also
  match at -O1, but those are small functions that come out the same
  at either level, pins included, so this says nothing about how they
  were built. No object outside the EEPROM library has an SDK version
  string, an SDK source counterpart or an address next to the library.

So the AgbEeprom library is the only SDK C in the ROM, and all of it
is now built at -O1.
