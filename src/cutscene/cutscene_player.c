#include "core.h"
#include "bg_scroll_layer.h"
#include "text.h"
#include "cutscene.h"
#include <libgcc.h>
#include "system.h"
#include "gfx.h"

/* GitHub issue #39: 0x08024810-0x08024E68 (game_loop) - the remainder of
 * the UpdateGameFrame-MainLoop cluster between the sound-channel-handle
 * family (slideshow.c/slideshow_display.c) and the terrain-tile decode cache
 * (bg_layer_base.c, GitHub issue #40). docs/rom_map.md's "A new find: a
 * custom RLE/delta token-stream decoder" and its two follow-up sections
 * ("Follow-up: resolved the semantics by tracing callers", "The
 * background streamer's missing 'level load' half") already
 * characterized this whole cluster from a read-only pass - this is
 * where it gets turned into (attempted) byte-exact C.
 *
 * Two systems share this address range:
 *
 * - `InitSlideshow`/`RunCutscenePlayer`/`DestroyCutscenePlayer`/`InitCutscenePlayer`
 *   are the cutscene player (`struct cutscene_player`, include/cutscene.h)
 *   that slideshow.c/slideshow_display.c also drive: `ResetSlideshow`
 *   sets the VRAM-bank toggle to 1, and `InitCutscenePlayer` also clears
 *   `pages`/`font`. `RunCutscenePlayer` is `RunSlideshow`'s (slideshow.c)
 *   loop over the slides, interleaved with an explicit OAM-shadow-buffer
 *   flush (`ResetOamBuffer`/`HideUnusedOamEntries`/`WaitForVBlank`/
 *   `CommitOamBuffer` on `gOamBuffer`) and a nested text-paging loop
 *   through each slide's `struct cutscene_page`, rendering each string
 *   via `DrawWrappedText` (wrapped_text.c) with `font` into `box`,
 *   continuing to the next string while a held-input mask (9, versus
 *   `RunSlideshow`'s 8) stays set. `box.h` divided by the font's line
 *   height gives the per-call text-wrap `limit`.
 *   `DestroyCutscenePlayer` is a plain two-argument forwarding trampoline to
 *   `DestroySlideshow` (slideshow_display.c).
 *
 * - `DecodeLayerChunk` through `StepBgLayerScroll` are the "visual scrolling
 *   background streamer" docs/rom_map.md names: a circular 4x4-block
 *   (64 halfword columns x 32 rows, 0x1000 bytes total) ring-buffer
 *   tilemap fed by the same custom RLE/delta token-stream decoder
 *   (`DecodeLayerChunk`) the terrain-tile cache's `DecodeCollisionChunk`
 *   (bg_layer_base.c) also uses, just writing into a 2D buffer (row
 *   stride 64 halfwords) instead of a flat one. `ScrollBgStreamer` is the
 *   per-frame driver: it right-shifts the world position by 7/6 (128/64
 *   px tile granularity), and on each axis the camera crosses a tile
 *   boundary, streams in exactly the newly-exposed row (`StreamBgRow`)
 *   or column (`StreamBgColumn`) via `DecodeLayerChunk`, while a mod-4
 *   "sub-block" accumulator (`+0x14`/`+0x15`) tracks which of the ring
 *   buffer's 4 blocks is now the logical edge. `FillBgStreamer` is the
 *   "level load" half - seeds the ring buffer's *entire* initial
 *   contents the same way, from a fresh camera position. `GetBgStreamerColumn`/
 *   `GetBgStreamerRow`/`GetBgStreamerCell` convert a world pixel position into the
 *   ring buffer's wrapped (col, row) address; `GetBgStreamerCell` combines
 *   both and returns the decoded halfword value directly.
 *   `SetBgStreamerSource`/`DestroyBgStreamer`/`InitBgStreamer`/`GetBgStreamerHeight`/
 *   `GetBgStreamerWidth`/`SetBgStreamerSizeVec`/`SetBgStreamerSize`/`DestroyBgLayerBase`/
 *   `InitBgLayerBase`/`ClampBgLayerScrollStep`/`ClampBgLayerScrollMax`/`ScaleBgLayerScroll`/
 *   `StepBgLayerScroll` round out the streamer object's own construction
 *   (allocates the 0x1000-byte ring buffer, wires up two
 *   `_call_via_r2`-style interworking-trampoline tables -
 *   `gBgStreamerVtable`/`gBgLayerBaseVtable` - for notifying a
 *   parent object of size/position changes), plain position/clamp
 *   accessors, and the Q8 scale/accumulate step
 *   (`ScaleBgLayerScroll`/`StepBgLayerScroll`) bg_layer_base.c's `ScrollBgLayerBase`/
 *   `ResetBgLayerBase` already call into.
 *
 * The streamer functions use `struct bg_streamer` below, the layer
 * functions `struct bg_scroll_layer` (include/bg_scroll_layer.h). Built
 * with old_agbcc - see
 * docs/matching/game-loop-old-agbcc.md. */

