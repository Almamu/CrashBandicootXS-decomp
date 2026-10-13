extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x086377C0-0x0863D4C0: the level-select cursor's zoom tiles (OAM
 * picks their palette at run time; the PNG carries the cursor part's,
 * docs/graphics.md "Run-time palettes"), the ten level-select pictures
 * (8bpp, one per level_info.theme, which ZoomBg::SetPicture indexes
 * gLevelSelectPictures by) and their palettes (each the palette of its
 * picture's PNG), in ROM order.
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */

/* 4bpp tiles of the level-select cursor's zoom: graphics/level_select/cursor_zoom.png. */
extern const u8 gLevelSelectCursorZoomTiles[] __attribute__((aligned(4))) = {
#include "level_select/cursor_zoom.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 0 (theme 0's, snow; drawn with
 * gLevelSelectPicture0Palette):
 * graphics/level_select/pictures/snow.png.
 */
extern const u8 gLevelSelectPicture0Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/snow.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 1 (theme 1's, jungle; drawn with
 * gLevelSelectPicture1Palette):
 * graphics/level_select/pictures/jungle.png.
 */
extern const u8 gLevelSelectPicture1Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/jungle.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 4 (theme 4's, sky; drawn with
 * gLevelSelectPicture4Palette):
 * graphics/level_select/pictures/sky.png.
 */
extern const u8 gLevelSelectPicture4Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/sky.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 2 (theme 2's, sewer; drawn with
 * gLevelSelectPicture2Palette):
 * graphics/level_select/pictures/sewer.png.
 */
extern const u8 gLevelSelectPicture2Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/sewer.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 5 (theme 5's, space; drawn with
 * gLevelSelectPicture5Palette):
 * graphics/level_select/pictures/space.png.
 */
extern const u8 gLevelSelectPicture5Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/space.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 3 (theme 3's, underwater; drawn with
 * gLevelSelectPicture3Palette):
 * graphics/level_select/pictures/underwater.png.
 */
extern const u8 gLevelSelectPicture3Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/underwater.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 9 (theme 9's, Neo Cortex; drawn with
 * gLevelSelectPicture9Palette):
 * graphics/level_select/pictures/neo_cortex.png.
 */
extern const u8 gLevelSelectPicture9Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/neo_cortex.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 7 (theme 7's, Dingodile; drawn with
 * gLevelSelectPicture7Palette):
 * graphics/level_select/pictures/dingodile.png.
 */
extern const u8 gLevelSelectPicture7Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/dingodile.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 8 (theme 8's, N. Gin; drawn with
 * gLevelSelectPicture8Palette):
 * graphics/level_select/pictures/n_gin.png.
 */
extern const u8 gLevelSelectPicture8Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/n_gin.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 6 (theme 6's, Tiny; drawn with
 * gLevelSelectPicture6Palette):
 * graphics/level_select/pictures/tiny.png.
 */
extern const u8 gLevelSelectPicture6Tiles[] __attribute__((aligned(4))) = {
#include "level_select/pictures/tiny.img.bin.lz.inc"
};

/*
 * Palette of level-select picture 0, 256 colours: the palette of
 * graphics/level_select/pictures/snow.png.
 */
extern const u8 gLevelSelectPicture0Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/snow.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 1, 256 colours: the palette of
 * graphics/level_select/pictures/jungle.png.
 */
extern const u8 gLevelSelectPicture1Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/jungle.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 4, 256 colours: the palette of
 * graphics/level_select/pictures/sky.png.
 */
extern const u8 gLevelSelectPicture4Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/sky.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 2, 256 colours: the palette of
 * graphics/level_select/pictures/sewer.png.
 */
extern const u8 gLevelSelectPicture2Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/sewer.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 5, 256 colours: the palette of
 * graphics/level_select/pictures/space.png.
 */
extern const u8 gLevelSelectPicture5Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/space.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 3, 256 colours: the palette of
 * graphics/level_select/pictures/underwater.png.
 */
extern const u8 gLevelSelectPicture3Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/underwater.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 9, 256 colours: the palette of
 * graphics/level_select/pictures/neo_cortex.png.
 */
extern const u8 gLevelSelectPicture9Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/neo_cortex.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 7, 256 colours: the palette of
 * graphics/level_select/pictures/dingodile.png.
 */
extern const u8 gLevelSelectPicture7Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/dingodile.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 8, 256 colours: the palette of
 * graphics/level_select/pictures/n_gin.png.
 */
extern const u8 gLevelSelectPicture8Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/n_gin.gbapal.lz.inc"
};

/*
 * Palette of level-select picture 6, 256 colours: the palette of
 * graphics/level_select/pictures/tiny.png.
 */
extern const u8 gLevelSelectPicture6Palette[] __attribute__((aligned(4))) = {
#include "level_select/pictures/tiny.gbapal.lz.inc"
};
