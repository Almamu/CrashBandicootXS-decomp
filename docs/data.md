# Data: turning ROM data into source

Every compiled function in the ROM is C. The data after the code
(`0x0803B8B0` to the end of the ROM) is still mostly `.incbin
"baserom.gba", ...` in `data/data.s`. This document is the convention for
turning it into real source, starting with pointer tables. For how data
progress is measured, see [decomp_dev.md](./decomp_dev.md), "Data
progress".

## The convention: C `const` arrays in `src/data/`

A converted table is a C `const` object in a `src/data/*.c` file, with a
real element type (from a shared header when the consuming code has one)
and real symbol references for every pointer. The linker script places
the file's `.rodata` at the table's ROM address, between the raw parts of
`data/data.s`.

- **One file per contiguous run of converted tables.** Name it
  `<what>_<ROM offset>.c`, the offset in lowercase hex without the
  `0x08` (`action_table_16bf20.c` is at `0x0816BF20`), the way
  `src/graphics/actor_part_16048.c` is named. Two C files can sit next to
  each other when their tables are unrelated (`bg_package_16c58c.c`,
  then `image_table_16c5a0.c`).
- **Symbol names don't change.** The code references
  `gStaticData_XXXXXXXX`, and so does the frozen `expected/` assembly the
  code report diffs against, so a converted table keeps its label. The
  meaning goes into the type and a comment naming the consumer. A new
  label is fine where a table had none (`gStaticData_0816C2B0`), and a
  label nothing references can be dropped when a C table spans it (see
  `actor_category_175558.c`).
- **Types come from the code's headers** when there is one:
  `struct actor_pmf` (`actor_self.h`), `struct category_descriptor` /
  `struct category_vtable` (`actor_anim.h`), `struct bg_package`
  (`graphics_package.h`), and the new `struct vtable_slot` (`vtable.h`).
  When the only view is a struct local to one `.c` file, the data file
  uses a plain type or a small local struct and says whose view it
  mirrors. Moving such structs into headers is a separate cleanup.

### Layout: `data/data.s` sections and `ldscript.txt`

`data/data.s` is split into sections at every C object. Each section is
named after the ROM address of its first byte, and a comment marks the
hole:

```
gStaticData_0816BF14:
	.incbin "baserom.gba", 0x0016BF14, 0x0000000C

@ gStaticData_0816BF20..gStaticData_0816C070: src/data/action_table_16bf20.c

.section .rodata.0816C090

.global gStaticData_0816C090
gStaticData_0816C090:
	.incbin "baserom.gba", 0x0016C090, 0x000001C0
```

The `/* Data */` block of `ldscript.txt` lists every piece in ROM order:

```
build/crashbandicootxs/data/data.o(.rodata);
build/crashbandicootxs/src/data/action_table_16bf20.o(.rodata);
build/crashbandicootxs/data/data.o(.rodata.0816C090);
...
```

An input-section name in the linker script matches exactly, so
`data.o(.rodata)` never pulls in `.rodata.0816C090`. The first section is
plain `.rodata` (from `0x0803B8B0`). This scales to any number of C
objects, generated ones included: each one is one more line pair.
`src/data/*.c` is already covered by the Makefile's `src/*/*.c` wildcard;
nothing else needs registering. Everything the linker script doesn't
name is discarded (`/DISCARD/`), so a C data file's empty `.text` is
harmless.

### Converting a table

1. **Find it and decode it.** Dump the blob's words. `0x08xxxxxx` values
   are ROM pointers. Odd ones that land on a function symbol (`nm
   crashbandicootxs.elf`) are Thumb function pointers, and even ones point
   at data. Then read how the C uses it: `grep -rn gStaticData_XXXXXXXX
   src include`. The patterns found so far:
   - `{0xFFFF0000, fn}` pairs: a gcc 2.x pointer-to-member-function
     (thisOffset 0, index -1 = non-virtual, then the code address), the
     tables behind `ACTOR_PMF_CALL`. Initialize them with
     `ACTOR_PMF(fn)`.
   - `{0, fn}` pairs after an all-zero first pair: a gcc 2.x virtual
     table (`struct vtable_slot`, `VTABLE_SLOT(fn)`).
   - Plain words that are all odd code addresses: a function-pointer
     array.
   - Records with pointers among scalars: find the struct the consumer
     reads them through (`struct bg_package` is `{w, h, palette, tiles,
     map}`).
