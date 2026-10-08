#include "bg_layer.hpp"

extern "C" {
#include "math_util.h"
}

/* Sets up the layer for hardware BG `bgIndex`: screen block
 * `bgIndex + 0x1c`'s address as `screen`, `&REG_BGnCNT` as `cntReg`,
 * `&REG_BGnHOFS` as `ofsReg`, and the BGnCNT shadow `cnt` (screen base
 * `(bgIndex + 0x1c) & 0x1f`, char base 2, priority 0). */
BgLayer::BgLayer(s32 bgIndex) : BgLayerBase(bgIndex)
{
    s32 block = bgIndex + 0x1c;

    screen = (u16 *)BG_SCREEN_ADDR(block);
    cntReg = (vu16 *)(bgIndex * 2 + REG_ADDR_BG0CNT);
    ofsReg = (vu32 *)(bgIndex * 4 + REG_ADDR_BG0HOFS);
    cnt.raw = 0;
    SetColors256(0);
    SetScreenBase(block);
    SetCharBase(2);
}

/* Grows the resident row range down to `lo` and up to `hi` one row at a
 * time, drawing each new row (DrawRow, virtual): the streamed-range
 * grower Scroll drives for one axis; GrowColumns is its twin for the
 * other. */
void BgLayer::GrowRows(s32 lo, s32 hi)
{
    while (rowLo > lo) {
        s32 v = rowLo - 1;

        rowLo = v;
        DrawRow(v);
    }
    while (rowHi < hi) {
        s32 v = rowHi + 1;

        rowHi = v;
        DrawRow(v);
    }
}

/* Same shape as GrowRows, for the resident columns (DrawColumn). */
void BgLayer::GrowColumns(s32 lo, s32 hi)
{
    while (colLo > lo) {
        s32 v = colLo - 1;

        colLo = v;
        DrawColumn(v);
    }
    while (colHi < hi) {
        s32 v = colHi + 1;

        colHi = v;
        DrawColumn(v);
    }
}

/* Narrows the resident column range to [lo, hi]: the base layer only
 * records it (PooledBgLayer's override also releases the tiles). */
void BgLayer::ClipColumns(s32 lo, s32 hi)
{
    LIMIT_MIN(colLo, lo);
    LIMIT_MAX(colHi, hi);
}

/* Same as ClipColumns, for the resident rows. */
void BgLayer::ClipRows(s32 lo, s32 hi)
{
    LIMIT_MIN(rowLo, lo);
    LIMIT_MAX(rowHi, hi);
}
