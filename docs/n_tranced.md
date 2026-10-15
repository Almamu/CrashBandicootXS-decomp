# Crash Bandicoot 2: N-Tranced

The repository is starting to build a second ROM, Crash Bandicoot 2:
N-Tranced (Europe), next to Crash Bandicoot XS. The plan, the feasibility
numbers and the ground rules are in #1106. In short: XS stays
byte-identical on every change, things that do the same job in both
games keep the XS names, and differences go behind `GAME_XS` /
`GAME_N2`.

| | XS | N-Tranced |
|---|---|---|
| ROM | `crashbandicootxs.gba` | `ntranced.gba` |
| SHA-1 | `bdd061e1b5187c0528928ec0ddb6886b1de16970` | `ee9234ca15ba2caa71fad7d6af71e27b2030ae0b` |
| Build | `make compare` | `make GAME=n2 compare` |
| Build dir | `build/crashbandicootxs/` | `build/ntranced/` |
| Checksum | `games/xs/checksum.sha1` | `games/n2/checksum.sha1` |
| Linker script | `games/xs/ldscript.txt` | `games/n2/ldscript.txt` (generated) |
| RAM symbols | `games/xs/sym_ewram.txt`, `games/xs/sym_iwram.txt` | `games/n2/symbols.txt` |
| Defines | `GAME_XS=1` | `GAME_N2=1` |
| Original ROM | not needed | `baserom_n2.gba`, in the project root |

## The `GAME` switch

The Makefile's `GAME` variable picks the ROM: `xs` (the default) or `n2`.
`make`, `make compare` and every other target with no `GAME` are XS's,
exactly as before. With `GAME=n2` the Makefile includes
[`games/n2/game.mk`](../games/n2/game.mk) instead of its XS part, so the
two builds share only the toolchain variables at the top (`AS`, `LD`,
`CC1FLAGS`, `CPPFLAGS`, ...). Each game defines `GAME_XS` or `GAME_N2`,
for the preprocessor and as an assembler symbol.

Each game's own files are in `games/<game>/` (#1108): its checksum, its
linker script, its RAM symbols (XS's `sym_ewram.txt`/`sym_iwram.txt`,
N-Tranced's `symbols.txt`) and, for XS, the frozen disassembly the
progress report diffs against (`games/xs/expected/`). How the two games
share sources is "Sharing sources" below.

The compile rules and the per-object compiler choices (which objects
old_agbcp builds, `-fno-implement-inlines`, `-O1`, ...) are in
[`compile.mk`](../compile.mk), included by both builds. They are XS's;
where N-Tranced's original build chose differently, `games/n2/game.mk`
says so before including it (`NEW_COMPILER_OBJS`: objects N-Tranced built
with agbcp where XS used old_agbcp, e.g. `vram_dma_queue.o`).

## Checking both ROMs

`make compare` checks XS against `games/xs/checksum.sha1` and
`make GAME=n2 compare` checks N-Tranced against
`games/n2/checksum.sha1`. Before a PR, check both from clean:

```
rm -rf build crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map && make compare
rm -rf build/ntranced ntranced.elf ntranced.gba ntranced.map && make GAME=n2 compare
```

CI does the same as for XS. `build.yml` builds and compares N-Tranced
after XS, on pushes and same-repository PRs. `fork-rom-check.yml` builds a
fork PR's N-Tranced (after a maintainer approves the run) and checks it
against **main's** `games/n2/checksum.sha1`, so a PR can't pass by editing
the checksum. Both need the N-Tranced ROM: the `BASEROM_N2_URL` secret,
fetched with the same `BASEROM_USERNAME`/`BASEROM_PASSWORD` as XS's. The
N-Tranced steps are skipped until that secret exists.

## Sharing sources (#1109)

The two games share XS's sources, with N-Tranced's own on top:

- **`src/` and `include/`** are XS's, and shared. N-Tranced builds the
  `src/` files `games/n2/objects.txt` lists, with `GAME_N2` defined (XS's
  build defines `GAME_XS`).
- **`games/n2/src/<path>`**, when it exists, replaces `src/<path>` in
  N-Tranced's build: the same object (`build/ntranced/src/<path>.o`), so
  compile.mk's per-object flags apply to it too. `objects.txt` can name
  it either way. **`games/n2/include/`** comes before `include/` on
  N-Tranced's include path, for a header of its own or its copy of one.
  Only C++ sources can be replaced (all the game code is C++).
- **XS-only files** need nothing: N-Tranced doesn't list them.

No XS file moves for N-Tranced.

### `#ifdef GAME_N2` or a copy

A difference stays in the shared file when it is small and contained:

- a block of a struct's members (the layouts that differ: #1111);
- a constant, a table size, a different value;
- a few lines of a function.

```c
#ifdef GAME_N2
    s32 extraState;        // 0x2C
#endif
```

```c
#ifdef GAME_N2
    count = DoTheN2Thing(count);
#else
    count = DoTheXsThing(count);
#endif // GAME_N2
```

- Test `GAME_N2` (XS is the `#else`, or nothing when N-Tranced only adds
  lines); never nest them, and end a block longer than a few lines with
  `#endif // GAME_N2`.
- A generated constant that comes from the game's data (`LEVEL_COUNT`,
  `ENTITY_TYPES`, ...) needs no `#ifdef`: each game generates its own
  (`build/<rom>/include/`).
