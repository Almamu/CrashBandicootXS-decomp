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

/* GitHub issue #39: 0x08024810-0x08024E68 (game_loop) - the remainder of
 * the UpdateGameFrame-MainLoop cluster between the sound-channel-handle
 * family (slideshow.cpp/slideshow_display.cpp) and the terrain-tile decode cache
 * (bg_layer_base.cpp, GitHub issue #40). docs/rom_map.md's "A new find: a
 * custom RLE/delta token-stream decoder" and its two follow-up sections
 * ("Follow-up: resolved the semantics by tracing callers", "The
 * background streamer's missing 'level load' half") already
 * characterized this whole cluster from a read-only pass - this is
 * where it gets turned into (attempted) byte-exact C.
 *
 * Two systems share this address range:
 *
 * - `InitSlideshow`/`RunCutscenePlayer`/`DestroyCutscenePlayer`/`InitCutscenePlayer`
 *   are the cutscene player (class CutscenePlayer, include/cutscene.hpp)
 *   that slideshow.cpp/slideshow_display.cpp also drive: `ResetSlideshow`
 *   sets the VRAM-bank toggle to 1, and `InitCutscenePlayer` also clears
 *   `pages`/`font`. `RunCutscenePlayer` is `RunSlideshow`'s (slideshow.cpp)
 *   loop over the slides, interleaved with an explicit OAM-shadow-buffer
 *   flush (`ResetOamBuffer`/`HideUnusedOamEntries`/`WaitForVBlank`/
 *   `CommitOamBuffer` on `gOamBuffer`) and a nested text-paging loop
 *   through each slide's `struct cutscene_page`, rendering each string
 *   via `DrawWrappedText` (wrapped_text.cpp) with `font` into `box`,
 *   continuing to the next string while a held-input mask (9, versus
 *   `RunSlideshow`'s 8) stays set. `box.h` divided by the font's line
 *   height gives the per-call text-wrap `limit`.
 *   `DestroyCutscenePlayer` is a plain two-argument forwarding trampoline to
 *   `DestroySlideshow` (slideshow_display.cpp).
 *
 * - `BgStreamer` (gBgStreamerVtable) is the "visual scrolling background
 *   streamer" docs/rom_map.md names: a circular 4x4-block (64 halfword
 *   columns x 32 rows, 0x1000 bytes total) ring-buffer tilemap fed by the
 *   same custom RLE/delta token-stream decoder (`DecodeChunk`) the
 *   terrain-tile cache's `DecodeCollisionChunk` (bg_layer_base.cpp) also
 *   uses, just writing into a 2D buffer (row stride 64 halfwords) instead
 *   of a flat one. `Scroll` is the per-frame driver: it right-shifts the
 *   world position by 7/6 (128/64 px tile granularity), and on each axis
 *   the camera crosses a tile boundary, streams in exactly the
 *   newly-exposed row (`StreamRow`) or column (`StreamColumn`), while a
 *   mod-4 "sub-block" accumulator (`subX`/`subY`) tracks which of the
 *   ring buffer's 4 blocks is now the logical edge. `Fill` is the "level
 *   load" half - seeds the ring buffer's *entire* initial contents the
 *   same way, from a fresh camera position. `GetColumn`/`GetRow`/`GetCell`
 *   convert a world pixel position into the ring buffer's wrapped (col,
 *   row) address. Then `BgLayerBase`'s constructor, destructor, scroll
 *   clamps and the Q8 scale/accumulate step (gBgLayerBaseVtable; the rest
 *   of its methods are in bg_layer_base.cpp). Both classes are in
 *   include/bg_layer.hpp, and their destructors are their key methods:
 *   g++ emits both vtables here.
 *
 * Built with old_agbcp (the Makefile's OLD_AGBCC_OBJS) - see
 * docs/matching/archive/game-loop-old-agbcc.md. */

/* InitSlideshow, Slideshow's constructor (include/cutscene.hpp): Reset. */
Slideshow::Slideshow()
{
    Reset();
}

/* Per-frame driver loop over `self`'s slides (the loop `RunSlideshow`
 * (slideshow.cpp) also runs), interleaved with an explicit OAM-shadow-buffer
 * flush and a nested text-paging walk through each slide's page. See
 * this file's header comment for the full shape.
 *
 * Matched (old_agbcc). The ROM reloads `&gOamBuffer` from the
 * literal pool at each of the three OAM flushes (rotating r1/r2/r3):
 * that is a function-scope local `oamp` set to the address before the
 * loop, which global-alloc leaves without a register, so reload
 * rematerializes it at each use and r4 stays free for `self`. The
 * prologue reads the box word and `font` into locals before the
 * store, and the page loop is a plain `for` with `j++`. */

