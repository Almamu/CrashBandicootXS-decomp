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
PLATFORM ?= gba
PLATFORM_DIR := platform/$(PLATFORM)
CPPFLAGS := ... -iquote include -I $(PLATFORM_DIR)/include \
	-I lib/*/include -I $(PLATFORM_DIR)/lib/*/include ...
```

and the XS part assembles `platform/gba/asm/*.s` and
`platform/gba/lib/*/*.s` into `build/crashbandicootxs/platform/gba/`,
where `games/xs/ldscript.txt` places them (crt0 first, the SWI wrappers after
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
   `IWRAM_DATA`/`EWRAM_DATA` can be empty. `NULL_READ` and
   `MEM_FREE_LOST` cover two of the original's bugs that the GBA's
   hardware makes harmless (a read through NULL, a free of ROM data);
   a port where those fault defines them (see `platform/pc/`).
   `STRUCT_RETURN_VIEW` names the function a struct-returning view of a
   method's C name calls, for an ABI that returns small structs in
   registers.
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
   How is the port's business: the pc platform compiles lib/gax's C
   with its own units for the parts that can't run on a host (its ARM
   routines, their self-modifying calls, the DMA), see "The sound" below.
5. **Save** behind `lib/agb_eeprom`'s API, and a link session that never
   connects.

The issue (#1093) has the phases (compile-only, headless boot, PPU,
input, save, audio, link) and the per-subsystem seams. Phase 0, on the
GBA with the ROMs unchanged, is this split, the PORTABLE branches'
removal and the 64-bit fixes ("64-bit hosts" below).

## 64-bit hosts

The code must compile for a 64-bit host with the GBA build unchanged
(the owner's decision: no `-m32`). The rules:

- **An address held or computed as an integer is a `uintptr_t`** (an
  `intptr_t` where the ROM compares it signed), never a `u32`/`s32`:
  `(uintptr_t)table + (kind << 2)`, a `uintptr_t` field for a VRAM block
  (`CompanyLogos::tilesA`, `TileSlotPool::vramBase`, `Starfield::mapVramBase`),
  the allocator's `start`/`end`. agbcc's `<stdint.h>` (which
  `gba/defines.h` includes) has `uintptr_t` as `unsigned int` and
  `intptr_t` as `int`: the same types as `u32` and `s32`, so the GBA code,
  the aliasing and the C++ mangled names are unchanged. Many of these
  integer forms are there for matching (the ROM's operand order); on a
  64-bit host they are only wider.
- **The memory map is integers.** `REG_BASE`, `EWRAM_START`,
  `IWRAM_START`, `PLTT`, `VRAM` and `OAM` are integer constants on the
  GBA, and the game adds to them and stores them in `uintptr_t`s; a
  port's headers define them as integers too
  (`#define VRAM ((uintptr_t)gHostVram)`), not as pointers.
- **A table of addresses is pointer-sized.** `anim_table_record.table_B`
  (actor_anim.h) is a `const void *` to either a `const u32[]` of sheet
  offsets or a `const u8 *const[]` of frame addresses, and AnimPart
  (actor_self.hpp) reads it as `frameOffsets` (`u32 *`) or `frameAddrs`
  (`uintptr_t *`, a union with it) by the record's mode. The airship's
  and the hovercraft's `mapFrames` are `uintptr_t[]`. The generated
  animation tables (tools/levels.py) need no casts.
- **A CpuSet of pointers counts words.** CpuSet's count is in 32-bit
  units, one a pointer on the GBA: the part list's and the crate list's
  compaction (PartList::Update, Remove, RemoveAt; CrateList::RemoveAt,
  Detach) copy `(count - i) * (sizeof(*items) / sizeof(u32))` words, the
  same `count - i` on the GBA. With `count - i` alone the host moved half
  the entries: a wumpa that flew off (MarkGone) left the touchable list
  with one wumpa twice and another dropped, from frame 961 of boot.keys.
- **A union of a pointer and an `s32` is zeroed through the pointer.**
  Crate's per-kind word (0x48) is an iron switch crate's `group` pointer
  and other kinds' `s32`s; Crate::Reset zeroes `group`, the widest, not
  `solidKind`, which on the host left `group`'s top half as the heap
  block held, so ActivateIronSwitch's `group != 0` test returned at once
  and the outline crates never turned solid.
- **Layout asserts are the platform's.** A `sizeof`/`offsetof` assert on
  a struct or class that holds a pointer (or a vtable) is a
  `LAYOUT_ASSERT` (gba/defines.h): `COMPILE_TIME_ASSERT` on the GBA, and
  `#define LAYOUT_ASSERT(T, X) COMPILE_TIME_ASSERT(T, 1)` (checks nothing,
  still a typedef, so the `;` after it stays valid) in a platform whose
  pointers aren't 4 bytes. A pointer-free struct's assert stays a
  `COMPILE_TIME_ASSERT`, which holds everywhere. 127 asserts are
  `LAYOUT_ASSERT`s; the other 38, and the ones tools/levels.py generates,
  are not. A new assert follows the same rule.

What phase 0 left (none of it can change without changing the GBA code,
or it is replaced at its API, see "What a port provides"):

- `vsprintf` (src/engine/util/printf.cpp) reads its arguments as 4-byte
  slots of the stack (`u32 *args`, `%s` as `(u8 *)*args`), not with
  `va_arg`: a host needs its own. Nothing in the game calls `sprintf`.
