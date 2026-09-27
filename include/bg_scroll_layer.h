#ifndef __BG_SCROLL_LAYER_H__
#define __BG_SCROLL_LAYER_H__

/* The BG-scroll layer (0x5C bytes, constructor `sub_8025D74` in
 * game_loop15.c, base method table `gStaticData_087E4C14`), and its
 * tile-slot-pooled subclass used for BG layer 0 (0x60 bytes, constructor
 * `sub_8026448` in tile_slot_pool.c, method table `gStaticData_087E4C64`).
 * The level-layers singleton (level_layers.c) owns one of each kind per
 * hardware BG. See docs/matching/issue-42-bg-scroll-layer.md.
 *
 * The layer keeps a 32x32-entry window of the level's tile map (a
 * `sub_8024960`-family ring-buffer streamer at `+0x2C`, game_loop57.c)
 * resident in its BG screen block: `+0x3C..+0x40` is the resident tile
 * row range and `+0x44..+0x48` the column range. Methods draw/release
 * one row or column at a time as the window moves. */

struct bg_scroll_layer;

/* A method-table entry: `this` adjustment plus a function pointer, called
 * through the `sub_803AD7C`/`sub_803AD80` `_call_via_rN` veneers. */
struct bg_layer_method
{
    s16 thisOffset; // 0x0
    u8 unk_2[2];    // 0x2
    void *fn;       // 0x4
};

struct bg_layer_vtable
{
    u8 unk_00[8];                     // 0x00
    struct bg_layer_method destroy;   // 0x08 - sub_80261B8 / sub_8026418
    struct bg_layer_method reset;     // 0x10 - sub_802608C / sub_80263DC
    struct bg_layer_method method_18; // 0x18 - sub_8025E98
    struct bg_layer_method method_20; // 0x20 - sub_8024DCC / sub_8026250
    struct bg_layer_method loadTiles; // 0x28 - sub_80260B4 / sub_80263F8
    struct bg_layer_method drawRow;   // 0x30 - sub_8025FC8 / sub_8026368
    struct bg_layer_method drawCol;   // 0x38 - sub_8025F3C / sub_80261CC
    struct bg_layer_method clipCols;  // 0x40 - sub_8025E70 / sub_80262E8
    struct bg_layer_method clipRows;  // 0x48 - sub_8025E84 / sub_8026328
};

struct bg_scroll_layer
{
    s32 x;                          // 0x00 - pixels
    s32 y;                          // 0x04
    u8 unk_08[0x20];                // 0x08 - see sub_8024EB4 (game_loop3.c)
    u8 enabled;                     // 0x28
    u8 unk_29[3];                   // 0x29
    void *streamer;                 // 0x2C - tile-map ring-buffer streamer
    struct bg_layer_vtable *vtable; // 0x30
    union
    {
        u16 raw;
        struct
        {
            u8 priority:2;   // BGnCNT bits 0-1
            u8 charBase:2;   // bits 2-3
            u8 unk4:3;       // bits 4-6 (bit 6 = mosaic)
            u8 colors256:1;  // bit 7
            u8 screenBase:5; // bits 8-12
            u8 unk13:3;      // bits 13-15
        } bits;
    } cnt;                          // 0x34 - BGnCNT shadow (the union pads to 4 bytes)
    vu16 *cntReg;                   // 0x38 - &REG_BGnCNT
    s32 rowLo;                      // 0x3C - resident tile rows rowLo..rowHi
    s32 rowHi;                      // 0x40
    s32 colLo;                      // 0x44 - resident tile columns colLo..colHi
    s32 colHi;                      // 0x48
    u16 *screen;                    // 0x4C - BG screen block (32x32 entries)
    void *tileData;                 // 0x50 - tagged asset for the char block
    u16 hofs;                       // 0x54
    u16 vofs;                       // 0x56
    vu32 *ofsReg;                   // 0x58 - &REG_BGnHOFS (written with VOFS as one word)
};

struct tile_slot_pool;

/* BG layer 0: a BG-scroll layer whose tiles go through a VRAM tile-slot
 * pool (tile_slot_pool.c). */
struct pooled_bg_layer
{
    struct bg_scroll_layer base;  // 0x00
    struct tile_slot_pool *pool;  // 0x5C
};

#endif /* __BG_SCROLL_LAYER_H__ */
