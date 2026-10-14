[![Build and compare](https://github.com/Almamu/CrashBandicootXS-decomp/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/Almamu/CrashBandicootXS-decomp/actions/workflows/build.yml)
[![Matching decompilation](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/matching.svg?raw=true)](docs/decomp_dev.md#progress-badges)
[![Non-matching decompilation](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/nonmatching.svg?raw=true)](docs/decomp_dev.md#progress-badges)
[![Functions with no matching workarounds](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/clean.svg?raw=true)](docs/decomp_dev.md#progress-badges)
[![Functions with matching workarounds left](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/workarounds.svg?raw=true)](docs/decomp_dev.md#progress-badges)

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

- **Code: all 2062 functions are byte-exact; 2061 are C.** The ARM functions in
  the IWRAM image (`src/engine/gfx/sprite_arm.cpp`,
  `src/engine/util/string_arm.cpp`) are built with a locally patched ARM
  compiler (`agbcp_arm_patched`, see INSTALL.md): the ROM's ARM compiler
  is a later, unreleased build whose return code and jump rules stock
  agbcc_arm can't produce (#553,
  [docs/matching/iwram-image.md](./docs/matching/iwram-image.md)). The
  other one, `itoa_arm`, is assembly by owner decision (`asm/itoa_arm.s`,
  #662): its ROM code is another compiler's, most likely armcc's, which
  no gcc reproduces. It is still counted as a function. Hand-written
  assembly (crt0, IntrMain, libgcc's lib1funcs, the BIOS SWI wrappers,
  GAX2's ARM DSP routines) isn't counted.
- **Data: 100%.** Every ROM table is a typed C `const` array in
  `src/data/` (a library's own tables are in `lib/<name>/data/`), and
  every asset is rebuilt from a source file (see "Data and assets"
  below).
- **Done:** the cleanup pass (raw pointer arithmetic and hardware
  addresses replaced with structs, fields and `REG_*` macros), naming
  (no `sub_XXXXXXXX` function names left), the library split (#573), the
  file layout (#575) and the headers (#574: every function and global is
  declared once, in a header).
- **Matching workarounds:** the *clean functions* and *workarounds left*
  badges above (redrawn by CI on every push to main) show how many
  functions still need a register pin, empty-asm nudge
  (`include/match.h`), instruction asm, retyped field access, scoped
  `volatile` or self-init to come out byte-exact. Each has a comment
  saying why (#662,
  [docs/matching_techniques.md](./docs/matching_techniques.md)). After a
  build, `tools/match_idioms.py --functions` prints the numbers per
  directory and `--functions --files` lists the functions.
- **What's left** is code quality: the remaining placeholder names
  (`gUnknown_`, `gStaticData_`, `nullsub_N`, `unk_XX` fields), compiler
  warnings, formatting, and the matching workarounds above. It's
  tracked as `cleanup` issues on GitHub.


`src/` follows the code's namespaces: the engine in `src/engine/`
(`sys/`, `gfx/`, `actor/`, `audio/`, `save/`, `link/`, `text/`,
`util/`) and the game in `src/game/` (`crash/`, `entities/`,
`enemies/`, `bosses/`, `vehicles/`, `crates/`, `objects/`, `level/`,
`ui/`), with `include/` mirroring it, and every file is named after what
it holds, never after its ROM address (see
[CONTRIBUTING.md](./CONTRIBUTING.md#source-layout) and
[docs/file_layout.md](./docs/file_layout.md)). The
third-party and SDK code linked into the ROM (the GAX2 sound engine,
Nintendo's AgbEeprom library, libgcc, the BIOS SWI wrappers) is under
`lib/` (see [docs/libraries.md](./docs/libraries.md)). Declarations live
in `include/` and in each library's `include/` (see
[docs/headers.md](./docs/headers.md)).

## Building

See [INSTALL.md](./INSTALL.md) for the toolchain. Then:

```
make compare
```

builds `crashbandicootxs.gba` and checks its SHA-1. It must print
`crashbandicootxs.gba: OK` (`sha1sum` translates the "OK" in some
locales). A plain `make` builds the ROM from whatever is in the tree
without checking it, for example with edited graphics or levels. The
Makefile tracks header dependencies, so
after editing a header a plain `make` rebuilds the files that include
it. Before opening a PR, check from clean:

```
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
rm -rf build objdiff.json && make NON_MATCHING=1 report && objdiff-cli report generate -o report.json
```

The second line builds the [decomp.dev](https://decomp.dev) progress
report (see [docs/decomp_dev.md](./docs/decomp_dev.md)). It must show
2062/2062 functions and 100% data.

## Data and assets

The build regenerates every asset under `build/` from its source; no
generated file is checked in.

- **Graphics** (`graphics/`: indexed PNGs, most carrying their palette,
  and JASC `.pal` OBJ palettes, in folders by what the game does with
  them: `cutscenes/`, `fonts/`, `logos/`, `title/`, `menus/`,
  `level_select/`, `sprites/`, `vehicles/`, `bosses/`): `tools/grit` lays
  out bitmaps, tilesets and tilemaps, `tools/gbagfx` does the LZ77
  compression, and `tools/bin2c.py` turns the result into the bytes a
  `src/data/*.cpp` file `#include`s. The rules are in `graphics.mk`.
  `graphics/manifest.json` records where the ROM has each asset, for
  `tools/gfx_manifest.py extract`. See [docs/graphics.md](./docs/graphics.md)
  and [docs/data.md](./docs/data.md), "Resources (grit-style)".
- **Levels** (`data/levels/<level>/<slot>/`: a `room.json`, one picture
  per BG layer and the collision layer as an image, for each of the 41
  rooms): `tools/levels.py` builds the room data, the level assets and the
  BG tile sets, which it makes from the pictures. The rules are in
  `levels.mk`. A room added there is built too, after the ROM's data. See
  [docs/levels.md](./docs/levels.md).
- **Audio** (`sound/`: XM songs, WAV samples and JSON manifests):
  `tools/gax_audio.py` rebuilds the GAX2 music and sound-effect data, and
  `tools/sfx_table.py` the sound-effect table. See
  [docs/audio.md](./docs/audio.md).
- **Text** (`text/`: the gettext template `crashxs.pot` and a `.po` per
  language): msgfmt compiles each language and `tools/text.py` writes the
  UI and cutscene string tables from them. The rules are in `text.mk`;
  `make text-check` runs the translation checks and `make text-update`
  merges a template change into the `.po` files. See
  [docs/text.md](./docs/text.md).
- **Tables** are C++ `const` arrays in `src/data/`, placed at their ROM
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
  [docs/matching.md](./docs/matching.md) and [docs/matching/](./docs/matching/)
  exist so an AI agent (or a human)
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
- [docs/data.md](./docs/data.md) - how ROM data tables became C `const` arrays in `src/data/`, linked in ROM order, and the asset pipeline
- [docs/text.md](./docs/text.md) - the game's text as gettext files (`text/`): the keys, the checks and the translators' workflow
- [docs/libraries.md](./docs/libraries.md) - the third-party/SDK libraries under `lib/` (GAX2, AgbEeprom, libgcc, libagbsyscall): layout, headers, flags
- [docs/cplusplus.md](./docs/cplusplus.md) - the game is g++ 2.x C++: what the ROM shows, the agbcp/old_agbcp C++ compilers, and building objects from C++ source (every `.cpp` under `src/`, `CXX_OBJS`)
- [docs/file_layout.md](./docs/file_layout.md) - the source tree's layout (by namespace) and the map from the old file names
- [docs/headers.md](./docs/headers.md) - where declarations live, and the codegen exceptions
- [docs/audio.md](./docs/audio.md) - how the Shin'en GAX2 sound engine's data is laid out and rebuilt
- [docs/graphics.md](./docs/graphics.md) - how graphics were extracted, and notes on the sprite/actor system
- [docs/levels.md](./docs/levels.md) - the level data: rooms, layers, chunk streams, entities
- [docs/shifting.md](./docs/shifting.md) - the ROM's contents can move and grow: `make shiftcheck` and the mGBA boot test (`tools/boottest.py`)
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
