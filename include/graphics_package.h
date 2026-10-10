#ifndef __GRAPHICS_PACKAGE_H__
#define __GRAPHICS_PACKAGE_H__

#include "gba/types.h"

/* A 5-field {width, height, paletteAsset, tileAsset, mapAsset} asset
 * package descriptor, read by `LoadGraphicsPackage`
 * (src/gfx/graphics_package.cpp) and the level/obj loaders
 * (`LoadTitleScreenBg`/`LoadTitleScreenObjTiles`, src/frontend/title_screen.cpp).
 * The src/data tables define the packages, so C needs it. */
struct bg_package {
    u32 width;
    u32 height;
    void *paletteAsset;
    void *tileAsset;
    void *mapAsset;
};

/* The BG setup (BGnCNT's `ctrl` and the blocks it names) is the C++ class
 * BgSetup, include/graphics_package.hpp, with REG_BGnCNT's bitfields
 * (union bgcnt). */

#endif /* __GRAPHICS_PACKAGE_H__ */
