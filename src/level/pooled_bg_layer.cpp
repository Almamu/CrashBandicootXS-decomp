#include "bg_layer.hpp"

extern "C" {
#include "math_util.h"
#include "gfx.h"
#include "memory.h"
}

/* GitHub issue #42: the overrides of BgLayer's tile-slot-pooled subclass
 * used for BG layer 0 (PooledBgLayer, include/bg_layer.hpp), then (GitHub
 * issue #43) its destructor, constructor and GetPriority, which were in
 * tile_slot_pool.cpp until #770. The class is BgLayer extended with a
 * TileSlotPool at `+0x5C`: the constructor sets the 256-colour bit and
 * char base 0 and allocates the pool, the destructor frees it and then
 * expands BgLayer's inline one. The destructor is the class's key method:
 * g++ emits gPooledBgLayerVtable here. Instead of copying map entries straight into the
 * screen block, it routes each source tile through the VRAM tile-slot pool
 * (`AcquireTileSlot` acquire / `ReleaseTileSlot` release) and releases
 * the tiles of rows/columns that scroll out (ClipColumns, ClipRows).
 *
 * Compiled with old_agbcp (Makefile OLD_AGBCC_OBJS), as bg_layer.cpp, the
 * other half of what was one C file. Real bytes formerly the end of
 * `asm/code_3_2_17_25fc8.s`. */

/* Layer-0 DrawColumn (slot 7): acquires a pool slot for every resident
 * row's tile in map column `col` and writes the returned map entry, moving
 * down one screen row (mod 32x32) per step. */
void PooledBgLayer::DrawColumn(s32 col)
{
    s32 srcRow;
    u16 *src = streamer->GetColumn(col, rowLo, &srcRow);
    s32 idx = GetScreenIndex(col, rowLo);
    s32 r;

    for (r = rowLo; r <= rowHi; r++) {
        screen[idx] = pool->Acquire(src[srcRow * 64]);
        srcRow = (srcRow + 1) & 0x1F;
        idx = (idx + 32) % 0x400;
    }
}

/* Layer-0 override of slot 4 (base: BgLayerBase::ClampScrollStep): clamps
 * a scroll step to [-8, 8]. */
s32 PooledBgLayer::ClampScrollStep(s32 step)
{
    LIMIT_MIN(step, -8);
    LIMIT_MAX(step, 8);
    return step;
}

/* Releases the pool slots of map column `col`'s resident rows. */
void PooledBgLayer::ReleaseColumn(s32 col)
{
    s32 srcRow;
    u16 *src = streamer->GetColumn(col, rowLo, &srcRow);
    s32 r;

    for (r = rowLo; r <= rowHi; r++) {
        pool->Release(src[srcRow * 64]);
        srcRow = (srcRow + 1) & 0x1F;
    }
}

/* Releases the pool slots of map row `row`'s resident columns. */
void PooledBgLayer::ReleaseRow(s32 row)
{
    s32 srcCol;
    u16 *src = streamer->GetRow(colLo, row, &srcCol);
    s32 c;

    for (c = colLo; c <= colHi; c++) {
        pool->Release(src[srcCol++]);
        srcCol &= 0x3F;
    }
}

/* Layer-0 ClipColumns (slot 8; the base only narrows the range):
 * releases the columns that leave [lo, hi] one at a time. */
void PooledBgLayer::ClipColumns(s32 lo, s32 hi)
{
    while (colLo < lo) {
        ReleaseColumn(colLo);
        colLo++;
    }
    while (colHi > hi) {
        ReleaseColumn(colHi);
        colHi--;
    }
}

/* Layer-0 ClipRows (slot 9): same for rows. */
void PooledBgLayer::ClipRows(s32 lo, s32 hi)
{
    while (rowLo < lo) {
        ReleaseRow(rowLo);
        rowLo++;
    }
    while (rowHi > hi) {
        ReleaseRow(rowHi);
        rowHi--;
    }
}

/* Layer-0 DrawRow (slot 6): like BgLayer::DrawRow, but each tile goes
 * through the pool. */
void PooledBgLayer::DrawRow(s32 row)
{
    u16 *dst = &screen[WrapRow(row) * 32];
    s32 srcCol;
    u16 *src = streamer->GetRow(colLo, row, &srcCol);
    s32 c;

    for (c = colLo; c <= colHi; c++) {
        s32 i = WrapColumn(c);

        dst[i] = pool->Acquire(src[srcCol++]);
        srcCol &= 0x3F;
    }
}

/* Layer-0 Reset (slot 2): empties the pool, then the base reset. */
void PooledBgLayer::Reset(const s32 *pos)
{
    pool->Reset();
    BgLayer::Reset(pos);
}

/* Layer-0 LoadTiles (slot 5): instead of unpacking the tile asset into
 * VRAM, points the pool at the character block and the asset's tile
 * data (past its 4-byte header). */
void PooledBgLayer::LoadTiles()
{
    u32 tiles = (u32)tileData;

    pool->SetSource(GetCharBase(), tiles + 4);
}

/* UNUSED - no caller anywhere in the ROM (checked asm/, expected/, src/
 * and every word-aligned Thumb pointer in baserom.gba). Empty and in no
 * method table (PooledBgLayer's slots all point elsewhere), so it keeps
 * the nullsub_N name (docs/naming.md). */
void nullsub_26(void)
{
}

PooledBgLayer::~PooledBgLayer()
{
    if (pool != NULL)
        delete pool;
}

/* Through BgLayer's inline setters, each field store is a general insert
 * (the field cleared, then the value ORed in) even with a constant: the
 * ROM's `& 0x7f` before the `| 0x80`. */
PooledBgLayer::PooledBgLayer(s32 bgIndex) : BgLayer(bgIndex)
{
    SetColors256(1);
    SetCharBase(0);
    pool = new TileSlotPool;
}

/* UNUSED - no caller anywhere in the ROM (checked src/, asm/ and the
 * method tables). Layer 0's BG priority, BGnCNT bits 0-1. */
u32 PooledBgLayer::GetPriority()
{
    return cnt.bits.priority;
}
