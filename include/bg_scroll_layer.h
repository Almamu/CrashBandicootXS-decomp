#ifndef __BG_SCROLL_LAYER_H__
#define __BG_SCROLL_LAYER_H__

/* The BG-scroll layer (0x5C bytes, constructor `InitBgLayer` in
 * bg_layer_init.c, base method table `gBgLayerVtable`), and its
 * tile-slot-pooled subclass used for BG layer 0 (0x60 bytes, constructor
 * `InitPooledBgLayer` in tile_slot_pool.c, method table `gPooledBgLayerVtable`).
 * The level-layers singleton (level_layers.c) owns one of each kind per
 * hardware BG. See docs/matching/archive/issue-42-bg-scroll-layer.md.
 *
 * The layer keeps a 32x32-entry window of the level's tile map (a
 * `DecodeLayerChunk`-family ring-buffer streamer at `+0x2C`, cutscene_player.c)
 * resident in its BG screen block: `+0x3C..+0x40` is the resident tile
 * row range and `+0x44..+0x48` the column range. Methods draw/release
 * one row or column at a time as the window moves. */

struct bg_scroll_layer;

/* A method-table entry: `this` adjustment plus a function pointer, called
 * through the `_call_via_r1`/`_call_via_r2` `_call_via_rN` veneers. */
struct bg_layer_method {
    s16 thisOffset; // 0x0
    u8 unk_2[2];    // 0x2
    void *fn;       // 0x4
};

struct bg_layer_vtable {
    u8 unk_00[8];                     // 0x00
    struct bg_layer_method destroy;   // 0x08 - DestroyBgLayer / DestroyPooledBgLayer
    struct bg_layer_method reset;     // 0x10 - ResetBgLayer / ResetPooledBgLayer
    struct bg_layer_method method_18; // 0x18 - ScrollBgLayer
    // 0x20 - ClampBgLayerScrollStep / ClampPooledBgLayerScrollStep
    struct bg_layer_method method_20;
    struct bg_layer_method loadTiles; // 0x28 - LoadBgLayerTiles / LoadPooledBgLayerTiles
    struct bg_layer_method drawRow;   // 0x30 - DrawBgLayerRow / DrawPooledBgLayerRow
    struct bg_layer_method drawCol;   // 0x38 - DrawBgLayerColumn / DrawPooledBgLayerColumn
    struct bg_layer_method clipCols;  // 0x40 - ClipBgLayerColumns / ClipPooledBgLayerColumns
    struct bg_layer_method clipRows;  // 0x48 - ClipBgLayerRows / ClipPooledBgLayerRows
};

struct bg_scroll_layer {
    s32 x; // 0x00 - pixels
    s32 y; // 0x04
    /* 0x08-0x24: set from the level layer by SetBgLayerSource (bg_layer_base.c) */
    s32 maxX;                       // 0x08 - widthPx - 240, the scroll limit
    s32 maxY;                       // 0x0C - heightPx - 160
    s32 widthPx;                    // 0x10
    s32 heightPx;                   // 0x14
    s32 widthTiles;                 // 0x18
    s32 heightTiles;                // 0x1C
    s32 scaleX;                     // 0x20 - Q8 parallax factor
    s32 scaleY;                     // 0x24
    u8 enabled;                     // 0x28
    u8 unk_29[3];                   // 0x29
    void *streamer;                 // 0x2C - tile-map ring-buffer streamer
    struct bg_layer_vtable *vtable; // 0x30
    union {
        u16 raw;
        struct {
            u8 priority:2;   // BGnCNT bits 0-1
            u8 charBase:2;   // bits 2-3
            u8 unk4:3;       // bits 4-6 (bit 6 = mosaic)
            u8 colors256:1;  // bit 7
            u8 screenBase:5; // bits 8-12
            u8 unk13:3;      // bits 13-15
        } bits;
    } cnt;          // 0x34 - BGnCNT shadow (the union pads to 4 bytes)
    vu16 *cntReg;   // 0x38 - &REG_BGnCNT
    s32 rowLo;      // 0x3C - resident tile rows rowLo..rowHi
    s32 rowHi;      // 0x40
    s32 colLo;      // 0x44 - resident tile columns colLo..colHi
    s32 colHi;      // 0x48
    u16 *screen;    // 0x4C - BG screen block (32x32 entries)
    void *tileData; // 0x50 - tagged asset for the char block
    u16 hofs;       // 0x54
    u16 vofs;       // 0x56
    vu32 *ofsReg;   // 0x58 - &REG_BGnHOFS (written with VOFS as one word)
};

struct tile_slot_pool;

/* BG layer 0: a BG-scroll layer whose tiles go through a VRAM tile-slot
 * pool (tile_slot_pool.c). */
struct pooled_bg_layer {
    struct bg_scroll_layer base; // 0x00
    struct tile_slot_pool *pool; // 0x5C
};

#endif /* __BG_SCROLL_LAYER_H__ */
