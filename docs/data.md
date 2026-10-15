# Data: turning ROM data into source

Every compiled function in the ROM is matched. The data after the code
(`0x0803B8B0` to the end of the ROM) is still mostly `.incbin
"baserom.gba", ...` in `data/data.s`. This document is the convention for
turning it into real source, starting with pointer tables. For how data
progress is measured, see [progress_report.md](./progress_report.md), "Data
progress".

## The convention: `const` arrays in `src/*/data/`

A converted table is a `const` object in a `src/*/data/*.cpp` file, written
in C++ like the rest of the game code (#844; the tables were C files until
then), with a real element type (from a shared header when the consuming
code has one) and real symbol references for every pointer. The string
tables are generated from gettext files instead (see "Text tables"
below and [text.md](./text.md)). The linker
script places the file's `.rodata` at the table's ROM address, between the
raw parts of `data/data.s`. A library's own tables (GAX2's, AgbEeprom's,
libgcc's `__clz_tab`) follow the same convention in C under
`lib/<name>/data/` instead (see [libraries.md](./libraries.md)).

- **One file per contiguous run of converted tables.** Name it
  `<what>_<ROM offset>.cpp`, the offset in lowercase hex without the
  `0x08` (`action_ctrl_state_table.cpp` is at `0x0816BF20`), the way
  `src/game/crash/swim_ctrl.cpp` is named. Two files can sit next to
  each other when their tables are unrelated (`level_select_page_bg_package.cpp`,
  then `level_select_picture_table.cpp`).
- **Symbol names don't change.** The code references
  `gStaticData_XXXXXXXX`, and so does the frozen `expected/` assembly the
  code report diffs against, so a converted table keeps its label. The
  meaning goes into the type and a comment naming the consumer. A new
  label is fine where a table had none (`gMegaMixMotionEntries`), and a
  label nothing references can be dropped when a table spans it (see
  `actor_categories.cpp`).
- **Types come from the code's headers** when there is one:
  `struct category_descriptor` /
  `struct category_vtable` (`actor_anim.h`) and `struct bg_package`
  (`graphics_package.hpp`).
  When the only view is a struct local to one source file, the data file
  uses a plain type or a small local struct and says whose view it
  mirrors. Moving such structs into headers is a separate cleanup.

### Layout: `data/data.s` sections and `ldscript.txt`

`data/data.s` is split into sections at every C object. Each section is
named after the ROM address of its first byte, and a comment marks the
hole:

```
gWumpaHopWidths:
	.incbin "baserom.gba", 0x0016BF14, 0x0000000C

@ gActionCtrlStateTable..gSwimCtrlModeAnimRows: src/game/crash/data/action_ctrl_state_table.cpp

.section .rodata.0816C090

.global gSwimCtrlTurnSpeeds
gSwimCtrlTurnSpeeds:
	.incbin "baserom.gba", 0x0016C090, 0x000001C0
```

The `/* Data */` block of `ldscript.txt` lists every piece in ROM order:

```
build/crashbandicootxs/data/data.o(.rodata);
build/crashbandicootxs/src/game/crash/data/action_ctrl_state_table.o(.rodata);
build/crashbandicootxs/data/data.o(.rodata.0816C090);
...
```

An input-section name in the linker script matches exactly, so
`data.o(.rodata)` never pulls in `.rodata.0816C090`. The first section is
plain `.rodata` (from `0x0803B8B0`). This scales to any number of C
objects, generated ones included: each one is one more line pair.
`src/*/data/*.cpp` is already covered by the Makefile's `find src -name '*.cpp'`
(built by agbcp like the other C++ objects); nothing else needs
registering. Everything the linker script doesn't name is discarded
(`/DISCARD/`), so a data file's empty `.text` is harmless.

### Converting a table

1. **Find it and decode it.** Dump the blob's words. `0x08xxxxxx` values
   are ROM pointers. Odd ones that land on a function symbol (`nm
   crashbandicootxs.elf`) are Thumb function pointers, and even ones point
   at data. Then read how the C uses it: `grep -rn gStaticData_XXXXXXXX
   src include`. The patterns found so far:
   - `{0xFFFF0000, fn}` pairs: a gcc 2.x pointer-to-member-function
     (thisOffset 0, index -1 = non-virtual, then the code address): the
     classes' state tables. All of them are C++ now, `const
     X::StateFunc X::stateFuncs[] = { &X::f, ... }` (the
     the `*_state_table(s).cpp`, `action_ctrl_state_table.cpp` and
     `swim_input_ctrl_state_tables.cpp` files), each `&X::f` g++'s own record.
   - `{0, fn}` pairs after an all-zero first pair: a gcc 2.x virtual
     table (8-byte slots: a `this` delta, a zero index, the code
     address; slot 0 is empty). The game's 93 are all found, and all
     are emitted by g++ from the classes
     ([cplusplus.md](cplusplus.md#emitting-the-vtables-step-10)).
   - Plain words that are all odd code addresses: a function-pointer
     array.
   - Records with pointers among scalars: find the struct the consumer
     reads them through (`struct bg_package` is `{w, h, palette, tiles,
     map}`).
2. **Write the table.** One `const` object per label, in ROM order. The
   headers go inside `extern "C" { }`, as in the other C++ files. Declare
   each function it references with its real prototype (from its header)
   and each data label as `extern const u8 gStaticData_XXXX[];`. A
   pointer into the middle of a still-raw blob is `gStaticData_XXXX +
   0x20`. Put a comment on each table naming the functions that read it.
3. **Cut it out of `data/data.s`.** Delete the labels' blocks, leave the
   `@ first..last: src/*/data/file.cpp` comment, and start a new
   `.section .rodata.<ADDR>` at the next label, `<ADDR>` being that
   label's ROM address. (When the next thing is already another data
   object, no new section is needed.)
4. **Add it to `ldscript.txt`:** the object's `(.rodata)` line and the new
   `data.o(.rodata.<ADDR>)` line, right after the `data.o` section the
   table was cut from.
5. **Check.** `make compare` must print `crashbandicootxs.gba: OK`. Then
   `rm -rf build objdiff.json && make NON_MATCHING=1 report &&
   objdiff-cli report generate -o report.json`: the table shows up as a
   `data_src_data_<file>_<ADDR>` unit at 100%, and `matched_data` grows by
   its size. `tools/report_units.py` stops with an error if the layout
   doesn't add up (a table of the wrong size shifts every raw blob after
   it off its incbin offset).

### What agbcp does with `const` data

Checked by compiling test tables and by the conversions themselves (the
C tables' objects were compared symbol by symbol and byte by byte when
they became C++, #844):

- A `const` object goes to `.section .rodata`, in definition order,
  each preceded by `.align` for its type (`.align 2, 0` for anything with
  a word or pointer), with zero fill. No hidden padding between objects
  beyond that alignment. The same as agbcc's C.
- A function reference is emitted as `.word sub_XXXX`. The linker sets
  the Thumb bit itself: the R_ARM_ABS32 relocation against a Thumb
  function symbol yields the odd address, so never write `sub_XXXX + 1`.
- A `const` object at namespace scope has internal linkage in C++. A
  table whose label a header declares (`extern const ...`, seen before
  the definition) keeps external linkage; any other is defined `extern
  const ...`, or it becomes a local symbol, and one nothing in the file
  uses is dropped.
- No designated initializers (`{ .fn = f }`, `[IDX] = x`): g++ 2.9
  rejects them. Write the elements in order with the designator as a
  comment (`/* CATEGORY_SNOW_JOB */ { ... }`, `/* seq */ ...`), and zero
  the elements a sparse one skipped. Only a union's first member can be
  initialized.
- A null pointer is `0`: `NULL` is C's `((void *)0)`, which C++ doesn't
  convert to other pointer types.
- Integer constants cast to pointers work in initializers
  (`(void (*)(void))0xffffffef` for the non-code slots of
  `struct category_vtable`).

### Text tables

The string tables, `cutscene_text_english.cpp` and
`cutscene_text_translations.cpp` (the cutscenes' pages) and
`ui_text.cpp` (the UI text), are generated: each `#include`s the
`.inc` that `tools/text.py` writes into `build/crashbandicootxs/text/`
from `text/crashxs.pot` and the languages' `.po` files (#1070, see
[text.md](./text.md) for the files and the translators' workflow). Each
is a run of `extern const u8 s[] = "..."` arrays with, after them, the
arrays of pointers that list them, every pointer spelled
`(const u8 *)&s`.

The cast is what lets C++ write these tables: g++ 2.9's
`decay_conversion` (`cp/typeck.c`) replaces a `const` variable by its
initializer wherever it decays to a pointer, unless the initializer is a
brace list, and it doesn't except arrays. A string array named in another
table's initializer (`{ s }`) then becomes a pointer to a new copy of the
literal, so the table points at a duplicate string (`.LC0: .ascii
"..."`, `.word .LC0`) instead of the array. `&s[0]`, `s + 0`, `+s` and
`*&s` are folded the same way, at any optimization level, and a static
class member or a namespace doesn't help. `&s` is the array's address,
which isn't a decay, and the cast gives it the element type; the object
comes out byte for byte what agbcc made of the C files these tables
were until #1070. Nobody edits the generated files, so the cast at every
entry costs nothing (#844 turned down the same cast for hand-written
tables).

