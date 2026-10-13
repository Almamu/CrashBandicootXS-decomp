# Level data: rooms, layers, chunk streams, entities

The game has 41 rooms (playable areas). Everything a room needs besides
its tile graphics is built from [`data/levels/`](../data/levels) by
[`tools/levels.py`](../tools/levels.py):

- the room data proper, 178,208 bytes in two ROM regions
  (`0x0824B638`-`0x08270F08`, 33 rooms, and `0x082B91D0`-`0x082BF120`,
  8 rooms): descriptors, chunk grids, palettes and entity lists, as typed
  C in `src/data/level_rooms_24b638.cpp` and `src/data/level_rooms_2b91d0.cpp`;
- the 41 level assets (the per-layer tilemaps, compressed), `gRoomNNAsset`
  in `src/data/level_assets_64f830.cpp`: 7 raw ones at `0x086C127C` and
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
(0x14 bytes). All of it is C in `src/data/level_table_16c814.cpp`, with
one record per room (`gLevelRoom00`..`gLevelRoom40`) plus seven records
of kind 3 for the stages played in an actor category, which have no
room data. The union at +0x10 (`param`) is a stage's actor category
(`param.catIndex`) or a room's colour effect (`param.blend`, which
`SetupRoomBlend` turns into BLDCNT/BLDALPHA: the seven rooms that set it
use alpha blending at EVA 4, EVB 16). The room record is the widget `RunRoom` hands to
`LoadRoom` (`LevelLayers::LoadRoom`, `level_layers.cpp`), which loads a room: it unpacks or
references the asset, feeds each layer its descriptor, feeds the terrain
cache the collision layer, hands the entity list and links to
`SpawnRoomEntities` (`room_entities.c`) and DMAs the palette to BG palette RAM.
The rooms are numbered here in the order of their records
(`room00`..`room40`); the directory names add the descriptor's ROM
offset (`jungle_jam/main` is the room whose `level_desc` is at
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

The source of a BG layer and its streamer (`BgLayerBase::SetSource`,
`BgStreamer`, include/bg_layer.hpp) and the terrain cache's `source`.

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
one of six 4bpp LZ77 tile sets, BG0 one of five 8bpp tag-0x00 tile sets.
No set is a source: the build makes each one from the pictures of the
layers that use it (see "Tile sets" below). Each set belongs to one level
theme, which names it (`tools/levels.py`'s `TILESET_SYMBOLS`):

| Theme | Levels | BG0 | BG1-3 |
|---|---|---|---|
| 0 | snow crash, snow job, frostbite cavern | `gSnowBg0Tiles` | `gSnowBgTiles` |
| 1 | jungle jam, ruined | `gJungleBg0Tiles` | `gJungleBgTiles`, and `gTempleBgTiles` on BG1 of some rooms |
| 1 | temple of boom, down the hole | `gJungleBg0Tiles` | `gTempleBgTiles` |
| 2 | just in slime, just hangin', drip drip drip | `gSewerBg0Tiles` | `gSewerBgTiles` |
| 3 | shipwrecked, shark attack, sunken city, air supply | `gUnderwaterBg0Tiles` | `gUnderwaterBgTiles` |
| 5 | ace of space, star to finish, final countdown | `gSpaceBg0Tiles` | `gSpaceBgTiles` |

The boss rooms borrow a theme's sets: tiny the jungle ones, dingodile the
underwater ones, neo cortex and mega-mix the space ones.

### Cells

Every layer's cells are `u16`:

- **BG1-3**: a BG screen entry (tile 0-9, h/v flip 10-11, palette bank
  12-15), copied straight into the screen block (`DrawBgLayerRow`).
- **BG0**: a source tile id (bits 0-13, into the 8bpp tile set) with the
  h/v flip in bits 14-15. `AcquireTileSlot` (`tile_slot_pool.cpp`) gives the
  tile a VRAM slot and returns the screen entry.
- **Collision**: terrain type in bits 0-7 (types 1-0x23 are the
  non-solid ones `GetTerrainHeights` returns, above that the solid shapes of the
  `gTerrainTypes` table), a flag nibble in bits 8-11, and in bits
  12-15 the per-collision-mode "not solid" bits `GetSolidTerrainHeights`/`GetSolidTerrainModeValue`
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
`dingodile.c`) and per-type words, e.g. `struct entity_params`'
`p.platform` of `CreatePlatform` (mover kind and distances); type 0x1A takes its effective
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
code it gives the object and where the levels place it. Their names are
in `data/levels/entity_types.json` (one record per type: `name`,
`spawner`, and for an enemy its `enemy_kind`), which the build turns into
the `ENTITY_*` and `ENEMY_KIND_*` defines of `constants/entities.h` (see
"Sources and build"):

| Type | Spawner | What |
|---|---|---|
| 0x00, 0x02, 0x04 | `SpawnStartMarker`, `SpawnUnderwaterStartMarker`, `SpawnHoverStartMarker` | the player's start; every room places exactly one: 0x02 in the kind-1 (underwater) rooms, 0x04 in the kind-2 (hover vehicle) room 16, 0x00 in the others. 0x02 and 0x04 forward to `SpawnStartMarker` |
| 0x01, 0x03, 0x05 | `SpawnPlayerPosition`, `SpawnUnderwaterPlayerPosition`, `SpawnHoverPlayerPosition` | set the player's position to the entity's (0x05 is empty); they pair with the start markers by slot (normal, underwater, hover). No level places them |
| 0x06 | `SpawnWumpa` | a wumpa fruit (bank 35) |
| 0x07 | `SpawnCrystal` | the level's crystal (bank 37) |
| 0x08 | `SpawnGemPathGem` | the gem-path clear gem (bank 32, kind 0x1E, level flag bit 2); placed only in the gem-path rooms |
| 0x09, 0x0A, 0x0B, 0x0C | `SpawnBlueGem`, `SpawnRedGem`, `SpawnGreenGem`, `SpawnYellowGem` | the coloured gems (bank 32) |
| 0x10 | `SpawnStopwatch` | the time-trial stopwatch (bank 36) |
| 0x12, 0x13, 0x14, 0x4A | `SpawnTurboRunPower`, `SpawnDoubleJumpPower`, `SpawnBodySlamPower`, `SpawnTornadoSpinPower` | the four power pictures (bank 38); no level places them |
| 0x15-0x27 | `SpawnBasicCrate` .. `SpawnTimeCrate3` | crates: `CreateCrate` kinds 0-18 (bank 31), `CRATE_KIND_*` (`data/levels/crate_kinds.json`) |
| 0x51-0x54 | `SpawnRedGemPlatform`, `SpawnYellowGemPlatform`, `SpawnGreenGemPlatform`, `SpawnBlueGemPlatform` | a gem outline over a platform (bank 32) |
| 0x28 | `SpawnLizard` | enemy, bank 13 |
| 0x29 | `SpawnVulture` | enemy, bank 11 |
| 0x2A | `SpawnVenusFlytrap` | enemy, bank 10 |
| 0x2B | `SpawnPatrollingJungleEnemy` | enemy, bank 14, patrols (`UpdateEnemyPatrol`); jungle rooms 15, 27, 32. Species not identified |
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
| 0x38 | `SpawnStationarySpaceEnemy` | enemy, bank 27 (a shell that shoots out flaming spikes), attacks in place (`UpdateEnemyAttackCycle`); space rooms 30, 33, 36, 37. Species not identified |
| 0x39 | `SpawnPatrollingSpaceEnemy` | enemy, bank 24, patrols and attacks; space rooms 33, 36, 37. Species not identified |
| 0x3A, 0x3F | `SpawnSaucerLabAssistant` | enemy, bank 29 |
| 0x3B | `SpawnPistonCrusher` | hazard, bank 26 |
| 0x3D | `SpawnLaunchPadEntity` | the green launch pad (`SpawnLaunchPad`, bank 28) |
| 0x40 | `SpawnFlamethrowerLabAssistant` | enemy, bank 23 |
| 0x41 | `SpawnHomingSewerEnemy` | enemy, bank 22, follows the player's X within its range (`UpdateEnemyHomingX`); sewer rooms 14, 22, 25, 35. Species not identified |
| 0x42 | `SpawnPatrollingSewerEnemy` | enemy, bank 20, patrols; sewer rooms 14, 25, 35. Species not identified |
| 0x43 | `SpawnRat` | enemy, bank 21 |
| 0x44 | `SpawnFrog` | enemy, bank 19 |
| 0x45 | `SpawnDingodile` | Dingodile (`CreateDingodile`, bank 54) |
| 0x47 | `SpawnTiny` | Tiny Tiger (`CreateTiny`, bank 55) |
| 0x48 | `SpawnCortexBoss` | the Neo Cortex fight's controller (`CreateCortexBoss`, bank 53); only the "neo cortex" level places it |
| 0x49 | `SpawnMegaMix` | Mega-Mix, the boss that chases the player (`UpdateMegaMix`, bank 30); only room 37 (level 24, "mega-mix") places it |
| 0x4B, 0x4C | `SpawnSeaMine` | hazard, bank 6 |
| 0x4D | `SpawnWoodenCrusher` | hazard, bank 18 |
| 0x4E, 0x4F, 0x50 | `SpawnLargePlatform`, `SpawnSmallPlatform`, `SpawnMediumPlatform` | `CreatePlatform` kinds 0-2 (bank 39) |
| 0x55 | `SpawnRoomExit` | the room's exit: an invisible class-0x12 zone (player event 18, `RequestRoomExit`), or in the gem-path/bonus rooms the glowing exit pad (`CreatePlatform` kind 4); every non-boss room but room 16 places one |
| 0x56 | `SpawnBonusPlatform` | the "?" platform (bank 39 anim 5) |
| 0x57 | `SpawnFlipPlatform` | `CreatePlatform` kind 6 (bank 39 anim 6, a green pad that turns red and tips over, then comes back 120 frames later); space rooms 30, 33, 36 and the Neo Cortex room |
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

## Room folders

Each room's folder is `data/levels/<level>/<slot>/`: the level whose
`gLevelRoomLists` entry plays it (named after its name in `levels.json`),
and its slot there:

| slot | `struct level_room_list` | rooms |
|---|---|---|
| `main` | the level's one played room (`rooms[0]`) | 18 |
| `part1`, `part2` | `rooms[0]` and `rooms[2]` of Frostbite Cavern, Snow Crash and Snow Job, around the polar ride (`rooms[1]`, a stage with no room data) | 6 |
| `bonus` | `extra1`, `EnterBonusRoom` | 13 |
| `gem_path` | `extra2`, `EnterGemPathRoom` | 4 |

Rocket Racket, Blimp Bonanza, No-Fly Zone and N. Gin are stages only, so
they have no folder. The rooms keep their `gRoomNN` symbols (room.json's
`"symbols"`), NN being the room's index in the room table and in
`levels.json`'s `"rooms"`. `tools/levels.py extract` names the folders
the same way, from the ROM's `gLevelRoomLists` and the hand-written level
names.

The build takes its rooms from `levels.json`'s `"rooms"` (`levels.py
rooms`), not from the folders on disk.

## Sources and build

`data/levels/`:

- `levels.json`: the rooms (their folders, `"rooms"`, in room-table
  order: `gRoomNN` is the NNth), and per region its C file name and its
  rooms in ROM order, and the rooms' assets in ROM order; and the levels
  in id order (`"levels"`: each one's
  `LEVEL_*` name, name text and world), from which
  `tools/levels.py constants levels` generates `constants/levels.h`. `extract`
  keeps this list, which doesn't come from the ROM's room data.
- `entity_types.json`: the name of each entity type (see "Entity
  types"); `crate_kinds.json`: the name of each crate kind (`crate.kind`).
- one folder per room, `<level>/<slot>/` (see "Room folders" below).
- `<level>/<slot>/room.json`: the asset's symbol and whether it is packed, per layer its tile set, scale, `cnt`, size in
  chunks and tiles and map file, the parameter records (lists of words),
  the links and the entities (in id order; an entity's `type` is its
  name from `entity_types.json`, or a number for an unnamed type), and
  the symbol names of the
  room's objects: `gRoomNN` and what the object is (`gRoom17Desc`,
  `gRoom17Palette`, `gRoom17Bg1Grid`, `gRoom17Asset`, ...; the tile sets
  keep their own names).
- `<level>/<slot>/<layer>.png` (`bg0`-`bg3`): the layer's whole picture,
  8bpp indexed with the room's palette, which it carries: the room's
  `gRoomNNPalette` is read from these pictures (see "BG layers"). 8x8
  pixels per cell,
  `gridWidth * 128` by `gridHeight * 64` pixels (see "BG layers"). The
  build decodes it back to the cells above, which aren't ROM bytes
  either: the ROM only has the chunk streams.
- `<level>/<slot>/collision.png`: the collision layer as an image the
  size of the BG0 picture, each cell an 8x8 block, `gridWidth * 128` by
  `gridHeight * 64` pixels, coloured by the legend below (see "Collision
  layers").

Everything else is derived again: the BG tile sets and the cells (from
the pictures, see "Tile sets"), the chunk set and grid,
the asset and the layers' `assetOffset`s, the entity groups, the counts,
the parameter offsets (`LEVEL_PARAM_OFFSET`, an `offsetof` into the
room's generated parameter struct).

`levels.mk` runs:

- `tools/levels.py tileset <set> --build build/.../data/levels` per BG
  tile set (`tilesets.mk`, which `levels.py tilesets-mk` writes): the
  set's tiles (`tilesets/<set>.bin`, its size in `tilesets/<set>.h`, a
  viewing picture `tilesets/<set>.png`) and the cells of every layer that
  uses it (`<room>/<layer>.map.bin`, little-endian `u16`, row-major),
  each written only if it changed (see "Tile sets");
- `tools/levels.py asset data/levels/<room> build/.../data/levels/<room>/asset.bin --build build/.../data/levels`
  per room, from those cells and `collision.png`; it also writes
  `asset.layout.json`, the layers' asset offsets and chunk grids, which
  the room data takes. gbagfx compresses the 34 packed assets, and
  `tools/bin2c.py` turns each asset into C initializer bytes
  (`asset.bin.inc`, or `asset.bin.lz.inc` trimmed to the LZ77 stream);
- `tools/levels.py assets build/.../data/levels/level_assets_64f830.inc`:
  the 41 asset definitions, in the ROM order of `levels.json`'s
  `"assets"` list, `#include`d by `src/data/level_assets_64f830.cpp`;
- `tools/levels.py c <region> build/.../data/levels/<region>.inc --build build/.../data/levels`
  per region, `#include`d by the two `src/data/level_rooms_*.cpp` files;
- `tools/levels.py constants entities|crates build/include/constants/<name>.h`:
  the constants headers named in this data, `constants/entities.h`
  (`ENTITY_*`, `ENEMY_KIND_*`) and `constants/crates.h` (`CRATE_KIND_*`).
  `build/include` is on the include path and every C object waits for
  them, so code includes them like the hand-written ones. To rename a
  type or a kind, edit the JSON, not the header.

A layer edit rebuilds only its room's map, asset and the room data.

`tools/levels.py extract` writes `data/levels/` from `baserom.gba` (and
checks that every asset re-encodes exactly from the written sources);
extracting again gives the same files byte for byte. `tools/levels.py
render data/levels/<room> <layer> out.png` draws a layer's picture again
from its cells (for the collision layer, the collision image).

### BG layers

Each BG layer is `<layer>.png`, its whole picture: the chunk grid, with
every cell drawn as its tile, flipped as the cell says, 8x8 pixels per
cell. It is 8bpp indexed, and its palette is the room's 256-colour BG
palette (see "The room palette" below). Cell `(cx, cy)` is pixels `cx * 8` to
`cx * 8 + 7` across and `cy * 8` to `cy * 8 + 7` down, and the image is
`gridWidth * 128` by `gridHeight * 64` pixels. BG0's picture is the same
size as `collision.png` and lines up with it (see "Collision layers").
The largest is room 27's BG0, 10240 x 1600 pixels.

**Drawing.** The pixels are palette indices:

- BG0's tile sets are 8bpp, so a pixel is the tile's own colour index.
- BG1-3's tile sets are 4bpp, and a cell
  picks one 16-colour bank (cell bits 12-15). A pixel is
  `bank * 16 + p`, `p` the tile's 4-bit pixel, for all 64 pixels of the
  cell. That includes the transparent ones (`p = 0`), which are index
  `bank * 16`, not 0: that is how the picture keeps a cell's bank even
  where the cell is blank, so every cell can be read back exactly. The
  cost is that a transparent area shows the colour at entry `bank * 16`
  of the room palette. That colour is never drawn in game; the layers
  behind show through there.

**Reading a picture back** (`levels.py tileset`). The pictures are read
together with the tile set they name, which they make (see "Tile sets"):
each 8x8 block becomes the tile it is under one of the four flips, or a
new tile if no tile is it yet. For BG1-3, the block's bank is the high
nibble of its pixels, and the low nibbles are the tile. That gives back
every one of the ROM's 4,356,096 BG cells (131 layers) and its 11 tile
sets, so the ROM still matches.

**Editing.** Open the PNG in an editor that keeps indexed images and
their palette as they are (Aseprite, GIMP in indexed mode, ...). Paint
freely: a block that isn't a tile of the layer's set yet becomes a new
tile at the end of the set (for the ROM's rooms; see "Adding a room" for
new ones). Mind that:

- on BG1-3, all 64 pixels of a block must come from one bank. To move a
  cell to another bank, shift all of its indices, the transparent ones
  too, by a multiple of 16;
- reusing a block that is already a tile, even flipped, costs nothing; a
  new tile makes the set bigger, which moves the ROM's later data (the
  tile limits are 16,384 tiles for BG0, 1,024 for BG1-3), and an early
  room's new tile renumbers the later rooms' tiles;
