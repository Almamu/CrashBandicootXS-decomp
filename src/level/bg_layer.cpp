#include "bg_layer.hpp"

extern "C" {
#include "math_util.h"
#include "system.h"
}

/* GitHub issue #42: BgLayer's own methods (include/bg_layer.hpp,
 * gBgLayerVtable; the constructor and the grow/clip methods are in
 * bg_layer_init.cpp). Reset is the class's key method, so g++ emits
 * gBgLayerVtable here, and after the methods the out-of-line copies of
 * the class's inline methods (GetBgLayerScreenIndex ... DestroyBgLayer,
 * the end of this object): nothing calls them, the code expands them.
 * The overrides of its tile-slot-pooled subclass used for BG layer 0
 * follow in pooled_bg_layer.cpp.
 *
 * The layer keeps a 32x32-entry window of the level's tile map resident in
 * its BG screen block: `rowLo..rowHi` / `colLo..colHi` are the resident
 * tile row / column ranges, and the streamer (BgStreamer::GetColumn/GetRow,
 * bg_streamer.cpp) resolves a (column, row) of the level map to its
 * ring-buffer entries. Screen entries wrap modulo 32 on both axes.
 *
 * The base layer copies map entries straight into the screen block.
 *
 * Compiled with old_agbcp (Makefile OLD_AGBCC_OBJS): the BGnCNT bitfield
 * setters' copies (`SetScreenBase`, `SetPriority`, `SetColors256`,
 * `SetCharBase`) schedule the mask constant before the `ldrb`, which the
 * current agbcc never does. See
 * docs/matching/archive/issue-42-bg-scroll-layer.md.
 *
 * Real bytes formerly part of `asm/code_3_2_17_25fc8.s`. */

/* Moves the layer (BgLayerBase::Scroll), then works out the tile columns
 * and rows the 240x160 screen covers at the new position, clips the
 * resident range to them (ClipRows/ClipColumns, virtual), refreshes the
 * streamer and streams the new range in (GrowColumns/GrowRows). */
void BgLayer::Scroll(const s32 *delta)
{
    s32 cLo;
    s32 cHi;
    s32 rLo;
    s32 rHi;

    BgLayerBase::Scroll(delta);
    cLo = x / 8;
    cHi = (x + 0xef) / 8;
    rLo = y / 8;
    rHi = (y + 0x9f) / 8;
    ClipRows(rLo, rHi);
    ClipColumns(cLo, cHi);
    streamer->Scroll(&x);
    GrowColumns(cLo, cHi);
    GrowRows(rLo, rHi);
}

/* Truncates the X/Y position to the `hofs`/`vofs` halfwords, then
 * writes the pair as one word through `ofsReg` - the `BGnHOFS`/`BGnVOFS`
 * register pair address the constructor (bg_layer_init.cpp) caches there. */
void BgLayer::CommitScroll()
{
    hofs = x;
    vofs = y;
    WriteOffsetRegs();
}

/* Base DrawColumn (slot 7): copies the resident rows of map column `col`
 * from the streamer into screen column `col % 32`. */
void BgLayer::DrawColumn(s32 col)
{
    s32 srcRow;
    u16 *src = streamer->GetColumn(col, rowLo, &srcRow);
    s32 i = GetScreenIndex(col, rowLo);
    s32 r;

    for (r = rowLo; r <= rowHi; r++) {
        screen[i] = src[srcRow * 64];
        srcRow = (srcRow + 1) & 0x1f;
        i = (i + 32) % 0x400;
    }
}

/* Base DrawRow (slot 6): copies the resident columns of map row `row`
 * from the streamer into screen row `row % 32`. */
void BgLayer::DrawRow(s32 row)
{
    u16 *dst = &screen[WrapRow(row) * 32];
    s32 srcCol;
    u16 *src = streamer->GetRow(colLo, row, &srcCol);
    s32 c = colLo;
    s32 end;
    s32 mask;

    /* A goto loop, not for/while: with a structured loop gcc's load_mems
     * keeps `srcCol` in a register across the loop and stores it once
     * after it; the ROM reloads/stores it through the stack every
     * iteration. Explicit `end`/`mask` locals give the ROM's hoisted
     * r4/r7. See docs/matching/archive/issue-42-bg-scroll-layer.md. */
    if (c > colHi)
        return;
    mask = 0x3F;
    end = colHi;
loop:
    dst[WrapColumn(c)] = src[srcCol];
    srcCol = (srcCol + 1) & mask;
    c++;
    if (c <= end)
        goto loop;
}

/* Recomputes the resident tile ranges from the pixel scroll position (a
 * 240x160 screen spans columns x/8..(x+239)/8 and rows y/8..(y+159)/8),
 * then draws every resident row (DrawRow, virtual). */
void BgLayer::Redraw()
{
    s32 r;

    colLo = x / 8;
    colHi = (x + 0xEF) / 8;
    rowLo = y / 8;
    rowHi = (y + 0x9F) / 8;
    for (r = rowLo; r <= rowHi; r++)
        DrawRow(r);
}

/* Base Reset (slot 2): sets the position (BgLayerBase::Reset), reloads
 * the tiles (LoadTiles, virtual), redraws the whole window and writes
 * BGnCNT. */
void BgLayer::Reset(const s32 *pos)
{
    BgLayerBase::Reset(pos);
    LoadTiles();
    Redraw();
    WriteCntReg();
}

/* Base LoadTiles (slot 5): unpacks the tile asset into the layer's
 * character base block. */
void BgLayer::LoadTiles()
{
    LoadTaggedAsset(tileData, (void *)(VRAM + (GetCharBase() << 14)));
}

/* Level-load hook for one layer (called by level_layers.cpp's `LoadRoom`):
 * `SetSource` picks up the map size, then an enabled layer takes its
 * tile asset and BG priority from the descriptor. */
void BgLayer::Load(const struct level_layer_desc *desc)
{
    SetSource(desc);
    if (enabled) {
        u16 c;

        tileData = (void *)desc->tileData;
        c = desc->cnt;
        SetPriority(c);
    }
}
