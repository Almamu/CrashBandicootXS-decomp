Matching decompilation of Crash Bandicoot XS (Crash Bandicoot: The Huge Adventure).

It builds the following ROM:
* [**crashbandicootxs.gba**](https://datomatic.no-intro.org/index.php?page=show_record&s=23&n=0310) `sha1: bdd061e1b5187c0528928ec0ddb6886b1de16970` (Europe) (En,Fr,De,Es,It,Nl)

The whole ROM is built from the sources in this repository: C, a little
hand-written assembly, and the assets as PNGs, palettes, level JSON, XM
songs and WAV samples. `baserom.gba` isn't needed to build it.

> :warning: **Changing code? Read [CONTRIBUTING.md](./CONTRIBUTING.md) and
> [docs/workflow.md](./docs/workflow.md) first.** Every change has to keep
> the ROM byte-identical, and only a full clean `make compare` proves it.

## Current state

- **Code: all 2059 functions are byte-exact C.** Two of them, ARM
  functions in the IWRAM image (`src/iwram/`), are built with a locally
  patched agbcc_arm (`agbcc_arm_patched`, see INSTALL.md): the ROM's ARM
  compiler is a later, unreleased build whose prologue and return code
  stock agbcc_arm can't produce (#553,
  [docs/matching/iwram-image.md](./docs/matching/iwram-image.md)).
