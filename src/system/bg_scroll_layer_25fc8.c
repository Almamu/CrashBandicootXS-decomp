#include "core.h"
#include "bg_scroll_layer.h"

/* GitHub issue #42: the BG-scroll layer's own methods (`struct
 * bg_scroll_layer`, constructor `sub_8025D74` in game_loop15.c, method
 * table `gStaticData_087E4C14`) and the overrides of its tile-slot-pooled
 * subclass used for BG layer 0 (`struct pooled_bg_layer`, constructor
 * `sub_8026448` in tile_slot_pool.c, table `gStaticData_087E4C64`).
 *
 * The layer keeps a 32x32-entry window of the level's tile map resident in
 * its BG screen block. `+0x3C..+0x40` / `+0x44..+0x48` are the resident
 * tile row / column ranges; the streamer at `+0x2C` (game_loop57.c's
 * `sub_8024B18`/`sub_8024B48`) resolves a (column, row) of the level map
 * to its ring-buffer entries. Screen entries wrap modulo 32 on both axes.
 *
 * The base layer copies map entries straight into the screen block; the
 * layer-0 subclass instead routes each source tile through the VRAM
 * tile-slot pool (`sub_80264F8` acquire / `sub_80265A0` release) and
 * releases the tiles of rows/columns that scroll out (`sub_80262E8`,
 * `sub_8026328`).
 *
 * Compiled with old_agbcc (Makefile OLD_AGBCC_OBJS): the BGnCNT bitfield
 * setters (`sub_802614C`, `sub_8026160`, `sub_8026174`, `sub_8026190`)
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

extern u16 *sub_8024B18(void *streamer, s32 col, s32 row, s32 *rowOut);
extern u16 *sub_8024B48(void *streamer, s32 col, s32 row, s32 *colOut);
extern void sub_8024E90(struct bg_scroll_layer *self, void *pos);
extern void sub_8024EB4(struct bg_scroll_layer *self, struct bg_layer_desc *desc);
extern void sub_8024D74(struct bg_scroll_layer *self, u32 flags);
extern void sub_803AD7C(void *self, void *fn);
extern void sub_803AD80(void *self, s32 arg, void *fn);
extern void LoadTaggedAsset(void *asset, void *dest);
extern u16 sub_80264F8(struct tile_slot_pool *pool, u16 tile);
extern void sub_80265A0(struct tile_slot_pool *pool, u32 tile);
extern void sub_802648C(struct tile_slot_pool *pool);
extern void sub_8026618(struct tile_slot_pool *pool, s32 charBase, u32 src);
extern u8 gStaticData_087E4C14[];

static inline s32 Mod32(s32 v)
{
    return v % 32;
}

/* Base `drawRow` (table +0x30): copies the resident columns of map row
 * `row` from the streamer into screen row `row % 32`. */
