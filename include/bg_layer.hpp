#ifndef GUARD_BG_LAYER_HPP
#define GUARD_BG_LAYER_HPP

/* The level's background layers as C++ (#664, docs/cplusplus.md, step
 * 10b): the tile-map streamer, the BG layer classes behind
 * gBgStreamerVtable, gBgLayerBaseVtable, gBgLayerVtable and
 * gPooledBgLayerVtable, and the level-layers singleton that owns them.
 *
 * No `#pragma interface`: g++ emits each vtable in the object that
 * defines the class's key method, its destructor (BgStreamer's and
 * BgLayerBase's in src/cutscene/cutscene_player.cpp, PooledBgLayer's in
 * src/level/tile_slot_pool.cpp; BgLayer's is inline, so its key method is
 * Reset, in src/level/bg_layer.cpp),
 * and ldscript.txt places them at their ROM addresses (docs/cplusplus.md,
 * "Emitting the vtables"). cxx_symbols.txt maps the mangled names onto
 * the C names. bg_scroll_layer.h's struct bg_scroll_layer is the C view
 * of a layer, for the files that read gLevelLayers's layers. */

extern "C" {
#include "core.h"
#include "bg_scroll_layer.h"
#include "level_data.h"
#include "level.h"
}

/* The tile-map ring buffer of a BG layer (BgLayerBase::streamer, 0x24
 * bytes): a circular 4x4-block (64 halfword columns x 32 rows, 0x1000
 * bytes) window of the layer's decoded chunks, around the camera's
 * 128x64-pixel chunk (`tileX`, `tileY`). `subX`/`subY` are the ring's
 * block (mod 4) at the window's left and top edges. Scroll streams in
 * the chunk row or column the camera has just reached, Fill seeds the
 * whole window. Its code is in src/cutscene/cutscene_player.cpp. */
class BgStreamer
{
public:
    const struct level_layer_desc *source; // 0x00
    const u16 *records;                    // 0x04 - the chunk decoder's record table
    u8 *ring;                              // 0x08 - 0x1000 bytes
    s32 tileX;                             // 0x0C - the window's chunk column
    s32 tileY;                             // 0x10 - and row
    u8 subX;                               // 0x14
    u8 subY;                               // 0x15
    s32 widthTiles;                        // 0x18 - the source's widthTiles
    s32 heightTiles;                       // 0x1C
    // 0x20: the vtable pointer, gBgStreamerVtable

    BgStreamer();                                    // InitBgStreamer
    virtual ~BgStreamer();                           // 1 DestroyBgStreamer
    void DecodeChunk(s32 recordId, u16 *dest);       // DecodeLayerChunk
    void Scroll(const s32 *worldpos);                // ScrollBgStreamer
    u16 *GetColumn(s32 x, s32 y, s32 *rowOut);       // GetBgStreamerColumn
    u16 *GetRow(s32 x, s32 y, s32 *colOut);          // GetBgStreamerRow
    u16 GetCell(s32 x, s32 y);                       // GetBgStreamerCell
    void StreamRow(s32 row);                         // StreamBgRow
    void StreamColumn(s32 col);                      // StreamBgColumn
    void Fill(const s32 *pos);                       // FillBgStreamer
    void SetSource(const struct level_layer_desc *); // SetBgStreamerSource
    s32 GetHeight();                                 // GetBgStreamerHeight
    s32 GetWidth();                                  // GetBgStreamerWidth
    void SetSize(const s32 *size);                   // SetBgStreamerSizeVec
    void SetSize(s32 x, s32 y);                      // SetBgStreamerSize
};

COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(BgStreamer) == 0x24);

/* A scrolling layer's position and size (0x34 bytes): the base of
 * BgLayer. Its methods are in src/cutscene/cutscene_player.cpp and
 * src/level/bg_layer_base.cpp. Scroll scales a move by the layer's
 * parallax factors and steps the position by it, ClampScrollStep (virtual)
 * limiting each axis's step. */
