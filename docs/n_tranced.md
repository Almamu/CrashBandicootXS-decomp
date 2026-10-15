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
   `tools/nt_compare`'s `match.json` gives the candidates: the XS objects
   whose functions are all "exact", in the same order and as contiguous
   in N-Tranced. `tools/nt_compare/n2_objects.py` checks them for real:
   it builds each for N-Tranced and compares it with the ROM, and lists
   the RAM symbols it needs. A short function N-Tranced has several copies of can be
   matched to the wrong copy (`dup` > 1), which breaks an object's order:
   look for the whole object's bytes too (`operator_new.o`,
   `actor_spawn_collected.o`), and check a name the comparison gave such
   a function (`ClearCollectedSpawns` was another copy's).
2. Add it to `objects.txt` (address, size of its `.text`, source; for
   libgcc's functions the object, `lib/libgcc/_divdi3.o`). A code object's
   `.rodata` that the script doesn't place is discarded, as XS's script
   does (the agb_eeprom objects' tables).
3. Give every function the disassembly still calls in it the name the
   source defines, in `gbadisasm.cfg` (`disasm.py` lists them), and add
   the RAM/data symbols it uses to `symbols.txt`. A function the
   disassembly hasn't found (one only a pointer reaches) needs its
   `thumb_func` line before the split can cut after the object. A name
   the disassembly uses inside an object that the object doesn't define
   (a lib1funcs stub with no global name) gets a `PROVIDE` in the linker
   script, from the object's first function (`disasm.py`).
4. `python3 games/n2/disasm.py path/to/gbadisasm`, then
   `make GAME=n2 compare`. If it doesn't match, try the other compiler
   (`NEW_COMPILER_OBJS`), or agbcp without `-fprologue-bugfix`
   (`N2_NO_PROLOGUE_BUGFIX_OBJS`: a leaf function that pushes `lr` and
   returns with `pop {r0}; bx r0`), before changing the source.

A source that includes a generated header (`constants/levels.h`, ...)
gets N-Tranced's placeholder for it (`games/n2/levels.mk`): only the
counts the shared headers' declarations need (`LEVEL_COUNT` 29,
`UI_TEXT_COUNT` 196, `THEME_COUNT` 12, `ENTITY_COUNT` 110,
`CRATE_KIND_COUNT` 26, `SFX_COUNT` 64, `SONG_COUNT` 21), N-Tranced's
own, until #1113
generates the headers from its data. A source that needs another of
their constants doesn't build for N-Tranced yet.

`gbadisasm.cfg` seeds the disassembly with crt0, the N-Tranced address of
each XS function the XS/N-Tranced comparison found in it (#1107) and the
targets of the ROM's code pointers. 785 of its 2,705 functions have their
XS name; the rest are `sub_<address>`. `symbols.txt` names 76 of its RAM
variables and `data_names.txt` 37 data symbols after XS's, and the
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

### Struct layouts (#1111)

Where N-Tranced's code shows a struct or class laid out otherwise than
XS's, the header has both, under `GAME_N2`, and the shared code compiles
to each game's offsets. The layout asserts (`LAYOUT_ASSERT`) are XS's
sizes: N-Tranced's build checks only `N2_LAYOUT_ASSERT`, one per layout
of N-Tranced's its code shows (`platform/gba/include/gba/defines.h`). So
far:

- `struct hitbox_quad`: `w` and `h` are u16 (XS's u8 and two bytes of
  padding).
- `Entity` (0x28 bytes, XS 0x1C): three words at +0x0C its `Reset` sets
  (1, 0, 0), so the flags are at +0x18, the box (its sizes u16) at +0x1C
  and the vtable pointer at +0x24; and two more virtual methods, slot 2
  (before `GetBounds`) and 12 (after the destructor), empty in Entity
  (`UnkVirtual2`, `UnkVirtual12`).
- `Sprite` (0x60 bytes, XS 0x40): Entity's 12 bytes, a word before `tag`
  and three after `affine`, which its `Reset` sets; two more virtual
  methods, slots 13 and 15 (`GetPriority` 14, `ApplyVelocity` 16).
  `MovingSprite` is 0x98 (XS 0x78).
- `gCrateHitResponse`'s rows: `ATTACK_KIND_COUNT`, 8 (XS 7).

`tools/nt_compare`'s `structs.txt` gives the members' offsets the code
shows in both games, and `n2_objects.py` which objects a layout makes
match.

### Tables shared with XS (#1131)

`tools/nt_compare/xs_data_objects.py` lists XS's data objects that
N-Tranced has whole: each one's bytes in N-Tranced's ROM as one run, and
every pointer in it to the N-Tranced address of the same symbol. Those are
linked into N-Tranced from XS's sources, as the code's are, with
`data_names.txt` giving XS's names to what N-Tranced's code points at in
them (`disasm.py` stops at a pointer into an object to an unnamed
`gUnknown_`):

| Source | N-Tranced |
|---|---|
| `src/engine/gfx/data/obj_piece_sizes.cpp` | `0x081CDFEC` |
| `src/game/crash/data/input_ctrl_motion_set.cpp` | `0x081CE454` |
| `src/game/level/data/terrain_types.cpp` | `0x081D0E10` |
| `lib/libgcc/data/clz_tab.c` | `0x085291D0` |
| `lib/agb_eeprom/data/eeprom_data.c` | `0x087F8430` |

`pause_menu_bg.cpp` and `continue_prompt_bgs.cpp` match too, but they
include `game/ui/menus.hpp`, which needs the level select's and the
worlds' counts: they come with N-Tranced's level select. Most of XS's
other tables are in N-Tranced as well, but inside data objects that hold
other tables too and differ as a whole: those become N-Tranced's own
sources, table by table.

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
every layer's cells. The folders are `<level>/<slot>`: the level's name
and `main`/`part<N>`/`bonus`/`gem_path` as in data/levels. The names are
`levels.json`'s `"level_names"`, one per level of `gLevelTable`, which
`extract-rooms` keeps: each level's own name (its record's `name_text`, an
index into the UI strings, 41-69). Its 29 levels are 24 and then the 5
bosses:

| | Levels (theme) |
|---|---|
| 0-11 | island_intro (5), prints_of_persia (0), lagoony_tunes (4), globe_trottin (6), pharaohs_funhouse (1), runaway_rug (0), tiki_torture (2), hoppin_coffins (1), barrel_roll (6), flockful_of_seagulls (4), magma_mania (2), run_from_the_sun (3) |
| 12-23 | now_its_istanbul (0), mister_lava_lava (2), water_logged (4), slip_n_slidin_sphinx (1), rocks_can_roll (6), rock_the_casaba (0), eruption_disruption (2), spaced_out (3), king_too_uncommon (1), wild_nile_ride (1), 101_arabian_kites (0), fire_walker (2) |
| 24-28 | evil_crunch (7), evil_coco (8), fake_crash (9), n_trance (10), n_tropy (11) |

Only 21 of them have rooms in this format (the others, the atlasphere's
and the jet ski's among them, are N-Tranced's own kinds of level). The 8
tile sets are named by the levels that use them, `levels.json`'s
`"tileset_names"` (kept by `extract-rooms` too): `gArabiaBg0Tiles`,
`gVolcanoBg0Tiles` and `gEgyptBg0Tiles` (BG0), and `gArabiaBgTiles`,
`gArabiaSkyBgTiles` (Runaway Rug, 101 Arabian Kites, Evil Crunch, N.
Tropy's first part), `gEgyptBgTiles`, `gVolcanoBgTiles` and
`gIslandBgTiles` (BG1-3).

Its 110 entity types (XS has 93) are in
[`entity_types.json`](../games/n2/data/levels/entity_types.json), one per
`gEntitySpawnFuncs` slot (`0x081D0244`), as XS's: `room.json` gives a
placed entity's type by its name, and the build generates
`constants/entities.h` from it. 45 have names: the ones whose spawner is
XS's (the crates, `WUMPA`, ...) keep XS's name, and the ones whose
spawner loads a sprite bank no other type does are named after it (its
`Bank(n)` call, `0x08007D80`: `POT_CARRIER`, `MUMMY`, `N_TRANCE`, ...),
their spawner `Spawn<Name>` in `gbadisasm.cfg`. The others stay numbers
until their spawner is read: the ones sharing a bank (`traps`,
`platforms`, `gems`, ...), whose spawner each type's entry gives, and 21
empty spawners (`bx lr`, XS's `SpawnNoEntity`).

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
93): `levels.py` takes both from the game's `levels.json`. The regions'
names (`level_rooms_<N>`) are placeholders, and the `constants/*.h`
headers are placeholders with only the counts the shared headers need
(`levels.mk`, "Linking a shared source into N-Tranced"). Still to do for
#1113: the level table, its themes and the level select as sources, and
the entity types' names.

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
| The sprite tile pool (49 banks) and the 93 OBJ palettes | `0x082B3914`-`0x084768B4` | `games/n2/src/engine/gfx/data/sprite_tiles.cpp` |
| The sprite banks' tables (animations, frames, piece arrays) | `0x084768B4`-`0x08492ACB` | `games/n2/src/engine/gfx/data/sprite_banks_*.cpp` |
| The pause screen (N-Tranced's own) and the continue prompt's three BG packages (XS's) | `0x0852D868`-`0x085346AC` | `games/n2/src/game/ui/data/menu_graphics.cpp` |
| The framed sprite sheet of the water levels' categories (262 frames) | `0x080BF458` | `games/n2/src/game/vehicles/ocean/data/ocean_sprite_sheet.cpp` |
| The framed sprite sheet of the space levels' categories (213 frames) | `0x08106C58` | `games/n2/src/game/vehicles/space/data/space_sprite_sheet.cpp` |

The framed sheets (XS's format, docs/graphics.md "The framed sheets") are
extracted in the sheet's order into one folder, `frames/`, until the
actor categories that use them are known: XS's tools split a sheet by its
categories' animation tables. Their PNGs are drawn in the first 16
colours of the category palette at the entry's `view_palette`, only for
viewing. Some of their frames are 3 tiles a side, which XS's aren't
(`framed_gfx.py to-bin-folder --any-size`, `FRAMED_TO_BIN_FLAGS`).

Every LZ77 stream N-Tranced has is one gbagfx makes, but 28 of them
only with `-search 1` (no VRAM-safe minimum distance): a built file in
`GFX_LZ_SEARCH1` is compressed that way (`graphics_rules.mk`).

### Sprite banks

N-Tranced's sprite banks are XS's format (docs/graphics.md, "Sprite
banks"): its `gSpriteBankTable` (`0x084768B4`) has 49 banks, 2,754
frames and 93 OBJ palettes (XS: 56, 2,429 and 125). Their sources are in
`games/n2/graphics/sprites/`, one `bankNN_<name>/` per bank and
`palettes/NNN_<owner>.pal` per palette, which `gfx_manifest.py
--graphics-dir games/n2/graphics extract` writes with
`tools/sprite_frames.py --graphics-dir`. The manifest's `sprite_banks`
entry gives the table, `gObjPieceWidths` (`piece_sizes`) and the banks'
names. A bank N-Tranced shares with XS keeps XS's name (`crash`,
`crates`, `aku_aku`, `wumpa`, ...), even where it has other frames; the
others are named for what their frames show (`genie`, `mummy`,
`scarab`, ...), until the code that loads them is known. The bosses'
are named by the boss whose room places their spawner (the spawner's
`Bank(n)` call): `evil_crunch` (42), `evil_coco` (43), `n_trance` (44)
and `n_tropy` (45); Fake Crash uses Crash's, bank 0. The palettes are
named as XS's are, after the first bank using them.

Seven frames of bank 31 (`platforms`) have a piece that an earlier piece
draws over with other pixels, which XS's frames never do. Such a piece is
its own PNG, `fKKK_pN.png` (N its index in the frame), marked
`"own_png"` in `bank.json`, so the frame keeps every pixel.

The tile pool and the palettes are built from these sources
(`sprite_tiles.cpp`, with graphics_rules.mk's rules), and so are the
banks' tables (`0x084768B4`-`0x08492ACB`: `gSpriteBankTable`, the
animations, the frames and their piece arrays), which
`tools/sprite_banks.py --graphics-dir games/n2/graphics baserom_n2.gba`
extracted into `games/n2/src/engine/gfx/data/sprite_banks_*.cpp` as it did
XS's. Their frames give their tiles' offsets with
`SPRITE_TILES_BANKnn_FRAMEkkk`, so a bank can change size.

N-Tranced's sprite boxes (`struct hitbox_quad`) are `{s16 offX, s16 offY,
u16 w, u16 h}` where XS's are `{s16, s16, u8 w, u8 h}` and two bytes of
padding: in all 2,903 of them the height is at +6, and +5 (XS's `h`) and
`w`'s high byte are 0. `hitbox_quad.h` has both, under `GAME_N2`.

## Text (#1114)

N-Tranced's UI text is built as XS's is (docs/text.md), from gettext
files: [`games/n2/text/ntranced.pot`](../games/n2/text/ntranced.pot) (the
keys and the English) and `games/n2/text/<lang>.po` (French, German,
Spanish, Italian, Dutch), with `tools/text.py --game n2`, by
[`games/n2/text.mk`](../games/n2/text.mk):

```
python3 tools/text.py --game n2 check
python3 tools/text.py --game n2 extract --check
```

| What | Where (ROM) | Source |
|---|---|---|
| The UI text: per language its strings, then its 196 pointers (`gUiText<Lang>`) | `0x081D1C8C`-`0x081DEA40` | `games/n2/src/game/ui/data/ui_text.cpp` |

Its 196 texts (XS has 70) are keyed `ui.level.<level>` for the 29 level
names (each level's `name_text`, `levels.json`'s `"level_names"`) and
`ui.text.<id>` for the others, until they have names. A text's final
newline is written `␤` (U+2424) in these files: the ROM's French,
German and Spanish texts 36 and 37 end in a newline their English doesn't,
or the other way round, which `msgfmt` rejects. N-Tranced's font isn't
known yet, so the font checks are XS's only. Its cutscenes' and dialogues'
text is still to do.

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
