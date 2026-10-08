#ifndef __BG_SCROLL_LAYER_H__
#define __BG_SCROLL_LAYER_H__

/* A BG layer of the level-layers singleton (gLevelLayers->layer0 and
 * layers[]), as the C++ class BgLayer (include/bg_layer.hpp, 0x5C bytes;
 * layer 0 is a PooledBgLayer, 0x60 bytes) has it: this is its C view, for
 * the files that read a layer's position and size. The layer keeps a
 * 32x32-entry window of the level's tile map (the BgStreamer at `+0x2C`)
 * resident in its BG screen block: `+0x3C..+0x40` is the resident tile
 * row range and `+0x44..+0x48` the column range. See
 * docs/matching/archive/issue-42-bg-scroll-layer.md. */

/* The BGnCNT shadow of a BG layer. */
union bg_cnt {
    u16 raw;
    struct {
        u8 priority:2;   // BGnCNT bits 0-1
        u8 charBase:2;   // bits 2-3
        u8:2;            // bits 4-5, unused by the hardware
        u8 mosaic:1;     // bit 6
        u8 colors256:1;  // bit 7
        u8 screenBase:5; // bits 8-12
        u8 wrap:1;       // bit 13, the affine wrap-around
        u8 size:2;       // bits 14-15
    } bits;
};

struct bg_scroll_layer {
    s32 x; // 0x00 - pixels
    s32 y; // 0x04
    /* 0x08-0x24: set from the level layer by BgLayerBase::SetSource */
    s32 maxX;           // 0x08 - widthPx - 240, the scroll limit
    s32 maxY;           // 0x0C - heightPx - 160
    s32 widthPx;        // 0x10
    s32 heightPx;       // 0x14
    s32 widthTiles;     // 0x18
    s32 heightTiles;    // 0x1C
    s32 scaleX;         // 0x20 - Q8 parallax factor
    s32 scaleY;         // 0x24
    u8 enabled;         // 0x28
    void *streamer;     // 0x2C - the tile-map ring buffer (BgStreamer)
    const void *vtable; // 0x30
    union bg_cnt cnt;   // 0x34 - BGnCNT shadow (the union pads to 4 bytes)
    vu16 *cntReg;       // 0x38 - &REG_BGnCNT
    s32 rowLo;          // 0x3C - resident tile rows rowLo..rowHi
    s32 rowHi;          // 0x40
    s32 colLo;          // 0x44 - resident tile columns colLo..colHi
    s32 colHi;          // 0x48
    u16 *screen;        // 0x4C - BG screen block (32x32 entries)
    void *tileData;     // 0x50 - tagged asset for the char block
    u16 hofs;           // 0x54
    u16 vofs;           // 0x56
    vu32 *ofsReg;       // 0x58 - &REG_BGnHOFS (written with VOFS as one word)
};

#endif /* __BG_SCROLL_LAYER_H__ */
