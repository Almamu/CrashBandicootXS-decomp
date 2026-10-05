# Data map: triage of every raw ROM data blob

Everything after the code, `0x0803B8B0`-`0x08800000`, lives in
[`data/data.s`](../data/data.s). This document covers the **412 labels
there that still copy their bytes from `baserom.gba`** (6,504,293 bytes,
of which the last 106,548 are `0xFF` cartridge fill, so 6,397,745 bytes
count toward the data total). For each one it records what reads it,
what the bytes are, and how hard it would be to build from repo sources.
The data total and the matched share are the ones
[`docs/decomp_dev.md`](./decomp_dev.md) "Data progress" defines:
8,038,172 bytes, of which 1,640,427 (20.41%) are built today.

The same information is in machine-readable form in
[`tools/data_map.json`](../tools/data_map.json), one object per raw
label, with `regions` sub-lists for the composite blobs.

## How this was produced

- **Code references.** The ROM was re-linked with `ld -q` (emit
  relocations) into a scratch ELF. Every `R_ARM_ABS32` relocation in
  `.text` whose resolved value falls in the data range is a literal-pool
  reference from a C function. That gives 648 references, all to label
  starts; the enclosing function comes from the symbol table. A brute-force
  scan of 4-byte-aligned code words for `0x0803B8B0`-`0x08800000` values
  found 41 more hits, all instruction pairs, no missed references.
- **Types.** `extern` declarations of each `gStaticData_*` in `src/` and
  `include/`, plus `COMPILE_TIME_ASSERT` struct sizes.
- **Data-to-data pointers.** Every aligned word in the raw blobs was
  checked for ROM addresses. Many hits in graphics and audio data are
  coincidental (two 16-bit tilemap entries, or a smooth `u32` ramp,
  often look like `0x08xxxxxx`). A pointer was only trusted when a
  consumer's C code explains it, or when a whole structure of them
  checks out (the chained tag-0 assets, the 2,429 frames of the sprite
  banks, the 41 level descriptors).
- **Structure walkers** for the big blobs, written from the matched C:
  the sprite-bank walker (`GetSpriteTileBase`/`GetSpriteFrame`/`graphics_7634.c`),
  the level-descriptor walker (`level_layers.c`, `bg_scroll_layer_25fc8.c`,
  `game_loop5.c`), the category-descriptor fields (`actor_part95.c`,
  `actor_part45d.c`, `include/actor_anim.h`), and a `LoadTaggedAsset`
  header test (tag `0x00`/`0x10`/`0x30`) at every pointer target.

The helper scripts are not in the repo; the algorithms are described
in each section below so the next pass can rebuild them.

**Effort** means the work to turn the bytes into a repo source that
rebuilds them byte-exact: *easy* is a small typed table with known
consumers, *medium* needs a small converter or a careful typed
description, *hard* needs a real format decoder or has an overlap or
layout problem. **Confidence** is about the format identification.

## Summary

Region-level view in ROM order. The six huge blobs are split into the
components found inside them (each gets its own section below). The
404 remaining raw labels are grouped. Every one of them is listed in
the appendix.