/* The room descriptor the streamer reads its tile map from. */
struct stream_source
{
    u16 *map;                   // 0x00 - width x height tile ids
    s32 assetOffset;            // 0x04 - this layer's section in the level asset
    u8 unk_08[0xE];
    u16 width;                  // 0x16 - in tiles
    u16 height;                 // 0x18
    u16 widthTiles;             // 0x1A - 8px tiles, cached by SetBgStreamerSource
    u16 heightTiles;            // 0x1C
};

/* The streamer's method table (gBgStreamerVtable). */
struct streamer_vtable
{
    u8 unk_00[8];
    struct bg_layer_method destroy; // 0x08 - called with 3 by DestroyBgLayerBase
};

/* The ring-buffer background streamer (see this file's header comment). */
struct bg_streamer
{
    struct stream_source *source; // 0x00
    u16 *records;               // 0x04 - decoder record table
    u8 *ring;                   // 0x08 - 0x1000-byte 4x4-block ring buffer
    s32 tileX;                  // 0x0C
    s32 tileY;                  // 0x10
    u8 subX;                    // 0x14 - mod-4 block of the left edge
    u8 subY;                    // 0x15 - mod-4 block of the top edge
    u8 pad_16[2];
    s32 widthTiles;             // 0x18 - the source's widthTiles
    s32 heightTiles;            // 0x1C
    struct streamer_vtable *vtable; // 0x20
};

/* Trivial wrapper: runs `ResetSlideshow`'s reset, then returns `self`
 * unchanged (a "chained constructor" idiom this project sees a lot of -
 * see e.g. `InitBgStreamer` below for another instance). */
struct cutscene_player *InitSlideshow(struct cutscene_player *self)
{
    ResetSlideshow(self);
    return self;
}

/* Per-frame driver loop over `self`'s slides (the loop `RunSlideshow`
 * (slideshow.c) also runs), interleaved with an explicit OAM-shadow-buffer
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
extern void *gOamBuffer;

void RunCutscenePlayer(struct cutscene_player *self)
{
    void **oamp = &gOamBuffer;
    s32 limit;
    s32 i;

    {
        s32 b = self->box.x;
        struct bitmap_font *t = self->font;

        t->marginX = b;
        limit = __udivsi3(self->box.h, t->lineHeight);
    }
    for (i = 0; i < self->count; i++)
    {
        u8 res = 1;

        ShowSlidePicture(self, i);
        ResetOamBuffer(*oamp);
        HideUnusedOamEntries(*oamp);
        WaitForVBlank();
        CommitOamBuffer(*oamp);
        BeginSlide(self, i);
        if (self->pages[i].count == 0)
        {
            res = WaitForKeyPress(self->slides[i]->wait, self->slides[i]->buttons, 9);
        }
        else
        {
            s32 j;

            for (j = 0; j < self->pages[i].count && res == 1; j++)
            {
                u8 *str = (u8 *)self->pages[i].strings[j];
                s32 pos = 0;

                while (str[pos] != 0 && res == 1)
                {
                    pos += DrawWrappedText(str + pos, self->font, &self->box, limit, 1);
                    res = WaitForKeyPress(self->slides[i]->wait, self->slides[i]->buttons, 9);
                }
            }
        }
        EndSlide(self, i);
        i = SkipSlides(self, i, res);
    }
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

/* Plain two-argument forwarding trampoline to `DestroySlideshow`
 * (slideshow_display.c) - a same-shaped alias for a different call site
 * (matches this project's other trivial-wrapper aliases, e.g.
 * `sub_802425C`/`DestroySlideshow` themselves). */
void DestroyCutscenePlayer(struct cutscene_player *self, s32 flags)
{
    DestroySlideshow(self, flags);
}

