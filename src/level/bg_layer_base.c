#include "core.h"
#include "bg_scroll_layer.h"
#include "level_data.h"
#include "level.h"
#include "globals.h"

/* Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

/* A viewport/parallax-scroll-layer object: `struct bg_scroll_layer`
 * (include/bg_scroll_layer.h; docs/rom_map.md's "Visual scrolling
 * background streamer" family is the sibling system built on the same
 * source-descriptor shape - see `ScrollBgStreamer` there), initialized from a
 * `struct level_layer_desc`. */

/* Applies the layer's scale-then-clamp step to `vec2` and accumulates
 * the (Q8, floor-divided) result into the layer's own position. */
void ScrollBgLayerBase(struct bg_scroll_layer *self, s32 *vec2)
{
    s32 scaled[2];
    s32 x = vec2[0];
    s32 y = vec2[1];

    scaled[0] = x;
    scaled[1] = y;
    ScaleBgLayerScroll(self, scaled);
    StepBgLayerScroll(self, scaled);
}

/* Same shape as `ScrollBgLayerBase`, but seeds the scale step directly from
 * `vec2` in place (no separate stack copy) and finishes by re-deriving
 * the layer's cached-tile buffers from its `streamer` instead of
 * accumulating a position. */
void ResetBgLayerBase(struct bg_scroll_layer *self, s32 *vec2)
{
    s32 x = vec2[0];
    s32 y = vec2[1];

    self->x = x;
    self->y = y;
    ScaleBgLayerScroll(self, self);
    FillBgStreamer(self->streamer, (s32 *)self);
}

/* (Re)initializes the layer from `source`: caches its pixel
 * dimensions/scroll bounds, resets the accumulated position to the
 * origin, and re-populates the `streamer` tile-cache sub-object from
 * the same descriptor. Does nothing (besides clearing the ready flag)
 * when `source` is NULL. */
void SetBgLayerSource(struct bg_scroll_layer *self, const struct level_layer_desc *source)
{
    u8 *readyFlag;
    s32 zero;

    readyFlag = &self->enabled;
    zero = 0;
    *readyFlag = zero;

    if (source != NULL) {
        s32 w, h;

        w = source->widthTiles;
        self->widthTiles = w;
        h = source->heightTiles;
        self->heightTiles = h;

        w <<= 3;
        self->widthPx = w;
        h <<= 3;
        self->heightPx = h;
        w -= 0xf0;
        self->maxX = w;
        h -= 0xa0;
        self->maxY = h;
        self->scaleX = source->scaleX;
        self->scaleY = source->scaleY;
        self->x = zero;
        self->y = zero;

        SetBgStreamerSource(self->streamer, (void *)source);
        FillBgStreamer(self->streamer, (s32 *)self);

        *readyFlag = 1;
    }
}

u8 IsBgLayerEnabled(struct bg_scroll_layer *self)
{
    return self->enabled;
}

s32 GetBgLayerY(struct bg_scroll_layer *self)
{
    return self->y;
}

s32 GetBgLayerX(struct bg_scroll_layer *self)
{
    return self->x;
}

s32 GetBgLayerHeightTiles(struct bg_scroll_layer *self)
{
    return self->heightTiles;
}

s32 GetBgLayerWidthTiles(struct bg_scroll_layer *self)
{
    return self->widthTiles;
}

s32 GetBgLayerHeight(struct bg_scroll_layer *self)
{
    return self->heightPx;
}

s32 GetBgLayerWidth(struct bg_scroll_layer *self)
{
    return self->widthPx;
}

/* Returns the cache slot holding decoded record recordId. On a miss,
 * decodes it (DecodeCollisionChunk) into the slot just behind the ring cursor and
 * advances the cursor. */
