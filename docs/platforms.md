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
- GAX2 (`lib/gax/`, its `.4byte` pointer blobs included) and AgbEeprom
  (`lib/agb_eeprom/`, which writes the DMA registers with addresses as
  `u32`s) are replaced at their APIs, not compiled for a host.

The pc platform (below) is the check that this holds: every `src/`
object compiles and links on a 64-bit host, in CI.

## `platform/pc/`

The game for a 64-bit host (Linux, a stock g++/gcc). Phases 1 and 2 of
#1093: it compiles, links and boots headless. It runs the game's own
frame loop on the host's main thread, with scripted input, and hashes
palette RAM, VRAM and OAM at checkpoints, which tools/boottest.py compares
with mGBA running the ROM on the same input. Nothing is shown or heard
yet (the PPU and audio are phases 3 and 6).

```
make PLATFORM=pc        # build/pc/crashbandicootxs_pc; XS only
build/pc/crashbandicootxs_pc --keys platform/pc/tests/boot.keys --frames 960
```

It needs no ARM toolchain, agbcc or baserom, only what the data
generators need (CI: `.github/workflows/host.yml`).

```
platform/pc/platform.mk            the build: the Makefile includes it instead of
                                   its GBA link and compile rules (compile.mk), with
                                   the rest (object lists, generated data) shared,
                                   under build/pc/crashbandicootxs/
platform/pc/include/gba/*.h        the GBA's headers, included by relative path, with
                                   REG_BASE, EWRAM_START, IWRAM_START, PLTT, VRAM, OAM
                                   on host arrays, DmaSet a call, REG_KEYINPUT a call,
                                   LAYOUT_ASSERT empty, IWRAM_DATA/EWRAM_DATA empty,
                                   NULL_READ and MEM_FREE_LOST (below)
platform/pc/include/agb_syscall.h  the GBA's prototypes
platform/pc/src/
    memory.S                       the host memory (gba/host.h), in the GBA's order
    dma.c                          DmaSet: an immediate transfer done at once
    bios.c                         the BIOS calls in C but VBlankIntrWait (irq.c)
    irq.c                          the frame loop: VBlankIntrWait, the scanlines,
                                   VCOUNT/DISPSTAT, IF, and IntrMain's dispatch
    input.c                        REG_KEYINPUT from the key file, the frame count,
                                   the checkpoints
    main.c                         the command line, then AgbMain
    sha1.c                         the checkpoints' hash
    gax.c, eeprom.c                GAX2's API (silent stubs) and AgbEeprom's (a RAM
                                   buffer), in place of lib/gax and lib/agb_eeprom
    libgcc.c                       __divsi3 & co., which the game calls by name
    sound_data.S                   gSfxTable and gGaxMusicData, data/data.s's
                                   blocks the game reads, as the built bytes
platform/pc/units/                 units built in place of src/'s
    engine/sys/bios_util.cpp       DivMod in C (the GBA's is an inline `svc #6`)
