#include "core.h"

/* GitHub issues #9/#10/#41's remaining piece of `sub_8026628`'s own
 * "umbrella 5-mode dispatcher" cluster (`game_loop43.c`,
 * docs/matching/issue-9-10-41-0x08026628-game-loop.md): that pass fully
 * derived both of these axis resolvers' semantics from their own raw
 * bytes (see that doc's "`sub_8026AE8`/`sub_8026A18`: which axis each
 * one actually resolves" section). Both are plain C, built with
 * old_agbcc - see docs/matching/game-loop-old-agbcc.md.
 *
 * `s32 fn(struct collider *self, struct probe_pos *pos, s32 span, s32 *outValue,
 * s32 submode)`:
 *
 *   - `sub_8026A18` (the Y-axis/floor-ceiling resolver, `mode ==
 *     4`/`8` in `sub_8026628`): scans `pos->x` over `[x, x+span-1]>>3`
 *     at a fixed `pos->y>>3` tile row, clamping the start index up to
 *     0 if it computes to exactly -1 and the end index down by one if
 *     it lands exactly on `(*(struct tile_cache **)(self+0x20))+0x10`
 *     (that cache's own cached width-in-tiles field, `struct
 *     tile_cache::unk010` in `game_loop3.c` - confirmed genuinely read
 *     here, unlike that struct's own comment there which predates this
 *     pass), calling `GetSolidTerrainHeights(self->0x20, tileX, tileY, submode,
 *     &scratch)` (matched, `game_loop3.c`) per tile until a hit or the
 *     range is exhausted. On a hit, accumulates into `*outValue` using
 *     `pos->y & 7`: `submode == 2` adds `(8-(y&7))<<8`, `submode == 0`
 *     subtracts `(y&7)<<8` (any other submode value leaves `*outValue`
 *     untouched on a hit - dead code in practice, since `sub_8026628`
 *     only ever passes `0`/`2` here).
 *   - `sub_8026AE8` (the X-axis/wall resolver, `mode == 1`/`2`): the
 *     same shape, scanning `pos->y` over the same span at a fixed
 *     `pos->x>>3` tile column, clamped against `unk014` (the cache's
 *     cached height-in-tiles field) instead of `unk010`. On a hit,
 *     using `pos->x & 7`: `submode == 3` adds `1+(8-(x&7))<<8`
 *     (`(*outValue+1)` is read before the shift-add, giving the extra
 *     `+1` epsilon term `sub_8026628`'s own doc flagged as this
 *     resolver's asymmetry versus the Y-axis one), `submode == 1`
 *     subtracts `(x&7)<<8` from `(*outValue-1)` (any other submode
 *     value again leaves `*outValue` untouched - `sub_8026628` only
 *     ever passes `1`/`3` here).
 *   - Both finish with the same tail: if `self+0x2a` (the "flag held
 *     set" byte `docs/matching/issue-9-10-0x0800a884-graphics.md`
 *     already named for this same `self`/`gLevelLayers`-shaped
 *     object) is nonzero *and* the scan's own scratch out-flag from
 *     its last `GetSolidTerrainHeights` call is nonzero, writes that scratch byte
 *     into `self+0x29` (the same doc's "dispatch nibble" byte).
 *     Returns the scan's own hit flag either way. */

struct probe_pos
{
    s32 x;
    s32 y;
};

/* game_loop3.c's `struct tile_cache`, as far as these read it. */
struct tile_cache
{
    u8 unk_00[0x10];
    s32 unk010;         // 0x10 - width in tiles
    s32 unk014;         // 0x14 - height in tiles
};

/* The gLevelLayers-shaped collider these resolvers run on. */
struct collider
{
    u8 unk_00[0x20];
    struct tile_cache *cache;   // 0x20
    u8 unk_24[5];
    u8 nibble;                  // 0x29
    u8 flagHeld;                // 0x2A
};

extern void *GetSolidTerrainHeights(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut);

/* Y-axis resolver: scans the tiles under [pos->x, pos->x + span) at
 * pos->y's row until GetSolidTerrainHeights reports a hit. On a hit, moves
 * *outValue to the tile edge (down for submode 2, up for submode 0).
 * Returns whether anything was hit. */
s32 sub_8026A18(struct collider *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode)
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
    if (end == self->cache->unk010)
        end--;
    for (; x <= end && !hit; x++)
    {
        if (GetSolidTerrainHeights(self->cache, x, y, submode, &flag))
            hit = 1;
    }
    if (hit)
    {
        switch (submode)
        {
        case 2:
            *outValue += (8 - (pos->y & 7)) << 8;
            break;
        case 0:
            *outValue -= (pos->y & 7) << 8;
            break;
        }
    }
    if (self->flagHeld && flag)
        self->nibble = flag;
    return hit;
}

/* X-axis resolver: scans the tiles beside [pos->y, pos->y + span) at
 * pos->x's column until GetSolidTerrainHeights reports a hit. On a hit, moves
 * *outValue to the tile edge (right for submode 3, left for submode 1),
 * one unit past it. Returns whether anything was hit. */
s32 sub_8026AE8(struct collider *self, struct probe_pos *pos, s32 span, s32 *outValue, s32 submode)
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
    if (end == self->cache->unk014)
        end--;
    for (; y <= end && !hit; y++)
    {
        if (GetSolidTerrainHeights(self->cache, x, y, submode, &flag))
            hit = 1;
    }
    if (hit)
    {
        switch (submode)
        {
        case 3:
        {
            s32 v = *outValue + 1;
            *outValue = v + ((8 - (pos->x & 7)) << 8);
        }
            break;
        case 1:
        {
            s32 v = *outValue - 1;
            *outValue = v - ((pos->x & 7) << 8);
        }
            break;
        }
    }
    if (self->flagHeld && flag)
        self->nibble = flag;
    return hit;
}

/* Zero-fill the trailing halfword, as the ROM does. */
asm(".align 2, 0");
