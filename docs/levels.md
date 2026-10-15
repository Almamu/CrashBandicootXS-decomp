# Level data: rooms, layers, chunk streams, entities

The game has 41 rooms (playable areas). Everything a room needs besides
its tile graphics is built from [`data/levels/`](../data/levels) by
[`tools/levels.py`](../tools/levels.py):

- the room data proper, 178,208 bytes in two ROM regions
  (`0x0824B638`-`0x08270F08`, 33 rooms, and `0x082B91D0`-`0x082BF120`,
  8 rooms): descriptors, chunk grids, palettes and entity lists, as typed
  C in `src/game/level/data/level_rooms_main_bonus.cpp` and `src/game/level/data/level_rooms_bosses_gem_paths.cpp`;
- the 41 level assets (the per-layer tilemaps, compressed), `gRoomNNAsset`
  in `src/game/level/data/level_assets.cpp`: 7 raw ones at `0x086C127C` and
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
(0x14 bytes). All of it is in `src/game/level/data/level_table.cpp`, generated
from `levels.json` and the rooms' `room.json` (see "The level table"), with
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

The types are in [`include/game/level/level_data.h`](../include/level_data.h). Each
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
`BgStreamer`, include/game/level/bg_layer.hpp) and the terrain cache's `source`.

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

`data/levels/` (XS's; `tools/levels.py --levels-dir DIR` works on another
game's, such as N-Tranced's `games/n2/data/levels/`, docs/n_tranced.md):

- `levels.json`: the rooms (their folders, `"rooms"`, in room-table
  order: `gRoomNN` is the NNth), and per region its C file name and its
  rooms in ROM order, and the rooms' assets in ROM order; the ROM layout
  of the level table (`"table"`); and the levels in id order
  (`"levels"`): each one's `LEVEL_*` name, name and world, from which
  `tools/levels.py constants levels` generates `constants/levels.h` (the
  ids and the counts, see "The counts the code takes from the levels"),
  its `gLevelTable` fields and the rooms it plays (see "The level
  table"); and the level themes (`"themes"`: each one's `THEME_*` name
  and name, for `constants/themes.h`); and the actor categories
  (`"categories"`, see "Actor categories"). `extract` keeps the names, the
  worlds and the themes, which don't come from the ROM, and writes the
  rest.
- `spawns/<name>.json`: an actor category's spawn list (see "Actor
  categories").
