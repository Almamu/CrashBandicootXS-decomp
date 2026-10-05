#include "core.h"
#include "bg_scroll_layer.h"

/* GitHub issue #42: the BG-scroll layer's own methods (`struct
 * bg_scroll_layer`, constructor `InitBgLayer` in game_loop15.c, method
 * table `gBgLayerVtable`) and the overrides of its tile-slot-pooled
 * subclass used for BG layer 0 (`struct pooled_bg_layer`, constructor
 * `InitPooledBgLayer` in tile_slot_pool.c, table `gPooledBgLayerVtable`).
 *
 * The layer keeps a 32x32-entry window of the level's tile map resident in
 * its BG screen block. `+0x3C..+0x40` / `+0x44..+0x48` are the resident
 * tile row / column ranges; the streamer at `+0x2C` (game_loop57.c's
 * `GetBgStreamerColumn`/`GetBgStreamerRow`) resolves a (column, row) of the level map
 * to its ring-buffer entries. Screen entries wrap modulo 32 on both axes.
 *
 * The base layer copies map entries straight into the screen block; the
 * layer-0 subclass instead routes each source tile through the VRAM
 * tile-slot pool (`AcquireTileSlot` acquire / `ReleaseTileSlot` release) and
 * releases the tiles of rows/columns that scroll out (`ClipPooledBgLayerColumns`,
 * `ClipPooledBgLayerRows`).
 *
 * Compiled with old_agbcc (Makefile OLD_AGBCC_OBJS): the BGnCNT bitfield
 * setters (`SetBgLayerScreenBase`, `SetBgLayerPriority`, `SetBgLayerColors256`, `SetBgLayerCharBase`)
 * schedule the mask constant before the `ldrb`, which the current agbcc
 * never does. See docs/matching/issue-42-bg-scroll-layer.md.
 *
 * Real bytes formerly the whole of `asm/code_3_2_17_25fc8.s`. */

/* Per-layer part of the level descriptor (level_layers.c's
 * `struct level_desc` points at one per layer). */
struct bg_layer_desc
{
    u8 unk_00[8];     // 0x00
    void *tileData;   // 0x08
    s32 scaleX;       // 0x0C
    s32 scaleY;       // 0x10
    u16 cnt;          // 0x14 - BGnCNT bits (only the priority is used here)
    u8 unk_16[4];     // 0x16
    u16 widthTiles;   // 0x1A
    u16 heightTiles;  // 0x1C
};

extern u16 *GetBgStreamerColumn(void *streamer, s32 col, s32 row, s32 *rowOut);
extern u16 *GetBgStreamerRow(void *streamer, s32 col, s32 row, s32 *colOut);
extern void ResetBgLayerBase(struct bg_scroll_layer *self, void *pos);
extern void SetBgLayerSource(struct bg_scroll_layer *self, struct bg_layer_desc *desc);
extern void DestroyBgLayerBase(struct bg_scroll_layer *self, u32 flags);
extern void _call_via_r1(void *self, void *fn);
extern void _call_via_r2(void *self, s32 arg, void *fn);
extern void LoadTaggedAsset(void *asset, void *dest);
extern u16 AcquireTileSlot(struct tile_slot_pool *pool, u16 tile);
extern void ReleaseTileSlot(struct tile_slot_pool *pool, u32 tile);
extern void ResetTileSlotPool(struct tile_slot_pool *pool);
extern void SetTileSlotPoolSource(struct tile_slot_pool *pool, s32 charBase, u32 src);
extern u8 gBgLayerVtable[];

static inline s32 Mod32(s32 v)
{
    return v % 32;
}

/* Base `drawRow` (table +0x30): copies the resident columns of map row
 * `row` from the streamer into screen row `row % 32`. */
void DrawBgLayerRow(struct bg_scroll_layer *self, s32 row)
{
    u16 *dst = &self->screen[Mod32(row) * 32];
    s32 srcCol;
    u16 *src = GetBgStreamerRow(self->streamer, self->colLo, row, &srcCol);
    s32 c = self->colLo;
    s32 end;
    s32 mask;

    /* A goto loop, not for/while: with a structured loop gcc's load_mems
     * keeps `srcCol` in a register across the loop and stores it once
     * after it; the ROM reloads/stores it through the stack every
     * iteration. Explicit `end`/`mask` locals give the ROM's hoisted
     * r4/r7. See docs/matching/issue-42-bg-scroll-layer.md. */
    if (c > self->colHi)
        return;
    mask = 0x3F;
    end = self->colHi;
loop:
    dst[Mod32(c)] = src[srcCol];
    srcCol = (srcCol + 1) & mask;
    c++;
    if (c <= end)
        goto loop;
}