void CutscenePlayer::Run()
{
    CutscenePlayer *self = this;
    OamBuffer **oamp = &gOamBuffer;
    s32 limit;
    s32 i;

    {
        s32 b = self->box.x;
        Font *t = self->font;

        t->SetMargin(b);
        limit = t->HeightToLines(self->box.h);
    }
    for (i = 0; i < self->count; i++) {
        u8 res = 1;

        self->ShowPicture(i);
        (*oamp)->Reset();
        (*oamp)->HideUnused();
        WaitForVBlank();
        (*oamp)->Commit();
        self->BeginSlide(i);
        if (self->pages[i].count == 0) {
            res = WaitForKeyPress(self->slides[i]->wait, self->slides[i]->buttons, 9);
        } else {
            s32 j;

            for (j = 0; j < self->pages[i].count && res == 1; j++) {
                u8 *str = (u8 *)self->pages[i].strings[j];
                s32 pos = 0;

                while (str[pos] != 0 && res == 1) {
                    pos += DrawWrappedText(str + pos, self->font, &self->box, limit, 1);
                    res = WaitForKeyPress(self->slides[i]->wait, self->slides[i]->buttons, 9);
                }
            }
        }
        self->EndSlide(i);
        i = self->Skip(i, res);
    }
}

/* DestroyCutscenePlayer: g++'s destructor of a derived class with nothing
 * of its own to tear down passes its __in_chrg on to the base's
 * (DestroySlideshow), which frees `this` if asked. */
CutscenePlayer::~CutscenePlayer()
{
}

/* InitCutscenePlayer: the slideshow's constructor, then no pages and no
 * font yet. */
CutscenePlayer::CutscenePlayer()
{
    pages = 0;
    font = 0;
}

/* The custom RLE/delta token-stream decoder (docs/rom_map.md's "A new
 * find" section): looks up a base pointer via `records` indexed by
 * `recordId`'s halfword table entry, then decodes a token stream,
 * budget-limited to 0x7f halfwords, with the same three run modes
 * (literal-fill, signed-delta-accumulate, raw-copy) as the terrain-tile
 * cache's `DecodeCollisionChunk` (bg_layer_base.cpp) - just writing into a
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

/* Destroys the layer's streamer (a virtual `delete`: slot 1 with 3). */
BgLayerBase::~BgLayerBase()
{
    delete streamer;
}

/* Creates the layer's streamer. `unused` is the BG index the subclass's
 * constructor passes on. */
BgLayerBase::BgLayerBase(s32 unused)
{
    streamer = new BgStreamer;
}

/* Clamps a scroll step to [-0x10, 0x10]. */
s32 BgLayerBase::ClampScrollStep(s32 step)
{
    LIMIT_MIN(step, -0x10);
    LIMIT_MAX(step, 0x10);
    return step;
}

/* Clamps `pos[0]`/`pos[1]` to the scroll limits. UNUSED: no caller
 * anywhere in the ROM. */
void BgLayerBase::ClampScrollMax(s32 *pos)
{
    s32 a = maxX;

    LIMIT_MAX(a, pos[0]);
    pos[0] = a;

    {
        s32 b = maxY;

        LIMIT_MAX(b, pos[1]);
        pos[1] = b;
    }
}

/* Applies the layer's per-axis scale (`scaleX`/`scaleY`) to `vec`,
 * dividing the Q8 product by 256 rounded toward zero (an arithmetic
 * `>> 8` alone rounds negative products toward negative infinity; the
 * `+0xff` bias before it makes it truncate, as `/ 256` would). Called by
 * bg_layer_base.cpp's `Scroll`/`Reset`. */
void BgLayerBase::ScaleScroll(s32 *vec)
{
    s32 v;

    v = vec[0] * scaleX;
    if (v >= 0) {
        v = Q8_TO_INT(v);
    } else {
        v = Q8_TO_INT(v + 0xff);
    }
    vec[0] = v;

    v = vec[1] * scaleY;
    if (v >= 0) {
        v = Q8_TO_INT(v);
    } else {
        v = Q8_TO_INT(v + 0xff);
    }
    vec[1] = v;
}

/* Moves the layer toward `target`, each axis by at most the step
 * ClampScrollStep (virtual) allows. */
void BgLayerBase::StepScroll(const s32 *target)
{
    s32 dx, dy;

    dx = ClampScrollStep(target[0] - x);
    dy = ClampScrollStep(target[1] - y);

    x += dx;
    y += dy;
}
