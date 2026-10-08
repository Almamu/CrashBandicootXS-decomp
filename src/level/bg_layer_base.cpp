#include "bg_layer.hpp"

extern "C" {
#include "globals.h"
}

/* Built with old_agbcp - see docs/matching/archive/game-loop-old-agbcc.md. */

/* BgLayerBase's other methods (include/bg_layer.hpp; the constructor,
 * destructor and scroll steps are in src/cutscene/cutscene_player.cpp),
 * then the collision tile cache's lookups (TileCache's, include/bg_layer.hpp;
 * free functions taking a `struct tile_cache *` until #752). */

/* Scales the move `delta` by the layer's parallax factors and steps the
 * layer's position toward the result. */
void BgLayerBase::Scroll(const s32 *delta)
{
    s32 scaled[2];
    s32 dx = delta[0];
    s32 dy = delta[1];

    scaled[0] = dx;
    scaled[1] = dy;
    ScaleScroll(scaled);
    StepScroll(scaled);
}

/* Sets the position to `pos` scaled by the parallax factors, and seeds
 * the streamer's window there. */
void BgLayerBase::Reset(const s32 *pos)
{
    s32 px = pos[0];
    s32 py = pos[1];

    x = px;
    y = py;
    ScaleScroll(&x);
    streamer->Fill(&x);
}

/* (Re)initializes the layer from `source`: caches its pixel
 * dimensions/scroll bounds, resets the accumulated position to the
 * origin, and re-populates the streamer from the same descriptor. Does
 * nothing (besides clearing the enabled flag) when `source` is NULL. */
void BgLayerBase::SetSource(const struct level_layer_desc *source)
{
    u8 *readyFlag;
    s32 zero;

    readyFlag = &enabled;
    zero = 0;
    *readyFlag = zero;

    if (source != NULL) {
        s32 w, h;

        w = source->widthTiles;
        widthTiles = w;
        h = source->heightTiles;
        heightTiles = h;

        w <<= 3;
        widthPx = w;
        h <<= 3;
        heightPx = h;
        w -= 0xf0;
        maxX = w;
        h -= 0xa0;
        maxY = h;
        scaleX = source->scaleX;
        scaleY = source->scaleY;
        x = zero;
        y = zero;

        streamer->SetSource(source);
        streamer->Fill(&x);

        *readyFlag = 1;
    }
}

/* The accessors below are UNUSED: no caller anywhere in the ROM. */
u8 BgLayerBase::IsEnabled()
{
    return enabled;
}

s32 BgLayerBase::GetY()
{
    return y;
}

s32 BgLayerBase::GetX()
{
    return x;
}

s32 BgLayerBase::GetHeightTiles()
{
    return heightTiles;
}

s32 BgLayerBase::GetWidthTiles()
{
    return widthTiles;
}

s32 BgLayerBase::GetHeight()
{
    return heightPx;
}

s32 BgLayerBase::GetWidth()
{
    return widthPx;
}

/* Returns the cache slot holding decoded record recordId. On a miss,
 * decodes it (DecodeChunk) into the slot just behind the ring cursor and
 * advances the cursor. */
void *TileCache::GetChunk(s32 recordId)
{
    if (recordId == id[0])
        return buf[0];
    if (recordId == id[1])
        return buf[1];
    if (recordId == id[2])
        return buf[2];
    if (recordId == id[3])
        return buf[3];
    if (recordId == id[4])
        return buf[4];
    if (recordId == id[5])
        return buf[5];
    if (recordId == id[6])
        return buf[6];
    if (recordId == id[7])
        return buf[7];
    if (recordId == id[8])
        return buf[8];
    if (recordId == id[9])
        return buf[9];
    if (recordId == id[10])
        return buf[10];
    if (recordId == id[11])
        return buf[11];
    if (recordId == id[12])
        return buf[12];
    if (recordId == id[13])
        return buf[13];
    if (recordId == id[14])
        return buf[14];
    if (recordId == id[15])
        return buf[15];
    {
        s32 slot = (nextSlot + 15) & 0xf;
        u8 *dest = buf[slot];

        DecodeChunk(recordId, dest);
        id[slot] = recordId;
        nextSlot = (nextSlot + 1) & 0xf;
        return dest;
    }
}

/* The terrain-property row (36 bytes, gTerrainHeights0) for the cell at
 * pixel (x, y), or NULL when out of bounds or the type is 0 or above 0x23.
 * The cell's flag nibble goes to a local nothing reads. */
void *TileCache::GetTerrainHeights(s32 x, s32 y)
{
    u8 hi;
    u8 *hiOut = &hi;
    u16 cell;
    u8 nibble;
    s32 type;

    if (x < 0 || y < 0)
        return NULL;
    cell = CellAt(x, y);
    nibble = (cell >> 8) & 0xf;
    if (nibble)
        *hiOut = nibble;
    type = cell & 0xff;
    if (type == 0 || type > 0x23)
        return NULL;
    return gTerrainHeights0 + type * 36;
}

/* Like GetTerrainHeights with a collision mode (0-3): each mode has its own
 * property table and its own "not solid" bit in the cell's top nibble.
 * The flag nibble is written to flagsOut. */
