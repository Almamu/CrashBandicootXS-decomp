# EEPROM SDK library at -O1 (issue #69 retry)

The three EEPROM helpers left NAKED in issue #69's range are now real C:

| function | SDK name | file | before | now |
|---|---|---|---|---|
| `sub_803AAD4` | `DMA3Transfer` | `src/system/timer_util_aa90.c` | NAKED | real C, -O1 |
| `sub_803AB54` | `EEPROMRead` | `src/system/eeprom_util.c` | NAKED | real C, -O1 |
| `sub_803AC04` | `EEPROMWrite` (timer-watchdog version) | `src/system/eeprom_util.c` | NAKED | real C, -O1 |

`sub_803AA90` (`StopEepromTimer`, already matched with three register
pins and a `vu16 * volatile` global) was rewritten as the SDK's plain C
too, because it shares the -O1 object.

## Why: this is Nintendo SDK C built at -O1

The ROM has the string `EEPROM_V122` at `0x085A9EEC`: Nintendo's
AgbEeprom library, linked from the SDK as C compiled with the SDK's
own settings. zeldaret/tmc reconstructed the V124 version of the same
library (`src/eeprom.c`). V122 differs mainly in the write's timeout:
it uses a timer IRQ (`sub_803AA08`/`sub_803AA90`, the same
start/stop pair as pokeemerald's `agb_flash` flash timer) where V124
counts VCOUNT scanlines.

That source shape, dropped in unchanged, is off at the game's -O2
with both compilers. With `-O1` (agbcc, not old_agbcc) it is
byte-identical:

| function | agbcc -O2 | agbcc -O1 | old_agbcc -O1 |
|---|---|---|---|
| `sub_803AA90` (plain SDK C) | 2 | **MATCH** | MATCH |
| `sub_803AAD4` | 43 (size 120 vs 128) | **MATCH** | 2 |
| `sub_803AB54` | 79 | **MATCH** | 44 (size 184 vs 176) |
| `sub_803AC04` | 107 (size 248 vs 220) | **MATCH** | 35 (size 228 vs 220) |

(halfwords differing, relocations masked, whole file compiled.)

For `sub_803AAD4`, a flag sweep over every -O2 sub-flag the compilers
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

- `sub_803AB54`: the address-bit pointer has to be written as a byte
  offset, `(u16 *)((u8 *)buffer + ((width << 1) + 1) + 1)` (TMC's
  `(u8 *)ptr += ...; ((u8 *)ptr)++` in standard C). `&buffer[width + 1]`
  is 3 halfwords off, `(u8 *)buffer + (width << 1) + 1 + 1` is 1 off.
- `sub_803AC04`: `ptr = (u16 *)(0x42 + (u32)buffer + (u32)(width * 2) +
  0x42)` (TMC's spelling) gives the ROM's `lsl; add sp; add #0x84`
  order. `ret = 0` goes *after* the `sub_803AA08` (StartEepromTimer)
  call.
- `sub_803AAD4`: WAITCNT is one expression,
  `REG_WAITCNT = (REG_WAITCNT & 0xF8FF) | cfg->waitcntBits`. TMC's
  three-statement `temp` version swaps the `ands` operands.

## Build change

`Makefile` gets an `O1_OBJS` list (`timer_util_aa90.o`,
`eeprom_util.o`) that swaps `-O2` for `-O1`. Both files match as a
whole with it.

Not changed: `src/system/timer_util.c` (`sub_803A968` = type select,
`sub_803A9D0`/`sub_803AA08` = timer setup, plus unrelated BIOS SWI
stubs) and `src/system/eeprom_verify.c` (`sub_803ACE0` =
`EEPROMCompare`, `sub_803AD38` = `EEPROMWrite1_check`). They already
match at -O2, but with pins that are probably stand-ins for -O1.
Their current C does not match at -O1 as written: `sub_803ACE0` is 19
off and `sub_803AA08` 2 off. Moving them to -O1 with their plain SDK
C is a possible cleanup, outside this pass's files.

## Issue #69 status

Every compiler-generated function in `0x0803A944`-`0x0803ADB0` is now
real C. What's left NAKED there is hand-written library code: the BIOS
SWI wrappers in `timer_util.c` and the libgcc `_call_via_rN`
trampolines in `reg_trampolines.c`.
