# Level data: rooms, layers, chunk streams, entities

The game has 41 rooms (playable areas). Everything a room needs besides
its tile graphics is built from [`data/levels/`](../data/levels) by
[`tools/levels.py`](../tools/levels.py):

- the room data proper, 178,208 bytes in two ROM regions
  (`0x0824B638`-`0x08270F08`, 33 rooms, and `0x082B91D0`-`0x082BF120`,
  8 rooms): descriptors, chunk grids, palettes and entity lists, as typed
  C in `src/data/level_rooms_24b638.c` and `src/data/level_rooms_2b91d0.c`;
- the 41 level assets (the per-layer tilemaps, compressed), `gRoomNNAsset`
  in `data/data.s`: 7 raw ones at `0x086C127C` and
  `0x086ECCD4`-`0x087BC13C` (1,112,392 bytes), and 34 LZ77-packed ones
  between the intro graphics (`0x0864F830`-`0x086EC63C`, formerly the
  "unidentified" `graphics/tileset1/27`-`60` `.bin` files).

The format below comes from the matched C that reads it. The consumer of
each part is named in its section.

## The pointer graph

The level table `gLevelTable` (`struct level_info`, one per
level) gives each level a room list: the rooms played in order and up
to two extra rooms, each a `struct level_room` record
`{const u16 *palette; const struct level_desc *desc; s32 kind; ...}`
(0x14 bytes). All of it is C in `src/data/level_table_16c814.c`, with
one record per room (`gLevelRoom00`..`gLevelRoom40`) plus seven records
of kind 3 for the stages played in an actor category, which have no
room data. The room record is the widget `RunRoom` hands to
`LoadRoom` (`level_layers.c`), which loads a room: it unpacks or
references the asset, feeds each layer its descriptor, feeds the terrain
cache the collision layer, hands the entity list and links to
`SpawnRoomEntities` (`game_loop41.c`) and DMAs the palette to BG palette RAM.
The rooms are numbered here in the order of their records
(`room00`..`room40`); the directory names add the descriptor's ROM
offset (`room17_25e7dc` is the room whose `level_desc` is at
`0x0825E7DC`).

The types are in [`include/level_data.h`](../include/level_data.h). Each
struct names the code's own local views of the same record.

### `struct level_desc` (0x30)

| Offset | Field | Consumer |
|---|---|---|
| 0x00 | `layers[3]`: BG1-3 layer descriptors (NULL = layer unused) | `LoadBgLayer` via `LoadRoom` |
| 0x0C | `layer0`: BG0 (tile-slot pooled, 8bpp) | same |
| 0x10 | `collision`: the collision layer | `SetCollisionSource` (terrain cache) |
| 0x14 | `asset`: the level asset | `LoadRoom` |
| 0x18 | `u8 assetPacked`: 1 = LZ77 stream (unpacked to the heap), 0 = used in place | same |
| 0x1C | `entities`: `struct level_entity_list` | `SpawnRoomEntities`, `CountCrateEntities`, `CreatePlatform`, ... |
| 0x20 | `links`: `struct level_link_list` or NULL | `SpawnRoomEntities` |
| 0x24 | 12 zero bytes in every room | - |

### `struct level_layer_desc` (0x20)

`struct bg_layer_desc` in `bg_scroll_layer_25fc8.c`, `struct
stream_source` in `game_loop57.c`, the terrain cache's `source`.

| Offset | Field |
|---|---|
| 0x00 | `chunkGrid`: `u16[gridWidth * gridHeight]`, row-major chunk ids |
| 0x04 | `assetOffset`: the layer's section in the asset |
| 0x08 | `tileData`: the BG tile set (NULL for the collision layer) |
| 0x0C | `scaleX`, `scaleY` (`s32`, Q8): parallax factors (`ScaleBgLayerScroll`) |
| 0x14 | `u16 cnt`: BGnCNT bits; only the priority is used |
| 0x16 | `u16 gridWidth`, `gridHeight`: in chunks |
| 0x1A | `u16 widthTiles`, `heightTiles`: the scrollable size |
| 0x1E | `u16`, 2 in every layer |

A layer is a grid of **chunks** of 16x8 cells (128x64 pixels). BG1-3 use
the 4bpp LZ77 tile sets `gStaticData_0863D4C0`..`gStaticData_0864D7E8`
(`graphics/tileset1/21`-`26`), BG0 uses one of the five 8bpp tag-0x00 tile
sets (`graphics/level_tilesets/`).