void sub_8025FC8(struct bg_scroll_layer *self, s32 row)
{
    u16 *dst = &self->screen[Mod32(row) * 32];
    s32 srcCol;
    u16 *src = sub_8024B48(self->streamer, self->colLo, row, &srcCol);
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
void sub_802602C(struct bg_scroll_layer *self)
{
    s32 r;

    self->colLo = self->x / 8;
    self->colHi = (self->x + 0xEF) / 8;
    self->rowLo = self->y / 8;
    self->rowHi = (self->y + 0x9F) / 8;
    for (r = self->rowLo; r <= self->rowHi; r++)
        sub_803AD80((u8 *)self + self->vtable->drawRow.thisOffset, r, self->vtable->drawRow.fn);
}

/* Base `reset` (table +0x10): sets the position (`sub_8024E90`), reloads
 * the tiles (`loadTiles`), redraws the whole window and writes BGnCNT. */
void sub_802608C(struct bg_scroll_layer *self, void *pos)
{
    sub_8024E90(self, pos);
    sub_803AD7C((u8 *)self + self->vtable->loadTiles.thisOffset, self->vtable->loadTiles.fn);
    sub_802602C(self);
    *self->cntReg = self->cnt.raw;
}

/* Base `loadTiles` (table +0x28): unpacks the tile asset into the layer's
 * character base block. */
void sub_80260B4(struct bg_scroll_layer *self)
{
    LoadTaggedAsset(self->tileData, (void *)(VRAM + (self->cnt.bits.charBase << 14)));
}

/* Level-load hook for one layer (called by level_layers.c's `sub_80266BC`):
 * `sub_8024EB4` picks up the map size, then an enabled layer takes its
 * tile asset and BG priority from the descriptor. */
void sub_80260D4(struct bg_scroll_layer *self, struct bg_layer_desc *desc)
{
    sub_8024EB4(self, desc);
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
s32 sub_8026108(void *self, s32 col, s32 row)
{
    return Mod32(row) * 32 + Mod32(col);
}

/* Column wrap `col % 32`. Not a named symbol in the old disassembly (it was
 * folded into `sub_8026108`'s listing after its padding halfword).
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
void sub_802614C(struct bg_scroll_layer *self, u32 screenBase)
{
    self->cnt.bits.screenBase = screenBase;
}

void sub_8026160(struct bg_scroll_layer *self, u32 priority)
{
    self->cnt.bits.priority = priority;
}

void sub_8026174(struct bg_scroll_layer *self, u32 colors256)
{
    self->cnt.bits.colors256 = colors256;
}

u32 sub_8026184(struct bg_scroll_layer *self)
{
    return self->cnt.bits.charBase;
}

void sub_8026190(struct bg_scroll_layer *self, u32 charBase)
{
    self->cnt.bits.charBase = charBase;
}

/* Writes the cached HOFS/VOFS pair to the hardware registers.
 * UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/); level_layers.c uses `sub_8025F24` instead. */
void sub_80261A8(struct bg_scroll_layer *self)
{
    *self->ofsReg = *(u32 *)&self->hofs;
}

/* Writes the BGnCNT shadow to the hardware register (same as the tail of
 * `sub_802608C`). UNUSED - no caller anywhere in the ROM (checked asm/,
 * expected/ and src/). */
void sub_80261B0(struct bg_scroll_layer *self)
{
    *self->cntReg = self->cnt.raw;
}

/* Base `destroy` (table +0x08): restores the base table and chains to the
 * streamer-owning base destructor `sub_8024D74`. */
void sub_80261B8(struct bg_scroll_layer *self, u32 flags)
{
    self->vtable = (struct bg_layer_vtable *)gStaticData_087E4C14;
    sub_8024D74(self, flags);
}

/* Layer-0 `drawCol` (table +0x38): acquires a pool slot for every resident
 * row's tile in map column `col` and writes the returned map entry, moving
 * down one screen row (mod 32x32) per step. */
void sub_80261CC(struct pooled_bg_layer *self, s32 col)
{
    s32 srcRow;
    u16 *src = sub_8024B18(self->base.streamer, col, self->base.rowLo, &srcRow);
    s32 idx = Mod32(self->base.rowLo) * 32 + Mod32(col);
    s32 r;

    for (r = self->base.rowLo; r <= self->base.rowHi; r++)
    {
        self->base.screen[idx] = sub_80264F8(self->pool, src[srcRow * 64]);
        srcRow = (srcRow + 1) & 0x1F;
        idx = (idx + 32) % 0x400;
    }
}

/* Layer-0 override of table +0x20 (base: `sub_8024DCC`): clamps a scroll
 * step to [-8, 8]. */
s32 sub_8026250(struct pooled_bg_layer *self, s32 v)
{
    if (v < -8)
        v = -8;
    if (v > 8)
        v = 8;
    return v;
}

/* Releases the pool slots of map column `col`'s resident rows. */
void sub_8026264(struct pooled_bg_layer *self, s32 col)
{
    s32 srcRow;
    u16 *src = sub_8024B18(self->base.streamer, col, self->base.rowLo, &srcRow);
    s32 r;

    for (r = self->base.rowLo; r <= self->base.rowHi; r++)
    {
        sub_80265A0(self->pool, src[srcRow * 64]);
        srcRow = (srcRow + 1) & 0x1F;
    }
}

/* Releases the pool slots of map row `row`'s resident columns. */
void sub_80262A4(struct pooled_bg_layer *self, s32 row)
{
    s32 srcCol;
    u16 *src = sub_8024B48(self->base.streamer, self->base.colLo, row, &srcCol);
    s32 c;

    for (c = self->base.colLo; c <= self->base.colHi; c++)
    {
        sub_80265A0(self->pool, src[srcCol++]);
        srcCol &= 0x3F;
    }
}

/* Layer-0 `clipCols` (table +0x40; base `sub_8025E70` only narrows the
 * range): releases the columns that leave [lo, hi] one at a time. */
void sub_80262E8(struct pooled_bg_layer *self, s32 lo, s32 hi)
{
    while (self->base.colLo < lo)
    {
        sub_8026264(self, self->base.colLo);
        self->base.colLo++;
    }
    while (self->base.colHi > hi)
    {
        sub_8026264(self, self->base.colHi);
        self->base.colHi--;
    }
}

/* Layer-0 `clipRows` (table +0x48): same for rows. */
void sub_8026328(struct pooled_bg_layer *self, s32 lo, s32 hi)
{
    while (self->base.rowLo < lo)
    {
        sub_80262A4(self, self->base.rowLo);
        self->base.rowLo++;
    }
    while (self->base.rowHi > hi)
    {
        sub_80262A4(self, self->base.rowHi);
        self->base.rowHi--;
    }
}

/* Layer-0 `drawRow` (table +0x30): like `sub_8025FC8`, but each tile goes
 * through the pool. */
void sub_8026368(struct pooled_bg_layer *self, s32 row)
{
    u16 *dst = &self->base.screen[Mod32(row) * 32];
    s32 srcCol;
    u16 *src = sub_8024B48(self->base.streamer, self->base.colLo, row, &srcCol);
    s32 c;

    for (c = self->base.colLo; c <= self->base.colHi; c++)
    {
        s32 i = Mod32(c);

        dst[i] = sub_80264F8(self->pool, src[srcCol++]);
        srcCol &= 0x3F;
    }
}

/* Layer-0 `reset` (table +0x10): empties the pool, then the base reset. */
void sub_80263DC(struct pooled_bg_layer *self, void *pos)
{
    sub_802648C(self->pool);
    sub_802608C(&self->base, pos);
}

/* Layer-0 `loadTiles` (table +0x28): instead of unpacking the tile asset
 * into VRAM, points the pool at the character block and the asset's tile
 * data (past its 4-byte header). */
void sub_80263F8(struct pooled_bg_layer *self)
{
    u32 tiles = (u32)self->base.tileData;

    sub_8026618(self->pool, self->base.cnt.bits.charBase, tiles + 4);
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, expected/ and
 * src/). */
void nullsub_26(void)
{
}

asm(".align 2, 0");