| Range | Size | Format | Consumers | Conf. | Effort |
|---|---:|---|---|---|---|
| `0803B8B0`-`080B1444` | 482,196 | BG0 cell animation A (palette, 19x13 cells, 60 frames) | `InitCellAnim`/`UploadCellAnimFrame` via category descriptors 0-2 | high | **converted** |
| `080B1444`-`080B2120` | 3,292 | category 0 `sub_effect_record` table (164 records) | `SelectActorCategory`, `sub_802A5xx` | high | **converted** |
| `080C0C36`-`080C2758` | 6,946 | 2 B pad + category 1/2 `sub_effect_record` tables | same | high | **converted** (the pad is gbagfx's padding) |
| `080C2758`-`080FF1B0` | 248,408 | compressed OBJ frame sets A and B (the old "rotation strips") | `GetAnimFrameData` via table_B `0817941C`/`0817A880`, unpacked by the `gUnpackRleSpriteFrameFunc` IWRAM hook | high | **converted** |
| `080FF1B0`-`0813D934` | 255,876 | BG0 cell animation B (38x10 cells, 21 frames) | `InitCellAnim` via category descriptors 3-6 | high | **converted** |
| `0813D934`-`0814174C` | 15,896 | category 3 BG1 picture + `sub_effect_record` table | `LoadBgPicture`, `SelectActorCategory` | high | **converted** |
| `08151AC2`-`0815A050` | 34,190 | category 4-6 BG1 pictures + `sub_effect_record` tables | same | high | **converted** (the pad is gbagfx's padding) |
| `0815A050`-`08167AD4` | 55,940 | compressed OBJ frame set C | `GetAnimFrameData` via table_B `0817BA44` | high | **converted** |
| `08167AD4`-`0817E78C` | 92,180 | 200 small/mid tables: gameplay, menus, HUD, actors, text (the built sfx table sits in between) | direct, see appendix | mostly high | easy (a few medium) |
| `0817E78C`-`0817E7AC` | 32 | `u16[16]` | `InitLanguageSelectGraphics` | high | **done** (C) |
| `0817E7AC`-`0824B638` | 839,308 | level tile sets 1-3 (tag-0x00 raw 8bpp tiles) | `bg_scroll_layer_25fc8.c` via `bg_layer_desc.tileData` | high | **done** (grit) |
| `0824B638`-`08270F08` | 153,808 | per-room level data, 33 rooms | `level_layers.c`, `game_loop5.c`, `game_loop57.c`, `game_loop41.c` | high | **done** (C, [levels.md](./levels.md)) |
| `08270F08`-`082B91D0` | 295,624 | level tile sets 4-5 | as tile sets 1-3 | high | **done** (grit) |
| `082B91D0`-`082BF120` | 24,400 | per-room level data, 8 rooms | as block 1 | high | **done** (C) |
| `082BF120`-`084A4660` | 1,987,904 | sprite tile pool for the 56 sprite banks | `graphics_7634.c`/`graphics_73dc.c` (`GetSpriteTileBase` + frame offset) | high | **done** (grit) |
| `084A4660`-`084A5600` | 4,000 | 125 OBJ palettes (`gObjPalettes`) | `InitLevelState`/`RunPauseMenu` palette cache, `GetPaletteSlot` | high | **done** (grit) |
| `084A5600`-`084C0006` | 109,062 | sprite-bank table ("master asset table"): header, 56 banks, 2,429 frames | `RunPauseMenu`, `InitLevelState`, every `**gSpriteBankSet` user | high | **converted** (C) |
| `084C0006`-`0855BCB4` | 638,126 | GAX2 sound-effect data set: 88 instruments, 87 8-bit samples, sample table, the SFX voice handler type | `PlaySfx`/`GAX_fx_ex` voices via `GaxSongHeader.sfxTypes` (`StartSong`) | high | **converted** (`gax_audio.py --sfx`) |
| `085A4C5C`-`086ECCD2` | 28,979 | 111 labels between the built intro/tileset1 LZ77 blobs: GAX2 tables and strings, libgcc `__clz_tab` x2, EEPROM tables, 23 intro palettes, 67 alignment pads | direct / slide packages | high | easy (the pads and palettes **done**) |
| `086C127C`-`086D9CAC` | 100,912 | raw level asset (room `0825E7DC`) | `LoadRoom` -> `SetCollisionSource`/`SetBgStreamerSource` | high | **done** (decoded tilemaps) |
| `086ECCD2`-`087E3BEC` | 1,011,482 | 2 B pad + 6 raw level assets | same | high | **done** (the pad is gbagfx's padding) |
| `087E3BEC`-`087E55E4` | 6,648 | 93 gcc 2.x vtables | constructors (`CreateMovingSprite`, ...) | high | easy |
| `087E55E4`-`087E5FCC` | 2,536 | IWRAM image (ARM code + data, copied by `crt0`) | `crt0.s` | high | **converted** (`asm/intr_main.s`, `src/iwram/`; the code counts as code, 540 B of data) |
| `087E5FCC`-`08800000` | 106,548 | `0xFF` fill (not counted as data) | - | high | - |

By category, as a share of the 8,038,172-byte data total:

| Category | Bytes | % of data | Effort |
|---|---:|---:|---|
| Sprite tile pool | 1,987,904 | 24.73% | medium |
| Level tile sets (5 tag-0 assets) | 1,134,932 | 14.12% | medium |
| Raw level assets (7) | 1,112,392 | 13.84% | medium (opaque `.bin`), hard (real decode) |
| BG0 cell animations (2) | 738,072 | 9.18% | medium |
| GAX2 second data set (sound effects) | 638,126 | 7.94% | **converted** |
| Compressed OBJ frame sets (3, the old "rotation strips") | 304,348 | 3.79% | **converted** |
| Per-room level data | 178,208 | 2.22% | hard |
| 404 small/mid labels | 127,807 | 1.59% | easy (a few medium) |
| Sprite-bank table | 109,062 | 1.36% | **converted** |
| BG1 pictures (3) | 37,096 | 0.46% | medium |
| `sub_effect_record` tables (7) | 23,224 | 0.29% | easy |
| Other (125-tile pool, IWRAM image, pads, `u16[16]`) | 6,574 | 0.08% | easy/medium |

### Findings that change earlier notes

- **Testing the tag-0x00 theory (docs/graphics.md, "The two largest
  untyped ROM regions").** It is half right. `gLanguageSelectPalette3` does
  hold tag-`0x00` assets: five of them, `{u32 size << 8}` plus raw bytes,
  in two chains where each asset ends exactly where the next begins.
  But they are **BG level tile sets** (8bpp, 64-byte tiles, fed to the
  VRAM tile-slot pool at `tiles + 4`), not sprites. The **sprite tiles
  have no tag header at all**. They are one raw 1.99 MB pool, addressed
  as `tileBase + (frame.packed & 0xFFFFFF)`, where `tileBase` is the
  second word of the `gSpriteBankTable` header. `gSpriteBankTable`
  itself is not graphics: it is the sprite-animation metadata plus an
  audio bank. That is why no LZ77/RL signature was ever found.
- **"The body is one dense 12-byte record array"** (docs/rom_map.md,
  `gSpriteBankTable`) is only true of 56 records. Past them come the
  per-bank animation/frame/piece tables (to `0x084C0006`), and then
  638 KB of GAX2 audio.
- **"There is no separate sound-effect sample bank"** (docs/audio.md)
  was wrong. `0x084C0006`-`0x0855BCB4` has the exact GAX2
  instrument and sample-table layouts, with 87 samples and 88
  instruments, and the 36-byte prefix of the built music block (then
  `gax_header_prefix.bin`) points into its tail (`0x0855BC98`). That
  prefix is the engine's `sfxTypes` array, so this set is the sound
  effects (confirmed from the matched engine code, see below).
- **Most of `graphics/tileset1/*.bin` were not graphics.** Files `27`-`60`
  (all but `21`-`26`) were LZ77-packed **level assets** (per-room chunk
  streams, `level_desc.asset` with `assetPacked = 1`, the same format as
  the seven raw ones). They are gone from `graphics/tileset1/` now: the
  build re-encodes them from the rooms' decoded tilemaps in `data/levels/`
  and gbagfx packs them again ([levels.md](./levels.md)). Files `23`-`26`
  are BG tile sets used by `bg_layer_desc.tileData`, and could become
  PNGs like `21`/`22`.
- **`gActorCategories/64/84` are one array.** They are one
  `category_descriptor[7]`, cut into three labels because code takes
  `+0x0C` and `+0x2C` literals. Convert them together.

## The huge blobs, in ROM order

### `gCategoryFamily0CellAnim` (485,488 B): BG0 cell animation A + category-0 table

Nothing references this label directly. It is reached through the
category descriptors `gActorCategories[0..2]` (`include/actor_anim.h`):
`cellAnim = 0x0803B8B0`, `cellAnimSize = 0x75B94`, and
`sub_effect_table = 0x080B1444` for category 0. `InitActorCategory`
passes the first two to `InitCellAnim` (`src/graphics/actor_part95.c`).
That function reads a "cell record": a 256-colour palette (DMA'd whole to
`0x05000000` by `ResetCellAnimBg`), `s16` cols and rows at `+0x200`/`+0x202`,
then frames of `cols*rows*32` bytes of 4bpp tiles. When the category type
is 0, each frame also carries `((cols*rows)+7)/8*4` bytes of side data.
`UploadCellAnimFrame` DMAs one frame per tick to BG0 and hands the side data to
the `gDrawMirroredTilemapFunc` callback. The frame count is `(size - 0x204) /
frameSize`.

| Range | Size | Content | Effort |
|---|---:|---|---|
| `0803B8B0` | 0x75B94 | palette[256], cols=19, rows=13, **60 frames x 8,028 B** (7,904 B tiles + 124 B side data). `0x204 + 60*8028 = 0x75B94` exactly | medium |
| `080B1444` | 0xCDC | category 0 `sub_effect_record[164]` (`0x14` each, count in record 0's `field_04`) + a 12-byte terminator (`{threshold, -1, ...}`) | easy |

**Status: converted** (`src/data/cell_anim_03b8b0.c`, see
[data.md](./data.md) "Category backgrounds"). The side data turned out
to be one 4-bit palette bank per cell (low nibble first, 247 cells
padded to 124 bytes): drawn with those banks, every frame is a clean
picture. The animation is one 152x6240 indexed PNG
(`graphics/category_bg/03b8b0_cell_anim.png`, the 60 frames stacked,
each cell drawn in its bank), converted by grit, and the table is a
`SUB_EFFECT_TABLE(164)` of `struct sub_effect_record`.

### `gStaticData_080C0C36` (527,126 B): sub-effect tables, compressed frame sets, cell animation B, BG1 picture

Every region is reached through the category descriptors or an
animation-table `table_B` array:

| Range | Size | Content | Evidence | Effort |
|---|---:|---|---|---|
| `080C0C36` | 2 | zero pad (gbagfx's padding of the `.lz` stream before it) | | done |
| `080C0C38` | 0xDB8 | category 1 `sub_effect_record[175]` + 0xC | descriptor 1 `+0x14`, count in record 0 | easy |
| `080C19F0` | 0xD68 | category 2 `sub_effect_record[171]` + 0xC | descriptor 2 `+0x14` | easy |
| `080C2758` | 0x17A80 | **compressed frame set A**: 152 back-to-back zero-run-compressed frames `{8, 8, 0x30, 0}` + run stream | the 152 absolute pointers of `table_B` `0x0817941C` (anim record 0 of categories 0-2, 148 distinct) all land on a frame header | medium |
| `080DA1D8` | 0x24FD8 | **compressed frame set B**: 112 frames of 10x10 tiles | the 123 pointers of `gYetiFrames` (the `CreateYeti` singleton's `table_B`, 112 distinct) | medium |
| `080FF1B0` | 0x3E784 | **BG0 cell animation B**: palette[256], cols=38, rows=10, **21 frames x 12,160 B** (no side data, type != 0) | descriptors 3-6 `+0x04`/`+0x08` = `{0x080FF1B0, 0x3E784}`; `0x204 + 21*12160` exact | medium |
| `0813D934` | 0x3498 | **BG1 picture** (category 3, descriptor `+0x0C`) | `LoadBgPicture` layout below; size exact | medium |
| `08140DCC` | 0x980 | category 3 `sub_effect_record[121]` + 0xC | descriptor 3 `+0x14`; ends exactly at the `0814174C` LZ77 sheet | easy |

**Status:** the two tables are converted (`src/data/sub_effect_0c0c38.c`),
and cell animation B, the category 3 picture and its table too
(`src/data/cell_anim_0ff1b0.c`), and the two frame sets
(`src/data/rle_sprites_0c2758.c`, see below). The pad at `080C0C36` is
built too: it is the zero padding gbagfx writes after the `.lz` stream
(docs/data.md, "LZ77 stream padding").

The **BG1 picture** format, from `LoadBgPicture` (`actor_part45d.c`):
`u16 palette[256]`, `s16 cols @0x200`, `s16 rows @0x202`,
`u32 tiles @0x204`, `u16 map[cols*rows]` padded to 4 bytes (`((n+1)/2)*4`),
`tiles*32` bytes of 4bpp tiles, then `(n+1)/2` bytes of palette-bank
nibbles (one per map entry). All three pictures (`0813D934`, `08151AC4`,
`08155260`) end exactly at the next known structure. `gbagfx` can handle
the tiles, and a tiny packer can handle the header, map and nibbles.

The **compressed frame sets** (formerly "rotation strips"). Earlier
passes read every `table_B` target as a `{w, h, 0x30, 0}` header plus
`w*h*32` raw tile bytes, the way `LoadSpriteFrameTiles` uploads a plain
frame. Read that way, consecutive frames overlapped ("sliding windows"
106-2,818 bytes apart), each header sat inside the previous window's
pixels, the last windows ran past each region's end, and the tiles came
out scrambled. The `0x30` byte is the clue: these frames are
**compressed**, and they are never passed to `LoadSpriteFrameTiles`.

The consumers (`actor_part127.c`, `actor_part128.c`,
`graphics_loading_3686c.c`) call `gUnpackRleSpriteFrameFunc(vramBlock, frame)`.
That IWRAM variable is initialised by the `crt0` copy of the IWRAM image
(`0x087E55E4 + 0x874`) to `0x03000634`, an ARM routine in the same image.
Disassembled, it fills `w*h*32` bytes of VRAM from a stream of `u16`
counts after the header: a count of zero halfwords (DMA3 fixed-source
fill from a zero on the stack), then alternately a literal count
followed by that many halfwords (DMA3 copy) and a zero count, until the
frame is full. (Its neighbour `gLookupSpriteFrameCacheFunc = 0x030006FC`, the
`LoadSpriteFrameTiles` hook, only looks the frame up in the VRAM frame
cache.) The frames are simply stored back to back, and decoding them in
order covers each region exactly, with no overlap and nothing past the
end:

| Set | Frames | Frame size | Distinct `table_B` targets | Content (category palette bank 0) |
|---|---:|---|---:|---|
| A `080C2758`-`080DA1D8` | 152 | 8x8 tiles | 148 of 152 pointers (4 frames unused) | Crash riding the polar bear |
| B `080DA1D8`-`080FF1B0` | 112 | 10x10 tiles | 112 of 123 | the yeti |
| C `0815A050`-`08167AD4` | 80 | 8x8 tiles | 80 of 80 | a boss-sized character (categories 3-6) |

The original encoder's rule is simple: every stretch of at least 7 zero
halfwords is a zero run, shorter ones stay inside the literal run, and
the leading zeros are always a zero run. Re-encoding the decoded frames
with that rule gives all 344 frames byte for byte.

**Status: converted.** Each set is one 4bpp PNG in `graphics/rle_sprites/`
(frames stacked top to bottom), built with grit and compressed again by
`tools/rle_sprites.py`, see docs/data.md "Compressed sprite frames".

### `gStaticData_08151AC2` (90,130 B): categories 4-6

The category data continued after the second LZ77 sheet, same formats as
above:

| Range | Size | Content | Effort |
|---|---:|---|---|
| `08151AC2` | 2 | zero pad (gbagfx's padding of the `.lz` stream before it) | done |
| `08151AC4` | 0x2598 | BG1 picture, category 4 (38x16, 237 tiles) | medium |
| `0815405C` | 0x1204 | category 4 `sub_effect_record[230]` + 0xC | easy |
| `08155260` | 0x36B8 | BG1 picture shared by categories 5 and 6 (38x16, 374 tiles) | medium |
| `08158918` | 0x1718 | category 5 `sub_effect_record[295]` + 0xC | easy |
| `0815A030` | 0x20 | category 6 `sub_effect_record[1]` + 0xC | easy |
| `0815A050` | 0xDA84 | **compressed frame set C**, `table_B` `0x0817BA44` (anim record 0 of categories 3-6, 80 pointers), 80 frames of 8x8 tiles | medium |

**Status:** the pictures and tables (`08151AC4`-`0815A050`) are converted
(`src/data/bg_picture_151ac4.c`), and so is frame set C
(`src/data/rle_sprites_15a050.c`). The pad at `08151AC2` is gbagfx's
padding of the `.lz` stream before it, like the one at `080C0C36`.

### `gLanguageSelectPalette3` (3,305,076 B): level tile sets, room data, sprite tile pool

The only direct reference is `InitLanguageSelectGraphics` reading the first 0x20 bytes
as a `u16[16]`, like its three siblings just before it. Everything else
is reached through two pointer graphs.

**1. Level data.** The level table's room lists (after
`gLevelTable`) point at 48 room records of 0x14 bytes, 41 of
them rooms: `{u16 (*palette)[256]; struct level_desc *desc; s32 kind;
...; u16 catIndex @0x10}` (now `struct level_room` in
`src/data/level_table_16c814.c`). The same record is `struct level_load_args` in
`level_layers.c` (first two fields), `gl_widget_kind` in `game_loop56.c`
(`kind`), and `MedalListItem` in `game_loop18.c` (`linkedObj` is the
`desc`, and `linkedObj->0x1C` is the descriptor's object list). A room
record is the `self->widget` that `RunRoom` hands to `LoadRoom`.
Walking them:

- `struct level_desc` (0x24): `layerData[3]`, `layer0Data`, `tileData`
  (all `bg_layer_desc *`), `asset`, `u8 assetPacked`, and the two object
  lists passed to `SpawnRoomEntities`. The 41 descriptors sit at `0824C400`
  ... `08270BCC` (33) and `082B9ED0` ... `082BEADC` (8).
- `struct bg_layer_desc` (0x20, `bg_scroll_layer_25fc8.c`): `u16 *chunkGrid`,
  `u32 assetOffset` (the streamers use `level asset + assetOffset` as
  their decode base, `game_loop5.c`/`game_loop57.c`), `tileData`,
  `scaleX`, `scaleY`, `cnt`, grid width/height in chunks
  (`+0x16`/`+0x18`), size in tiles (`+0x1A`/`+0x1C`).
- `tileData` is either one of the built LZ77 tile sets
  (`graphics/tileset1/21`-`26`), or one of **five tag-0x00 raw assets in
  this blob**. The pooled layer 0 reads those at `tiles + 4`, 64 bytes
  per tile, i.e. 8bpp (`tile_slot_pool.c`, `SetTileSlotPoolSource`).
- `asset` is either an LZ77 blob between the intro graphics
  (`assetPacked = 1`, formerly `graphics/tileset1/27`-`60`), or one of
  **seven raw level assets** in `gRoom17Asset`/`gStaticData_086ECCD2`
  (`assetPacked = 0`).

The full format (the object lists included) is in [levels.md](./levels.md).

**Status: converted.** The blob is now five `src/data` objects
(docs/data.md, "Raw tile pools"): the sprite banks, the 125 fixed tiles
and the five tile sets are indexed PNGs built with grit
(`graphics/sprites/`, `graphics/level_tilesets/`, extracted by
`tools/tile_pools.py`), and `gLanguageSelectPalette3` is a C `u16[16]`. The
two room-data blocks (`0824B638`, `082B91D0`) are typed C
(`level_rooms_24b638.c`, `level_rooms_2b91d0.c`) generated by
`tools/levels.py` from `data/levels/` ([levels.md](./levels.md)).

**2. Sprite banks.** The `gSpriteBankTable` header's second word is
`0x082BF120`, and `GetSpriteTileBase` returns it. `graphics_7634.c` and
`graphics_73dc.c` upload `GetSpriteTileBase() + (frame.packed & 0xFFFFFF)`.

| Range | Size | Content | Evidence | Effort |
|---|---:|---|---|---|
| `0817E78C` | 0x20 | `u16[16]` | `InitLanguageSelectGraphics` | easy |
| `0817E7AC` | 0x67B84 | level tile set 1: header `0x067B8000` (tag 0, 0x67B80 B), 6,638 8bpp tiles | 10 `bg_layer_desc.tileData` refs; ends exactly at the next asset | medium |
| `081E6330` | 0x1AAC4 | level tile set 2 (0x1AAC0 B) | 10 refs; chains | medium |
| `08200DF4` | 0x4A844 | level tile set 3 (0x4A840 B) | 5 refs; chains | medium |
| `0824B638` | 0x258D0 | **room data, 33 rooms**: 256-colour BG palettes, `level_desc`, `bg_layer_desc`, `u16` chunk grids, entity lists, parameter records, per-type counts, links | every byte typed ([levels.md](./levels.md)) | **done** |
| `08270F08` | 0x28EC4 | level tile set 4 (0x28EC0 B) | 7 refs; chains | medium |
| `08299DCC` | 0x1F404 | level tile set 5 (0x1F400 B) | 9 refs; ends where room block 2 starts | medium |
| `082B91D0` | 0x5F50 | **room data, 8 rooms** (same shapes) | | **done** |
| `082BF120` | 0x1E5540 | **sprite tile pool**: raw OBJ tiles, no header | see the next section: the 56 banks use disjoint, back-to-back tile ranges in bank order, and the last one ends at exactly `+0x1E5540` | medium |
| `084A4660` | 0xFA0 | 125 x 32-byte OBJ palettes (16 colours each) | header `+0x08`/`+0x0E` of `gSpriteBankTable`; `InitLevelState`/`RunPauseMenu` hand it to the palette cache, `GetPaletteSlot` copies one palette per `paletteId` | easy |

**Conversion.** The sprite tile pool is plain tile data with known bank
boundaries. One 4bpp PNG per bank through `gbagfx` is a lossless round
trip (any byte string is valid 4bpp data; the pool is 61,994 whole
tiles), and it's viewable with a bank's palette. Banks drawn in 8bpp
(part flag 28) would look scrambled in a 4bpp view but still round-trip.
Doing this single step moves **24.7% of the data total**. The five tile
sets are 8bpp PNG + a generated 4-byte tag header: another 14.1%.
The room blocks are pointer-dense typed data: they became generated C,
see [levels.md](./levels.md).

### `gSpriteBankTable` (747,188 B): sprite-bank table + second GAX2 data set

This is the "master asset table" of docs/rom_map.md. Its structure,
from the matched readers (`RunPauseMenu` in `settings_menu15.c`, `GetSpriteTileBase`/
`GetSpriteFrame`/`GetSpriteAnimPaletteSlot`/`GetSpriteAnimPaletteId` in `actor_part4.c`-`actor_part6.c`,
`graphics_7634.c`, and the `**gSpriteBankSet + N` users):

```
0x084A5600 header (0x10):
    bank_record *banks      = 0x084A5610
    u8 *tileBase            = 0x082BF120   (sprite tile pool, in 0817E78C)
    u8 *palettes            = 0x084A4660   (125 OBJ palettes)
    u16 nbanks = 56, u16 paletteCount = 125
0x084A5610 bank_record[56] (12 B): { anim_record *anims; frame_desc **frames; u16 unk; u16 nanims; }
    (every "**gSpriteBankSet + 0x27C"-style offset in src/ is 12*N: bank N)
per bank, back to back:
    anim_record[nanims] (0x1C): u16 *keyframes @0, two boxes @4/@0xC, u8 paletteId @0x14 (GetPaletteSlot id),
                                u8 duration @0x15, u8 frameCount @0x16, u8 flags @0x17 (bit 1 = loop)
    u16 keyframes[]             (frame indices, frameCount per anim)
    frame_desc *frames[nframes]
    frame_desc[nframes]: piece_offset *offsets; u8 *ids; u32 count<<24 | tileOffset;
                         then 0-3 boxes {s16 x, y; u8 w, h; u16 0} and an optional {s16 x, y} anchor
    piece_offset[] (s16 x, s16 y), u8 ids[] (low nibble = OBJ shape/size index into 0816B2E0/0816B2EC)
```

A frame descriptor is **not** a fixed 0x18 bytes, as first noted here:
its size depends on the high nibble of its first piece byte (the
"layout type"), the same nibble `GetSpriteAttackBox`/`GetSpriteBodyBox`/
`GetSpriteFrameAnchor`/`GetSpriteFrameThirdBox`/`GetSpriteFrameAttackBox`/`GetSpriteFrameBodyBox` switch on to
pick a box or anchor off the frame. Type 1 has nothing after the 12-byte
header, types 2 and 5 one box, 6 a box and an anchor, 3 two boxes, 4
three, and 0 three boxes and an anchor (12 to 40 bytes). With those
sizes, a walk of all 56 banks (330 animations, 2,429 frames) covers
`0x084A5600`-`0x084C0006` with nothing in between but 57 zero alignment
pads of 1-3 bytes; the "385 holes" of the first walk were the boxes and
anchors of a fixed 0x18-byte reading. The only irregularity is three
frames with no pieces (in banks 0, 42 and 47): 12-byte headers that
share the next frame's piece arrays and are directly followed by it.
The anim record's last word (`+0x18`), every box's last `u16` and the
bank's `unk` are always 0. Bank 0 alone is 48 animations, 465 frames and 250 KB of tiles, probably
the player.

**Status: converted** to typed C: `src/data/sprite_banks_4a5600.c` (the
header, the bank table and banks 0-9), `sprite_banks_4b0ae0.c` (10-21),
`sprite_banks_4b414c.c` (22-38) and `sprite_banks_4b9d7c.c` (39-55),
with the structs in `include/sprite_bank.h` (`struct sprite_bank_table`,
`sprite_bank`, `sprite_anim`, `sprite_frame` and its five box/anchor
variants). Every pointer is a symbol reference; frame tile offsets are
written relative to the owning bank's range of the tile pool
(`SPRITE_TILES_BANKnn`). `tools/sprite_banks.py` extracted the C once
from `baserom.gba`, checking the layout above as it goes. The files are
cut after banks whose piece bytes end on a word, because each C object
starts word-aligned. See docs/data.md.

`0x084C0006`-`0x0855BCB4` is **a second GAX2 data set** in the same
formats docs/audio.md describes for music:

| Range | Size | Content |
|---|---:|---|
| `084C0006` | 0x2 | zero pad to a word boundary |
| `084C0008` | 0x3B20 | 88 instruments, each envelope + 12-byte unknown block + rows + 0x8C header (0xAC in all) |
| `084C3B28` | 0x160 | instrument pointer table (88 entries) |
| `084C3C88` | 0x97D30 | 87 signed 8-bit PCM samples, byte-aligned (+3 B pad) |
| `0855B9B8` | 0x2C0 | sample table: 88 x `{u8 *data, u32 length}`, entry 0 empty |
| `0855BC78` | 0x3C | song header (0x1C: volume 0x100, the instrument and sample tables), a 1-entry NULL child-type array, and the handler type (`init`/`unknown`/`play` = `GaxFxChannelInit`/`sub_803A228`/`GaxFxChannelPlay`, 0x48-byte instances) |

The first reading (the "2 B pad + 0x20 B not yet modelled" and the
"8-byte header" after the instruments) was off by one record: the 0x20
bytes are instrument 0's envelope, unknown block and row, and every
instrument is laid out that way, exactly as in the music block.

**Role: the sound effects.** The built music block starts with a 36-byte
prefix that pointed at `0x0855BC98` nine times. It is
`GaxSongHeader.sfxTypes`: `StartSong` passes `gGaxMusicData` there
with `numSfx` = 3, so the engine's sound-effect voices are instances of the
handler type at `0x0855BC98`, and its play function `GaxFxChannelPlay` plays
instruments from this set's tables. `PlaySfx`'s table ids (1-87) are
instrument numbers in this set. docs/audio.md, "Sound effects", has the
whole path.

**Status: converted.** The set is built by `tools/gax_audio.py --sfx` from
`sound/gax_sfx_manifest.json` (the instruments, in the music manifest's
format) and `sound/sfx_samples/01`-`87.wav`, and the music block's
prefix is generated from the set's layout instead of the old verbatim
`gax_header_prefix.bin`. `data/data.s` incbins the built
`build/crashbandicootxs/sound/gax_sfx_data.bin` as
`gGaxSfxData`. The samples are not deduplicated here (84 and 87
are the same bytes, stored twice), unlike the music's.

### `gRoom17Asset` (100,912 B) and `gStaticData_086ECCD2` (1,011,482 B): raw level assets

Each is a `level_desc.asset` with `assetPacked = 0`: the uncompressed form
of what `graphics/tileset1/27`-`60` hold LZ77-packed. The asset is the
"chunk stream" pack read by the custom RLE/delta decoders `DecodeLayerChunk`
(visual layers) and `DecodeCollisionChunk` (terrain cache, 16x8 `u16` chunks). It
starts with a `u16` offset table (offsets x 4). Each layer reads it at
`asset + bg_layer_desc.assetOffset`.

| Asset | Size | Room (`level_desc`) |
|---|---:|---|
| `086C127C` (`gRoom17Asset`) | 0x18A30 | `0825E7DC` |
| `086ECCD4` (after 2 B pad) | 0x1B484 | `082BDF98` |
| `08708158` (`gRoom19Asset`) | 0x2F568 | `08260768` |
| `087376C0` (`gRoom26Asset`) | 0x1B684 | `0825BCDC` |
| `08752D44` (`gRoom27Asset`) | 0x38064 | `0825A390` |
| `0878ADA8` (`gRoom32Asset`) | 0x31394 | `0824E104` |
| `087BC13C` (`gRoom33Asset`) | 0x27AB0 | `0825233C` (ends at the first vtable, `087E3BEC`) |

**Status: converted** (the 2-byte pad at `086ECCD2` is gbagfx's padding
of the LZ77 asset before it). The chunk
streams are decoded into one tilemap per layer (`data/levels/<room>/`),
and `tools/levels.py` re-encodes them byte for byte, the LZ77 ones
included: its encoder reproduces the original tool's token choices and
its leftover bytes. `data/data.s` incbins the built assets under new
labels (`gRoom15Asset`, `gRoom19Asset`, ...). The format
and the encoder are in [levels.md](./levels.md).

### The IWRAM image (`087E55E4`-`087E5FCC`) + fill

`crt0.s` DMA-copies `(gIntrTable - IntrMain_Buffer) / 4` words
from here (`__iwram_lma`) to `0x03000000` at boot. **Converted**: the
image is now built from source and linked to run at `0x03000000`
(ldscript.txt's `iwram` section, stored in ROM with `AT(...)`):
`IntrMain`, the hand-written interrupt dispatcher (`asm/intr_main.s`),
ten ARM C routines (`src/iwram/string_arm.c`, `src/iwram/sprite_arm.c`,
built with agbcc_arm) and the initialised IWRAM globals
(`src/iwram/iwram_data.c`, 540 bytes from `0x030007CC`). The code counts
toward code progress, the globals toward data. The `0xFF` fill from
`0x087E5FCC` is the linker's `rom_fill` section, excluded from data as
before. See docs/decomp_dev.md's "The IWRAM image" and
docs/matching/iwram-image.md.

## Small and mid-size blobs

The other 404 raw labels total 127,807 bytes. Most are cut exactly at
the address the code references, so each one is a single table with one
to a few consumers, and most already have an `extern` with a real type.
By kind (from `tools/data_map.json`):

| Kind | Labels | Bytes | Notes |
|---|---:|---:|---|
| gcc 2.x vtables (`087E3BEC`-`087E55C4`) | 93 | 6,648 | `{0, 0}` then `{s16 delta, s16 0, fn}` slots. Every function is matched C, so these become `.4byte 0, sub_X` or C initialisers and **relocate** |
| function-pointer / pointer-to-member tables | 16 | 1,232 | `0816BF20` (42 `act_pmf`), `0816C6A4` (92 fn ptrs), `081756C4` (3 `category_vtable`), the `actor_pmf` tables in `0817A6B8`-`0817C4F8` |
| pointer tables | 11 | 272 | e.g. `0816AA20` (19 song pointers into the built audio), `0816C5A0`, `0817E714` |
| palettes (16- or 256-colour BGR555) | 51 | 63,397 | 23 are the 256-colour palettes of the intro Mode-4 slides (`085ADBD1`...`08619F51`, each after 0-3 B of alignment pad; some entries have bit 15 set, so they can't round-trip through `gbagfx .pal`; **converted** as C `u16` arrays, `cutscene_pictures_5a9f70.c`), plus `08175760` (32 palette-cycle frames x 0x1C0) and many 0x20 menu/HUD palettes |
| alignment padding | 67 | 133 | 1-3 zero bytes before a 4-aligned LZ77 blob, all between `085A5519` and `086EA0C9`. **Built**: each one is exactly the zero padding gbagfx appends to the `.lz` stream before it, so `data.s` incbins the whole padded file (docs/data.md, "LZ77 stream padding") |
| other typed tables | 166 | 56,125 | `s32`/`u16`/struct tables with known consumers; notable ones below |

Notable mid-size tables:

- `0816C86C` (0x514): the level table, 25 x 0x24 level records (the
  first word, 1, is record 0's name text id, not a header) + 25 room-list
  headers `{count, rooms**, extra1, extra2}`. It is declared under five
  different struct names across `src/` (`gl_level_entry`, `level_info`,
  `level_guard`, `MedalTableEntry`, `threshold_table_entry`).
  **Converted** with the next one (`src/data/level_table_16c814.c`,
  `struct level_info` in `level_data.h`).
- `0816CD80` (0x474): 11 theme music cues, 17 extra-room records, the
  room-list pointer arrays and 31 more records (24 rooms, 7 category
  stages). It is the root of the level-data graph. **Converted**, except
  its last 0x2C bytes, which are the English cutscene text table
  (`0816D1C8`, see `0816D1F4`).
- `0816D1F4` (0x53B4): the cutscenes: slide lists, the text of six
  languages (tables, pages, strings), and the 24 slides (0x1C each) that
  point at the intro pictures (palette + bitmap). **Converted**
  (`src/data/cutscenes_16d1c8.c`, `src/data/cutscene_pictures_5a9f70.c`,
  docs/data.md "Cutscenes").
- `08178F80` (0x1738) / `0817AA98` (0x1728): the two actor-category
  families: palettes, `table_A` keyframes, `anim_table_record[41]`/`[47]`
  (`081796CC`/`0817B2A4`, already declared), and the `table_B` arrays.
  `graphics/unknown/*/entities.json` already records all of it.
  **Converted** (`src/data/anim_family_178f80.c`,
  `src/data/anim_family_17aa6c.c`, docs/data.md "Category families").
- `08175558`+`08175564`+`08175584`: one `category_descriptor[7]`, split over three labels.
- `081725C4` (0x261C): terrain shape records (36 B, heights 0-7, `0xFF`
  empty) for the collision streamer, then the game's UI text in six
  languages. **Converted** (`src/data/terrain_1725a8.c`,
  `src/data/ui_text_172cd4.c`).
- `0817C5D0` (0x96C): popup/credits text opcode stream (`"\x03developed by\n\n\x01..."`).
  **Converted** (`src/data/credits_17c5d0.c`).
- `0816AF10`: the CRC-16/CCITT table (poly `0x1021`) for link-cable packets.
  **Converted** (`src/data/link_crc_16af10.c`).
- `0816A820`: `s16[256]` sine table. **Converted**
  (`src/data/boss_pictures_167ad4.c`).
- `085A4C70`/`085A4D70`: two copies of libgcc's `__clz_tab` (`u8[256]`).
  **Converted** (`src/data/clz_tab_5a4c70.c`).
- `085A60FF`...`085A62CC`: GAX2 version string (`"GAX Sound Engine 2.01D
  (Sep 28 2001) (c) Shin'en Multimedia. Code: B.Wodok"`) and error strings,
  the `RateEntry` table, and `085A62DC`, the GAX2 `u32` period table
  (0x3BD0; its `0x08xxxxxx` values are a smooth ramp, not pointers). **Converted**, with the vibrato wave
  and the EEPROM library's data after it (`src/data/gax_tables_5a6100.c`,
  `src/data/eeprom_5a9eec.c`).

Every one of the 412 labels is listed in the appendix.

## Recommendations

### Order of work (data progress per unit of effort)

1. **Infrastructure first (small, unblocks everything).** Teach
   `tools/report_units.py` to handle anything besides one `incbin` per
   label. The easiest way is to take blob addresses and sizes from the
   linker map (`crashbandicootxs.map`) or the ELF symbol table, not from
   summing `data/data.s`. Today `parse_data_s()` exits on any other line.
   A split `data.s` breaks its address check too: the C experiment below
   tripped it. Also add a "source group" for typed data (`.s` directives
   or `src/data/*.c`), so those units count as built.
2. **Done.** **Sprite tile pool to PNGs** (`082BF120`, 1.99 MB, **+24.7%**). Split
   by bank (the ranges are exact and back to back), one 4bpp PNG each,
   no header. It needs a label split in `data.s`, a `graphics/sprites/`
   directory, and the existing `%.4bpp` rule. It is the best ratio by
   far.
3. **Done (the five tag-0 sets; `tileset1/23`-`26` not yet).** **Level tile sets to 8bpp PNGs** (5 assets, 1.13 MB, **+14.1%**), with the
   4-byte tag header generated by a one-line rule (`size << 8`). Convert
   `graphics/tileset1/23`-`26` (`.bin` today) to PNG in the same pass for
   consistency.
4. **BG0 cell animations and BG1 pictures** (775 KB, **+9.6%**). One small
   packer tool for the two formats documented above.
5. **Done, decoded.** **Raw level assets** (1.11 MB, **+13.8%**): decoded
   into tilemaps and re-encoded by `tools/levels.py` together with the
   34 LZ77 ones (`graphics/tileset1/27`-`60` before), not as `.bin`
   passthroughs.
6. **Done.** **Second GAX2 data set** (638 KB, **+7.9%**): extend `tools/gax_audio.py`.
7. **Small tables** (128 KB, +1.6%, about 400 labels). Do the vtables and
   function-pointer tables early, even though they are small. They are
   the only data that pins code addresses, so converting them to
   symbolic `.4byte sub_X`/C initialisers is what lets code shift without
   breaking the ROM. Then the typed tables, in consumer-file batches.
8. **Done (the sprite-bank table).** **Sprite-bank table** (109 KB) and **category family data**
   (`08178F80`/`0817AA98`, `sub_effect_record` tables): typed C with
   generators. The layouts are all known.
9. **Room data** (178 KB, **done**, +2.2%, generated C from
   `data/levels/`) and the **compressed frame sets** (304 KB, **done**,
   +3.8%, the old "rotation strips": `tools/rle_sprites.py`).

Steps 2-5 alone take data progress from 20.4% to about 83%.

### Conversion convention for small tables

**Recommendation: C `const` arrays in `src/data/*.c`, placed by
`ldscript.txt` in ROM order, with typed `.s` directives as the fallback**
for the cases below. C matches the standing "prefer structs" direction:
the structs already exist in `include/` and `src/`, so the table and its
consumers share one definition, and function pointers are just the
function names.

The experiment (done in the worktree, then reverted, so no ROM-producing
file is changed): `gObjPieceWidths`/`0816B2EC` (the OBJ piece
width/height tables, `u8[12]` each) moved into `src/data/obj_piece_sizes.c`
as `const u8` arrays. `data/data.s` got a second section
(`.section .rodata.after_0816B2F8, "a"`) from the next label on.
`ldscript.txt` got
`data.o(.rodata); src/data/obj_piece_sizes.o(.rodata); data.o(.rodata.after_0816B2F8);`.
Result: `make compare` said **`crashbandicootxs.gba: OK`**, with the
symbols at `0x0816B2E0`/`0x0816B2EC`. `src/*/*.c` is already globbed, so
the Makefile needed no change.

agbcc/ldscript constraints found along the way:

- `const` data goes to `.rodata` (non-`const` to `.data`, which the
  ldscript discards: always use `const`). Function pointers become
  `R_ARM_ABS32` relocations against the Thumb symbol, so they relocate
  properly. Thumb symbols carry bit 0, so `.4byte sub_X` gives `sub_X|1`.
- **Every C object needs its own ldscript slot**, and `data.s` must be cut
  into named sections around it. Each converted table (or run of adjacent
  tables) adds one `.section .rodata.<next>` in `data.s` and two ldscript
  lines. With hundreds of tables, the better fix is to split `data.s`
  into one file per region/unit with matching ldscript entries (the
  `data/*.s` glob already builds them).
- **Alignment.** An object's `.rodata` is aligned to its strictest member.
  agbcc gives `.align 2` (4 bytes) to every struct-typed object and to
  every array initialised from a string literal (`const u8 s[5] = "abcd"`),
  but not to a scalar byte array written as a brace list. Labels at odd
  or 2-mod-4 addresses (`0816B13A`, `0816C308`, `0816C35F`, the 0x16- and
  0x22-byte tables...) must therefore be scalar arrays, or stay in `.s`.
- **Struct padding.** agbcc rounds every struct up to a multiple of 4
  (`struct {u8 x, y, z;}` is 4 bytes, so an array of 2 is 8). Tables with
  3-, 6- or 22-byte records can't use a natural struct. Use flat scalar
  arrays or `.s`.
- **No two C objects can interleave with `data.s` bytes in one section.**
  Where a tiny odd table sits between two C-convertible ones, convert the
  run together or keep it in `.s`.

Use `.s` typed directives (`.4byte sub_X`, `.2byte`, `.byte`, `.ascii`,
`.balign`) for: alignment pads, odd-aligned byte tables, the vtables, if
C++-style vtables are awkward to spell in C (a flat
`{s32 delta; void *fn}[]` works fine in C too), and anything that is
really an opaque stream. Either way, `report_units.py` must learn about
the new source kind first (step 1 above).

## Appendix: every raw label

One row per `baserom.gba` incbin in `data/data.s`, in ROM order. The
consumers are the C functions that load the label's address (or, for
the composites, the functions that reach it through pointers). Composite
blobs are split into regions in the sections above and in
`tools/data_map.json`. Element counts come from the size and the
`extern` type where one exists. Hand-written formats are shown as
written; the rest come from the classifier (pointer, palette and
vtable shapes).

| Address | Size | Format | Consumers | Conf. | Effort |
|---|---:|---|---|---|---|
| `0803B8B0` | 0x76870 | composite: BG0 streamed cell animation A + category-0 sub-effect table. **Converted** (`src/data/cell_anim_03b8b0.c`) | `InitCellAnim`, `UploadCellAnimFrame`, `ResetCellAnimBg` +2 | high | done |
| `080C0C36` | 0x80B16 | composite: sub-effect tables, two compressed OBJ frame sets, BG0 cell animation B, BG1 picture. **Converted**, the 2-byte pad as gbagfx's padding of `00_0b2120.bin.lz` | `SelectActorCategory`, `GetAnimFrameData`, `InitCellAnim` +2 | high | done |
| `08151AC2` | 0x16012 | composite: BG1 pictures, sub-effect tables, compressed OBJ frame set C (categories 3-6). **Converted**, the 2-byte pad as gbagfx's padding of `01_14174c.bin.lz` | `SelectActorCategory`, `GetAnimFrameData`, `LoadBgPicture` | high | done |
| `08167AD4` | 0x200 | N. Gin's airship: 16-colour palette (+ zero to 0x200). **Converted** (`src/data/boss_pictures_167ad4.c`) | `LoadAirshipGraphics`, `ConvertAirshipTiles` | high | done |
| `08167CD4` | 0x1E14 | N. Gin's airship picture: {cols 18, rows 12}, 4 frames of {tile count, u16 map, 4bpp tiles} sharing one pool (graphics/boss_pictures/, tools/boss_pictures.py). **Converted** (`src/data/boss_pictures_167ad4.c`) | `CreateAirship` | medium | done |
| `08169AE8` | 0x200 | Cortex's hovercraft: palette (16 colours + 240 x 0x03E0). **Converted** (`src/data/boss_pictures_167ad4.c`) | `UpdateHovercraftHitFlash`, `LoadHovercraftGraphics`, `ConvertHovercraftTiles` | high | done |
| `08169CE8` | 0xB28 | Cortex's hovercraft picture: {cols 16, rows 10}, 1 frame (graphics/boss_pictures/). **Converted** (`src/data/boss_pictures_167ad4.c`) | `CreateHovercraft` | medium | done |
| `0816A810` | 0x10 | u8[16] d-pad direction lookup. **Converted** (`src/data/boss_pictures_167ad4.c`) | `GetDpadDirection` | medium | done |
| `0816A820` | 0x200 | s16[256] sine/direction table (`s16` x 256). **Converted** (`src/data/boss_pictures_167ad4.c`) | `DrawPlayer`, `UpdateEnemyOscillateX`, `UpdateEnemyBob` +16 | high | done |
| `0816AA20` | 0x4C | song table: 19 pointers into the music block, `gGaxMusicData + GAX_SONG_<NAME>` from the generated `gax_songs.h`. **Converted** (`src/data/song_table_16aa20.c`) | `StartSong` | high | done |
| `0816AF10` | 0x228 | CRC-16/CCITT lookup table (poly 0x1021, u16[256]) + the two link-cable pairing names "crash 1 <-> crash 2"/"crash 1 <-> crash 3" (`gCrash2LinkText`/`0816B124`, which the IWRAM data `gCrash2LinkTextPtr`/`0814` points at). **Converted** (`src/data/link_crc_16af10.c`) | `MakeLinkHandshakeId`, `HandleLinkSerial` | high | done |
| `0816B138` | 0x2 | the text ">". **Converted** (`src/data/menu_tables_16b138.c`) | `DrawYesNoPrompt` | medium | done |
| `0816B13A` | 0x20 | table of u16; 1 word(s) look like ROM pointers (`u16` x 16). **Converted** (`src/data/menu_tables_16b138.c`) | `InitSaveMenuIcons` | high | done |
| `0816B15A` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/menu_tables_16b138.c`) | `InitSaveMenuIcons` | high | done |
| `0816B17A` | 0x20 | table of u16 (`u16` x 16). **Converted** (`src/data/menu_tables_16b138.c`) | `InitSaveMenuIcons` | high | done |
| `0816B19A` | 0x22 | table of u16 (`u16` x 17). **Converted** (`src/data/menu_tables_16b138.c`) | `InitSaveMenuIcons` | high | done |
| `0816B1BC` | 0x14 | table of s32 (`s32` x 5). **Converted** (`src/data/menu_tables_16b138.c`) | `DrawSaveMenuMain` | high | done |
| `0816B1D0` | 0x14 | table of void* (`void*` x 5). **Converted** (`src/data/menu_tables_16b138.c`) | `DrawPauseMenuPageTitle` | high | done |
| `0816B1E4` | 0x8 | table of struct icon_pos. **Converted** (`src/data/menu_tables_16b138.c`) | `InitPauseCrystalsPage`, `DrawPauseCrystalsPage` | high | done |
| `0816B1EC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/menu_tables_16b138.c`) | `InitPausePowersPage` | high | done |
| `0816B20C` | 0x10 | table of u32 (`u32` x 4). **Converted** (`src/data/menu_tables_16b138.c`) | `InitPausePowersPage` | high | done |
| `0816B21C` | 0x28 | table of struct icon_pos. **Converted** (`src/data/menu_tables_16b138.c`) | `DrawPauseGemsPage`, `InitPauseGemsPage` | high | done |
| `0816B244` | 0x14 | table of u32 (`u32` x 5). **Converted** (`src/data/menu_tables_16b138.c`) | `InitPauseGemsPage` | high | done |
| `0816B258` | 0x18 | table of struct icon_pos. **Converted** (`src/data/menu_tables_16b138.c`) | `DrawPauseRelicsPage`, `InitPauseRelicsPage` | high | done |
| `0816B270` | 0xC | table of u32 (`u32` x 3). **Converted** (`src/data/menu_tables_16b138.c`) | `InitPauseRelicsPage`, `InitPauseTimeTrialPage` | high | done |
| `0816B27C` | 0x8 | table of struct icon_pos. **Converted** (`src/data/menu_tables_16b138.c`) | `InitPauseTimeTrialPage`, `DrawPauseTimeTrialPage` | high | done |
| `0816B284` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `InitPauseMenu` | medium | easy |
| `0816B298` | 0x28 | table (element layout: see consumers). **Converted** (`src/data/pause_rows_16b298.c`) | `InitPauseMenu` | medium | done |
| `0816B2C0` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/pause_rows_16b298.c`) | `RunPauseMenu` | medium | done |
| `0816B2E0` | 0xC | table (element layout: see consumers). **Converted** (`src/data/obj_sizes_16b2e0.c`) | `DrawSpritePieces`, `DrawAffineSpritePieces` | medium | done |
| `0816B2EC` | 0xC | table (element layout: see consumers). **Converted** (`src/data/obj_sizes_16b2e0.c`) | `DrawSpritePieces`, `DrawAffineSpritePieces` | medium | done |
| `0816B2F8` | 0x8 | all zero (zero-initialised table). **Converted** (`src/data/obj_sizes_16b2e0.c`) | `QueueCratePlayerCollision`, `GetSpriteAttackBox`, `GetSpriteBodyBox` +3 | high | done |
| `0816B300` | 0x4 | all zero (zero-initialised table). **Converted** (`src/data/obj_sizes_16b2e0.c`) | `GetSpriteFrameAnchor`, `CollidePlayer`, `ActionCtrlHandleEvent` +1 | high | done |
| `0816B304` | 0x318 | 44 {s32, s32, s32} motion records + 33 {a, b} entry pairs (`gActionCtrlMotionEntries`, gActionCtrlMotionSet's entries). **Converted** (`src/data/motion_records_16b304.c`) | `StartCtrlTargetMotionYFromSet`, `StartCtrlTargetMotionXFromSet`, `ApplyActionCtrlMotion` | high | done |
| `0816B61C` | 0x2A4 | 31 {s32, s32, s32} motion records + 38 {a, b} entry pairs (`gPlayerCtrlMotionEntries`, gPlayerCtrlMotionSet's entries). **Converted** (`src/data/motion_records_16b304.c`) | `ApplyPlayerCtrlMotion`, `StartPlayerCtrlMotionYFromSet`, `StartPlayerCtrlMotionXFromSet` | high | done |
| `0816B8C0` | 0x6C | table (element layout: see consumers). **Converted** (`src/data/motion_records_16b304.c`) | `ApplyInputCtrlMotion` | medium | done |
| `0816B92C` | 0x8 | pointer table (1 data pointers) | `PlayRoom` | high | easy |
| `0816B934` | 0x8 | pointer table (1 data pointers) | `PlayRoom` | high | easy |
| `0816B93C` | 0x50 | entry set {entries, 0x100} + its 9 {a, b} entries (`gInputCtrlMotionEntries`). **Converted** (`src/data/entry_set_16b93c.c`) | `PlayRoom` | medium | done |
| `0816B98C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnLizard`, `SpawnVulture`, `SpawnVenusFlytrap` +22 | medium | done |
| `0816B9AC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnPenguin` | high | done |
| `0816B9CC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnPufferfish` | high | done |
| `0816B9EC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnBlowgunTribesman` | high | done |
| `0816BA0C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnVenusFlytrap` | medium | done |
| `0816BA2C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnVulture` | medium | done |
| `0816BA4C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnShark` | medium | done |
| `0816BA6C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnSquid` | medium | done |
| `0816BA8C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnElectricEel` | medium | done |
| `0816BAAC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnFlamethrowerLabAssistant` | high | done |
| `0816BACC` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `sub_8020138` | medium | done |
| `0816BAEC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/popup_tables_16b98c.c`) | `sub_802026C` | high | done |
| `0816BB0C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `sub_80208C4`, `SpawnFrog` | medium | done |
| `0816BB2C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnPistonCrusher`, `SpawnWoodenCrusher` | medium | done |
| `0816BB4C` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/popup_tables_16b98c.c`) | `SpawnSaucerLabAssistant` | high | done |
| `0816BB6C` | 0x28 | entry set {entries, 0x100} + its 4 {a, b} entries (`gEnemyCtrlMotionEntries`). **Converted** (`src/data/object_tables_16bb6c.c`) | `ResetEnemyCtrl` | medium | done |
| `0816BB94` | 0x4 | small constant (281e140a). **Converted** (`src/data/object_tables_16bb6c.c`) | `UpdateSlotCrate`, `CreateCrate` | medium | done |
| `0816BB98` | 0x16 | table (element layout: see consumers). **Converted** (`src/data/object_tables_16bb6c.c`) | `OpenCheckpointCrate`, `BreakCrate`, `ExplodeCrate` | medium | done |
| `0816BBAE` | 0x16 | table (element layout: see consumers). **Converted** (`src/data/object_tables_16bb6c.c`) | `BlastNearbyCrates`, `BreakCratesInArea`, `IsCrateKindBreakable` | medium | done |
| `0816BBC4` | 0x16 | table (element layout: see consumers). **Converted** (`src/data/object_tables_16bb6c.c`) | `BreakCrateTouchedByPlayer`, `DropCratesAbove`, `BlastNearbyCrates` +3 | medium | done |
| `0816BBDA` | 0x16 | table (element layout: see consumers). **Converted** (`src/data/object_tables_16bb6c.c`) | `QueueCratePlayerCollision`, `BreakCrateInStack` | medium | done |
| `0816BBF0` | 0xA8 | table of s32 (`s32` x 42). **Converted** (`src/data/object_tables_16bb6c.c`) | `QueueCratePlayerCollision` | high | done |
| `0816BC98` | 0x268 | table of s32[7] (`s32[7]` x 22). **Converted** (`src/data/object_tables_16bb6c.c`) | `QueueCratePlayerCollision`, `ApplyCrateCollision` | high | done |
| `0816BF00` | 0x8 | small constant (0000010000000000). **Converted** (`src/data/object_tables_16bb6c.c`) | `QueueCratePlayerCollision` | medium | done |
| `0816BF08` | 0xC | table of s32 (`s32` x 3). **Converted** (`src/data/object_tables_16bb6c.c`) | `UpdateExtraLifeHop` | high | done |
| `0816BF14` | 0xC | table of struct three_words. **Converted** (`src/data/object_tables_16bb6c.c`) | `UpdateWumpaHop` | high | done |
| `0816BF20` | 0x150 | pointer-to-member dispatch table: 42 x {0xFFFF0000, fn} (`struct act_pmf` x 42) | `UpdateActionCtrl` | high | easy |
| `0816C070` | 0x20 | pointer table (8 data pointers) (`struct level_anim*` x 8) | `UpdatePlayerCtrl`, `PlayerCtrlStateTurn`, `PlayerCtrlStateStop` +2 | high | easy |
| `0816C090` | 0x1C0 | `struct speed_table` (8 s32) + `struct level_anim[8][13]` (`gPlayerCtrlModeLevelAnims`, the rows gPlayerCtrlModeAnimRows points at). **Converted** (`src/data/speed_table_16c090.c`) | `StartPlayerCtrlStroke` | high | done |
| `0816C250` | 0x40 | function-pointer / pointer-to-member table (8 code pointers) | `UpdatePlayerCtrl` | high | easy |
| `0816C290` | 0x40 | table of struct pmf; 4 word(s) look like ROM pointers | `UpdateInputCtrl` | high | easy |
| `0816C2D0` | 0x8 | pointer table (1 data pointers) | `ResetChaserCtrl` | high | easy |
| `0816C2D8` | 0x30 | table (element layout: see consumers). **Converted** (`src/data/actor_tables_16c2d8.c`) | `SetChaserMotionYFromSet`, `SetChaserMotionXFromSet`, `StartChaserMotionYFromSet` +1 | medium | done |
| `0816C308` | 0x3 | small constant (040100). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateTiny`, `SetTinyState` | medium | done |
| `0816C30B` | 0x4D | table (element layout: see consumers). **Converted** (`src/data/actor_tables_16c2d8.c`) | `PickTinyHopTarget` | medium | done |
| `0816C358` | 0x4 | small constant (100e0a20). **Converted** (`src/data/actor_tables_16c2d8.c`) | `SetCortexTargetDest` | medium | done |
| `0816C35C` | 0x3 | small constant (181612). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateCortexTarget` | medium | done |
| `0816C35F` | 0x3 | small constant (040404). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateCortexTarget` | medium | done |
| `0816C362` | 0x6 | small constant (020202000000). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateCortexTarget` | medium | done |
| `0816C368` | 0x10 | table of s32 (`s32` x 4). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateDingodile` | high | done |
| `0816C378` | 0x18 | table of s32 (`s32` x 6). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateDingodile` | high | done |
| `0816C390` | 0x10 | table of s32 (`s32` x 4). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateDingodile` | high | done |
| `0816C3A0` | 0x18 | table of s32 (`s32` x 6). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateDingodile` | high | done |
| `0816C3B8` | 0x30 | table of s32 (`s32` x 12). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateDingodileShark`, `StartDingodileMotion` | high | done |
| `0816C3E8` | 0xC | table (element layout: see consumers). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateDingodileProjectile` | medium | done |
| `0816C3F4` | 0x24 | table (element layout: see consumers). **Converted** (`src/data/actor_tables_16c2d8.c`) | `UpdateDingodileProjectile` | medium | done |
| `0816C418` | 0x40 | table of struct vec_pair | `StartDingodileMotion` | high | easy |
| `0816C458` | 0x8 | pointer table (1 data pointers) | `CreatePlatformMover` | high | easy |
| `0816C460` | 0x24 | table of struct vec3. **Converted** (`src/data/velocity_16c460.c`) | `UpdatePlatformMover`, `SetPlatformMoverMotionYFromSet`, `SetPlatformMoverMotionXFromSet` +2 | high | done |
| `0816C484` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `LoadSaveMenuBg`, `InitPowerDialog`, `InitLevelSelect` +1 | medium | easy |
| `0816C498` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect` | high | done |
| `0816C4A0` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect` | high | done |
| `0816C4A8` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect` | high | done |
| `0816C4B0` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect` | high | done |
| `0816C4B8` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect` | high | done |
| `0816C4C0` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect`, `DrawLevelSelectTime` | high | done |
| `0816C4C8` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect` | high | done |
| `0816C4D0` | 0x8 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect` | high | done |
| `0816C4D8` | 0x30 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `LevelSelectTurnPage`, `PlaceLevelSelectEntries` | high | done |
| `0816C508` | 0x30 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c498.c`) | `LevelSelectTurnPage`, `PlaceLevelSelectEntries` | high | done |
| `0816C538` | 0x10 | table of u32 (`u32` x 4). **Converted** (`src/data/map_tables_16c498.c`) | `LevelSelectTurnPage`, `SetLevelSelectEntryBoxes` | high | done |
| `0816C548` | 0x10 | table of u32 (`u32` x 4). **Converted** (`src/data/map_tables_16c498.c`) | `InitLevelSelect`, `RefreshLevelSelectPage` | high | done |
| `0816C558` | 0x14 | table of u32 (`u32` x 5). **Converted** (`src/data/map_tables_16c498.c`) | `LoadLevelSelectRecord` | high | done |
| `0816C56C` | 0x20 | table (element layout: see consumers). **Converted** (`src/data/map_tables_16c498.c`) | `RunLevelSelect` | medium | done |
| `0816C58C` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `CreateLevelSelectPageBg` | medium | easy |
| `0816C5A0` | 0x50 | pointer table (20 data pointers) | `UpdateZoomBg` | high | easy |
| `0816C5F0` | 0x20 | table of struct xy_pair. **Converted** (`src/data/map_tables_16c5f0.c`) | `InitZoomBg` | high | done |
| `0816C610` | 0x14 | table of u32 (`u32` x 5). **Converted** (`src/data/map_tables_16c5f0.c`) | `SetLevelSelectEntryBox` | high | done |
| `0816C624` | 0x10 | table of u32 (`u32` x 4). **Converted** (`src/data/map_tables_16c5f0.c`) | `SetLevelSelectEntryLevel` | high | done |
| `0816C634` | 0x10 | table of u32 (`u32` x 4). **Converted** (`src/data/map_tables_16c5f0.c`) | `UpdateLevelSelectCursor` | high | done |
| `0816C644` | 0x30 | table of s32 (`s32` x 12). **Converted** (`src/data/map_tables_16c5f0.c`) | `FitScaledSprite`, `DrawScaledSprite` | high | done |
| `0816C674` | 0x30 | table of s32 (`s32` x 12). **Converted** (`src/data/map_tables_16c5f0.c`) | `FitScaledSprite`, `DrawScaledSprite` | high | done |
| `0816C6A4` (`gEntitySpawnFuncs`) | 0x170 | function-pointer table: 92 Thumb function pointers (menu/trigger-effect dispatch) (`void (*)(void)` x 92) | `CreateEntitySpawner` | high | easy |
| `0816C814` (`gThemePaletteCycle2`) | 0xA | `u16[5]` palette-entry list of a level-start colour cycle. **Converted** (`src/data/level_table_16c814.c`) | `RunRoom` | high | done |
| `0816C81E` (`gThemePaletteCycle1A`) | 0x12 | `u16[9]` colour-cycle list. **Converted** (same) | `RunRoom` | high | done |
| `0816C830` (`gThemePaletteCycle1B`) | 0x12 | `u16[9]` colour-cycle list. **Converted** (same) | `RunRoom` | high | done |
| `0816C842` (`gThemePaletteCycle3`) | 0x20 | `u16[16]` colour-cycle list (palette entries 0x20-0x2F, not a palette). **Converted** (same) | `RunRoom` | high | done |
| `0816C862` (`gThemePaletteCycle5`) | 0xA | `u16[5]` colour-cycle list. **Converted** (same) | `RunRoom` | high | done |
| `0816C86C` (`gLevelTable`) | 0x514 | level table: 25 x 0x24 `struct level_info`, then 25 x 0x10 room lists `{count, rooms**, extra1, extra2}`. **Converted** (same) | `InitPauseMenuInfo`, `InitPauseTimeTrialPage`, `CountPlatinumRelics` +17 | high | done |
| `0816CD80` (`gThemeMusicCues`) | 0x474 | `u8[11]` theme music cues, 17 room records (0x14 each), the room lists' pointer arrays, 31 more room records. **Converted** (same); its last 0x2C bytes are the English cutscene table `0816D1C8` | `PlayRoomMusic` | high | done |
| `0816D1F4` (`gCutscenes`) | 0x53B4 | the cutscenes: 11 slide lists `{slides*, count}` (`struct cutscene_slides`), the English pages, 5 more language tables, slide arrays, text strings and pointer arrays of 6 languages, 24 slides (0x1C, `struct cutscene_slide`). **Converted** (`src/data/cutscenes_16d1c8.c`, from `0816D1C8`) | `PlayCutscene` | high | done |
| `081725A8` (`gTerrainTypes`) | 0x4 | 51 `struct terrain_type` {u8 modeValue[4]; u8 heights[4][8]} (the rest of the old blob is the UI text, see `081725C4`). **Converted** (`src/data/terrain_1725a8.c`) | `sub_8025228` | high | done |
| `081725AC` (`gTerrainHeights0`) | 0x8 | name for heights[0] of terrain type 0 (an `asm` `.set` alias into gTerrainTypes). **Converted** (`src/data/terrain_1725a8.c`) | `GetTerrainHeights`, `GetSolidTerrainHeights` | medium | done |
| `081725B4` (`gTerrainHeights1`) | 0x8 | name for heights[1] of terrain type 0 (alias). **Converted** (`src/data/terrain_1725a8.c`) | `GetSolidTerrainHeights` | medium | done |
| `081725BC` (`gTerrainHeights2`) | 0x8 | name for heights[2] of terrain type 0 (alias). **Converted** (`src/data/terrain_1725a8.c`) | `GetSolidTerrainHeights` | medium | done |
| `081725C4` (`gTerrainHeights3`) | 0x261C | heights[3] of terrain type 0 (alias); the old label ran on through the terrain table and then the game's UI text: 367 strings and the six 70-entry language tables `gUiTextEnglish`...`gUiTextDutch` (`src/data/ui_text_172cd4.c`). **Converted** (`src/data/terrain_1725a8.c, src/data/ui_text_172cd4.c`) | `GetSolidTerrainHeights` | medium | done |
| `08174BE0` | 0x8C | table of u32 (`u32` x 35). **Converted** (`src/data/hud_fonts_174be0.c`) | `InitHud`, `ConfigureHudParts` | high | done |
| `08174C6C` | 0x118 | table of struct hud_pos. **Converted** (`src/data/hud_fonts_174be0.c`) | `InitHud`, `ConfigureHudParts`, `UpdateHudBoss` +2 | high | done |
| `08174D84` | 0x50 | HUD font A's characters in glyph order (a string, Latin-1). **Converted** (`src/data/hud_fonts_174be0.c`) | `InitSmallFont` | medium | done |
| `08174DD4` | 0x3B4 | table (element layout: see consumers). **Converted** (`src/data/hud_fonts_174be0.c`) | `InitSmallFont` | medium | done |
| `08175188` | 0x4C | HUD font B's characters in glyph order. **Converted** (`src/data/hud_fonts_174be0.c`) | `InitLargeFont` | medium | done |
| `081751D4` | 0x384 | table (element layout: see consumers). **Converted** (`src/data/hud_fonts_174be0.c`) | `InitLargeFont` | medium | done |
| `08175558` | 0xC | first 0xC bytes of category_descriptor[0] (label splits the struct array) (`struct category_descriptor` x 7) | `InitActorCategory`, `SetupActorVramPool`, `CountCategoryCrates` +1 | high | easy |
| `08175564` | 0x20 | category_descriptor[0] bytes 0x0C-0x2B (part of gActorCategories[7]) | `InitActorCategory` | high | easy |
| `08175584` | 0x140 | category_descriptor[0] +0x2C .. [6] end (part of gActorCategories[7]) | `InitActorCategory` | high | easy |
| `081756C4` | 0x9C | category_vtable[3]: 3 x 13 Thumb function pointers (slots 7/8 of type 0 hold junk values) | `SelectActorCategory` | high | easy |
| `08175760` | 0x3800 | palette-cycle frames: 32 x 0x1C0-byte BGR555 blocks (BG palettes 0-13). **Converted** (`src/data/palette_cycle_175760.c`) | `UpdateActorPaletteCycle` | high | done |
| `08178F60` | 0x10 | palette-cycle cursor starts `s32[4]`. **Converted** | `SetActorPaletteCycle` | high | done |
| `08178F70` | 0x10 | palette-cycle cursor bounds `s32[4]`. **Converted** | `SetActorPaletteCycle` | high | done |
| `08178F80` | 0x1738 | categories 0-2 family data: OBJ palette, keyframe tables (table_A), anim table gCategoryFamily0AnimTable (41 x 0x28) and table_B arrays. **Converted** (`src/data/anim_family_178f80.c`, docs/data.md "Category families") | `RunCompanyLogos` | high | done |
| `0817A6B8` | 0x70 | function-pointer / pointer-to-member table (14 code pointers) | `UpdatePolarPlayer`, `RunPolarPlayerState` | high | easy |
| `0817A728` | 0x20 | 16-colour palette. **Converted** (`src/data/actor_tables_17a728.c`) | `HurtPolarPlayer`, `PolarPlayerStateShocked` | high | done |
| `0817A748` | 0x20 | 16-colour palette. **Converted** | `PolarPlayerStateShocked` | high | done |
| `0817A768` | 0xC | `struct anim_box`. **Converted** | `UpdatePolarNitroCrate` | medium | done |
| `0817A774` | 0xC | `struct anim_box`. **Converted** | `UpdatePolarElectricFence` | medium | done |
| `0817A780` | 0xC | `struct anim_box`. **Converted** | `UpdatePolarElectricFence` | medium | done |
| `0817A78C` | 0xC | `struct anim_box`. **Converted** | `UpdatePolarElectricFence` | medium | done |
| `0817A798` | 0x20 | 16-colour palette (gauge tier 1). **Converted** | `RefreshPolarAkuAku` | high | done |
| `0817A7B8` | 0x20 | 16-colour palette. **Converted** | `UpdatePolarAkuAku` | high | done |
| `0817A7D8` | 0x20 | 16-colour palette. **Converted** | `UpdatePolarAkuAku` | high | done |
| `0817A7F8` | 0x48 | 6 x `{s32 value, s32 threshold, s32 threshold}`. **Converted** | `YetiStateChase`, `YetiStateCharge` | medium | done |
| `0817A840` | 0x10 | function-pointer / pointer-to-member table (4 code pointers) (`void (*)(void)` x 4) | `UpdateYeti` | high | easy |
| `0817A850` | 0x30 | 4 `struct anim_frame_record` (the CreateYeti singleton's keyframes). **Converted** (`src/data/anim_frames_17a850.c`) | `CreateYeti` | medium | done |
| `0817A880` | 0x1EC | table_B: 123 absolute pointers into compressed frame set B (0x080DA1D8..) | `CreateYeti` | high | easy |
| `0817AA6C` | 0x20 | 16-colour gradient palette. **Converted** (`src/data/anim_family_17aa6c.c`) | `UpdateYetiPalette` | high | done |
| `0817AA8C` | 0xC | `struct anim_box`. **Converted** | `IsTouchingYeti` | medium | done |
| `0817AA98` | 0x1728 | categories 3-6 family data: a box, 2 OBJ palettes, table_A, anim table gCategoryFamily1AnimTable (47 x 0x28), table_B arrays. **Converted** (`src/data/anim_family_17aa6c.c`) | `UpdateYeti` | high | done |
| `0817C1C0` | 0x40 | function-pointer / pointer-to-member table (8 code pointers) | `UpdateJetpackPlayer`, `RunJetpackPlayerState` | high | easy |
| `0817C200` | 0x60 | 3-frame 16-colour palette strip. **Converted** (`src/data/palette_strip_17c200.c`) | `AnimateJetpackPlayerPalette` | high | done |
| `0817C260` | 0x20 | function-pointer / pointer-to-member table (4 code pointers) | `UpdateJetpackPlane`, `RunJetpackPlaneState` | high | easy |
| `0817C280` | 0x38 | function-pointer / pointer-to-member table (7 code pointers) | `UpdateJetpackBomber`, `RunJetpackBomberState` | high | easy |
| `0817C2B8` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `UpdateAirshipFireball`, `RunAirshipFireballState` | high | easy |
| `0817C2D0` | 0xA8 | 6 `struct weapon_kind` (7 words). **Converted** (`src/data/weapon_kind_17c2d0.c`) | `SpawnAirship` | high | done |
| `0817C378` | 0x60 | 3-frame 16-colour palette strip. **Converted** | `SpawnAirship`, `AnimateAirshipPalette` | high | done |
| `0817C3D8` | 0xC | `struct anim_box`. **Converted** | `AirshipStateExplode`, `SteerAirship`, `IsTouchingAirship` | high | done |
| `0817C3E4` | 0x18 | 2 `struct anim_frame_record`. **Converted** | `CreateAirship` | high | done |
| `0817C3FC` | 0x18 | function-pointer / pointer-to-member table (6 code pointers) (`void*` x 6) | `UpdateAirship` | high | easy |
| `0817C414` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `RunJetpackBalloonState` | high | easy |
| `0817C42C` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `UpdateJetpackBalloonCrate`, `RunJetpackBalloonCrateState` | high | easy |
| `0817C444` | 0xC | `struct anim_box`. **Converted** (`src/data/actor_box_17c444.c`) | `UpdateJetpackRocket` | medium | done |
| `0817C450` | 0x10 | function-pointer / pointer-to-member table (2 code pointers) | `UpdateHovercraftFireball`, `RunHovercraftFireballState` | high | easy |
| `0817C460` | 0x50 | 2 `struct singleton_kind` (10 words). **Converted** (`src/data/singleton_kind_17c460.c`) | `SpawnHovercraft` | high | done |
| `0817C4B0` | 0xC | `struct anim_box`. **Converted** | `HovercraftStateCloseIn` | high | done |
| `0817C4BC` | 0xC | 1 `struct anim_frame_record`. **Converted** | `CreateHovercraft` | medium | done |
| `0817C4C8` | 0x18 | function-pointer / pointer-to-member table (6 code pointers) (`void*` x 6) | `RunHovercraftState` | high | easy |
| `0817C4E0` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `UpdateHovercraftCannon`, `RunHovercraftCannonState` | high | easy |
| `0817C4F8` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `UpdateHovercraftLauncher`, `RunHovercraftLauncherState` | high | easy |
| `0817C510` | 0x2 | the text ">". **Converted** (`src/data/hud_palettes_17c510.c`) | `DrawContinuePrompt` | medium | done |
| `0817C512` | 0x20 | 16 palette halfwords. **Converted** | `InitContinuePromptGraphics` | high | done |
| `0817C532` | 0x20 | 16 palette halfwords. **Converted** | `InitContinuePromptGraphics` | high | done |
| `0817C552` | 0x20 | 16 halfwords (mostly 0xFFFF). **Converted** | `InitContinuePromptGraphics` | high | done |
| `0817C572` | 0x22 | 16 halfwords + the 2-byte zero pad. **Converted** | `InitContinuePromptGraphics` | high | done |
| `0817C594` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `InitContinuePrompt` | medium | easy |
| `0817C5A8` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `InitContinuePrompt` | medium | easy |
| `0817C5BC` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `InitContinuePrompt` | medium | easy |
| `0817C5D0` | 0x96C | the credits: text lines with opcodes 1 n (picture n), 2 (small font), 3 (large font), zero-terminated. **Converted** (`src/data/credits_17c5d0.c`) | `InitCredits` | high | done |
| `0817CF3C` | 0x4 | an empty string (4 bytes). **Converted** (`src/data/credits_17c5d0.c`) | `UpdateCreditsText` | high | done |
| `0817CF40` | 0x64 | table of struct popup_glyph_src; 10 word(s) look like ROM pointers | `LoadCreditsLogos` | high | easy |
| `0817CFA4` | 0x50 | table (element layout: see consumers); 9 word(s) look like ROM pointers | `RunTitleScreen`, `ResetTitleLogoPieces` | medium | easy |
| `0817CFF4` | 0x40 | 8 {x, y} OAM piece offsets. **Converted** (`src/data/level_gfx_17cff4.c`) | `DrawTitleLogoPieces` | high | done |
| `0817D034` | 0x20 | table (element layout: see consumers); 1 word(s) look like ROM pointers. **Converted** (`src/data/level_gfx_17cff4.c`) | `InitTitleScreen` | medium | done |
| `0817D054` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/level_gfx_17cff4.c`) | `InitTitleScreen` | high | done |
| `0817D074` | 0x70 | 16-colour palette + 4 OBJ sprite `struct bg_package`s (`gTitleBandicootObj`/`0A8`/`0BC`/`0D0`, gTitleObjPackages's). **Converted** (`src/data/level_gfx_17cff4.c`) | `InitTitleScreen` | medium | done |
| `0817D0E4` | 0x5B4 | `struct bg_package` (BG2) + 9 motion sequences of `struct delta_record` (`gTitleLogoPieceMotion0`...), the seeds' targets. **Converted** (`src/data/level_gfx_17cff4.c`) | `LoadTitleScreenBg` | high | done |
| `0817D698` | 0x28 | an animation record (actor_anim.h's `struct anim_table_record`, family A record 0's arrays). **Converted** (`src/data/level_gfx_17cff4.c`) | `RunCompanyLogos` | medium | done |
| `0817D6C0` | 0xA8 | table (element layout: see consumers); 20 word(s) look like ROM pointers | `InitVvLogoPieces` | medium | easy |
| `0817D768` | 0x14 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `LoadVvLogoGraphics` | medium | easy |
| `0817D77C` | 0x14 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `LoadVvLogoGraphics` | medium | easy |
| `0817D790` | 0x14 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `LoadVvLogoGraphics` | medium | easy |
| `0817D7A4` | 0xF70 | `struct bg_package` (BG2) + 20 motion sequences of `struct delta_record` + the 6 language names (`gLanguageNameEnglish`...). **Converted** (`src/data/countdown_17d7a4.c`) | `LoadUniversalLogoBg` | medium | done |
| `0817E714` | 0x18 | pointer table (6 data pointers) (`void*` x 6) | `DrawLanguageSelect` | high | easy |
| `0817E72C` | 0x20 | table of u16; 1 word(s) look like ROM pointers (`u16` x 16). **Converted** (`src/data/palettes_17e72c.c`) | `InitLanguageSelectGraphics` | high | done |
| `0817E74C` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16). **Converted** (`src/data/palettes_17e72c.c`) | `InitLanguageSelectGraphics` | high | done |
| `0817E76C` | 0x20 | table of u16 (`u16` x 16). **Converted** (`src/data/palettes_17e72c.c`) | `InitLanguageSelectGraphics` | high | done |
| `0817E78C` | 0x326E74 | composite: level BG tile sets (raw tag-0x00 assets), per-room level data, sprite-bank tile pool | `InitLanguageSelectGraphics` | high | medium |
| `084A5600` | 0xB66B4 | composite: sprite-bank (animation) table (**converted**, C) + GAX2 sound-effect bank (**converted**, `gax_audio.py --sfx`) | `RunPauseMenu`, `InitLevelState` | high | medium |
| `085A4C5C` | 0x14 | the default song's GAX2_Song struct `{4, unknownc, info, unk_ptr, channel}` (the engine's default handler layout). **Built** by `tools/gax_audio.py` (`gax_default_layout.bin`) | `GAX2_estimate`, `GAX2_init` | high | done |
| `085A4C70` | 0x100 | u8[256] count-leading-zeros table (libgcc `__clz_tab` of `__divdi3`). **Converted** (`src/data/clz_tab_5a4c70.c`) | `__divdi3` | high | done |
| `085A4D70` | 0x100 | u8[256] count-leading-zeros table (the second copy, `__udivdi3`'s). **Converted** (`src/data/clz_tab_5a4c70.c`) | `__udivdi3` | high | done |
| `085A5519` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `085A60FF` | 0x4D | 1 B pad (gbagfx's padding of the `.lz` stream before it, now built) + GAX2 version string "GAX Sound Engine 2.01D (Sep 28 2001) (c) Shin'en Multimedia. Code: B.Wodok". **Converted** (`gGaxVersionString`, `src/data/gax_tables_5a6100.c`) | `GAX2_init (via gGaxVersionStringPtr)` | high | done |
| `085A614C` | 0x4 | pointer to the GAX2 version string. **Converted** (`gGaxVersionStringPtr = gGaxVersionString`) | `GAX2_init` | high | done |
| `085A6150` | 0x60 | 12 `struct RateEntry` `{rate in Hz, timer reload}`. **Converted** | `GaxFindMixRate`, `GAX2_estimate`, `GAX2_init` | high | done |
| `085A61B0` | 0xC | GAX2 error string "GAX2_NEW". **Converted** | `GAX2_new` | medium | done |
| `085A61BC` | 0x14 | GAX2 error string "PARAMS ARG IS NULL". **Converted** | `GAX2_new` | medium | done |
| `085A61D0` | 0xC | GAX2 error/tag string "GAX2_INIT". **Converted** | `GAX2_init`, `GAX2_jingle` | high | done |
| `085A61DC` | 0x10 | GAX2 error/tag string "OUT OF MEMORY". **Converted** | `GAX2_init`, `GAX2_jingle` | high | done |
| `085A61EC` | 0xC | GAX2 error/tag string "GAX2_JINGLE". **Converted** | `GAX2_jingle` | high | done |
| `085A61F8` | 0x1C | GAX2 error/tag string "GAX_NO_JINGLE FLAG IS SET". **Converted** | `GAX2_jingle` | high | done |
| `085A6214` | 0x8 | GAX2 error/tag string "GAX_IRQ". **Converted** | `GAX_irq` | high | done |
| `085A621C` | 0xAC | GAX2 error string "GAX_PLAY HAS NOT FINISHED BEFORE GAX_IRQ. ...", then the halt banner "GAX ENGINE V2.01D Sep 28 2001\n\nEXCEPTION. PROGRAM HALT." (`gGaxHaltBanner`, `+0x74`). **Converted** | `GAX_irq` | high | done |
| `085A62C8` | 0x4 | pointer to the halt banner. **Converted** (`gGaxHaltBannerPtr = gGaxHaltBanner`) | `GaxFatalError` | high | done |
| `085A62CC` | 0x10 | GAX2 halt-screen string "FUNCTION NAME:". **Converted** | `GaxFatalError` | medium | done |
| `085A62DC` | 0x3BD0 | GAX2 u32 note period table (`u32` x 3828; its 0x08xxxxxx values are a smooth ramp, not pointers). **Converted** | `GaxChannelMix` | high | done |
| `085A9EAC` | 0x4C | s8[64] vibrato sine wave, then the SDK's "EEPROM_V122" id string (`gEepromLibraryVersion`, `+0x40`). **Converted** (`gax_tables_5a6100.c`, `eeprom_5a9eec.c`) | `GaxChannelTickVibrato` | high | done |
| `085A9EF8` | 0xC | `struct EepromConfig` of the 4 Kbit chip. **Converted** (`src/data/eeprom_5a9eec.c`) | `EEPROMConfigure` | high | done |
| `085A9F04` | 0xC | `struct EepromConfig` of the 64 Kbit chip. **Converted** | `EEPROMConfigure` | high | done |
| `085A9F10` | 0x260 | EEPROM write timeout `u16[3]` + pad, then the library's 22 address constants (`gEepromLibraryAddresses`, no reader), then the palette of cutscene picture 00. **Converted** (`eeprom_5a9eec.c`, `cutscene_pictures_5a9f70.c`) | `EEPROMWrite` | high | done |
| `085ADBD1` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 01), bit 15 set in many entries. **Converted** (`gCutscenePicture01`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085B34E1` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 02), bit 15 set in many entries. **Converted** (`gCutscenePicture02`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085B82BC` | 0x200 | 256-colour palette of the next Mode 4 bitmap (cutscene picture 03), bit 15 set in many entries. **Converted** (`gCutscenePicture03`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085BC6E8` | 0x200 | 256-colour palette of the next Mode 4 bitmap (cutscene picture 04), bit 15 set in many entries. **Converted** (`gCutscenePicture04`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085C1936` | 0x202 | 2 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 05), bit 15 set in many entries. **Converted** (`gCutscenePicture05`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085C674D` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 06), bit 15 set in many entries. **Converted** (`gCutscenePicture06`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085CB15D` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 07), bit 15 set in many entries. **Converted** (`gCutscenePicture07`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085CFECA` | 0x202 | 2 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 08), bit 15 set in many entries. **Converted** (`gCutscenePicture08`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085D6531` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 09), bit 15 set in many entries. **Converted** (`gCutscenePicture09`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085DB765` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 10), bit 15 set in many entries. **Converted** (`gCutscenePicture10`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085DFABC` | 0x200 | 256-colour palette of the next Mode 4 bitmap (cutscene picture 11), bit 15 set in many entries. **Converted** (`gCutscenePicture11`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085E550B` | 0x201 | 1 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 12), bit 15 set in many entries. **Converted** (`gCutscenePicture12`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085EAC44` | 0x200 | 256-colour palette of the next Mode 4 bitmap (cutscene picture 13), bit 15 set in many entries. **Converted** (`gCutscenePicture13`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085F0367` | 0x201 | 1 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 14), bit 15 set in many entries. **Converted** (`gCutscenePicture14`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085F4CC1` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 15), bit 15 set in many entries. **Converted** (`gCutscenePicture15`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085FA1D2` | 0x202 | 2 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 16), bit 15 set in many entries. **Converted** (`gCutscenePicture16`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `085FF779` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 17), bit 15 set in many entries. **Converted** (`gCutscenePicture17`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `086039B2` | 0x202 | 2 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 18), bit 15 set in many entries. **Converted** (`gCutscenePicture18`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `08608A50` | 0x200 | 256-colour palette of the next Mode 4 bitmap (cutscene picture 19), bit 15 set in many entries. **Converted** (`gCutscenePicture19`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `0860D577` | 0x201 | 1 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 20), bit 15 set in many entries. **Converted** (`gCutscenePicture20`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `08611BD9` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 21), bit 15 set in many entries. **Converted** (`gCutscenePicture21`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `08616360` | 0x200 | 256-colour palette of the next Mode 4 bitmap (cutscene picture 22), bit 15 set in many entries. **Converted** (`gCutscenePicture22`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `08619F51` | 0x203 | 3 B zero pad (alignment after the previous bitmap) + 256-colour palette of the next Mode 4 bitmap (cutscene picture 23), bit 15 set in many entries. **Converted** (`gCutscenePicture23`, `src/data/cutscene_pictures_5a9f70.c`) | `gCutscenes (slide packages)` | high | done |
| `0861BF2E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0861C182` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0861C309` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0861E5F6` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862556B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08628C4F` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862A957` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862C2C5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862C4AD` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862CC3B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862CE6E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862D0CB` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862E3AF` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0862FFF3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086308EF` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08630E19` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08631155` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08631372` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086313AD` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08631557` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086315BB` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863183A` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863199E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086324B3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863281E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08632BC2` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086334C1` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08636EF1` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08637603` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086377BD` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08637A6E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086382C6` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08639459` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863A60A` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863AD8D` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863B666` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863BDD2` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863CF95` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863D333` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0863D4BF` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08640EF5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08644A86` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08649417` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0864D7E5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0864F82F` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08651CF7` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08655175` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0865BAEB` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0865E05A` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08661F46` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086652B6` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08665935` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0867045E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0867829B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0867EDF3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086834F5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08689F72` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `0868D311` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `08693B8E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086A8349` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086AF12B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086BACB3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086C127C` | 0x18A30 | raw (unpacked) level asset for room 0x0825E7DC: u16 chunk offset table + chunk token streams (custom RLE/delta, DecodeLayerChunk/DecodeCollisionChunk) | `LoadRoom`, `SetCollisionSource`, `SetBgStreamerSource` +2 | high | **converted** |
| `086E044B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086E2212` | 0x2 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086E3541` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086EA0C9` | 0x3 | padding (zero, aligns the next LZ77 blob to 4). **Built**: gbagfx's zero padding of the preceding `.lz` stream | - | high | done |
| `086ECCD2` | 0xF6F1A | 6 raw (unpacked) level assets, same format as 0x086C127C | `LoadRoom`, `SetCollisionSource`, `SetBgStreamerSource` +2 | high | **converted** (the 2 B pad is gbagfx's padding) |
| `087E3BEC` | 0x58 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateEntity`, `InitEntity`, `DestroyEntity` +2 | high | easy |
| `087E3C44` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateSpriteObj`, `InitSpriteObj` | high | easy |
| `087E3CAC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyUiSpriteObj`, `InitUiSpriteObj` | high | easy |
| `087E3D14` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateMovingSprite`, `DestroyMovingSprite`, `InitMovingSprite` | high | easy |
| `087E3D8C` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateGroundSprite`, `DestroyGroundSprite`, `InitGroundSprite` | high | easy |
| `087E3E04` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyPlayer`, `InitPlayer` | high | easy |
| `087E3E7C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCtrl`, `InitCtrl` | high | easy |
| `087E3EE4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyEnemyCtrl`, `CreateEnemyCtrl` | high | easy |
| `087E3F4C` | 0x58 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePeriodicSpawner` | high | easy |
| `087E3FA4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyKnockedEnemyCtrl`, `CreateKnockedEnemyCtrl` | high | easy |
| `087E400C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyEffectCtrl`, `InitEffectCtrl` | high | easy |
| `087E4074` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateCrate`, `DestroyCrate`, `InitCrate` | high | easy |
| `087E40DC` | 0x70 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateExtraLife`, `DestroyExtraLife`, `InitExtraLife` | high | easy |
| `087E414C` | 0x70 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateWumpa`, `DestroyWumpa`, `InitWumpa` | high | easy |
| `087E41BC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateStopwatch`, `DestroyStopwatch`, `InitStopwatch` | high | easy |
| `087E4224` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyActionCtrl`, `InitActionCtrl` | high | easy |
| `087E428C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyPlayerCtrl`, `InitPlayerCtrl` | high | easy |
| `087E42F4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyInputCtrl`, `CreateInputCtrl` | high | easy |
| `087E435C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8017A78`, `sub_8017A8C` | high | easy |
| `087E43C4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyChaserCtrl`, `CreateChaserCtrl` | high | easy |
| `087E442C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyStompedHopPadCtrl`, `CreateStompedHopPadCtrl` | high | easy |
| `087E4494` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateOneShotAnimCtrl`, `DestroyOneShotAnimCtrl` | high | easy |
| `087E44FC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8018948`, `sub_8018960` | high | easy |
| `087E4564` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyTiny`, `CreateTiny` | high | easy |
| `087E45CC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCortexBossGemCtrl`, `CreateCortexBossGemCtrl` | high | easy |
| `087E4634` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCortexBossPlatformMover`, `CreateCortexBossPlatformMover` | high | easy |
| `087E469C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCortexShotCtrl`, `CreateCortexShotCtrl` | high | easy |
| `087E4704` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCortexTargetCtrl`, `CreateCortexTargetCtrl` | high | easy |
| `087E476C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCortexCannonCtrl`, `CreateCortexCannonCtrl` | high | easy |
| `087E47D4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCortexBoss`, `CreateCortexBoss` | high | easy |
| `087E483C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateDingodileSharkCtrl`, `DestroyDingodileSharkCtrl` | high | easy |
| `087E48A4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `SpawnDingodileStalactite`, `DestroyDingodileProjectileCtrl`, `CreateDingodileProjectileCtrl` | high | easy |
| `087E490C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyDingodileShieldCtrl`, `CreateDingodileShieldCtrl` | high | easy |
| `087E4974` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyDingodile`, `CreateDingodile` | high | easy |
| `087E49DC` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePlatform`, `DestroyPlatform`, `InitPlatform` | high | easy |
| `087E4A54` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyPlatformMover`, `CreatePlatformMover` | high | easy |
| `087E4ABC` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyCameraLead`, `CreateCameraLead` | high | easy |
| `087E4B34` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `SpawnLaunchPad`, `DestroyLaunchPad`, `InitLaunchPad` | high | easy |
| `087E4BAC` | 0x30 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyLevelSelectEntry`, `CreateLevelSelectEntry` | high | easy |
| `087E4BDC` (`gBgStreamerVtable`) | 0x10 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyBgStreamer`, `InitBgStreamer` | high | easy |
| `087E4BEC` (`gBgLayerBaseVtable`) | 0x28 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyBgLayerBase`, `InitBgLayerBase` | high | easy |
| `087E4C14` (`gBgLayerVtable`) | 0x50 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitBgLayer`, `DestroyBgLayer`, `DestroyPooledBgLayer` | high | easy |
| `087E4C64` (`gPooledBgLayerVtable`) | 0x50 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyPooledBgLayer`, `InitPooledBgLayer` | high | easy |
| `087E4CB4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802710C`, `InitHudPart` | high | easy |
| `087E4D1C` | 0x48 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitLargeFont`, `DestroyLargeFont` | high | easy |
| `087E4D64` | 0x48 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitSmallFont`, `DestroySmallFont` | high | easy |
| `087E4DAC` | 0x48 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitSmallFont`, `InitLargeFont`, `DestroyFont` +3 | high | easy |
| `087E4DF4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitActorPart`, `DestroyActor`, `DestroyPolarPlayer` +41 | high | easy |
| `087E4E14` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor` | high | easy |
| `087E4E34` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarCheckpointText` | high | easy |
| `087E4E54` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `ConstructActorPart`, `DestroyPolarPlayer` | high | easy |
| `087E4E74` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyPolarCollectedWumpa`, `CreatePolarCollectedWumpa` | high | easy |
| `087E4E94` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `CreatePolarWumpa` | high | easy |
| `087E4EB4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `CreatePolarTimeCrate` | high | easy |
| `087E4ED4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `CreatePolarQuestionCrate` | high | easy |
| `087E4EF4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `CreatePolarAkuAkuCrate` | high | easy |
| `087E4F14` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `CreatePolarNitroCrate` | high | easy |
| `087E4F34` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `CreatePolarLifeCrate` | high | easy |
| `087E4F54` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `sub_802CC54` | high | easy |
| `087E4F74` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateActor`, `CreatePolarBasicCrate` | high | easy |
| `087E4F94` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitPolarCrate` | high | easy |
| `087E4FB4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarElectricFence` | high | easy |
| `087E4FD4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802CE38` | high | easy |
| `087E4FF4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarLauncher` | high | easy |
| `087E5014` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarPenguin` | high | easy |
| `087E5034` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarIcicle` | high | easy |
| `087E5054` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarAkuAku` | high | easy |
| `087E5074` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarGoal` | high | easy |
| `087E5094` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarBoostPad` | high | easy |
| `087E50B4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreatePolarCheckpointCrate` | high | easy |
| `087E50D4` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackCheckpointText` | high | easy |
| `087E510C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackExplosion` | high | easy |
| `087E5144` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitJetpackPlayer`, `DestroyJetpackPlayer` | high | easy |
| `087E517C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackShot` | high | easy |
| `087E51B4` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackPlane` | high | easy |
| `087E51EC` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackBomber` | high | easy |
| `087E5224` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackCannonball` | high | easy |
| `087E525C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateAirshipFireball` | high | easy |
| `087E5294` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackBalloon` | high | easy |
| `087E52CC` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackHealthCrate` | high | easy |
| `087E530C` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackTimeCrate` | high | easy |
| `087E534C` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackQuestionCrate` | high | easy |
| `087E538C` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackTimeCrate`, `CreateJetpackHealthCrate`, `CreateJetpackQuestionCrate` +1 | high | easy |
| `087E53CC` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackParachuteNitro` | high | easy |
| `087E5404` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackRocket` | high | easy |
| `087E543C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateJetpackRing` | high | easy |
| `087E5474` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `DestroyJetpackCollectedWumpa`, `CreateJetpackCollectedWumpa` | high | easy |
| `087E54AC` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateHovercraftFireball` | high | easy |
| `087E54E4` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateHovercraftCannon` | high | easy |
| `087E551C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateHovercraftLauncher` | high | easy |
| `087E5554` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateHovercraftSideGun` | high | easy |
| `087E558C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `CreateHovercraftCannonFlash` | high | easy |
| `087E55C4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitLogoActor`, `DestroyLogoActor` | high | easy |
| `087E55E4` | 0x1AA1C | IWRAM image (ARM code + initialised data, 0x9E8 B) copied to 0x03000000 by crt0, then 0xFF cartridge fill | `_0800012C` | high | **converted** (`asm/intr_main.s`, `src/iwram/`, ldscript `iwram`/`rom_fill`) |
