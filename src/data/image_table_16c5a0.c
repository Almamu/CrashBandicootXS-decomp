#include "core.h"
#include "menus.h"

/*
 * ROM 0x0816C5A0-0x0816C5F0. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern const u8 gLevelSelectPicture0Tiles[];
extern const u8 gLevelSelectPicture1Tiles[];
extern const u8 gLevelSelectPicture4Tiles[];
extern const u8 gLevelSelectPicture2Tiles[];
extern const u8 gLevelSelectPicture5Tiles[];
extern const u8 gLevelSelectPicture3Tiles[];
extern const u8 gLevelSelectPicture9Tiles[];
extern const u8 gLevelSelectPicture7Tiles[];
extern const u8 gLevelSelectPicture8Tiles[];
extern const u8 gLevelSelectPicture6Tiles[];
extern const u8 gLevelSelectPicture0Palette[];
extern const u8 gLevelSelectPicture1Palette[];
extern const u8 gLevelSelectPicture4Palette[];
extern const u8 gLevelSelectPicture2Palette[];
extern const u8 gLevelSelectPicture5Palette[];
extern const u8 gLevelSelectPicture3Palette[];
extern const u8 gLevelSelectPicture9Palette[];
extern const u8 gLevelSelectPicture7Palette[];
extern const u8 gLevelSelectPicture8Palette[];
extern const u8 gLevelSelectPicture6Palette[];

/* Ten {palette, tiles} tagged-asset pairs, indexed by image number:
 * UpdateZoomBg (zoom_bg.cpp) loads both through
 * LoadTaggedAsset. */
const struct image_pair gLevelSelectPictures[10] = {
    { gLevelSelectPicture0Palette, gLevelSelectPicture0Tiles },
    { gLevelSelectPicture1Palette, gLevelSelectPicture1Tiles },
    { gLevelSelectPicture2Palette, gLevelSelectPicture2Tiles },
    { gLevelSelectPicture3Palette, gLevelSelectPicture3Tiles },
    { gLevelSelectPicture4Palette, gLevelSelectPicture4Tiles },
    { gLevelSelectPicture5Palette, gLevelSelectPicture5Tiles },
    { gLevelSelectPicture6Palette, gLevelSelectPicture6Tiles },
    { gLevelSelectPicture7Palette, gLevelSelectPicture7Tiles },
    { gLevelSelectPicture8Palette, gLevelSelectPicture8Tiles },
    { gLevelSelectPicture9Palette, gLevelSelectPicture9Tiles },
};