- A declaration that only one game has sits under its `#ifdef` in the
  shared header, so each game's build still declares everything exactly
  once (docs/headers.md).

When a function differs throughout, most of a file does, or the file is
N-Tranced's own, it is a copy in `games/n2/src/` instead: start it with
`cp src/<path> games/n2/src/<path>` and change what N-Tranced does
differently. Moving code out of a shared file into copies (or back) is
fine as long as both ROMs keep matching.

### Names

- **Same job, same name.** A function, member, global or table that does
  the same thing in both games keeps XS's name, even where N-Tranced
  reordered or rewrote it.
- N-Tranced's own things are named by docs/naming.md's rules, with the
  same placeholders (`sub_<address>`, `gUnknown_<address>`, `unk_XX`)
  until they are understood.

Every change keeps both ROMs matching: `make compare` and
`make GAME=n2 compare`, from clean.

## The N-Tranced bootstrap (#1110)

N-Tranced builds from a disassembly, the shared sources that already
match it, and the original's bytes:

- [`games/n2/asm/`](../games/n2/asm/): the code,
  `0x08000000`-`0x0805BEA8` (2,711 functions), cut into
  `code_<address>.s` files around the shared sources. Generated by
  [`games/n2/disasm.py`](../games/n2/disasm.py) with gbadisasm
  (jiangzhengwenjz/gbadisasm, `new_features`) from
  [`games/n2/gbadisasm.cfg`](../games/n2/gbadisasm.cfg).
- [`games/n2/objects.txt`](../games/n2/objects.txt): the shared sources
  (`src/...`) N-Tranced links in place of the disassembly, with the
  N-Tranced address and size of each one's code; and, after the code,
  the data objects linked in place of the incbin'd data (the level
  assets). They are built for
  N-Tranced into `build/ntranced/src/` with `GAME_N2` and compile.mk's
  rules. The first four are `line.cpp`, `time_format.cpp`,
  `iwram_alloc.cpp` and `vram_dma_queue.cpp`.
- [`games/n2/symbols.txt`](../games/n2/symbols.txt): the N-Tranced
  addresses of its RAM variables, for the linker script, until N-Tranced's
  RAM is laid out from sources.
- [`games/n2/data/`](../games/n2/data/): everything after the code
  that isn't built from sources yet, incbin'd from `baserom_n2.gba`
  (`data_<address>.s`, cut around the data objects), with a label at each
  of the 516 addresses the code points at: the XS name
  ([`games/n2/data_names.txt`](../games/n2/data_names.txt)) or
  `gUnknown_<address>`. Generated by `disasm.py` too.
- [`games/n2/ldscript.txt`](../games/n2/ldscript.txt), generated by
  `disasm.py` from `objects.txt`, links them all in ROM order; nothing
  moves.

### Linking a shared source into N-Tranced