class BgLayerBase
{
public:
    s32 x;                // 0x00 - pixels
    s32 y;                // 0x04
    s32 maxX;             // 0x08 - widthPx - 240, the scroll limit
    s32 maxY;             // 0x0C - heightPx - 160
    s32 widthPx;          // 0x10
    s32 heightPx;         // 0x14
    s32 widthTiles;       // 0x18
    s32 heightTiles;      // 0x1C
    s32 scaleX;           // 0x20 - Q8 parallax factor
    s32 scaleY;           // 0x24
    u8 enabled;           // 0x28
    BgStreamer *streamer; // 0x2C
    // 0x30: the vtable pointer, gBgLayerBaseVtable or a subclass's

    BgLayerBase(s32 unused);                               // InitBgLayerBase
    virtual ~BgLayerBase();                                // 1 DestroyBgLayerBase
    virtual void Reset(const s32 *pos);                    // 2 ResetBgLayerBase
    virtual void Scroll(const s32 *delta);                 // 3 ScrollBgLayerBase
    virtual s32 ClampScrollStep(s32 step);                 // 4 ClampBgLayerScrollStep
    void ClampScrollMax(s32 *pos);                         // ClampBgLayerScrollMax
    void ScaleScroll(s32 *vec);                            // ScaleBgLayerScroll
    void StepScroll(const s32 *target);                    // StepBgLayerScroll
    void SetSource(const struct level_layer_desc *source); // SetBgLayerSource
    u8 IsEnabled();                                        // IsBgLayerEnabled
    s32 GetY();                                            // GetBgLayerY
    s32 GetX();                                            // GetBgLayerX
    s32 GetHeightTiles();                                  // GetBgLayerHeightTiles
    s32 GetWidthTiles();                                   // GetBgLayerWidthTiles
    s32 GetHeight();                                       // GetBgLayerHeight
    s32 GetWidth();                                        // GetBgLayerWidth
};

COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(BgLayerBase) == 0x34);

/* A hardware BG layer (0x5C bytes; level_layers' BG1-3): keeps a 32x32
 * window of the level's tile map resident in its BG screen block, rows
 * rowLo..rowHi and columns colLo..colHi, drawing and clipping one row or
 * column at a time as the window moves. Screen entries wrap modulo 32 on
 * both axes. Its methods are in src/level/bg_layer_init.cpp and
 * src/level/bg_layer.cpp.
 *
 * The destructor and the small accessors are inline: the subclasses and
 * the methods expand them, and bg_layer.cpp, which has the class's key
 * method (Reset, its first non-inline virtual one) and so its vtable,
 * also gets an out-of-line copy of each at its end, in the reverse of
 * their declaration order, as the ROM has them (GetBgLayerScreenIndex ...
 * DestroyBgLayer). Nothing calls those copies. GetScreenIndex is declared
 * after the two wraps it calls, or g++ would call them out of line. */
class BgLayer : public BgLayerBase
{
public:
    union bg_cnt cnt; // 0x34 - BGnCNT shadow
    vu16 *cntReg;     // 0x38 - &REG_BGnCNT
    s32 rowLo;        // 0x3C
    s32 rowHi;        // 0x40
    s32 colLo;        // 0x44
    s32 colHi;        // 0x48
    u16 *screen;      // 0x4C - BG screen block (32x32 entries)
    void *tileData;   // 0x50 - tagged asset for the char block
    u16 hofs;         // 0x54
    u16 vofs;         // 0x56
    vu32 *ofsReg;     // 0x58 - &REG_BGnHOFS (written with VOFS as one word)

    BgLayer(s32 bgIndex); // InitBgLayer

    /* 1 DestroyBgLayer: stores gBgLayerVtable and chains to ~BgLayerBase. */
    virtual ~BgLayer()
    {
    }

