# Shifting the ROM

The ROM can be rebuilt with its contents at other addresses: code or
data that grows moves everything linked after it, and the ROM still
works. So a table, a room, a picture or a sample can grow in place;
nothing has to go after the IWRAM image only to keep the ROM's
addresses (#967).

This holds because every address the ROM stores is a relocation: a C or
C++ pointer, a `.4byte symbol` in assembly, or a pointer in the GAX2
sound data, which `tools/gax_audio.py` writes as assembly with
relocations. Nothing stores an address as a number. `make shiftcheck`
checks it, and `tools/boottest.py` plays shifted ROMs in mGBA.

## What can and can't move

- **ROM code and data** (`ldscript.txt`'s `ROM` section and the `iwram`
  image's load address): anything can grow, shrink or move. crt0 copies
  the IWRAM image from `__iwram_lma`, a symbol.
- **Alignment** comes from the objects themselves (their sections'
  alignment), not from where they used to be. The one place that had a
  pad worked out by hand, the 2 zero bytes in front of the GAX2
  sound-effect set at 0x084C0006, is now the linker's fill in front of
  a `.balign 4` (`data/data.s`); `gGaxSfxData` is at 0x084C0008, where
  the set starts.
- **The IWRAM image** runs at 0x03000000, and the IWRAM variables after
  it are placed by `sym_iwram.txt` at fixed offsets, starting with
  `gIntrTable` at 0x030009E8, right after the image. crt0 copies
  `gIntrTable - IntrMain_Buffer` bytes. So the image (IntrMain, the ARM
  routines, `src/engine/sys/iwram_data.cpp`) can't grow past 0x9E8 bytes
  without moving those variables. EWRAM is laid out the same way
  (`sym_ewram.txt`). That is RAM, not ROM: it doesn't limit the ROM's
  data.
- **`make report`** (docs/decomp_dev.md) still describes the original
  layout: `tools/report_units.py`'s `DATA_END` and `IWRAM_LMA` are the
  ROM's addresses, and it fails on a shifted tree. Only `make compare`
  and the report need the original layout; a plain `make` builds
  whatever is in the tree.

## `make shiftcheck`

`tools/shiftcheck.py` links the ROM again with `ld -q` (`--emit-relocs`,
so the ELF keeps every relocation), once as it is and once for each of
several shifts: N bytes of padding inserted in front of one object in
a copy of `ldscript.txt`. The default shifts are in the code (0x100
bytes in front of `bios_util.o`, 2 in front of `audio.o`), in front of
the data, the rooms (0x1004), the sprite banks, the sound blocks (1 and
2 bytes, to move them off their alignment), the level assets and the
vtables (3). For each, it compares the shifted link with the unshifted
one, input section by input section (from the two link maps):

1. Every byte no relocation covers must be the same. A difference is
   something the build worked out from the layout without a relocation.
2. Every `R_ARM_ABS32` word must hold the new address of what it pointed
   at, so a `symbol + addend` that runs out of its symbol's object into
   another one is caught.
3. No word that no relocation covers may hold a ROM symbol's address (a
   Thumb function's: the address + 1): that is a pointer written as a
   number, which stays put when its target moves.

Check 3 only sees a pointer to the start of a symbol: one into the middle
of an object, written as a number, looks like any other data. Before the
GAX2 blocks were relocatable, it found 417 of their 1,697 pointers, the
ones that hold a symbol's address (the handler functions'); all 1,697
are relocations now.

Check 3 also hits data that only looks like a pointer: 8 words of
pixels, PCM and one LZ77 stream in the whole ROM hold a symbol's address
by chance. `KNOWN_DATA` in the script lists them, by object, offset and
value. An entry stops applying when its word changes; when an edit to
the data makes a new one, the check prints the entry to add, once the
word is known to be data.

`make shiftcheck` needs only the objects (no `baserom.gba`) and takes a
few seconds. `make shiftcheck SHIFTCHECK_FLAGS=--roms` also writes each
shifted ROM, `build/crashbandicootxs/shiftcheck/<shift>.gba`, next to
its ELF. `--shift NAME:OBJECT:BYTES` (repeatable) replaces the default
shifts.

## `tools/boottest.py`

```
tools/boottest.py run crashbandicootxs.gba crashbandicootxs.elf --out control.json
tools/boottest.py run build/crashbandicootxs/shiftcheck/data.gba build/crashbandicootxs/shiftcheck/data.elf --out data.json
tools/boottest.py compare control.json data.json
```

It boots a ROM in mGBA (`mgba`, the SDL frontend, with SDL's dummy video
and audio drivers) and drives it through mGBA's GDB stub with scripted
input, from a fresh copy of the ROM (no save file). The keys go in at a
breakpoint in `UpdateKeys`, right after it reads `REG_KEYINPUT`, once a
frame, and a script picks a level by setting `LevelSelectLoop`'s return
value. The scripts:

- `cutscene`: a new game, then the intro cutscene without skipping it;
- `save`: Start at the level select opens the save menu; save to slot 1,
  then back to the level select and into jungle jam;
- `levelN`: through the menus, level N from the level select, then 900
  frames of running, jumping, attacking and steering: jungle jam (0),
  shipwrecked (1), rocket racket (6), ruined (9), ace of space (11),
  blimp bonanza (14) and the five bosses (20-24);
- `levelN_room1`: the same, starting at the level's second room (a
  breakpoint in `SelectRoom` sets `roomIndex`): the polar stages of
  frostbite cavern (3), snow crash (5) and snow job (10).