### Cells

Every layer's cells are `u16`:

- **BG1-3**: a BG screen entry (tile 0-9, h/v flip 10-11, palette bank
  12-15), copied straight into the screen block (`DrawBgLayerRow`).
- **BG0**: a source tile id (bits 0-13, into the 8bpp tile set) with the
  h/v flip in bits 14-15. `AcquireTileSlot` (`tile_slot_pool.c`) gives the
  tile a VRAM slot and returns the screen entry.
- **Collision**: terrain type in bits 0-7 (types 1-0x23 are the
  non-solid ones `GetTerrainHeights` returns, above that the solid shapes of the
  `gTerrainTypes` table), a flag nibble in bits 8-11, and in bits
  12-15 the per-collision-mode "not solid" bits `GetSolidTerrainHeights`/`sub_8025228`
  test (mode 0: 4, 1: 1, 2: 8, 3: 2). One cell is one 8x8 tile.

## The level asset

The asset is the layers' sections back to back, in slot order (BG1, BG2,
BG3, BG0, collision); `assetOffset` is each one's start. The packed form
is one LZ77 stream of the whole thing (`LoadTaggedAsset`).

A section is a table of `u16` chunk offsets, in words from the section
start (one zero entry pads an odd count), then one token stream per chunk,
each padded to a word. The chunk id in the grid indexes the table. The
chunks are numbered in order of first appearance in the grid (row-major),
and no two are equal, so the grid and the chunk set are both determined
by the layer's full tilemap.

A chunk stream decodes to exactly 128 cells, row-major. `DecodeLayerChunk`
(visual layers, into the 64x32 ring buffer) and `DecodeCollisionChunk` (terrain
cache, into a 256-byte slot) run the same loop: read a `u16` token, `n` =
its low byte, subtract `n` from a budget of 0x7F, stop once it goes
negative.

| Token | Meaning | Following halfwords |
|---|---|---|
| bit 15 set | fill: `n` copies of a value | the value |
| bit 14 set | delta: a first value, then `n - 1` signed byte deltas | the value, then the deltas two per halfword (low byte first); an odd last delta has its own halfword, high byte ignored |
| else | copy: `n` literal cells | `n` values |

A delta run of `n < 3` would never terminate, so the encoder never writes
one.

### Reproducing the ROM's streams

`tools/levels.py` re-encodes every one of the 16,323 chunks byte for byte:

- **Tokens.** A run of 4+ equal cells is a fill (the whole run). Any
  other stretch up to the next such run is a single copy, unless a delta
  run at its start saves space: the encoder counts a delta run of `d` as
  `2 + (d - 1) / 2` halfwords (rounded down, one short for even `d`), plus
  `1 + rest` for the copy of the rest of the stretch, which it always
  counts at the end of the chunk, and takes the delta only if that is
  less than `1 + stretch`. Deltas are limited to -127..127.