2. **Write the C.** One `const` object per label, in ROM order. Declare
   each function it references as `extern void sub_XXXX();` (unprototyped,
   so it can't clash with the real signature) and each data label as
   `extern const u8 gStaticData_XXXX[];`. A pointer into the middle of a
   still-raw blob is `gStaticData_XXXX + 0x20`. Put a comment on each
   table naming the functions that read it.
3. **Cut it out of `data/data.s`.** Delete the labels' blocks, leave the
   `@ first..last: src/data/file.c` comment, and start a new
   `.section .rodata.<ADDR>` at the next label, `<ADDR>` being that
   label's ROM address. (When the next thing is already another C
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

### What agbcc does with `const` data

Checked by compiling test tables and by the conversions themselves:

- A `const` object goes to `.section .rodata`, in definition order,
  each preceded by `.align` for its type (`.align 2, 0` for anything with
  a word or pointer), with zero fill. No hidden padding between objects
  beyond that alignment.
- A function reference is emitted as `.word sub_XXXX`. The linker sets
  the Thumb bit itself: the R_ARM_ABS32 relocation against a Thumb
  function symbol yields the odd address, so never write `sub_XXXX + 1`.
- Designated initializers work (`{ .fn = sub_XXXX }`), which a union
  whose first member isn't the pointer needs (`struct actor_pmf`'s
  `u.vtableOffset`/`u.fn`).
- Integer constants cast to pointers work in initializers
  (`(void (*)(void))0xffffffef` for the non-code slots of
  `struct category_vtable`).

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
- **Alignment.** A C table starts at its type's alignment. Only convert a
  table whose ROM address already has that alignment (all pointer tables
  do); `tools/report_units.py` refuses a misaligned one rather than let
  the linker insert fill.
- **Pointers into a built blob.** A table pointing into data that is
  itself built from an editable source (the GAX2 audio, a sprite sheet)
  must not hard-code an offset into it, since editing the source moves
  things. `gStaticData_0816AA20`, the 19-entry song table, points into
  the rebuilt GAX2 blob, and stays raw until `tools/gax_audio.py` exports
  per-song symbols. Offsets into still-raw blobs are fine: the raw bytes
  can't move.

### Fallback: typed directives in `.s`

The other route is to write the table in assembly: `.4byte sub_XXXX` (the
linker adds the Thumb bit here too), `.2byte`, `.byte`, in `data/data.s`
or a `data/*.s` file linked the same way. The C route has matched
byte-for-byte on every table so far, so this is only for a table C
provably can't express. Nothing uses it yet. If something does,
`tools/report_units.py` needs to learn to size and classify directive
blobs (today it only understands `.incbin` lines in `data/data.s`).

## What has been converted

The first batch (all pointer tables, all byte-exact):