The log records the frame of each `PlaySong`, `LoadRoom` (the room's
symbol), `InitActorCategory` (the polar, jetpack and boss stages),
`PlayCutscene`, `RunSaveMenu` and `EEPROMWrite`; every 60 frames a hash
of palette RAM, VRAM and OAM (the picture: no ROM addresses in it) and
whether the GAX2 mixer ran; and the save file's hash. `compare` fails if
two logs differ in any of it. The game is deterministic for the same
input, so a shifted ROM gives the same log as the original.

It isn't part of CI: it needs mGBA, and mGBA's GDB stub takes about 60 ms a
stop, so a script runs at about 16 frames a second: the 16 scripts take
about 30 minutes a ROM. `make shiftcheck` is in CI.

## Results

The audit (#967) found one kind of fixed address: the GAX2 sound data.
`tools/gax_audio.py` linked both blocks at their ROM addresses
(`BASE_ADDR = 0x0855BCB4`, `SFX_BASE_ADDR = 0x084C0006`) and wrote the
engine's handler functions as numbers (`0x080393FD`, ...): 1,697
pointers, none of them relocations, and a check that stopped the build
if the sound-effect set changed size. Anything that moved the code or
the data before them broke the sound. With the 0x100-byte `data` shift
and those blocks, the boot test shows it: the mixer never runs, and the
game hangs at frame 481, when the intro cutscene starts its song. The
blocks are relocatable now (docs/audio.md).

Nothing else stores an address as a number:

- crt0 and IntrMain use symbols (`__iwram_lma`, `gIntrTable`,
  `IntrMain_Buffer`, `AgbMain`) and hardware addresses only;
- the C and C++ data's pointers are all relocations (`make shiftcheck`,
  checks 1 and 3); `src/` has no ROM address literal outside comments,
  and the build-time tools (`levels.py`, `bin2c.py`, the graphics
  tools) write symbols, not addresses (their ROM addresses are for
  `extract`, which reads `baserom.gba`);
- `expected/` is for the progress report only; the ROM's build doesn't
  read it;
- nothing depends on an alignment it doesn't ask for: the shifts by 1, 2
  and 3 bytes pass `make shiftcheck`.

With the fix, all nine default shifts pass `make shiftcheck`, and the
`data` and `code` shifted ROMs play every script the same as the
original, frame for frame: the cutscene, the save (the same save file),
jungle jam, shipwrecked, ruined, ace of space, rocket racket and blimp
bonanza (jetpack), the polar stages of frostbite cavern, snow crash and
snow job, and the five bosses, with the mixer running throughout.