    virtual void Reset(const s32 *pos);             // 2 ResetBgLayer
    virtual void Scroll(const s32 *delta);          // 3 ScrollBgLayer
    virtual void LoadTiles();                       // 5 LoadBgLayerTiles
    virtual void DrawRow(s32 row);                  // 6 DrawBgLayerRow
    virtual void DrawColumn(s32 col);               // 7 DrawBgLayerColumn
    virtual void ClipColumns(s32 lo, s32 hi);       // 8 ClipBgLayerColumns
    virtual void ClipRows(s32 lo, s32 hi);          // 9 ClipBgLayerRows
    void GrowRows(s32 lo, s32 hi);                  // GrowBgLayerRows
    void GrowColumns(s32 lo, s32 hi);               // GrowBgLayerColumns
    void CommitScroll();                            // CommitBgLayerScroll
    void Redraw();                                  // RedrawBgLayer
    void Load(const struct level_layer_desc *desc); // LoadBgLayer

    /* Writes the BGnCNT shadow to the hardware register (WriteBgLayerCntReg). */
    void WriteCntReg()
    {
        *cntReg = cnt.raw;
    }

    /* Writes the HOFS/VOFS pair as one word (WriteBgLayerOffsetRegs). */
    void WriteOffsetRegs()
    {
        *ofsReg = *(u32 *)&hofs;
    }

    /* The BGnCNT shadow's fields (SetBgLayerCharBase ... SetBgLayerScreenBase). */
    void SetCharBase(u32 charBase)
    {
        cnt.bits.charBase = charBase;
    }

    u32 GetCharBase()
    {
        return cnt.bits.charBase;
    }

    void SetColors256(u32 colors256)
    {
        cnt.bits.colors256 = colors256;
    }

    void SetPriority(u32 priority)
    {
        cnt.bits.priority = priority;
    }

    void SetScreenBase(u32 screenBase)
    {
        cnt.bits.screenBase = screenBase;
    }

    /* A map row's and column's screen row and column: 32x32 wrap
     * (WrapBgLayerRow, WrapBgLayerColumn; the two are byte-identical, and
     * which is which is assumed from GetScreenIndex's (col, row) order). */
    s32 WrapRow(s32 row)
    {
        return row % 32;
    }

    s32 WrapColumn(s32 col)
    {
        return col % 32;
    }

    /* Screen-block entry index of map (col, row) (GetBgLayerScreenIndex). */
    s32 GetScreenIndex(s32 col, s32 row)
    {
        return WrapRow(row) * 32 + WrapColumn(col);
    }
};

COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(BgLayer) == 0x5C);

/* bg_scroll_layer.h's struct bg_scroll_layer, the C view (its `vtable` is
 * the class's vtable pointer). */
COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(BgLayer) == sizeof(struct bg_scroll_layer));
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, x);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, y);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, maxX);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, maxY);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, widthPx);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, heightPx);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, widthTiles);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, heightTiles);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, scaleX);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, scaleY);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, enabled);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, streamer);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, cnt);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, cntReg);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, rowLo);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, rowHi);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, colLo);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, colHi);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, screen);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, tileData);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, hofs);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, vofs);
ASSERT_VIEW_FIELD(bg_layer_hpp, BgLayer, bg_scroll_layer, ofsReg);

#define TILE_SLOT_NONE 0x200

/* Layer 0's VRAM tile-slot pool (PooledBgLayer::pool, 0x480C bytes;
 * src/level/tile_slot_pool.cpp): up to 0x2000 source tiles mapped onto
 * 0x200 reference-counted VRAM tile slots. PooledBgLayer's constructor
 * allocates it (`new TileSlotPool`, no constructor) and Reset empties it.
 * It has no vtable. */
class TileSlotPool
{
public:
    u32 vramBase;            // 0x0000
    u32 srcBase;             // 0x0004
    u16 refCount[0x200];     // 0x0008
    u16 slotForTile[0x2000]; // 0x0408 - TILE_SLOT_NONE when not resident
    u16 freeSlots[0x200];    // 0x4408
    s32 freeTop;             // 0x4808

