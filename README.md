[![Build and compare](https://github.com/Almamu/CrashBandicootXS-decomp/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/Almamu/CrashBandicootXS-decomp/actions/workflows/build.yml)
[![Mods](https://github.com/Almamu/CrashBandicootXS-decomp/actions/workflows/mods.yml/badge.svg?branch=main)](https://github.com/Almamu/CrashBandicootXS-decomp/actions/workflows/mods.yml)
[![Matching decompilation](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/matching.svg?raw=true)](docs/progress_report.md#progress-badges)
[![Non-matching decompilation](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/nonmatching.svg?raw=true)](docs/progress_report.md#progress-badges)
[![Functions with no matching workarounds](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/clean.svg?raw=true)](docs/progress_report.md#progress-badges)
[![Functions with matching workarounds left](https://github.com/Almamu/CrashBandicootXS-decomp/blob/badges/workarounds.svg?raw=true)](docs/progress_report.md#progress-badges)
[![decompilati.onl](https://decompilati.onl/badge/crash-bandicoot-xs-the-huge-adventure.svg)](https://decompilati.onl/projects/crash-bandicoot-xs-the-huge-adventure)
[![bytematch.dev](https://bytematch.dev/Almamu/CrashBandicootXS-decomp.svg?label=bytematch.dev&measure=matched_code_percent)](https://bytematch.dev/Almamu/CrashBandicootXS-decomp)

# Crash Bandicoot XS

A matching decompilation of Crash Bandicoot XS (Crash Bandicoot: The Huge
Adventure), Vicarious Visions' 2002 Game Boy Advance game. It builds:

* [**crashbandicootxs.gba**](https://datomatic.no-intro.org/index.php?page=show_record&s=23&n=0310) `sha1: bdd061e1b5187c0528928ec0ddb6886b1de16970` (Europe) (En,Fr,De,Es,It,Nl)

The whole ROM is built from this repository: C++ game code, C libraries,
a little hand-written assembly, and the assets as PNGs, palettes, level
JSON, XM songs, WAV samples and gettext text. You don't need a copy of
the game to build it.

- **Want to make a level or a mod?** Start at [docs/modding.md](./docs/modding.md);
  the levels themselves are in [docs/levels.md](./docs/levels.md).
- **Want to build it?** See [INSTALL.md](./INSTALL.md), then [Building](#building).
- **Want to change code?** Read [CONTRIBUTING.md](./CONTRIBUTING.md) and
  [docs/workflow.md](./docs/workflow.md) first.

## Status

- **The decompilation is complete and byte-exact.** All 2062 functions
  match and the data is at 100% (the objdiff report, see
  [docs/progress_report.md](./docs/progress_report.md)). Every ROM table is
  a typed `const` array and every asset is rebuilt from a source file.
- **The cleanup is complete.** The game code is real g++ 2.9 C++
  (`engine/src/**/*.cpp`, [docs/cplusplus.md](./docs/cplusplus.md)) with its
  classes and namespaces; pointer arithmetic and hardware addresses are
  structs, fields and `REG_*` macros; every function and global is
  declared once, in a header; and the text is gettext files. Functions
  have real names, apart from 19 small ones (mostly empty or unused) that
  keep their `sub_`/`nullsub_` placeholder, as
  [docs/naming.md](./docs/naming.md) allows. No `cleanup` issue is open.
- **Two functions keep matching workarounds**, by owner decision:
  `GAX2_init` and `GaxChannelMix`, in the GAX2 sound library. Each
  workaround has a comment saying why ([#662](https://github.com/Almamu/CrashBandicootXS-decomp/issues/662),
  [docs/matching_techniques.md](./docs/matching_techniques.md)). After a
  build, `tools/match_idioms.py --functions` prints the count.
- **Hand-written assembly** is left only where C or C++ can't produce the
  ROM's code: crt0, `IntrMain`, libgcc's `lib1funcs`, the BIOS SWI
  wrappers, GAX2's ARM DSP routines, and `itoa_arm` (`asm/itoa_arm.s`,
  [#662](https://github.com/Almamu/CrashBandicootXS-decomp/issues/662): another compiler's output, most likely armcc's; it is counted as a
  function).
- **The ARM functions of the IWRAM image** (`engine/src/engine/gfx/sprite_arm.cpp`,
  `engine/src/engine/util/string_arm.cpp`) are built with a locally patched ARM
  compiler, `agbcp_arm_patched` (see INSTALL.md): the ROM's ARM compiler
  was a later build whose code stock agbcc_arm can't produce ([#553](https://github.com/Almamu/CrashBandicootXS-decomp/issues/553),
  [docs/matching/iwram-image.md](./docs/matching/iwram-image.md)).

The project's progress is also on [decompilati.onl](https://decompilati.onl/projects/crash-bandicoot-xs-the-huge-adventure)
and [bytematch.dev](https://bytematch.dev/Almamu/CrashBandicootXS-decomp).

### Crash Bandicoot 2: N-Tranced

The repository has started on a second ROM, Crash Bandicoot 2: N-Tranced
(Europe), with `make GAME=n2 compare` ([#1106](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1106)). It is an early bootstrap:
a disassembly with XS's names where they are known, a few shared sources
that match both games, and the rest of the ROM incbin'd from
`baserom_n2.gba`, which this build needs. See
[docs/n_tranced.md](./docs/n_tranced.md); its progress is on
[decompilati.onl](https://decompilati.onl/projects/crash-bandicoot-2-n-tranced).
The next steps are open issues: the build layout
([#1108](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1108),
[#1109](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1109)),
the bootstrap ([#1110](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1110)),
the shared structs and code
([#1111](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1111),
[#1116](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1116)),
its sound engine ([#1112](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1112)),
levels ([#1113](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1113)),
text ([#1114](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1114)),
graphics ([#1115](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1115))
and its own code ([#1117](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1117)).

## Building

See [INSTALL.md](./INSTALL.md) for the toolchain. Then:

```
make compare
```

builds `crashbandicootxs.gba` and checks its SHA-1 against
[`games/xs/checksum.sha1`](./games/xs/checksum.sha1). It must print
`crashbandicootxs.gba: OK` (`sha1sum` translates the "OK" in some
locales).

A plain `make` builds the ROM from whatever data is in the tree without
checking it: edited graphics, new levels, a mod. `make compare` only
applies to the original data. The Makefile tracks header dependencies,
so a plain `make` rebuilds what an edit touches. Before opening a PR,
check from clean:

```
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
rm -rf build objdiff.json && make NON_MATCHING=1 report && objdiff-cli report generate -o report.json
```

The second line builds the objdiff progress report
([docs/progress_report.md](./docs/progress_report.md)). It must show
2062/2062 functions and 100% data. The compile rules and the per-object
compiler choices are in [`compile.mk`](./compile.mk).

## Modding

The game's content is data, and a plain `make` builds whatever data the
tree has: no code change and no `baserom.gba`. **[docs/modding.md](./docs/modding.md)
is the modders' entry point**: what a mod can change, how to write one,
and the sample mod.

- **The sample mod.** [`mods/sample/`](./mods/sample/) adds a fifth world
  with a new room, theme, song, recoloured vehicle stage, boss level,
  cutscene and level names in the six languages. `make mod MOD=mods/sample`
  applies it to a copy of the tree in `build/mods/sample` and builds
  `build/mods/sample/crashbandicootxs.gba`; your own tree isn't changed.
  CI builds and boot-tests it in mGBA on every PR.
- **Levels** ([docs/levels.md](./docs/levels.md)): rooms (one picture per
  BG layer, the collision layer as an image, and a `room.json` of
  entities), the level table, themes and their colour cycles, the
  level-select pages and what unlocks them, level names, actor
  categories (the vehicle and boss stages, with their own graphics,
  palettes, sky and animation), boss levels and what clearing a level
  does.
- **The save** ([docs/levels.md, "The save"](./docs/levels.md#the-save))
  grows with the levels: past 28 levels the build switches to a 64 Kbit
  EEPROM. A build with other level data gets another save version, from
  a hash of the level data, so a save made with other levels is reset
  instead of misread.
- **Graphics** ([docs/graphics.md](./docs/graphics.md)): indexed PNGs and
  JASC palettes under `games/xs/graphics/`, sprite banks and new frames.
- **Text and translations** ([docs/text.md](./docs/text.md)): every string
  and the credits, as gettext files in `games/xs/text/` (the template
  `crashxs.pot` and a `.po` per translation). `make text-check` runs the
  translation checks.
- **Audio** ([docs/audio.md](./docs/audio.md)): XM songs and WAV samples
  under `games/xs/sound/`, new songs on the ROM's instruments, and which song plays
  where.
- **Cutscenes** ([docs/data.md, "Cutscenes"](./docs/data.md#cutscenes)):
  slides, pictures, timing, music and text.

New or grown data moves everything after it: the ROM's contents can
shift and grow, and the modded ROM still works
([docs/shifting.md](./docs/shifting.md)). Only the original data is
pinned to the original layout.

### What a mod can't do yet

- **New instruments and samples** for new songs: [#1118](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1118).
- **Level and cutscene editors, and a platform library for ports**: [#1093](https://github.com/Almamu/CrashBandicootXS-decomp/issues/1093).
- **New behaviour**: a new actor, boss, power or entity type is code; the
  data decides what the existing kinds look like and where they spawn,
  not what they do.
- **N-Tranced**: the modding pipeline is XS's; N-Tranced's data isn't
  decompiled yet.

The full list is in [docs/modding.md, "What a mod can't do yet"](./docs/modding.md#what-a-mod-cant-do-yet).

## How the code is organised

`engine/src/` follows the code's namespaces: `Engine` in `engine/src/engine/` (`sys/`,
`gfx/`, `actor/`, `audio/`, `save/`, `link/`, `text/`, `util/`) and
`Game` in `engine/src/game/` (`crash/`, `entities/`, `enemies/`, `bosses/`,
`vehicles/`, `crates/`, `objects/`, `level/`, `ui/`), with `engine/include/`
mirroring it. Each area's ROM tables are in its `data/` folder. No file
is named after a ROM address. XS's own files (its levels, cutscenes and
credits in `games/xs/data/`, its `graphics/`, `sound/` and `text/`, the
linker script) are under `games/xs/`, N-Tranced's under `games/n2/`. See
[CONTRIBUTING.md, "Source layout"](./CONTRIBUTING.md#source-layout) and
[docs/file_layout.md](./docs/file_layout.md).

The third-party and SDK code linked into the ROM is C under `lib/`: the
GAX2 sound engine, Nintendo's AgbEeprom library and libgcc
([docs/libraries.md](./docs/libraries.md)). The GBA hardware is the
platform's, under `platform/gba/`: its headers (`gba/io_reg.h`, ...),
crt0, `IntrMain` and the BIOS SWI wrappers
([docs/platforms.md](./docs/platforms.md)); a port to another system
supplies its own platform there.

`games/xs/expected/` holds frozen copies of the original disassembly, the target
the progress report diffs the build against. Never edit them; see
[games/xs/expected/README.md](./games/xs/expected/README.md).

## Contributing

Read [CONTRIBUTING.md](./CONTRIBUTING.md) and
[docs/workflow.md](./docs/workflow.md) before changing code. Every change
has to keep the ROM byte-identical with the original data, and only a
full clean `make compare` proves it. A change under `games/xs/text/` also needs
`make text-check`, and a change to what a doc says updates that doc in
the same PR.

## AI-assisted decompilation

Much of this project's work (reversing functions, the byte-exact
reconstruction, mapping the ROM, and the documentation) has been done
with AI coding agents (Claude Code), under human supervision and review.
It is the main way the project has made progress, and it will keep being
used. So:

- **Nothing is trusted on the AI's say-so.** A function counts as matched
  only after the full clean rebuild in [docs/workflow.md](./docs/workflow.md):
  an isolated compile matching is not proof, a byte-identical
  `make compare` is. This applies to a human's change as much as an
  agent's.
- **AI-assisted contributions are welcome**, with your own tooling too,
  as long as they meet the same standard: the ROM stays byte-identical,
  and nothing is presented as more certain than it is.
  [docs/matching.md](./docs/matching.md) and
  [docs/matching/](./docs/matching/) give an agent (or a human) starting a
  fresh session the context to keep going.
- **Names stay honest.** A placeholder name (`gUnknown_XXXXXXXX`,
  `unk_XX`) is valid; replace it only when genuinely confident, per
  [docs/naming.md](./docs/naming.md). An invented name is worse than an
  honest placeholder.
- **Humans make the calls that matter**: what gets merged, how the
  project is organised, and every judgement that isn't mechanical
  verification.

If that's a reason not to contribute, that's a reasonable position; just
know it going in.

## Documentation

**Modding and the game's data**

- [docs/modding.md](./docs/modding.md): **the modders' entry point**, the sample mod and what a mod can and can't change
- [docs/editors.md](./docs/editors.md): the contract editors build on: the data files' JSON Schemas, `--json` errors and `make data-check`
- [docs/levels.md](./docs/levels.md): rooms, layers, entities, themes, level-select pages, actor categories, boss levels and the save
- [docs/graphics.md](./docs/graphics.md): the graphics, the manifest, sprite banks and the sprite/actor system
- [docs/text.md](./docs/text.md): the game's text as gettext files, the keys, the checks and the translators' workflow
- [docs/audio.md](./docs/audio.md): the Shin'en GAX2 sound engine's data, and how songs and samples are built
- [docs/data.md](./docs/data.md): the ROM's data tables as C++ `const` arrays, the asset pipeline and the cutscenes
- [docs/shifting.md](./docs/shifting.md): how the ROM's contents move and grow, `make shiftcheck` and the mGBA boot test
- [docs/screen.md](./docs/screen.md), [docs/fixed_point.md](./docs/fixed_point.md), [docs/dma.md](./docs/dma.md): the screen size, the fixed-point formats and the DMA macros
- [docs/flags/](./docs/flags/): the game's flag and state values, one page per field

**Building and contributing**

- [INSTALL.md](./INSTALL.md): **the toolchain and building the ROM**
- [CONTRIBUTING.md](./CONTRIBUTING.md): **picking up work, human or agent, and getting it to a PR**
- [docs/workflow.md](./docs/workflow.md): **the required loop for changing matched code**
- [docs/naming.md](./docs/naming.md): **the naming convention, and what a rename touches**
- [docs/cplusplus.md](./docs/cplusplus.md): the game is g++ 2.9 C++: the agbcp/old_agbcp compilers and the namespaces
- [docs/file_layout.md](./docs/file_layout.md): the source tree's layout and the map from the old file names
- [docs/headers.md](./docs/headers.md): where declarations live
- [docs/libraries.md](./docs/libraries.md): the libraries under `lib/` (GAX2, AgbEeprom, libgcc) and the platform's libagbsyscall
- [docs/platforms.md](./docs/platforms.md): `platform/gba/` (the hardware headers, crt0, IntrMain, the BIOS calls) and what a port provides
- [docs/progress_report.md](./docs/progress_report.md): the objdiff progress report, the badges and the progress sites
- [docs/n_tranced.md](./docs/n_tranced.md): the second ROM, Crash Bandicoot 2: N-Tranced, and the `GAME` switch

**Matching history**

- [docs/matching_techniques.md](./docs/matching_techniques.md): the matching techniques (compilers and flags, source shapes, register pins, the `engine/include/match.h` idioms)
- [docs/matching/](./docs/matching/): the matching references (the IWRAM image, per-file flags) and the [archive](./docs/matching/archive/)
- [docs/matching.md](./docs/matching.md): the frozen per-pass matching log
- [docs/casts.md](./docs/casts.md), [docs/simplify.md](./docs/simplify.md): the cleanup of casts and of matching-only source shapes
- [docs/rom_map.md](./docs/rom_map.md), [docs/data_map.md](./docs/data_map.md): the historical whole-ROM code and data maps used to plan the work

The [Kirby & The Amazing Mirror](https://github.com/jiangzhengwenjz/katam/)
decompilation is of a game on a very similar codebase.

## License

This repository's source code, tools and documentation are licensed under
the [GNU General Public License v3.0](./LICENSE).

That covers this project's own work. It doesn't grant any rights to
Crash Bandicoot XS itself: its characters, art, music and text, including
the assets extracted into this repository (`games/xs/graphics/`, `games/xs/sound/`, `games/xs/text/`,
`games/xs/data/levels/`), belong to their owners. The libraries in [`lib/`](./lib/) (Shin'en's GAX2 sound engine,
Nintendo's AgbEeprom library, libgcc) are reconstructions of third-party
code and keep their authors' terms; see [docs/libraries.md](./docs/libraries.md).

## Credits

- [@normmatt](https://github.com/normmatt) for the initial repository setup he did for the [Sonic Advance 2](https://github.com/SAT-R/sa2) decompilation
- The [Sonic Advance 2](https://github.com/SAT-R/sa2) decompilation, the starting point that got this project building
- [Pokemon Reverse Engineering Tools](https://github.com/pret) for some of the GBA tooling
