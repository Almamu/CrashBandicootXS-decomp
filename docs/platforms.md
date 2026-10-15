# Platforms (`platform/`)

The game and engine code (`src/`, `include/`) and the libraries (`lib/`)
are built on a platform: the hardware headers they include, the startup
and interrupt code, and the BIOS calls. The original is the GBA, in
`platform/gba/`, and both ROMs (Crash Bandicoot XS and N-Tranced) are
built on it. A port to another system (#1093) supplies its own
`platform/<name>/` with the same interface, and the game code compiles
unchanged on top of it.

There is no `#if PORTABLE` or other platform test in the game code: each
platform has its own copy of the platform headers, and the build picks one
by its include path. (The `#if PORTABLE` branches the project inherited
from sa2's headers were removed in #1093.)

## `platform/gba/`

```
platform/gba/include/gba/          the hardware headers
    types.h                        u8..u64, s8..s64, vu8..vs32
    io_reg.h                       REG_BASE, the REG_* registers and their bits
    defines.h                      the memory map (EWRAM_START, IWRAM_START, PLTT,
                                   VRAM, OAM), the sizes, DISPLAY_WIDTH/HEIGHT,
                                   OAM_X/OAM_Y, IWRAM_DATA/EWRAM_DATA, RGB16,
                                   the BIOS IWRAM words (INTR_VECTOR, ...)
    dma_macros.h                   DmaSet, DmaCopy16/32, DmaFill16/32, ...
    gba.h                          all four
platform/gba/asm/
    crt0.s                         `start`: the boot code at 0x08000000 (CPU
                                   modes and stacks, the IWRAM image's copy,
                                   gIntrTable/INTR_VECTOR, AgbMain)
    rom_header.inc                 the cartridge header crt0 starts with
    intr_main.s                    IntrMain (IntrMain_Buffer), the IRQ dispatcher,
                                   first in the IWRAM image
platform/gba/lib/libagbsyscall/
    include/agb_syscall.h          the BIOS calls' prototypes
    libagbsyscall.s                the 8 SWI wrappers (BgAffineSet ... VBlankIntrWait)
```

All of it is the original's: the headers are the ones the code was
matched against, and the three `.s` files are hand-written assembly
(`HANDWRITTEN` in `tools/report_units.py`, out of the progress totals;
crt0 never had a unit). `asm/` keeps what isn't the platform's: itoa_arm
(game code, assembly by #662's decision) and the assembler macros
(`asm/macros.inc`, `asm/macros/`), which the GBA platform's `.s` files
and the N-Tranced disassembly include.

The build (Makefile, before the per-game part, so every game gets it):

```
PLATFORM_DIR := platform/gba
CPPFLAGS := ... -iquote include -I $(PLATFORM_DIR)/include \
	-I lib/*/include -I $(PLATFORM_DIR)/lib/*/include ...
```

and the XS part assembles `platform/gba/asm/*.s` and
`platform/gba/lib/*/*.s` into `build/crashbandicootxs/platform/gba/`,
where `ldscript.txt` places them (crt0 first, the SWI wrappers after
GAX2's ARM DSP code, IntrMain first in the IWRAM image).

## Including the platform headers

The game includes them by their name in the platform's include directory:
`#include "gba/gba.h"` (core.h), `"gba/types.h"`, `"gba/io_reg.h"`,
`"gba/dma_macros.h"`, `"gba/defines.h"`, and `<agb_syscall.h>`. A quoted
include is looked up in the including file's own directory first, then
the `-iquote` directories, then the `-I` ones, so:

- no header named `gba/<x>.h` may exist anywhere but
  `platform/<name>/include/`: one next to a file that includes it, or
  under the `-iquote` directories (`include/`, `build/include/`, and for
  N-Tranced `games/n2/include/` and `build/ntranced/include/`), would be
  picked before the platform's;
- the platform headers include each other relative to themselves
  (`gba.h`'s `"./types.h"`, `agb_syscall.h`'s `"gba/types.h"`), which
  stays inside the platform's own tree.

#1093 checked this when the headers moved out of `include/gba/`: every
include of them (5 of `gba/gba.h`, 19 of `gba/types.h`, 9 of
`gba/io_reg.h`, 13 of `gba/dma_macros.h`, 2 of `gba/defines.h`, 23 of
`<agb_syscall.h>`, in 64 files of `include/`, `src/`, `lib/` and
`platform/`, and the `"gba/types.h"` that tools/levels.py,
tools/gfx_manifest.py and tools/sprite_banks.py write into their
generated sources) uses one of these names, none uses a relative path, and the only
`gba/` directory and the only files with these names in the tree are the
platform's (`tools/agbcc/include` has a `sys/types.h` and a
`machine/types.h`, which nothing includes as `gba/`).

## What a port provides

A port is a `platform/<name>/` with:

1. **The headers**, `platform/<name>/include/gba/{types,io_reg,defines,dma_macros,gba}.h`
   and `agb_syscall.h`, with the same names and macros. The usual way
   (every GBA decomp port has done it): `REG_BASE`, `EWRAM_START`,
   `IWRAM_START`, `PLTT`, `VRAM` and `OAM` are host arrays the platform
   library owns instead of the GBA's addresses, and the `Dma*` macros are
   `memcpy`/`memset`. It may also set another `DISPLAY_WIDTH` x
   `DISPLAY_HEIGHT` (docs/screen.md; `make screen-check` builds the game
   at 426x240) and make `OAM_X`/`OAM_Y` no-ops for wider OAM fields.
   `IWRAM_DATA`/`EWRAM_DATA` can be empty.
2. **Startup and interrupts** in place of crt0 and IntrMain: run
   `AgbMain` (src/engine/sys/main.cpp), and drive the game's interrupt
   handlers from `gIntrTable` (Irq::Setup fills it, irq.cpp) once a
   frame: VBlank at line 160, VCount at the line `DISPSTAT` asks for
   (the music fade's, 0x35), Timer2 for the EEPROM, Timer3 and Serial for
   the link.
3. **The BIOS calls** of `agb_syscall.h` (`CpuSet`, `CpuFastSet`,
   `LZ77UnCompVram`, `RLUnCompVram`, `BgAffineSet`, `ObjAffineSet`,
   `Sqrt`, `VBlankIntrWait`), as C, plus the DivMod `svc #6`
   (src/engine/sys/bios_util.cpp).
4. **Audio** behind GAX2's API (`lib/gax/include/gax.h`: `GAX_irq`,
   `GAX_play`, the init and fx calls), reading the built music and sfx
   data, and `gGaxIrqEnabled`; the game's `AudioContext` stays as it is.
   GAX2 itself (self-modifying ARM) is not ported.
5. **Save** behind `lib/agb_eeprom`'s API, and a link session that never
   connects.

The issue (#1093) has the phases (compile-only, headless boot, PPU,
input, save, audio, link) and the per-subsystem seams. Phase 0, on the
GBA with the ROMs unchanged, is this split, the PORTABLE branches'
removal and the 64-bit fixes ("64-bit hosts" below).

## 64-bit hosts

The code must compile for a 64-bit host with the GBA build unchanged
(the owner's decision: no `-m32`). Where the ROM stores or computes with
an address as an integer, the code uses `uintptr_t` (`<stdint.h>`, which
`gba/defines.h` already includes) or a real pointer type, which compile
to the same code on the GBA, where both are 32 bits.