void *GetCollisionChunk(struct tile_cache *self, s32 recordId)
{
    if (recordId == self->id[0])
        return self->buf[0];
    if (recordId == self->id[1])
        return self->buf[1];
    if (recordId == self->id[2])
        return self->buf[2];
    if (recordId == self->id[3])
        return self->buf[3];
    if (recordId == self->id[4])
        return self->buf[4];
    if (recordId == self->id[5])
        return self->buf[5];
    if (recordId == self->id[6])
        return self->buf[6];
    if (recordId == self->id[7])
        return self->buf[7];
    if (recordId == self->id[8])
        return self->buf[8];
    if (recordId == self->id[9])
        return self->buf[9];
    if (recordId == self->id[10])
        return self->buf[10];
    if (recordId == self->id[11])
        return self->buf[11];
    if (recordId == self->id[12])
        return self->buf[12];
    if (recordId == self->id[13])
        return self->buf[13];
    if (recordId == self->id[14])
        return self->buf[14];
    if (recordId == self->id[15])
        return self->buf[15];
    {
        s32 slot = (self->nextSlot + 15) & 0xf;
        u8 *dest = self->buf[slot];

        DecodeCollisionChunk(self, recordId, dest);
        self->id[slot] = recordId;
        self->nextSlot = (self->nextSlot + 1) & 0xf;
        return dest;
    }
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r1` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

/* The decoded cell at pixel (x, y): 16x8-pixel tiles, one 256-byte cache
 * slot per tile record. */
static inline u16 GetCell(struct tile_cache *self, s32 x, s32 y)
{
    s32 tileX = x >> 4;
    s32 tileY = y >> 3;
    u16 *buf = GetCollisionChunk(self, (*(u16 **)self->source)[tileY * self->width + tileX]);
    return buf[(y & 7) * 16 + (x & 0xf)];
}

/* The terrain-property row (36 bytes, gTerrainHeights0) for the cell at
 * pixel (x, y), or NULL when out of bounds or the type is 0 or above 0x23.
 * The cell's flag nibble goes to a local nothing reads. */
void *GetTerrainHeights(struct tile_cache *self, s32 x, s32 y)
{
    u8 hi;
    u8 *hiOut = &hi;
    u16 cell;
    u8 nibble;
    s32 type;

    if (x < 0 || y < 0)
        return NULL;
    cell = GetCell(self, x, y);
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
void *GetSolidTerrainHeights(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    void *result = NULL;
    s32 hi = 0;
    s32 type;

    if (x < 0 || y < 0)
        type = 0;
    else
    {
        u16 cell = GetCell(self, x, y);
        u8 nibble;

        hi = cell >> 12;
        nibble = (cell >> 8) & 0xf;
        if (nibble)
            *flagsOut = nibble;
        type = cell & 0xff;
    }
    if (type <= 0x23)
        return NULL;
    switch (mode)
    {
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
s8 sub_8025228(struct tile_cache *self, s32 x, s32 y, s32 mode, u8 *flagsOut)
{
    s8 result = 0;
    s32 hi = 0;
    s32 type;

    if (x < 0 || y < 0)
        type = 0;
    else
    {
        u16 cell = GetCell(self, x, y);
        u8 nibble;

        hi = cell >> 12;
        nibble = (cell >> 8) & 0xf;
        if (nibble)
            *flagsOut = nibble;
        type = cell & 0xff;
    }
    if (type <= 0x23)
        return -1;
    switch (mode)
    {
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
 * via `self->decodeBase` indexed by `recordId`, then decodes a token
 * stream, budget-limited to 0x7f halfwords, with three run modes per
 * token byte - a literal-fill run, a signed-delta-accumulate run, and a
 * raw-copy run - writing the decoded halfwords into `dest` (a linear
 * 256-byte cache slot in `GetCollisionChunk`'s caller).
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
 * - `asm("" : : "r"(n))` after `n -= 2` (an extra-reference nudge that
 *   emits no code, see #468) gives `n` one more reference, so the pair
 *   loop's run counter wins r3 and `acc` keeps r4. Without it the
 *   allocator swaps the two. */
void DecodeCollisionChunk(struct tile_cache *self, s32 recordId, void *dest)
{
    u16 *out = dest;
    u16 *src = self->decodeBase;
    s32 budget;
    s32 written;

    src = (u16 *)((u32 *)src + src[recordId]);
    budget = 0x7F;
    written = 0;

    do
    {
        u16 token = *src;
        u16 n = *(u8 *)src;

        src++;
        if (token & 0x8000)
        {
            u16 value = *src++;

            budget -= n;
            do
            {
                out[written] = value;
                written++;
                n--;
            } while (n != 0);
        }
        else if (token & 0x4000)
        {
            s16 acc;

            budget -= n;
            acc = *src++;
            out[written] = acc;
            n--;
            written++;
            do
            {
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
                asm("" : : "r"(n));
            } while (n > 1);
            if (n != 0)
            {
                u16 last = *src++;
                s32 lo = last << 24;
                s32 a = acc;

                out[written] = a + (lo >> 24);
                written++;
            }
        }
        else
        {
            budget -= n;
            do
            {
                out[written] = *src++;
                written++;
                n--;
            } while (n != 0);
        }
    } while (budget >= 0);
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");
