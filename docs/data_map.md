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
  the sprite-bank walker (`sub_80083A8`/`sub_80083B8`/`graphics_7634.c`),
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
| `0803B8B0`-`080B1444` | 482,196 | BG0 cell animation A (palette, 19x13 cells, 60 frames) | `sub_8029890`/`sub_80297C8` via category descriptors 0-2 | high | **converted** |
| `080B1444`-`080B2120` | 3,292 | category 0 `sub_effect_record` table (164 records) | `SelectActorCategory`, `sub_802A5xx` | high | **converted** |
| `080C0C36`-`080C2758` | 6,946 | 2 B pad + category 1/2 `sub_effect_record` tables | same | high | **converted** (the pad stays raw) |
| `080C2758`-`080FF1B0` | 248,408 | rotation strips A and B (overlapping sprite frames) | `GetAnimFrameData` via table_B `0817941C`/`0817A880` | high | hard |
| `080FF1B0`-`0813D934` | 255,876 | BG0 cell animation B (38x10 cells, 21 frames) | `sub_8029890` via category descriptors 3-6 | high | **converted** |
| `0813D934`-`0814174C` | 15,896 | category 3 BG1 picture + `sub_effect_record` table | `sub_802F7B0`, `SelectActorCategory` | high | **converted** |
| `08151AC2`-`0815A050` | 34,190 | category 4-6 BG1 pictures + `sub_effect_record` tables | same | high | **converted** (the pad stays raw) |
| `0815A050`-`08167AD4` | 55,940 | rotation strip C | `GetAnimFrameData` via table_B `0817BA44` | high | hard |
| `08167AD4`-`0817E78C` | 92,180 | 200 small/mid tables: gameplay, menus, HUD, actors, text (the built sfx table sits in between) | direct, see appendix | mostly high | easy (a few medium) |
| `0817E78C`-`0817E7AC` | 32 | `u16[16]` | `sub_8037388` | high | **done** (C) |
| `0817E7AC`-`0824B638` | 839,308 | level tile sets 1-3 (tag-0x00 raw 8bpp tiles) | `bg_scroll_layer_25fc8.c` via `bg_layer_desc.tileData` | high | **done** (grit) |
| `0824B638`-`08270F08` | 153,808 | per-room level data, 33 rooms | `level_layers.c`, `game_loop5.c`, `game_loop57.c` | high | hard (raw until decoded) |
| `08270F08`-`082B91D0` | 295,624 | level tile sets 4-5 | as tile sets 1-3 | high | **done** (grit) |
| `082B91D0`-`082BF120` | 24,400 | per-room level data, 8 rooms | as block 1 | high | hard (raw until decoded) |
| `082BF120`-`084A4660` | 1,987,904 | sprite tile pool for the 56 sprite banks | `graphics_7634.c`/`graphics_73dc.c` (`sub_80083A8` + frame offset) | high | **done** (grit) |
| `084A4660`-`084A5600` | 4,000 | 125 fixed 4bpp tiles | `sub_8004D74` pool, `sub_8006DF8` | high | **done** (grit) |
| `084A5600`-`084C0006` | 109,062 | sprite-bank table ("master asset table"): header, 56 banks, 2,429 frames | `sub_8004D74`, `sub_8022230`, every `**gUnknown_030012D0` user | high | **converted** (C) |
| `084C0006`-`0855BCB4` | 638,126 | second GAX2 data set: 88 instruments, 87 8-bit samples, sample table, FX handler | GAX2 engine (header prefix of the built audio points here) | high (format), medium (role) | medium |
| `085A4C5C`-`086ECCD2` | 28,979 | 111 labels between the built intro/tileset1 LZ77 blobs: GAX2 tables and strings, libgcc `__clz_tab` x2, EEPROM tables, 23 intro palettes, 67 alignment pads | direct / slide packages | high | easy |
| `086C127C`-`086D9CAC` | 100,912 | raw level asset (room `0825E7DC`) | `sub_80266BC` -> `sub_80254F8`/`sub_8024CF0` | high | medium |
| `086ECCD2`-`087E3BEC` | 1,011,482 | 2 B pad + 6 raw level assets | same | high | medium |
| `087E3BEC`-`087E55E4` | 6,648 | 93 gcc 2.x vtables | constructors (`sub_8009ED0`, ...) | high | easy |
| `087E55E4`-`087E5FCC` | 2,536 | IWRAM image (ARM code + data, copied by `crt0`) | `crt0.s` | high | medium |
| `087E5FCC`-`08800000` | 106,548 | `0xFF` fill (not counted as data) | - | high | - |

By category, as a share of the 8,038,172-byte data total:

| Category | Bytes | % of data | Effort |
|---|---:|---:|---|
| Sprite tile pool | 1,987,904 | 24.73% | medium |
| Level tile sets (5 tag-0 assets) | 1,134,932 | 14.12% | medium |
| Raw level assets (7) | 1,112,392 | 13.84% | medium (opaque `.bin`), hard (real decode) |
| BG0 cell animations (2) | 738,072 | 9.18% | medium |
| GAX2 second data set | 638,126 | 7.94% | medium |
| Rotation strips (3) | 304,348 | 3.79% | hard |
| Per-room level data | 178,208 | 2.22% | hard |
| 404 small/mid labels | 127,807 | 1.59% | easy (a few medium) |
| Sprite-bank table | 109,062 | 1.36% | **converted** |
| BG1 pictures (3) | 37,096 | 0.46% | medium |
| `sub_effect_record` tables (7) | 23,224 | 0.29% | easy |
| Other (125-tile pool, IWRAM image, pads, `u16[16]`) | 6,574 | 0.08% | easy/medium |

### Findings that change earlier notes