/* Extends `InitSlideshow`'s reset: no pages and no font yet. */
struct cutscene_player *InitCutscenePlayer(struct cutscene_player *self)
{
    InitSlideshow(self);
    self->pages = NULL;
    self->font = NULL;
    return self;
}

/* The custom RLE/delta token-stream decoder (docs/rom_map.md's "A new
 * find" section): looks up a base pointer via `self+4` indexed by
 * `recordId`'s halfword table entry, then decodes a token stream,
 * budget-limited to 0x7f halfwords, with the same three run modes
 * (literal-fill, signed-delta-accumulate, raw-copy) as the terrain-tile
 * cache's `DecodeCollisionChunk` (bg_layer_base.c) - just writing into a 2D buffer
 * (row = idx>>4, 64-halfword row stride) instead of a flat one.
 *
 * Matched (old_agbcc) by porting `DecodeCollisionChunk`'s matched shape: `src`
 * starts as the record table itself, and the delta run's sign extensions
 * are explicit `<< 24` shifts into `s32` locals with `acc` copied to an
 * `s32` first. Two local changes: the odd trailing delta is stored back
 * into `acc` before the cell store, and the copy loop builds its cell
 * index in a local `k` (row first), which gives the ROM's
 * `asr; lsl` order there. */
#define RING_CELL(out, i) (out)[((i) >> 4) * 64 + ((i) & 0xf)]

void DecodeLayerChunk(struct bg_streamer *self, s32 recordId, void *dest)
{
    u16 *out = dest;
    u16 *src = self->records;
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
                RING_CELL(out, written) = value;
                written++;
                n--;
            } while (n != 0);
        }
        else if (token & 0x4000)
        {
            s16 acc;

            budget -= n;
            acc = *src++;
            RING_CELL(out, written) = acc;
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
            if (n != 0)
            {
                u16 last = *src++;
                s32 lo = last << 24;
                s32 a = acc;

                acc = a + (lo >> 24);
                RING_CELL(out, written) = acc;
                written++;
            }
        }
        else
        {
            budget -= n;
            do
            {
                s32 k = written >> 4;

                k = k * 64 + (written & 0xf);
                out[k] = *src++;
                written++;
                n--;
            } while (n != 0);
        }
    } while (budget >= 0);
}

extern void StreamBgColumn(struct bg_streamer *self, s32 col);
extern void StreamBgRow(struct bg_streamer *self, s32 row);

/* Per-frame background-streamer driver (docs/rom_map.md: "Visual
 * scrolling background streamer"): right-shifts the world position by
 * 7/6 (128/64px tile granularity) and, on each axis the camera has
 * crossed a tile boundary since last call, streams in exactly the
 * newly-exposed column (`StreamBgColumn`) or row (`StreamBgRow`) - the
 * tile 4 ahead of the old edge when scrolling forward, or the tile
 * directly behind when scrolling back - while advancing the mod-4
 * sub-block accumulator (`self+0x14`/`self+0x15`) that feeds
 * `GetBgStreamerColumn`/`GetBgStreamerRow`/`GetBgStreamerCell`'s wrapped addressing. */
void ScrollBgStreamer(void *self0, void *worldpos0)
{
    struct bg_streamer *self = self0;
    s32 *worldpos = (s32 *)worldpos0;
    s32 tileX = worldpos[0] >> 7;
    s32 tileY = worldpos[1] >> 6;
    s32 oldX = self->tileX;
    s32 oldY = self->tileY;

    if (tileX > oldX) {
        StreamBgColumn(self, oldX + 4);
        self->subX = (self->subX + 1) & 3;
    } else if (tileX < oldX) {
        self->subX = (self->subX - 1) & 3;
        StreamBgColumn(self, oldX - 1);
    }
    self->tileX = tileX;

    if (tileY > oldY) {
        StreamBgRow(self, oldY + 4);
        self->subY = (self->subY + 1) & 3;
    } else if (tileY < oldY) {
        self->subY = (self->subY - 1) & 3;
        StreamBgRow(self, oldY - 1);
    }
    self->tileY = tileY;
}

/* Converts a world pixel `(x, y)` into the background streamer's
 * circular ring-buffer address (see this file's header comment):
 * `self+0xc`/`self+0x10` are the last-known tile coordinates,
 * `self+0x14`/`self+0x15` the mod-4 sub-block accumulator
 * `ScrollBgStreamer` advances. Returns the column-wrapped halfword pointer
 * (col wrapped mod 0x40) and writes the row (wrapped mod 0x20) out
 * through `rowOut`. */