/* Recomputes the resident tile ranges from the pixel scroll position (a
 * 240x160 screen spans columns x/8..(x+239)/8 and rows y/8..(y+159)/8),
 * then draws every resident row through the method table's `drawRow`. */
void RedrawBgLayer(struct bg_scroll_layer *self)
{
    s32 r;

    self->colLo = self->x / 8;
    self->colHi = (self->x + 0xEF) / 8;
    self->rowLo = self->y / 8;
    self->rowHi = (self->y + 0x9F) / 8;
    for (r = self->rowLo; r <= self->rowHi; r++)
        _call_via_r2((u8 *)self + self->vtable->drawRow.thisOffset, r, self->vtable->drawRow.fn);
}

/* Base `reset` (table +0x10): sets the position (`ResetBgLayerBase`), reloads
 * the tiles (`loadTiles`), redraws the whole window and writes BGnCNT. */
void ResetBgLayer(struct bg_scroll_layer *self, void *pos)
{
    ResetBgLayerBase(self, pos);
    _call_via_r1((u8 *)self + self->vtable->loadTiles.thisOffset, self->vtable->loadTiles.fn);
    RedrawBgLayer(self);
    *self->cntReg = self->cnt.raw;
}

/* Base `loadTiles` (table +0x28): unpacks the tile asset into the layer's
 * character base block. */
void LoadBgLayerTiles(struct bg_scroll_layer *self)
{
    LoadTaggedAsset(self->tileData, (void *)(VRAM + (self->cnt.bits.charBase << 14)));
}

/* Level-load hook for one layer (called by level_layers.c's `LoadRoom`):
 * `SetBgLayerSource` picks up the map size, then an enabled layer takes its
 * tile asset and BG priority from the descriptor. */
void LoadBgLayer(struct bg_scroll_layer *self, struct bg_layer_desc *desc)
{
    SetBgLayerSource(self, desc);
    if (self->enabled)
    {
        u16 cnt;

        self->tileData = desc->tileData;
        cnt = desc->cnt;
        self->cnt.bits.priority = cnt;
    }
}

/* Screen-block entry index of map (col, row): 32x32 wrap.
 * UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/); no method table points at it either. */
s32 GetBgLayerScreenIndex(void *self, s32 col, s32 row)
{
    return Mod32(row) * 32 + Mod32(col);
}

/* Column wrap `col % 32`. Not a named symbol in the old disassembly (it was
 * folded into `GetBgLayerScreenIndex`'s listing after its padding halfword).
 * UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/). */
s32 sub_802612C(void *self, s32 col)
{
    return col % 32;
}

/* Byte-identical twin of `sub_802612C`. The old disassembly labelled it
 * `sub_802613E`, two bytes into its first instruction.
 * UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/). */
s32 sub_802613C(void *self, s32 col)
{
    return col % 32;
}

/* BGnCNT shadow setters/getter, this one and the next four.
 * UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/). */
void SetBgLayerScreenBase(struct bg_scroll_layer *self, u32 screenBase)
{
    self->cnt.bits.screenBase = screenBase;
}

void SetBgLayerPriority(struct bg_scroll_layer *self, u32 priority)
{
    self->cnt.bits.priority = priority;
}

void SetBgLayerColors256(struct bg_scroll_layer *self, u32 colors256)
{
    self->cnt.bits.colors256 = colors256;
}

u32 GetBgLayerCharBase(struct bg_scroll_layer *self)
{
    return self->cnt.bits.charBase;
}

void SetBgLayerCharBase(struct bg_scroll_layer *self, u32 charBase)
{
    self->cnt.bits.charBase = charBase;
}

/* Writes the cached HOFS/VOFS pair to the hardware registers.
 * UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/); level_layers.c uses `CommitBgLayerScroll` instead. */
void WriteBgLayerOffsetRegs(struct bg_scroll_layer *self)
{
    *self->ofsReg = *(u32 *)&self->hofs;
}

/* Writes the BGnCNT shadow to the hardware register (same as the tail of
 * `ResetBgLayer`). UNUSED - no caller anywhere in the ROM (checked asm/,
 * expected/ and src/). */
void WriteBgLayerCntReg(struct bg_scroll_layer *self)
{
    *self->cntReg = self->cnt.raw;
}

/* Base `destroy` (table +0x08): restores the base table and chains to the
 * streamer-owning base destructor `DestroyBgLayerBase`. */
void DestroyBgLayer(struct bg_scroll_layer *self, u32 flags)
{
    self->vtable = (struct bg_layer_vtable *)gBgLayerVtable;
    DestroyBgLayerBase(self, flags);
}