- **Leftover bytes.** The ignored high byte of an odd last delta and the
  word-alignment pads aren't zero. The original tool wrote each stretch as
  a copy followed by the next fill, then rewrote a stretch that is better
  as a delta (+ copy) in place and moved the fill down. That leaves the
  copy's cell bytes under the delta's last halfword and stale bytes past
  the end of the chunk. Simulating that in a zeroed buffer reproduces all
  but two pads (8,085 of 8,087); those two hold bytes of another chunk
  (the tool's heap), and are kept in `room.json` as `pad_overrides`,
  keyed by the chunk's index and checked against its CRC, so an edit to
  that chunk drops the override instead of misapplying it.

## Entities

`struct level_entity_list` (0x14):

| Offset | Field |
|---|---|
| 0x00 | `u16 count`: all entities |
| 0x02 | `u16 groupCount` |
| 0x04 | `groups`: `{u16 first; u16 count; struct level_entity *entities;}` per 256-px column |
| 0x08 | `paramOffsets`: `u16` byte offset of each parameter record |
| 0x0C | `params`: the parameter records |
| 0x10 | `typeCounts`: `u16[93]`, entities per type |

An entity is `{u16 type, x, y, param}`. `SpawnRoomEntities` walks the columns
from the last to the first and gives each entity, in that order, a
running id (its bit in the `gEntityFlags` bitmaps, and the id the
links use), and unless that bit is set calls the spawn function `type`
of the table at `gEntitySpawner` (`SpawnEntity`) with the id, x, y and
`param`. `param` indexes the parameter records: a flags word (bits 1 and
2 go to the spawned object's `+0x28` flags: `SpawnBasicCrate`, `SpawnStartMarker`,
`actor_part_1967c.c`) and per-type words, e.g. `struct spawn_rec` of
`CreatePlatform` (mover kind and distances); type 0x1A takes its effective
type from the record's `+8` (`CountCrateEntities`).

In the ROM:

- the entities are stored in id order (the last column first), and a
  column's `entities` points at its first one, empty columns included;
- `first` is the number of entities in the columns before it (from the
  first column), `groupCount` is `widthTiles * 8 / 256 + 1` of the
  collision layer, and every entity's column is `x >> 8`;
- the records' offsets are increasing and the records back to back, and
  `typeCounts` is the histogram of `type`.

`struct level_link_list` is `{s32 count; {s32 from, to} links[count]}`:
`SpawnRoomEntities` chains entity `from` to entity `to` after spawning.
The links stack crates: `from` is the lower crate and `to` the one on
top of it (`SetCrateAbove`/`SetCrateBelow`).

### Entity types

The spawn functions in `gEntitySpawnFuncs`, identified from the sprite
bank and animation each one sets up (`graphics/sprites/`), the pickup
code it gives the object and where the levels place it:

| Type | Spawner | What |
|---|---|---|
| 0x06 | `SpawnWumpa` | a wumpa fruit (bank 35) |
| 0x07 | `SpawnCrystal` | the level's crystal (bank 37) |
| 0x08 | `SpawnGemPathGem` | the gem-path clear gem (bank 32, kind 0x1E, level flag bit 2); placed only in the gem-path rooms |
| 0x09, 0x0A, 0x0B, 0x0C | `SpawnBlueGem`, `SpawnRedGem`, `SpawnGreenGem`, `SpawnYellowGem` | the coloured gems (bank 32) |
| 0x10 | `SpawnStopwatch` | the time-trial stopwatch (bank 36) |
| 0x12, 0x13, 0x14, 0x4A | `SpawnTurboRunPower`, `SpawnDoubleJumpPower`, `SpawnBodySlamPower`, `SpawnTornadoSpinPower` | the four power pictures (bank 38); no level places them |
| 0x15-0x27 | `SpawnBasicCrate` .. `SpawnTimeCrate3` | crates: `CreateCrate` types 0-18 (bank 31), see `include/crate.h` |
| 0x51-0x54 | `SpawnRedGemPlatform`, `SpawnYellowGemPlatform`, `SpawnGreenGemPlatform`, `SpawnBlueGemPlatform` | a gem outline over a platform (bank 32) |
| 0x28 | `SpawnLizard` | enemy, bank 13 |
| 0x29 | `SpawnVulture` | enemy, bank 11 |
| 0x2A | `SpawnVenusFlytrap` | enemy, bank 10 |
| 0x2C | `SpawnBlowgunTribesman` | enemy, bank 12 |
| 0x2D | `SpawnPenguin` | enemy, bank 15 |
| 0x2E | `SpawnSealSpawner` | sends a seal (`SpawnSeal`, bank 17) every 0x78 frames |
| 0x2F | `SpawnPolarBear` | enemy, bank 16 |
| 0x30 | `SpawnPufferfish` | enemy, bank 5 |
| 0x31 | `SpawnShark` | enemy, bank 4 |
| 0x32 | `SpawnMorayEel` | enemy, bank 3 |
| 0x33 | `SpawnElectricEel` | enemy, bank 8 |
| 0x34 | `SpawnSquid` | enemy, bank 7 |
| 0x35 | `SpawnJellyfish` | enemy, bank 9 |
| 0x37 | `SpawnLaserBarrier` | hazard, bank 25 |
| 0x3A, 0x3F | `SpawnSaucerLabAssistant` | enemy, bank 29 |
| 0x3B | `SpawnPistonCrusher` | hazard, bank 26 |
| 0x3D | `SpawnLaunchPadEntity` | the green launch pad (`SpawnLaunchPad`, bank 28) |
| 0x40 | `SpawnFlamethrowerLabAssistant` | enemy, bank 23 |
| 0x43 | `SpawnRat` | enemy, bank 21 |
| 0x44 | `SpawnFrog` | enemy, bank 19 |
| 0x45 | `SpawnDingodile` | Dingodile (`CreateDingodile`, bank 54) |
| 0x47 | `SpawnTiny` | Tiny Tiger (`CreateTiny`, bank 55) |
| 0x48 | `SpawnCortexBoss` | the Neo Cortex fight's controller (`CreateCortexBoss`, bank 53); only the "neo cortex" level places it |
| 0x49 | `sub_8021668` | the chaser (`UpdateChaser`, bank 30); only room 37 places it |
| 0x4B, 0x4C | `SpawnSeaMine` | hazard, bank 6 |
| 0x4D | `SpawnWoodenCrusher` | hazard, bank 18 |
| 0x4E, 0x4F, 0x50 | `SpawnLargePlatform`, `SpawnSmallPlatform`, `SpawnMediumPlatform` | `CreatePlatform` kinds 0-2 (bank 39) |
| 0x56 | `SpawnBonusPlatform` | the "?" platform (bank 39 anim 5) |
| 0x58 | `SpawnRockPlatform` | `CreatePlatform` kind 8 (bank 39) |
| 0x5A | `SpawnFlame` | a flame (bank 44) |
| 0x5B | `SpawnSeaweed` | seaweed (bank 45) |

Every enemy is a sprite part on its bank driven by one shared controller
(`CreateEnemyCtrl`, `UpdateEnemyCtrl`), whose `kind` is the bank number.

In time trial (`gLevelState->timeTrial`) a crate whose parameter record
says so becomes the time crate its record's `+4` names (`CreateCrate`).

## ROM layout

Each room's data is one contiguous run, in this order: parameter
records, their offsets, the chunk grids (slot order), the palette, the
type counts, the links (if any), the `level_desc`, the entity list
header, the layer descriptors (slot order), the groups, the entities.
The only gaps are the zero bytes agbcc's alignment gives a 4-aligned
object after a `u16` one. `data/levels/levels.json` lists the rooms of
each region in ROM order.

## Sources and build

`data/levels/`:

- `levels.json`: the rooms, and per region its C file name and its rooms
  in ROM order.
- `roomNN_xxxxxx/room.json`: the palette (BGR555), the asset's symbol and
  whether it is packed, per layer its tile set, scale, `cnt`, size in
  chunks and tiles and map file, the parameter records (lists of words),
  the links and the entities (in id order), and the symbol names of the
  room's objects: `gRoomNN` and what the object is (`gRoom17Desc`,
  `gRoom17Palette`, `gRoom17Bg1Grid`, `gRoom17Asset`, ...; the tile sets
  keep their own names).