- AgbEeprom (`lib/agb_eeprom/`, which writes the DMA registers with
  addresses as `u32`s) is replaced at its API, not compiled for a host.
  GAX2 (`lib/gax/`) is compiled, but for its ARM parts, and its song data
  is converted ("The sound" below).

The pc platform (below) is the check that this holds: every `src/`
object compiles and links on a 64-bit host, in CI.

## `platform/pc/`

The game for a 64-bit host (Linux, a stock g++/gcc). Phases 1 to 7 of
#1093: it compiles, links and runs the game's own frame loop on the
host's main thread; a software PPU draws the picture a scanline at a
time; an SDL2 window shows it at the GBA's frame rate and reads the
keyboard and gamepads; the EEPROM can live in a file; GAX2 plays the
music and the sound effects, mixed as the GBA mixes them; the link cable
is stubbed, a GBA with no cable attached (the owner's decision). Headless, with
scripted input, it hashes palette RAM, VRAM, OAM and the picture at
checkpoints, which tools/boottest.py compares with mGBA running the ROM
on the same input, and it can write the sound to a WAV file.

```
make PLATFORM=pc        # build/pc/crashbandicootxs_pc; XS only
build/pc/crashbandicootxs_pc --save crash.sav          # in a window (with SDL2)
build/pc/crashbandicootxs_pc --headless --keys platform/pc/tests/boot.keys --frames 6000
```

It needs no ARM toolchain, agbcc or baserom, only what the data
generators need, and SDL2 (found by `pkg-config sdl2`) for the window.
Without SDL2, or with `make PLATFORM=pc HOST_SDL=0`, the executable runs
headless only (CI: `.github/workflows/host.yml` builds both).

**Controls.** Keyboard: the arrows are the D-pad, X is A, Z is B, A and
S are L and R, Enter is Start, Backspace (or right Shift) is Select. A
gamepad (SDL's game controller layout): the D-pad or the left stick, the
right face button (B on an Xbox pad) is A and the bottom one (A) is B,
as the GBA's are placed; the shoulders and triggers are L and R, Start
and Back are Start and Select. F12 writes a screenshot
(crashbandicootxs-N.png, at the window's size), Tab held runs without
waiting, Escape or closing the window quits.

```
platform/pc/platform.mk            the build: the Makefile includes it instead of
                                   its GBA link and compile rules (compile.mk), with
                                   the rest (object lists, generated data) shared,
                                   under build/pc/crashbandicootxs/
platform/pc/include/gba/*.h        the GBA's headers, included by relative path, with
                                   REG_BASE, EWRAM_START, IWRAM_START, PLTT, VRAM, OAM
                                   on host arrays, DmaSet a call, REG_KEYINPUT a call,
                                   LAYOUT_ASSERT empty, IWRAM_DATA/EWRAM_DATA empty,
                                   NULL_READ, MEM_FREE_LOST, STRUCT_RETURN_VIEW
                                   (below)
platform/pc/include/agb_syscall.h  the GBA's prototypes
platform/pc/src/
    memory.S                       the host memory (gba/host.h), in the GBA's order
    dma.c                          DmaSet: an immediate transfer done at once
    bios.c                         the BIOS calls in C but VBlankIntrWait (irq.c)
    irq.c                          the frame loop: VBlankIntrWait, the scanlines,
                                   VCOUNT/DISPSTAT, IF, and IntrMain's dispatch
    ppu.c                          the PPU: each visible line drawn from the
                                   registers, VRAM, palette RAM and OAM
    sdl.c                          the window, the keyboard and gamepads, the frame
                                   pacing (only with SDL2; else says there's none)
    input.c                        REG_KEYINPUT from the key file and the window, the
                                   frame count, the checkpoints
    main.c                         the command line, then AgbMain
    sha1.c, png.c                  the checkpoints' hash, the pictures' PNG files
    eeprom.c                       AgbEeprom's API (a RAM buffer, --save's file), in
                                   place of lib/agb_eeprom
    sio.c                          SIOCNT as a GBA's with no cable attached (the link)
    audio.c                        the sound: GAX2's blocks at each VBlank to the
                                   window's audio device, --wav's file, the hash
    gax_dsp.c                      GAX2's ARM routines (lib/gax/asm) in C
    gax_host.h                     what the platform's GAX2 parts share
    gax_trace.c                    --gax-trace (GAX2 replays the GBA's calls) and
                                   --gax-log (the game's calls, logged)
    level_pick.c                   --level and --room (boottest's level scripts)
    sprite_boxes.c                 GetSpriteAttackBox and GetSpriteBodyBox's C names
                                   (STRUCT_RETURN_VIEW, below)
    libgcc.c                       __divsi3 & co. and __udivdi3, which the game and
                                   GAX2 call by name
    sound_data.S                   gSfxTable, gGaxMusicData and gGaxSfxData,
                                   data/data.s's sound blocks, as the built bytes
platform/pc/units/                 units built in place of src/'s and lib/'s
    engine/sys/bios_util.cpp       DivMod in C (the GBA's is an inline `svc #6`)
    lib/gax/src/*.c                GAX2's files that can't run on a host as they are
                                   ("The sound")
platform/pc/libc_names.txt         the game's functions named as the C library's,
                                   and its operator new and delete
platform/pc/tests/boot.keys        the headless boot's input (CI)
platform/pc/tests/boot_mgba.json   mGBA's log of it (boottest.py), what CI compares
platform/pc/tests/intro.keys       the boot to New Game and the whole intro cutscene
platform/pc/tests/link.keys        the save menu's "load link game", with no cable, aborted
platform/pc/tests/*_mgba_screens.json
                                   the hashes of mGBA's pictures for the three (boottest.py
                                   `screens`), what CI compares the PPU's with
platform/pc/tests/screens/         some of mGBA's pictures of the boot, as PNGs
platform/pc/tests/boot_gax.trace   mGBA's trace of GAX2 in the boot (boottest.py
                                   `gax-trace`), what CI replays and compares with
platform/pc/tools/host_link.py     the link's symbol assignments (below)
platform/pc/tools/layout_check.py  checks the placed globals fit (below)
platform/pc/tools/mgba_screens.c   the ROM in libmgba with a key file: mGBA's pictures
                                   (boottest.py `screens` builds and runs it)
platform/pc/tools/mgba_gax_trace.c the same, logging the game's GAX2 calls and blocks
                                   (boottest.py `gax-trace`)
platform/pc/tools/gax_host_data.py GAX2's song data with pointers, as C (below)
```

**Running it.** `crashbandicootxs_pc [--headless] [--scale N] [--save
FILE] [--wav FILE] [--frames N] [--vblanks N] [--keys FILE] [--level N
[--room R]] [--checkpoint-every N] [--dump DIR] [--screens DIR]
[--screenshot-at N] [--ppu-stats] [--gax-log FILE] [--quiet]`, or
`crashbandicootxs_pc --gax-trace FILE`. In a window unless `--headless`
(or built without SDL2): `--scale N` sizes it (3 times the GBA's by
default), `--screenshot-at N` takes F12's screenshot of the picture frame
N reads its keys under. `--save FILE` keeps the EEPROM there. `--wav
FILE` writes the sound there (see "The sound"), in a window or headless.
`--level N` makes the first level select pick level N (gLevelTable's
index) and `--room R` start it in its room R, as boottest's level scripts
do on mGBA. At the end it prints `audio BLOCKS HASH`: how many blocks
GAX2 mixed and a SHA-1 of them.

A game frame is a read of REG_KEYINPUT (KeyInput::Update reads it once a frame; the
platform's io_reg.h makes the read a call of HostReadKeys), as it is in
tools/boottest.py, which sets the keys at a breakpoint right after that
read: frame 1 is the first read. `--frames N` stops at frame N+1
(`end done at frame N+1`, boottest's "done"). The key file holds a line
`FRAME KEYS` for each change: KEYS (`-`, a number, or names joined by
`+`: A B SELECT START RIGHT LEFT UP DOWN R L) are held from FRAME on.
In a window the keys held there are added to the key file's.
Every `--checkpoint-every` frames (60) it prints `checkpoint FRAME HASH
PICTURE SCREEN`: HASH is boottest's (the SHA-1 of palette RAM, VRAM and
OAM), PICTURE the same with the OAM bits the display ignores cleared (a
hidden entry's attributes, a regular entry's matrix bits), because the
game leaves those as whatever its stack held (DrawSpritePieces' `struct
oam_attrs`), which differs between the GBA and a host, and SCREEN the
SHA-1 of the RGB bytes of the picture the PPU finished last, the one
shown while the game reads that frame's keys. `--dump DIR` writes each
checkpoint's bytes to DIR/FRAME.bin, `--screens DIR` its picture to
DIR/FRAME.png. `--ppu-stats` counts, at the end, the lines that used
each PPU feature (the survey below).

`tools/boottest.py host EXE --keys FILE --frames N` runs it headless and
writes boottest's log; `boottest.py run ROM ELF --keys FILE --frames N`
plays the same key file on mGBA, and `compare` of the two compares the
checkpoints' picture hashes. Both take `--every N` and `--dump DIR`.
`boottest.py screens ROM --keys FILE --frames N` plays it on libmgba
(platform/pc/tools/mgba_screens.c, built against the mGBA on the PATH or
`--mgba PREFIX`) and logs the hashes of mGBA's own pictures, which
`compare` checks a host log's SCREENs against; `--screens DIR` writes
both sides' PNGs and `diff-screens REF DIR` compares them pixel by pixel.
`host` and `screens` take `--level N --room R` (`screens` with `--elf`
for the addresses), `screens` takes `--save FILE`.

**The frame loop** (irq.c). The game runs on the main thread and takes
no time: the display's time passes only in VBlankIntrWait, the one place
the game waits for it (FrameLimiter::WaitForVBlank). Each call runs the
scanlines from where the last one stopped (line 160) to the next VBlank:
161 to 227, then 0 to 160. Each line sets VCOUNT and DISPSTAT's VBlank
and VCount flags and raises what DISPSTAT enables: VCount at the line in
DISPSTAT's top byte (0x35, the music fade's, audio.cpp) and VBlank at
line 160. A raised IRQ that IE enables, with IME on, runs the game's
handler from gIntrTable at once, lowest bit first, as IntrMain does,
and a VBlank sets the BIOS's IntrCheck flag, which VBlankIntrWait waits
for (it turns IME on, as the BIOS's does). IF is kept by the platform and
copied into REG_IF: the game acknowledges IRQs by writing 1s to it (the
link's `REG_IF |= INTR_FLAG_SERIAL`), which plain memory can't do.
HBlank, the timers, the serial port and the DMAs raise nothing: the game
enables no HBlank IRQ, the EEPROM in a RAM buffer needs no Timer2 timeout
(SetEepromTimerIntr installs a no-op), and no cable is attached (sio.c,
below), so no link transfer ever raises Serial, and the game, a child
console, arms no Timer3. A
VBlankIntrWait with the VBlank IRQ off stops the run with an error (the
GBA would hang).

**The PPU** (ppu.c). irq.c's loop draws each visible line (0-159) once
its VCount IRQ has run, as the GBA draws a line after the IRQ at its
start, and at line 160 makes the frame the finished picture
(gHostScreen, 8-bit XRGB), before the VBlank IRQ. So a register the game
writes in the middle of a frame (an IRQ handler) changes the lines after
it; the game's own code otherwise all runs at line 160, so it sees a
whole frame at a time, as it does on the GBA, which latches the affine
reference points at VBlank. It draws:

- modes 0 to 5: text BGs (4 and 8 bpp, flips, the four screen sizes,
  scrolling), affine BGs (BG2PA-PD, BG2X/Y and BG3's, wrap-around or
  transparent outside: the reference point is loaded at the frame's
  start, stepped by PB/PD each line and reloaded when the game rewrites
  it), the bitmap modes 3, 4 and 5 with their frame select; BG tiles
  above VRAM 0x10000 are transparent;
- OBJs: regular and affine, double size, 1D and 2D mapping, 4 and 8 bpp,
  flips, priorities, semi-transparent and OBJ-window OBJs, the per-line
  sprite budget (1210 cycles, 954 with H-blank interval free), tiles
  below 512 hidden in the bitmap modes;
- windows 0 and 1, the OBJ window and the outside, mosaic (BGs and OBJs),
  BLDCNT's alpha blending, brightening and darkening, forced blank
  (white), the backdrop.

Its colours are 8-bit RGB, each 5-bit channel widened as `c << 3 | c >>
2`, with the colour effects on the 8-bit channels (`I1*EVA/16 +
I2*EVB/16` capped at 255, `I + (255-I)*EVY/16`, `I - I*EVY/16`), as mGBA
computes them, so that the two compare exactly (the GBA computes on the
5-bit channels, which rounds some blended pixels apart). mGBA also rounds a
darkened green and blue channel's step up and red's down (its packed
arithmetic drops only red's fraction), and ppu.c does the same: without
it, 14 frames of a fade in the boot test differ by that one step. Where
the hardware is subtle it does what mGBA 0.10 does: a semi-transparent
OBJ with no second target under it takes BLDCNT's brightness effect, and
a transparent pixel of a higher-priority OBJ passes its priority on to
the OBJ pixel under it. It was written from GBATEK's description of the
hardware; no emulator's code is in it. Not modelled: the GBA's
three-line delay when a BG is switched on mid-frame (the game switches
them in VBlank), and VRAM/OAM access timing.

What the game uses (`--ppu-stats` over the intro test and the level runs
below, 4 million lines): modes 0, 1 and 4 (not 2, 3, 5); text BGs 4
and 8 bpp, 256x256 and 512x256; affine BG2 in mode 1 and the mode 4
bitmap, transparent outside (no wrap-around); OBJs 1D-mapped, 4 bpp,
regular, affine and double-size, semi-transparent ones; alpha blending,
brightening, darkening. Not used in any of these runs: windows (the OBJ
window included), mosaic, 2D mapping, 8-bpp OBJs, the 256x512 and
512x512 text sizes, forced blank. The source agrees: no
WININ/WINOUT/WINxH/MOSAIC writes, and every DISPCNT mode it sets is 0, 1
or 4.

**The picture against mGBA.** `boottest.py screens` and `host` with the
same key file, each frame's picture compared (`--every 1`):

| run | frames | the same as mGBA's |
|---|---|---|
| boot.keys (logos, title, menus, Jungle Jam) | 6000 | 6000 |
| boot.keys' play pattern on to frame 20000 (Jungle Jam, then a sewer level) | 20000 | 19991 |
| intro.keys (the whole intro cutscene, into Jungle Jam) | 2400 | 2400 |
| boot.keys, `--level` 0, 1, 9, 11 (Jungle Jam, Shipwrecked, Ruined, Ace of Space) | 960 each | 960 each |
| boot.keys, `--level` 20, 22, 23, 24 (Dingodile, Tiny, Neo Cortex, Mega-Mix) | 960 each | 960 each |
| boot.keys, `--level` 3, 5, 10 `--room 1` (the polar stages) | 960 each | 959 each |
| boot.keys, `--level` 6, 14 (Rocket Racket, Blimp Bonanza: jetpack) | 960 each | 927 each |
| boot.keys, `--level` 21 (N. Gin: jetpack) | 960 | 943 |
| link.keys (the save menu's "load link game" with no cable, aborted) | 1000 | 1000 |

The pictures that differ are of the CPU's time, not the PPU's: the host's
game takes no time, the GBA's does. Frame 512 is the first after the
level's load, which spans 98 VBlanks on mGBA and 77 on the host (the
decompression and copies take none), so the last picture before it is
drawn at another point of the load. In levels 6, 14 and 21 the fade-in
after it steps BLDY from the VBlank IRQ (BrightnessFade::Step), and
mGBA's extra VBlanks put it a step ahead, until it ends (frame 547).
The 9 frames of the 20000-frame run (6513, 6531, 6600, 6693, 8369, 8387,
8456, 8549, 8552, one at a time, the same steps in two passes over the
same part of the level) differ only in their top 2 to 49 lines, at the
background's scrolling edge, and the next frame is the same again: a
tear, the GBA's game still writing VRAM while the top lines are drawn,
where the host's game takes no time.
Every other frame is pixel for pixel mGBA's. A save made with the same
keys is mGBA's .sav byte for byte but for one padding byte of the slot
record (stack garbage) and so the checksum.

**The window** (sdl.c). At each VBlank (line 160) it shows the finished
picture, scaled (`SDL_RenderSetLogicalSize`, nearest neighbour, the GBA's
3:2 kept when resized), reads the events and the keys and pads, and waits
until the frame's time is up: 280896 cycles of 16.78 MHz, 59.7275 frames
a second, against a running deadline, so a short wait evens out; more than
four frames behind (a long load, a debugger) it starts again from now. No
vsync: the pace is the GBA's, not the monitor's. Measured with SDL's
dummy video driver: 1200 VBlanks in 20.3 s from start to end, loads
included.

**The sound.** The owner's decision (#1093) was that a port provides
GAX2's API however it likes. The pc platform builds GAX2 itself: lib/gax's
matched C, compiled by the host compiler, plays the songs and the sound
effects with the original's own code, so the replay (patterns, the
instruments' sequences, envelopes, vibrato, sweeps, the sound effects'
voice stealing, the volumes) is the original's by construction. What
can't run on a host is replaced, in units (platform/pc/units/lib/gax/src/,
built in place of the same files, as a src/ unit is) or in the
platform's own code:

- the ARM routines (lib/gax/asm/gax_arm_dsp.s: the resampler, the downmix
  to 8 bits, the echo, the filter) are C (gax_dsp.c) that does what the ARM
  code does, step for step: 32-bit products, arithmetic shifts, the
  halfword stores' wrap, the clamps' bounds (the filter's `mvn` gives
  -limit - 1), the loops entered at their test or not, the echo's and the
  filter's state (kept in their code, so reset by every GAX2_init), and
  the half-rate loop's word accesses, which the ARM7TDMI makes on the
  aligned word when the address isn't (a load rotated, a store aligned
  down). Tested against the ARM code itself (qemu-arm, 26000 random work
  items): the same, but for the unaligned words, where qemu doesn't do
  the ARM7TDMI's rotation and gax_dsp.c does;
- GaxChannelMix (gax_note_trigger.c) patches two instructions of its
  IWRAM copy of the resampler before each call (forward or backward
  stepping) and calls it through `GAX_CALL_ARM_R`; the unit sets which
  way the C resampler steps and calls it with the same work item.
  GaxMixerPlay and its neighbours (gax_sound_handler_mixer_play.c) read
  the handlers through views with the GBA's offsets; the unit reads
  gax_internal.h's structures and calls the C routines, and GaxMixFrame
  hands each 8-bit block to audio.c;
- GAX2_init (gax_playstart.c) carves the player state, the handler array
  and the handlers by the GBA's sizes (0x18c, 4-byte pointers) out of
  AudioContext's 0x2000-byte buffer; the unit carves them by the host's
  (sizeof) out of a 64 KB buffer of its own and copies no ARM code.
  GAX2_jingle (gax_channel_pool_alloc.c, which the game never calls) the
  same;
- the DMA settle delay (`GAX_DMA_WAIT`, ARM asm) is dropped in
  GaxResetSoundHardware, GAX_irq and GAX_stop; their register writes are
  host memory nothing reads. GAX2's fatal-error screen (gax_fatal_error.c,
  with its BIOS `swi` in gax_swi.c) prints the error and ends the run;
- the handler array's slot 2 (the layout's list of alternative layouts,
  no handler) is read by GaxCreateHandlers' search for a handler by type:
  on the GBA it holds whatever the work RAM did and the search finds no
  match; the units put a handler of no type there.

**The song data.** tools/gax_audio.py builds the music block, the
sound-effect set and the default layout as the GBA's bytes, with every
pointer a relocation in the .s next to each .bin (`.4byte gGaxMusicData
+ 0x24`, `.4byte GaxChannelInit`), and the .bin has zeros there. GAX2's
structures that hold pointers (the handler layouts and their lists, the
handler types, their child-type arrays, the song data, the instrument
tables and instruments, the sample tables) can't be read in the GBA's
layout through 8-byte pointers, so platform/pc/tools/gax_host_data.py
writes them again as C (gax_host_data.c in the build), typed with
gax_internal.h's declarations, so the host compiler lays them out: the
handler types' callbacks are the host's functions, their instance sizes
the host's sizeof. It walks from the songs (gax_songs.h's offsets), the
sound-effect voice types at the start of the music block and the default
layout, following the relocations, so it converts what the engine can
reach. The envelopes are converted too: their points are 6 bytes of
fields, which agbcc pads to 8 (an ARM struct's size is a multiple of 4)
and a host compiler doesn't. Everything else (patterns, order lists,
instrument sequences, the DSP parameters, the samples) is read in place
from the built bytes, which sound_data.S links in. The game hands
GAX2_init its song as the GBA layout's address (gSongTable's entry,
gGaxMusicData + the offset) and gGaxMusicData as the sound-effect types;
the unit's GAX2_init swaps in the converted structures (gHostGaxLayouts)
and leaves the game's parameter block pointing at them.

**The output.** On the GBA GAX_play (AudioContext::Update, from the
VCount IRQ at line 0x35) mixes a frame's samples into half of its 8-bit
buffer, and DMA1 plays the halves to Direct Sound A at the mix rate
(Timer0), re-armed at every other VBlank by GAX_irq: the blocks are
played in the order they are mixed, one a frame. The host's irq.c runs
the same IRQs (so AudioContext's fades, ducking and ambient sounds run
from the VCount IRQ as on the GBA), and at each VBlank audio.c passes
the frame's block on, or silence of the last block's length when none
was mixed (no song, or paused):

- in a window, to SDL's default audio device (sdl.c), at its rate (48 kHz
  asked for), by linear interpolation from the song's mix rate (15769 to
  42049 Hz in this game), queued about three frames ahead. The blocks
  come at the frame pacing's rate and the device takes them at its own
  clock, so the conversion runs up to 0.5% faster or slower to keep the
  queue at its length (an eighth of a semitone at most); if it runs dry
  anyway a frame of silence restarts it, and while it is far too long
  (Tab) blocks are dropped. No audio device is not an error;
- `--wav FILE`: the samples as they are, 8-bit mono at the mix rate of
  the first block (one of a song at another rate is resampled to it,
  nearest sample), from the first block on;
- the `audio BLOCKS HASH` at the end.

**The sound against mGBA.** `boottest.py gax-trace ROM ELF --keys FILE
--frames N [--level N]` plays a key file on libmgba with breakpoints on
GAX2's API (platform/pc/tools/mgba_gax_trace.c) and writes a trace: each
call the game makes, with its arguments (GAX2_init's parameter block,
GAX_play's master volume and filter, which AudioContext writes into the
block for the fades), and each block GAX2 mixed, by a hash.
`crashbandicootxs_pc --gax-trace FILE` replays the calls through the
host's GAX2 and checks each block against the GBA's: the engine,
without the game's timing. With intro.keys and boot.keys into 11 levels
(`--level` 1, 3, 6, 9, 11, 14 and 20-24: Shipwrecked, a polar stage,
Rocket Racket, Ruined, Ace of Space, Blimp Bonanza and the five bosses),
14 of the 19 songs, 665 sound effects and the fades and ducks: 21,984
blocks, every one the GBA's byte for byte. `--gax-log FILE` writes the same trace of
the game running on the host, and `boottest.py gax-calls` compares two
traces' calls without their timing: in all 12 runs the game makes the
same calls with the same arguments in the same order as on mGBA (the
song starts, sound effects, their volumes, stops), and the same music
volumes (the fades). Only when they come differs: the host's loads take
fewer VBlanks than the GBA's (see "How far it matches mGBA"), so a sound
effect starts a few VBlanks away from where it does on mGBA against the
music. The whole boot's sound is mGBA's sample for sample for its first
214 blocks (3.6 s, the boot cutscene), up to the first such sound
effect. CI replays tests/boot_gax.trace (1634 blocks) and compares the
host boot's calls with it.

**The save** (eeprom.c). `--save FILE` reads FILE at the game's first
EEPROMConfigure (if it exists) and writes it back whole at the VBlank
after the game writes the EEPROM, and at the end. The buffer holds the
chip's bits in the order the GBA sends them, the layout of mGBA's .sav:
an 8-byte block is EEPROMWrite's `data[0..3]` bytes reversed. The game
uses the 4 Kbit chip, so the file is 512 bytes; a longer file is read as
far as the chip goes.

**The link cable** (sio.c). By the owner's decision (#1093) the link
is stubbed: the host is a GBA with no cable attached, and the game's
own code runs as it does on one. The link's one use is the save menu's
"load link game" (SaveMenu::MainInput's cursor 1, from the level
select's Start or the title's load), which exchanges save records with
a second console (SaveMenu::LinkExchange, src/game/ui/save_menu_draw.cpp);
nothing probes the link at boot or anywhere else. The save menu's
constructor makes the session (`new LinkSession`, whose Reset stops the
port: RCNT 0, SIOCNT in UART mode, Timer3 stopped). With the option
picked, the exchange loops a frame at a time: LinkSession::Update
(src/engine/link/link_session.cpp) puts the port in multi-player mode
and waits for SIOCNT's SD terminal (bit 3, every console ready), and
SaveTransfer::Poll waits for the session to connect, while the screen
says "transferring data" and that B aborts. On a GBA with no cable both
terminals are pulled high: SIOCNT reads SI high (bit 2, a child) and SD
high, multi-player id 0 and no error, and a child's start bit is read
only (mGBA with no link reads the same, but keeps a written start bit,
which the game doesn't read). REG_SIOCNT is a platform call on the host
(gba/io_reg.h, HostSioCnt) that sets those read-only bits in multi-player
mode before the game reads or writes the register; the other serial
registers are plain memory (SIOMULTI0-3 are only read by the serial IRQ).
So the session starts as a child, installs its serial IRQ handler, gets
no transfer (no parent clocks one, and the platform raises no Serial
IRQ), steps its counter down to -15 in 15 frames, resets (Stop and
ResetState) and starts over, until B: LinkExchange returns 3, and the
menu goes back to its main page with "load link game" under the cursor,
as on a GBA with nothing plugged in. It never hangs and never shows
the "connection failed" message (that needs a session that connected).
platform/pc/tests/link.keys plays it (Start from the level select, "load
link game" at frame 620, B at 800 and 860, back to the level select), and
CI compares every frame's picture with mGBA's with no link
(tests/link_mgba_screens.json): all 1000 the same. Before sio.c the
host's SIOCNT was plain memory, SD read low ("bad connection"), and
the session never started (Update stopped and re-primed the port every
frame); the pictures were the same, the code it ran was not.

**DMA.** Every DMA the game starts is immediate (`DmaCopy*`, `DmaFill*`,
DMA3, through DmaSet): none is timed to VBlank, HBlank or a FIFO (only
GAX2 uses DMA1 for its FIFO, which it arms by writing the registers; the
platform's audio takes its blocks from GaxMixFrame). dma.c does an
immediate one at once; any other is only written to the registers.

**The memory** (memory.S): EWRAM, IWRAM, I/O, palette RAM, VRAM and OAM in
the GBA's order in one block, each on a 1 MB boundary. The order matters
to the allocator, which tells its heaps apart by comparing an address
with the IWRAM heap's (mem_free), the alignment to the game taking an OBJ
tile's number from the low bits of its VRAM address (company_logos.cpp,
`tilesB >> 5`).

**Swapping a unit.** `platform/pc/units/<path>.cpp` is built in place of
`src/<path>.cpp`, into the same object, so that object's dependencies and
flags still apply, and `platform/pc/units/lib/gax/src/<file>.c` in place
of `lib/gax/src/<file>.c`. It is for a unit the host can't compile or run
as it is: bios_util.cpp, and GAX2's ("The sound").

**The link.** On the GBA, games/xs/ldscript.txt places the globals that
have no C definition (games/xs/sym_iwram.txt, sym_ewram.txt) and
`objcopy --redefine-syms` gives the C++ symbols the C names of
cxx_symbols.txt. platform/pc/tools/host_link.py does both for the host,
as a linker script of `PROVIDE` assignments passed to the link:

- each linker-placed global at twice its GBA offset in gHostIwram or
  gHostEwram, under its C name and under the host's mangled name of the
  C++ symbol cxx_symbols.txt gives that C name (Irq::table is
  gIntrTable). The heaps (memory.cpp) are twice the GBA's too;
- each C name of a C++ symbol that a declaration uses
  (`asm("SetCheckpointAtPlayer")`, `extern "C" SetEntityPos`, ...) as an
  alias of the host's definition, matched by qualified name (g++ 2.9's
  mangling decoded from cxx_symbols.txt, the host's with c++filt).

The doubling is checked: `platform/pc/tools/layout_check.py` reads each
placed global's host size from the executable's debug information (the
host flags include `-femit-class-debug-always` for it) and fails if one
runs into the next. Of the 227 globals, 216 have a size: 60 are bigger
than their GBA slot (the pointers, at most 2x), none overlaps the next.
The other 11 are arrays declared without a bound (gCollectedSpawns,
gAirshipMapFrames, gHovercraftMapFrames: pointer-sized elements, so
exactly 2x), AgbEeprom's (replaced, unused on the host), the stacks and
the EWRAM heap.

The game's strcpy, strlen, strcat, strncpy, sprintf, vsprintf, rand and
srand (src/engine/util/) have the C library's names, and the executable's
would stand in for the C library's for its other users (libstdc++, the
platform's own code). platform.mk renames them in the game's objects
after compiling each (`objcopy --redefine-syms=libc_names.txt`, so
`GameStrcpy`), and host_link.py `--libs` fails the link if a game object
defines any other name the host's libc, libm or libstdc++ exports. The
game's global operator new and delete (operator_new.cpp, the game's heap)
are renamed too (GameOperatorNew & co.): C++ lets a program replace
them, but then every C++ library in the process allocates from the
game's heap, which crashed when SDL's audio loaded libjack before
AgbMain. host_link.py `--renames` binds OperatorNew & co.
(cxx_symbols.txt) to the renamed definitions.

**Host flags.** GNU C++98 with no RTTI or exceptions, as on the GBA, and
`-funsigned-char` (ARM's `char`), `-fno-strict-aliasing -fwrapv` (gcc
2.9's semantics), `-fno-builtin` (the game's own strcpy, strlen, sprintf),
`-fno-toplevel-reorder` (each file's data in source order, as agbcc
emits it: the game reads a cutscene picture's bitmap at +0x200 from its
palette, two definitions in cutscene_pictures.cpp, which g++ otherwise
emits in reverse). entity.o gets `-fkeep-inline-functions`: g++ 2.9's
`#pragma implementation` emitted Entity's inline methods there, and other
files call SetEntityPos, SetEntityPixelPos and the constructor out of
line. clang has no such option: with `HOST_CC=clang HOST_CXX=clang++`
everything compiles, but those three are missing at the link. No
`-Werror`: the build's warnings are the GBA's.

**Seams** (gba/defines.h, each nothing on the GBA). Two for the
original's own bugs:

- `NULL_READ(p)`: the ROM reads through a pointer while it can still be
  NULL. The level's first SetCheckpointAtPlayer (bonus_round.cpp) reads
  the room (`room.cat`) and the player (`gPlayer`) before either exists;
  on the GBA the reads land in the BIOS area. The pc platform points a
  NULL one at a zeroed page (gHostNullPage).
- `MEM_FREE_LOST(p)`: LevelLayers' destructor frees the room's asset
  even when it is the ROM's own data (`assetOwned != 0 || asset !=
  NULL`). On the GBA mem_free's writes land in the ROM, which ignores
  them, and the blocks it would merge with are open-bus reads (checked
  for every room asset that isn't unpacked), so nothing happens; on the
  host the ROM data is read-only memory. The pc platform skips a free of
  an address outside its heaps.

And one for an ABI difference:

- `STRUCT_RETURN_VIEW(name)`: Sprite::GetAttackBox and GetBodyBox return
  a struct aabb. The bosses call them by their C names as functions that
  fill a box through a pointer (objects.h, `GetSpriteAttackBox(&box,
  sprite)`), and dingodile.cpp also as one that returns it
  (GetSpriteAttackBox_s, which the ROM's frame needs). On the ARM those
  are the same call (g++ 2.9 returns the struct through a pointer in
  r0); on x86-64 the method takes `this` first and returns the 16-byte
  box in two registers. The pc platform defines the C names as the
  pointer form (sprite_boxes.c, calling the methods) and gives the
  struct-return view its own function, HostStructReturn_<name>.

**Shared code changed for it** (each byte-identical on the GBA, both ROMs
and maps unchanged):

- sprite_obj.hpp: `Sprite::MirrorBits`/`MirrorFlags` are declared in the
  class, not inside the anonymous union (a g++ 2.9 extension).
- A variable of an unnamed struct type has internal linkage on a modern
  g++: the tile sets (`TAGGED_RAW_ASSET`), the spawn tables
  (`ACTOR_SPAWN_TABLE`) and the room links (`LEVEL_LINKS`, both written
  by tools/levels.py) are defined `extern "C"`.
- Phase 2 (each found by a crash or a checkpoint that differed from
  mGBA's):
  - mem_alloc rounds a block up from `sizeof(struct mem_block)` and a
    pointer's alignment, not 0x13 and ~3 (the GBA's 16-byte header, the
    host's is 24);
  - EntitySpawner::Spawn indexes its table by `sizeof(*table)`, not
    `<< 2`; Starfield's `tileBase` and ScaledSprite's `dest` are
    `uintptr_t`s;
  - LoadTaggedAsset calls LZ77UnCompVram and RLUnCompVram with their
    destination (a one-argument declaration had relied on `dest` still
    being in r1);
  - Font::DrawGlyph adds `&oam_scratch` to the OAM buffer, not `this`
    (g++ 2.9 put Font's vtable pointer last, the host's ABI puts it
    first);
  - PART_FLAG_SET reads `Sprite::mirror`, not the byte at +0x28;
  - a cutscene picture's bitmap is `aligned(4)` like its palette
    (tools/gfx_manifest.py), so that the host's 16-byte alignment of a
    large array leaves no gap between them;
  - NULL_READ in SetCheckpointAtPlayer, MEM_FREE_LOST in mem_free.
- Phases 3-5 (each a crash on the host):
  - the save menu's icons (save_menu_draw.cpp) set the palette nibble in
    the byte after `Sprite::mirror`, not at +0x29, which on the host is
    inside the bank pointer;
  - Sprite::CheckPlayerContact reads the player's flags through
    NULL_READ: the save menu's icons are Sprites too, and outside a level
    gPlayer is NULL;
  - STRUCT_RETURN_VIEW in dingodile.cpp (the bosses crashed in
    GetSpriteAttackBox).
- Phase 6 (GAX2 compiled for the host):
  - GaxPlayerState's `mixBuf`, `echoBuf` and `echoTaps` and
    GaxMixerHandler's `mixBuf` (gax_internal.h), addresses kept as
    integers, are `uintptr_t`s;
  - GaxCreateHandlers sizes a handler's children array by
    `sizeof(struct GaxHandler *)`, not 4, and reads the echo taps' address
    as a `uintptr_t`; GAX2_new clears `sizeof(struct GaxSongHeader)`, not
    0x3c (the host's block is larger, and GAX_play reads its `scratch`).

**How far it matches mGBA.** With platform/pc/tests/boot.keys (Start and
A mashed through the boot cutscene, the company logos, the language
select, the title, the intro and the level select, then Jungle Jam from
frame 511, played with run, jump and spin to frame 6000), every frame's
picture hash is mGBA 0.10.5's. Until the CpuSet and the crate fixes
("64-bit hosts" above) the first difference was at frame 961, one of five
wumpa fruit from a bouncy wumpa crate drawn 5 pixels off (the part list's
compaction), and with the same play kept up, the outline crates of an
iron switch never solidified (frame 17215). Played on to frame 20000,
only the 9 torn frames above differ. CI plays boot.keys to frame 6000 and
compares it with mGBA's logs (tests/boot_mgba.json, every 60 frames,
and tests/boot_mgba_screens.json, every frame).

**Not done yet:** the gamepad mapping is built but untested with a real
pad (the tests had none). The link is stubbed by decision ("The link
cable" above), not left to do.
`vsprintf` stays as it is (4-byte argument slots, see above; renamed
GameVsprintf, and never called).
Other ABI differences that a later phase may meet: the `asm("CName")`
views with another return type (`RandRange_s32`, `GetPaletteSlot_s32`)
read a register the host doesn't promise to extend, and the game's
objects are bigger on the host, so its heaps are laid out differently
and a stale block's contents differ (as Crate's `group` showed: a field
the GBA zeroes whole, the host only in part).