    void Reset();                          // ResetTileSlotPool
    u16 Acquire(u16 tile);                 // AcquireTileSlot
    void Release(u32 tile);                // ReleaseTileSlot
    void Upload(s32 tileId, s32 slot);     // UploadTileSlot
    void SetSource(s32 charBase, u32 src); // SetTileSlotPoolSource

private:
    /* The free-stack push and the slot-table accessors: as inline
     * functions they make the ROM recompute `this + 0x4808`/`this +
     * 0x408` instead of reusing one address register (code written in
     * place CSEs them). */
    void PushFreeSlot(s32 slot)
    {
        freeSlots[--freeTop] = slot;
    }

    void ClearSlot(s32 id)
    {
        slotForTile[id] = TILE_SLOT_NONE;
    }

    u16 PopFreeSlot()
    {
        return freeSlots[freeTop++];
    }

    void SetSlot(s32 id, u16 slot)
    {
        slotForTile[id] = slot;
    }

    u16 GetSlot(s32 id)
    {
        return slotForTile[id];
    }
};

COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(TileSlotPool) == 0x480C);

/* BG layer 0 (0x60 bytes): a BgLayer whose tiles go through a VRAM tile
 * slot pool (TileSlotPool, src/level/tile_slot_pool.cpp) instead
 * of the layer's own character block, releasing the tiles of the rows and
 * columns that scroll out. Its methods are in src/level/pooled_bg_layer.cpp
 * and src/level/tile_slot_pool.cpp. */
class PooledBgLayer : public BgLayer
{
public:
    TileSlotPool *pool; // 0x5C

    PooledBgLayer(s32 bgIndex);               // InitPooledBgLayer
    virtual ~PooledBgLayer();                 // 1 DestroyPooledBgLayer
    virtual void Reset(const s32 *pos);       // 2 ResetPooledBgLayer
    virtual s32 ClampScrollStep(s32 step);    // 4 ClampPooledBgLayerScrollStep
    virtual void LoadTiles();                 // 5 LoadPooledBgLayerTiles
    virtual void DrawRow(s32 row);            // 6 DrawPooledBgLayerRow
    virtual void DrawColumn(s32 col);         // 7 DrawPooledBgLayerColumn
    virtual void ClipColumns(s32 lo, s32 hi); // 8 ClipPooledBgLayerColumns
    virtual void ClipRows(s32 lo, s32 hi);    // 9 ClipPooledBgLayerRows
    void ReleaseColumn(s32 col);              // ReleasePooledBgLayerColumn
    void ReleaseRow(s32 row);                 // ReleasePooledBgLayerRow
    u32 GetPriority();                        // GetPooledBgLayerPriority
};

COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(PooledBgLayer) == 0x60);

/* The collision tile cache (LevelLayers' `tiles`, 0x1064 bytes; level.h's
 * struct level_layers sees a `TileCache *`): a 16-slot decode/LRU cache of
 * the room's collision tile records (docs/rom_map.md's "Collision/terrain-map
 * streamer"). `id[N]` holds the record ID currently decoded into the
 * matching 256-byte `buf[N]` slot; `nextSlot` is the ring-buffer eviction
 * cursor GetChunk advances every time it decodes a new record (evicting
 * slot `(nextSlot - 1) & 0xf`, the slot filled just before the cursor).
 * `source` is the level_layer_desc SetSource was given, read as a pointer
 * to the record grid. It has no vtable. Its methods are in
 * src/level/bg_layer_base.cpp (GetChunk, the terrain lookups, DecodeChunk),
 * src/level/tile_cache.cpp (the constructor, destructor and GetTerrainType)
 * and src/level/collision_map.cpp (GetCell, SetSource). */