The generator stores each distinct string once, under its first use:
the languages in `gLanguage`'s order (English, French, German, Spanish,
Italian, Dutch), within a language the UI text ids in order or the
cutscene pages in ROM order (the template's order). A later use, in the
same language or another one, points at the stored string. That is the
ROM's own rule: it has no repeat stored twice, and the foreign cutscenes
reuse two English strings (cutscene 8 page 1, cutscene 9 page 5). The
UI and the cutscenes are stored apart (no string is in both). A string
is named after its first use (`gUiTextEnglish00`, `gUiTextFrench31`,
`gCutscene01EnglishPage3_0Text`), so the symbols, their order and the
`.map` are the C files' ones.

The pointer arrays are defined `extern` (a namespace-scope `const` has
internal linkage in C++, see above), and so is every string, which keeps
the C files' global symbols. `system.h` declares the UI text's six arrays
with `UI_TEXT_COUNT`, so a template with another number of `ui.*` entries
stops the build. The cutscene files (`cutscenes.cpp`,
`cutscene_slides.cpp`, generated from `data/cutscenes.json`, see
"Cutscenes") declare the page arrays their `struct cutscene_page` tables
point at, sized by the template. A table that has strings and other data
is split at the ROM boundary between them: the cutscenes are four files
in ROM order.

### Pitfalls

- **Forgetting `const`.** A non-`const` initialized object goes to
  `.data`, which the linker script discards: the bytes vanish from the
  ROM (or the link fails if the code references the symbol). A pointer
  array needs the `const` on the array, not the pointee:
  `const u8 *const tbl[]`, `void (*const tbl[])()`.
- **Clashing declarations.** Consumers declare these tables `extern`
  with their own (often non-`const`, often `u8[]`) types, each in its own
  translation unit, which is harmless. But a data file can't include a
  header that declares the same symbol without `const`
  (`conflicting type qualifiers`). Either make the header's declaration
  `const` or don't include it. Making a header declaration `const` can in
  principle change the consumers' code (gcc treats loads from `const`
  objects as unchanging), so rebuild everything that includes it and run
  `make compare` - `actor_anim.h`'s two tables became `const` this way
  with no code change. Don't retype the consumers' own local `extern`s
  as part of a data conversion for the same reason.
- **Struct padding.** agbcc pads every struct to a multiple of 4 bytes,
  so an array of small structs (say, three `u8`s) is not the ROM's
  layout. Use a plain array for such records, or check `sizeof` with
  `COMPILE_TIME_ASSERT`.
- **Alignment.** A table starts at its type's alignment. Only convert a
  table whose ROM address already has that alignment (all pointer tables
  do); `tools/report_units.py` refuses a misaligned one rather than let
  the linker insert fill.
- **Pointers into a built blob.** A table pointing into data that is
  itself built from an editable source (the GAX2 audio, a sprite sheet)
  must not hard-code an offset into it, since editing the source moves
  things. Have the build export the offsets instead:
  `gSongTable`, the song table (`song_table.cpp`, 19 entries in the
  ROM), is `gGaxMusicData + GAX_SONG_<NAME>`,
  with the offsets from the `gax_songs.h` that `tools/gax_audio.py`
  writes next to the blob (`-iquote build/crashbandicootxs/sound` for
  that object); the table itself is generated too (`song_table.inc`,
  `gax_audio.py --table`, one entry per song of the manifest), and the RLE sprite tables use `rle_sprites.py`'s frame
  headers the same way. Offsets into still-raw blobs are fine: the raw
  bytes can't move.

### Fallback: typed directives in `.s`

The other route is to write the table in assembly: `.4byte sub_XXXX` (the
linker adds the Thumb bit here too), `.2byte`, `.byte`, in `data/data.s`
or a `data/*.s` file linked the same way. The C++ route has matched
byte-for-byte on every table so far, so this is only for a table it
provably can't express. Nothing uses it yet. If something does,
`tools/report_units.py` needs to learn to size and classify directive
blobs (today it only understands `.incbin` lines in `data/data.s`).

## What has been converted

The first batch (all pointer tables, all byte-exact):

| File | ROM | Contents |
|---|---|---|
| `boss_pictures.cpp` | `0x08167AD4` | the two boss pictures (palette + frames, see "Boss pictures"), the d-pad direction table, the sine table |
| `song_table.cpp` | `0x0816AA20` | the 19-song table, offsets into the built GAX2 music block (`gax_songs.h`); generated since #1091 (`song_table.inc`) |
| `link_crc_table.cpp` | `0x0816AF10` | the link-cable CRC-16 table, 2 pairing names |
| `menu_tables.cpp` | `0x0816B138` | menu text, palette halves, label ids, icon positions and frames |
| `pause_menu_bg.cpp` | `0x0816B284` | 1 `struct bg_package` |
| `pause_menu_rows.cpp` | `0x0816B298` | the pause screen rows, a palette half |
| `obj_piece_sizes.cpp` | `0x0816B2E0` | OBJ piece sizes, the empty sprite box and point |
| `motion_records.cpp` | `0x0816B304` | 84 motion records, the entries of the two entry sets |
| `action_swim_motion_sets.cpp` | `0x0816B92C` | 2 `{entries, 0x100}` sets |
| `input_ctrl_motion_set.cpp` | `0x0816B93C` | 1 entry set and its entries |
| `enemy_anim_maps.cpp` | `0x0816B98C` | 15 text-popup tables |
| `crate_tables.cpp` | `0x0816BB6C` | 1 entry set, the collision system's kind tables, 2 scale triples |
| `action_ctrl_state_table.cpp` | `0x0816BF20` | 42-slot player action PMF table, per-mode animation row pointers |
| `swim_speeds.cpp` | `0x0816C090` | the player speed table, the per-mode level animation rows |
| `swim_input_ctrl_state_tables.cpp` | `0x0816C250` | 2 player-controller PMF tables, an entry table and its set |
| `boss_motion_tables.cpp` | `0x0816C2D8` | small actor tables (vectors, per-round bytes, thresholds, argument blocks) |
| `dingodile_motion_entries.cpp` | `0x0816C418` | an entry table and its set |
| `platform_mover_motion.cpp` | `0x0816C460` | 3 velocity vectors |
| `menu_sky_bg.cpp` | `0x0816C484` | 1 `struct bg_package` |
| `level_select_layout.cpp` | `0x0816C498` | level-select positions, animation ids, a palette half |
| `level_select_page_bg_package.cpp` | `0x0816C58C` | 1 `struct bg_package` |
| `level_select_picture_table.cpp` | `0x0816C5A0` | 10 `{palette, tiles}` asset pairs, a level-select picture per theme, generated from `levels.json`'s `"themes"` (`levels.py table pictures`) |
| `level_select_anims.cpp` | `0x0816C5F0` | level-select offsets, animation ids, OBJ sizes |
| `entity_spawn_funcs.cpp` | `0x0816C6A4` | the unified 92-slot function-pointer dispatch array |
| `level_table.cpp` | `0x0816C814` | the level table (25 `struct level_info`), the levels' room lists and 48 room records, the theme music cues and 5 colour-cycle lists, all generated from `data/levels/` (`levels.py table`, see "Level data") |
| `cutscenes.cpp` | `0x0816D1C8` | the 11 cutscenes' language tables and slide lists, the English pages, generated from `data/cutscenes.json` (see "Cutscenes") |
| `cutscene_text_english.cpp` | `0x0816D4D8` | the cutscenes' English text, generated from `text/` (see "Text tables") |
| `cutscene_slides.cpp` | `0x0816E064` | the other five languages' pages, the 24 slides, generated from `data/cutscenes.json` |
| `cutscene_text_translations.cpp` | `0x0816E8A4` | the French, German, Spanish, Italian and Dutch text, generated from `text/` |
| `terrain_types.cpp` | `0x081725A8` | the 51 terrain types of the collision maps |
| `ui_text.cpp` | `0x08172CD4` | the game's UI text in six languages, generated from `text/` (see "Terrain types and UI text") |
| `hud_font_tables.cpp` | `0x08174BE0` | the HUD digit slots and the two HUD fonts' character lists and glyph metrics |
| `actor_categories.cpp` | `0x08175558` | 7 `struct category_descriptor`, 3 `struct category_vtable` |
| `actor_palette_cycle_frames.cpp` | `0x08175760` | the 32-frame BG palette cycle and its 4 cursor start/bound pairs (`ActorPaletteCycle::frames`, `startFrames`, `targetFrames`, docs/cplusplus.md) |
| `polar_anim_family.cpp` | `0x08178F80` | categories 0-2: OBJ palette, animation table `gCategoryFamily0AnimTable`, keyframe and frame arrays (see "Category families") |
| `polar_player_state_table.cpp` | `0x0817A6B8` | 1 actor PMF table, in C++ (`PolarPlayer::stateFuncs`, docs/cplusplus.md) |
| `polar_palettes_boxes.cpp` | `0x0817A728` | 5 small palettes, 4 `struct anim_box`, 6 threshold records |
| `yeti_state_table.cpp` | `0x0817A840` | 4 state functions, in C++ (`Yeti::stateFuncs`, docs/cplusplus.md) |
| `yeti_keyframes.cpp` | `0x0817A850` | 4 keyframes (`struct anim_frame_record`) |
| `yeti_frames.cpp` | `0x0817A880` | 123 frame pointers |
| `jetpack_anim_family.cpp` | `0x0817AA6C` | a palette, 2 boxes, then categories 3-6: 2 OBJ palettes, animation table `gCategoryFamily1AnimTable`, keyframe and frame arrays |
| `jetpack_player_state_table.cpp` | `0x0817C1C0` | 1 actor PMF table |
| `jetpack_flash_palettes.cpp` | `0x0817C200` | a 3-frame palette strip |
| `jetpack_plane_bomber_state_tables.cpp` | `0x0817C260` | 2 actor PMF tables, in C++ (`JetpackPlane::stateFuncs`, `JetpackBomber::stateFuncs`) |
| `airship_fireball_state_table.cpp` | `0x0817C2B8` | 1 actor PMF table, in C++ (`AirshipFireball::stateFuncs`, docs/cplusplus.md) |
| `airship_attacks.cpp` | `0x0817C2D0` | 6 weapon-kind records, a 3-frame palette strip, a box, 2 keyframes |
| `airship_state_table.cpp` | `0x0817C3FC` | 1 function table, in C++ (`Airship::stateFuncs`, docs/cplusplus.md) |
| `jetpack_balloon_state_table.cpp` | `0x0817C414` | 1 actor PMF table, in C++ (`JetpackBalloon::stateFuncs`) |
| `jetpack_balloon_crate_state_table.cpp` | `0x0817C42C` | 1 actor PMF table |
| `jetpack_rocket_box.cpp` | `0x0817C444` | 1 `struct anim_box` |
| `hovercraft_fireball_state_table.cpp` | `0x0817C450` | 1 actor PMF table, in C++ (`HovercraftFireball::stateFuncs`, docs/cplusplus.md) |
| `hovercraft_attacks.cpp` | `0x0817C460` | 2 singleton-kind records, a box, 1 keyframe |
| `hovercraft_state_tables.cpp` | `0x0817C4C8` | 1 function table, 2 actor PMF tables, in C++ (`HovercraftCannon::stateFuncs`, `HovercraftLauncher::stateFuncs`) |
| `continue_prompt_palettes.cpp` | `0x0817C510` | the ">" icon text, 4 palette halves |
| `continue_prompt_bgs.cpp` | `0x0817C594` | 3 `struct bg_package` |
| `credits_text.cpp` | `0x0817C5D0` | the credits (see "Credits") |
| `credits_title_logos.cpp` | `0x0817CF40` | 5 glyph packages, 9 slot seeds |
| `title_screen_gfx.cpp` | `0x0817CFF4` | OAM offsets, palettes, 5 `struct bg_package`, 9 motion sequences, 1 animation record |
| `vv_logo.cpp` | `0x0817D6C0` | 20 slot seeds, 3 `struct bg_package` |
| `universal_vv_logo_motions.cpp` | `0x0817D7A4` | 1 `struct bg_package`, 20 motion sequences, the 6 language names |
| `language_names.cpp` | `0x0817E714` | 6 pointers to the language names |
| `language_select_palettes.cpp` | `0x0817E72C` | 3 palette-cache halves |
| `level_tilesets_jungle_snow_underwater.cpp` | `0x0817E78C` | a `u16[16]` (`InitLanguageSelectGraphics`), level BG tile sets 1-3 (grit, see "Resources") |
| `level_rooms_main_bonus.cpp` | `0x0824B638` | the level data of 33 rooms, generated from `data/levels/` (see "Level data") |
| `level_tilesets_sewer_space.cpp` | `0x08270F08` | level BG tile sets 4-5 (grit) |
| `level_rooms_bosses_gem_paths.cpp` | `0x082B91D0` | the level data of the other 8 rooms (same) |
| `sprite_tiles.cpp` | `0x082BF120` | the sprite tile pool (56 banks, `tools/sprite_frames.py`) and the 125 OBJ palettes (JASC `.pal`) |
| `sprite_banks_00_09.cpp` | `0x084A5600` | the sprite-bank table header, 56 `struct sprite_bank`, banks 0-9 (see "Sprite banks" below) |
| `sprite_banks_10_21.cpp` | `0x084B0AE0` | sprite banks 10-21 |
| `sprite_banks_22_38.cpp` | `0x084B414C` | sprite banks 22-38 |
| `sprite_banks_39_55.cpp` | `0x084B9D7C` | sprite banks 39-55 |
| `clz_tab.c` | `0x085A4C70` | libgcc's `__clz_tab`, twice (see "Library data" below) |
| `font_tiles.cpp` | `0x085A4E70` | the two HUD fonts' glyph tiles (grit, LZ77) |
| `gax_tables.c` | `0x085A6100` | the GAX2 engine's strings, mixing-rate table, period table and vibrato wave |
| `eeprom_data.c` | `0x085A9EEC` | the SDK EEPROM library's id string, chip configs, timeout and address constants |
| `cutscene_pictures.cpp` | `0x085A9F70` | the 24 cutscene pictures: palette + Mode 4 bitmap each, both from one PNG |
| `menu_graphics.cpp` | `0x0861BADC` | the menu, title, legal-screen and credits palettes, `bg_package` tiles and maps and logo tiles (LZ77, docs/graphics.md) |
| `level_select_graphics.cpp` | `0x086377C0` | the level-select cursor zoom tiles, the ten level-select pictures and their palettes (LZ77) |
| `level_bg_tiles.cpp` | `0x0863D4C0` | the six level BG1-3 tile sets (LZ77) |
| `level_assets.cpp` | `0x0864F830` | the 41 level assets `gRoomNNAsset` (generated, see "Level data") |