- `entity_types.json`: the name of each entity type (see "Entity
  types"); `crate_kinds.json`: the name of each crate kind (`crate.kind`).
- one folder per room, `<level>/<slot>/` (see "Room folders" below).
- `<level>/<slot>/room.json`: the asset's symbol and whether it is packed,
  the room's record in the level table (`"record"`, see "The level
  table"), per layer its tile set, scale, `cnt`, size in
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
- `tools/levels.py assets build/.../data/levels/level_assets.inc`:
  the 41 asset definitions, in the ROM order of `levels.json`'s
  `"assets"` list, `#include`d by `src/game/level/data/level_assets.cpp`;
- `tools/levels.py c <region> build/.../data/levels/<region>.inc --build build/.../data/levels`
  per region, `#include`d by the two `src/*/data/level_rooms_*.cpp` files;
- `tools/levels.py table levels|records build/.../data/levels/level_table[_rooms].inc`:
  the level table, `#include`d by `src/game/level/data/level_table.cpp` (see
  "The level table");
- `tools/levels.py constants entities|crates|levels|themes build/include/constants/<name>.h`:
  the constants headers named in this data, `constants/entities.h`
  (`ENTITY_*`, `ENEMY_KIND_*`), `constants/crates.h` (`CRATE_KIND_*`),
  `constants/levels.h` (`LEVEL_*` and the counts) and
  `constants/themes.h` (`THEME_*`), each written only if it changed.
  `build/include` is on the include path and every C object waits for
  them, so code includes them like the hand-written ones. To rename a
  type or a kind, edit the JSON, not the header.
- `tools/levels.py classes build/include/rooms.hpp`: the room classes
  (see "Room classes"), written only if it changed.

### Room classes

Each room is a class of the generated `rooms.hpp` (#915), like a graphic
of graphics/manifest.json (graphics.md, "Graphics classes"). Its graphics
are static const data members: the BG palette (`palette`), its layers'
chunk grids (`bg0Grid`, `bg1Grid`, `bg2Grid`, `bg3Grid`,
`collisionGrid`, those of the layers it has) and its level asset
(`asset`):

```cpp
class Room37
{
public:
    static const u16 palette[256];    // gRoom37Palette - the BG palette
    static const u16 bg1Grid[];       // gRoom37Bg1Grid - bg1's chunk grid
    ...
    static const u8 asset[];          // gRoom37Asset - the layers' chunk streams, LZ77
};
```

The class is named after the room's `level_desc` symbol, as its
parameter struct is (`gRoom37Desc`: `Room37`, `struct
level_params_room37`). The classes are in the namespace `Game::Level`
(levels.py's `NAMESPACE`, #1033, docs/cplusplus.md "Namespaces"). The
generated definitions name the members qualified (`const u16
Game::Level::Room37::palette[256] = ...`, the layer descriptors'
`Game::Level::Room37::bg1Grid`, the descriptor's
`Game::Level::Room37::asset`, the room record's
`Game::Level::Room37::palette`), and each keeps its room.json symbol as
its C name: `tools/levels.py symbols` writes cxx_symbols.txt's room
classes block (`_Q34Game5Level6Room37.palette gRoom37Palette`), and `tools/gfx_manifest.py check`
(or `levels.py symbols --check`) fails when it is out of date. A new
room gets its class with the rest of its data; run `levels.py symbols`
to give its members their room.json names.

The other room objects (the descriptor, the layer descriptors, the
entities and their records) stay globals, and so do the BG tile sets,
which the rooms of a theme share.

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
tile at the end of the set. Mind that:

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

### The level table

The level table is generated, like the room data. `src/game/level/data/level_table.cpp`
`#include`s two files that `tools/levels.py table` writes into
`build/.../data/levels/`:

- `level_table.inc` (`table levels`): `gLevelTable` and
  `gLevelRoomLists`, one entry per level of `levels.json`'s `"levels"`,
  in id order, between two tables of the level themes: the colour
  cycles' lists before them and `gThemeMusicCues` after (see
  "Themes");
- `level_table_16c814_rooms.inc` (`table records`): the room records
  (`struct level_room`) and the arrays of the rooms each level plays
  (`gLevelNNRooms`, NN the level id).

The theme tables belong to the level themes, not to the levels: they
come from `levels.json`'s `"themes"` and `"palette_cycles"`, see
"Themes".

Where each field comes from:

| table | field | from |
|---|---|---|
| `gLevelTable` (`struct level_info`) | `nameText` | the level's `"name_text"` |
| | `theme` | `"theme"`: 0 snow, 1 jungle, 2 sewer, 3 underwater, 4 sky, 5 space, 6 tiny, 7 dingodile, 8 n. gin, 9 neo cortex, 10 mega-mix |
| | `times[3]` | `"times"`, in centiseconds, loosest first |
| | `maskAssistDeaths`, `crateAssistDeaths` | `"mask_assist_deaths"`, `"crate_assist_deaths"` |
| | `isBoss` | `"boss"` (1 if true) |
| | `rooms` | always `&gLevelRoomLists[<the level>]` |
| `gLevelRoomLists` (`struct level_room_list`) | `count`, `rooms` | the level's `"rooms"`: its array and the array's length |
| | `extra1`, `extra2` | the records of `"bonus"` and `"gem_path"` (room folders), or 0 |
| `gLevelNNRooms` | the entries | the record of each entry of the level's `"rooms"`: a room folder, or `{"stage": "<NAME>"}` |
| `gLevelRoomNN` (a room's record) | `palette`, `desc` | the room's `gRoomNNPalette`, `gRoomNNDesc` (room.json's `"symbols"`) |
| | `kind` | room.json's `"record"` `"kind"`: `ON_FOOT`, `UNDERWATER` or `HOVER` (`ROOM_KIND_*`) |
| | `unk_0C` | 0 |
| | `param.blend` | `"record"` `"blend"`: `{"effect", "eva", "evb"}` |
| `gLevelStageN` (a stage's record) | all | `{"stage": "<NAME>"}`: kind `ROOM_KIND_CATEGORY`, `param.catIndex` = `CATEGORY_<NAME>` (`constants/categories.h`, from `"categories"`, see "Actor categories"), N its value; no palette or descriptor |

The record's symbol is room.json's `"record"` `"symbol"`
(`gLevelRoomNN`). A level looks like this:

```json
{
  "constant": "LEVEL_SNOW_CRASH",
  "name": "snow crash",
  "world": 1,
  "name_text": 6,
  "theme": 0,
  "times": [1030, 967, 880],
  "mask_assist_deaths": 5,
  "crate_assist_deaths": 5,
  "rooms": ["snow_crash/part1", {"stage": "SNOW_CRASH"}, "snow_crash/part2"],
  "bonus": "snow_crash/bonus",
  "gem_path": "snow_crash/gem_path"
}
```

and a room's record, in its room.json:

```json
"record": {"symbol": "gLevelRoom18", "kind": "UNDERWATER", "blend": {"effect": 1, "eva": 4, "evb": 16}},
```

**The ROM's layout.** `levels.json`'s `"table"` gives the ROM's 25
played-room arrays, in ROM order (`"lists"`: each one's level and
entries). `tools/levels.py extract` writes it, and nobody edits it: it
is the residual that puts the table back in the ROM's order, as
`"regions"` does for the room data. The objects are defined in this
order:

1. the colour cycles' lists, in `"palette_cycles"` order, then
   `gLevelTable` and `gLevelRoomLists`, in level id order;
2. `gThemeMusicCues`, in theme order;
3. the records of the extra rooms: the rooms of the ROM's regions that no
   `"table"` array plays, in `"rooms"` order;
4. the arrays, in `"table"` order (`gLevel24Rooms`, mega-mix's, comes
   after `gLevel19Rooms`, and `gLevel21Rooms`, n. gin's, last);
5. the records of the rooms and stages those arrays play, each the first
   time an array plays it.

`extract` checks that this gives the ROM's objects back to back at their
ROM addresses, and `make compare` that it gives the ROM's bytes.

**New data goes in place.** The table is generated from the levels as
they are now, in the layout above:

- a field of a level or of a room's record changes in place;
- a level plays the rooms of its `"rooms"`: its `gLevelNNRooms` holds
  them, whatever the ROM's array held, and grows or shrinks in place;
- a new room's record is one more record (an extra room's with the extra
  rooms, a played room's where an array first plays it), and the arrays
  and the `extra1`/`extra2` pointers that name it are pointers like any
  other;
- a new level (one `"table"` doesn't list) adds an entry to `gLevelTable`
  and `gLevelRoomLists`, and its array comes after the ROM's. Its id is
  its place in `"levels"`, which the code's arithmetic needs in a set
  order (see "The counts the code takes from the levels"). Its name is
  the UI string `"name_text"`: a new `ui.level.<level>` entry after the
  last `ui.*` one in `text/crashxs.pot`, with its translations in the
  five `.po` files ([text.md, "A new level's name"](./text.md#a-new-levels-name)),
  which `make text-check` requires;
- a stage of a new actor category (see "Actor categories") is one more
  `gLevelStageN` record, where an array first plays it, like a new
  room's.

Whatever grows moves the ROM data linked after it, which is safe: every
address the ROM stores is a relocation ([shifting.md](./shifting.md),
#967).

### The counts the code takes from the levels

The code never spells a level count as a number: it uses the macros of
`constants/levels.h` and `constants/themes.h`, which `tools/levels.py
constants levels|themes` derives from `levels.json` (#966). Each one is
a rule over the data, so a new level changes the code's numbers with
it; with the ROM's levels each rule gives the ROM's number, and
`levels.py extract` checks that it does (`ROM_COUNTS`).

| macro | ROM | rule | used by |
|---|---|---|---|
| `LEVEL_COUNT` | 25 | the levels | `gLevelTable`, `GameProgress::levels`, the save slots |
| `WORLD_COUNT` | 4 | the numbered levels' worlds | the level select's pages (`UpdatePageArrows`), its per-world tables (`"worlds"`, see "Level-select pages") |
| `LEVELS_PER_WORLD` | 5 | the most numbered levels a world has: 5, the page's layout (1 to 5 per world, see "Partial pages") | the level select's `world * LEVELS_PER_WORLD + index` (with full pages) |
| `LEVEL_NUMBERED_COUNT` | 20 | the numbered levels | the relic, gem and crystal counts (`GameProgress`) |
| `LEVEL_LAST_NUMBERED`, `LEVEL_FIRST_BOSS` | 19, 20 | the last numbered level, the first boss | the level select, the pause menu |
| `LEVEL_LAST_ON_GRID` | `LEVEL_NEO_CORTEX` | the last boss | `UpdateGameFrame`'s level-select cap |
| `LEVEL_FINALE` | `LEVEL_MEGA_MIX` | the first level off the grid | `CountGems`, `CountClearGems` (Neo Cortex's step on to mega-mix is its `"on_clear"`, see "Boss levels") |
| `LEVEL_SELECT_ENTRIES` | 6 | `LEVELS_PER_WORLD` + the boss: a full page | `LevelSelect::items`, the entry positions |
| `CRYSTAL_TOTAL`, `RELIC_TOTAL` | 20 | the numbered levels | the pause menu |
| `GEM_TOTAL` | 28 | a crate gem per numbered level, the `GEM_PATH_GEM` entities in the rooms of the numbered levels and the finale, and the 4 coloured gems | the pause menu |
| `COMPLETION_TOTAL` | 72 | the three totals and the 4 powers | `GetCompletionPercent` |
| `UI_TEXT_COUNT` | 70 | 70, or one past the highest `"name_text"` | the `gUiText*` tables |
| `THEME_COUNT` | 11 | `"themes"` | `gThemeMusicCues` |
| `ZOOM_BG_NO_PICTURE` | 11 | `THEME_COUNT` | `ZoomBg::image` |
| `LEVEL_SELECT_PICTURE_COUNT` | 10 | up to the last theme with a `"picture"` | `gLevelSelectPictures` |

The rules need the levels in the order the code finds them by id: the
numbered levels world by world, 1 to 5 in each and 5 in the largest;
then the bosses (`"boss": true` and a `"world"`), at most one per world,
in world order; then the levels off the level select's grid (`"boss": true`, no
`"world"`), mega-mix first. `levels.py` stops with a message when the
data breaks that, when a total needs three digits (the pause menu prints
two), or past 60 levels (the 64 Kbit save's largest record; see "The
save"). Past 28 levels the build takes the 64 Kbit EEPROM by itself.
`PROGRESS_EXTRA_FLAG_BYTES` (0 in the ROM) is the length of
`GameProgress::extraFlags`, from `"extra_progress_flags"` (see "The
save"). `levels.json`'s `"themes"` names each theme
(`THEME_*`); a level's `"theme"` is its number (see "Themes").

### Adding a room

A room is a folder `data/levels/<level>/<slot>/` (room.json, the layer
pictures and `collision.png`), and `levels.json`'s `"rooms"` lists it.
The ROM's rooms are those its `"regions"` list; a room in no region is a
**new room**. Everything about it is data, and a plain `make` builds it:

1. **Make the folder.** The quickest start is a copy of a room of the
   same theme, `cp -r data/levels/snow_crash/part1 data/levels/jungle_jam/extra`.
   Then edit the pictures, `collision.png` and the entities in room.json.
2. **Give it its own symbols.** Every name in room.json is a symbol:
   `"symbols"`, the asset's `"symbol"`, each layer's `"grid_symbol"` and
   the record's `"symbol"`. For a copy of room 23, rename `gRoom23` to
   `gRoom41` and `gLevelRoom23` to `gLevelRoom41`:
   `sed -i 's/gRoom23/gRoom41/g; s/gLevelRoom23/gLevelRoom41/g' data/levels/jungle_jam/extra/room.json`.
   Set the record's `"kind"` (`ON_FOOT`, `UNDERWATER` or `HOVER`, which
   picks the player's controller) and `"blend"`. The room's class follows
   its descriptor's symbol (`gRoom41Desc`: `Room41`, see "Room classes").
3. **List it** at the end of `levels.json`'s `"rooms"`.
4. **Play it.** Add it to a level in `"levels"`: to `"rooms"`, in play
   order (`["jungle_jam/extra", "jungle_jam/main"]` plays it first), or
   as the level's `"bonus"` or `"gem_path"` room.
5. **Run `make`.** `tools/levels.py symbols` gives its class's members
   their room.json names in cxx_symbols.txt (`gfx_manifest.py check`
   asks for it); the build works without, under their C++ names.

The build generates the new room with the ROM's, in place: its data
after the rooms of the last region (`level_rooms_bosses_gem_paths.cpp`), its asset
after the ROM's assets (`level_assets.cpp`), its record and the
level's array in the level table. With the original data there is no new
room, so the ROM still matches. Everything linked after a grown table
moves ([shifting.md](./shifting.md)); past the end of the ROM's 0xFF
fill, `rom_fill`'s `ALIGN(0x800000)` grows the ROM to the next 8 MB.

The steps above, with jungle jam playing the copy first, shipwrecked
taking it as its bonus room and one new 8x8 block painted into the
copy's `bg0.png`, still give an 8 MB ROM. `gLevelRoom41` and the grown
`gLevel00Rooms` add 0x18 bytes to the level table and the new tile 0x40
bytes to `gSnowBg0Tiles`; the room's data (0x9C0 bytes) follows the last
region's rooms and its LZ77-packed asset (0x4709 bytes) the ROM's
assets. The ROM data after each moves, which the relocations
`make shiftcheck` checks keep working. In mGBA, a new game's jungle jam loads `gLevelRoom41`
where the original loads `gLevelRoom17` (`tools/boottest.py`).

A room's layers may name any tile set. A block that isn't a tile of the
set yet becomes a new one at the end of the set, after the tiles of the
ROM's rooms, so the ROM's rooms keep their cells. To draw on a set of
their own, name a new set (`"tileset": "gMyBg0Tiles"`): a set no ROM room
uses is made from its rooms' pictures alone, in first-use order, and
goes after the last region's rooms, before the new rooms.

Editing the ROM's own rooms changes their data in place, which moves
the ROM data after it; that is safe ([shifting.md](./shifting.md)).
A room of a new theme is a room like any other; the theme is the
level's (see "Adding a theme"). A new stage is a new actor category (see
"Actor categories").

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

## Themes

A level's `"theme"` (`struct level_info.theme`) picks everything the
game does by theme: its music, its level-select picture, the colour
cycle that runs on its BG palette and how its first room exits. All of
it is data, `levels.json`'s `"themes"` (one per theme, in theme order)
and `"palette_cycles"`, and a plain `make` builds whatever is there; with
the ROM's themes everything comes out as the ROM's, which `make compare`
checks. `tools/levels.py extract` reads the songs, the pictures and the
cycles' palette entries back from the ROM and keeps the rest: the
names, and what is code in the ROM (which cycle a theme picks and the
cycles' rates and directions, RunRoom's; the exit zone, SpawnRoomExit's).

### What the game picks by theme

| what | where | from |
|---|---|---|
| the music | `gThemeMusicCues[theme]`, a song id, in `PlayRoomMusic` (unless the bonus round's or the invincibility drums play) | the theme's `"song"` |
| the level-select picture | `gLevelSelectPictures[theme]`, the `{palette, tiles}` pair `ZoomBg::SetPicture` loads for the level under the cursor | the theme's `"picture"` |
| the colour cycle | `RunRoom`'s `switch` on the theme at a level's start: `AddPaletteCycle` on the BG palette with each of the cycle's lists, or `gPaletteCycles->active = 0` | the theme's `"palette_cycle"`, one of `"palette_cycles"` |
| the first room's exit | `SpawnRoomExit`: in a level's first room a plain 0x64 x 0x64 exit zone instead of the exit pad (the snow levels', whose first room leads into the polar ride) | the theme's `"first_room_exit_zone"` |
| the count | `THEME_COUNT` (`gThemeMusicCues`' length), `ZOOM_BG_NO_PICTURE`, `LEVEL_SELECT_PICTURE_COUNT` | the themes |

That is every use of the theme in the code (`level_info.theme` is read
by these four functions and the level select, which passes it to
`ZoomBg::SetPicture`).

### The data

```json
"themes": [
  {"constant": "THEME_SNOW", "name": "snow", "song": "arctic", "picture": "level_select/pictures/snow.png", "first_room_exit_zone": true},
  {"constant": "THEME_JUNGLE", "name": "jungle", "song": "jungle", "picture": "level_select/pictures/jungle.png", "palette_cycle": "jungle"},
  ...
  {"constant": "THEME_MEGA_MIX", "name": "mega-mix", "song": "neo_cortex"}
],
"palette_cycles": [
  {
    "name": "sewer",
    "lists": [
      {"symbol": "gThemePaletteCycle2", "rate": 6, "direction": 1, "entries": [177, 178, 179, 180, 181]}
    ]
  },
  ...
]
```

A theme has:

- `"constant"` and `"name"`: its `THEME_*` constant (`constants/themes.h`)
  and what it is;
- `"song"`: a song of `sound/gax_manifest.json`'s `song_table`, by name
  (`SONG_<NAME>`, `constants/songs.h`);
- `"picture"` (optional): a level-select picture, by its path under
  `graphics/` (`level_select/pictures/*.png`, an entry of
  `graphics/manifest.json` with a graphics class: one of the ROM's ten, or
  a new one, an entry with no ROM address, [graphics.md, "New
  graphics"](./graphics.md#new-graphics)). A theme with none
  behaves like mega-mix's: it has no entry of `gLevelSelectPictures`
  (before the last theme with a picture, an empty `{0, 0}` one), so its
  levels can't be on the level select's grid. `levels.py` stops when a
  level with a `"world"` has such a theme: `ZoomBg::SetPicture` would load
  whatever follows the table;
- `"palette_cycle"` (optional): the name of one of `"palette_cycles"`. A
  theme with none stops the cycles (RunRoom's `default`);
- `"first_room_exit_zone"` (optional, `true`): the exit zone above.

A cycle of `"palette_cycles"` has a `"name"` and its `"lists"`, each a
`u16` array of the BG palette entries it cycles (`"symbol"` and
`"entries"`) with the `AddPaletteCycle` `"rate"` (frames per step) and
`"direction"` (0 or 1). The cycles are in ROM order, which is also the
order of RunRoom's cases.

### What is generated

| output | what | used by |
|---|---|---|
| `level_table.inc` (`levels.py table levels`) | the cycles' lists, before `gLevelTable`, and `gThemeMusicCues[THEME_COUNT]`, after `gLevelRoomLists` | `level_table.cpp` |
| `level_select_pictures.inc` (`levels.py table pictures`) | `gLevelSelectPictures[LEVEL_SELECT_PICTURE_COUNT]`, the pictures' graphics class members (`Engine::Gfx::LevelSelectPicture0::palette`, ...) | `level_select_picture_table.cpp` |
| `theme_palette_cycles.h` (`levels.py palette-cycles`) | the lists' declarations and `THEME_PALETTE_CYCLE_CASES`: per cycle a case labelled with the themes that pick it, `gPaletteCycles->Clear()` and a `FX_CYCLE(list, rate, count, direction)` per list | `RunRoom` (`play_room.cpp`) |
| `constants/themes.h` (`levels.py constants themes`) | `THEME_*`, `THEME_COUNT`, `ZOOM_BG_NO_PICTURE`, `LEVEL_SELECT_PICTURE_COUNT` and `THEME_FIRST_ROOM_EXIT_ZONE(theme)`, the test of the themes with `"first_room_exit_zone"` | everything that names a theme |

With the ROM's themes RunRoom's cases are the ROM's (sewer; jungle and
tiny; underwater; space; then the default), and `THEME_FIRST_ROOM_EXIT_ZONE(theme)`
is `((theme) == THEME_SNOW)`: the same code. No `#if`: a different set of
themes changes the tables and the cases with it.

### The boss themes' music

The ROM's `gThemeMusicCues` is `02 00 03 01 05 04 07 08 09 0A 0A`. Each
song's name is the title the ROM stores with it, right after its
patterns (`"tiny" © Manfred Linzner` at `0x0859D74A`, before the song at
`0x0859DB2C`, `gSongTable[9]`; `"dingodile"` at `0x0859E76A` before
`0x0859EBB0`, `gSongTable[7]`; `"n gin"` at `0x0859EFA1` before
`0x0859F394`, `gSongTable[8]`). The themes are named after their levels
and pictures: theme 6 is `LEVEL_TINY`'s (its picture is Tiny's arena),
7 `LEVEL_DINGODILE`'s (Dingodile in his bubble), 8 `LEVEL_N_GIN`'s (his
airship). So:

| theme | level | plays | its title |
|---|---|---|---|
| 6, `THEME_TINY` | Tiny | song 7, `SONG_DINGODILE` | `dingodile` |
| 7, `THEME_DINGODILE` | Dingodile | song 8, `SONG_N_GIN` | `n gin` |
| 8, `THEME_N_GIN` | N. Gin | song 9, `SONG_TINY` | `tiny` |

Each of the three bosses plays the song titled for another one. Both
names are right: the songs keep the ROM's titles and the themes their
levels', and `levels.json` says which song each theme plays
(`THEME_TINY`'s `"song"` is `"dingodile"`). It looks like a slip in the
original: the song table and the boss levels (20, 21, 22) are in Crash
XS's boss order, Dingodile, N. Gin, Tiny, while themes 6-8 are in Crash
3's, Tiny, Dingodile, N. Gin, and the cue table maps them by position
(theme 6 + k to song 7 + k). The ROM can't say which song was written
for which boss, only which one plays.

### Adding a theme

A new theme is data: add it to the end of `"themes"` and give levels
its number.

1. **Add the theme** at the end of `"themes"` (its number is its place):
   `{"constant": "THEME_LAB", "name": "lab", "song": "intro",
   "picture": "level_select/pictures/space.png", "palette_cycle": "lab"}`.
   It picks one of the songs (a new one is an XM, [audio.md, "Adding a
   song"](./audio.md#adding-a-song)) and one of the
   level-select pictures, or none (then its levels must be off the grid,
   like mega-mix). A picture of its own is a new PNG in
   `graphics/level_select/pictures/` with a manifest entry without a ROM
   address ([graphics.md, "New graphics"](./graphics.md#new-graphics)).
2. **Give it a colour cycle**, an existing one by name, or a new one at
   the end of `"palette_cycles"`: `{"name": "lab", "lists": [{"symbol":
   "gThemePaletteCycleLab", "rate": 8, "direction": 1, "entries": [16, 17,
   18, 19]}]}`. Or none.
3. **Use it**: a level's `"theme"` is its number (`11` for the 12th).
4. **Run `make`.** The theme tables grow to the new count, RunRoom gets
   the new case and everything linked after them moves
   ([shifting.md](./shifting.md)).

A theme doesn't change the save data's meaning, so it isn't part of the
save version's CRC; the level that uses it is.

Tested in scratch with plain `make`: the theme above (the intro's song,
space's picture, a new cycle of 4 entries) and a 26th level using it
(`LEVEL_LAB`, a boss off the grid, playing `jungle_jam/main`). The ROM
stays 8 MB; `THEME_COUNT` and `LEVEL_SELECT_PICTURE_COUNT` are 12;
`gThemeMusicCues` is 0xC bytes and `gLevelSelectPictures` 0x60 (12
pairs, mega-mix's `{0, 0}` and the new theme's space picture); the new
list `gThemePaletteCycleLab` (8 bytes) comes before `gLevelTable`, and
RunRoom's literal pool names it. A second `make` does nothing. In mGBA
(`tools/boottest.py`, a `level25` script) the level loads
`gLevelRoom17` and plays song 15 (`SONG_INTRO`), the theme's.

A theme with a picture of its own, tested in scratch the same way:
`lab.png`, snow's picture mirrored with its red and blue swapped, a
manifest entry with no ROM address (`LevelSelectPictureLab`, object
`level_select_graphics`), a 12th theme showing it and jungle jam given
that theme. Plain `make` builds it: the picture's palette and tiles
follow the ROM's pictures, and `gLevelSelectPictures` gets a 12th pair
naming them. In mGBA the level select's zoom picture for jungle jam is the
new one: BG2's character block holds `lab.png`'s tiles byte for byte, and
the BG palette its colours.

## Level-select pages

How the level select pages through the worlds and opens them, and how a
new world (a page) is added (#966, phase 4).

**A page is a world.** `LevelSelect` (`level_select.cpp`,
`level_select_pages.cpp`) shows one world at a time: its
`LEVELS_PER_WORLD` numbered levels (`world * LEVELS_PER_WORLD + index`)
and its boss (`LEVEL_FIRST_BOSS + world`), `LEVEL_SELECT_ENTRIES`
entries; or fewer, on a partial page (see "Partial pages"). Up turns to the next page (`NextWorld`), Down to the previous
one (`PrevWorld`); the platform (BG1, `LevelSelectPageBg`) turns 0x100
pixels a page and wraps, so it has no page count. Left and Right move
the cursor on the page. The level select opens on the page of the last
level played (`room.level`, capped at `LEVEL_LAST_ON_GRID`).

**What opens what.**

| what | opens when | where |
|---|---|---|
| a world's numbered levels | always, once the world is open | `PlaceItems` |
| a world's boss (the sixth entry) | all `LEVELS_PER_WORLD` levels of the world are cleared (`levels[k].cleared`); the entries then take `gLevelSelectEntryPositionsAllCleared`. On a partial page, the entry after its levels, once they are all cleared, if the world has a boss | `PlaceItems` |
| world 1, 2, 3 | the power of the boss before: Super Body Slam (Dingodile), Double Jump (N. Gin), Tornado Spin (Tiny), `PROGRESS_BIT_*` of `save->flags` | `IsNextWorldOpen`'s `switch (world)`, cases 0-2 |
| (no world after the last) | `UpdatePageArrows` draws no next arrow on page `WORLD_COUNT - 1`; `IsNextWorldOpen` returns 0 there (no case) | |
| mega-mix (`LEVEL_FINALE`, off the grid) | beating Neo Cortex at over 99% completion: `UpdateGameFrame` plays it straight after | `game_frame.cpp` |

Beating a boss for the first time gives its power and calls
`LevelSelect::SetNewWorldOpened`: the next level select turns to the
new page by itself (`Loop`), if `IsNextWorldOpen` says it is open (the
level's `"on_clear"`, see "Boss levels"). Neo Cortex's power (Turbo Run)
opens no page in the ROM. The eight bits of `save->flags`
(`GameProgress` byte 2) are all used: the four coloured gems and the
four powers. A ROM boss's level word gets no `cleared` bit: its
`"on_clear"` gives the power instead.

### The worlds

`levels.json`'s `"worlds"` has one entry per world (the worlds of the
numbered levels, `WORLD_COUNT`), in world order: what its page shows and
what opens it.

```json
"worlds": [
  {"page_title": 9, "entry_box": 0, "boss_icon": 2},
  {"page_title": 8, "entry_box": 1, "boss_icon": 3, "opened_by": {"power": "SUPER_BODY_SLAM"}},
  ...
]
```

| key | what | the table | ROM |
|---|---|---|---|
| `"page_title"` | the page title sprite's animation (bank 47, 0-12) | `gLevelSelectWorldAnims` | 9, 8, 6, 7 |
| `"entry_box"` | the entries' box colour, an index of `gLevelSelectEntryBoxAnims` (bank 49; 0-4, 4 unused by the ROM) | `gLevelSelectWorldEntryBoxAnims` | 0, 1, 2, 3 |
| `"boss_icon"` | the boss entry's icon animation (bank 51, 0-4) | `gLevelSelectEntryWorldAnims` | 2, 3, 1, 4 |
| `"opened_by"` | every world but the first: what opens it, the case of `IsNextWorldOpen` for the world before it. `{"power": NAME}`, a power of `save->flags` (`TURBO_RUN`, `SUPER_BODY_SLAM`, `TORNADO_SPIN` or `DOUBLE_JUMP`, `PROGRESS_BIT_<NAME>`), or `{"levels_cleared": true}`, every numbered level of the world before cleared (`LevelSelect::IsWorldCleared`) | `LEVEL_SELECT_NEXT_WORLD_CASES` | the three powers above |

`tools/levels.py worlds` writes `level_select_worlds.h` (build/include)
from it: the three tables' initializers (`LEVEL_SELECT_WORLD_ANIMS`,
`LEVEL_SELECT_WORLD_ENTRY_BOX_ANIMS`, `LEVEL_SELECT_ENTRY_WORLD_ANIMS`,
which `level_select_layout.cpp` and `level_select_anims.cpp` define the
tables with) and `IsNextWorldOpen`'s cases
(`LEVEL_SELECT_NEXT_WORLD_CASES`, like RunRoom's
`THEME_PALETTE_CYCLE_CASES`). With the ROM's worlds they are the ROM's
values and its three cases, so the bytes don't change. It stops when
`"worlds"` doesn't have one entry per world, a value is out of range, a
rule isn't one of the two, or bank 51 has no number icon for a numbered
level (below). `levels.py extract` reads the three tables back from the
ROM and keeps the rules (code, like the colour cycles' rates), and checks
that they are the ROM's. The worlds aren't part of the save version's
CRC: they don't change what a save's records mean.

**The number icons.** A numbered level's entry shows step `level id` of
bank 51's animation 0 (`LevelSelectEntry::SetLevel`). In the ROM its 20
steps are frames 0-19, the numbers 1-20 (frames 20-23 are the bosses'
heads, the boss icons' animations 1-4). A level past the 20th needs a
number: a new frame of bank 51, appended to animation 0 ([graphics.md,
"New frames"](./graphics.md#new-frames)). `levels.py worlds` stops if
there are fewer steps than numbered levels.

### Adding a world

1. **The levels.** `LEVELS_PER_WORLD` numbered levels with the new
   `"world"` (or 1 to 4, a partial page, see below), after the last
   world's numbered levels, and its boss (`"boss": true` and the
   `"world"`; or none) after the last boss, before mega-mix (see "The counts the code takes from the levels"), each
   with its name ([text.md, "A new level's name"](./text.md#a-new-levels-name)).
   The level ids after them move, which only changes the generated
   constants. More than 28 levels take the 64 Kbit save ("The save").
2. **The world.** An entry at the end of `"worlds"`: the page title,
   box and boss icon (the boss icon only for a world with a boss; the
   ROM's animations: new art is new frames of banks 47, 49 or 51, which
   their C would have to take, like bank 51's animation 0 below), and
   `"opened_by"`.
3. **The number icons.** For each numbered level past the 20th, a new
   frame of bank 51 and a step of its animation 0 (graphics.md, "New
   frames").
4. **Run `make`.**

**Defaults for a new world** (the owner's decisions on #966, and the
ROM's behaviour where they say nothing):

- A new world comes after Neo Cortex's page, as post-game content, like
  mega-mix. The ending is the ROM's: Neo Cortex plays the ending
  cutscene and the credits, or mega-mix at over 99% (its `"on_clear"`'s
  `"next_level"`: `UpdateGameFrame` goes `LEVEL_MEGA_MIX -
  LEVEL_NEO_CORTEX` levels on, 1 in the ROM, past the new bosses
  otherwise).
- A world after Neo Cortex's opens when every numbered level of the
  world before is cleared (`{"levels_cleared": true}`). There is no free
  bit in `save->flags` for a new boss's win, and the four powers are
  the ROM's bosses' (a power is given by one level), so the rule the
  boss entry already uses is the one that needs no new save data. Neo Cortex's
  first win calls `SetNewWorldOpened` as in the ROM, so if the world
  after it is open then, the next level select turns to it by itself.
- Its boss entry appears once its levels are cleared, like the ROM's
  (`PlaceItems` doesn't change). Beating a new boss does what its
  `"on_clear"` says (see "Boss levels"); without one it takes
  `UpdateGameFrame`'s default case: no power, dialog or cutscene.
- The completion percentage counts the new levels' crystals, gems and
  relics (`COMPLETION_TOTAL`), so mega-mix (over 99%) needs them too.
- After mega-mix the level select opens on the last page's boss
  (`LEVEL_LAST_ON_GRID`, as in the ROM, where that is Neo Cortex).

Tested in scratch with a fifth world: five numbered levels (ids 20-24,
"crash cove" to "slime pit", world 0's rooms), its boss "dingo rematch"
after Neo Cortex (Dingodile's room), world 0's title, box and boss icon,
`{"levels_cleared": true}`, and bank 51's frames 24-28 ("21" to "25",
the "2" of frame 1 and the digits of frames 0-4) as animation 0's steps
20-24: 31 levels and the 64 Kbit save. Plain `make` builds it and a
second `make` does nothing. In mGBA, with the four powers and worlds
0-2's levels cleared poked into the level select's save block, Up turns
to page 4 (world 3), whose next arrow shows the locked frame; Up again
plays the error sound and stays. With world 3's levels cleared the arrow
lights and Up turns to page 5: world 0's title (Dingodile's head), red
boxes, the icons 21-25 and no next arrow. With its levels cleared the
boss entry (Dingodile's head) appears, and A on "21" starts level 20
(crash cove, jungle jam's first room). `levels.py worlds` stops with a
missing world, a bad rule, a rule on world 0 and too few number icons.

### Partial pages

A world can have fewer than `LEVELS_PER_WORLD` numbered levels (1 to 4)
or no boss (#966, phase 6), so the level count needn't be a multiple of
six: 27 levels are the ROM's 25 and a world of 2 levels with no boss,
which still fits the 4 Kbit save. The world's levels and boss are where
"The counts the code takes from the levels" puts them: its numbered
levels after the world before's, its boss (if any) in world order among
the bosses.

**The code.** The ROM finds a page's levels by arithmetic, and
`level_select_worlds.h` spells each piece of it as a macro, which the
level select uses instead:

| macro | full pages (the ROM's code) | a partial page |
|---|---|---|
| `LEVEL_SELECT_WORLD_FIRST(world)` | `world * LEVELS_PER_WORLD` | `gLevelSelectWorldFirstLevels[world]` |
| `LEVEL_SELECT_WORLD_LEVELS(world)` | `LEVELS_PER_WORLD` | `gLevelSelectWorldLevelCounts[world]` |
| `LEVEL_SELECT_WORLD_BOSS(world)` | `world + LEVEL_FIRST_BOSS` | `gLevelSelectWorldBosses[world]` (`LEVEL_SELECT_NO_BOSS`, 0xFF, for none) |
| `LEVEL_SELECT_BOSS_SHOWN(world, cleared)` | `cleared == LEVELS_PER_WORLD` | all the world's levels cleared, and it has a boss |
| `LEVEL_SELECT_WORLD_OF_LEVEL(level)`, `LEVEL_SELECT_INDEX_OF_LEVEL(level, world)` | `level / 5`, `level % 5` (`__divsi3`, `__modsi3`) | `gLevelSelectLevelWorlds[level]`, `level` - the world's first |
| `LEVEL_SELECT_WORLD_OF_BOSS(level)` | `level - LEVEL_FIRST_BOSS` | `gLevelSelectLevelWorlds[level]` |
| `LEVEL_SELECT_POSITIONS(world)`, `_ALL_CLEARED(world)` | `gLevelSelectEntryPositions`, `gLevelSelectEntryPositionsAllCleared` | `gLevelSelectWorldEntryPositions[world]`, `...AllCleared[world]` |

`tools/levels.py worlds` writes the left column when every page is full
(every world has `LEVELS_PER_WORLD` levels and a boss), as with the ROM's
levels, so the code is the ROM's, byte for byte; and the right column
otherwise. `LEVEL_SELECT_PAGE_TABLES`, which `level_select_layout.cpp`
expands after `gLevelSelectPalette`, defines the tables: nothing with
full pages, and with a partial page the per-world first levels, counts
and bosses, `gLevelSelectLevelWorlds` (the world of each level on the
grid, `LEVEL_SELECT_GRID_LEVELS`), the layouts the pages need and the two
per-world layout pointers. No `#if`: the generator writes one or the
other, like `THEME_FIRST_ROOM_EXIT_ZONE`.

**What the level select does with it.**

- The cursor: `lastIndex` is the page's last shown entry, its levels'
  count - 1, or the count once the boss entry shows. `CursorLeft` and
  `CursorRight` stop at 0 and `lastIndex` (the error sound past them),
  `RefreshPage` clamps the cursor to it after a page turn, and `Update`
  draws (`Animate`) only the entries up to it, so the empty slots never
  show and the cursor never lands on one.
- The entries: `SetLevel` gives entry `index` the world's level
  `first + index` while `index` is below the count, and the boss past it.
  The level's record panel (`LoadRecord`, `DrawRecord`) shows for the
  numbered entries, `index` below the count.
- The layouts: a page of 5 levels takes the ROM's two
  (`gLevelSelectEntryPositions`, its sixth slot hidden, and
  `gLevelSelectEntryPositionsAllCleared`, 6 entries). A page of `k` < 5
  entries shown takes `gLevelSelectEntryPositions<k>`: `k` entries 50
  pixels apart, centred on the ROM's arc (x 120, y 136 in the middle and
  16 higher 90 pixels out, the ROM's ends), then `{0, 0}` up to
  `LEVEL_SELECT_ENTRIES`. A page of 2 levels has 2 entries, then 3 with
  its boss.
- The boss entry shows once all the world's levels are cleared, if it
  has a boss (`PlaceItems`). A world with no boss never shows one.
- The `{"levels_cleared": true}` rule (`IsWorldCleared`) checks the
  world's own levels, so the world after a partial one opens when the
  partial one's levels are cleared.
- The level select opens on the page of the level played: its world and
  entry from `gLevelSelectLevelWorlds` (a boss is the entry after its
  world's levels).
- The completion percentage, the pause menu's totals and the save count
  the levels, not the pages (`LEVEL_NUMBERED_COUNT`, `COMPLETION_TOTAL`),
  so they need nothing more.

`levels.py` stops when a world has no numbered level or more than 5,
when no world has 5 (the ROM's layouts are a full page's), when the
bosses aren't in world order or a world has two, and when a world with a
boss has no `"boss_icon"` (a world with none may leave it out).

Tested in scratch with plain `make` and mGBA, with a fifth world of two
numbered levels ("crash cove" and "boulder bay", ids 20-21, world 0's
rooms), `{"levels_cleared": true}`, world 0's title and box, and bank
51's frames 24-25 ("21", "22"):

- with no boss, 27 levels on the 4 Kbit save. With the powers and
  worlds 0-2 cleared poked into the save block, Up stays on page 4 (the
  error sound) until world 3's levels are cleared, then turns to page 5:
  the two entries "21" and "22" centred on the arc, `lastIndex` 1, no
  next arrow. Right moves to "22" and then plays the error sound, as
  does Up. With both levels cleared the page still has 2 entries (no
  boss entry), and A on "22" starts level 21. Opened on level 21 (the
  level select's `*level` set to 21), it shows page 5 with the cursor
  on "22";
- with a boss ("dingo rematch", Dingodile's room, after Neo Cortex), 28
  levels: the page has 2 entries, then, once both levels are cleared, 3
  (the boss entry on the right, `lastIndex` 2). Right ends on it and A
  starts level 26. Opened on Neo Cortex (level 25), the screen is on page
  4, entry 5.

## Actor categories

A stage (`{"stage": "<NAME>"}` in a level's `"rooms"`, a
`ROOM_KIND_CATEGORY` record) runs an actor category: the vehicle and boss
stages, which `ActorCategory::Init(catIndex)` plays (#966, phase 7).
`ActorCategory::descriptors[catIndex]` (`struct category_descriptor`,
`actor_anim.h`) says what it plays, and it is data: `levels.json`'s
`"categories"` and a spawn list per category in `data/levels/spawns/`. A
new category, or a new mix of the existing actor kinds, builds with plain
`make` and no code change.

**What the code indexes by category.** Only `ActorCategory::descriptors`
(`CATEGORY_COUNT` entries): `Init`, `ReloadGraphics`, `CountCrates`,
`IsMaskAssistDue` and the VRAM pool read the current category's
descriptor, and the stage record's `catIndex` is all `game_frame.cpp` and
`level_query.cpp` pass on. No code switches on the category or compares
it with a `CATEGORY_*` constant. What does differ in the code is the
category's **type**:

| type | `vtables[type]` (the player, the spawner, the boss) | also by type |
|---|---|---|
| `POLAR` (0) | `PolarPlayer`, the yeti | the BG palette cycle (`ActorPaletteCycle::Enable`), `ActorBg::InitScroll`'s depths, the cell animation's per-cell banks, 2 palette-cache slots, the HUD without the HP gauge, `CountCrates`' polar crate kinds, a death that isn't a boss death |
| `JETPACK` (1) | `JetpackPlayer`, the airship | the other side of each of those |
| `HOVERCRAFT` (2) | `JetpackPlayer`, n. gin's hovercraft | as `JETPACK` |

A new type is new code (a vtable and those tests), so a category is one
of the three. The boss HUD (`LevelState::GetBossHealth` and
`GetBossIndex`) is the level's, not the category's: a level that plays a
hovercraft stage names its boss, `"boss_kind": "N_GIN"` (see "Boss
levels"). What clearing it gives is the level's too, its `"on_clear"`.

**`"categories"`**, one per category in `CATEGORY_*` order:

```json
"categories": [
  {"constant": "CATEGORY_FROSTBITE_CAVERN", "name": "frostbite cavern", "type": "POLAR", "spawns": "frostbite_cavern", "mask_assist_deaths": 4, "bonus_kind_deaths": 3, "retry_boss_deaths": 2, "boss_level": 1, "retry_boss_level": 0},
  ...
  {"constant": "CATEGORY_N_GIN", "name": "n. gin", "type": "HOVERCRAFT", "sky": "no_fly_zone", "spawns": "n_gin", ...}
]
```

| key | the descriptor's | what |
|---|---|---|
| `"constant"`, `"name"` | (its index) | `CATEGORY_<NAME>` (`constants/categories.h`), which a stage names as `<NAME>` |
| `"type"` | `type` | `POLAR`, `JETPACK` or `HOVERCRAFT` (`CATEGORY_TYPE_*`) |
| (the type) | `cellAnim`, `cellAnimSize`, `palette`, `anim_table`, `sprite_sheet` | the type's: polar `gCategoryFamily0CellAnim`, `gPolarCategoryPalette`, `gCategoryFamily0AnimTable`, `gPolarSpriteSheet`; jetpack `gCategoryFamily1CellAnim`, `gAirshipCategoryPalette`, `gCategoryFamily1AnimTable`, `gJetpackSpriteSheet`; hovercraft the jetpack's with `gHovercraftCategoryPalette`. The ROM's categories of a type all have the same ones, and the actor kinds index the type's animation table (data, see "Animation tables") |
| `"cell_anim"` (optional) | `cellAnim`, `cellAnimSize` | the BG0 cell animation instead of the type's: a `cell_anim` of `graphics/manifest.json`, by its path (see "Category graphics") |
| `"sky"` (optional) | `bgPicture` | the BG1 picture: one of the jetpack skies, `rocket_racket`, `blimp_bonanza`, `no_fly_zone` (`gCategory3/4/5BgPicture`, `graphics/vehicles/jetpack/skies/`), or a `bg_picture` of the manifest by its path (see "Category graphics"). None for a polar category |
| `"palette"` (optional) | `palette` | the OBJ palette instead of the type's: a `category_palette` of the manifest, by its path (see "Category graphics") |
| `"sprite_sheet"`, `"anim_table"` (optional) | `sprite_sheet`, `anim_table` | the sprite sheet (a `framed_sheet` of the manifest, by its path) and the animation table (of `data/levels/anim_tables/`, by name) instead of the type's (see "Category sprite sheets") |
| `"spawns"` | `spawnTable` | its spawn list, `data/levels/spawns/<spawns>.json` |
| `"mask_assist_deaths"`, `"bonus_kind_deaths"`, `"retry_boss_deaths"` | the same | deaths before the mask assist, before the spawns' `bonus` kinds, before the boss's retry level |
| `"boss_level"`, `"retry_boss_level"` | `bossLevel`, `retryBossLevel` | the boss's level: 0-5 for the yeti (`Yeti::chargeParams`), 0-3 for the airship (`Airship::attacks[kind + level]`), 0-1 for the hovercraft (`Hovercraft::attacks`) |

**A spawn list** (`struct actor_spawn_table`, `actor.h`):

```json
{
  "symbol": "gCategory1SpawnTable",
  "family": "POLAR",
  "length": 4000,
  "spawns": [
    {"kind": "CHECKPOINT_CRATE", "x": 0, "y": 54, "z": 18},
    {"kind": "BASIC_CRATE", "x": 30, "y": 54, "z": 32},
    ...
  ]
}
```

- `"symbol"`: its C name.
- `"family"`: `POLAR` or `JETPACK`, whose kind names it uses
  (`POLAR_KIND_*`, `JETPACK_KIND_*`, `constants/actor_kinds.h`, without
  the prefix; a number for an unnamed kind). It must be the type's: a
  polar category's kinds are `POLAR`, a jetpack or hovercraft one's
  `JETPACK`.
- `"length"`: the course length (past it `RunFrame` calls the vtable's
  `reachCourseEnd`).
- `"spawns"`, in depth order (`"z"`): each its `"kind"`, its `"x"`, `"y"`
  and `"z"`; `"alt"` (the time-trial kind) and `"bonus"` (the kind after
  `"bonus_kind_deaths"`) when they aren't `"kind"`; `"unk_03"` when not
  0 (nothing reads it); `"next"`, the index of the spawn a plane or
  penguin flies on to, when not -1.
- `"end"`: the 4 bytes after the last spawn, which nothing reads, when
  not 0.

`levels.py` stops when a value is out of range, a kind isn't the family's,
the spawns aren't in depth order, a `"next"` isn't a spawn, or a list has
a kind only another type spawns: the airships (`AIRSHIP_0`-`2`) need the
jetpack type's boss (`Airship::Create`), the `HOVERCRAFT` the hovercraft
type. What a type's spawner makes of each kind is
`PolarPlayer::CreateActor` and `JetpackPlayer::CreateActor`; a kind
neither makes spawns nothing.

**What is generated.**

| output | what | used by |
|---|---|---|
| `constants/categories.h` (`levels.py constants categories`) | `CATEGORY_*` and `CATEGORY_COUNT`, with the hand-written types and exit statuses (`constants/category_types.h`) | everything that names a category |
| `actor_categories.inc` (`levels.py categories`) | the spawn lists that aren't the ROM's, then `ActorCategory::descriptors` | `actor_categories.cpp` |
| `spawns/<symbol>.inc` (`levels.py spawns <symbol>`) | the ROM's spawn list `<symbol>` (`gCategory0SpawnTable`..`gCategory6SpawnTable`), if a category has it | where the ROM has it: `polar_cell_anim.cpp`, `polar_spawn_tables.cpp`, `jetpack_cell_anim.cpp`, `jetpack_sky_pictures.cpp` |

With the ROM's categories every output is the ROM's C, so `make compare`
gives the ROM's bytes. `levels.py extract` reads the descriptors and the
seven spawn lists back from the ROM (keeping the constants, names and
spawn-list names) and checks that each type's graphics are the ROM's.
The categories aren't part of the save version's CRC: they don't change
what a save's records mean.

### Adding a category

1. **The spawn list.** A new `data/levels/spawns/<name>.json` with its
   own `"symbol"`, for example a copy of a ROM list with kinds swapped,
   spawns added or moved.
2. **The category.** An entry at the end of `"categories"`: its constant,
   type, sky, spawn list and numbers.
3. **Use it.** A level's `"rooms"` entry `{"stage": "<NAME>"}`.
4. **Run `make`.** `CATEGORY_COUNT` and `descriptors` grow, the new spawn
   list goes before them, the new stage record goes where a level first
   plays it, and the ROM data after them moves ([shifting.md](./shifting.md)).

A category can also have its own cell animation, sky or OBJ palette as
new graphics (see "Category graphics"), and its own sprite sheet and
animation table (see "Category sprite sheets"). Its actor kinds stay the
type's: the code makes them.

Tested in scratch with plain `make` and mGBA, with two new categories:
`CATEGORY_SNOW_STORM` (polar, snow crash's spawn list with each basic
crate a life crate, `"boss_level"` 5) played by jungle jam
(`"rooms": [{"stage": "SNOW_STORM"}]`), and `CATEGORY_SKY_HIGH` (jetpack,
rocket racket's sky, blimp bonanza's list with each ring a circling
bomber) played by shipwrecked. Starting jungle jam calls
`CountCategoryCrates(7)` (36, as snow crash: a life crate is a crate)
and `InitActorCategory(7)`, and the stage's `CreateActor` makes life
crates (kind 8) where snow crash has basic ones, with the lives counter
going up. Starting shipwrecked calls `InitActorCategory(8)`, loads
`gCategory3BgPicture` (rocket racket's sky) and makes circling bombers
(kind 8) where blimp bonanza has rings. `levels.py` stops on a polar
category with a sky, an unknown type or sky, a boss level out of range,
a jetpack list on a polar category, a hovercraft kind in a jetpack list,
an airship kind in a hovercraft category, an unknown kind name, spawns
out of depth order, a bad `"next"`, a duplicate constant and a missing
spawn list.

### Category graphics

A category's BG0 cell animation, BG1 sky, OBJ palette and (polar only)
BG palette cycle are the type's unless it names its own (#966, #1089):
`"cell_anim"`, `"sky"`, `"palette"` and `"palette_cycle"` name an entry
of `graphics/manifest.json` by its path under `graphics/`.
That can be one of the ROM's (`vehicles/jetpack/clouds.png`, a sky) or a
new graphic, an entry with no ROM address ([graphics.md, "New
graphics"](./graphics.md#new-graphics)):

```json
{"paths": ["vehicles/jetpack/skies/sky_red.png", "vehicles/jetpack/skies/sky_red_tiles.png"], "kind": "bg_picture", "symbol": "gSkyRedBgPicture"}
{"path": "vehicles/polar/ice_wall_red.png", "kind": "cell_anim", "symbol": "gIceWallRedCellAnim", "frames": 60, "banks": true}
{"path": "vehicles/polar/polar_green.pal", "kind": "category_palette", "symbol": "gPolarGreenCategoryPalette", "colors": 256}
{"path": "vehicles/polar/palette_cycle_red/", "kind": "palette_cycle", "symbol": "gPaletteCycleRedFrames", "frames": 32}
```

```json
{"constant": "CATEGORY_SKY_RED", "name": "sky red", "type": "JETPACK", "sky": "vehicles/jetpack/skies/sky_red.png", "spawns": "rocket_racket", ...}
{"constant": "CATEGORY_POLAR_GREEN", "name": "polar green", "type": "POLAR", "cell_anim": "vehicles/polar/ice_wall_red.png", "palette": "vehicles/polar/polar_green.pal", "spawns": "snow_crash", ...}
{"constant": "CATEGORY_POLAR_RED", "name": "polar red", "type": "POLAR", "palette_cycle": "vehicles/polar/palette_cycle_red/", "spawns": "snow_crash", ...}
```

| kind | source | what the code takes |
|---|---|---|
| `cell_anim` | one PNG, its `"frames"` stacked top to bottom, 8bpp with the 256-colour BG palette, each 8x8 cell in one 16-colour bank (as `ice_wall.png`, `clouds.png`); `"banks"`: whether each frame carries its cells' banks | the type's geometry (`CELL_ANIM_GEOMETRY`: 19x13 cells for a polar category, 38x10 otherwise), and banks for a polar category only (`CellAnim::Init`'s `hasBanks` is the type's). Any number of frames: `cellAnimSize` gives the count. Its palette is the BG palette until something replaces it, as in the ROM: a polar category's palette cycle writes banks 0-13 every frame (`ActorPaletteCycle::Update`), and a sky's palette replaces it (`LoadBgPicture`) |
| `bg_picture` | the picture and its tile set as an 8-pixel-wide strip (as the skies); its palette is the BG palette, the cell animation's too | a 38x16-cell picture (`BG_PICTURE_GEOMETRY`) whose tiles fit after the cell animation's two pages (`LoadBgPicture` puts them at `CellAnim::GetFreeTile()`, and the BG1 map reaches tile 0x5FF), for a jetpack or hovercraft category |
| `category_palette` | a JASC `.pal` of 256 colours | the OBJ palette, `DmaCopy`'d to OBJ palette RAM by `ActorCategory::Init`. New only: the ROM's three are C arrays with other data |
| `palette_cycle` | a directory of 32 JASC `.pal` files, `00.pal` to `31.pal`, each 224 colours: BG palettes 0-13 (the ROM's is `vehicles/polar/palette_cycle/`, `ActorPaletteCycle::frames`) | a polar category's only: `ActorPaletteCycle::Update` DMAs the cursor's frame to BG palettes 0-13 whenever the cell animation scrolls. 32 frames, as the ROM's: the polar spawn kinds' cycles (`POLAR_KIND_PALETTE_CYCLE_*`, `startFrames`/`targetFrames`) run 0-15 and 16-31, forwards or back |

`levels.py` stops on any other: a path that isn't such an entry, a
cell animation of another geometry or with the other banks, a sky of
another size or too many tiles, a sky on a polar category, a palette of
another count, a palette cycle on a jetpack or hovercraft category, a
palette cycle of another frame count or a frame of another colour count.

**A category's own palette cycle.** `ActorPaletteCycle::Update` takes
its frames from `ACTOR_PALETTE_CYCLE_FRAMES`
(`category_palette_cycles.h`, `levels.py category-cycles`). With no
category naming its own `"palette_cycle"` that is `frames`, the ROM's, so
the code is the ROM's. Otherwise it is
`categoryFrames[ActorCategory::current]`: `actor_categories.inc` then
defines `ActorPaletteCycle::categoryFrames`, each category's frames (its
own, or the ROM's `frames`), after the descriptors. The descriptors
don't change (the ROM's have no field for it). The ROM's frames are
`graphics/vehicles/polar/palette_cycle/` (the manifest's
`gActorPaletteCycleFrames`, `actor_palette_cycle_frames.cpp` `#include`s
them), so a recolour of every polar category is an edit of those 32
files, and one category's a copy of them it names.

Tested in scratch with plain `make` and mGBA: `CATEGORY_POLAR_RED`
(snow crash's, with `"palette_cycle": "vehicles/polar/palette_cycle_red/"`,
the ROM's 32 frames with red and blue swapped) played by snow crash's
level, and `CATEGORY_SNOW_CRASH` by snow job's. 500 and 800 frames after
the level select, BG palettes 0-13 are the red cycle's frame 0 in the
first and the ROM's frame 0 in the second, and the ice wall shows gold
where it is blue. `UpdateActorPaletteCycle` then reads
`categoryFrames[gActorCategory]`. `levels.py` stops on a cycle on a
jetpack category, one of 31 frames, one with 256-colour frames and a
path that isn't a `palette_cycle`; the ROM's cycle named by a polar
category builds the ROM's code.

**Colours the engine replaces** (#1089). Two things load over a
category's own cell animation palette. `levels.py categories` (the
`actor_categories.inc` rule) warns about each, and the build goes on, so
a mod can keep the colours on purpose:

- **The polar palette cycle.** `ActorPaletteCycle::Update` DMAs a
  0x1C0-byte frame to BG palettes 0-13 every moving frame, for a polar
  category only. So a polar cell animation of its own keeps only banks
  14-15 of its palette. The warning lists the colours its pixels use in
  banks 0-13 that differ from the ice wall's, the colours a mod changed
  (the ice wall's own banks 0-13 don't show either: its palette differs
  from every cycle frame). Recolour such an animation through its pixels
  (each cell's indices in its bank), through banks 14-15, or through
  the category's own `"palette_cycle"` (above).
- **A sky.** `LoadBgPicture` (from `ActorCategory::Init`, and again
  after the pause menu) loads the sky's whole 256-colour palette over
  the cell animation's (`CellAnim::ResetBg`). The warning lists the
  colours a jetpack or hovercraft category's own cell animation uses
  that its sky's palette doesn't have at the same index. Give the sky
  those colours.

Only a category's own cell animation is checked, not the type's: the
ROM's skies recolour the type's clouds on purpose (blimp bonanza's and
no fly zone's palettes differ from the clouds' in all 15 of their
colours). Bank colour 0 isn't checked: it is transparent in a cell.

Tested in scratch with four new cell animations made from the ROM's. The
ice wall with bank 2's colours inverted in its palette warns (indices 33
on); the ice wall with each pixel's index reversed in its bank, its
palette unchanged, doesn't; neither does the ice wall itself named as a
category's. The clouds with red and blue swapped in the palette warn
under rocket racket's and no fly zone's skies (a hovercraft category);
the clouds copied as they are don't under rocket racket's sky (the same
colours) but warn under blimp bonanza's; the type's clouds under blimp
bonanza's sky, as the ROM has them, don't.

**What is generated.** `actor_categories.inc` defines the new graphics the
categories name, each once, between the new spawn lists and the
descriptors: a `struct category_cell_anim_<symbol>` or `struct
category_bg_picture_<symbol>` sized from the PNGs (with a size assert),
or a `u16[256]`, each `#include`ing the built `.inc` files. The
descriptor points at it (`cellAnimSize` its size). `graphics_manifest.mk`
gives the build rules from the manifest, the ROM's and the new ones
alike: `CELL_ANIMS` (with `CELL_ANIM_FRAMES_<anim>` and
`CELL_ANIM_BANKS_<anim>`), `BG_PICTURES` and `CATEGORY_PALETTES`, and the
new ones' sources and `.inc` files, which `actor_categories.inc` and its
object depend on. With the ROM's categories nothing is new and every
output is the ROM's.

Tested in scratch with plain `make` and mGBA: `CATEGORY_SKY_RED` (jetpack,
rocket racket's spawn list, its sky with red and blue swapped in the
palette) played by jungle jam, and `CATEGORY_POLAR_GREEN` (polar, snow
crash's spawn list, the ice wall with each cell's colours reversed in their bank, and
`gPolarCategoryPalette` with red and green swapped) played by
shipwrecked. In mGBA, jungle jam's stage (`InitActorCategory(7)`) shows
rocket racket's sky and clouds in orange where they are blue in rocket
racket's own stage. Shipwrecked's (`InitActorCategory(8)`) shows Crash,
the wumpa and the crates green where snow crash's stage shows them in
their colours. Its ice wall is the new animation, with each cell's
colours reversed in their bank: the polar palette cycle sets those
banks' colours, so a polar cell animation shows in its pixels, not its
palette. The ROM's descriptors are unchanged, and with no new graphic
nothing new is defined.

### Animation tables

A type's animation table (`category_descriptor.anim_table`, `struct
anim_table_record`, `actor_anim.h`) has one record per actor kind: the
spawner indexes it with the spawn's kind (`gActorAnimTable[kind]`,
`gJetpackAnimTable[kind]`, set by `ActorCategory::Select` from the
descriptor), and the actors it makes draw the record's keyframes and
frames. The ROM's two are data, `data/levels/anim_tables/polar.json`
(`gCategoryFamily0AnimTable`, 41 records) and `jetpack.json`
(`gCategoryFamily1AnimTable`, 47), with the arrays their records point
at:

```json
{
  "symbol": "gCategoryFamily0AnimTable",
  "family": "POLAR",
  "records": [
    {"keyframes": "gPolarPlayerKeyframes", "frames": "gPolarPlayerFrames", "palette": 0, "depth": 12032, "box": [-10, -20, -1, 20, 44, 3]},
    {"keyframes": "gPolarBasicCrateKeyframes", "frames": "gPolarBasicCrateFrames", "palette": 1, "depth": 12604, "box": [-10, -10, -1, 20, 20, 3]},
    ...
    {},
    ...
  ],
  "arrays": [
    {"keyframes": "gPolarPlayerKeyframes", "list": [
      [128, 0, 20, 0, 0],
      ...
    ]},
    {"player_frames": "gPolarPlayerFrames", "list": [
      0, 1, 2, ...
    ]},
    {"frames": "gRiderlessPolarFrames", "list": [
      "riderless_polar/00", "riderless_polar/01", ...
    ]},
    "table",
    ...
  ]
}
```

- `"symbol"`: the table's C name. `"family"`: `POLAR` or `JETPACK`, the
  kinds that index it (`constants/actor_kinds.h`) and the sprite sheet
  and player frames its records' frames are. `"sheet"` (optional, a
  category's own table only): another sprite sheet its frames are, a
  `framed_sheet` of the manifest by its path (see "Category sprite
  sheets").
- `"records"`, one per kind the code indexes (41 polar, 47 jetpack):
  `"keyframes"` and `"frames"` (arrays of `"arrays"`, by symbol),
  `"palette"` (the OBJ palette bank, 0-15), `"depth"` (the depth at which
  it draws unscaled, `DrawActor` divides by it), `"box"` (`[x, y, z, w, h,
  d]`, `struct anim_box`), and when not 0 `"spawn_x"` and `"spawn_y"`
  (added to the spawn's position by `CreateActor`). `"index"` is the
  record's first byte when it isn't the kind (records 15 of both tables
  have 0). `{}` is a kind with no record (polar 32-34).
- `"arrays"`, in the order they are defined (the ROM's), with `"table"`
  where the table itself goes. Each is one of:
  - `{"keyframes": symbol, "list": [...]}`: the clip's keyframes, `struct
    anim_frame_record`, each `[duration, frame, loop end, loop start, OAM
    attribute]`: it shows frames `frame` to `frame + loop end - 1` of the
    record's frames and loops back to `frame + loop start`; the attribute
    is OR'd into the OAM attributes (`0x1000`: flipped horizontally);
  - `{"frames": symbol, "list": [...]}`: frames of the family's sprite
    sheet (`graphics/vehicles/<family>/sprite_sheet/`, or the table's
    `"sheet"`), each its entity folder and frame PNG, `"wumpa/03"`, which
    the build turns into the frame's offset in the decompressed sheet
    (`FRAMED_POLAR_WUMPA_03`, `tools/framed_gfx.py header`, under the
    sheet's `"prefix"`);
  - `{"player_frames": symbol, "list": [...]}`: record 0's, Crash's
    compressed frames (`graphics/vehicles/<family>/player.png`) by number
    (`PolarPlayerFrameSet::frames + RLE_SPRITES_POLAR_PLAYER_FRAME_nnn`).
    Only record 0 has them: the others' frames are sheet offsets.

  and has an optional `"comment"`, which goes into the C.

`levels.py` stops on an unknown key, a family that isn't one, a record
count that isn't the family's, a record with a missing key or a value out
of range, a record naming an array that isn't there or isn't its kind (a
player frame array on a record but 0, or the other way round), a frame
that isn't a PNG of the sheet, a player frame past the PNG's, a keyframe
whose frames run past its record's frames or whose loop start isn't
before its end, and a duplicate symbol.

**What is generated.** `levels.py anim-table <name>` writes
`anim_tables/<name>.inc`: the arrays' declarations, then the arrays and
the table in `"arrays"` order, which `polar_anim_family.cpp` and
`jetpack_anim_family.cpp` `#include` after their palettes (the C keeps
the palettes and, in the jetpack file, the yeti's palette and boxes
before them). With the ROM's data it is the ROM's C, so `make compare`
gives the ROM's bytes, and `levels.py extract` writes both JSONs again
from the ROM, keeping their arrays' symbols, lengths, order and comments.
Editing a value (a box, a palette bank, a keyframe, which frames a clip
shows) builds with plain `make`; adding or removing an array moves the
ROM data after the table ([shifting.md](./shifting.md)). A table only a
category names goes with the categories (see "Category sprite sheets").

### Category sprite sheets

A category's actors draw their frames from its sprite sheet, through its
animation table: `ActorCategory::Init` unpacks
`descriptors[cat].sprite_sheet` (`CategorySpriteSheet::Decompress`, into
a buffer of the size the LZ77 header gives) and hands
`descriptors[cat].anim_table` to the spawner (`ActorCategory::Select`),
and a record's frames are offsets into the unpacked sheet. Both are the
type's unless the category names its own (#1087):

| key | the descriptor's | what |
|---|---|---|
| `"sprite_sheet"` (optional) | `sprite_sheet` | a `framed_sheet` of `graphics/manifest.json`, by its path: the ROM's two, or a new one ([graphics.md, "New graphics"](./graphics.md#new-graphics)) |
| `"anim_table"` (optional) | `anim_table` | an animation table of `data/levels/anim_tables/`, by name (see "Animation tables"), of the type's family |

```json
{"path": "vehicles/jetpack/sprite_sheet_red/", "kind": "framed_sheet", "symbol": "gJetpackRedSpriteSheet", "prefix": "JETPACK_RED"}
```

```json
{"constant": "CATEGORY_ROCKET_RED", "name": "rocket red", "type": "JETPACK", "sky": "rocket_racket", "sprite_sheet": "vehicles/jetpack/sprite_sheet_red/", "spawns": "rocket_racket", ...}
{"constant": "CATEGORY_SNOW_ALT", "name": "snow alt", "type": "POLAR", "anim_table": "polar_alt", "spawns": "snow_crash", ...}
{"constant": "CATEGORY_FROST_SMALL", "name": "frost small", "type": "POLAR", "sprite_sheet": "vehicles/polar/sprite_sheet_small/", "anim_table": "polar_small", "spawns": "snow_crash", ...}
```

- **A reskin: the type's frames, other pixels.** A sheet with the type's
  sheet's folders, frames and frame sizes has the same offsets, so the
  type's table works with it: only the sheet changes. Copy the type's
  sheet folder, edit the pixels, add the manifest entry, and name it as
  the category's `"sprite_sheet"`.
- **Other frames: its own table.** A sheet with other frames (a frame of
  another size, frames added or removed) moves the offsets, so the
  category also names a table of that sheet: a table with `"sheet"`, the
  sheet's path. Its frame arrays name that sheet's frames, under the
  sheet's prefix (`FRAMED_POLAR_SMALL_WUMPA_03`).
- **Another table on the type's sheet.** A table with no `"sheet"` is of
  the family's, so a category can change its records (which frames a
  kind shows, its keyframes, palette bank, box, depth) without a new
  sheet.

A table that isn't the ROM's is like the ROM's (see "Animation tables")
but its records can also point at the arrays of its family's ROM table,
by symbol: the keyframes, the player frames (record 0's: a new table has
none of its own, so Crash's frames stay the type's), and, if it is of the
type's sheet, the frames. So a table that changes a few records names its
own arrays only for those. Its symbols are its own: no other table has
them.

`levels.py` stops on a `"sprite_sheet"` that isn't a framed sheet of the
manifest, a sheet with frames other than the type's (it names the first
frame that differs) and no `"anim_table"`, an `"anim_table"` that isn't a
table or is another family's, a table of another sheet than the
category's, a sheet that unpacks bigger than the type's (`Decompress`
allocates it from the EWRAM heap: the polar sheet is 0x34048 bytes of
EWRAM's 0x40000), a new table with player
frames of its own, a record pointing at a ROM table's frames of another
sheet, and a symbol two tables share.

**What is generated.** `actor_categories.inc` defines a new sheet (`u8[]`,
LZ77, `#include`ing its built `.bin.lz.inc`) with the other new graphics,
then the tables the categories name that aren't the ROM's: the frame
offsets of their sheets (`#include "<sheet>_frames.h"`), the declarations
of the ROM tables' arrays they point at, and each table. The descriptor
points at them. `graphics_manifest.mk` lists every framed sheet
(`FRAMED_SHEETS`, with `FRAMED_NAME_<sheet>`), so a new one gets the
ROM's rules: its folders packed in `sheet.json`'s order, LZ77-compressed,
and its frames' offsets. With the ROM's categories nothing is new and
every output is the ROM's.

**The OBJ palette.** A recolour without new pixels is the category's
`"palette"` (see "Category graphics"): the 256 colours `ActorCategory::
Init` copies to OBJ palette RAM. A table's records pick each kind's bank
of it (`"palette"`). The palette-cache slots `ActorCategory::
SetupVramPool` binds by type from sprite bank 35 (`actor_vram_pool.cpp`)
stay the type's: they're code.

**New actor kinds stay code.** What the data reaches is the records the
kinds index: a kind's frames, keyframes, bank, box, depth and spawn
offset. Which class a kind makes, and how it moves, collides and what
breaking it does, is `PolarPlayer::CreateActor` and `JetpackPlayer::
CreateActor`'s `switch (kind)` and the classes' code, and the code also
names some records itself (`JETPACK_KIND_QUESTION_CRATE_WUMPA_1`'s for a
question crate, the explosion's, the checkpoint text's). So a table has
the family's record count, a record of a kind is drawn by that kind's
class, and a reskinned or re-animated kind is data; a kind with a new
class or new behaviour is a new `case` and a class, code.

Tested in scratch with plain `make` and mGBA, with three new categories,
each played by a level instead of its own stage: `CATEGORY_ROCKET_RED`
(jetpack, rocket racket's list, a copy of the jetpack sheet with the
plane's and the bomber's colour indices reversed) by rocket racket,
`CATEGORY_SNOW_ALT` (polar, snow crash's list, table `polar_alt` on the
type's sheet: the basic crate records show the aku aku crate's frames
through its own frame array, the wumpa its own keyframes, the rest the
ROM table's arrays) by snow crash, and `CATEGORY_FROST_SMALL` (polar,
snow crash's list, a copy of the polar sheet with the wumpa frames
16x16 instead of 32x32, and table `polar_small`, a copy of `polar.json`
with its own symbols on that sheet) by frostbite cavern. Rocket racket's
stage (`InitActorCategory(7)`) unpacks `gJetpackRedSpriteSheet`, keeps
`gCategoryFamily1AnimTable`, and its planes show in the reversed colours
where the original stage's are red. Snow crash's (`InitActorCategory(8)`)
selects `gPolarAltAnimTable`, and its basic crates show as aku aku
crates. Frostbite cavern's (`InitActorCategory(9)`) unpacks
`gPolarSmallSpriteSheet` and selects `gPolarSmallAnimTable`: the wumpa
are half the size, and the crates, Crash and the checkpoint text (after
the wumpa in the sheet) show right. `levels.py` stops on each of the
errors above.

## Boss levels

A level's boss is its `"boss_kind"` in `levels.json` (#966). The boss HUD
comes from it: `LevelState::GetBossIndex` (the boss's icon, `BOSS_*`,
`constants/bosses.h`; `BOSS_NONE` without one) and `GetBossHealth` (the
HUD's health) switch on the level id, and their case labels are generated
per kind. So a new level with a boss, or a hovercraft stage in another
level, gets the boss HUD with no code change.

```json
{"constant": "LEVEL_N_GIN", "name": "n. gin", "world": 1, "boss": true, "boss_kind": "N_GIN", ...}
```

The kinds are code: each is a boss controller and what the HUD shows of
it (`BOSS_KINDS`, `tools/levels.py`).

| `"boss_kind"` | the boss | `GetBossIndex` (HUD icon) | `GetBossHealth` | the ROM's level |
|---|---|---|---|---|
| `DINGODILE` | a room's `DINGODILE` entity (`DingodileCtrl::Spawn`) | `BOSS_DINGODILE` | `3 - boss->counter` (hits left) | `LEVEL_DINGODILE` |
| `N_GIN` | a stage of a `HOVERCRAFT` category (n. gin's hovercraft) | `BOSS_N_GIN` | `Hovercraft::GetPartsLeft()` (weapons left, 4) | `LEVEL_N_GIN` |
| `TINY` | a room's `TINY` entity | `BOSS_TINY` | `3 - boss->counter` | `LEVEL_TINY` |
| `NEO_CORTEX` | a room's `CORTEX_BOSS` entity | `BOSS_NEO_CORTEX` | `3 - boss->counter` | `LEVEL_NEO_CORTEX` |
| `MEGA_MIX` | a room's `MEGA_MIX` entity | `BOSS_NONE` (its own case) | 0 | `LEVEL_MEGA_MIX` |

The health is the kind's, not a number in the data: the 2D bosses' three
hits are their controllers' code (`counter > 2`), and the hovercraft has
four weapons. A level without `"boss_kind"` has no boss HUD.

**Checks.** `levels.py` stops unless a level's `"boss_kind"` is the boss its
rooms and stages have: a level whose rooms place a boss entity, or that
plays a stage of a `HOVERCRAFT` category, must name that kind, a level
with a `"boss_kind"` must have that boss, and a level has one boss at most
(`LevelState::boss` is one controller). `"boss_kind"` is independent of
`"boss"` (the level select's boss entry, `isBoss`).

**What is generated.** `level_bosses.h` (`levels.py bosses`): per kind,
`LEVEL_BOSS_CASES_<KIND>`, the `case` labels of its levels. The two
functions keep a block per kind; a kind no level has gets an empty macro,
and its block is dead code. With the ROM's levels each macro is one
label, so the functions are the ROM's code (`levels.py extract` checks
the kept kinds against `ROM_BOSS_KINDS`). The three 2D bosses' health
blocks stay apart: g++ merges them into the ROM's one, while a single
block with their labels comes out 4 bytes shorter.

The `BOSS_NEO_CORTEX` tests of the gem spawners (`SpawnRedGem` and
friends, `platform_create.cpp`) follow the kind, since they ask
`GetBossIndex`.

`"boss_kind"` isn't part of the save version's CRC (`level_data_crc`
leaves it out): it doesn't change what a save's records mean.

Tested in scratch with plain `make` and mGBA, with two new levels off the
grid after mega-mix: `LEVEL_HOVER_REMATCH` (25), playing
`{"stage": "N_GIN"}` with `"boss_kind": "N_GIN"`, and
`LEVEL_DINGO_REMATCH` (26), playing `dingodile/main` with
`"boss_kind": "DINGODILE"`. `InitHud`'s `GetBossIndex` gives 2
(`BOSS_N_GIN`) and 3 (`BOSS_DINGODILE`), and the HUD shows the boss's
icon and health. In level 25, played with the jetpack firing and
sweeping, `GetBossHealth` goes from 4 to 3 when a shot knocks one of the
hovercraft's weapons off, and the gauge drops a pip. In level 26 it is
3, and 2 (one pip less) once the boss controller's hit counter is
raised by hand, as a hit does. `levels.py` stops on a level that plays
the hovercraft stage with no `"boss_kind"`, on a level that places a
boss with none, on `"boss_kind": "TINY"` for Dingodile's room, on a
kind for a level with no such boss, on an unknown kind, and on a level
with two bosses.

### What clearing a level does

A level's optional `"on_clear"` in `levels.json` (#1086) is what
clearing it does besides the save: the power, its dialog, a cutscene,
opening the next world, the level's `cleared` bit, going straight on to
another level, the credits. It is `UpdateGameFrame`'s switch on the
cleared level (`game_frame.cpp`), a generated case per level that has
one. The HUD and the health are the boss kind's (above); what clearing
a level gives is the level's.

```json
{"constant": "LEVEL_DINGODILE", ..., "boss_kind": "DINGODILE",
 "on_clear": {"power": "SUPER_BODY_SLAM", "open_world": true, "cutscene": 4}, ...}
```

| key | what | the code |
|---|---|---|
| `"power"` | a power of `save->flags`: `TURBO_RUN`, `SUPER_BODY_SLAM`, `TORNADO_SPIN` or `DOUBLE_JUMP` (a new power is code). Given the first time the level is cleared, with its dialog | `if (!Has<Power>()) { ...; Give<Power>(); Show<Power>Dialog(); ... }` |
| `"open_world"` | with a `"power"`, when it is given: the next level select turns to the next page by itself if it is open (see "Level-select pages") | `LevelSelect::SetNewWorldOpened()`, first in the block |
| `"cutscene"` | a cutscene of `data/cutscenes.json` (`gCutscenes`, `PlayCutscene`): its index (0-10 for the ROM's) or its constant (`"CUTSCENE_LAB"`, a new one, [data.md, "A new cutscene"](./data.md#a-new-cutscene)). With a `"power"`, played once, last in its block; without one, at every clear | `PlayCutscene(N)`, N the index |
| `"mark_cleared"` | sets the level word's `cleared` bit (`LEVEL_FLAG_CRYSTAL`, the bit the crystal pickup sets) | `GetCurrentLevelFlags()->w.cleared = 1` |
| `"next_level"` | `{"if_completion_over": N, "cutscene": C, "level": L}`: over N% completion (0-99), play C (optional) and go straight on to level L, with no level select in between | `if (progress.GetCompletionPercent() > N) { PlayCutscene(C); room.level += L - <this level>; room.roomIndex = 0; goto start; }` |
| `"credits"` | `{}` or `{"cutscene": C}`: play C, then the credits | `PlayCutscene(C); RunCredits()` |

They come in that order in the case, then `break`. The ROM's
`"on_clear"`s:

| level | `"on_clear"` |
|---|---|
| `LEVEL_DINGODILE` | `{"power": "SUPER_BODY_SLAM", "open_world": true, "cutscene": 4}` |
| `LEVEL_N_GIN` | `{"power": "DOUBLE_JUMP", "open_world": true, "cutscene": 5}` |
| `LEVEL_TINY` | `{"power": "TORNADO_SPIN", "open_world": true, "cutscene": 6}` |
| `LEVEL_NEO_CORTEX` | `{"power": "TURBO_RUN", "open_world": true, "next_level": {"if_completion_over": 99, "cutscene": 8, "level": "LEVEL_MEGA_MIX"}, "credits": {"cutscene": 10}}` |
| `LEVEL_MEGA_MIX` | `{"credits": {"cutscene": 9}}` |

None of them has `"mark_cleared"`: the ROM's bosses don't set their
`cleared` bit. A boss's (`"boss": true`) bit is only read by
`Pickups::SpawnCrystal` (a crystal in its rooms isn't spawned again):
the crystal count, the boss entry (`PlaceItems`) and `{"levels_cleared":
true}` (`IsWorldCleared`) count the numbered levels only. It matters for
a numbered level with a boss (a `"boss_kind"`), which has no crystal to
pick up.

**A level without `"on_clear"`** takes the switch's default case, as
every level but the ROM's five does: its `cleared` bit is set when the
room it ended in is a category stage (`room.cat->kind ==
ROOM_KIND_CATEGORY`), so a polar or jetpack level, or a hovercraft boss
whose stage is its last room. A level with an `"on_clear"` never takes
it: a hovercraft boss level with an `"on_clear"` needs
`"mark_cleared"` to be marked.

**The power dialogs** stay the ROM's: each power's dialog has its text
and icon (`power_dialog_draw.cpp`). Any level can give any of the four
powers, but only one level each: the power is a bit of the save, and the
second level's block would never run.

**Checks.** `levels.py` stops on an unknown key or power, a power given
by two levels, `"open_world"` without a `"power"`, a cutscene that isn't
one of `data/cutscenes.json`'s (an index past `CUTSCENE_COUNT`, 11 in the
ROM, or an unknown constant), a `"next_level"` level that doesn't
exist, a percentage past 0-99, and `"next_level"`s that come back round
(a level that leads back to itself would replay forever). It warns
(and builds) when a numbered level with a `"boss_kind"` is never marked
cleared: no `"mark_cleared"`, no `CRYSTAL` entity in its rooms, and
(without an `"on_clear"`) not ending in a category stage. Its world's
boss entry, the world after it if `{"levels_cleared": true}` opens it,
and the crystal count wait for its bit.

**What is generated.** `level_on_clear.h` (`levels.py on-clear`):
`LEVEL_ON_CLEAR_CASES`, the cases with their statements, in the levels'
order, like RunRoom's `THEME_PALETTE_CYCLE_CASES`. With the ROM's levels
they are the ROM's five cases, so `UpdateGameFrame` is the ROM's code
(`levels.py extract` keeps the `"on_clear"`s, which are code, and checks
them against `ROM_ON_CLEAR`). Neo Cortex's step is `room.level +=
LEVEL_MEGA_MIX - LEVEL_NEO_CORTEX`: the ROM's `add #1`.

`"on_clear"` isn't part of the save version's CRC either: it doesn't
change what a save's records mean.

Tested in scratch with plain `make` and mGBA: a new level off the grid
after mega-mix, `LEVEL_DINGO_REMATCH` (25), playing `dingodile/main`
with `"boss_kind": "DINGODILE"` and `"on_clear": {"power":
"DOUBLE_JUMP", "open_world": true, "cutscene": 4, "mark_cleared":
true}`, with n. gin's `"on_clear"` taken out (the power is given by one
level). From a new game, with the level picked and its room exit
requested by hand (as the boss's power pickup does), clearing it calls
`SetNewWorldOpened`, `GiveDoubleJump` and `ShowDoubleJumpDialog` (the
Double Jump dialog shows) and plays cutscene 4 ("Cortex: Meddling
marsupial!"). Back at the level select, the save's flags are 0x80
(`DOUBLE_JUMP`) and level 25's word has its `cleared` bit, both 0 before.
On the level select, Up on page 1 turns to page 2 (`IsNextWorldOpen`'s
`DOUBLE_JUMP` rule), and Up there plays the error sound (no Tornado
Spin). `levels.py` stops on each check above (a power given twice, an
unknown power, cutscenes 11, 40 and -1, a `"next_level"` loop, one to
the level itself, an unknown level, 100%, `"open_world"` without a
power, an unknown key, a `"mark_cleared"` of 1) and warns for jungle jam
playing Dingodile's room with no `"on_clear"`, and with one without
`"mark_cleared"` and world 1 opened by `{"levels_cleared": true}`.

## The save

The cartridge save is one record, `SaveData` (`save_data.hpp`): four
slots, then a 12-byte trailer. A slot (`struct save_slot`) is a
`GameProgress` (the halfword of lives, mask and wumpa, the progress
flags byte, a word per level, then the extra progress flags) and 8 bytes
more (the level and the two volumes). The trailer is the slots' empty
flags, the `'C'` marker, the version byte, two flag bytes and the
checksum, the additive sum of the words before it.

Everything about the record's size comes from the level data:
`tools/levels.py constants save` writes `constants/save.h`, and the code
uses its constants where the ROM had numbers. There is no `#if`: with
the ROM's levels every constant is the ROM's number and the ROM matches.

| | ROM (25 levels) | 4 Kbit (up to 28 levels) | 64 Kbit (29 to 60 levels) |
|---|---|---|---|
| `SAVE_EEPROM_TYPE` (`EEPROMConfigure`'s chip) | `EEPROM_CHIP_4KBIT` (4) | `EEPROM_CHIP_4KBIT` | `EEPROM_CHIP_64KBIT` (0x40) |
| `SAVE_DATA_SIZE` (the record) | 0x200 | 0x200, the whole chip | the four slots and the trailer, rounded up to the EEPROM's 8-byte blocks: 0x230 for 31 levels |
| a slot | 0x70 | 4 bytes more a level | 4 bytes more a level, and `PROGRESS_EXTRA_FLAG_BYTES` |
| the slots | 0x000-0x1BF | 0x000-0x1EF at most | 0x000 to the trailer |
| unused | 0x1C0-0x1F3 (0x34 bytes) | 0-0x34 bytes | 0-7 bytes |
| the trailer | 0x1F4-0x1FF | 0x1F4-0x1FF | the record's last 12 bytes |
| EEPROM blocks read and written | 0x40 (`gEepromConfig->maxCount`, the chip) | the same | `SAVE_DATA_SIZE / 8` of the chip's 0x400 |

The record keeps its order (slots, unused, trailer), and only grows:
`slotEmpty`, `magic`, `versionNibble`, `flags` and `checksum` stay the
last 12 bytes, and `ChecksumOk`/`UpdateChecksum` sum
`SAVE_DATA_SIZE / 4 - 1` words. The save menu keeps its four slots:
`SaveMenu::DrawSlots` draws four fixed rows, so a larger save holds
longer slots, not more of them.

### The EEPROM

The AgbEeprom library (`lib/agb_eeprom`, EEPROM_V122) already knows both
chips, and nothing in it changes. `EEPROMConfigure(4)` picks
`gEepromConfig512` (0x40 blocks, 6 address bits) and `EEPROMConfigure(0x40)`
`gEepromConfig8k` (0x400 blocks, 14 address bits); a read sends
`addrBitCount + 3` bits and a write `addrBitCount + 0x43`.
`SaveData::Read` and `Write` call `EEPROMConfigure(SAVE_EEPROM_TYPE)`
and transfer `SAVE_EEPROM_BLOCKS` blocks: `gEepromConfig->maxCount` when
the save is the 4 Kbit chip, which is the ROM's code, else the record's
`SAVE_DATA_SIZE / 8`. That is a constant condition, so the compiler keeps
only one side. The 64 Kbit save doesn't use the whole chip: the record is
copied through a stack buffer (below), and 0x400 block writes would take
some seconds.

The ROM's `"EEPROM_V122"` string is the same for both chips. Emulators
and flash carts tell the size from the accesses. mGBA starts at 4 Kbit
and turns the save into 8 KB on the first access past 0x200 bytes; other
tools count the bits of a request (17 or 81 for 64 Kbit). A 64 Kbit build
does both: 14-bit addresses, and blocks past 0x40.

### The stack

`SaveData::Read` and `Write` copy the record through a stack buffer the
record's size (`u8 buf[sizeof(SaveData)]`), and `SaveMenu::SaveToSlot`
holds two slots on the stack above `Store`. The stack is the top of
IWRAM above the IWRAM heap: `mem_heap_init(0x400)` in the ROM. The ROM's
deepest save (`SaveToSlot`, `Store`, `Write`, `EEPROMWrite1_check`,
`EEPROMWrite`) already runs about 0x6C bytes past those 0x400, into the
top of the IWRAM heap, which is free in the save menu (measured in mGBA:
the IWRAM heap's last used block ends at 0x03003B08 there). So main
reserves `SAVE_STACK_GROWTH` bytes more, the record's and the two slots'
growth (0 in the ROM, 0x78 for 31 levels with one extra flag word), and
a larger save goes no further into the heap than the ROM's.
`SAVE_64KBIT_MAX_SIZE`, 0x400 bytes, keeps that small: at 60 levels the
IWRAM heap loses 0x318 bytes, and it had at least 13,668 bytes free while
the ROM's levels played in mGBA.

### The link cable

`SaveMenu::LinkExchange` sends the cartridge's record and receives the
other console's (`SaveTransfer`), for "load from link" and for the Crash
2/3 bonus. The ROM sends and expects 0x200 bytes, and so do Crash 2 and
3: a receiver counts the bytes in until it has exactly 0x200, so a longer
record would be written past its buffer. So the link record stays
`SAVE_LINK_SIZE`, 0x200 bytes, in every build. A 4 Kbit build (the ROM,
or modified levels that still fit) sends its record as it is.

A 64 Kbit build sends a 0x200-byte view of its record instead, a
`LinkRecord` (`save_data.hpp`), and turns the view it receives back into
a record (`SAVE_LINK_VIEW`, `SAVE_DATA_SIZE != SAVE_LINK_SIZE`: a
constant condition, so `LinkExchange` keeps only the ROM's side with the
ROM's data). The view has the ROM's layout:

| offset | the ROM's record | the view |
|---|---|---|
| 0x000 | four 0x70-byte slots | the four slots, with the first 25 level words (`SAVE_LINK_LEVELS`), no extra flags |
| 0x1C0 | unused, 0 (`Reset` clears it) | the sender's `SAVE_CHECKSUM_SEED`, its `slotEmpty`, its `SAVE_DATA_SIZE` (`wholeSize`) |
| 0x1F4 | `slotEmpty` | 1, 1, 1, 1 |
| 0x1F8 | `'C'`, the version, the flags, 0 | `'C'`, the sender's version, its flags, 0 |
| 0x1FC | the checksum from `SAVE_CHECKSUM_SEED` | the checksum from 0, the ROM's seed (`SAVE_LINK_CHECKSUM_SEED`) |

What each side gets:

| the other console | it reads the view as | the 64 Kbit build reads its record as |
|---|---|---|
| the ROM | a Crash XS record (game id 1) with four empty slots: "load link game" shows nothing to load | a Crash XS record with four empty slots, since the seed in its gap isn't the build's |
| Crash 2 or 3 | a Crash XS record, as from the ROM | the Crash 2 or 3 record: the bonus (`SetFlags(2)` or `(4)`) and its message |
| a 4 Kbit build of other level data | "connection failed": it checks the view from its own seed | "connection failed": the record's checksum isn't from 0 |
| a 64 Kbit build of other level data | a Crash XS record with four empty slots | the same |
| a 64 Kbit build of the same level data | then the whole record, below | the same |

`LinkRecord::Pack` marks the four slots empty in the ROM's trailer: they
mean something only with the sender's levels, and the ROM would load them
as its own. Only a build with the same version and seed takes them back
(`Unpack`), and then only what the view holds: a loaded slot has the
first 25 level words, and the levels past them and the extra progress
flags are 0. A view whose checksum fails is "connection failed"
(`LinkExchange` returns 2).

Two 64 Kbit builds of the same level data then exchange their whole
records. Each view says it with `wholeSize`, the sender's
`SAVE_DATA_SIZE` (`LinkRecord::SendsWhole`: the same version and seed,
and `wholeSize` is the build's), so both sides decide the same thing
from the view they receive. `ExchangeWholeRecords` runs two more
transfers of `SAVE_LINK_SIZE` bytes, the record's first 0x200 bytes and
its last 0x200 (they overlap: a record is 0x400 bytes at most), into
`linkSave`, whose own checksum then has to hold. A slot then loads with
all of its levels and extra flags. The transfers are the ROM's
(`SaveTransfer`, `RunLinkTransfer` is `LinkExchange`'s loop), so B
cancels them as it cancels the first. A view from a build of the same
levels without `wholeSize` (0 there) gets only the view's slots, both
ways.

The link session has no flow control. A console starts the next
transfer once its own has settled (`SaveTransfer::Poll`'s 0x1F extra
polls after both directions are done), and the other console may still
be settling: until it starts the next transfer too, it doesn't take the
new bytes out of its 0x80-byte incoming ring. They wait there, and only
past 0x80 bytes would they be lost, which fails the record's checksum
("connection failed"). In mGBA the ring held 8 bytes at most at the end
of a frame.
If only one side's view fails its checksum, that side stops while the
other waits for the next transfer, until B.

Tested on two mGBA 0.10.5 cores linked by its lockstep link cable, with
a 31-level build: with the ROM, both show four empty slots and load none;
with the ROM patched to version 0x22 and 0x32 (as Crash 2 and 3 identify
themselves), the 31-level build stores flag 2 or 4 and shows the Crash 2
or 3 message; between two 31-level builds, `linkSave` is the sender's
record byte for byte and a slot loads with all 31 level words, and with
a build without `wholeSize`, with the first 25.

### Extra progress flags

`GameProgress::flags`' 8 bits are all used (the 4 coloured gems and the 4
powers). `levels.json`'s optional `"extra_progress_flags"` lists more:

```json
"extra_progress_flags": [{"name": "WORLD_5_SEEN", "comment": "what sets it"}]
```

Each is `PROGRESS_EXTRA_BIT_<name>` (`constants/levels.h`), bit N % 8 of
`GameProgress::extraFlags[N / 8]`, after the level words. The array is
a word per 32 flags (`PROGRESS_EXTRA_FLAG_BYTES`, 0 and no bytes in the
ROM), so it is saved with the rest of the block and grows the slots like
a level: the 4 Kbit save holds 27 levels and up to 32 extra flags. The
flags are part of the save version's CRC.

### Compatibility

A save's records mean something only with the levels it was made with:
`GameProgress::levels` is one word per level id, and the save slots grow
with the level count. So the save data is made incompatible between
builds of different level data, with no code of its own: the build
derives two constants from the level data, in `constants/save.h`, and
the save code uses them where the ROM had numbers.

- **The CRC.** `level_data_crc` is the CRC-32 of the canonical JSON
  (sorted keys, no spaces, UTF-8) of `levels.json`'s `"levels"`, then
  of each room's `[folder, room.json]` in `"rooms"` order, then of
  `"extra_progress_flags"` when there are any. The ROM layout
  residuals (`"regions"`, `"assets"`, `"table"`), the levels'
  `"boss_kind"` (see "Boss levels"), the pictures and the collision
  images don't count: a picture edit keeps the saves. With the
  ROM's data it is `ROM_LEVEL_DATA_CRC` (0x7ED35075), which `levels.py
  extract` checks.
- **`SAVE_VERSION`**, the record's version byte (`versionNibble`, the
  trailer's second byte: 0x1F9 in the 4 Kbit record).
  Its high nibble is the game, 1 for Crash XS: over the link cable,
  `SaveMenu::LinkInput` reads it to tell a Crash XS save from the Crash 2
  and Crash 3 ones that give the link bonus, so it stays 1. The ROM's low
  nibble is 2. With other level data the low nibble is one of the other
  15 values, the (CRC mod 15)th. `SaveData::Load` returns 1 for a record of
  another version, and `SaveMenu::LoadData` then resets the cartridge
  save.
- **`SAVE_CHECKSUM_SEED`**, the start value of the record's additive
  checksum (`UpdateChecksum`, `ChecksumOk`): 0 in the ROM, the CRC xor
  `ROM_LEVEL_DATA_CRC` otherwise, which is never 0. A record made with a
  different seed fails the checksum by the difference of the two seeds:
  `Load` returns 3 (reset), and a record received over the link cable
  gets "connection failed".

With the ROM's level data both constants are the ROM's numbers and the
ROM matches. With any other data:

| save made with | loads? |
|---|---|
| the same level data | yes |
| the ROM's levels (an original cartridge, or `baserom.gba`'s `.sav`) | no, always: the version's low nibble is never 2 and the seed is never 0 |
| other level data | no, unless the two CRC-32s are equal: about 1 in 2^32 for unrelated data, and never for data that differs in one span of 32 bits or less (CRC-32 detects every such burst). Since the version nibble is taken from the CRC, it adds nothing to that |
| a save of the other size (a 4 Kbit save in a 64 Kbit build, or the reverse) | no: the size follows from the level data, so the CRCs differ (the row above). The trailer isn't even where the other build reads it: a 64 Kbit build reads its `magic` past a 4 Kbit save's 0x200 bytes, and a 4 Kbit build reads a 64 Kbit save's slot data at 0x1F8 |

The version nibble alone would only give 15 values, a 1-in-15 chance
that two modified builds accept each other's saves; the seed is what
makes it 32 bits. It costs a 4 Kbit modified build the link bonus: it
checks the Crash 2/3 records against its own seed, so their checksum
fails ("connection failed"), and they will likely refuse its records for
the same reason. The original data keeps the bonus, and so does a 64 Kbit
build, whose link record is checked from the ROM's seed ("The link
cable").

## Numbers

41 rooms, 172 layers, 16,323 unique chunks (2.09 M cells; the full
tilemaps are 5.9 M cells), 3,794 entities. Data progress: the room data
(178,208 B) and the 7 raw assets (1,112,392 B + the 2-byte pad before
them stays raw) moved from `baserom.gba` to built sources; the 34 packed
assets were already counted as built when they were `.bin` files.
