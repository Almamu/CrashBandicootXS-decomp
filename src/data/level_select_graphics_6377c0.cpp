extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x086377C0-0x0863D4C0: the level-select cursor's zoom tiles (OAM
 * picks their palette at run time, so the PNG carries a grayscale
 * placeholder one), the ten level-select pictures (8bpp) and their
 * palettes, in ROM order.
 * Every asset is an LZ77 stream (gbagfx) that starts on a word boundary,
 * hence aligned(4): the 0-3 zero bytes after a stream are the next one's
 * alignment. tools/bin2c.py --lz writes the stream's bytes (graphics.mk).
 * Linked in ROM order between data/data.s sections by ldscript.txt, see
 * docs/data.md.
 */

/* 4bpp tiles of the level-select cursor's zoom: graphics/tileset1/00_6377c0_tiles.png. */
extern const u8 gLevelSelectCursorZoomTiles[] __attribute__((aligned(4))) = {
#include "tileset1/00_6377c0_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 0 (a circular level icon vignette,
 * drawn with gLevelSelectPicture0Palette):
 * graphics/tileset1/01_637a70_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture0Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/01_637a70_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 1 (a circular level icon vignette,
 * drawn with gLevelSelectPicture1Palette):
 * graphics/tileset1/02_6382c8_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture1Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/02_6382c8_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 4 (a circular level icon vignette,
 * drawn with gLevelSelectPicture4Palette):
 * graphics/tileset1/03_638d68_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture4Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/03_638d68_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 2 (a circular level icon vignette,
 * drawn with gLevelSelectPicture2Palette):
 * graphics/tileset1/04_63945c_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture2Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/04_63945c_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 5 (a circular level icon vignette,
 * drawn with gLevelSelectPicture5Palette):
 * graphics/tileset1/05_639d18_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture5Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/05_639d18_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 3 (a circular level icon vignette,
 * drawn with gLevelSelectPicture3Palette):
 * graphics/tileset1/06_63a60c_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture3Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/06_63a60c_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 9 (a circular level icon vignette,
 * drawn with gLevelSelectPicture9Palette):
 * graphics/tileset1/07_63ad90_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture9Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/07_63ad90_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 7 (a circular level icon vignette,
 * drawn with gLevelSelectPicture7Palette):
 * graphics/tileset1/08_63b668_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture7Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/08_63b668_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 8 (a circular level icon vignette,
 * drawn with gLevelSelectPicture8Palette):
 * graphics/tileset1/09_63bdd4_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture8Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/09_63bdd4_8bpp_tiles.img.bin.lz.inc"
};

/*
 * 8bpp tiles of level-select picture 6 (a circular level icon vignette,
 * drawn with gLevelSelectPicture6Palette):
 * graphics/tileset1/10_63c5e4_8bpp_tiles.png.
 */
extern const u8 gLevelSelectPicture6Tiles[] __attribute__((aligned(4))) = {
#include "tileset1/10_63c5e4_8bpp_tiles.img.bin.lz.inc"
};

/* Palette of level-select picture 0, 256 colours: graphics/tileset1/11_63cf98.pal. */
extern const u8 gLevelSelectPicture0Palette[] __attribute__((aligned(4))) = {
#include "tileset1/11_63cf98.gbapal.lz.inc"
};

/* Palette of level-select picture 1, 256 colours: graphics/tileset1/12_63d01c.pal. */
extern const u8 gLevelSelectPicture1Palette[] __attribute__((aligned(4))) = {
#include "tileset1/12_63d01c.gbapal.lz.inc"
};

/* Palette of level-select picture 4, 256 colours: graphics/tileset1/13_63d0a0.pal. */
extern const u8 gLevelSelectPicture4Palette[] __attribute__((aligned(4))) = {
#include "tileset1/13_63d0a0.gbapal.lz.inc"
};

/* Palette of level-select picture 2, 256 colours: graphics/tileset1/14_63d124.pal. */
extern const u8 gLevelSelectPicture2Palette[] __attribute__((aligned(4))) = {
#include "tileset1/14_63d124.gbapal.lz.inc"
};

/* Palette of level-select picture 5, 256 colours: graphics/tileset1/15_63d1a8.pal. */
extern const u8 gLevelSelectPicture5Palette[] __attribute__((aligned(4))) = {
#include "tileset1/15_63d1a8.gbapal.lz.inc"
};

/* Palette of level-select picture 3, 256 colours: graphics/tileset1/16_63d22c.pal. */
extern const u8 gLevelSelectPicture3Palette[] __attribute__((aligned(4))) = {
#include "tileset1/16_63d22c.gbapal.lz.inc"
};

/* Palette of level-select picture 9, 256 colours: graphics/tileset1/17_63d2b0.pal. */
extern const u8 gLevelSelectPicture9Palette[] __attribute__((aligned(4))) = {
#include "tileset1/17_63d2b0.gbapal.lz.inc"
};

/* Palette of level-select picture 7, 256 colours: graphics/tileset1/18_63d334.pal. */
extern const u8 gLevelSelectPicture7Palette[] __attribute__((aligned(4))) = {
#include "tileset1/18_63d334.gbapal.lz.inc"
};

/* Palette of level-select picture 8, 256 colours: graphics/tileset1/19_63d3b8.pal. */
extern const u8 gLevelSelectPicture8Palette[] __attribute__((aligned(4))) = {
#include "tileset1/19_63d3b8.gbapal.lz.inc"
};

/* Palette of level-select picture 6, 256 colours: graphics/tileset1/20_63d43c.pal. */
extern const u8 gLevelSelectPicture6Palette[] __attribute__((aligned(4))) = {
#include "tileset1/20_63d43c.gbapal.lz.inc"
};