void *GetBgStreamerColumn(void *self0, s32 x, s32 y, s32 *rowOut)
{
    struct bg_streamer *self = self0;
    s32 col = x - self->tileX * 16;
    s32 row = y - self->tileY * 8;
    u8 subX;
    u8 subY;
    s32 shifted;
    u8 *base;
    void *ret;

    subX = self->subX;
    shifted = subX * 16;
    col += shifted;
    subY = self->subY;
    shifted = subY * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = self->ring;
    col <<= 1;
    ret = base + col;
    *rowOut = row;
    return ret;
}

/* Sibling of `GetBgStreamerColumn`: same wrapped `(col, row)` computation, but
 * returns the row-wrapped pointer (row stride 0x80 bytes = 0x40
 * halfwords, matching `DecodeLayerChunk`'s own row stride) and writes the
 * column out through `colOut` instead. */
void *GetBgStreamerRow(void *self0, s32 x, s32 y, s32 *colOut)
{
    struct bg_streamer *self = self0;
    s32 col = x - self->tileX * 16;
    s32 row = y - self->tileY * 8;
    u8 subX;
    u8 subY;
    s32 shifted;
    u8 *base;
    void *ret;

    subX = self->subX;
    shifted = subX * 16;
    col += shifted;
    subY = self->subY;
    shifted = subY * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = self->ring;
    row <<= 7;
    ret = base + row;
    *colOut = col;
    return ret;
}

/* Combines `GetBgStreamerColumn`/`GetBgStreamerRow`'s address computation and
 * returns the decoded halfword value directly, with no out-param. */
u16 GetBgStreamerCell(void *self0, s32 x, s32 y)
{
    struct bg_streamer *self = self0;
    s32 col = x - self->tileX * 16;
    s32 row = y - self->tileY * 8;
    u8 subX;
    u8 subY;
    s32 shifted;
    u8 *base;
    s32 idx;

    subX = self->subX;
    shifted = subX * 16;
    col += shifted;
    subY = self->subY;
    shifted = subY * 8;
    row += shifted;
    col &= 0x3f;
    row &= 0x1f;
    base = self->ring;
    idx = row << 6;
    idx += col;
    idx <<= 1;
    return *(u16 *)(base + idx);
}

/* Streams in the newly exposed tile row `row`: decodes each of its up to
 * 4 in-bounds tiles into the row's ring-buffer blocks. */
