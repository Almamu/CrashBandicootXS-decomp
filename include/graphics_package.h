#ifndef __GRAPHICS_PACKAGE_H__
#define __GRAPHICS_PACKAGE_H__

#include "gba/types.h"

/* A 5-field {width, height, paletteAsset, tileAsset, mapAsset} asset
 * package descriptor - shared shape between `LoadGraphicsPackage`
 * (src/gfx/graphics_package.cpp) and the level/obj loaders
 * (`LoadTitleScreenBg`/`LoadTitleScreenObjTiles`, src/frontend/title_screen_init.cpp)
 * that first surfaced it as `struct bg_package`. Moved here (rather than
 * duplicated in each file) per docs/workflow.md step 7's "check whether a
 * struct for the same object already exists elsewhere first" rule. */
struct bg_package {
    u32 width;
    u32 height;
    void *paletteAsset;
    void *tileAsset;
    void *mapAsset;
};

/* REG_BGnCNT as bitfields. As a stack variable it is 4 bytes (agbcc pads
 * every union to a word); level_select.hpp's `ZoomBg` holds
 * a packed 2-byte copy. */
union bgcnt {
    u16 raw;
    struct {
        u16 priority:2;
        u16 charBase:2;
        u16:2; // bits 4-5, unused by the hardware
        u16 mosaic:1;
        u16 colorMode:1;
        u16 screenBase:5;
        u16 wrap:1;
        u16 size:2;
    } bits;
};

/* The BG setup buffer callers fill with InitBgSetup before calling
 * LoadGraphicsPackage; GetBgSetupControl reads the control value back for
 * REG_BGnCNT. */
struct bg_setup {
    u32 charBlock;    // 0x00
    u32 screenBlock;  // 0x04
    u32 paletteBank;  // 0x08
    union bgcnt ctrl; // 0x0C - BGnCNT
};

#endif /* __GRAPHICS_PACKAGE_H__ */