- **Data: 100%.** Every ROM table is a typed C `const` array in
  `src/data/` (a library's own tables are in `lib/<name>/data/`), and
  every asset is rebuilt from a source file (see "Data and assets"
  below).
- **Done:** the cleanup pass (raw pointer arithmetic and hardware
  addresses replaced with structs, fields and `REG_*` macros), naming
  (no `sub_XXXXXXXX` function names left), the library split (#573), the
  file layout (#575) and the headers (#574: every function and global is
  declared once, in a header).
- **What's left** is code quality: the remaining placeholder names
  (`gUnknown_`, `gStaticData_`, `nullsub_N`, `unk_XX` fields), compiler
  warnings, formatting, and documenting the matching workarounds. It's
  tracked as `cleanup` issues on GitHub.

`src/` has one directory per subsystem (`system/`, `gfx/`, `text/`,
`audio/`, `objects/`, `player/`, `crates/`, `enemies/`, `level/`,
`bosses/`, `actor/`, `vehicle/`, `menus/`, `hud/`, ...), with the ROM
data tables in `src/data/`, and every file is named after what it holds
(see [docs/file_layout_plan.md](./docs/file_layout_plan.md)). The
third-party and SDK code linked into the ROM (the GAX2 sound engine,
Nintendo's AgbEeprom library, libgcc, the BIOS SWI wrappers) is under
`lib/` (see [docs/libraries.md](./docs/libraries.md)). Declarations live
in `include/` and in each library's `include/` (see
[docs/headers_plan.md](./docs/headers_plan.md)).
[docs/status/](./docs/status/) has the per-system breakdown.

## Building

See [INSTALL.md](./INSTALL.md) for the toolchain. Then:

```
make compare
```

builds `crashbandicootxs.gba` and checks its SHA-1. It must print
`crashbandicootxs.gba: OK` (`sha1sum` translates the "OK" in some
locales). The Makefile tracks header dependencies, so
after editing a header a plain `make` rebuilds the files that include
it. Before opening a PR, check from clean:

```
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
rm -rf build objdiff.json && make NON_MATCHING=1 report && objdiff-cli report generate -o report.json
```

The second line builds the [decomp.dev](https://decomp.dev) progress
report (see [docs/decomp_dev.md](./docs/decomp_dev.md)). It must show
2059/2059 functions and 100% data.

## Data and assets

The build regenerates every asset under `build/` from its source; no
generated file is checked in.

- **Graphics** (`graphics/`: PNGs, JASC `.pal` palettes and a few raw
  `.bin` blobs): `tools/grit` lays out bitmaps, tilesets and tilemaps,
  `tools/gbagfx` converts tiles and palettes and does the LZ77
  compression, and `tools/bin2c.py` turns the result into the bytes a
  `src/data/*.c` file `#include`s. The rules are in `graphics.mk`. See
  [docs/graphics.md](./docs/graphics.md) and
  [docs/data.md](./docs/data.md), "Resources (grit-style)".
- **Levels** (`data/levels/`: a `room.json` and one tilemap per layer for
  each of the 41 rooms): `tools/levels.py` builds the room data and the
  level assets. The rules are in `levels.mk`. See
  [docs/levels.md](./docs/levels.md).
- **Audio** (`sound/`: XM songs, WAV samples and JSON manifests):
  `tools/gax_audio.py` rebuilds the GAX2 music and sound-effect data, and
  `tools/sfx_table.py` the sound-effect table. See
  [docs/audio.md](./docs/audio.md).
- **Tables** are C `const` arrays in `src/data/`, placed at their ROM
  addresses by `ldscript.txt`. See [docs/data.md](./docs/data.md).

`expected/` holds frozen copies of the original disassembly: the target
the progress report diffs the build against. Never edit them; see
[expected/README.md](./expected/README.md).

## AI-assisted decompilation

A large part of this project's work (function reversal, byte-exact C
reconstruction, ROM address-space mapping, and documentation) has been
done with the help of AI coding agents (Claude Code), under human
supervision and review. This isn't a secret or an experiment on the side -
it's the primary way this project has been making progress, and it's
expected to keep being used that way. A few things follow from that:

- **Nothing is trusted on the AI's say-so.** Every function marked
  "matched" has gone through the full clean-rebuild verification described
  in [docs/workflow.md](./docs/workflow.md) - an isolated compile matching
  is not proof, only a byte-identical `make compare` against the real ROM
  is. This applies equally whether a human or an AI made the change.
- **Contributions from AI-assisted work are welcome**, including from
  contributors using their own AI tooling - as long as the same
  verification standard is met (the ROM stays byte-identical, and nothing
  is presented as more certain than it is).
  [docs/matching.md](./docs/matching.md), [docs/matching/](./docs/matching/)
  and [docs/status/](./docs/status/) exist so an AI agent (or a human)
  picking up a fresh session has enough context to keep going without
  re-deriving everything from scratch.
- **Semantic naming should stay honest.** A placeholder name
  (`gUnknown_XXXXXXXX`, `unk_XX`) is a valid name - AI agents (and
  humans) should only replace it with a real name when genuinely
  confident, per [docs/naming.md](./docs/naming.md). An overconfident or
  invented name is worse than an honest placeholder.
- **Humans still make the calls that matter**: what gets merged, how the
  project is organized, and any judgment call that isn't purely mechanical
  verification.

If this isn't the kind of project you want to contribute to for that
reason, that's a completely reasonable position - just know it going in.

## Notes

- [INSTALL.md](./INSTALL.md) - **setting up the toolchain and building the ROM**
- [CONTRIBUTING.md](./CONTRIBUTING.md) - **how to pick up work from the issue tracker, human or agent, and get it to a PR**
- [docs/workflow.md](./docs/workflow.md) - **the required loop for changing matched code, must be followed for every change**
- [docs/naming.md](./docs/naming.md) - **the naming convention, and the checklist of what a rename touches**
- [docs/status/](./docs/status/) - per-system matched/parked function status
- [docs/data.md](./docs/data.md) - how ROM data tables became C `const` arrays in `src/data/`, linked in ROM order, and the asset pipeline
- [docs/libraries.md](./docs/libraries.md) - the third-party/SDK libraries under `lib/` (GAX2, AgbEeprom, libgcc, libagbsyscall): layout, headers, flags
- [docs/headers_plan.md](./docs/headers_plan.md) - where declarations live, and the codegen exceptions
- [docs/audio.md](./docs/audio.md) - how the Shin'en GAX2 sound engine's data is laid out and rebuilt
- [docs/graphics.md](./docs/graphics.md) - how graphics were extracted, and notes on the sprite/actor system
- [docs/levels.md](./docs/levels.md) - the level data: rooms, layers, chunk streams, entities
- [docs/matching_techniques.md](./docs/matching_techniques.md) - the matching techniques (compilers and flags, source shapes, register pins and holds, the `include/match.h` asm idioms)
- [docs/matching/](./docs/matching/) - the current matching references (the IWRAM image, per-file flags), plus [archive/](./docs/matching/archive/) and [docs/matching.md](./docs/matching.md), the frozen per-pass matching logs
- [docs/decomp_dev.md](./docs/decomp_dev.md) - how this project's [decomp.dev](https://decomp.dev) progress report is generated in CI
- [docs/rom_map.md](./docs/rom_map.md) - the historical whole-ROM map used to plan the matching work
- The [Kirby & The Amazing Mirror](https://github.com/jiangzhengwenjz/katam/) decompilation uses a very similar codebase, as it was written by the same dev team (Dimps)
- `ldscript.txt` tells the linker the order in which files are linked, which places every object at its ROM address

## Credits

- [@normmatt](https://github.com/normmatt) for the basic initial repository setup that he did for [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation
- [Sonic Adventure 2](https://github.com/SAT-R/sa2) decompilation as a starting point to get the project building too
- [Pokemon Reverse Engineering Tools](https://github.com/pret) for some of the GBA tooling