- don't let the editor reduce, sort or merge the palette (several
  entries of a room palette have the same colour): the indices are
  what is read, not the colours. A true-colour image is rejected.

**The room palette.** The pictures carry the room's palette: the build
reads `gRoomNNPalette` from them (`levels.py c`, `room_palette`), with
`tools/jasc_pal.py`'s rule (a channel's 5-bit value is its 8-bit value /
8; bit 15 is set when red's lowest bit differs from `c * 255 / 31`'s, see
[graphics.md, "Palettes"](./graphics.md#palettes)). So every layer
picture of a room carries the same palette, all 256 entries of it, the
ones no pixel uses too (they are in the ROM). The build fails on a
picture whose palette doesn't have 256 entries, and on one whose palette
differs from the room's other pictures, naming both and the first entry
that differs. To change a room's colours, change the palette of all of
its layer pictures the same way. Rooms that share a tile set may use
different palettes (Jungle BG0 has 3, Temple, Space and Underwater 2):
the pictures store indices, so the palette never changes which tile a
block is.

These are errors, and each message names the PNG, the cell and its pixels:

- in a new room, a block that isn't a tile of a set of the ROM's rooms
  (see "Adding a room");
- a BG1-3 block that mixes palette banks (`... mixes palette banks 2,
  3: a BG1-3 cell draws all 64 pixels from one bank`);
- an image that isn't the layer's size (`... 2040x320 pixels, expected
  2048x320 (16 x 5 chunks of 128x64 pixels)`), or isn't indexed;
- a palette that isn't 256 entries, or isn't the same as the room's other
  layer pictures' (see "The room palette").

The make rule that failed names the tile set too; at most 20 cells are
listed.

### Tile sets

The BG tile sets aren't sources. A room's layer names its set in
room.json (`"tileset"`), and the build makes each set from the pictures
of every layer that names it (`tools/levels.py tileset`, run per set by
`levels.mk`): which rooms share a set follows from the rooms, nothing
declares it. A set named by BG0 layers is 8bpp, one named by BG1-3 layers
4bpp. The tiles come in this order:

1. tile 0, the blank tile;
2. the rooms using the set, one after another: first the ones
   `data/levels/tileset_order.json` lists for it, then the other rooms of
   the ROM's regions in `levels.json` order, then the new rooms (see
   "Adding a room");