| File | ROM | Contents |
|---|---|---|
| `bg_package_16b284.c` | `0x0816B284` | 1 `struct bg_package` |
| `entry_set_16b92c.c` | `0x0816B92C` | 2 `{entries, 0x100}` sets |
| `action_table_16bf20.c` | `0x0816BF20` | 42-slot player action PMF table, per-mode animation row pointers |
| `player_pmf_16c250.c` | `0x0816C250` | 2 player-controller PMF tables, an entry table and its set |
| `entry_set_16c418.c` | `0x0816C418` | an entry table and its set |
| `bg_package_16c484.c` | `0x0816C484` | 1 `struct bg_package` |
| `bg_package_16c58c.c` | `0x0816C58C` | 1 `struct bg_package` |
| `image_table_16c5a0.c` | `0x0816C5A0` | 10 `{palette, tiles}` asset pairs |
| `dispatch_table_16c6a4.c` | `0x0816C6A4` | the unified 92-slot function-pointer dispatch array |
| `actor_category_175558.c` | `0x08175558` | 7 `struct category_descriptor`, 3 `struct category_vtable` |
| `actor_pmf_17a6b8.c` | `0x0817A6B8` | 1 actor PMF table |
| `actor_state_fn_17a840.c` | `0x0817A840` | 4 state functions |
| `frame_table_17a880.c` | `0x0817A880` | 123 frame pointers |
| `actor_pmf_17c1c0.c` | `0x0817C1C0` | 1 actor PMF table |
| `actor_pmf_17c260.c` | `0x0817C260` | 3 actor PMF tables |
| `actor_state_17c3fc.c` | `0x0817C3FC` | 1 function table, 2 actor PMF tables |
| `actor_pmf_17c450.c` | `0x0817C450` | 1 actor PMF table |
| `actor_state_17c4c8.c` | `0x0817C4C8` | 1 function table, 2 actor PMF tables |
| `bg_package_17c594.c` | `0x0817C594` | 3 `struct bg_package` |
| `popup_glyphs_17cf40.c` | `0x0817CF40` | 5 glyph packages, 9 slot seeds |
| `slot_seeds_17d6c0.c` | `0x0817D6C0` | 20 slot seeds, 3 `struct bg_package` |
| `digit_glyphs_17e714.c` | `0x0817E714` | 6 glyph pointers |
| `level_tilesets_17e78c.c` | `0x0817E78C` | a `u16[16]` (`sub_8037388`), level BG tile sets 1-3 (grit, see "Resources") |
| `level_rooms_24b638.c` | `0x0824B638` | the level data of 33 rooms, generated from `data/levels/` (see "Level data") |
| `level_tilesets_270f08.c` | `0x08270F08` | level BG tile sets 4-5 (grit) |
| `level_rooms_2b91d0.c` | `0x082B91D0` | the level data of the other 8 rooms (same) |
| `sprite_tiles_2bf120.c` | `0x082BF120` | the sprite tile pool (56 banks) and the 125 fixed OBJ tiles (grit) |
| `sprite_banks_4a5600.c` | `0x084A5600` | the sprite-bank table header, 56 `struct sprite_bank`, banks 0-9 (see "Sprite banks" below) |
| `sprite_banks_4b0ae0.c` | `0x084B0AE0` | sprite banks 10-21 |
| `sprite_banks_4b414c.c` | `0x084B414C` | sprite banks 22-38 |
| `sprite_banks_4b9d7c.c` | `0x084B9D7C` | sprite banks 39-55 |
| `entity_vtables_7e3bec.c` | `0x087E3BEC` | the 93 entity virtual tables |