void *TileCache::GetSolidTerrainHeights(s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    void *result = NULL;
    s32 hi = 0;
    s32 type;

    if (x < 0 || y < 0)
        type = 0;
    else {
        u16 cell = CellAt(x, y);
        u8 nibble;

        hi = cell >> 12;
        nibble = (cell >> 8) & 0xf;
        if (nibble)
            *flagsOut = nibble;
        type = cell & 0xff;
    }
    if (type <= 0x23)
        return NULL;
    switch (mode) {
    case 0:
        if (hi & 4)
            result = NULL;
        else
            result = gTerrainHeights0 + type * 36;
        break;
    case 1:
        if (hi & mode)
            result = NULL;
        else
            result = gTerrainHeights1 + type * 36;
        break;
    case 2:
        if (hi & 8)
            result = NULL;
        else
            result = gTerrainHeights2 + type * 36;
        break;
    case 3:
        if (hi & 2)
            result = NULL;
        else
            result = gTerrainHeights3 + type * 36;
        break;
    }
    return result;
}

/* The mode byte (0-3) of the cell's terrain type at pixel (x, y): -1
 * when out of bounds or the type is 0x23 or below, 0 when the mode's
 * "not solid" bit is set in the cell's top nibble. */
s8 TileCache::GetSolidTerrainModeValue(s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    s8 result = 0;
    s32 hi = 0;
    s32 type;

    if (x < 0 || y < 0)
        type = 0;
    else {
        u16 cell = CellAt(x, y);
        u8 nibble;

        hi = cell >> 12;
        nibble = (cell >> 8) & 0xf;
        if (nibble)
            *flagsOut = nibble;
        type = cell & 0xff;
    }
    if (type <= 0x23)
        return -1;
    switch (mode) {
    case 0:
        if (hi & 4)
            result = 0;
        else
            result = gTerrainTypes[type].modeValue[0];
        break;
    case 1:
        if (hi & mode)
            result = 0;
        else
            result = gTerrainTypes[type].modeValue[1];
        break;
    case 2:
        if (hi & 8)
            result = 0;
        else
            result = gTerrainTypes[type].modeValue[2];
        break;
    case 3:
        if (hi & 2)
            result = 0;
        else
            result = gTerrainTypes[type].modeValue[3];
        break;
    }
    return result;
}

/* Custom RLE/delta token-stream decoder (docs/rom_map.md's "A new find:
 * a custom RLE/delta token-stream decoder"): looks up a base pointer
 * via `decodeBase` indexed by `recordId`, then decodes a token
 * stream, budget-limited to 0x7f halfwords, with three run modes per
 * token byte - a literal-fill run, a signed-delta-accumulate run, and a
 * raw-copy run - writing the decoded halfwords into `dest` (a linear
 * 256-byte cache slot of `GetChunk`).
 *
 * Matched (near-miss sweep 2, old_agbcc). Three pieces closed the old
 * 33-halfword gap:
 * - `src` is first loaded with `decodeBase` itself and then advanced by
 *   the record's word offset, so the base lives in `src`'s register (r6)
 *   rather than a scratch one.
 * - The delta run's sign extensions are spelled as explicit `<< 24` /
 *   `<< 16` shifts into an `s32` local, then `acc` is copied into an `s32`
 *   before the `>> 24`. That makes gcc emit the pair's left shift, then
 *   `acc`'s own `lsl/asr #16`, then the pair's `asr #24`, which is the
 *   ROM's interleaving; `(s8)pair` emits the two shifts back to back.
 * - The token's mode bits are tested through an inline (`TokenHas`): the
 *   pair loop's run counter then wins r3 and `acc` keeps r4. With the two
 *   `&` tests written out the allocator swaps the two; until #662 round 2
 *   an extra `MATCH_USE(n)` reference stood in for the inline. */
static inline s32 TokenHas(u16 token, s32 flag)
{
    return token & flag;
}

void TileCache::DecodeChunk(s32 recordId, void *dest)
{
    u16 *out = (u16 *)dest;
    u16 *src = (u16 *)decodeBase;
    s32 budget;
    s32 written;

    src = (u16 *)((u32 *)src + src[recordId]);
    budget = 0x7F;
    written = 0;

    do {
        u16 token = *src;
        u16 n = *(u8 *)src;

        src++;
        if (TokenHas(token, CHUNK_TOKEN_FILL)) {
            u16 value = *src++;

            budget -= n;
            do {
                out[written] = value;
                written++;
                n--;
            } while (n != 0);
        } else if (TokenHas(token, CHUNK_TOKEN_DELTA)) {
            s16 acc;

            budget -= n;
            acc = *src++;
            out[written] = acc;
            n--;
            written++;
            do {
                u16 pair = *src++;

                {
                    s32 lo = pair << 24;
                    s32 a = acc;

                    acc = a + (lo >> 24);
                }
                out[written++] = acc;
                {
                    s32 hi = pair << 16;
                    s32 a = acc;

                    acc = a + (hi >> 24);
                }
                out[written++] = acc;
                n -= 2;
            } while (n > 1);
            if (n != 0) {
                u16 last = *src++;
                s32 lo = last << 24;
                s32 a = acc;

                out[written] = a + (lo >> 24);
                written++;
            }
        } else {
            budget -= n;
            do {
                out[written] = *src++;
                written++;
                n--;
            } while (n != 0);
        }
    } while (budget >= 0);
}
