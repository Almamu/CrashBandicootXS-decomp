#ifndef __BG_SCROLL_LAYER_H__
#define __BG_SCROLL_LAYER_H__

/* The BGnCNT shadow of the level's BG layers (BgLayer's `cnt`,
 * include/bg_layer.hpp). The layers are C++ classes with no C view: this
 * header's `struct bg_scroll_layer` went with its last reader (#754). See
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

#endif /* __BG_SCROLL_LAYER_H__ */