- `roomNN_xxxxxx/<layer>.map.bin`: the layer's decoded tilemap,
  `gridWidth * 16` by `gridHeight * 8` little-endian `u16` cells,
  row-major (grit's flat `-mLf` map layout). These are the cells above,
  not ROM bytes: the ROM only has the chunk streams.

Everything else is derived again: the chunk set and grid (from the map),
the asset and the layers' `assetOffset`s, the entity groups, the counts,
the parameter offsets (`LEVEL_PARAM_OFFSET`, an `offsetof` into the
room's generated parameter struct).

`levels.mk` runs:

- `tools/levels.py asset data/levels/<room> build/.../data/levels/<room>/asset.bin`
  per room; gbagfx compresses the 34 packed ones, and `data/data.s`
  incbins the results;
- `tools/levels.py c <region> build/.../data/levels/<region>.inc` per
  region, `#include`d by the two `src/data/level_rooms_*.c` files.

`tools/levels.py extract` writes `data/levels/` from `baserom.gba` (and
checks that every asset re-encodes exactly). `tools/levels.py render
data/levels/<room> <layer> out.png` draws a layer with the room's palette
and its tile set (from `graphics/`), or the collision layer as terrain
types, to see what a map holds.

Editing notes: an entity must stay in its column's id order (the list
runs from the last column to the first); a room can't change its layers'
tile sets without the matching palette; `data/data.s`'s incbin lengths of
the packed assets and the region sizes in `ldscript.txt`'s layout are
fixed by what follows them in the ROM, so a change of size needs those
moved too. The room records point at each room's palette and descriptor
by symbol, so they follow the room data wherever it is linked.

## Numbers

41 rooms, 172 layers, 16,323 unique chunks (2.09 M cells; the full
tilemaps are 5.9 M cells), 3,794 entities. Data progress: the room data
(178,208 B) and the 7 raw assets (1,112,392 B + the 2-byte pad before
them stays raw) moved from `baserom.gba` to built sources; the 34 packed
assets were already counted as built when they were `.bin` files.
