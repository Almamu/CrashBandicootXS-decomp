#ifndef __GRAPHICS_PACKAGE_H__
#define __GRAPHICS_PACKAGE_H__

#include "gba/types.h"

/* A 5-field {width, height, paletteAsset, tileAsset, mapAsset} asset
 * package descriptor - shared shape between `LoadGraphicsPackage`
 * (src/graphics/graphics_package_1e578.c) and the level/obj loaders
 * (`LoadBg2Background`/`LoadObjSpriteTiles`, src/graphics/level_graphics.c)
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

#endif /* __GRAPHICS_PACKAGE_H__ */