1. Check the source's code is the same in N-Tranced: same functions, same
   order, contiguous, and byte-identical once compiled (`GAME_N2` blocks,
   or a copy in `games/n2/src/`, where it isn't: "Sharing sources"). The
   comparison's "exact" (`tools/nt_compare`) is a good start but not
   proof: N-Tranced's build sometimes used the other compiler for a file
   (`vram_dma_queue.o`).
2. Add it to `objects.txt` (address, size of its `.text`, source).
3. Give every function the disassembly still calls in it the name the
   source defines, in `gbadisasm.cfg` (`disasm.py` lists them), and add
   the RAM/data symbols it uses to `symbols.txt`.
4. `python3 games/n2/disasm.py path/to/gbadisasm`, then
   `make GAME=n2 compare`. If it doesn't match, try the other compiler
   (`NEW_COMPILER_OBJS`) before changing the source.

A source that includes a generated header (`constants/levels.h`, ...)
can't be built for N-Tranced yet: those come from the game's data, and
N-Tranced's aren't generated yet (`build/ntranced/include/`, #1113).

`gbadisasm.cfg` seeds the disassembly with crt0, the N-Tranced address of
each XS function the XS/N-Tranced comparison found in it (#1107) and the
targets of the ROM's code pointers. 760 of its 2,703 functions have their
XS name; the rest are `sub_<address>`. `symbols.txt` names 76 of its RAM
variables and `data_names.txt` 19 data tables after XS's, and the
disassembly's literal-pool words use the names, or the data's
`gUnknown_<address>` labels (`.4byte gLevelTable`), or the code's own
labels: no word in the code holds a raw ROM address.

The names come from the comparison (`tools/nt_compare/run.py`, #1107), and
only confident ones (docs/naming.md): `name_cfg.py` names a function when
an XS function only one N-Tranced address maps to starts there (or a
close match lands a few bytes in), and `name_data.py` names an address at
least two of the matched functions' pool words agree on. To add the
names a newer comparison finds, and regenerate the disassembly and the
linker script (also after changing the config or `objects.txt`):

```
python3 tools/nt_compare/run.py
python3 tools/nt_compare/name_cfg.py
python3 tools/nt_compare/name_data.py
python3 games/n2/disasm.py path/to/gbadisasm/gbadisasm
make GAME=n2 compare
```

The ROM can't shift yet (unlike XS's, docs/shifting.md), although the
code can: the data is the original's bytes, and the pointers in it
(about 44,000 aligned words in it look like ROM addresses, 40,459 into
the data and 3,445 into the code) stay where they are when what they
point at moves. It shifts once its data is source, as XS's is: the
level, text and graphics data (#1113, #1114, #1115) and the tables.

## Levels (#1113)

N-Tranced's level data is XS's format (docs/levels.md): its 36 rooms are
in [`games/n2/data/levels/`](../games/n2/data/levels/), extracted by
`tools/levels.py` the way XS's are, in the same form (each room's
`room.json`, a picture per BG layer and `collision.png`):

```
python3 tools/levels.py --levels-dir games/n2/data/levels extract-rooms baserom_n2.gba 081d0430
```

`extract-rooms` walks the level table (`gLevelTable`, `0x081D0430`) and
checks what `extract` checks for XS: every layer's chunks re-encode to
the ROM's bytes (2 rooms need `pad_overrides`, as 2 of XS's do), and the
8 BG tile sets the build makes from the pictures are the ROM's, and so is
every layer's cells. The folders are `level<NN>/<slot>` (the level's
index, and `main`/`part<N>`/`bonus`/`gem_path` as in data/levels) until
the levels have names, the tile sets are `gStaticData_<address>`, and
the entity types are numbers (there is no `entity_types.json` yet: 110
types, XS has 93).

What differs from XS, in `levels.py`:

- Each room's data is in another order in the ROM: N-Tranced has the
  type counts, the entity list and the links between the parameter
  offsets and the chunk grids.
- Its extra rooms' records set `unk_0C` (room.json's `"unk_0C"`).
- Its collision layers use 18 values XS's don't:
  [`collision_legend.json`](../games/n2/data/levels/collision_legend.json)
  is XS's legend plus those, not identified yet.

**N-Tranced's level data is built from these sources** (#1113), as XS's
is, by [`games/n2/levels.mk`](../games/n2/levels.mk) with
`tools/levels.py --levels-dir`, in place of the incbin:

| What | Where (ROM) | Source |
|---|---|---|
| The rooms' data (level_desc, layers, chunk grids, palette, entity lists), 4 regions | `0x081E44C4`-`0x082B3914`, between the BG0 tile sets | `games/n2/src/game/level/data/level_rooms_<N>.cpp` (`levels.py c`) |
| The 3 BG0 tile sets (8bpp) | between the regions | `level_bg0_tiles_<N>.cpp` (`levels.py tileset`) |
| The 5 BG1-3 tile sets (4bpp, LZ77) | `0x085346AC`-`0x08544635`, in two runs | `level_bg_tiles_<N>.cpp` |
| The 36 level assets | `0x08544638`-`0x0869B8CC` | `src/game/level/data/level_assets.cpp` (shared) |

That's 2,313,432 bytes, 28.9% of its data. A room's parts are in another
order than XS's (`levels.json`'s `"room_layout"`: the type counts, entity
list and links before the chunk grids, the groups before the layer
descriptors) and N-Tranced has 110 entity types (`"entity_types"`; XS
93): `levels.py` takes both from the game's `levels.json`. The tile
sets' names (`gStaticData_<address>`) and the regions' (`level_rooms_<N>`)
are placeholders until the themes and levels have names, and
`constants/levels.h` and `constants/categories.h` are empty for now
(`levels.mk`). Still to do for #1113: the level table, its themes and the
level select, and names.

