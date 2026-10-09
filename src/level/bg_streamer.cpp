#include "sprite_obj.hpp"
#include "bg_layer.hpp"
#include "font.hpp"
#include "cutscene.hpp"

extern "C" {
#include "math_util.h"
#include "text.h"
#include "cutscene.h"
#include <libgcc.h>
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* GitHub issue #39: 0x08024960-0x08024D74 (game_loop), split from
 * src/cutscene/cutscene_player.cpp (#770). docs/rom_map.md's "A new
 * find: a custom RLE/delta token-stream decoder" and its two follow-up
 * sections ("Follow-up: resolved the semantics by tracing callers", "The
 * background streamer's missing 'level load' half") characterized it
 * from a read-only pass.
 *
 * `BgStreamer` (gBgStreamerVtable) is the "visual scrolling background
 * streamer" docs/rom_map.md names: a circular 4x4-block (64 halfword
 * columns x 32 rows, 0x1000 bytes total) ring-buffer tilemap fed by the
 * same custom RLE/delta token-stream decoder (`DecodeChunk`) the
 * terrain-tile cache's `DecodeCollisionChunk` (tile_cache.cpp) also
 * uses, just writing into a 2D buffer (row stride 64 halfwords) instead
 * of a flat one. `Scroll` is the per-frame driver: it right-shifts the
 * world position by 7/6 (128/64 px tile granularity), and on each axis
 * the camera crosses a tile boundary, streams in exactly the
 * newly-exposed row (`StreamRow`) or column (`StreamColumn`), while a
 * mod-4 "sub-block" accumulator (`subX`/`subY`) tracks which of the
 * ring buffer's 4 blocks is now the logical edge. `Fill` is the "level
 * load" half - seeds the ring buffer's *entire* initial contents the
 * same way, from a fresh camera position. `GetColumn`/`GetRow`/`GetCell`
 * convert a world pixel position into the ring buffer's wrapped (col,
 * row) address. The class is in include/bg_layer.hpp; its destructor is
 * its key method, so g++ emits its vtable here.
 *
 * Built with old_agbcp (the Makefile's OLD_AGBCC_OBJS) - see
 * docs/matching/archive/game-loop-old-agbcc.md. */

/* The custom RLE/delta token-stream decoder (docs/rom_map.md's "A new
 * find" section): looks up a base pointer via `records` indexed by
 * `recordId`'s halfword table entry, then decodes a token stream,
 * budget-limited to 0x7f halfwords, with the same three run modes
 * (literal-fill, signed-delta-accumulate, raw-copy) as the terrain-tile
 * cache's `DecodeCollisionChunk` (tile_cache.cpp) - just writing into a
 * 2D buffer (row = idx>>4, 64-halfword row stride) instead of a flat one.
 *
 * Matched (old_agbcc) by porting `DecodeCollisionChunk`'s matched shape: `src`
 * starts as the record table itself, and the delta run's sign extensions
 * are explicit `<< 24` shifts into `s32` locals with `acc` copied to an
 * `s32` first. Two local changes: the odd trailing delta is stored back
 * into `acc` before the cell store, and the copy loop builds its cell
 * index in a local `k` (row first), which gives the ROM's
 * `asr; lsl` order there. */
#define RING_CELL(out, i) (out)[((i) >> 4) * 64 + ((i) & 0xf)]

/* A token's run mode is one of the CHUNK_TOKEN_* bits
 * (constants/chunk_tokens.h, shared with DecodeCollisionChunk); its low
 * byte is the run length, and with neither bit set the run is copied raw. */
void BgStreamer::DecodeChunk(s32 recordId, u16 *dest)
{
    u16 *out = dest;
    const u16 *src = records;
    s32 budget;
    s32 written;

    src = (const u16 *)((const u32 *)src + src[recordId]);
    budget = 0x7F;
    written = 0;

    do {
        u16 token = *src;
        u16 n = *(const u8 *)src;

        src++;
        if (token & CHUNK_TOKEN_FILL) {
            u16 value = *src++;

            budget -= n;
            do {
                RING_CELL(out, written) = value;
                written++;
                n--;
            } while (n != 0);
        } else if (token & CHUNK_TOKEN_DELTA) {
            s16 acc;

            budget -= n;
            acc = *src++;
            RING_CELL(out, written) = acc;
            n--;
            written++;
            do {
                u16 pair = *src++;

                {
                    s32 lo = pair << 24;
                    s32 a = acc;

                    acc = a + (lo >> 24);
                }
                RING_CELL(out, written) = acc;
                written++;
                {
                    s32 hi = pair << 16;
                    s32 a = acc;

                    acc = a + (hi >> 24);
                }
                RING_CELL(out, written) = acc;
                written++;
                n -= 2;
            } while (n > 1);
            if (n != 0) {
                u16 last = *src++;
                s32 lo = last << 24;
                s32 a = acc;

                acc = a + (lo >> 24);
                RING_CELL(out, written) = acc;
                written++;
            }
        } else {
            budget -= n;
            do {
                s32 k = written >> 4;

                k = k * 64 + (written & 0xf);
                out[k] = *src++;
                written++;
                n--;
            } while (n != 0);
        }
    } while (budget >= 0);
}

/* Per-frame background-streamer driver (docs/rom_map.md: "Visual
 * scrolling background streamer"): right-shifts the world position by
 * 7/6 (128/64px tile granularity) and, on each axis the camera has
 * crossed a tile boundary since last call, streams in exactly the
 * newly-exposed column (`StreamColumn`) or row (`StreamRow`) - the
 * tile 4 ahead of the old edge when scrolling forward, or the tile
 * directly behind when scrolling back - while advancing the mod-4
 * sub-block accumulator (`subX`/`subY`) that feeds
 * `GetColumn`/`GetRow`/`GetCell`'s wrapped addressing. */
void BgStreamer::Scroll(const s32 *worldpos)
{
    s32 newX = worldpos[0] >> 7;
    s32 newY = worldpos[1] >> 6;
    s32 oldX = tileX;
    s32 oldY = tileY;

    if (newX > oldX) {
        StreamColumn(oldX + 4);
        subX = (subX + 1) & 3;
    } else if (newX < oldX) {
        subX = (subX - 1) & 3;
        StreamColumn(oldX - 1);
    }
    tileX = newX;

    if (newY > oldY) {
        StreamRow(oldY + 4);
        subY = (subY + 1) & 3;
    } else if (newY < oldY) {
        subY = (subY - 1) & 3;
        StreamRow(oldY - 1);
    }
    tileY = newY;
}

/* Converts a world pixel `(x, y)` into the ring buffer's address (see
 * this file's header comment): `tileX`/`tileY` are the last-known chunk
 * coordinates, `subX`/`subY` the mod-4 sub-block accumulator `Scroll`
 * advances. Returns the column-wrapped halfword pointer (col wrapped mod
 * 0x40) and writes the row (wrapped mod 0x20) out through `rowOut`. */
u16 *BgStreamer::GetColumn(s32 x, s32 y, s32 *rowOut)
{
    s32 col = x - tileX * 16;
    s32 row = y - tileY * 8;
    u8 sx;
    u8 sy;
    s32 shifted;
    u8 *base;
    u16 *ret;

    sx = subX;
    shifted = sx * 16;
    col += shifted;
    sy = subY;
    shifted = sy * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = ring;
    col <<= 1;
    ret = (u16 *)(base + col);
    *rowOut = row;
    return ret;
}

/* Sibling of `GetColumn`: same wrapped `(col, row)` computation, but
 * returns the row-wrapped pointer (row stride 0x80 bytes = 0x40
 * halfwords, matching `DecodeChunk`'s own row stride) and writes the
 * column out through `colOut` instead. */
u16 *BgStreamer::GetRow(s32 x, s32 y, s32 *colOut)
{
    s32 col = x - tileX * 16;
    s32 row = y - tileY * 8;
    u8 sx;
    u8 sy;
    s32 shifted;
    u8 *base;
    u16 *ret;

    sx = subX;
    shifted = sx * 16;
    col += shifted;
    sy = subY;
    shifted = sy * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = ring;
    row <<= 7;
    ret = (u16 *)(base + row);
    *colOut = col;
    return ret;
}

/* Combines `GetColumn`/`GetRow`'s address computation and returns the
 * decoded halfword value directly, with no out-param. UNUSED: no caller
 * anywhere in the ROM. */
u16 BgStreamer::GetCell(s32 x, s32 y)
{
    s32 col = x - tileX * 16;
    s32 row = y - tileY * 8;
    u8 sx;
    u8 sy;
    s32 shifted;
    u8 *base;
    s32 idx;

    sx = subX;
    shifted = sx * 16;
    col += shifted;
    sy = subY;
    shifted = sy * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = ring;
    idx = row << 6;
    idx += col;
    idx <<= 1;
    return *(u16 *)(base + idx);
}

/* Streams in the newly exposed tile row `row`: decodes each of its up to
 * 4 in-bounds tiles into the row's ring-buffer blocks. */
void BgStreamer::StreamRow(s32 row)
{
    s32 idx;
    s32 i;

    if (row < source->gridHeight) {
        idx = source->gridWidth;
        idx *= row;
        idx += tileX;
        for (i = 0; i <= 3; i++) {
            if (i + tileX < source->gridWidth) {
                u8 *dest = ring;
                dest += ((subY + 4) & 3) << 10;
                dest += ((subX + i + 4) & 3) << 5;
                DecodeChunk(source->chunkGrid[idx++], (u16 *)dest);
            }
        }
    }
}

/* Streams in the newly exposed tile column `col`: decodes each of its up
 * to 4 in-bounds tiles into the column's ring-buffer blocks. */
void BgStreamer::StreamColumn(s32 col)
{
    s32 idx;
    s32 i;

    if (col < source->gridWidth) {
        idx = tileY * source->gridWidth;
        idx += col;
        for (i = 0; i <= 3; i++) {
            const struct level_layer_desc *src;

            if (i + tileY < (src = source)->gridHeight) {
                u8 *dest = ring;
                u16 id;

                dest += ((subY + i + 4) & 3) << 10;
                dest += ((subX + 4) & 3) << 5;
                id = src->chunkGrid[idx];
                idx += src->gridWidth;
                DecodeChunk(id, (u16 *)dest);
            }
        }
    }
}

/* Seeds the whole ring buffer for a fresh camera position `pos` (Q8
 * world x/y): resets the sub-block origin, derives the top-left tile
 * (128x64 px tiles) and decodes all 4x4 in-bounds tiles. The row stride
 * and block height stay in registers across the loops, as in the ROM. */
void BgStreamer::Fill(const s32 *pos)
{
    s32 rowLen = 0x40;
    s32 blockH = 0x20;
    s32 j;
    s32 i;

    subX = 0;
    subY = 0;
    tileX = pos[0] >> 7;
    tileY = pos[1] >> 6;
    for (j = 0; j <= 3; j++) {
        if (j + tileY < source->gridHeight) {
            s32 rowBase;

            for (i = 0, rowBase = rowLen * (j << 3); i <= 3; i++) {
                s32 x = i + tileX;
                const struct level_layer_desc *src;

                if (x < (src = source)->gridWidth) {
                    u16 id = src->chunkGrid[(tileY + j) * src->gridWidth + x];
                    u16 *cells = (u16 *)ring;

                    DecodeChunk(id, &cells[rowBase + (blockH >> 1) * i]);
                }
            }
        }
    }
}

/* Takes the layer's map from `src`: caches its 8px-tile size and the
 * decoder's record table, gLevelLayers's asset plus the layer's
 * section offset - the same "asset + offset" shape as the terrain-tile
 * cache's `decodeBase` (TileCache, bg_layer.hpp). */
void BgStreamer::SetSource(const struct level_layer_desc *src)
{
    s32 w;
    s32 h;

    source = src;
    w = src->widthTiles;
    h = src->heightTiles;
    widthTiles = w;
    heightTiles = h;
    records = (const u16 *)((u8 *)gLevelLayers->asset + src->assetOffset);
}

/* Frees the ring buffer (`delete[]` tests it for NULL). */
BgStreamer::~BgStreamer()
{
    delete[] ring;
}

/* Allocates the 0x1000-byte ring buffer (64 halfword columns x 32 rows,
 * per this file's header comment). */
BgStreamer::BgStreamer()
{
    ring = new u8[0x1000];
}

/* UNUSED: no caller anywhere in the ROM. */
s32 BgStreamer::GetHeight()
{
    return heightTiles;
}

/* UNUSED: no caller anywhere in the ROM. */
s32 BgStreamer::GetWidth()
{
    return widthTiles;
}

/* UNUSED: no caller anywhere in the ROM. */
void BgStreamer::SetSize(const s32 *size)
{
    s32 h = size[1];
    s32 w = size[0];

    widthTiles = w;
    heightTiles = h;
}

/* UNUSED: no caller anywhere in the ROM. */
void BgStreamer::SetSize(s32 w, s32 h)
{
    widthTiles = w;
    heightTiles = h;
}
