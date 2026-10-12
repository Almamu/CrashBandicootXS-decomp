# The terrain types, collision modes and open bits (#843)

The level collision map (TileCache, include/bg_layer.hpp) has one 16-bit
cell per 8x8 tile. The names are in include/level.h, next to
`struct terrain_type`.

## The cell's terrain type (bits 0-7)

| Value | Name | Meaning |
|---|---|---|
| 0 | `TERRAIN_TYPE_NONE` | empty; also what GetSolidTerrainHeights / GetSolidTerrainModeValue use off the map (`x < 0 \|\| y < 0`) |
| 1-0x23 | (up to `TERRAIN_TYPE_LAST_HEIGHTS`) | a type with a height per pixel column: GetTerrainHeights returns its `gTerrainHeights0` row, the solid lookups return NULL / -1 |
| 0x24-0xFF | | solid: GetSolidTerrainHeights returns its row of the mode's table, GetSolidTerrainModeValue its `modeValue` |

Bits 8-11 are the flag nibble the lookups hand back in `flagsOut`
(LevelLayers::ProbeX/ProbeY store it in `kind` while `probeFlag` is set).

## The collision mode, `TERRAIN_MODE_*`

GetSolidTerrainHeights' and GetSolidTerrainModeValue's `mode`, and
LevelLayers::ProbeX's and ProbeY's `submode`, is the direction the probe
moves in; it indexes `modeValue[]` and `heights[]` (gTerrainHeights0-3).
LevelLayers::Probe picks it from its PART_DIR_* direction (box_part.h):

| Probe's `mode` | Resolver | `TERRAIN_MODE_*` | On a hit | Short-circuit |
|---|---|---|---|---|
| `PART_DIR_DOWN` (8) | ProbeY | `DOWN` (0) | `*outValue` moves up to the cell's top | past the layer's `heightPx`: a hit at the bottom edge |
| `PART_DIR_RIGHT` (1) | ProbeX | `RIGHT` (1) | moves left of the cell, one unit past it | past `widthPx`: a hit at the right edge |
| `PART_DIR_UP` (4) | ProbeY | `UP` (2) | moves down below the cell | none |
| `PART_DIR_LEFT` (2) | ProbeX | `LEFT` (3) | moves right of the cell, one unit past it | `x < 0`: a hit at 0 |

Probe's callers pass `dir & PART_DIR_X_MASK` / `PART_DIR_Y_MASK`
(GroundSprite::ProbeTerrainAxes; MovingSprite::ProbeEdgeTerrain gets `PART_DIR_DOWN`) or, in
Player::HasRoomForAnim, `PART_DIR_LEFT`/`RIGHT` from the facing.
ProbeSolidFloor (terrain.cpp) reads the `DOWN` value.

Probe's `PART_DIR_RIGHT` arm passes its own `mode` (1, which is
`TERRAIN_MODE_RIGHT`) on to ProbeX, as the ROM does; it has a comment.

## The open bits (bits 12-15), `TERRAIN_FLAG_OPEN_*`

A solid cell is skipped by one mode's probes when its bit is set: the
lookups then return NULL / 0.

| Bit | Name | Mode it opens |
|---|---|---|
| 1 | `TERRAIN_FLAG_OPEN_RIGHT` | `TERRAIN_MODE_RIGHT` |
| 2 | `TERRAIN_FLAG_OPEN_LEFT` | `TERRAIN_MODE_LEFT` |
| 4 | `TERRAIN_FLAG_OPEN_DOWN` | `TERRAIN_MODE_DOWN` |
| 8 | `TERRAIN_FLAG_OPEN_UP` | `TERRAIN_MODE_UP` |

The `RIGHT` arms of both lookups test `hi & mode` (mode is 1 there), the
register the ROM ANDs; they keep that spelling with a comment.

The collision layers themselves are images now: `data/levels/<room>/collision.bmp`,
one colour per cell value, legend in [levels.md, "Collision layers"](../levels.md#collision-layers).