The actor-category backgrounds and tables (grit-built, see "Category
backgrounds" below):

| File | ROM | Contents |
|---|---|---|
| `cell_anim_03b8b0.c` | `0x0803B8B0` | BG0 cell animation A (19x13, 60 frames, per-cell banks), category 0 `sub_effect_table` |
| `sub_effect_0c0c38.c` | `0x080C0C38` | categories 1 and 2 `sub_effect_table`s |
| `cell_anim_0ff1b0.c` | `0x080FF1B0` | BG0 cell animation B (38x10, 21 frames), category 3 BG1 picture and `sub_effect_table` |
| `bg_picture_151ac4.c` | `0x08151AC4` | category 4 and 5/6 BG1 pictures, categories 4-6 `sub_effect_table`s |

The GAX2 audio isn't C: `tools/gax_audio.py` builds it from `sound/`
into two blobs that `data/data.s` incbins from `build/`, the
sound-effect set (`gStaticData_084C0006`, `--sfx`: 88 instruments and 87
samples, `sound/gax_sfx_manifest.json` + `sound/sfx_samples/*.wav`) and
the music block (`gStaticData_0855BCB4`). See docs/audio.md.

### Sprite banks (`gStaticData_084A5600`)

The sprite-bank animation system (`0x084A5600`-`0x084C0006`, 2,429
frames in 56 banks) is typed C in four files, with the structs in
`include/sprite_bank.h`. It was extracted once by `tools/sprite_banks.py`,
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
  `SPRITE_TILES_BANKnn + offset`, relative to the owning bank's array in
  `sprite_tiles_2bf120.c`; those bases are in `sprite_bank.h`.
- The file cuts sit after banks whose piece bytes end on a word: each
  file starts with a word-aligned struct, and `tools/report_units.py`
  doesn't accept linker fill between objects.

Still raw and worth doing next: the per-level record table
`gStaticData_0816C86C` (self-referencing records, used all over the
game loop), `gStaticData_0816CD80` (its 41 room records point at the
palettes and descriptors in `level_rooms_*.c`, so converting it would make
the level data movable), and the animation tables inside
`gStaticData_08178F80`/`gStaticData_0817AA98` (`actor_anim.h`'s
`gStaticData_081796CC`/`gStaticData_0817B2A4`, not labeled yet).

## Resources (grit-style)

Graphics, and other assets built from files, use the same layout as the
hand-written tables: each asset becomes a `const` array in a `src/data/*.c`
object, and `ldscript.txt` places that object's `.rodata` at the asset's
address in the `/* Data */` block. The pixel conversion is done by
[grit](https://github.com/devkitPro/grit). Built this way so far:
`gStaticData_085AA170`, the first intro Mode 4 bitmap
(`src/data/intro_bitmap_5aa170.c`), the actor-category backgrounds
(see "Category backgrounds" at the end), and the uncompressed tile pools
of the old `gStaticData_0817E78C` blob (see "Raw tile pools" below).

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
| Adopted pipeline | grit `-ftb` (layout) -> gbagfx (LZ77) -> `tools/bin2c.py --lz` -> `#include` in a hand-written `src/data/*.c` declaration |

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
do anyway. Every `_bitmap` PNG, every other `_8bpp_tiles` PNG, and every
framed sprite-sheet frame is already indexed. Tile order is row-major
8x8, the same as gbagfx. Flips only matter for maps (see below).

**Palettes.** An indexed PNG carrying `intro/24_61badc.pal` (16 colours)
gives the ROM's bytes with `-g! -p -pn16`. One carrying
`tileset1/11_63cf98.pal` (256 colours) gives them with `-g! -p -pn256`.
The `.bin` palettes whose entries have bit 15 set can't be produced by grit
or by gbagfx's `.pal`. They stay raw `.bin`.

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
src/data/<asset>.c:   const u8 gStaticData_XXXXXXXX[] = {
                      #include "<dir>/<name>.img.bin.lz.inc"
                      };
```

`tools/bin2c.py --lz` trims gbagfx's 4-byte padding back to the stream's
real length. It fails if the trimmed bytes aren't zero. The declaration
(symbol name, type, comment) is hand-written in `src/data/*.c`, so names
follow the repo's conventions, and only the bytes are generated. This
replaces the earlier plan of generating the whole `.c` file under
`build/`. Keeping the declaration checked in means the symbol name, the
element type and the comment live in the repo, and the object sits in
`src/data/`, where the linker script and the report already handle it.
The element type is `u8`: the ROM's LZ77 streams have odd lengths and
are packed back to back. Objects
under `src/data/` get `-iquote build/crashbandicootxs/graphics`, so the
`#include` path is just `<dir>/<file>`.

Per asset kind:

| asset | grit flags | then | status |
|---|---|---|---|
| Mode 4 bitmap (`*_bitmap.png`, 240x160 8bpp linear) | `-gb -gB8 -p!` | gbagfx LZ77 | used (replaces `tools/linear_gfx.py`) |
| 8bpp tileset (`*_8bpp_tiles.png`) | `-gt -gB8 -p!` | gbagfx LZ77 | verified, 14/14 |
| 4bpp tileset (`*_tiles.png`) | `-gt -gB4 -p!` | gbagfx LZ77 | verified 35/35; needs the PNG re-saved as indexed first |
| 16-colour palette | `-g! -p -pn16` on an indexed PNG carrying it | gbagfx LZ77 | verified |
| 256-colour palette | `-g! -p -pn256` | gbagfx LZ77 | verified; bit-15 `.bin` palettes stay raw |
| tilemap (+ tileset) from one full image | `-gt -gB4 -m -mRtf -mLf -fx <tileset.png>` | gbagfx LZ77 | map verified; the tileset itself stays a separate PNG |
| framed OBJ sheets (`graphics/unknown/0*/`) | not migrated: each frame has a 4-byte header grit doesn't know about | `tools/framed_gfx.py` | - |
| raw 4bpp tile pool (`graphics/sprites/`) | `-gt -gB4 -p!` | `tools/bin2c.py --size` | used, 57 PNGs |
| raw 8bpp tag-0x00 tile set (`graphics/level_tilesets/`) | `-gt -gB8 -p!` | `tools/bin2c.py --size`, header in C | used, 5 PNGs |

**Never** use grit's `-gzl`/`-pzl`/`-mzl` or its `-ftc` for LZ77 assets.

### Raw tile pools (`gStaticData_0817E78C`)

The 3.3 MB `gStaticData_0817E78C` blob (`0x0817E78C`-`0x084A5600`, see
docs/data_map.md) is five C objects: three of tiles and two of room data.
None of it is compressed, so there is no gbagfx step: grit's `-ftb`
output goes straight to `tools/bin2c.py`, and the room data is generated
C (see "Level data").

| object | contents | source |
|---|---|---|
| `level_tilesets_17e78c.c` | `gStaticData_0817E78C` (`u16[16]`), tile sets 1-3 | `graphics/level_tilesets/tileset{1,2,3}_*.png` |
| `level_rooms_24b638.c` | room data, 33 rooms | `data/levels/` via `tools/levels.py` (see "Level data") |
| `level_tilesets_270f08.c` | tile sets 4-5 | `graphics/level_tilesets/tileset{4,5}_*.png` |
| `level_rooms_2b91d0.c` | room data, 8 rooms | same |
| `sprite_tiles_2bf120.c` | 56 sprite banks, 125 fixed tiles | `graphics/sprites/bankNN_*.png`, `tile_pool_4a4660.png` |

- **Sprite banks.** The pool at `0x082BF120` has no header. Each of the 56
  banks of `gStaticData_084A5600` owns one back-to-back range of it, in
  bank order. Each range becomes one `const u8 gStaticData_<addr>[]` and one
  4bpp PNG, `bankNN_<addr>.png`. The PNG is as wide as the bank's most
  common OBJ piece (at least 4 tiles), so with 1D OBJ mapping those pieces
  read as sprites. The palette is a grayscale placeholder. Nothing ties a
  bank to a palette: OAM picks one per actor at run time.
- **Level tile sets.** These are tag-0x00 raw tagged assets: a
  `{size << 8}` word, then 8bpp tiles. `include/tagged_asset.h` gives each
  one a sized struct (`TAGGED_RAW_ASSET(n)`, `TAGGED_RAW_HEADER(n)`) whose
  `data` member `#include`s the grit output. Each PNG is 16 tiles wide and
  carries the 256-colour BG palette of the first room that uses it.
- **Padding.** Tile counts are rarely a multiple of the PNG width, so the
  PNGs are padded to whole rows with blank tiles. `graphics.mk` lists each
  asset's real byte count (`TILE_BYTES_<name>`), and `bin2c.py --size`
  trims the padding. It fails if any trimmed byte is non-zero, so drawing
  into the padding is caught.
- **Room data** is decoded typed C now (see "Level data" below): an
  undecoded `.bin` copy wouldn't have counted as converted data.
- `tools/tile_pools.py` extracts all of it from `baserom.gba` and prints the
  `TILE_BYTES_*` lines. Its output is deterministic, and all 62 PNGs
  round-trip through grit byte-exact.

`graphics.mk` adds the PNGs to `GRIT_C_PNGS`.

### Level data

The rooms' level data and level assets ([levels.md](./levels.md)) are
built from `data/levels/` by `tools/levels.py`, from editable sources:
per room a `room.json` (palette, layer settings, entities, parameter
records, links, symbol names) and one decoded tilemap per layer
(`<layer>.map.bin`, `u16` cells, grit's flat map layout). The build
(`levels.mk`) generates:

| output | from | used by |
|---|---|---|
| `build/.../data/levels/<room>/asset.bin` | the room's tilemaps, re-encoded into the ROM's chunk streams | `data/data.s` incbin (7 raw assets) |
| `build/.../data/levels/<room>/asset.bin.lz` | the same, gbagfx LZ77 | `data/data.s` incbin (34 packed assets, formerly `graphics/tileset1/27`-`60`) |
| `build/.../data/levels/level_rooms_<addr>.inc` | all rooms of a region | `#include`d by `src/data/level_rooms_<addr>.c` |

Every pointer in the room data is a symbol reference (the room's own
objects, the tile sets, the assets), and everything the ROM derives
(chunk grids and sets, asset offsets, entity groups and counts, parameter
offsets) is computed from the sources again. `include/level_data.h` has
the types.

### Placing it and counting it

Placement is the same as for a hand-written table (see "Layout" above).
`data/data.s` drops the asset's label and `.incbin`, leaves a
`@ gStaticData_085AA170: src/data/intro_bitmap_5aa170.c` comment, and
starts `.section .rodata.085ADBD1` for what follows. `ldscript.txt` links
`src/data/intro_bitmap_5aa170.o(.rodata)` between the previous section
and that one. agbcc doesn't align a `u8` array, so the next section
starts right at the array's end, which is what the ROM has for these
unaligned stream lengths.

The report needs nothing extra. The object is in `src/data/`, so
`tools/report_units.py` counts it as built data from the `ldscript.txt`
layout like any other table. `make report` depends on `$(C_OBJS)`, and
`graphics.mk` makes the object depend on its generated `.inc`, so grit
and gbagfx run first. The asset was already counted as built data when it
was an `.incbin` of the gbagfx output, so converting it doesn't change
`matched_data`. It only moves the bytes from an `.incbin` into a C array.

### Adding the next asset

1. Make sure the PNG is indexed (convert the grayscale ones first) and the
   grit rule for its kind is in `graphics.mk`.
2. Add it to `GRIT_C_PNGS` in `graphics.mk`, so it's no longer built for a
   `data.s` incbin.
3. Write `src/data/<kind>_<addr>.c` with the hand-written declaration.
   Add a `$(C_BUILDDIR)/data/<kind>_<addr>.o: <...>.lz.inc` dependency
   line to `graphics.mk`.
4. Split `data/data.s` and add the `ldscript.txt` lines, as for any table.
5. Run a full clean `make` (`make tidy && make`). It must print
   `crashbandicootxs.gba: OK`.

### Category backgrounds

The actor categories (`struct category_descriptor`, `actor_anim.h`) point
at two kinds of uncompressed background, both now built from
`graphics/category_bg/` PNGs:

- **BG0 cell animations** (`family_shared_04`, played by `sub_8029890`/
  `sub_80297C8`): `struct cell_anim_header` (256-colour palette, `cols`,
  `rows`), then per frame `cols * rows` 4bpp tiles in row-major cell
  order. Type-0 categories (0-2) add one 4-bit palette bank per cell,
  padded to `(cells + 7) / 8 * 4` bytes, after each frame's tiles.
  `gStaticData_0803B8B0` (19x13, 60 frames, with banks) and
  `gStaticData_080FF1B0` (38x10, 21 frames, bank 0).
- **BG1 pictures** (`conditional_ptr_0C`, loaded by `sub_802F7B0`):
  `struct bg_picture_header` (palette, `cols`, `rows`, `tileCount`), the
  `u16` map, the tiles, then one bank nibble per map entry.
  `gStaticData_0813D934`, `gStaticData_08151AC4`, `gStaticData_08155260`
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

grit takes a tile's bank from its first pixel with a non-zero low
nibble, so a cell that is entirely colour 0 would lose its bank. None of
these assets has such a cell; an edit that adds one needs the pixel to
keep a non-zero colour somewhere. The header fields (`cols`, `rows`,
`tileCount`) are hand-written in the C files, and each C file checks
its struct's size with `COMPILE_TIME_ASSERT`.

The `sub_effect_record` tables in the same regions are hand-written C:
`SUB_EFFECT_TABLE(n)` (`actor_anim.h`) is the `n` records plus the
12-byte `struct sub_effect_table_end` that the one-record-ahead
accessors read after the last one.

The three rotation strips next to them (`gStaticData_080C2758`,
`gStaticData_080DA1D8`, `gStaticData_0815A050`) stay raw: their frame
windows start at offsets that aren't whole tiles into one shared byte
stream, so there's no tile grid for a PNG (docs/data_map.md has the
details). The 2-byte pads after the two LZ77 sheets
(`gStaticData_080C0C36`, `gStaticData_08151AC2`) stay raw `.incbin`s too.