- **Testing the tag-0x00 theory (docs/graphics.md, "The two largest
  untyped ROM regions").** It is half right. `gStaticData_0817E78C` does
  hold tag-`0x00` assets: five of them, `{u32 size << 8}` plus raw bytes,
  in two chains where each asset ends exactly where the next begins.
  But they are **BG level tile sets** (8bpp, 64-byte tiles, fed to the
  VRAM tile-slot pool at `tiles + 4`), not sprites. The **sprite tiles
  have no tag header at all**. They are one raw 1.99 MB pool, addressed
  as `tileBase + (frame.packed & 0xFFFFFF)`, where `tileBase` is the
  second word of the `gStaticData_084A5600` header. `gStaticData_084A5600`
  itself is not graphics: it is the sprite-animation metadata plus an
  audio bank. That is why no LZ77/RL signature was ever found.
- **"The body is one dense 12-byte record array"** (docs/rom_map.md,
  `gStaticData_084A5600`) is only true of 56 records. Past them come the
  per-bank animation/frame/piece tables (to `0x084C0006`), and then
  638 KB of GAX2 audio.
- **"There is no separate sound-effect sample bank"** (docs/audio.md)
  needs rechecking. `0x084C0006`-`0x0855BCB4` has the exact GAX2
  instrument and sample-table layouts, with 87 samples and 88
  instruments, and the 36-byte `gax_header_prefix.bin` of the built
  audio block points into its tail (`0x0855BC78`/`0x0855BC98`).
- **Most of `graphics/tileset1/*.bin` are not graphics.** Files `27`-`60`
  (all but `21`-`26`) are LZ77-packed **level assets** (per-room chunk
  streams, `level_desc.asset` with `assetPacked = 1`). Files `23`-`26`
  are BG tile sets used by `bg_layer_desc.tileData`, and could become
  PNGs like `21`/`22`.
- **`gStaticData_08175558/64/84` are one array.** They are one
  `category_descriptor[7]`, cut into three labels because code takes
  `+0x0C` and `+0x2C` literals. Convert them together.

## The huge blobs, in ROM order

### `gStaticData_0803B8B0` (485,488 B): BG0 cell animation A + category-0 table

Nothing references this label directly. It is reached through the
category descriptors `gStaticData_08175558[0..2]` (`include/actor_anim.h`):
`family_shared_04 = 0x0803B8B0`, `family_shared_08 = 0x75B94`, and
`sub_effect_table = 0x080B1444` for category 0. `InitActorCategory`
passes the first two to `sub_8029890` (`src/graphics/actor_part95.c`).
That function reads a "cell record": a 256-colour palette (DMA'd whole to
`0x05000000` by `sub_802996C`), `s16` cols and rows at `+0x200`/`+0x202`,
then frames of `cols*rows*32` bytes of 4bpp tiles. When the category type
is 0, each frame also carries `((cols*rows)+7)/8*4` bytes of side data.
`sub_80297C8` DMAs one frame per tick to BG0 and hands the side data to
the `gUnknown_0300087C` callback. The frame count is `(size - 0x204) /
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

### `gStaticData_080C0C36` (527,126 B): sub-effect tables, rotation strips, cell animation B, BG1 picture

Every region is reached through the category descriptors or an
animation-table `table_B` array:

| Range | Size | Content | Evidence | Effort |
|---|---:|---|---|---|
| `080C0C36` | 2 | zero pad | | easy |
| `080C0C38` | 0xDB8 | category 1 `sub_effect_record[175]` + 0xC | descriptor 1 `+0x14`, count in record 0 | easy |
| `080C19F0` | 0xD68 | category 2 `sub_effect_record[171]` + 0xC | descriptor 2 `+0x14` | easy |
| `080C2758` | 0x17A80 | **rotation strip A**: overlapping `struct sprite_frame` windows `{8, 8, 0x30, 0}` + 2 KB | the 152 absolute pointers of `table_B` `0x0817941C` (anim record 0 of categories 0-2) all land here | hard |
| `080DA1D8` | 0x24FD8 | **rotation strip B**: 10x10-tile windows | the 123 pointers of `gStaticData_0817A880` (the `sub_802DFDC` singleton's `table_B`) | hard |
| `080FF1B0` | 0x3E784 | **BG0 cell animation B**: palette[256], cols=38, rows=10, **21 frames x 12,160 B** (no side data, type != 0) | descriptors 3-6 `+0x04`/`+0x08` = `{0x080FF1B0, 0x3E784}`; `0x204 + 21*12160` exact | medium |
| `0813D934` | 0x3498 | **BG1 picture** (category 3, descriptor `+0x0C`) | `sub_802F7B0` layout below; size exact | medium |
| `08140DCC` | 0x980 | category 3 `sub_effect_record[121]` + 0xC | descriptor 3 `+0x14`; ends exactly at the `0814174C` LZ77 sheet | easy |

**Status:** the two tables are converted (`src/data/sub_effect_0c0c38.c`),
and cell animation B, the category 3 picture and its table too
(`src/data/cell_anim_0ff1b0.c`). The pad at `080C0C36` and the two
rotation strips stay raw `.incbin`s, now under their own labels
`gStaticData_080C2758` and `gStaticData_080DA1D8` (see below for why
the strips stay raw).

The **BG1 picture** format, from `sub_802F7B0` (`actor_part45d.c`):
`u16 palette[256]`, `s16 cols @0x200`, `s16 rows @0x202`,
`u32 tiles @0x204`, `u16 map[cols*rows]` padded to 4 bytes (`((n+1)/2)*4`),
`tiles*32` bytes of 4bpp tiles, then `(n+1)/2` bytes of palette-bank
nibbles (one per map entry). All three pictures (`0813D934`, `08151AC4`,
`08155260`) end exactly at the next known structure. `gbagfx` can handle
the tiles, and a tiny packer can handle the header, map and nibbles.

The **rotation strips** are the "sliding window" frames described in
docs/graphics.md: consecutive `table_B` entries point a few hundred bytes
apart into one byte stream, and each reads a whole frame. The last
windows run past each strip's nominal end: strip A's last frame reads
0x5BC bytes into strip B, and strip B's reads 0x700 bytes into cell
animation B. So no strip can be cut into independent frame files.
Converting them needs a tool that stores the stream once and emits the
frames as views of it. That is the reason for the *hard* rating. A
plain `.bin` split is trivial, but it isn't a real decode.

A second look (while converting the rest of these blobs) found no clean
image form either. Consecutive windows start 106-2,818 bytes apart, at
offsets that are rarely a multiple of 32, so the stream has no
fixed tile grid. Each window's 4-byte `{8, 8, 0x30, 0}` / `{10, 10,
0x30, 0}` header sits inside the previous window's pixel bytes. Drawn
as plain 8x8 4bpp tiles with the category palette, the windows come out
as scrambled fragments of the mask, not as rotation frames. So the
`0x30` frames are probably consumed some other way than plain tiles
(`LoadSpriteFrameTiles`' `gUnknown_03000870` override hook is the first
suspect), and no PNG can hold them until that is understood. They stay
raw.

### `gStaticData_08151AC2` (90,130 B): categories 4-6

The category data continued after the second LZ77 sheet, same formats as
above:

| Range | Size | Content | Effort |
|---|---:|---|---|
| `08151AC2` | 2 | zero pad | easy |
| `08151AC4` | 0x2598 | BG1 picture, category 4 (38x16, 237 tiles) | medium |
| `0815405C` | 0x1204 | category 4 `sub_effect_record[230]` + 0xC | easy |
| `08155260` | 0x36B8 | BG1 picture shared by categories 5 and 6 (38x16, 374 tiles) | medium |
| `08158918` | 0x1718 | category 5 `sub_effect_record[295]` + 0xC | easy |
| `0815A030` | 0x20 | category 6 `sub_effect_record[1]` + 0xC | easy |
| `0815A050` | 0xDA84 | **rotation strip C**, `table_B` `0x0817BA44` (anim record 0 of categories 3-6, 80 pointers). The highest window runs 0x4DC bytes past the blob's end | hard |

**Status:** the pictures and tables (`08151AC4`-`0815A050`) are converted
(`src/data/bg_picture_151ac4.c`). The pad at `08151AC2` stays raw, and so
does rotation strip C, now labeled `gStaticData_0815A050`.

### `gStaticData_0817E78C` (3,305,076 B): level tile sets, room data, sprite tile pool

The only direct reference is `sub_8037388` reading the first 0x20 bytes
as a `u16[16]`, like its three siblings just before it. Everything else
is reached through two pointer graphs.

**1. Level data.** `gStaticData_0816CD80` holds 41 room records of 0x14
bytes: `{u16 (*palette)[256]; struct level_desc *desc; s32 kind; ...;
u16 catIndex @0x10}`. The same record is `struct level_load_args` in
`level_layers.c` (first two fields), `gl_widget_kind` in `game_loop56.c`
(`kind`), and `MedalListItem` in `game_loop18.c` (`linkedObj` is the
`desc`, and `linkedObj->0x1C` is the descriptor's object list). A room
record is the `self->widget` that `sub_8023A1C` hands to `sub_80266BC`.
Walking them:

- `struct level_desc` (0x24): `layerData[3]`, `layer0Data`, `tileData`
  (all `bg_layer_desc *`), `asset`, `u8 assetPacked`, and the two object
  lists passed to `sub_80255D4`. The 41 descriptors sit at `0824C400`
  ... `08270BCC` (33) and `082B9ED0` ... `082BEADC` (8).
- `struct bg_layer_desc` (0x20, `bg_scroll_layer_25fc8.c`): `u16 *chunkGrid`,
  `u32 assetOffset` (the streamers use `level asset + assetOffset` as
  their decode base, `game_loop5.c`/`game_loop57.c`), `tileData`,
  `scaleX`, `scaleY`, `cnt`, grid width/height in chunks
  (`+0x16`/`+0x18`), size in tiles (`+0x1A`/`+0x1C`).
- `tileData` is either one of the built LZ77 tile sets
  (`graphics/tileset1/21`-`26`), or one of **five tag-0x00 raw assets in
  this blob**. The pooled layer 0 reads those at `tiles + 4`, 64 bytes
  per tile, i.e. 8bpp (`tile_slot_pool.c`, `sub_8026618`).
- `asset` is either an LZ77 blob in `graphics/tileset1/27`-`60`
  (`assetPacked = 1`), or one of **seven raw level assets** in
  `gStaticData_086C127C`/`gStaticData_086ECCD2` (`assetPacked = 0`).

**Status: converted except the room data.** The blob is now three `src/data` objects
(docs/data.md, "Raw tile pools"): the sprite banks, the 125 fixed tiles
and the five tile sets are indexed PNGs built with grit
(`graphics/sprites/`, `graphics/level_tilesets/`, extracted by
`tools/tile_pools.py`), and `gStaticData_0817E78C` is a C `u16[16]`. The
two room-data blocks (`0824B638`, `082B91D0`) stay raw `.incbin` slices in
`data/data.s`, in their own sections. An undecoded `.bin` copy doesn't
count as converted data, so they get converted once their format (the
object lists especially) is decoded into typed C.

**2. Sprite banks.** The `gStaticData_084A5600` header's second word is
`0x082BF120`, and `sub_80083A8` returns it. `graphics_7634.c` and
`graphics_73dc.c` upload `sub_80083A8() + (frame.packed & 0xFFFFFF)`.

| Range | Size | Content | Evidence | Effort |
|---|---:|---|---|---|
| `0817E78C` | 0x20 | `u16[16]` | `sub_8037388` | easy |
| `0817E7AC` | 0x67B84 | level tile set 1: header `0x067B8000` (tag 0, 0x67B80 B), 6,638 8bpp tiles | 10 `bg_layer_desc.tileData` refs; ends exactly at the next asset | medium |
| `081E6330` | 0x1AAC4 | level tile set 2 (0x1AAC0 B) | 10 refs; chains | medium |
| `08200DF4` | 0x4A844 | level tile set 3 (0x4A840 B) | 5 refs; chains | medium |
| `0824B638` | 0x258D0 | **room data, 33 rooms**: 256-colour BG palettes, `level_desc`, `bg_layer_desc`, `u16` chunk grids, object spawn lists | the walk above types about 68% of the bytes (grids 82 KB, palettes 17 KB, descriptors 5.7 KB); most of the rest follows the object-list pointers | hard |
| `08270F08` | 0x28EC4 | level tile set 4 (0x28EC0 B) | 7 refs; chains | medium |
| `08299DCC` | 0x1F404 | level tile set 5 (0x1F400 B) | 9 refs; ends where room block 2 starts | medium |
| `082B91D0` | 0x5F50 | **room data, 8 rooms** (same shapes) | | hard |
| `082BF120` | 0x1E5540 | **sprite tile pool**: raw OBJ tiles, no header | see the next section: the 56 banks use disjoint, back-to-back tile ranges in bank order, and the last one ends at exactly `+0x1E5540` | medium |
| `084A4660` | 0xFA0 | 125 x 32-byte 4bpp tiles | header `+0x08`/`+0x0E` of `gStaticData_084A5600`; `sub_8004D74` builds the 125-slot tile-asset cache from it | easy |

**Conversion.** The sprite tile pool is plain tile data with known bank
boundaries. One 4bpp PNG per bank through `gbagfx` is a lossless round
trip (any byte string is valid 4bpp data; the pool is 61,994 whole
tiles), and it's viewable with a bank's palette. Banks drawn in 8bpp
(part flag 28) would look scrambled in a 4bpp view but still round-trip.
Doing this single step moves **24.7% of the data total**. The five tile
sets are 8bpp PNG + a generated 4-byte tag header: another 14.1%.
The room blocks are pointer-dense, and some pieces (the object lists)
are not typed yet: they are C or JSON work for later.

### `gStaticData_084A5600` (747,188 B): sprite-bank table + second GAX2 data set

This is the "master asset table" of docs/rom_map.md. Its structure,
from the matched readers (`sub_8004D74` in `settings_menu15.c`, `sub_80083A8`/
`sub_80083B8`/`sub_800815C`/`sub_8008734` in `actor_part4.c`-`actor_part6.c`,
`graphics_7634.c`, and the `**gUnknown_030012D0 + N` users):

```
0x084A5600 header (0x10):
    bank_record *banks      = 0x084A5610
    u8 *tileBase            = 0x082BF120   (sprite tile pool, in 0817E78C)
    u8 *tilePool            = 0x084A4660   (125 fixed tiles)
    u16 nbanks = 56, u16 npool = 125
0x084A5610 bank_record[56] (12 B): { anim_record *anims; frame_desc **frames; u16 unk; u16 nanims; }
    (every "**gUnknown_030012D0 + 0x27C"-style offset in src/ is 12*N: bank N)
per bank, back to back:
    anim_record[nanims] (0x1C): u16 *keyframes @0, two boxes @4/@0xC, u8 tileRecord @0x14 (sub_8006DF8 id),
                                u8 duration @0x15, u8 frameCount @0x16, u8 flags @0x17 (bit 1 = loop)
    u16 keyframes[]             (frame indices, frameCount per anim)
    frame_desc *frames[nframes]
    frame_desc[nframes]: piece_offset *offsets; u8 *ids; u32 count<<24 | tileOffset;
                         then 0-3 boxes {s16 x, y; u8 w, h; u16 0} and an optional {s16 x, y} anchor
    piece_offset[] (s16 x, s16 y), u8 ids[] (low nibble = OBJ shape/size index into 0816B2E0/0816B2EC)
```

A frame descriptor is **not** a fixed 0x18 bytes, as first noted here:
its size depends on the high nibble of its first piece byte (the
"layout type"), the same nibble `sub_8007C30`/`sub_8007CF8`/
`sub_80084C4`/`sub_8008518`/`sub_8008564`/`sub_80085B8` switch on to
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
| `084C0006` | 0x22 | 2 B pad + 0x20 B not yet modelled |
| `084C0028` | 0x3B00 | 88 instrument records (0xAC stride, the last one shorter) + an 8-byte `{0x101, 0x084C3A94}` header |
| `084C3B28` | 0x160 | instrument pointer table (88 entries) |
| `084C3C88` | 0x97D30 | 87 signed 8-bit PCM samples, byte-aligned (+3 B pad) |
| `0855B9B8` | 0x2C0 | sample table: 88 x `{u8 *data, u32 length}`, entry 0 empty |
| `0855BC78` | 0x3C | handler/song header with the GAX2 `init`/`unknown`/`play` code pointers `0x0803A105`/`0x0803A229`/`0x0803A159` |

The built `gax_audio_data.bin` starts with a 36-byte "unmodelled" prefix
that points at `0x0855BC78`/`0x0855BC98`, i.e. into this set. Most
likely these are the sound effects (docs/audio.md's `PlaySfx` notes
should be re-checked against it), but which engine call selects it is
not confirmed.

**Conversion.** The bank table is done (see above). The audio set is medium: samples to `.wav` (lossless for 8-bit
PCM) and instruments/tables through an extension of `tools/gax_audio.py`,
which already encodes these exact record types for the music bank.

### `gStaticData_086C127C` (100,912 B) and `gStaticData_086ECCD2` (1,011,482 B): raw level assets

Each is a `level_desc.asset` with `assetPacked = 0`: the uncompressed form
of what `graphics/tileset1/27`-`60` hold LZ77-packed. The asset is the
"chunk stream" pack read by the custom RLE/delta decoders `sub_8024960`
(visual layers) and `sub_8025334` (terrain cache, 16x8 `u16` chunks). It
starts with a `u16` offset table (offsets x 4). Each layer reads it at
`asset + bg_layer_desc.assetOffset`.

| Asset | Size | Room (`level_desc`) |
|---|---:|---|
| `086C127C` | 0x18A30 | `0825E7DC` |
| `086ECCD4` (after 2 B pad) | 0x1B484 | `082BDF98` |
| `08708158` | 0x2F568 | `08260768` |
| `087376C0` | 0x1B684 | `0825BCDC` |
| `08752D44` | 0x38064 | `0825A390` |
| `0878ADA8` | 0x31394 | `0824E104` |
| `087BC13C` | 0x27AB0 | `0825233C` (ends at the first vtable, `087E3BEC`) |

**Conversion.** The cheap step matches the existing practice for their
LZ77 siblings: an uncompressed `graphics/level/*.bin` per asset, *medium*
(13.8% of data). That is still an opaque passthrough, though. A real
decode (chunk streams to editable tilemaps/collision maps) is hard, and
should come later as one tool that covers all 41 rooms, the LZ77 ones
included.

### `gStaticData_087E55E4` (109,084 B): IWRAM image + fill

`crt0.s` DMA-copies `(gUnknown_030009E8 - IntrMain_Buffer) / 4` words
from here to `0x03000000` at boot. The first `0x9E8` bytes are the IWRAM
overlay: ARM-mode code (`E3A0C301`... = `mov ip, #0x4000000`, the interrupt
dispatcher) and IWRAM initialisers. The rest, from `0x087E5FCC`, is
`0xFF` fill, which the data report already excludes. The overlay is code,
so it should move to `asm/` (ARM, `.section .iwram` or a load-address
section) or become C compiled for IWRAM. Medium effort, 2.5 KB.

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
| palettes (16- or 256-colour BGR555) | 51 | 63,397 | 23 are the 256-colour palettes of the intro Mode-4 slides (`085ADBD1`...`08619F51`, each after 0-3 B of alignment pad; some entries have bit 15 set, so keep the raw bytes, don't round-trip through `gbagfx .pal`), plus `08175760` (32 palette-cycle frames x 0x1C0) and many 0x20 menu/HUD palettes |
| alignment padding | 67 | 133 | 1-3 zero bytes before a 4-aligned LZ77 blob, all between `085A5519` and `086EA0C9`. Emit as `.balign 4, 0` |
| other typed tables | 166 | 56,125 | `s32`/`u16`/struct tables with known consumers; notable ones below |

Notable mid-size tables:

- `0816C86C` (0x514): the level table, `u32 1` + 24 x 0x24 level records +
  36 medal item-list headers. It is declared under five different struct
  names across `src/` (`gl_level_entry`, `level_info`, `level_guard`,
  `MedalTableEntry`, `threshold_table_entry`). Unify them first.
- `0816CD80` (0x474): the room records (see `0817E78C`) plus medal
  items. It is the root of the level-data graph.
- `0816D1F4` (0x53B4): text/menu lists, `{items*, count}` headers, strings,
  and the intro slide packages (0x1C each) that point at the intro palettes.
- `08178F80` (0x1738) / `0817AA98` (0x1728): the two actor-category
  families: palettes, `table_A` keyframes, `anim_table_record[41]`/`[47]`
  (`081796CC`/`0817B2A4`, already declared), and the `table_B` arrays.
  `graphics/unknown/*/entities.json` already records all of it.
- `08175558`+`08175564`+`08175584`: one `category_descriptor[7]`, split over three labels.
- `081725C4` (0x261C): terrain shape records (36 B, heights 0-7, `0xFF`
  empty) for the collision streamer.
- `0817C5D0` (0x96C): popup/credits text opcode stream (`"\x03developed by\n\n\x01..."`).
- `0816AF10`: the CRC-16/CCITT table (poly `0x1021`) for link-cable packets.
- `0816A820`: `s16[256]` sine table.
- `085A4C70`/`085A4D70`: two copies of libgcc's `__clz_tab` (`u8[256]`).
  They could come from `libgcc.a` itself if the link kept its `.rodata`.
- `085A60FF`...`085A62CC`: GAX2 version string (`"GAX Sound Engine 2.01D
  (Sep 28 2001) (c) Shin'en Multimedia. Code: B.Wodok"`) and error strings,
  the `RateEntry` table, and `085A62DC`, the GAX2 `u32` period table
  (0x3BD0; its `0x08xxxxxx` values are a smooth ramp, not pointers).

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
5. **Raw level assets as `.bin`** (1.11 MB, **+13.8%**). This is the same
   treatment `graphics/tileset1/27`-`60` already get, so it's cheap. Flag
   it as a passthrough in the docs, since a real decoder comes later.
6. **Second GAX2 data set** (638 KB, **+7.9%**): extend `tools/gax_audio.py`.
7. **Small tables** (128 KB, +1.6%, about 400 labels). Do the vtables and
   function-pointer tables early, even though they are small. They are
   the only data that pins code addresses, so converting them to
   symbolic `.4byte sub_X`/C initialisers is what lets code shift without
   breaking the ROM. Then the typed tables, in consumer-file batches.
8. **Done (the sprite-bank table).** **Sprite-bank table** (109 KB) and **category family data**
   (`08178F80`/`0817AA98`, `sub_effect_record` tables): typed C with
   generators. The layouts are all known.
9. **Room data** (178 KB) and **rotation strips** (304 KB): last. They
   need the object-list types and an overlap-aware frame tool.

Steps 2-5 alone take data progress from 20.4% to about 83%.

### Conversion convention for small tables

**Recommendation: C `const` arrays in `src/data/*.c`, placed by
`ldscript.txt` in ROM order, with typed `.s` directives as the fallback**
for the cases below. C matches the standing "prefer structs" direction:
the structs already exist in `include/` and `src/`, so the table and its
consumers share one definition, and function pointers are just the
function names.

The experiment (done in the worktree, then reverted, so no ROM-producing
file is changed): `gStaticData_0816B2E0`/`0816B2EC` (the OBJ piece
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
| `0803B8B0` | 0x76870 | composite: BG0 streamed cell animation A + category-0 sub-effect table. **Converted** (`src/data/cell_anim_03b8b0.c`) | `sub_8029890`, `sub_80297C8`, `sub_802996C` +2 | high | done |
| `080C0C36` | 0x80B16 | composite: sub-effect tables, two rotation-strip sprite pools, BG0 cell animation B, BG1 picture. **Converted** except the 2-byte pad (still `gStaticData_080C0C36`) and rotation strips A/B (`gStaticData_080C2758`, `gStaticData_080DA1D8`) | `SelectActorCategory`, `GetAnimFrameData`, `sub_8029890` +2 | high | hard (strips) |
| `08151AC2` | 0x16012 | composite: BG1 pictures, sub-effect tables, rotation strip C (categories 3-6). **Converted** except the 2-byte pad (still `gStaticData_08151AC2`) and rotation strip C (`gStaticData_0815A050`) | `SelectActorCategory`, `GetAnimFrameData`, `sub_802F7B0` | high | hard (strip) |
| `08167AD4` | 0x200 | u16[256] fill-meter ramp/palette table (`u16` x 256) | `sub_8031504`, `sub_8031604` | high | easy |
| `08167CD4` | 0x1E14 | per-level P1 meter grid table: s16 cols, rows, then per-level records (s16) (`s16` x 3850) | `sub_8030F88` | medium | medium |
| `08169AE8` | 0x200 | u16[256] fill-meter table (P2 twin of 0x08167AD4) (`u16` x 256) | `sub_8032AF8`, `sub_8033604`, `sub_80336CC` | high | easy |
| `08169CE8` | 0xB28 | per-level P2 singleton grid table (s16 cols, rows, then records) | `sub_80331BC` | medium | medium |
| `0816A810` | 0x10 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `sub_8000760` | medium | easy |
| `0816A820` | 0x200 | s16[256] sine/direction table (`s16` x 256) | `sub_800AFF4`, `sub_800C8F8`, `sub_800C940` +16 | high | easy |
| `0816AA20` | 0x4C | pointer table: 19 GAX2 song pointers (into the built gax_audio_data.bin) (`void*` x 19) | `sub_80017BC` | high | easy |
| `0816AF10` | 0x228 | CRC-16/CCITT lookup table (poly 0x1021, u16[256]) + 0x28 trailing bytes (`u16` x 276) | `sub_8001CB8`, `sub_8002114` | high | easy |
| `0816B138` | 0x2 | small constant (3e00) | `sub_8003D3C` | medium | easy |
| `0816B13A` | 0x20 | table of u16; 1 word(s) look like ROM pointers (`u16` x 16) | `sub_800450C` | high | easy |
| `0816B15A` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_800450C` | high | easy |
| `0816B17A` | 0x20 | table of u16 (`u16` x 16) | `sub_800450C` | high | easy |
| `0816B19A` | 0x22 | table of u16 (`u16` x 17) | `sub_800450C` | high | easy |
| `0816B1BC` | 0x14 | table of s32 (`s32` x 5) | `sub_8003A60` | high | easy |
| `0816B1D0` | 0x14 | table of void* (`void*` x 5) | `sub_80061E8` | high | easy |
| `0816B1E4` | 0x8 | table of struct icon_pos | `sub_8005A78`, `sub_800619C` | high | easy |
| `0816B1EC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_8005AE8` | high | easy |
| `0816B20C` | 0x10 | table of u32 (`u32` x 4) | `sub_8005AE8` | high | easy |
| `0816B21C` | 0x28 | table of struct icon_pos | `sub_80057E0`, `sub_8005B80` | high | easy |
| `0816B244` | 0x14 | table of u32 (`u32` x 5) | `sub_8005B80` | high | easy |
| `0816B258` | 0x18 | table of struct icon_pos | `sub_80058C0`, `sub_8005C58` | high | easy |
| `0816B270` | 0xC | table of u32 (`u32` x 3) | `sub_8005C58`, `sub_8005D44` | high | easy |
| `0816B27C` | 0x8 | table of struct icon_pos | `sub_8005D44`, `sub_8006124` | high | easy |
| `0816B284` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `sub_8004EC0` | medium | easy |
| `0816B298` | 0x28 | table (element layout: see consumers) | `sub_8004EC0` | medium | easy |
| `0816B2C0` | 0x20 | table (element layout: see consumers) | `sub_8004D74` | medium | easy |
| `0816B2E0` | 0xC | table (element layout: see consumers) | `sub_80073DC`, `sub_8007634` | medium | easy |
| `0816B2EC` | 0xC | table (element layout: see consumers) | `sub_80073DC`, `sub_8007634` | medium | easy |
| `0816B2F8` | 0x8 | all zero (zero-initialised table) | `sub_0800D18C`, `sub_8007C30`, `sub_8007CF8` +3 | high | easy |
| `0816B300` | 0x4 | all zero (zero-initialised table) | `sub_80084C4`, `sub_800A884`, `sub_8011BD4` +1 | high | easy |
| `0816B304` | 0x318 | table of struct anim_rec | `sub_800B704`, `sub_800B838`, `sub_8012AF4` | high | easy |
| `0816B61C` | 0x2A4 | table of struct pctrl_anim | `sub_8016AB0`, `sub_801721C`, `sub_8017240` | high | easy |
| `0816B8C0` | 0x6C | table (element layout: see consumers) | `sub_8017808` | medium | easy |
| `0816B92C` | 0x8 | pointer table (1 data pointers) | `sub_802375C` | high | easy |
| `0816B934` | 0x8 | pointer table (1 data pointers) | `sub_802375C` | high | easy |
| `0816B93C` | 0x50 | table (element layout: see consumers); 1 word(s) look like ROM pointers | `sub_802375C` | medium | easy |
| `0816B98C` | 0x20 | table (element layout: see consumers) | `sub_801EF0C`, `sub_801F050`, `sub_801F170` +22 | medium | easy |
| `0816B9AC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_801F528` | high | easy |
| `0816B9CC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_801F8DC` | high | easy |
| `0816B9EC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_801F3DC` | high | easy |
| `0816BA0C` | 0x20 | table (element layout: see consumers) | `sub_801F170` | medium | easy |
| `0816BA2C` | 0x20 | table (element layout: see consumers) | `sub_801F050` | medium | easy |
| `0816BA4C` | 0x20 | table (element layout: see consumers) | `sub_801FA3C` | medium | easy |
| `0816BA6C` | 0x20 | table (element layout: see consumers) | `sub_801FDEC` | medium | easy |
| `0816BA8C` | 0x20 | table (element layout: see consumers) | `sub_801FCB4` | medium | easy |
| `0816BAAC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802062C` | high | easy |
| `0816BACC` | 0x20 | table (element layout: see consumers) | `sub_8020138` | medium | easy |
| `0816BAEC` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802026C` | high | easy |
| `0816BB0C` | 0x20 | table (element layout: see consumers) | `sub_80208C4`, `sub_8020B0C` | medium | easy |
| `0816BB2C` | 0x20 | table (element layout: see consumers) | `sub_80204EC`, `sub_8020D4C` | medium | easy |
| `0816BB4C` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_80203A8` | high | easy |
| `0816BB6C` | 0x28 | table (element layout: see consumers); 1 word(s) look like ROM pointers | `sub_800CA48` | medium | easy |
| `0816BB94` | 0x4 | small constant (281e140a) | `sub_800F990`, `sub_800FF0C` | medium | easy |
| `0816BB98` | 0x16 | table (element layout: see consumers) | `sub_800E6B0`, `sub_800E888`, `sub_800EEF0` | medium | easy |
| `0816BBAE` | 0x16 | table (element layout: see consumers) | `sub_800F06C`, `sub_800F6B8`, `sub_8010908` | medium | easy |
| `0816BBC4` | 0x16 | table (element layout: see consumers) | `sub_800D040`, `sub_800EDBC`, `sub_800F06C` +3 | medium | easy |
| `0816BBDA` | 0x16 | table (element layout: see consumers) | `sub_0800D18C`, `sub_800E7A8` | medium | easy |
| `0816BBF0` | 0xA8 | table of s32 (`s32` x 42) | `sub_0800D18C` | high | easy |
| `0816BC98` | 0x268 | table of s32[7] (`s32[7]` x 22) | `sub_0800D18C`, `sub_800E08C` | high | easy |
| `0816BF00` | 0x8 | small constant (0000010000000000) | `sub_0800D18C` | medium | easy |
| `0816BF08` | 0xC | table of s32 (`s32` x 3) | `sub_8011248` | high | easy |
| `0816BF14` | 0xC | table of struct three_words | `sub_801192C` | high | easy |
| `0816BF20` | 0x150 | pointer-to-member dispatch table: 42 x {0xFFFF0000, fn} (`struct act_pmf` x 42) | `sub_8012420` | high | easy |
| `0816C070` | 0x20 | pointer table (8 data pointers) (`struct level_anim*` x 8) | `sub_8016288`, `sub_8016DDC`, `sub_80170EC` +2 | high | easy |
| `0816C090` | 0x1C0 | table of struct speed_table | `sub_80159F8` | high | easy |
| `0816C250` | 0x40 | function-pointer / pointer-to-member table (8 code pointers) | `sub_8016288` | high | easy |
| `0816C290` | 0x40 | table of struct pmf; 4 word(s) look like ROM pointers | `sub_8017650` | high | easy |
| `0816C2D0` | 0x8 | pointer table (1 data pointers) | `sub_8017FA4` | high | easy |
| `0816C2D8` | 0x30 | table (element layout: see consumers) | `sub_8017ECC`, `sub_8017F14`, `sub_8017F5C` +1 | medium | easy |
| `0816C308` | 0x3 | small constant (040100) | `sub_8018008`, `sub_8018400` | medium | easy |
| `0816C30B` | 0x4D | table (element layout: see consumers) | `sub_801865C` | medium | easy |
| `0816C358` | 0x4 | small constant (100e0a20) | `sub_80196B8` | medium | easy |
| `0816C35C` | 0x3 | small constant (181612) | `sub_8018E4C` | medium | easy |
| `0816C35F` | 0x3 | small constant (040404) | `sub_8018E4C` | medium | easy |
| `0816C362` | 0x6 | small constant (020202000000) | `sub_8018E4C` | medium | easy |
| `0816C368` | 0x10 | table of s32 (`s32` x 4) | `sub_80197F8` | high | easy |
| `0816C378` | 0x18 | table of s32 (`s32` x 6) | `sub_80197F8` | high | easy |
| `0816C390` | 0x10 | table of s32 (`s32` x 4) | `sub_80197F8` | high | easy |
| `0816C3A0` | 0x18 | table of s32 (`s32` x 6) | `sub_80197F8` | high | easy |
| `0816C3B8` | 0x30 | table of s32 (`s32` x 12) | `sub_801A64C`, `sub_801A7AC` | high | easy |
| `0816C3E8` | 0xC | table (element layout: see consumers) | `sub_801A2A8` | medium | easy |
| `0816C3F4` | 0x24 | table (element layout: see consumers) | `sub_801A2A8` | medium | easy |
| `0816C418` | 0x40 | table of struct vec_pair | `sub_801A7AC` | high | easy |
| `0816C458` | 0x8 | pointer table (1 data pointers) | `sub_801B7D8` | high | easy |
| `0816C460` | 0x24 | table of struct vec3 | `sub_801B304`, `sub_801B6EC`, `sub_801B734` +2 | high | easy |
| `0816C484` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `sub_80047F8`, `sub_80063D8`, `sub_801BC28` +1 | medium | easy |
| `0816C498` | 0x8 | table of struct xy_pair | `sub_801BC28` | high | easy |
| `0816C4A0` | 0x8 | table of struct xy_pair | `sub_801BC28` | high | easy |
| `0816C4A8` | 0x8 | table of struct xy_pair | `sub_801BC28` | high | easy |
| `0816C4B0` | 0x8 | table of struct xy_pair | `sub_801BC28` | high | easy |
| `0816C4B8` | 0x8 | table of struct xy_pair | `sub_801BC28` | high | easy |
| `0816C4C0` | 0x8 | table of struct xy_pair | `sub_801BC28`, `sub_801C3E8` | high | easy |
| `0816C4C8` | 0x8 | table of struct xy_pair | `sub_801BC28` | high | easy |
| `0816C4D0` | 0x8 | table of struct xy_pair | `sub_801BC28` | high | easy |
| `0816C4D8` | 0x30 | table of struct xy_pair | `sub_801CEE0`, `sub_801D5CC` | high | easy |
| `0816C508` | 0x30 | table of struct xy_pair | `sub_801CEE0`, `sub_801D5CC` | high | easy |
| `0816C538` | 0x10 | table of u32 (`u32` x 4) | `sub_801CEE0`, `sub_801D668` | high | easy |
| `0816C548` | 0x10 | table of u32 (`u32` x 4) | `sub_801BC28`, `sub_801D470` | high | easy |
| `0816C558` | 0x14 | table of u32 (`u32` x 5) | `sub_801C608` | high | easy |
| `0816C56C` | 0x20 | table (element layout: see consumers) | `sub_801BAF0` | medium | easy |
| `0816C58C` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `sub_801D7F8` | medium | easy |
| `0816C5A0` | 0x50 | pointer table (20 data pointers) | `sub_801DAD8` | high | easy |
| `0816C5F0` | 0x20 | table of struct xy_pair | `sub_801D828` | high | easy |
| `0816C610` | 0x14 | table of u32 (`u32` x 5) | `sub_801DF0C` | high | easy |
| `0816C624` | 0x10 | table of u32 (`u32` x 4) | `sub_801DEA4` | high | easy |
| `0816C634` | 0x10 | table of u32 (`u32` x 4) | `sub_801E190` | high | easy |
| `0816C644` | 0x30 | table of s32 (`s32` x 12) | `sub_801E688`, `sub_801E788` | high | easy |
| `0816C674` | 0x30 | table of s32 (`s32` x 12) | `sub_801E688`, `sub_801E788` | high | easy |
| `0816C6A4` | 0x170 | function-pointer table: 92 Thumb function pointers (menu/trigger-effect dispatch) (`void (*)(void)` x 92) | `sub_8022208` | high | easy |
| `0816C814` | 0xA | table of u16 (`u16` x 5) | `sub_8023A1C` | high | easy |
| `0816C81E` | 0x12 | table of u16 (`u16` x 9) | `sub_8023A1C` | high | easy |
| `0816C830` | 0x12 | table of u16 (`u16` x 9) | `sub_8023A1C` | high | easy |
| `0816C842` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_8023A1C` | high | easy |
| `0816C862` | 0xA | table of u16 (`u16` x 5) | `sub_8023A1C` | high | easy |
| `0816C86C` | 0x514 | level table: u32 header (1) + 24 x 0x24 level records, followed by 36 x 0x10 medal item-list headers {count, items**, extra1, extra2} (`struct MedalTableEntry` x 36) | `sub_800599C`, `sub_8005D44`, `sub_80067EC` +17 | high | medium |
| `0816CD80` | 0x474 | room/medal records: sound-cue bytes, 0x14-byte records whose +0x04 pointer is a room record {palette*, level_desc*, kind} in 0x0817E78C, and item pointer arrays | `sub_8024498` | high | medium |
| `0816D1F4` | 0x53B4 | text/menu lists: {items*, count} headers, item pointer arrays, strings, and 23 intro-slide package records (0x1C) pointing at the Mode-4 palettes | `sub_8022468` | high | medium |
| `081725A8` | 0x4 | table of struct terrain_type | `sub_8025228` | high | easy |
| `081725AC` | 0x8 | small constant (ffffffffffffffff) | `sub_80250BC`, `sub_8025130` | medium | easy |
| `081725B4` | 0x8 | small constant (ffffffffffffffff) | `sub_8025130` | medium | easy |
| `081725BC` | 0x8 | small constant (ffffffffffffffff) | `sub_8025130` | medium | easy |
| `081725C4` | 0x261C | terrain shape table: 36-byte records (per-column heights 0..7, 0xFF = empty) used by the collision streamer | `sub_8025130` | medium | medium |
| `08174BE0` | 0x8C | table of u32 (`u32` x 35) | `sub_8027138`, `sub_802732C` | high | easy |
| `08174C6C` | 0x118 | table of struct hud_pos | `sub_8027138`, `sub_802732C`, `sub_802757C` +2 | high | easy |
| `08174D84` | 0x50 | table (element layout: see consumers) | `InitHudIconWidgetA` | medium | easy |
| `08174DD4` | 0x3B4 | table (element layout: see consumers) | `InitHudIconWidgetA` | medium | easy |
| `08175188` | 0x4C | table (element layout: see consumers) | `InitHudIconWidgetB` | medium | easy |
| `081751D4` | 0x384 | table (element layout: see consumers) | `InitHudIconWidgetB` | medium | easy |
| `08175558` | 0xC | first 0xC bytes of category_descriptor[0] (label splits the struct array) (`struct category_descriptor` x 7) | `InitActorCategory`, `SetupActorVramPool`, `sub_802968C` +1 | high | easy |
| `08175564` | 0x20 | category_descriptor[0] bytes 0x0C-0x2B (part of gStaticData_08175558[7]) | `InitActorCategory` | high | easy |
| `08175584` | 0x140 | category_descriptor[0] +0x2C .. [6] end (part of gStaticData_08175558[7]) | `InitActorCategory` | high | easy |
| `081756C4` | 0x9C | category_vtable[3]: 3 x 13 Thumb function pointers (slots 7/8 of type 0 hold junk values) | `SelectActorCategory` | high | easy |
| `08175760` | 0x3800 | palette-cycle frames: 32 x 0x1C0-byte BGR555 blocks (14 x 16-colour palettes each) | `sub_802AB58` | high | easy |
| `08178F60` | 0x10 | table of s32 (`s32` x 4) | `sub_802ABC8` | high | easy |
| `08178F70` | 0x10 | table of s32 (`s32` x 4) | `sub_802ABC8` | high | easy |
| `08178F80` | 0x1738 | categories 0-2 family data: 16-colour palettes, keyframe tables (table_A), anim table gStaticData_081796CC (41 x 0x28) and table_B arrays (absolute ROM pointers or sheet offsets) | `sub_80361B0` | high | medium |
| `0817A6B8` | 0x70 | function-pointer / pointer-to-member table (14 code pointers) | `sub_802B364`, `sub_802C208` | high | easy |
| `0817A728` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802B730`, `sub_802BB4C` | high | easy |
| `0817A748` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802BB4C` | high | easy |
| `0817A768` | 0xC | table (element layout: see consumers) | `sub_802C6C0` | medium | easy |
| `0817A774` | 0xC | table (element layout: see consumers) | `sub_802CC9C` | medium | easy |
| `0817A780` | 0xC | table (element layout: see consumers) | `sub_802CC9C` | medium | easy |
| `0817A78C` | 0xC | table (element layout: see consumers) | `sub_802CC9C` | medium | easy |
| `0817A798` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802D204` | high | easy |
| `0817A7B8` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802D2DC` | high | easy |
| `0817A7D8` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802D2DC` | high | easy |
| `0817A7F8` | 0x48 | table (element layout: see consumers) | `sub_802DB2C`, `sub_802DCC0` | medium | easy |
| `0817A840` | 0x10 | function-pointer / pointer-to-member table (4 code pointers) (`void (*)(void)` x 4) | `sub_802D7B0` | high | easy |
| `0817A850` | 0x30 | table (element layout: see consumers) | `sub_802DFDC` | medium | easy |
| `0817A880` | 0x1EC | table_B: 123 absolute pointers into rotation strip B (0x080DA1D8..) (`struct sprite_frame *` x 123) | `sub_802DFDC` | high | easy |
| `0817AA6C` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_802D9A8` | high | easy |
| `0817AA8C` | 0xC | table (element layout: see consumers) | `sub_802DD9C` | medium | easy |
| `0817AA98` | 0x1728 | categories 3-6 family data: s16 header, palettes, table_A, anim table gStaticData_0817B2A4 (47 x 0x28), table_B arrays | `sub_802D7B0` | high | medium |
| `0817C1C0` | 0x40 | function-pointer / pointer-to-member table (8 code pointers) | `sub_802E84C`, `sub_802F748` | high | easy |
| `0817C200` | 0x60 | BGR555 palette(s): 3 x 16 colours (`u16` x 48) | `sub_802F4CC` | high | easy |
| `0817C260` | 0x20 | function-pointer / pointer-to-member table (4 code pointers) | `sub_802FA38`, `sub_802FEA4` | high | easy |
| `0817C280` | 0x38 | function-pointer / pointer-to-member table (7 code pointers) | `sub_802FFB8`, `sub_8030234` | high | easy |
| `0817C2B8` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `sub_8030574`, `sub_8030648` | high | easy |
| `0817C2D0` | 0xA8 | table of struct weapon_kind | `sub_8031040` | high | easy |
| `0817C378` | 0x60 | BGR555 palette(s): 3 x 16 colours (`u16` x 48) | `sub_8031040`, `sub_8031744` | high | easy |
| `0817C3D8` | 0xC | table of s16 (`s16` x 6) | `sub_80309B4`, `sub_8030E08`, `sub_8031378` | high | easy |
| `0817C3E4` | 0x18 | table of struct anim_frame_record | `sub_8030F88` | high | easy |
| `0817C3FC` | 0x18 | function-pointer / pointer-to-member table (6 code pointers) (`void*` x 6) | `sub_80311C4` | high | easy |
| `0817C414` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `sub_8031A08` | high | easy |
| `0817C42C` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `sub_8031A6C`, `sub_80322F4` | high | easy |
| `0817C444` | 0xC | table (element layout: see consumers) | `sub_8032480` | medium | easy |
| `0817C450` | 0x10 | function-pointer / pointer-to-member table (2 code pointers) | `sub_8032950`, `sub_8032A94` | high | easy |
| `0817C460` | 0x50 | table of struct singleton_kind | `sub_8033264` | high | easy |
| `0817C4B0` | 0xC | table of s16 (`s16` x 6) | `sub_8032C0C` | high | easy |
| `0817C4BC` | 0xC | table (element layout: see consumers) | `sub_80331BC` | medium | easy |
| `0817C4C8` | 0x18 | function-pointer / pointer-to-member table (6 code pointers) (`void*` x 6) | `sub_8032B6C` | high | easy |
| `0817C4E0` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `sub_8033B44`, `sub_8033C84` | high | easy |
| `0817C4F8` | 0x18 | function-pointer / pointer-to-member table (3 code pointers) | `sub_8033E80`, `sub_8033FE4` | high | easy |
| `0817C510` | 0x2 | small constant (3e00) | `sub_8034AA4` | medium | easy |
| `0817C512` | 0x20 | table of u16; 1 word(s) look like ROM pointers (`u16` x 16) | `sub_803487C` | high | easy |
| `0817C532` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_803487C` | high | easy |
| `0817C552` | 0x20 | table of u16 (`u16` x 16) | `sub_803487C` | high | easy |
| `0817C572` | 0x22 | table of u16 (`u16` x 17) | `sub_803487C` | high | easy |
| `0817C594` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `sub_803472C` | medium | easy |
| `0817C5A8` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `sub_803472C` | medium | easy |
| `0817C5BC` | 0x14 | table (element layout: see consumers); 3 word(s) look like ROM pointers | `sub_803472C` | medium | easy |
| `0817C5D0` | 0x96C | popup/credits text opcode stream (control bytes 0x01-0x04 + ASCII) | `sub_8034CEC` | high | medium |
| `0817CF3C` | 0x4 | all zero (zero-initialised table) | `sub_80350A4` | high | easy |
| `0817CF40` | 0x64 | table of struct popup_glyph_src; 10 word(s) look like ROM pointers | `sub_80352AC` | high | easy |
| `0817CFA4` | 0x50 | table (element layout: see consumers); 9 word(s) look like ROM pointers | `sub_8035E14`, `sub_80360DC` | medium | easy |
| `0817CFF4` | 0x40 | BGR555 palette(s): 2 x 16 colours (`u16` x 32) | `sub_80358A8` | high | easy |
| `0817D034` | 0x20 | table (element layout: see consumers); 1 word(s) look like ROM pointers | `LoadLevelGraphics` | medium | easy |
| `0817D054` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `LoadLevelGraphics` | high | easy |
| `0817D074` | 0x70 | table (element layout: see consumers); 12 word(s) look like ROM pointers | `LoadLevelGraphics` | medium | easy |
| `0817D0E4` | 0x5B4 | bg_package for LoadBg2Background {w, h, palette*, tiles*, map*, ...} + trailing records | `LoadBg2Background` | high | easy |
| `0817D698` | 0x28 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `sub_80361B0` | medium | easy |
| `0817D6C0` | 0xA8 | table (element layout: see consumers); 20 word(s) look like ROM pointers | `sub_8036600` | medium | easy |
| `0817D768` | 0x14 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `sub_8036528` | medium | easy |
| `0817D77C` | 0x14 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `sub_8036528` | medium | easy |
| `0817D790` | 0x14 | table (element layout: see consumers); 2 word(s) look like ROM pointers | `sub_8036528` | medium | easy |
| `0817D7A4` | 0xF70 | bg_package (graphics_loading_35780.c) + trailing per-screen records | `sub_8036CF4` | medium | medium |
| `0817E714` | 0x18 | pointer table (6 data pointers) (`void*` x 6) | `sub_80372BC` | high | easy |
| `0817E72C` | 0x20 | table of u16; 1 word(s) look like ROM pointers (`u16` x 16) | `sub_8037388` | high | easy |
| `0817E74C` | 0x20 | BGR555 palette(s): 1 x 16 colours (`u16` x 16) | `sub_8037388` | high | easy |
| `0817E76C` | 0x20 | table of u16 (`u16` x 16) | `sub_8037388` | high | easy |
| `0817E78C` | 0x326E74 | composite: level BG tile sets (raw tag-0x00 assets), per-room level data, sprite-bank tile pool | `sub_8037388` | high | medium |
| `084A5600` | 0xB66B4 | composite: sprite-bank (animation) table (**converted**, C) + GAX2 sound-effect bank | `sub_8004D74`, `sub_8022230` | high | medium |
| `085A4C5C` | 0x14 | pointer table (4 data pointers) | `sub_8037FC0`, `sub_8038538` | high | easy |
| `085A4C70` | 0x100 | u8[256] count-leading-zeros table (libgcc __clz_tab, used by the 64-bit divide) (`UQItype` x 256) | `sub_8037648` | high | easy |
| `085A4D70` | 0x100 | u8[256] count-leading-zeros table (second copy, __clz_tab of another libgcc object) (`UQItype` x 256) | `sub_8037A7C` | high | easy |
| `085A5519` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `085A60FF` | 0x4D | 1 B pad + GAX2 version string "GAX Sound Engine 2.01D (Sep 28 2001) (c) Shin'en Multimedia. Code: B.Wodok" + NUL padding | `sub_8038538 (via gStaticData_085A614C)` | high | easy |
| `085A614C` | 0x4 | pointer table (1 data pointers) (`u8*` x 1) | `sub_8038538` | high | easy |
| `085A6150` | 0x60 | table of struct RateEntry | `sub_8037FA0`, `sub_8037FC0`, `sub_8038538` | high | easy |
| `085A61B0` | 0xC | table (element layout: see consumers) | `sub_80381FC` | medium | easy |
| `085A61BC` | 0x14 | table (element layout: see consumers) | `sub_80381FC` | medium | easy |
| `085A61D0` | 0xC | GAX2 error/tag string "GAX2_INIT" (NUL-padded to 4) (`char` x 12) | `sub_8038538`, `sub_8038A1C` | high | easy |
| `085A61DC` | 0x10 | GAX2 error/tag string "OUT OF MEMORY" (NUL-padded to 4) (`char` x 16) | `sub_8038538`, `sub_8038A1C` | high | easy |
| `085A61EC` | 0xC | GAX2 error/tag string "GAX2_JINGLE" (NUL-padded to 4) (`char` x 12) | `sub_8038A1C` | high | easy |
| `085A61F8` | 0x1C | GAX2 error/tag string "GAX_NO_JINGLE FLAG IS SET" (NUL-padded to 4) (`char` x 28) | `sub_8038A1C` | high | easy |
| `085A6214` | 0x8 | GAX2 error/tag string "GAX_IRQ" (NUL-padded to 4) | `sub_8038B68` | high | easy |
| `085A621C` | 0xAC | GAX2 error string "GAX_PLAY HAS NOT FINISHED BEFORE GAX_IRQ. USE LOWER MIXING RATE ..." (+ padding) | `sub_8038B68` | high | easy |
| `085A62C8` | 0x4 | pointer table (1 data pointers) (`u8*` x 1) | `sub_80392E0` | high | easy |
| `085A62CC` | 0x10 | table (element layout: see consumers) | `sub_80392E0` | medium | easy |
| `085A62DC` | 0x3BD0 | GAX2 u32 note period/frequency table (smooth ramp; embedded 0x08xxxxxx values are coincidence) (`u32` x 3828) | `sub_8039B44` | high | easy |
| `085A9EAC` | 0x4C | table of s8 (`s8` x 76) | `sub_8039FFC` | high | easy |
| `085A9EF8` | 0xC | table of struct EepromConfig | `sub_803A968` | high | easy |
| `085A9F04` | 0xC | table of struct EepromConfig | `sub_803A968` | high | easy |
| `085A9F10` | 0x260 | EEPROM timing/timeout table (u16) (`u16` x 304) | `sub_803AC04` | high | easy |
| `085ADBD1` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085B34E1` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085B82BC` | 0x200 | 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085BC6E8` | 0x200 | 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085C1936` | 0x202 | 2 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085C674D` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085CB15D` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085CFECA` | 0x202 | 2 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085D6531` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085DB765` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085DFABC` | 0x200 | 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085E550B` | 0x201 | 1 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085EAC44` | 0x200 | 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085F0367` | 0x201 | 1 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085F4CC1` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085FA1D2` | 0x202 | 2 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `085FF779` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `086039B2` | 0x202 | 2 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `08608A50` | 0x200 | 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `0860D577` | 0x201 | 1 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `08611BD9` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `08616360` | 0x200 | 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `08619F51` | 0x203 | 3 B zero pad + 256-colour BGR555 palette (0x200) for the preceding Mode-4 intro bitmap | `gStaticData_0816D1F4 (slide packages)` | high | easy |
| `0861BF2E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0861C182` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0861C309` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0861E5F6` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862556B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08628C4F` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862A957` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862C2C5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862C4AD` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862CC3B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862CE6E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862D0CB` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862E3AF` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0862FFF3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086308EF` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08630E19` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08631155` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08631372` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086313AD` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08631557` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086315BB` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863183A` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863199E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086324B3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863281E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08632BC2` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086334C1` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08636EF1` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08637603` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086377BD` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08637A6E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086382C6` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08639459` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863A60A` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863AD8D` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863B666` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863BDD2` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863CF95` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863D333` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0863D4BF` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08640EF5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08644A86` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08649417` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0864D7E5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0864F82F` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08651CF7` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08655175` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0865BAEB` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0865E05A` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08661F46` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086652B6` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08665935` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0867045E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0867829B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0867EDF3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086834F5` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08689F72` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `0868D311` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `08693B8E` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086A8349` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086AF12B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086BACB3` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086C127C` | 0x18A30 | raw (unpacked) level asset for room 0x0825E7DC: u16 chunk offset table + chunk token streams (custom RLE/delta, sub_8024960/sub_8025334) | `sub_80266BC`, `sub_80254F8`, `sub_8024CF0` +2 | high | medium |
| `086E044B` | 0x1 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086E2212` | 0x2 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086E3541` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086EA0C9` | 0x3 | padding (zero, aligns the next LZ77 blob to 4) | - | high | easy |
| `086ECCD2` | 0xF6F1A | 6 raw (unpacked) level assets, same format as 0x086C127C | `sub_80266BC`, `sub_80254F8`, `sub_8024CF0` +2 | high | medium |
| `087E3BEC` | 0x58 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80071E4`, `sub_800725C`, `sub_80073BC` +2 | high | easy |
| `087E3C44` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8008434`, `sub_80084A4` | high | easy |
| `087E3CAC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80088F0`, `sub_8008904` | high | easy |
| `087E3D14` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8009ED0`, `sub_8009F1C`, `sub_8009F90` | high | easy |
| `087E3D8C` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800A604`, `sub_800A650`, `sub_800A6A4` | high | easy |
| `087E3E04` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800B3AC`, `sub_800B3F0` | high | easy |
| `087E3E7C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800B8A8`, `sub_800B8C8` | high | easy |
| `087E3EE4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800CA60`, `sub_800CA74` | high | easy |
| `087E3F4C` | 0x58 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800CB40` | high | easy |
| `087E3FA4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800CBC0`, `sub_800CBD4` | high | easy |
| `087E400C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800CCCC`, `sub_800CCE0` | high | easy |
| `087E4074` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_800FF0C`, `sub_801071C`, `sub_801075C` | high | easy |
| `087E40DC` | 0x70 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8011114`, `sub_80112F4`, `sub_8011310` | high | easy |
| `087E414C` | 0x70 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801173C`, `sub_80119D8`, `sub_80119FC` | high | easy |
| `087E41BC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8011B0C`, `sub_8011B5C`, `sub_8011B70` | high | easy |
| `087E4224` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8015878`, `sub_801588C` | high | easy |
| `087E428C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80174D8`, `sub_80174EC` | high | easy |
| `087E42F4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80179EC`, `sub_8017A00` | high | easy |
| `087E435C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8017A78`, `sub_8017A8C` | high | easy |
| `087E43C4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8017FD4`, `sub_8017FE8` | high | easy |
| `087E442C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8018858`, `sub_801886C` | high | easy |
| `087E4494` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80188D0`, `sub_80188E8` | high | easy |
| `087E44FC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8018948`, `sub_8018960` | high | easy |
| `087E4564` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80189C4`, `sub_80189EC` | high | easy |
| `087E45CC` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80195D8`, `sub_80195EC` | high | easy |
| `087E4634` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8019608`, `sub_801961C` | high | easy |
| `087E469C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801964C`, `sub_8019660` | high | easy |
| `087E4704` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80196E4`, `sub_80196F8` | high | easy |
| `087E476C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8019744`, `sub_8019758` | high | easy |
| `087E47D4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80197C8`, `sub_80197DC` | high | easy |
| `087E483C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801A724`, `sub_801A73C` | high | easy |
| `087E48A4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801A584`, `sub_801A750`, `sub_801A768` | high | easy |
| `087E490C` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801A780`, `sub_801A794` | high | easy |
| `087E4974` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801A824`, `sub_801A838` | high | easy |
| `087E49DC` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801A878`, `sub_801B2C4`, `sub_801B2E4` | high | easy |
| `087E4A54` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801B7C4`, `sub_801B7D8` | high | easy |
| `087E4ABC` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801B91C`, `sub_801B940` | high | easy |
| `087E4B34` | 0x78 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801B984`, `sub_801BAB0`, `sub_801BAD0` | high | easy |
| `087E4BAC` | 0x30 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_801DF98`, `sub_801DFEC` | high | easy |
| `087E4BDC` | 0x10 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8024D0C`, `sub_8024D38` | high | easy |
| `087E4BEC` | 0x28 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8024D74`, `sub_8024DAC` | high | easy |
| `087E4C14` | 0x50 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8025D74`, `sub_80261B8`, `sub_8026418` | high | easy |
| `087E4C64` | 0x50 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8026418`, `sub_8026448` | high | easy |
| `087E4CB4` | 0x68 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802710C`, `sub_8027120` | high | easy |
| `087E4D1C` | 0x48 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitHudIconWidgetB`, `sub_803AFF0` | high | easy |
| `087E4D64` | 0x48 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitHudIconWidgetA`, `sub_803B024` | high | easy |
| `087E4DAC` | 0x48 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitHudIconWidgetA`, `InitHudIconWidgetB`, `InitHudTextWidget` +3 | high | easy |
| `087E4DF4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `InitActorPart`, `sub_802AA54`, `sub_802C19C` +41 | high | easy |
| `087E4E14` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28` | high | easy |
| `087E4E34` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802B12C` | high | easy |
| `087E4E54` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `ConstructActorPart`, `sub_802C19C` | high | easy |
| `087E4E74` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802C394`, `sub_802C3E8` | high | easy |
| `087E4E94` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802C4A4` | high | easy |
| `087E4EB4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802CB9C` | high | easy |
| `087E4ED4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802CBC0` | high | easy |
| `087E4EF4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802CBE4` | high | easy |
| `087E4F14` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802CC08` | high | easy |
| `087E4F34` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802CC2C` | high | easy |
| `087E4F54` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802CC54` | high | easy |
| `087E4F74` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802AC28`, `sub_802CC78` | high | easy |
| `087E4F94` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802CB34` | high | easy |
| `087E4FB4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802CDE4` | high | easy |
| `087E4FD4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802CE38` | high | easy |
| `087E4FF4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802CF0C` | high | easy |
| `087E5014` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802D0C8` | high | easy |
| `087E5034` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802D1B8` | high | easy |
| `087E5054` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802D528` | high | easy |
| `087E5074` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802D5D4` | high | easy |
| `087E5094` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802D648` | high | easy |
| `087E50B4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802D764` | high | easy |
| `087E50D4` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802E3CC` | high | easy |
| `087E510C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802E420` | high | easy |
| `087E5144` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802E740`, `sub_802F6DC` | high | easy |
| `087E517C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802FA04` | high | easy |
| `087E51B4` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802FD8C` | high | easy |
| `087E51EC` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_802FF08` | high | easy |
| `087E5224` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8030300` | high | easy |
| `087E525C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80305F8` | high | easy |
| `087E5294` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8031920` | high | easy |
| `087E52CC` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8032054` | high | easy |
| `087E530C` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8031F78` | high | easy |
| `087E534C` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80320C4` | high | easy |
| `087E538C` | 0x40 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8031F78`, `sub_8032054`, `sub_80320C4` +1 | high | easy |
| `087E53CC` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8032440` | high | easy |
| `087E5404` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80325EC` | high | easy |
| `087E543C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80326E4` | high | easy |
| `087E5474` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_803283C`, `sub_8032890` | high | easy |
| `087E54AC` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80329D4` | high | easy |
| `087E54E4` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8033BB8` | high | easy |
| `087E551C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8033EF4` | high | easy |
| `087E5554` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8034058` | high | easy |
| `087E558C` | 0x38 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_80342D4` | high | easy |
| `087E55C4` | 0x20 | gcc 2.x vtable: 8-byte {s16 delta, s16 pad, fnptr} slots, first two words zero | `sub_8036E20`, `sub_803716C` | high | easy |
| `087E55E4` | 0x1AA1C | IWRAM image (ARM code + initialised data, 0x9E8 B) copied to 0x03000000 by crt0, then 0xFF cartridge fill | `_0800012C` | high | medium |