3. in a room, its layers using the set: `bg1`, `bg2`, `bg3`, or `bg0`;
4. each picture in chunk order: the 16x8-cell chunks row by row, and
   each chunk's cells row by row. A block that isn't a tile yet under any
   of the four flips becomes the next tile, in the orientation it is
   first seen in.

That gives back the ROM's 11 sets byte for byte (21,297 tiles; no tile
of them is unused, and none is another's flip), and the cells of every
layer.

The order the ROM's sets were made in doesn't follow `levels.json` for
8 of the 11 sets, so `tileset_order.json` gives those rooms, by folder,
32 names in all:

```json
{
  "gJungleBg0Tiles": ["temple_of_boom/main", "jungle_jam/main", ...],
  ...
}
```

It lists only what `levels.json` order doesn't give: the rooms up to the
last one out of that order. `tools/levels.py extract` finds the lists
from the ROM (each set is a run of whole rooms) and checks that the
generator then gives back every set and every cell. A new set, or a new
room of a set, needs no entry.

A set's outputs are written only when they change, and each output
depends on a stamp rather than on the pictures, so touching a picture
remakes its set (a few seconds for the biggest) but rebuilds nothing
else unless a set or a layer's cells really changed. A new tile in an
early room renumbers the tiles of the rooms after it, which then
re-encode. The build also writes `tilesets/<set>.png`, a viewing
picture of the set (16 tiles a row, a BG1-3 tile in the bank it is first
drawn in, the palette of the set's first room).

### Adding a room

A room is a folder `data/levels/<level>/<slot>/` (room.json, the layer
pictures and `collision.png`), and `levels.json`'s `"rooms"` lists it.
The ROM's rooms are those its `"regions"` list; a room in no region is a
**new room**. The build handles a new room through the same `make`, with
no setting:

- its data and its asset go into `src/data/level_appended.cpp`
  (`levels.py appended`), which `ldscript.txt` links after the IWRAM
  image. That is where the ROM's 0xFF fill starts, so the ROM's own data
  doesn't move; past the fill's 106,548 bytes, `rom_fill`'s
  `ALIGN(0x800000)` grows the ROM to the next 8 MB. With the original
  data the section is empty, so the ROM still matches;
- its symbols are room.json's (`"symbols"`, the asset's `"symbol"`, each
  layer's `"grid_symbol"`): give it its own, for example `gRoom41`;
- its layers may name a set of the ROM's rooms, but then only use tiles
  that set already has (a new tile there would move later ROM data): a
  block that isn't one fails the build. To draw new tiles, name a new
  set (`"tileset": "gMyBg0Tiles"`): a set no ROM room uses is made from
  its rooms' pictures alone, in first-use order, and goes with the new
  rooms after the IWRAM image.

The level tables (`gLevelTable`, `gLevelRoomLists`, the `gLevelRoomNN`
records in `src/data/level_table_16c814.cpp`) are still hand-written C:
to play a new room, point a room record at its descriptor and palette
there (#965 is about generating them from `levels.json`; #966 about new
levels and themes). Editing the ROM's own rooms changes their data in
place, which can move the ROM data after it (#967).

### Collision layers

Each room's collision layer is `collision.png`, an 8bpp indexed PNG at
the BG's scale. A collision cell covers one 8x8-pixel tile of the room,
like a BG0 cell (the terrain code looks a cell up at `x >> 3`, `y >> 3`,
`GetTerrainType`), so the image draws each cell as an 8x8 block of one
colour:

- cell `(cx, cy)` is pixels `cx * 8` to `cx * 8 + 7` across and `cy * 8`
  to `cy * 8 + 7` down;
- the image is `gridWidth * 16 * 8` by `gridHeight * 8 * 8` pixels (the
  whole chunk grid, `gridWidth * 128` by `gridHeight * 64`), the same as
  the BG0 picture `bg0.png`.
  The collision and BG0 layers always have the same grid. For example,
  room 00 is 16 x 5 chunks, 256 x 40 cells, and both its BG0 picture and
  `collision.png` are 2048 x 320 pixels;
- `widthTiles`, `heightTiles` (`size_tiles` in `room.json`) are the
  scrollable part of that, also in 8-pixel cells; the rest of the last
  chunk column and row is in the image too.

Every cell value (terrain type, flag nibble and open bits, see "Cells"
above and [flags/terrain.md](./flags/terrain.md)) has its own colour,
listed in `data/levels/collision_legend.json`. All 41 rooms use only the
37 values below. `tools/levels.py` reads the image directly when it
builds the asset and the room data. `tools/collision_bmp.py` converts in
either direction:

    tools/collision_bmp.py to-image MAP.bin OUT.png --width CELLS
    tools/collision_bmp.py to-bin collision.png OUT.bin

An image is read by colour, not by palette index, so an editor can
reorder or extend the palette, or save the image as true colour (PNG or
BMP). Paint with the legend's exact colours, in whole 8x8 blocks on the
8-pixel grid. These are errors, and the message names the cell and its
pixels:

- a colour that isn't in the legend;
- a block that isn't a single colour;
- an image that isn't the room's size (the chunk grid times 128 x 64
  pixels).

To use a value that no room has yet, add it to the legend with a new
colour. The PNG's own palette is the legend's colours in legend order, but
only the pixels' colours matter. `extract` writes the same PNGs again,
byte for byte.

To edit the collision against the level picture:

1. open the BG0 picture, `data/levels/<room>/bg0.png`, in an editor with
   layers (GIMP, Krita, Aseprite, ...) and add `collision.png` as a layer
   above it at offset 0, 0: the two are the same size, so they line up
   pixel for pixel;
2. lower the collision layer's opacity (about 50%) to see the level
   through it, and set a grid of 8x8 pixels to paint on whole cells;
3. paint on the collision layer only, with the pencil (no antialiasing)
   and the legend's colours, then export that layer alone as
   `collision.png`, at full opacity.

The BG1-3 layers scroll at their own parallax scale and have their own
grids, so they don't line up with the collision layer; only BG0 does.

Colour groups:
- black is empty;
- greens are the height types (1-0x23, `GetTerrainHeights`);
- greys are the solid types `0x2B`/`0x2E`;
- oranges are the solid types with bit 7 set (`0x81`-`0x87`, `0xAB`);
- blues, purples, pinks, yellows and cyans are the cells with a flag
  nibble (bits 8-11) or open bits (12-15) set.

| value | colour | cell |
|---|---|---|
| `0000` | `#000000` | TERRAIN_TYPE_NONE |
| `0001` | `#1B5E20` | heights 0x01 |
| `0002` | `#2E7D32` | heights 0x02 |
| `0003` | `#43A047` | heights 0x03 |
| `0005` | `#66BB6A` | heights 0x05 |
| `0006` | `#81C784` | heights 0x06 |
| `0007` | `#A5D6A7` | heights 0x07 |
| `0008` | `#00695C` | heights 0x08 |
| `000A` | `#00897B` | heights 0x0A |
| `000C` | `#26A69A` | heights 0x0C |
| `000D` | `#4DB6AC` | heights 0x0D |
| `000E` | `#80CBC4` | heights 0x0E |
| `000F` | `#B2DFDB` | heights 0x0F |
| `002B` | `#C8C8C8` | solid 0x2B |
| `002E` | `#8C8C8C` | solid 0x2E |
| `0081` | `#BF360C` | solid 0x81 |
| `0082` | `#D84315` | solid 0x82 |
| `0083` | `#E64A19` | solid 0x83 |
| `0085` | `#F4511E` | solid 0x85 |
| `0086` | `#FF7043` | solid 0x86 |
| `0087` | `#FF8A65` | solid 0x87 |
| `00AB` | `#FFB300` | solid 0xAB |
| `0100` | `#1A237E` | TERRAIN_TYPE_NONE, flags 0x1 |
| `012B` | `#5C6BC0` | solid 0x2B, flags 0x1 |
| `01AB` | `#FFD54F` | solid 0xAB, flags 0x1 |
| `0500` | `#4A148C` | TERRAIN_TYPE_NONE, flags 0x5 |
| `0505` | `#9C27B0` | heights 0x05, flags 0x5 |
| `052B` | `#BA68C8` | solid 0x2B, flags 0x5 |
| `0600` | `#880E4F` | TERRAIN_TYPE_NONE, flags 0x6 |
| `062B` | `#F06292` | solid 0x2B, flags 0x6 |
| `072B` | `#F8BBD0` | solid 0x2B, flags 0x7 |
| `0A2B` | `#FFF176` | solid 0x2B, flags 0xA |
| `3000` | `#006064` | TERRAIN_TYPE_NONE, TERRAIN_FLAG_OPEN_RIGHT|TERRAIN_FLAG_OPEN_LEFT |
| `302B` | `#4DD0E1` | solid 0x2B, TERRAIN_FLAG_OPEN_RIGHT|TERRAIN_FLAG_OPEN_LEFT |
| `30AB` | `#FFE082` | solid 0xAB, TERRAIN_FLAG_OPEN_RIGHT|TERRAIN_FLAG_OPEN_LEFT |
| `3500` | `#01579B` | TERRAIN_TYPE_NONE, flags 0x5, TERRAIN_FLAG_OPEN_RIGHT|TERRAIN_FLAG_OPEN_LEFT |
| `352B` | `#4FC3F7` | solid 0x2B, flags 0x5, TERRAIN_FLAG_OPEN_RIGHT|TERRAIN_FLAG_OPEN_LEFT |

Editing notes: an entity must stay in its column's id order (the list
runs from the last column to the first); a room can't change its layers'
tile sets without the matching palette. The assets and the room data
are C arrays linked back to back, so an asset or a room that changes size
moves everything after it (and the ROM no longer matches). The room records point at each room's palette and descriptor
by symbol, so they follow the room data wherever it is linked.

## Numbers

41 rooms, 172 layers, 16,323 unique chunks (2.09 M cells; the full
tilemaps are 5.9 M cells), 3,794 entities. Data progress: the room data
(178,208 B) and the 7 raw assets (1,112,392 B + the 2-byte pad before
them stays raw) moved from `baserom.gba` to built sources; the 34 packed
assets were already counted as built when they were `.bin` files.