## Graphics (#1115)

N-Tranced's graphics are built as XS's are (docs/graphics.md), from
[`games/n2/graphics/manifest.json`](../games/n2/graphics/manifest.json),
which has XS's format: its entries have N-Tranced's addresses, and its
`objects` are N-Tranced's graphics class objects (in `objects.txt` too).
A graphic both games have keeps XS's source: an entry whose file isn't
under `games/n2/graphics/` is XS's `graphics/<path>`, so the continue
prompt's three pictures, the same bytes in both ROMs, have one PNG each.
Only N-Tranced's own art is under `games/n2/graphics/`. A class keeps
XS's name and its parts XS's C names ("same job, same name"; both builds
rename with `cxx_symbols.txt`, and `check` says so if one isn't there).

The rules are XS's: [`graphics_rules.mk`](../graphics_rules.mk) has the
ones the manifest drives (palettes, BG packages, tile sheets, the
category graphics, the classes), for the game's `GRAPHICS_DIR`; XS's
`graphics.mk` includes it with the rules for its other assets, and
[`games/n2/graphics.mk`](../games/n2/graphics.mk) for N-Tranced.
`tools/gfx_manifest.py --graphics-dir games/n2/graphics` works on
N-Tranced's manifest:

```
python3 tools/gfx_manifest.py --graphics-dir games/n2/graphics check
python3 tools/gfx_manifest.py --graphics-dir games/n2/graphics extract baserom_n2.gba --check
```

`extract` (without `--check`) writes into `games/n2/graphics/` only what
XS's `graphics/` doesn't have the same; `--check` compares every source,
N-Tranced's or the shared XS one, with N-Tranced's ROM.

| What | Where (ROM) | Source |
|---|---|---|
| The pause screen (N-Tranced's own) and the continue prompt's three BG packages (XS's) | `0x0852D868`-`0x085346AC` | `games/n2/src/game/ui/data/menu_graphics.cpp` |

Every LZ77 stream N-Tranced has is one gbagfx makes, but 28 of them
only with `-search 1` (no VRAM-safe minimum distance): a built file in
`GFX_LZ_SEARCH1` is compressed that way (`graphics_rules.mk`).

## Progress

N-Tranced has its own project on [decompilati.onl](https://decompilati.onl).
It has no objdiff report yet, so
[`games/n2/progress.py`](../games/n2/progress.py) reads its progress from
the build and prints the document decompilati.onl's webhook takes
([docs](https://decompilati.onl/docs)), with the same measures as XS's
report:

- **code**, in bytes: the code, `0x08000000` to the start of the
  incbin'd data. **Matching** is the shared sources' (`games/n2/objects.txt`,
  built from `src/`, byte-identical); the disassembly (`games/n2/asm/`) is
  still to do.
- **functions:** every function of `ntranced.elf`; matching, the shared
  sources'.
- **data:** the rest of the ROM. **Done** is the data objects' bytes
  (`objects.txt`'s entries after the code, built from sources: the level
  assets, 17.5%); the rest is still incbin'd from `baserom_n2.gba`.
- **areas:** XS's report categories (System, Graphics, Actor, ...,
  `tools/report_units.py`). A function has the category of the XS function
  whose name the disassembly gives it (`gbadisasm.cfg`); N-Tranced's own
  code, and what isn't identified yet, is "Not identified yet". Plus "ROM
  data". The categories need the XS build (`crashbandicootxs.elf`); without
  it every function is in "Not identified yet".

On pushes to `main`, `build.yml`'s "Send N-Tranced's progress to
decompilati.onl" step runs it after the N-Tranced compare and POSTs the
result to `/api/v1/progress`, with the commit and version `europe`, using
the repository secret `DECOMPILATION2_PROGRESS_TOKEN`. Without the secret
the step does nothing, and a failed upload doesn't fail the build. To see
what would be sent: `make && make GAME=n2 && games/n2/progress.py`.


decompilati.onl's GitHub App imports each `<version>_report` artifact of a
successful run on `main` into the project linked to the repository. Here
that's XS's objdiff report (`europe_report`, 100%), so if the N-Tranced
project imports it too, it shows XS's numbers in place of its own (it did
after #1123: 2,062 functions, 100%). N-Tranced's project must take its
progress from this webhook only, not from the App.
