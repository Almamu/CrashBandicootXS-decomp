#include "bg_layer.hpp"

extern "C" {
#include "core.h"
#include "math_util.h"
#include "level.h"
}

/* GitHub issues #9/#10/#41's remaining piece of `ProbeTerrain`'s own
 * "umbrella 5-mode dispatcher" cluster (`terrain_probe.cpp`,
 * docs/matching/archive/issue-9-10-41-0x08026628-game-loop.md): that pass fully
 * derived both of these axis resolvers' semantics from their own raw
 * bytes (see that doc's "`ProbeTerrainX`/`ProbeTerrainY`: which axis each
 * one actually resolves" section). Both are plain C, built with
 * old_agbcc - see docs/matching/archive/game-loop-old-agbcc.md.
 *
 * `s32 fn(LevelLayers *self, struct vec2 *pos, s32 span, s32 *outValue,
 * s32 submode)`:
 *
 *   - `ProbeTerrainY` (the Y-axis/floor-ceiling resolver, `mode ==
 *     4`/`8` in `ProbeTerrain`): scans `pos->x` over `[x, x+span-1]>>3`
 *     at a fixed `pos->y>>3` tile row, clamping the start index up to
 *     0 if it computes to exactly -1 and the end index down by one if
 *     it lands exactly on `self->tiles->widthTiles` (the tile cache's
 *     width in tiles, TileCache in include/bg_layer.hpp), calling
 *     `self->tiles->GetSolidTerrainHeights(tileX, tileY, submode,
 *     &scratch)` (matched, `tile_cache.cpp`) per tile until a hit or the
 *     range is exhausted. On a hit, accumulates into `*outValue` using
 *     `pos->y & 7`: `submode == 2` adds `(8-(y&7))<<8`, `submode == 0`
 *     subtracts `(y&7)<<8` (any other submode value leaves `*outValue`
 *     untouched on a hit - dead code in practice, since `ProbeTerrain`
 *     only ever passes `0`/`2` here).
 *   - `ProbeTerrainX` (the X-axis/wall resolver, `mode == 1`/`2`): the
 *     same shape, scanning `pos->y` over the same span at a fixed
 *     `pos->x>>3` tile column, clamped against `heightTiles` (the cache's
 *     height in tiles) instead of `widthTiles`. On a hit,
 *     using `pos->x & 7`: `submode == 3` adds `1+(8-(x&7))<<8`
 *     (`(*outValue+1)` is read before the shift-add, giving the extra
 *     `+1` epsilon term `ProbeTerrain`'s own doc flagged as this
 *     resolver's asymmetry versus the Y-axis one), `submode == 1`
 *     subtracts `(x&7)<<8` from `(*outValue-1)` (any other submode
 *     value again leaves `*outValue` untouched - `ProbeTerrain` only
 *     ever passes `1`/`3` here).
 *   - Both finish with the same tail: if `self+0x2a` (the "flag held
 *     set" byte `docs/matching/archive/issue-9-10-0x0800a884-graphics.md`
 *     already named for this same `self`/`gLevelLayers`-shaped
 *     object) is nonzero *and* the scan's own scratch out-flag from
 *     its last `GetSolidTerrainHeights` call is nonzero, writes that scratch byte
 *     into `self+0x29` (the same doc's "dispatch nibble" byte).
 *     Returns the scan's own hit flag either way. */

/* Y-axis resolver: scans the tiles under [pos->x, pos->x + span) at
 * pos->y's row until GetSolidTerrainHeights reports a hit. On a hit, moves
 * *outValue to the tile edge (down for submode 2, up for submode 0).
 * Returns whether anything was hit. */
s32 ProbeTerrainY(LevelLayers *self, struct vec2 *pos, s32 span, s32 *outValue, s32 submode)
{
    s32 hit = 0;
    u8 flag = hit;
    s32 y = pos->y;
    s32 x = pos->x;
    s32 end = x + span - 1;

    x >>= 3;
    y >>= 3;
    end >>= 3;
    if (x == -1)
        x = 0;
    if (end == self->tiles->widthTiles)
        end--;
    for (; x <= end && !hit; x++) {
        if (self->tiles->GetSolidTerrainHeights(x, y, submode, &flag))
            hit = 1;
    }
    if (hit) {
        switch (submode) {
        case 2:
            *outValue += INT_TO_Q8(8 - (pos->y & 7));
            break;
        case 0:
            *outValue -= INT_TO_Q8(pos->y & 7);
            break;
        }
    }
    if (self->probeFlag && flag)
        self->kind = flag;
    return hit;
}

/* X-axis resolver: scans the tiles beside [pos->y, pos->y + span) at
 * pos->x's column until GetSolidTerrainHeights reports a hit. On a hit, moves
 * *outValue to the tile edge (right for submode 3, left for submode 1),
 * one unit past it. Returns whether anything was hit. */
s32 ProbeTerrainX(LevelLayers *self, struct vec2 *pos, s32 span, s32 *outValue, s32 submode)
{
    s32 hit = 0;
    u8 flag = hit;
    s32 y = pos->y;
    s32 x = pos->x;
    s32 end = y + span - 1;

    x >>= 3;
    y >>= 3;
    end >>= 3;
    if (y == -1)
        y = 0;
    if (end == self->tiles->heightTiles)
        end--;
    for (; y <= end && !hit; y++) {
        if (self->tiles->GetSolidTerrainHeights(x, y, submode, &flag))
            hit = 1;
    }
    if (hit) {
        switch (submode) {
        case 3:
            {
                s32 v = *outValue + 1;
                *outValue = v + INT_TO_Q8(8 - (pos->x & 7));
            }
            break;
        case 1:
            {
                s32 v = *outValue - 1;
                *outValue = v - INT_TO_Q8(pos->x & 7);
            }
            break;
        }
    }
    if (self->probeFlag && flag)
        self->kind = flag;
    return hit;
}