/* Layer-0 `drawCol` (table +0x38): acquires a pool slot for every resident
 * row's tile in map column `col` and writes the returned map entry, moving
 * down one screen row (mod 32x32) per step. */
void DrawPooledBgLayerColumn(struct pooled_bg_layer *self, s32 col)
{
    s32 srcRow;
    u16 *src = GetBgStreamerColumn(self->base.streamer, col, self->base.rowLo, &srcRow);
    s32 idx = Mod32(self->base.rowLo) * 32 + Mod32(col);
    s32 r;

    for (r = self->base.rowLo; r <= self->base.rowHi; r++)
    {
        self->base.screen[idx] = AcquireTileSlot(self->pool, src[srcRow * 64]);
        srcRow = (srcRow + 1) & 0x1F;
        idx = (idx + 32) % 0x400;
    }
}

/* Layer-0 override of table +0x20 (base: `ClampBgLayerScrollStep`): clamps a scroll
 * step to [-8, 8]. */
s32 ClampPooledBgLayerScrollStep(struct pooled_bg_layer *self, s32 v)
{
    if (v < -8)
        v = -8;
    if (v > 8)
        v = 8;
    return v;
}

/* Releases the pool slots of map column `col`'s resident rows. */
void ReleasePooledBgLayerColumn(struct pooled_bg_layer *self, s32 col)
{
    s32 srcRow;
    u16 *src = GetBgStreamerColumn(self->base.streamer, col, self->base.rowLo, &srcRow);
    s32 r;

    for (r = self->base.rowLo; r <= self->base.rowHi; r++)
    {
        ReleaseTileSlot(self->pool, src[srcRow * 64]);
        srcRow = (srcRow + 1) & 0x1F;
    }
}

/* Releases the pool slots of map row `row`'s resident columns. */
void ReleasePooledBgLayerRow(struct pooled_bg_layer *self, s32 row)
{
    s32 srcCol;
    u16 *src = GetBgStreamerRow(self->base.streamer, self->base.colLo, row, &srcCol);
    s32 c;

    for (c = self->base.colLo; c <= self->base.colHi; c++)
    {
        ReleaseTileSlot(self->pool, src[srcCol++]);
        srcCol &= 0x3F;
    }
}

/* Layer-0 `clipCols` (table +0x40; base `ClipBgLayerColumns` only narrows the
 * range): releases the columns that leave [lo, hi] one at a time. */
void ClipPooledBgLayerColumns(struct pooled_bg_layer *self, s32 lo, s32 hi)
{
    while (self->base.colLo < lo)
    {
        ReleasePooledBgLayerColumn(self, self->base.colLo);
        self->base.colLo++;
    }
    while (self->base.colHi > hi)
    {
        ReleasePooledBgLayerColumn(self, self->base.colHi);
        self->base.colHi--;
    }
}

/* Layer-0 `clipRows` (table +0x48): same for rows. */
void ClipPooledBgLayerRows(struct pooled_bg_layer *self, s32 lo, s32 hi)
{
    while (self->base.rowLo < lo)
    {
        ReleasePooledBgLayerRow(self, self->base.rowLo);
        self->base.rowLo++;
    }
    while (self->base.rowHi > hi)
    {
        ReleasePooledBgLayerRow(self, self->base.rowHi);
        self->base.rowHi--;
    }
}

/* Layer-0 `drawRow` (table +0x30): like `DrawBgLayerRow`, but each tile goes
 * through the pool. */
void DrawPooledBgLayerRow(struct pooled_bg_layer *self, s32 row)
{
    u16 *dst = &self->base.screen[Mod32(row) * 32];
    s32 srcCol;
    u16 *src = GetBgStreamerRow(self->base.streamer, self->base.colLo, row, &srcCol);
    s32 c;

    for (c = self->base.colLo; c <= self->base.colHi; c++)
    {
        s32 i = Mod32(c);

        dst[i] = AcquireTileSlot(self->pool, src[srcCol++]);
        srcCol &= 0x3F;
    }
}

/* Layer-0 `reset` (table +0x10): empties the pool, then the base reset. */
void ResetPooledBgLayer(struct pooled_bg_layer *self, void *pos)
{
    ResetTileSlotPool(self->pool);
    ResetBgLayer(&self->base, pos);
}

/* Layer-0 `loadTiles` (table +0x28): instead of unpacking the tile asset
 * into VRAM, points the pool at the character block and the asset's tile
 * data (past its 4-byte header). */
void LoadPooledBgLayerTiles(struct pooled_bg_layer *self)
{
    u32 tiles = (u32)self->base.tileData;

    SetTileSlotPoolSource(self->pool, self->base.cnt.bits.charBase, tiles + 4);
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/). */
void nullsub_26(void)
{
}

asm(".align 2, 0");