platform/pc/libc_names.txt         the game's functions named as the C library's
platform/pc/tests/boot.keys        the headless boot's input (CI)
platform/pc/tests/boot_mgba.json   mGBA's log of it (boottest.py), what CI compares
platform/pc/tools/host_link.py     the link's symbol assignments (below)
platform/pc/tools/layout_check.py  checks the placed globals fit (below)
```

**Running it.** `crashbandicootxs_pc [--frames N] [--vblanks N] [--keys
FILE] [--checkpoint-every N] [--dump DIR] [--quiet]`. A game frame is a
read of REG_KEYINPUT (KeyInput::Update reads it once a frame; the
platform's io_reg.h makes the read a call of HostReadKeys), as it is in
tools/boottest.py, which sets the keys at a breakpoint right after that
read: frame 1 is the first read. `--frames N` stops at frame N+1
(`end done at frame N+1`, boottest's "done"). The key file holds a line
`FRAME KEYS` for each change: KEYS (`-`, a number, or names joined by
`+`: A B SELECT START RIGHT LEFT UP DOWN R L) are held from FRAME on.
Every `--checkpoint-every` frames (60) it prints `checkpoint FRAME HASH
PICTURE`: HASH is boottest's (the SHA-1 of palette RAM, VRAM and OAM),
PICTURE the same with the OAM bits the display ignores cleared (a hidden
entry's attributes, a regular entry's matrix bits), because the game
leaves those as whatever its stack held (DrawSpritePieces' `struct
oam_attrs`), which differs between the GBA and a host. `--dump DIR`
writes each checkpoint's bytes to DIR/FRAME.bin.

`tools/boottest.py host EXE --keys FILE --frames N` runs it and writes
boottest's log; `boottest.py run ROM ELF --keys FILE --frames N` plays
the same key file on mGBA, and `compare` of the two compares the
checkpoints' picture hashes. Both take `--every N` and `--dump DIR`.

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
(SetEepromTimerIntr installs a no-op), and the link never connects. A
VBlankIntrWait with the VBlank IRQ off stops the run with an error (the
GBA would hang).

**DMA.** Every DMA the game starts is immediate (`DmaCopy*`, `DmaFill*`,
DMA3, through DmaSet): none is timed to VBlank, HBlank or a FIFO (only
GAX2 used DMA1 for its FIFO, and the platform replaces GAX2). dma.c does
an immediate one at once; any other is only written to the registers.

**The memory** (memory.S): EWRAM, IWRAM, I/O, palette RAM, VRAM and OAM in
the GBA's order in one block, each on a 1 MB boundary. The order matters
to the allocator, which tells its heaps apart by comparing an address
with the IWRAM heap's (mem_free), the alignment to the game taking an OBJ
tile's number from the low bits of its VRAM address (company_logos.cpp,
`tilesB >> 5`).

**Swapping a unit.** `platform/pc/units/<path>.cpp` is built in place of
`src/<path>.cpp`, into the same object, so that object's dependencies and
flags still apply. It is for a unit the host can't compile as it is; only
bios_util.cpp so far.

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
runs into the next. Of the 227 globals, 214 have a size: 59 are bigger
than their GBA slot (the pointers, at most 2x), none overlaps the next.
The other 13 are arrays declared without a bound (gCollectedSpawns,
gAirshipMapFrames, gHovercraftMapFrames: pointer-sized elements, so
exactly 2x), GAX2's and AgbEeprom's (replaced, unused on the host), the
stacks and the EWRAM heap.

The game's strcpy, strlen, strcat, strncpy, sprintf, vsprintf, rand and
srand (src/engine/util/) have the C library's names, and the executable's
would stand in for the C library's for its other users (libstdc++, the
platform's own code). platform.mk renames them in the game's objects
after compiling each (`objcopy --redefine-syms=libc_names.txt`, so
`GameStrcpy`), and host_link.py `--libs` fails the link if a game object
defines any other name the host's libc, libm or libstdc++ exports, but
the global operator new and delete, which C++ lets a program replace
(operator_new.cpp, the game's heap).

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

**Two seams for the original's own bugs** (gba/defines.h, both nothing
on the GBA):

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

**How far it matches mGBA.** With platform/pc/tests/boot.keys (Start and
A mashed through the boot cutscene, the company logos, the language
select, the title, the intro and the level select, then Jungle Jam from
frame 511, played with run, jump and spin), every frame's picture hash is
mGBA 0.10.5's up to frame 960. The first difference is at frame 961: one
of five wumpa fruit flying off from a bouncy wumpa crate is 5 pixels to
the side, and the OBJ tiles after it differ. gRandSeed is the same in
both every frame up to there, it doesn't depend on the host's
optimisation level (-O1 and -O2 agree) or on what a NULL_READ reads
(zeros or the BIOS's open-bus 0xE3A02004); not found yet. CI runs the
boot to frame 960 and compares it with mGBA's log
(tests/boot_mgba.json).

**Not done yet** (the later phases): the PPU and a window, live input
and frame timing, saving to a file (the EEPROM is a RAM buffer), audio
(GAX2's API is silent; gGaxMusicData's pointers are the GBA's, which
nothing on the host reads), the link. `vsprintf` stays as it is (4-byte
argument slots, see above; renamed GameVsprintf, and never called).
Other ABI differences that a later phase may meet: the `asm("CName")`
views with another return type (`RandRange_s32`, `GetPaletteSlot_s32`)
read a register the host doesn't promise to extend, and the game's
objects are bigger on the host, so its heaps are laid out differently
and a stale block's contents differ.
