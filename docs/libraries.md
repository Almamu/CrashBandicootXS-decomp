# Libraries (`lib/`)

Part of the ROM was never this game's code: it is third-party and SDK code
that was linked in. It lives under `lib/`, one directory per library, the
way pokeemerald keeps `libagbsyscall`/`libgcc` apart from the game. `src/`
holds only the game.

| Library | What it is | ROM code | Data |
|---|---|---|---|
| [`lib/gax/`](../lib/gax) | Shin'en's GAX Sound Engine 2.01D | `0x08037F3C`-`0x0803A944` | `lib/gax/data/gax_tables_5a6100.c` |
| [`lib/agb_eeprom/`](../lib/agb_eeprom) | Nintendo's AgbEeprom SDK library, `EEPROM_V122` | `0x0803A968`-`0x0803AD78` | `lib/agb_eeprom/data/eeprom_5a9eec.c` |
| [`lib/libgcc/`](../lib/libgcc) | gcc 2.9's libgcc: libgcc2.c's 64-bit helpers and lib1funcs.asm's routines | `0x08037648`-`0x08037F3C`, `0x0803AD78`-`0x0803AFDC` | `lib/libgcc/data/clz_tab_5a4c70.c` |
| [`lib/libagbsyscall/`](../lib/libagbsyscall) | the BIOS SWI wrappers | `0x0803A944`-`0x0803A968` | - |

## Layout

```
lib/<name>/include/   the library's public header(s), on the -I path
lib/<name>/src/       its C sources, one object each, plus internal headers
lib/<name>/data/      its constant data (C tables, linked in the data block)
```

`libgcc` and `libagbsyscall` are built the way their originals are:

- `lib/libgcc/libgcc2.c` is compiled once per function with `-DL_<name>`
  (`_divdi3.o`, `_udivdi3.o`, `_muldi3.o`), and `lib/libgcc/lib1funcs.s`
  is assembled once per routine with `L_<name>` defined (`_udivsi3.o`,
  `_divsi3.o`, `_dvmd_tls.o`, `_modsi3.o`, `_umodsi3.o`,
  `_call_via_rX.o`): the Makefile's `LIBGCC2_FUNCS`/`LIB1FUNCS`.
- `lib/libagbsyscall/libagbsyscall.s` holds the eight wrappers in the
  ROM's (alphabetical) order.

The lib1funcs routines and the SWI wrappers are hand-written assembly.
They used to be NAKED functions in C files (`src/util/math_div_util.c`,
`math_div64_util.c`, `src/system/reg_trampolines.c`, `timer_util.c`); they
are plain `.s` now, with the same bytes. Each `.s` block ends in
`.align 2, 0`: without it the assembler pads the end of the section with
`nop`s (`c0 46`) where the ROM has zero bytes.

### Headers

Game code includes a library's public header and nothing else from it:

- `<gax.h>`: `GAX2_new`/`GAX2_estimate`/`GAX2_init`/`GAX2_jingle`,
  `GAX_irq`/`GAX_play`, the music and sound-effect calls, and
  `struct GaxSongHeader` (GAX2's parameter block). `include/audio.h` (the
  game's audio manager) includes it. The engine's internal structures
  (player state, handler types, channel state, `GAX_CALL_ARM`) are in
  `lib/gax/src/gax_internal.h`.
- `<agb_eeprom.h>`: `EEPROMConfigure`, `SetEepromTimerIntr`, `EEPROMRead`,
  `EEPROMWrite`, `EEPROMCompare`, `EEPROMWrite1_check`, `struct
  EepromConfig` and `gEepromConfig`. The timer state and helpers are in
  `lib/agb_eeprom/src/agb_eeprom_internal.h`.
- `<agb_syscall.h>`: the SWI wrappers. `src/system/asset.cpp` keeps its
  own one-argument `LZ77UnCompVram`/`RLUnCompVram` declarations: its
  callers leave the destination in r1 from their own argument, and the
  two-argument call compiles differently.

The libraries include `gba/gba.h` (the hardware headers in `include/gba/`)
but not the game's headers.

## Linking

The libraries are linked as plain objects, not `.a` archives. They are
interleaved with game code in the ROM (the GAX2 block sits between the
language-select screen and the AgbEeprom library; game code resumes after
libgcc's lib1funcs routines), so `ldscript.txt` lists every object where
the ROM has it, exactly as for game code. Only the paths say which code is
a library's.

ROM order of the library code:

```
lib/libgcc/_divdi3.o _udivdi3.o _udivsi3.o _muldi3.o   (linked in with GAX2)
lib/gax/src/gax_*.o                                     (the engine)
lib/libagbsyscall/libagbsyscall.o
lib/agb_eeprom/src/eeprom_timer.o eeprom_timer_stop.o eeprom_read_write.o eeprom_verify.o
lib/libgcc/_call_via_rX.o _divsi3.o _dvmd_tls.o _modsi3.o _umodsi3.o
```

## Compiler flags

The per-object flags moved with the files, unchanged:

| Objects | Compiler | Flags |
|---|---|---|
| `lib/gax/src/*.o`, `lib/*/data/*.o` | agbcc | default (`-O2 -mthumb-interwork ... -fprologue-bugfix`) |
| `lib/agb_eeprom/src/*.o` | agbcc | `-O1` instead of `-O2` (`O1_OBJS`, [eeprom-sdk-o1.md](./matching/eeprom-sdk-o1.md)) |
| `lib/libgcc/_divdi3.o`, `_udivdi3.o`, `_muldi3.o` | agbcc | no `-mthumb-interwork` (`NO_INTERWORK_OBJS`, [gax-toolchain-retry.md](./matching/archive/gax-toolchain-retry.md)) |
| `lib/libgcc/_*.o` from lib1funcs.s, `lib/libagbsyscall/*.o` | as | `ASFLAGS` |

No library object uses old_agbcc, agbcc_arm(_patched) or
`-fno-rerun-loop-opt` (`OLD_AGBCC_OBJS`, `ARM_OBJS`,
`PATCHED_ARM_OBJS`, `NO_RERUN_LOOP_OPT_OBJS` are all game code).

## What stayed in the game

- **The songs and the sound-effect set** (`sound/`, `gGaxMusicData`,
  `gGaxSfxData`, built by `tools/gax_audio.py`): they are in GAX2's format
  but they are this game's content (its 19 songs and its sound effects),
  not part of the engine, so they stay with the game's assets. So does
  `gGaxDefaultSong`, the engine's default handler layout: it sits at the
  end of the music block and `tools/gax_audio.py` builds it with the songs
  (docs/audio.md).
- **The game-side audio code**: the audio manager (`src/audio/audio.cpp`), the song
  table (`src/data/song_table_16aa20.c`) and the sound-effect table
  (`sound/sfx_table.json`). They call GAX2 through `<gax.h>`.
- **GAX2's IWRAM variables**: `gGaxIrqEnabled` is the game's flag (its
  VBlank handler calls `GAX_irq` while it is set). `gGaxHaltFont`, the
  fatal-error screen's font, is GAX2 data, but it sits in the middle of
  the IWRAM image's initialised data (`src/iwram/iwram_data.c`, one
  object), so it stays there.
- **crt0 and IntrMain** (`asm/crt0.s`, `asm/intr_main.s`) stay in `asm/`.
  They derive from the AGB SDK's crt0 template, but they are this game's
  startup code: crt0 carries the game's ROM header and IWRAM-image copy,
  IntrMain dispatches through the game's `gIntrTable` and is the first
  thing in the game's IWRAM image. pokeemerald keeps its crt0 in the game
  tree too.

## decomp.dev

`tools/report_units.py` gives each library its own progress category:
`gax` ("GAX2 sound engine (lib)"), `agb_eeprom` ("AgbEeprom SDK (lib)")
and `libgcc` ("libgcc (lib)"); game audio keeps `audio`. The hand-written
ranges (lib1funcs.s, libagbsyscall.s) are `HANDWRITTEN`: no unit, out of
the totals. `tools/chunk_remaining_work.py` scans `lib/**/*.c` as well as
`src/**/*.c` for parked functions and cleanup candidates.