The actor-category backgrounds and tables (grit-built, see "Category
backgrounds" below):

| File | ROM | Contents |
|---|---|---|
| `polar_cell_anim.cpp` | `0x0803B8B0` | BG0 cell animation A (19x13, 60 frames, per-cell banks), category 0 `sub_effect_table` |
| `polar_sprite_sheet.cpp` | `0x080B2120` | `gPolarSpriteSheet`, the framed OBJ sheet of categories 0-2 (`tools/framed_gfx.py`, LZ77) |
| `polar_spawn_tables.cpp` | `0x080C0C38` | categories 1 and 2 `sub_effect_table`s |
| `polar_rle_sprites.cpp` | `0x080C2758` | compressed OBJ frame sets A and B (see "Compressed sprite frames") |
| `jetpack_cell_anim.cpp` | `0x080FF1B0` | BG0 cell animation B (38x10, 21 frames), category 3 BG1 picture and `sub_effect_table` |
| `jetpack_sprite_sheet.cpp` | `0x0814174C` | `gJetpackSpriteSheet`, the framed OBJ sheet of categories 3-6 |
| `jetpack_sky_pictures.cpp` | `0x08151AC4` | category 4 and 5/6 BG1 pictures, categories 4-6 `sub_effect_table`s |
| `jetpack_rle_sprites.cpp` | `0x0815A050` | compressed OBJ frame set C |

The GAX2 audio isn't C: `tools/gax_audio.py` builds it from `sound/`
into two blocks that `data/data.s` includes from `build/`, the
sound-effect set (`gGaxSfxData`, `--sfx`: 88 instruments and 87
samples, `sound/gax_sfx_manifest.json` + `sound/sfx_samples/*.wav`) and
the music block (`gGaxMusicData`). Each is a `.bin` and a `.s` that
incbins it with the pointers as relocations, so the blocks can move
(docs/shifting.md). See docs/audio.md.

### Sprite banks (`gSpriteBankTable`)

The sprite-bank animation system (`0x084A5600`-`0x084C0006`, 2,429
frames in 56 banks) is typed C in four files, with the structs in
`include/engine/gfx/sprite_bank.h`. It was extracted once by `tools/sprite_banks.py`,
which walks the tables the way the matched readers do and asserts the
layout (every object where agbcc will put it, only zero padding between);
the C is the source from then on.

- Every table is its own named `const` object in ROM order:
  `gSpriteBanks`, then per bank `gSpriteBankNNAnims`,
  `gSpriteBankNNAnimMMSeq`, `gSpriteBankNNFrames`, the frames
  `gSpriteBankNNFrameKKK` and their `...Pos`/`...Pieces` arrays. The
  pointers between them are symbol references, and counts that the ROM
  stores (`animCount`, `frameCount`, a frame's piece count) are
  `ARRAY_COUNT()`s of the arrays, so adding a piece or a step only means
  editing the array. The tables are referenced before they are defined,
  so each bank starts with `extern` declarations that carry the array
  sizes.
- A frame is a 12-byte `struct sprite_frame` plus the boxes and anchor
  its layout type (the high nibble of its first piece byte) calls for,
  so it is one of six struct types (`sprite_frame`,
  `sprite_frame_1box`, ..., `sprite_frame_3box_anchor`); the frame
  pointer array points at each one's `.frame`. Changing a frame's layout
  type means changing its struct type too.
- A frame's `tiles` is an offset into the sprite tile pool, not a
  pointer, so it can't be a symbol reference. It is written as
  `SPRITE_TILES_BANKnn_FRAMEkkk`, which `tools/sprite_frames.py`
  generates (`sprites/sprite_frames.h`) from the frame PNGs and
  `bank.json` of `graphics/sprites/bankNN_<name>/`.
- The piece arrays (`...FrameKKKPos`, `...FrameKKKPieces`) are generated
  from `bank.json` too (`sprites/bankNN_pieces.inc`, included where the
  ROM has them), and the same header declares the frames, with the
  struct type of their `layout`.
- The file cuts sit after banks whose piece bytes end on a word: each
  file starts with a word-aligned struct, and `tools/report_units.py`
  doesn't accept linker fill between objects.

#### Sprite bank frames (`graphics/sprites/bankNN_<name>/`)

The banks' art and pieces are not in the C. Each bank is a directory:
- `fKKK.png`, one per frame: the frame with its pieces in place, a 4bpp
  indexed PNG in the palette of the first animation that shows it.
- `bank.json`, with one line per frame:
  - `origin`;
  - `pieces` (`["WxH", x, y]` each);
  - `layout`, the high nibble of the first piece byte, which must match
    the frame's struct type in the C;
  - `tiles`, the tile source: `"own"`, or `{"frame": K, "tile": t}` for a
    run shared with an earlier frame, with `"bank": B` for another bank;
  - `palettes`, every palette id the frame is shown with, the first
    being the PNG's.

`tools/sprite_frames.py pack` builds:
- the pool arrays (`gSpriteBankNNTiles`, `sprite_tiles.cpp`);
- the frames' `...Pos`/`...Pieces` arrays (`sprites/bankNN_pieces.inc`,
  included at their place in `sprite_banks_*.cpp`);
- `sprites/sprite_frames.h`, with the frames' tile offsets
  (`SPRITE_TILES_BANKnn_FRAMEkkk`) and extern declarations.

`extract` writes the directories from `baserom.gba`. When you edit a frame,
the tiles it shares with other frames must stay the same in all of them,
or the build fails. A frame shown with several palettes (43 of them) is
drawn in the first, and the others are listed. docs/graphics.md, "Sprite
banks (#914)", has the details.

The category families' animation tables (`actor_anim.h`'s
`gCategoryFamily0AnimTable`/`gCategoryFamily1AnimTable`) are C as well now, see
"Category families" below.

### Category families

The two actor-category families (docs/data_map.md, `08178F80` and
`0817AA98`) are `src/game/vehicles/polar/data/polar_anim_family.cpp` (categories 0-2) and
`src/game/vehicles/jetpack/data/jetpack_anim_family.cpp` (categories 3-6). Each is an OBJ palette
(256 colours, then 0x200 zero bytes nothing reads; the second family has
two), the animation table (`struct anim_table_record`, `actor_anim.h`,
41 and 47 records), and the arrays the records point at:

- **table_A**: the clip's keyframes, `struct anim_frame_record`
  (`actor_anim.h`: duration, index into table_B, loop threshold and
  base, OAM attribute bits). Nothing stores a count.
- **table_B**: the frames. Record 0's points at compressed frames
  (`gPolarPlayerRleFrames + RLE_SPRITES_POLAR_PLAYER_FRAME_nnn`, set C for the
  second family), so it's a `const u8 *const []`. Every other record's
  is `u32` byte offsets into the family's framed sheet
  (`graphics/vehicles/polar/sprite_sheet/`, `vehicles/jetpack/sprite_sheet/`, decompressed), written
  `FRAMED_POLAR_<FOLDER>_<NN>`. `tools/framed_gfx.py header` generates
  those from the sheet's frame PNGs, in the order the sheet is built
  (`graphics.mk`), so a resized frame moves the offsets with it. Every
  offset in the ROM lands on a frame start. A sheet folder is one run of
  frames used by the same records (`sheet.json` beside it lists them
  and the palette bank its PNGs show, docs/graphics.md "The framed
  sheets' folders and colours"), so one array often spans several
  folders: `gPolarNitroCrateFrames` is `NITRO_CRATE` then
  `NITRO_CRATE_BURST`, the wooden crates all end in `CRATE_SHARDS`.

The arrays have no labels in the code (only the records point at them),
so they're named after their addresses. Each one runs to the next
array a record points at; several records share arrays, and the
partition has no leftover bytes (every table_A piece is a whole number
of 12-byte keyframes). The descriptors in `actor_categories.cpp`
point at the palettes and tables by symbol now instead of
`gPolarCategoryPalette + 0x74c`-style offsets.

`struct anim_table_record`'s fields past `table_B` were typed from the
data: `unknown_10` is a word (0 or 0x260C-0x36D5), `box_14` a `struct
anim_box` (`{x, y, z, w, h, d}`, the same box the small actor tables
next to the families hold), and `spawnX`/`spawnY` aren't always 0.
`actor_anim.h`'s `struct keyframe_entry` is gone: table_A is typed as
the matched code's `struct anim_frame_record`.

## Resources (grit-style)

Graphics, and other assets built from files, use the same layout as the
hand-written tables: each asset becomes a `const` array in a `src/*/data/*.cpp`
object, and `ldscript.txt` places that object's `.rodata` at the asset's
address in the `/* Data */` block. The pixel conversion is done by
[grit](https://github.com/devkitPro/grit). Built this way so far:
the 24 cutscene pictures (Mode 4 bitmaps,
`src/game/ui/data/cutscene_pictures.cpp`, see "Cutscenes" below), the actor-category backgrounds
(see "Category backgrounds" at the end), and the uncompressed tile pools
of the old `gLanguageSelectPalette3` blob (see "Raw tile pools" below).

This section records the grit feasibility study and the pipeline that
came out of it. The throwaway scripts behind the measurements aren't in
the repo, but each result below says what was run on what, so it can be
reproduced.

### Summary

| question | answer |
|---|---|
| Can grit be built reproducibly without changing the devshell? | **Yes.** It's vendored as `tools/grit` (v0.10.0) and built by the Makefile like `tools/gbagfx`. A small shim replaces FreeImage with libpng, which is already in the devshell and CI. |
| Is grit's pixel layout byte-exact? | **Yes, for all 73 PNG assets** (35 4bpp tilesets, 14 8bpp tilesets, 24 Mode 4 bitmaps): tile order and bitmap layout match exactly. The one condition: the PNG must be *indexed*. |
| Are grit's palettes byte-exact? | **Yes, for 16 and 256 colours** when the palette is in an indexed PNG (`-p -pn16`/`-pn256`). The exception is the `.bin` palettes with a stray bit 15, which grit can't produce. |
| Are grit's tilemaps byte-exact? | **Yes, when seeded** with the ROM tileset as grit's external tileset (`-fx`). Without `-fx`, grit forces a blank tile 0, so every index comes out one too high. |
| Is grit's LZ77 (`-gzl`) byte-exact? | **No.** 26 of 141 streams match (only tiny ones). gbagfx matches 141 of 141. |
| Can grit's C output (`-ftc`) be used as-is? | **No.** Its compressed output uses grit's own LZ77 (see above). It pads the compressed data to 4 bytes with uninitialised memory. It appends `Bitmap`/`Tiles`/`Pal`/`Map` to the symbol name. |
| Adopted pipeline | grit `-ftb` (layout) -> gbagfx (LZ77) -> `tools/bin2c.py --lz` -> `#include` in a hand-written `src/*/data/*.cpp` declaration |

### 1. Building grit

grit isn't in the devshell. **FreeImage, which upstream grit needs, doesn't
exist in the devshell's pinned nixpkgs at all** (`nix search
github:NixOS/nixpkgs/4975466d324710c576dc11ad614684e6bd8cad8e '^freeimage$'` finds nothing). So adding `pkgs.freeimage` to the devshell
isn't an option. Upstream's `cldib/cldib_png.cpp` loader doesn't help either: it
was written for libpng before 1.5 (it reads `png_info` fields directly) and
doesn't compile against the devshell's libpng 1.6.

`tools/grit` is therefore vendored source. See `tools/grit/README.md` for
the exact upstream commit, the file subset, and the license: upstream ships
GPLv2 `COPYING` plus an MIT `licence-mit.txt` by the author, and both are
kept. The changes are small and limited to image I/O:

- `extlib/fi.cpp` reimplements grit's two load/save hooks on libpng.
- `winglue.h` defines the Windows base types it used to get from `<FreeImage.h>`.
- `grit_main.cpp` no longer calls FreeImage.
- A plain `Makefile` replaces autotools.

None of grit's conversion or compression code is modified. The build needs
`g++` (the devshell's stdenv, `build-essential` in CI) and
`pkg-config libpng`. **No flake change is needed.** A Makefile rule
that downloads a pinned upstream tarball was rejected. It would make every
fresh build depend on network access and GitHub, and the FreeImage
replacement would still have to be applied as a patch on top of it.

### 2. Byte-exactness

**Layout.** Every existing PNG was run through grit with the per-kind flags
below, and the `-ftb` output was compared with the ROM's decompressed
payload at the address in the file name. The result is 73 of 73 exact.

The one condition: 36 of the tileset PNGs are the grayscale PNGs gbagfx
writes when it has no palette, where index = `max - gray`. grit reads a
grayscale PNG's gray level as the index, so these come out complemented
(`0xBF` for `0x40`). Re-saved as indexed PNGs with the same indices, they
all match. Ideally they'd get their real palettes, which a migration would
do anyway. Every cutscene PNG, every other `_8bpp_tiles` PNG, and every
framed sprite-sheet frame is already indexed. Tile order is row-major
8x8, the same as gbagfx. Flips only matter for maps (see below).

**Palettes.** An indexed PNG carrying `gMenuSkyBgPalette` (16 colours)
gives the ROM's bytes with `-g! -p -pn16`. One carrying
`gLevelSelectPicture0Palette` (256 colours) gives them with `-g! -p -pn256`.
The palettes whose entries have bit 15 set can't be produced by grit or
by gbagfx's `.pal`. They are kept in their pictures' PNGs, read by
`tools/jasc_pal.py png-to-gba`, which keeps bit 15 in the red value's
lowest bit (docs/graphics.md, "Palettes").

**Tilemaps.** The intro sky background (tileset `36_61c30c`, map
`48_62fb24`, palette `24_61badc`) was composited into one 256x160 indexed
PNG, the way an artist would edit it, and put through `-gt -gB4 -m -mRtf
-mLf`:

- **Tiles:** grit's reduction reproduces the ROM tileset exactly: the same
  first-appearance order, the same deduplication, and the same h/v-flip
  choices for the 3 h-flipped and 10 v-flipped entries. The only
  difference is that `tmap_init_from_dib()` always starts the tileset with
  a blank tile 0. So grit's tileset is `[blank] + ROM tileset`, and every
  map index is ROM + 1.
- **Map:** passing the ROM tileset as grit's external tileset (`-fx
  tileset.png`, an 8px-wide strip) gives a byte-exact map.

So a tileset and map can be kept as one full-image PNG only for tilesets
that really start with a blank tile. Otherwise they need `-fx`, or a
one-line grit option to skip the forced blank tile. `-mLs` pads the map to
32x32 screenblocks, so it's the wrong layout for these 32x20 maps; use
`-mLf`.

**LZ77.** Every graphics stream in `data.s` was decompressed and
recompressed with grit's `lz77gba_compress()` (the `-gzl` code) and with
gbagfx. gbagfx is byte-exact on all 141. grit is byte-exact on only 26,
all tiny (palettes, 1-tile graphics, 512-byte maps). On anything real,
grit's Okumura-tree LZSS picks different (valid) matches and usually a
slightly bigger stream. No grit option changes its match search, so
**gbagfx stays the compressor**.

**C output.** `-ftc -gu8` gives `const unsigned char <name>Bitmap[N]
__attribute__((aligned(4))) __attribute__((visibility("hidden")))`.

- agbcc compiles it once it goes through `cpp`: `visibility` is ignored
  with a warning, and `aligned(4)` is honoured.
- `-gu8/-gu16/-gu32` only change the element type and declared length, not
  the bytes.
- The symbol is always `<-s name>` + a fixed suffix, not the repo's names.
- Compressed output is `ALIGN4`-padded, and the pad bytes come from
  uninitialised memory. The ROM's streams are packed back to back without
  padding: the bitmap here is 0x3A61 bytes, and the next object starts
  right after it.
- The header has a timestamp.

So grit's C writer is only a fit for uncompressed assets with 4-aligned
sizes, and none of the assets identified so far is uncompressed.

### 3. Adopted pipeline

```
graphics/<dir>/<name>.png
  --grit <kind flags> -p! -ftb -fh!-->  build/.../<name>.img.bin     (layout)
  --gbagfx-->                           build/.../<name>.img.bin.lz  (byte-exact LZ77)
  --tools/bin2c.py --lz-->              build/.../<name>.img.bin.lz.inc
src/*/data/<asset>.c:   const u8 gStaticData_XXXXXXXX[] = {
                      #include "<dir>/<name>.img.bin.lz.inc"
                      };
```

`tools/bin2c.py --lz` trims gbagfx's 4-byte padding back to the stream's
real length. It fails if the trimmed bytes aren't zero. The declaration
(symbol name, type, comment) is hand-written in `src/*/data/*.cpp`, so names
follow the repo's conventions, and only the bytes are generated. This
replaces the earlier plan of generating the whole `.c` file under
`build/`. Keeping the declaration checked in means the symbol name, the
element type and the comment live in the repo, and the object sits in
`src/*/data/`, where the linker script and the report already handle it.
The element type is `u8`: the ROM's LZ77 streams have odd lengths and
are packed back to back. Objects
under `src/*/data/` get `-iquote build/crashbandicootxs/graphics`, so the
`#include` path is just `<dir>/<file>`.

Per asset kind:

| asset | grit flags | then | status |
|---|---|---|---|
| Mode 4 bitmap (`cutscenes/*.png`, 240x160 8bpp linear) | `-gb -gB8 -p!` | gbagfx LZ77 | used |
| 8bpp tileset (`*_8bpp_tiles.png`) | `-gt -gB8 -p!` | gbagfx LZ77 | verified, 14/14 |
| 4bpp tileset (`*_tiles.png`) | `-gt -gB4 -p!` | gbagfx LZ77 | verified 35/35; needs the PNG re-saved as indexed first |
| 16-colour palette | `-g! -p -pn16` on an indexed PNG carrying it | gbagfx LZ77 | verified |
| 256-colour palette | `-g! -p -pn256` | gbagfx LZ77 | verified; the bit-15 palettes are their PNGs' palettes, read by `tools/jasc_pal.py` |
| tilemap (+ tileset) from one full image | `-gt -gB4 -m -mRtf -mLf -fx <tileset.png>` | gbagfx LZ77 | map verified; the tileset itself stays a separate PNG |
| framed OBJ sheets (`graphics/vehicles/*/sprite_sheet/`) | not migrated: each frame has a 4-byte header grit doesn't know about | `tools/framed_gfx.py` | - |
| raw 4bpp tile pool (`graphics/sprites/`) | `-gt -gB4 -p!` | `tools/bin2c.py --size` | used, 57 PNGs |
| raw 8bpp tag-0x00 tile set | none: generated from the rooms' pictures (levels.md, "Tile sets") | `tools/bin2c.py`, header in C | used, 5 sets |

**Never** use grit's `-gzl`/`-pzl`/`-mzl` or its `-ftc` for LZ77 assets.

### Raw tile pools (`gLanguageSelectPalette3`)

The 3.3 MB `gLanguageSelectPalette3` blob (`0x0817E78C`-`0x084A5600`, see
docs/data_map.md) is five C objects: three of tiles and two of room data.
None of it is compressed, so there is no gbagfx step: grit's `-ftb`
output goes straight to `tools/bin2c.py`, and the room data is generated
C (see "Level data").

| object | contents | source |
|---|---|---|
| `level_tilesets_jungle_snow_underwater.cpp` | `gLanguageSelectPalette3` (`u16[16]`), tile sets 1-3 | generated from `data/levels/` (levels.md, "Tile sets") |
| `level_rooms_main_bonus.cpp` | room data, 33 rooms | `data/levels/` via `tools/levels.py` (see "Level data") |
| `level_tilesets_sewer_space.cpp` | tile sets 4-5 | generated from `data/levels/` |
| `level_rooms_bosses_gem_paths.cpp` | room data, 8 rooms | same |
| `sprite_tiles.cpp` | 56 sprite banks, 125 OBJ palettes | `graphics/sprites/bankNN_<name>/`, `graphics/sprites/palettes/NNN_<owner>.pal` |

- **Sprite banks.** The pool at `0x082BF120` has no header. Each of the 56
  banks of `gSpriteBankTable` owns one back-to-back range of it, in
  bank order, and each range is one `const u8 gSpriteBankNNTiles[]`. The
  sources are the banks' frames, a PNG each in the palette of its
  animation, in `graphics/sprites/bankNN_<name>/`; `tools/sprite_frames.py`
  extracts and packs them (see docs/graphics.md, "Sprite banks").
- **OBJ palettes.** `gObjPalettes` (`0x084A4660`, 4,000 bytes) is 125
  16-colour palettes, the table header's `palettes`/`paletteCount`. An
  animation's palette is `gObjPalettes[anim.paletteId]`
  (`struct sprite_anim` +0x14): `GetPaletteSlot` copies it into a free
  OBJ palette slot, and the slot goes into the OAM entries that draw the
  animation's frames. Each palette is one JASC file,
  `graphics/sprites/palettes/NNN_<owner>.pal`, `NNN` being the `paletteId`
  and `<owner>` what uses it (see docs/graphics.md, "OBJ palettes");
  `graphics.mk` builds each with `tools/jasc_pal.py` and concatenates them
  in index order. None has bit 15 set. 98 of them are named by some
  animation; 6, 14, 15, 20, 26, 31, 38-40, 42, 43, 63, 65, 66, 73, 90-94,
  103, 104, 107, 108 and 111-113 are named by none. Most banks use their
  own palettes; a few are shared (33 by banks 23, 35, 42, 44, 47 and 52;
  49-53 by banks 32, 39, 43 and 53). This used to be
  `tile_pool_4a4660.png`, read as "125 fixed OBJ tiles".
- **Level tile sets.** These are tag-0x00 raw tagged assets: a
  `{size << 8}` word, then 8bpp tiles. `include/engine/gfx/tagged_asset.h` gives each
  one a sized struct (`TAGGED_RAW_ASSET(n)`, `TAGGED_RAW_HEADER(n)`) whose
  `data` member `#include`s the tiles, its size being
  `TILES_SIZE_<set>` from the generated `tilesets/<set>.h`. The tiles
  aren't a source: the build makes each set from the room layer pictures
  that use it ([levels.md, "Tile sets"](./levels.md#tile-sets)).
- **Room data** is decoded typed C now (see "Level data" below): an
  undecoded `.bin` copy wouldn't have counted as converted data.
- `tools/gfx_manifest.py extract` writes the OBJ palettes from
  `baserom.gba`; `tools/levels.py extract` writes the rooms, from which
  the tile sets are made again.


### Boss pictures

`gAirshipPicture` (the green blimp of the jetpack stages, 18x12 cells,
4 frames: the propellers turn) and `gHovercraftPicture` (N. Gin's
hovercraft, 16x10 cells, 1 frame) were read as "per-level meter grid
tables" before; drawn, they are the two big enemies that fly on an affine
BG. The airship is a jetpack-stage object (`Airship`,
`src/vehicle/jetpack/airship*.cpp`, created by `CreateAirship` in
categories 3-5), and the hovercraft is N. Gin's (`Hovercraft`,
`src/game/bosses/hovercraft*.cpp`, category 6, `CATEGORY_TYPE_HOVERCRAFT`),
the craft of N. Gin's level-select picture; earlier notes called it
Cortex's. Each is `{s16 cols,
s16 rows}` and its frames, a frame being a tile count, a `cols * rows`
map of `u16` tile indices and that many 4bpp tiles. The frames share one
tile pool: frame 1's indices count on from frame 0's tiles, and so on.
The game (`ConvertAirshipTiles`, `ConvertHovercraftTiles`) converts the tiles to 8bpp tiles
of BG palette 1, since an affine BG only takes 8bpp, and `DrawAirshipMap`
lays them out by the maps. The palette in front of each picture is its
own label (`gAirshipPalette`, `gHovercraftPalette`), and the code
walks the frames from that label + 0x204, so the two stay back to back
in `src/game/bosses/data/boss_pictures.cpp`.

The maps are exactly what deduplicating the frames in reading order gives
(the first identical tile, no flips, one pool across the frames). So the
sources are the whole frames, `graphics/vehicles/jetpack/airship/frameN.png`
and `graphics/bosses/hovercraft.png` (4bpp, the 16 colours of BG palette
1). grit (`-gt -gB4`) turns each into
tiles in cell order, and `tools/boss_pictures.py pack` deduplicates them
into the `.inc` with the frames' initializers and a header with the tile
counts and the size in cells, which the C struct's array sizes use. So a
frame can be redrawn freely. `tools/gfx_manifest.py extract` writes the
PNGs from `baserom.gba` again (`boss_pictures.render`, which checks the
round trip).

### Small tables around 0x0816B000

The rest of `0x0816AF10`-`0x0816C6A4` is small typed tables written out
from the ROM: palettes and palette halves (`u16`), text and label ids,
icon positions (`struct icon_pos`), OBJ sizes, motion records and the
{a, b} entry pairs of the entry sets (see `action_swim_motion_sets.cpp`),
collision kind tables, and vectors. Labels the code has no symbol for,
because it only reaches them through a pointer, are named after their
address: the pairs in `motion_records.cpp` (`gActionCtrlMotionEntries`,
`gSwimCtrlMotionEntries`), which `action_swim_motion_sets.cpp` now points at by
name, the level animation rows `gSwimCtrlModeLevelAnims` that
`action_ctrl_state_table.cpp` points at, and the two link-cable names
`gCrash2LinkText`/`0816B124` that the IWRAM data points at (`src/engine/sys/iwram_data.cpp`). A few
byte tables sit at odd addresses (`gTinyHopTargets`); brace-list `u8`
arrays aren't aligned by agbcc, so they stay in place.

### Level data

The rooms' level data and level assets ([levels.md](./levels.md)) are
built from `data/levels/` by `tools/levels.py`, from editable sources:
per room a `room.json` (layer settings, entities, parameter records,
links, symbol names), each BG layer as its whole picture (`<layer>.png`,
8bpp indexed with the room palette, which the pictures carry, decoded back to
cells against the layer's tile set, see
[levels.md, "BG layers"](./levels.md#bg-layers)), and the
collision layer as an image (`collision.png`, the size of the BG0 picture, one
8x8 block per cell, one colour per cell value,
see [levels.md, "Collision layers"](./levels.md#collision-layers)). The build
(`levels.mk`) generates:

| output | from | used by |
|---|---|---|
| `build/.../data/levels/<room>/<layer>.map.bin` | the BG layer's cells, made with its tile set (`levels.py tileset`) | the room's asset |
| `build/.../data/levels/tilesets/<set>.bin` | a BG tile set, made from its rooms' pictures (`levels.py tileset`) | `level_tilesets_*.cpp`, `level_bg_tiles.cpp` |
| `build/.../data/levels/<room>/asset.bin.inc` | the room's tilemaps, re-encoded into the ROM's chunk streams, as `tools/bin2c.py` bytes | `level_assets.inc` (7 raw assets) |
| `build/.../data/levels/<room>/asset.bin.lz.inc` | the same, gbagfx LZ77, `tools/bin2c.py --lz` bytes | `level_assets.inc` (34 packed assets, formerly `graphics/tileset1/27`-`60`) |
| `build/.../data/levels/level_assets.inc` | the 41 `gRoomNNAsset` definitions, in `levels.json`'s `"assets"` (ROM) order, then the new rooms' | `#include`d by `src/game/level/data/level_assets.cpp` |
| `build/.../data/levels/level_rooms_<region>.inc` | all rooms of a region; the last region (`bosses_gem_paths`) then has the new tile sets and the new rooms | `#include`d by `src/game/level/data/level_rooms_<region>.cpp` |
| `build/.../data/levels/level_table.inc` | the colour cycles' lists, `gLevelTable`, `gLevelRoomLists` and `gThemeMusicCues`, from `levels.json`'s `"levels"`, `"themes"` and `"palette_cycles"` (`levels.py table levels`) | `#include`d by `src/game/level/data/level_table.cpp` |
| `build/.../data/levels/level_table_16c814_rooms.inc` | the room records and the played-room arrays, from room.json's `"record"` and `levels.json`'s `"table"` and `"levels"` (`levels.py table records`) | the same, after it |
| `build/.../data/levels/level_select_pictures.inc` | `gLevelSelectPictures`, from `levels.json`'s `"themes"` (`levels.py table pictures`) | `#include`d by `src/game/ui/data/level_select_picture_table.cpp` |
| `build/include/theme_palette_cycles.h` | RunRoom's colour-cycle cases per theme and the lists' declarations, from `levels.json`'s `"themes"` and `"palette_cycles"` (`levels.py palette-cycles`) | `play_room.cpp`, `level_table.cpp` |

Every pointer in the room data is a symbol reference (the room's own
objects, the tile sets, the assets), and everything the ROM derives
(chunk grids and sets, asset offsets, entity groups and counts, parameter
offsets) is computed from the sources again. `include/game/level/level_data.h` has
the types.

The records that point at the rooms are generated too, into
`src/game/level/data/level_table.cpp`: the level table `gLevelTable`
(25 `struct level_info`, indexed by level id), one `struct
level_room_list` per level (the rooms played in order, `gLevelNNRooms`,
and up to two extra rooms), and the 48 `struct level_room` records
(`gLevelRoom00`..`gLevelRoom40`, one per room, and 7 `gLevelStageN`
records for the stages played in actor category N, which have no room
data). A room record names the room's palette and `struct level_desc` by
symbol, so the room data in `level_rooms_*.cpp` can move or change size:
nothing outside the data files holds an address into it any more. The
code still reads these tables through its own local views
(`MedalTableEntry`, `threshold_table_entry`, `gl_level_entry`, ...); the
header comment of each struct lists them.

**The generated level table.** The .cpp only `#include`s the two
generated files, which hold its definitions in ROM order: the level
themes' tables (the colour-cycle lists and `gThemeMusicCues`, from
`levels.json`'s `"themes"` and `"palette_cycles"`, [levels.md,
"Themes"](./levels.md#themes)) around the level table. The generator
follows the conventions of the other generated data: every object is
`extern const` with a positional initializer (no designated initializers),
every pointer is a symbol (`&gLevelRoomLists[LEVEL_SNOW_CRASH]`,
`&gLevelRoom17`, `gRoom17Palette`), and counts are `ARRAY_COUNT` of the
array they count. The sources are `levels.json`'s `"levels"` (per level
its `gLevelTable` fields and the rooms it plays, `{"stage": "<NAME>"}`
for a stage) and `"table"` (the ROM's played-room arrays in ROM order,
written by `levels.py extract`, which fixes the definition order), and
each room's `"record"` (its symbol, `"kind"` and `"blend"`). What is new
(a new level, a new room's record, a level's changed room array) is
generated in place with the rest, and the ROM data after it moves
([shifting.md](./shifting.md)). [levels.md, "The level
table"](./levels.md#the-level-table) has the field-by-field sources.

### Cutscenes

The 11 cutscenes (the intro, the story scenes between worlds, the
endings) are `src/game/ui/data/cutscenes.cpp`, `cutscene_text_english.cpp`,
`cutscene_slides.cpp` and `cutscene_text_translations.cpp`
(`0x0816D1C8`-`0x081725A8` in that order, types in `include/game/ui/cutscene.hpp`;
the text is generated from `text/`, see "Text tables") and
`src/game/ui/data/cutscene_pictures.cpp`
(`0x085A9F70`-`0x0861BADC`). `PlayCutscene` plays cutscene `idx`: the
slides of `gCutscenes[idx]` (`struct cutscene_slides`), each a
`struct cutscene_slide` (picture, timing, fades, music cue, sound
effect), shown with the page of text at the same index of the current
language's `struct cutscene_page` array. The six language tables
(`gCutsceneTextEnglish` is English, then French, German, Spanish,
Italian and Dutch) are listed by `gCutsceneTexts`, a pointer table in
the IWRAM image (`src/engine/sys/iwram_data.cpp`). The text is strings (Latin-1, all lower case),
each page's strings in a `const u8 *const []`. The template's keys are
`cutscene<NN>.page<P>.<I>` ([text.md](./text.md)). The cutscenes, their
slides and their pages are data, `data/cutscenes.json` (below).

A picture is a 256-colour palette followed directly by its LZ77 Mode 4
bitmap: a slide points at the palette and `ShowSlidePicture` reads the bitmap
at `+0x200`. The bitmaps are the grit-built `graphics/cutscenes/NN_<what>.png`
(the Mode 4 row of the table above), NN being the picture's number in
`gCutscenePictureNN`. Each palette (`gCutscenePictureNN`,
an uncompressed `u16[256]`) is the PNG's own palette:
`tools/jasc_pal.py png-to-gba` reads it and `tools/bin2c.py --u16` writes
the C values. About half of the entries have bit 15 set, which the
hardware ignores; the PNG keeps it in the red value's lowest bit, as every
palette kept in a PNG does (docs/graphics.md, "Palettes"). Each palette is
`aligned(4)`: the 0-3 zero bytes before it in the ROM are the alignment
after the previous (odd-length) bitmap stream.

One picture is often shown by several cutscenes, so the pictures are
one flat folder, not one folder per cutscene. Slides 08 and 09 show
pictures 09 and 08, swapped.

| cutscene | constant | played by | pictures (`graphics/cutscenes/`) |
|---|---|---|---|
| 0 | `CUTSCENE_LEGAL_NOTICE` | boot, `PlayBootCutscene` | 23 `legal_notice` |
| 1 | `CUTSCENE_INTRO` | intro, `PlayIntroCutscene` | 00 `space_station`, 01 `uka_uka_and_cortex`, 02 `cortex`, 01 |
| 2 | `CUTSCENE_NEW_GAME` | new game, `PlayNewGameCutscene` | 00, 03 `cortex_hologram`, 04 `minimizer_console`, 05 `minimizer_beam`, 06 `earth_shrinking`, 07 `cortex_on_monitors`, 09 `cortex_holding_earth`, 08 `aku_aku_crash_and_coco`, 10 `aku_aku` |
| 3 | `CUTSCENE_CORTEX_WARNING` | nothing (unused) | 11 `cortex_on_screen` |
| 4 | `CUTSCENE_DINGODILE_CLEARED` | Dingodile beaten | 11 |
| 5 | `CUTSCENE_N_GIN_CLEARED` | N. Gin beaten | 12 `coco_on_screen` |
| 6 | `CUTSCENE_TINY_CLEARED` | Tiny beaten | 01, 02 |
| 7 | `CUTSCENE_ALL_CRYSTALS` | nothing (unused) | 12 |
| 8 | `CUTSCENE_NEO_CORTEX_COMPLETE` | Neo Cortex beaten at 100% (Mega Mix follows) | 13 `minimizer_overload`, 14 `bosses_shocked`, 15 `monster`, 16 `monster_and_crash` |
| 9 | `CUTSCENE_MEGA_MIX_CLEARED` | Mega Mix beaten | 17 `crash_flying_to_station`, 18 `coco_at_enlarger`, 19 `earth_growing`, 20 `earth_beam_ring`, 21 `station_exploding`, 22 `reversed_beam` |
| 10 | `CUTSCENE_NEO_CORTEX_CLEARED` | Neo Cortex beaten below 100% | 13, 17, 18, 19, 20, 10 |

Cutscenes 3 and 7 are never played. The ROM calls `PlayCutscene` ten
times, each with the id as an immediate (`movs r1, #N`): 0, 1, 2 (twice),
4, 5, 6, 8, 9 and 10. Nothing else reads `gCutscenes` (one literal-pool
word, in `PlayCutscene`), no word of the ROM points at `PlayCutscene` (no
indirect call), and none points into `gCutscenes` past its start. Their
pages are translated like the others.

#### The cutscene data

The cutscenes' structure is `data/cutscenes.json` (#1092), and
`tools/cutscenes.py` generates the tables of `cutscenes.cpp` and
`cutscene_slides.cpp` from it (`cutscenes.mk`, into
`build/crashbandicootxs/cutscenes/`); with the ROM's data they are the
ROM's bytes:

```json
{
  "cutscenes": [
    {"constant": "CUTSCENE_INTRO", "about": "the intro (PlayIntroCutscene)", "slides": [
      {"slide": "space_station", "text": "cutscene01.page0"},
      ...
    ]},
    ...
  ],
  "slides": [
    {"name": "space_station", "picture": "cutscenes/00_space_station.png", "wait": 0, "fade": 0,
     "fade_after": 0, "buttons": 1, "duck_music": 0, "rearm_sfx": 0, "song": "SONG_CUTSCENES_SPOOKY",
     "sfx": "SFX_SPACE_STATION_AMBIENCE"},
    ...
  ],
  "rom_order": {"pages": [...], "slide_lists": [...]}
}
```

- **`cutscenes`**, in id order. A cutscene's id is its position, the
  index of `gCutscenes` and of each language's pages, and its
  `constant` is the generated `CUTSCENE_*` define
  (`build/include/constants/cutscenes.h`, with `CUTSCENE_COUNT`, the
  tables' length). Code plays a cutscene by its constant
  (`PlayCutscene(CUTSCENE_INTRO)`), and data that names a cutscene names
  its constant, not its number. A cutscene's `slides` show in order:
  each names a slide record (`slide`) and, where text shows with it, its
  page (`text`), `cutscene<NN>.page<P>`. The page's strings are the
  template's `cutscene<NN>.page<P>.<I>` entries, `I` from 0: the page's
  string count is the template's, so a string added to a page in `text/`
  needs no other edit. A slide with no `text` shows the picture only
  (`{ 0, 0 }`). Two slides can show one page (cutscene 10's third slide
  shows `cutscene09.page1`).
- **`slides`**, the slide records (`gCutsceneSlideNN`, NN their
  position), in ROM order; several cutscenes can show one. The fields are
  `struct cutscene_slide`'s: `picture` (a cutscene picture of
  `graphics/manifest.json`, by its path; the record points at its
  palette), `wait` (WaitForKeyPress's frame count), `fade` and
  `fade_after` (BrightnessFade::Start's flags as the slide starts and
  after it, -1 none), `buttons` (1: a key press goes on, and SkipSlides
  skips it), `duck_music` (FadeOutMusic after it), `rearm_sfx` (play the
  sound again after it), `song` (the music cue, a `SONG_*` constant of
  `constants/songs.h`) and `sfx` (a `SFX_*` constant, `SFX_NONE` for
  none). `song` is a constant's name for now; once the songs have names
  of their own (#1091), it can take one.
- **`rom_order`**, the order the ROM stores two of the tables in: each
  language's pages are cutscene 1 to 10, then 0, and the slide lists 0
  to 3, 6, 4, 5, then 7 to 10. A cutscene a list leaves out comes after
  the listed ones, in id order, so a new cutscene needs no entry here.

The generated tables are, in ROM order: in `cutscenes.cpp`,
`gCutsceneTextEnglish`, `gCutscenes`, the English pages, the other five
languages' tables and the slide lists; in `cutscene_slides.cpp`, the
French to Dutch pages and the slide records. Every array is declared
`extern` first (a namespace-scope `const` has internal linkage in C++),
with the size the data gives it.

`tools/cutscenes.py` stops the build, and `make text-check` (which runs
`tools/cutscenes.py check`), on:

- a slide whose `picture` isn't a cutscene picture of the manifest (a
  `bitmap` entry under `graphics/cutscenes/`);
- a `text` page with no `.0` entry in the template, a page of the
  template that no slide shows, a slide record no cutscene shows, a
  slide name that no record has, or has twice;
- a constant that isn't `CUTSCENE_<NAME>`, or is there twice; a cutscene
  with no slides; a missing field or a number out of its field's range;
  a `rom_order` name that is no cutscene's.

#### A new cutscene

1. **The picture**, if it is a new one: a 240x160 8bpp PNG with its
   256-colour palette in `graphics/cutscenes/` (`24_<what>.png`), and a
   manifest entry with no `rom`, its `class` and `"object":
   "cutscene_pictures"` ([graphics.md, "New
   graphics"](./graphics.md#new-graphics)):

   ```json
   {"path": "cutscenes/24_lab.png", "kind": "bitmap", "symbol": "gCutscenePicture24Bitmap",
    "class": "CutscenePicture24", "about": "the lab", "object": "cutscene_pictures",
    "palette": {"symbol": "gCutscenePicture24", "colors": 256}}
   ```

   then `tools/gfx_manifest.py symbols`. Its palette and bitmap come
   after the ROM's 24 pictures in `cutscene_pictures.cpp`.
2. **The slides**: a record in `slides` for each new picture (or new
   timing, song or sound).
3. **The cutscene**: an entry at the end of `cutscenes`, with its
   constant and its slides, each with its page (`cutscene11.page0`, ...).
4. **The text**: the pages' strings in `text/crashxs.pot`
   (`cutscene11.page0.0`, ...), then `make text-update` and each
   language's text ([text.md](./text.md)).
5. **What plays it**: code calls `PlayCutscene(CUTSCENE_LAB)`, or a boss
   level names its constant (#1086).

`make` builds it: `CUTSCENE_COUNT` grows, and the tables with it.

Tested in scratch with picture 03 mirrored, its red and blue swapped, as
`24_lab.png`, a slide showing it, and `CUTSCENE_LAB` (id 11): that slide
with a one-string page, then the space station slide with a two-string
page, in the six languages; and a second string added to the intro's
first page (`cutscene01.page0.1`) in the six languages, with no other
edit. `make text-check` passes and `make` builds it. With
`PlayIntroCutscene` playing `CUTSCENE_LAB` first (the trigger, a scratch
code edit), mGBA shows the new picture with its page, the station
slide's two strings one after the other, then the intro with the added
string after the first one, in English, French and German. Each check
above stops `tools/cutscenes.py check` and the build with its message.

### Library data

The constants of the libraries linked into the game, between the GAX2
music block and the cutscene pictures, are C in three files:

- `clz_tab.c`: libgcc2.c's `__clz_tab` (bit length of each byte
  value), twice, because gcc 2.x's libgcc2.c made it `static` in each
  object. `__divdi3` and `__udivdi3` (`lib/libgcc/libgcc2.c`)
  each read their own. The values are written the way libgcc2.c writes
  them.
- `gax_tables.c`: the GAX2 engine's version string (and the
  pointer to it, which `GAX2_init` checks for "GAX"), the 12 mixing
  rates (`struct RateEntry`), the error strings `GaxFatalError` prints,
  its halt banner and the pointer to it, the 3,828-entry period table
  and the 64-step vibrato wave. The two pointers are symbol references
  now, so the music block before them can change size.
- `eeprom_data.c`: the AGB SDK EEPROM library's "EEPROM_V122" id
  string (read by flashers and emulators to detect the save type, not
  by the game), the two `struct EepromConfig`s, the write timeout
  `u16[3]`, and a list of the library's 22 address constants. That list
  holds, function by function, the literal-pool words of `EEPROMConfigure`
  ... `EEPROMCompare` that are symbol addresses (IWRAM variables, the two
  configs, the timeout, `EepromTimerIntr`). Nothing in the ROM points at
  it, but every word is a symbol, so it is written as symbol references
  and doesn't pin anything in place.

The one-byte pad before the version string is the alignment after the
LZ77 stream before it, `gLargeFontTiles` (`font_tiles.cpp`): the
linker's zero fill up to the 4-aligned `.rodata` of `gax_tables.o`.

### Placing it and counting it

Placement is the same as for a hand-written table (see "Layout" above).
`data/data.s` drops the asset's label and `.incbin`, leaves a
`@ ...: src/.../data/<file>.cpp` comment, and starts a new `.section` for what
follows; `ldscript.txt` links the object's `.rodata` in between (the
first asset done this way was the intro bitmap `gCutscenePicture00Bitmap`,
now part of `cutscene_pictures.cpp`). agbcc doesn't align a `u8`
array, so whatever follows starts right at the array's end, which is
what the ROM has for these unaligned stream lengths. Where the ROM starts
a stream on a word boundary (every other LZ77 asset), the definition says
`__attribute__((aligned(4)))`, see "LZ77 stream padding".

The report needs nothing extra. The object is in `src/*/data/`, so
`tools/report_units.py` counts it as built data from the `ldscript.txt`
layout like any other table. `make report` depends on `$(C_OBJS)`, and
`graphics.mk` makes the object depend on its generated `.inc`, so grit
and gbagfx run first. The asset was already counted as built data when it
was an `.incbin` of the gbagfx output, so converting it doesn't change
`matched_data`. It only moves the bytes from an `.incbin` into a C array.

### Adding the next asset

1. Make sure the PNG is indexed (convert the grayscale ones first) and the
   grit rule for its kind is in `graphics.mk`.
2. Write `src/<area>/<namespace>/data/<content>.cpp` (next to the code
   that reads it, named after what it holds, never its address) with the
   hand-written declaration. Add a `$(call data_obj,<content>): <...>.lz.inc`
   dependency line to `graphics.mk`.
3. Split `data/data.s` and add the `ldscript.txt` lines, as for any table.
4. Run a full clean `make` (`make tidy && make`). It must print
   `crashbandicootxs.gba: OK`.

### Terrain types and UI text

The old raw blob `gStaticData_081725C4` was two things. First
`src/game/level/data/terrain_types.cpp`: 51 terrain types (`gTerrainTypes`, `struct
terrain_type`, 36 bytes: a value per collision mode, then the height of
each of a cell's 8 pixel columns per mode). The code reads the height
rows of each mode as their own 36-byte-stride tables through the labels
`gTerrainHeights0`..`gTerrainHeights3` (formerly `gStaticData_081725AC`/
`B4`/`BC`/`C4`), which point 4, 12, 20 and 28 bytes into the first
record. Those stay as symbols (the code uses them), defined with
`asm(".set ...")` in the C file as names for addresses inside the
table: they aren't objects, so the report doesn't count them.

Then `src/game/ui/data/ui_text.cpp`: the game's own text (menus, level
names, popups) in six languages, 70 strings each. `GetUiText` looks a
text id up in `gUiTextTables[language]` (`src/engine/sys/iwram_data.cpp`),
which now points at `gUiTextEnglish`...`gUiTextDutch` by name. Each
language's new strings sit before its array, and strings several
languages share (mostly the level names) are stored once. Every string
in the region is referenced, and every pointer lands on a string start.
Strings are named after the first language and index that use them
(`gUiTextEnglish00` is "level"). The table is generated from `text/`
(see "Text tables" and [text.md](./text.md)): the template's `ui.*`
entries in text id order. The compiler's 4-byte alignment of string
arrays gives the ROM's zero padding.

### Credits

`gCreditsText` (`src/game/ui/data/credits_text.cpp`) is the credits
stream `UpdateCreditsText` draws line by line as floating glyphs. It is text
with three opcodes: 1, n (popup glyph picture n, the logos of
`credits_title_logos.cpp`), 2 and 3 (the small and the large HUD font).
A zero byte ends it, and the code starts over there. It is generated from
`text/crashxs.pot`'s `credits.*` entries (the English text, which every
language shows) and `data/credits.json` (the order, the
font switches and the logos), see [text.md, "The
credits"](./text.md#the-credits).

### Motion sequences

`gTitleScreenBg` and `gUniversalLogoBg` are each a BG2
picture (`struct bg_package`) followed by the motion scripts of the
countdown slots of `title_screen.cpp`: `struct delta_record`s
(a hold count, positions, velocities, per-frame deltas), in sequences
that end with a zero hold. The seed tables (`credits_title_logos.cpp`'s
`gTitleLogoPieceSeeds`, `vv_logo.cpp`'s `gVvLogoPieceSeeds`)
point at each sequence by name now (`gTitleLogoPieceMotion0`, ...), not at
`gUniversalLogoBg + 0x174`-style offsets. The six language names
after the second set are the strings `language_names.cpp` points at
(they aren't digit glyphs). The four OBJ sprite packages in front of
the first set are listed by the IWRAM table `gTitleObjPackages`, which
points at them by name too.

### Category backgrounds

The actor categories (`struct category_descriptor`, `actor_anim.h`) point
at two kinds of uncompressed background, both now built from PNGs in
`graphics/vehicles/`: the polar stages' ice wall
(`vehicles/polar/ice_wall.png`), the jetpack stages' clouds
(`vehicles/jetpack/clouds.png`) and the skies of Rocket Racket, Blimp
Bonanza and No-Fly Zone (`vehicles/jetpack/skies/`):

- **BG0 cell animations** (`cellAnim`, played by `InitCellAnim`/
  `UploadCellAnimFrame`): `struct cell_anim_header` (256-colour palette, `cols`,
  `rows`), then per frame `cols * rows` 4bpp tiles in row-major cell
  order. Type-0 categories (0-2) add one 4-bit palette bank per cell,
  padded to `(cells + 7) / 8 * 4` bytes, after each frame's tiles.
  `gCategoryFamily0CellAnim` (19x13, 60 frames, with banks) and
  `gCategoryFamily1CellAnim` (38x10, 21 frames, bank 0).
- **BG1 pictures** (`bgPicture`, loaded by `LoadBgPicture`):
  `struct bg_picture_header` (palette, `cols`, `rows`, `tileCount`), the
  `u16` map, the tiles, then one bank nibble per map entry.
  `gCategory3BgPicture`, `gCategory4BgPicture`, `gCategory5BgPicture`
  (all 38x16).

The 4bpp data uses all 16 palette banks, so each PNG is 8bpp indexed
with the asset's full 256-colour palette, and every 8x8 cell is drawn in
its own bank (index = bank * 16 + pixel). That's how the image looks in
game, and it's what grit's `-mRtp` map reduction reads the bank bits
back from. A cell animation is one tall PNG with the frames stacked top
to bottom (grit's row-major tile order then gives the frames back to
back). A picture is two PNGs: the picture itself (palette and map) and
its tile set as an 8px-wide strip, which is grit's external tileset
(`-fx`) so the map indices come out as the ROM's (none of these tile sets
starts with a blank tile).

grit does all the conversion (flags in `graphics.mk`):

| output | grit flags | then |
|---|---|---|
| palette | `-p -pn256` | `tools/bin2c.py --u16` |
| tiles | `-gt -gB4 -p!` | `tools/bin2c.py` (picture), `tools/grit_bg.py frames` (animation, split per frame) |
| picture map + banks | `-gt -gB4 -p! -m -mRtp -mLf -fx <tiles.png>` | `tools/grit_bg.py map` / `banks` split the entries into the index and the bank nibbles |
| animation banks | `-gt -gB4 -p! -m -mRtp -mLf` | only the bank bits are used; `tools/grit_bg.py frames --banks` packs them after each frame's tiles |

grit also writes the `-fx` tile set back out when it finishes, so the map
rule hands it a copy under `build/` rather than the PNG in `graphics/`.
Rewriting the source PNG raced with the tiles rule reading it in a
parallel build.

grit takes a tile's bank from its first pixel with a non-zero low
nibble, so a cell that is entirely colour 0 would lose its bank. None of
these assets has such a cell; an edit that adds one needs the pixel to
keep a non-zero colour somewhere. The header fields (`cols`, `rows`,
`tileCount`) are hand-written in the data files, and each one checks
its struct's size with `COMPILE_TIME_ASSERT`.

The spawn tables in the same regions are generated in place from
`data/levels/spawns/*.json` (docs/levels.md, "Actor categories"): each
`ACTOR_SPAWN_TABLE(n)` (`actor.h`) is the course length, the spawn
count, the `n` spawns (`struct actor_spawn_entry`: the `struct
actor_spawn` and the next target) and 4 bytes after the last one that
nothing reads (docs/casts/spawn_table.md).

The 2-byte pads after the two LZ77 sheets (formerly
`gStaticData_080C0C36`, `gStaticData_08151AC2`) are built: see "LZ77
stream padding" below.

### Compressed sprite frames

The three blobs next to the category backgrounds that were called
"rotation strips" (`gPolarPlayerRleFrames`, `gYetiRleFrames`,
`gJetpackPlayerRleFrames`) are sets of **zero-run-compressed OBJ frames**,
stored back to back (docs/data_map.md has how this was found). A frame
is:

```
u8  w, h;          // size in 8x8 tiles
u8  0x30, 0;
u16 zeros;         // then, until w*h*16 halfwords are written:
u16 literals; u16 data[literals];
u16 zeros;         // ...
```

The IWRAM routine `0x03000634` (`UnpackRleSpriteFrame` in
`src/engine/gfx/sprite_arm.cpp`, the `gUnpackRleSpriteFrameFunc` hook, called by
`polar_player.cpp`, `jetpack_player_update.cpp` and `company_logos.cpp`)
unpacks a frame into a VRAM tile block. The frame pointer tables
(`table_B` of animation record 0 of both category families, and
`gYetiFrames`) point at the frame headers.

Each set is one PNG: `graphics/vehicles/polar/player.png` (A, the polar
player), `vehicles/polar/yeti.png` (B, the Yeti) and
`vehicles/jetpack/player.png` (C, the jetpack player):
4bpp indexed, one frame per `w*8` x `h*8` block, the frames stacked top to
bottom, so grit's row-major tile order is the frames back to back. The
PNG's 16 colours are the ones the game draws the set with: the players'
sets (A and C) use bank 0 of their category's OBJ palette
(`gPolarCategoryPalette`, `gAirshipCategoryPalette`; both player records'
`palette` is 0), the Yeti's (B) uses `Yeti::palette`, which
`Yeti::UpdatePalette` DMAs to OBJ palette 15. The build reads only the
4-bit indices (grit `-p!`), so the colours never reach the ROM. The build
(`graphics.mk`):

| step | tool | output |
|---|---|---|
| tiles | grit `-gt -gB4 -p! -ftb -fh!` | `<set>.img.bin` |
| compression | `tools/rle_sprites.py pack --frame WxH` | `<set>.inc` (the bytes), `<set>.h` (`RLE_SPRITES_<NAME>_SIZE`, `_COUNT`, `_FRAME_nnn` byte offsets; NAME is `POLAR_PLAYER`, `YETI` or `JETPACK_PLAYER`) |

`src/game/vehicles/polar/data/polar_rle_sprites.cpp` (sets A and B) and `jetpack_rle_sprites.cpp`
(set C) declare `const u8 gStaticData_<addr>[RLE_SPRITES_<NAME>_SIZE]`
around the `.inc`. The frame size per set (graphics.mk's
`RLE_SPRITE_RULE` calls: 8x8, 10x10, 8x8) is the only thing not in the
PNG.

The compressor uses the ROM encoder's rule, found by comparing: a stretch
of at least 7 zero halfwords becomes a zero run, a shorter one stays in
the literal run, and the frame's leading zeros are always a zero run. It
rebuilds all 344 frames byte for byte. An edited frame compresses to a
different length, which moves every later frame, so pointers into a set
must use the generated offsets: `yeti_frames.cpp` writes
`gYetiRleFrames + RLE_SPRITES_YETI_FRAME_nnn`, and the other two
frame tables (`gPolarPlayerFrames`, `gJetpackPlayerFrames`, in the
category family files) use `RLE_SPRITES_POLAR_PLAYER_FRAME_nnn` /
`RLE_SPRITES_JETPACK_PLAYER_FRAME_nnn` the same way.

`tools/gfx_manifest.py extract` writes the three PNGs from `baserom.gba`
again (`rle_sprites.render`, which checks that every frame re-encodes to
the ROM's bytes).

### LZ77 stream padding

Apart from the cutscene bitmaps, the ROM's LZ77 streams (the graphics
and the packed level assets) start on a word boundary, and most are
followed by 1-3 zero bytes that bring the next asset back to one. Those
bytes used to be 70 raw labels (`gStaticData_080C0C36`,
`gStaticData_08151AC2`, `gStaticData_085A5519`, 66 between
`0x0861BF2E` and `0x086EA0C9`, `gStaticData_086ECCD2`); no code
referenced any of them.

They are alignment. Each of these streams is a `u8` array declared
`__attribute__((aligned(4)))` in `src/*/data/` (`polar_sprite_sheet.cpp`,
`jetpack_sprite_sheet.cpp`, `font_tiles.cpp`,
`menu_graphics.cpp`, `level_select_graphics.cpp`,
`level_bg_tiles.cpp`, `level_assets.cpp`), with
`tools/bin2c.py --lz` trimming gbagfx's output to the stream itself.
Between two arrays of one object the assembler pads with zeros; after an
object's last array the linker does, up to the next object's aligned
`.rodata`. `tools/report_units.py` counts the padding toward the blob in
front of it, the linker's fill included. An edited asset that compresses
to a different length gets the right padding for its new length, so the
next asset stays aligned. The cutscene bitmaps, which the ROM packs back
to back without a pad, have no `aligned(4)`.