void StreamBgRow(struct bg_streamer *self, s32 row)
{
    s32 idx;
    s32 i;

    if (row < self->source->height)
    {
        idx = self->source->width;
        idx *= row;
        idx += self->tileX;
        for (i = 0; i <= 3; i++)
        {
            if (i + self->tileX < self->source->width)
            {
                u8 *dest = self->ring;
                dest += ((self->subY + 4) & 3) << 10;
                dest += ((self->subX + i + 4) & 3) << 5;
                DecodeLayerChunk(self, self->source->map[idx++], dest);
            }
        }
    }
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

/* Streams in the newly exposed tile column `col`: decodes each of its up
 * to 4 in-bounds tiles into the column's ring-buffer blocks. */
void StreamBgColumn(struct bg_streamer *self, s32 col)
{
    s32 idx;
    s32 i;

    if (col < self->source->width)
    {
        idx = self->tileY * self->source->width;
        idx += col;
        for (i = 0; i <= 3; i++)
        {
            struct stream_source *src;

            if (i + self->tileY < (src = self->source)->height)
            {
                u8 *dest = self->ring;
                u16 id;

                dest += ((self->subY + i + 4) & 3) << 10;
                dest += ((self->subX + 4) & 3) << 5;
                id = src->map[idx];
                idx += src->width;
                DecodeLayerChunk(self, id, dest);
            }
        }
    }
}

/* Seeds the whole ring buffer for a fresh camera position `pos` (Q8
 * world x/y): resets the sub-block origin, derives the top-left tile
 * (128x64 px tiles) and decodes all 4x4 in-bounds tiles. The row stride
 * and block height stay in registers across the loops, as in the ROM. */
void FillBgStreamer(struct bg_streamer *self, s32 *pos)
{
    s32 rowLen = 0x40;
    s32 blockH = 0x20;
    s32 j;
    s32 i;

    self->subX = 0;
    self->subY = 0;
    self->tileX = pos[0] >> 7;
    self->tileY = pos[1] >> 6;
    for (j = 0; j <= 3; j++)
    {
        if (j + self->tileY < self->source->height)
        {
            s32 rowBase;

            for (i = 0, rowBase = rowLen * (j << 3); i <= 3; i++)
            {
                s32 x = i + self->tileX;
                struct stream_source *src;

                if (x < (src = self->source)->width)
                {
                    u16 id = src->map[(self->tileY + j) * src->width + x];
                    u16 *ring = (u16 *)self->ring;

                    DecodeLayerChunk(self, id, &ring[rowBase + (blockH >> 1) * i]);
                }
            }
        }
    }
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");

extern void *gLevelLayers;

/* Stores `source` (the room/level descriptor - see this file's header
 * comment) into `self+0`, caches its `+0x1a`/`+0x1c` 8px-tile dimensions at
 * `self+0x18`/`self+0x1c`, and derives `self+4` from
 * `gLevelLayers`'s own `+0x24` field plus `source+4` - the same
 * "camera offset + source field" shape as the terrain-tile cache's
 * `decodeBase` (bg_layer_base.c's `struct tile_cache`). */
void SetBgStreamerSource(void *self0, void *source0)
{
    struct bg_streamer *self = self0;
    struct stream_source *source = source0;
    s32 w;
    s32 h;

    self->source = source;
    w = source->widthTiles;
    h = source->heightTiles;
    self->widthTiles = w;
    self->heightTiles = h;
    self->records = (u16 *)(*(u8 **)((u8 *)gLevelLayers + 0x24) + source->assetOffset);
}

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern u8 gBgStreamerVtable[];
extern u8 gBgLayerBaseVtable[];

/* Wires up `self+0x20`'s `_call_via_r2`-style interworking-trampoline
 * table (a fixed `gBgStreamerVtable`), then tears down `self+8`'s
 * ring buffer (if already allocated - `InitBgStreamer` below is the
 * matching constructor) and/or notifies via `OperatorDelete` if bit 0 of
 * `flags` is set - the same conditional-teardown shape this project
 * sees a lot of (e.g. `DestroySlideshow`, slideshow_display.c). */
void DestroyBgStreamer(void *self0, s32 flags)
{
    struct bg_streamer *self = self0;

    self->vtable = (struct streamer_vtable *)gBgStreamerVtable;
    if (self->ring != NULL) {
        OperatorDeleteArray(self->ring);
    }
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Constructor: wires up the same `gBgStreamerVtable` trampoline
 * table as `DestroyBgStreamer` above, allocates the streamer's 0x1000-byte
 * ring buffer (64 halfword columns x 32 rows, per this file's header
 * comment), and returns `self`. */
void *InitBgStreamer(void *self0)
{
    struct bg_streamer *self = self0;

    self->vtable = (struct streamer_vtable *)gBgStreamerVtable;
    self->ring = OperatorNewArray(0x1000);
    return self;
}

/* Plain accessor: returns `self+0x1c`. */
s32 GetBgStreamerHeight(void *self0)
{
    return ((struct bg_streamer *)self0)->heightTiles;
}

/* Plain accessor: returns `self+0x18`. */
s32 GetBgStreamerWidth(void *self0)
{
    return ((struct bg_streamer *)self0)->widthTiles;
}

/* Plain setter: copies `vec[0]/vec[1]` into `self+0x18`/`self+0x1c`. */
void SetBgStreamerSizeVec(void *self0, void *vec0)
{
    struct bg_streamer *self = self0;
    s32 *vec = (s32 *)vec0;
    s32 y = vec[1];
    s32 x = vec[0];

    self->widthTiles = x;
    self->heightTiles = y;
}

/* Plain setter: stores `x`/`y` into `self+0x18`/`self+0x1c` directly
 * (same fields as `SetBgStreamerSizeVec` above, caller-supplied scalars instead
 * of a vector). */
void SetBgStreamerSize(void *self0, s32 x, s32 y)
{
    struct bg_streamer *self = self0;

    self->widthTiles = x;
    self->heightTiles = y;
}

/* Wires up `self+0x30`'s second `_call_via_r2`-style trampoline table
 * (`gBgLayerBaseVtable`, a different fixed table from
 * `DestroyBgStreamer`'s), then - if `self+0x2c`'s child object is already
 * set (per `InitBgLayerBase` below) - notifies it via its own `+0x20`
 * trampoline table with a fixed action code `3`, before the same
 * conditional `OperatorDelete` teardown notify `DestroyBgStreamer` has. */
void DestroyBgLayerBase(void *self0, s32 flags)
{
    struct bg_scroll_layer *self = self0;
    struct bg_streamer *child;

    self->vtable = (struct bg_layer_vtable *)gBgLayerBaseVtable;
    child = self->streamer;

    if (child != NULL) {
        struct streamer_vtable *mgr = child->vtable;

        _call_via_r2((u8 *)child + mgr->destroy.thisOffset, (void *)3, mgr->destroy.fn);
    }

    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Constructor: wires up the `gBgLayerBaseVtable` trampoline table
 * (same as `DestroyBgLayerBase` above), allocates a 0x24-byte child object and
 * runs `InitBgStreamer` on it (the ring-buffer-owning object those
 * `self+0x20`-rooted trampolines above notify), storing the result at
 * `self+0x2c`. */
void *InitBgLayerBase(void *self0)
{
    struct bg_scroll_layer *self = self0;

    self->vtable = (struct bg_layer_vtable *)gBgLayerBaseVtable;
    self->streamer = InitBgStreamer(OperatorNew(0x24));
    return self;
}

/* Clamps `value` to `[-0x10, 0x10]` - `self` (the first argument) is
 * unused. */
s32 ClampBgLayerScrollStep(void *self0, s32 value)
{
    if (value < -0x10) {
        value = -0x10;
    }
    if (value > 0x10) {
        value = 0x10;
    }
    return value;
}

/* Clamps `out[0]`/`out[1]` against `self+8`/`self+0xc` (an upper bound
 * pair), writing the smaller of each back into `out`. */
void ClampBgLayerScrollMax(void *self0, s32 *out)
{
    struct bg_scroll_layer *self = self0;
    s32 a = self->maxX;

    if (a > out[0]) {
        a = out[0];
    }
    out[0] = a;

    {
        s32 b = self->maxY;

        if (b > out[1]) {
            b = out[1];
        }
        out[1] = b;
    }
}

/* Applies the layer's per-axis scale (`self+0x20`/`self+0x24`) to
 * `vec2`, floor-dividing the Q8 product by 256 (the `+0xff` bias before
 * the arithmetic shift rounds negative products toward negative
 * infinity, matching a true floor division rather than C's
 * truncate-toward-zero `>>`). Called by bg_layer_base.c's `ScrollBgLayerBase`/
 * `ResetBgLayerBase`. */
void ScaleBgLayerScroll(void *self0, void *vec20)
{
    struct bg_scroll_layer *self = self0;
    s32 *vec2 = (s32 *)vec20;
    s32 v;

    v = vec2[0] * self->scaleX;
    if (v >= 0) {
        v = v >> 8;
    } else {
        v = (v + 0xff) >> 8;
    }
    vec2[0] = v;

    v = vec2[1] * self->scaleY;
    if (v >= 0) {
        v = v >> 8;
    } else {
        v = (v + 0xff) >> 8;
    }
    vec2[1] = v;
}

/* Two-line text draw / position notify (docs/rom_map.md: "a two-line
 * text draw, same family as the icon-renderer shapes"): for each axis,
 * forwards `delta - self`'s own position through `self+0x30`'s
 * trampoline table (action = the position delta itself, per the same
 * `_call_via_r2`-style convention `DestroyBgLayerBase`/`InitBgLayerBase` wire up),
 * then accumulates both trampoline results back into `self`'s own
 * position. */
void StepBgLayerScroll(void *self0, void *delta0)
{
    struct bg_scroll_layer *self = self0;
    s32 *delta = (s32 *)delta0;
    struct bg_layer_vtable *mgr;
    s32 dx, dy;

    mgr = self->vtable;
    dx = _call_via_r2((u8 *)self + mgr->method_20.thisOffset,
                      (void *)(delta[0] - self->x),
                      mgr->method_20.fn);

    mgr = self->vtable;
    dy = _call_via_r2((u8 *)self + mgr->method_20.thisOffset,
                      (void *)(delta[1] - self->y),
                      mgr->method_20.fn);

    self->x += dx;
    self->y += dy;
}
/* Trailing byte count isn't a multiple of 4 in the ROM's own raw block
 * (a bare `.align 2, 0` follows `bx r0` there too) - see the
 * `matching_decomp_alignment_fix` precedent. */
asm(".align 2, 0");