class TileCache
{
public:
    void *source;      // 0x000
    void *decodeBase;  // 0x004 - gLevelLayers->asset + source->assetOffset; DecodeChunk's table
    s32 widthPx;       // 0x008 - widthTiles << 3; nothing reads it
    s32 heightPx;      // 0x00C - heightTiles << 3; nothing reads it
    s32 widthTiles;    // 0x010 - source->widthTiles (ProbeTerrainY's right edge)
    s32 heightTiles;   // 0x014 - source->heightTiles (ProbeTerrainX's bottom edge)
    s32 width;         // 0x018 - tiles, source->gridWidth
    s32 height;        // 0x01C - tiles, source->gridHeight; nothing reads it
    u8 buf[16][0x100]; // 0x020 - 16 decoded 256-byte chunks
    s32 id[16];        // 0x1020 - the record IDs resident in `buf`
    s32 nextSlot;      // 0x1060

    TileCache();                           // InitTileCache
    ~TileCache();                          // DestroyTileCache
    void *GetChunk(s32 recordId);          // GetCollisionChunk
    void *GetTerrainHeights(s32 x, s32 y); // GetTerrainHeights
    void *GetSolidTerrainHeights(s32 x, s32 y, s32 mode, u8 *flagsOut);
    s8 GetSolidTerrainModeValue(s32 x, s32 y, s32 mode, u8 *flagsOut);
    void DecodeChunk(s32 recordId, void *dest); // DecodeCollisionChunk
    u16 GetTerrainType(s32 x, s32 y, u8 *flagsOut, s32 *hiOut);
    u16 GetCell(s32 x, s32 y);                       // GetCollisionCell
    void SetSource(struct level_layer_desc *source); // SetCollisionSource

private:
    /* The decoded cell at pixel (x, y): 16x8-pixel tiles, one 256-byte
     * cache slot per tile record. Inlined into the terrain lookups. */
    u16 CellAt(s32 x, s32 y)
    {
        s32 tileX = x >> 4;
        s32 tileY = y >> 3;
        u16 *cell = (u16 *)GetChunk((*(u16 **)source)[tileY * width + tileX]);
        return cell[(y & 7) * 16 + (x & 0xf)];
    }
};

COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(TileCache) == 0x1064);

/* The level-layers singleton (gLevelLayersSingleton, gLevelLayers;
 * src/level/level_layers.cpp): BG layer 0, the three other BG layers, the
 * collision tile cache and the level asset.
 * level.h's struct level_layers is its C view. It has no vtable. */
class LevelLayers
{
public:
    s32 maxScrollX;        // 0x00 - pixels
    s32 maxScrollY;        // 0x04
    s32 scrollX;           // 0x08 - pixels
    s32 scrollY;           // 0x0C
    PooledBgLayer *layer0; // 0x10
    BgLayer *layers[3];    // 0x14
    TileCache *tiles;      // 0x20
    void *asset;           // 0x24
    u8 assetOwned;         // 0x28
    u8 kind;               // 0x29
    u8 probeFlag;          // 0x2A
    u8 raiseObjPriority;   // 0x2B

    LevelLayers();                                // InitLevelLayers
    ~LevelLayers();                               // DestroyLevelLayers
    static LevelLayers *Get();                    // GetLevelLayers
    void LoadRoom(const struct level_room *args); // LoadRoom
    void SetScroll(s32 x, s32 y);                 // SetLevelScroll
    void CommitScroll();                          // CommitLevelScroll
    void Scroll();                                // ScrollLevelLayers
    void Reset();                                 // ResetLevelLayers
};


/* level.h's struct level_layers, the C view. */
COMPILE_TIME_ASSERT(bg_layer_hpp, sizeof(LevelLayers) == sizeof(struct level_layers));
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, maxScrollX);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, maxScrollY);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, scrollX);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, scrollY);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, layer0);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, layers);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, tiles);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, asset);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, assetOwned);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, kind);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, probeFlag);
ASSERT_VIEW_FIELD(bg_layer_hpp, LevelLayers, level_layers, raiseObjPriority);

#endif /* !GUARD_BG_LAYER_HPP */
