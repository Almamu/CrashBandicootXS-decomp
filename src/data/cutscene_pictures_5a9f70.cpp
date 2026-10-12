extern "C" {
#include "gba/types.h"
}

/*
 * ROM 0x085A9F70-0x0861BADC: the 24 cutscene pictures, each a 256-colour
 * palette directly followed by its Mode 4 bitmap (240x160 8bpp, LZ77).
 * A cutscene slide (src/data/cutscenes_16d1c8.cpp) points at the palette
 * and ShowSlidePicture (slideshow.cpp) finds the bitmap at +0x200, so each
 * bitmap must stay right after its palette.
 *
 * Both are built from graphics/intro/NN_xxxxxx_bitmap.png by graphics.mk:
 * the bitmap by grit -gb -gB8 -p!, then gbagfx LZ77, then tools/bin2c.py
 * --lz (docs/data.md, "Resources"); the palette from the PNG's own
 * palette by tools/jasc_pal.py png-to-gba, then tools/bin2c.py --u16.
 * Most entries have bit 15 set, which the hardware ignores; the PNG keeps
 * it in the red value's lowest bit (docs/graphics.md, "Palettes"). Each
 * palette starts on a word boundary (the zero fill after the previous
 * bitmap is the ROM's alignment padding).
 */

/* Picture 00: graphics/intro/00_5aa170_bitmap.png */
extern const u16 gCutscenePicture00[256] __attribute__((aligned(4))) = {
#include "intro/00_5aa170_bitmap.pal.inc"
};
extern const u8 gCutscenePicture00Bitmap[] = {
#include "intro/00_5aa170_bitmap.img.bin.lz.inc"
};

/* Picture 01: graphics/intro/01_5addd4_bitmap.png */
extern const u16 gCutscenePicture01[256] __attribute__((aligned(4))) = {
#include "intro/01_5addd4_bitmap.pal.inc"
};
extern const u8 gCutscenePicture01Bitmap[] = {
#include "intro/01_5addd4_bitmap.img.bin.lz.inc"
};

/* Picture 02: graphics/intro/02_5b36e4_bitmap.png */
extern const u16 gCutscenePicture02[256] __attribute__((aligned(4))) = {
#include "intro/02_5b36e4_bitmap.pal.inc"
};
extern const u8 gCutscenePicture02Bitmap[] = {
#include "intro/02_5b36e4_bitmap.img.bin.lz.inc"
};

/* Picture 03: graphics/intro/03_5b84bc_bitmap.png */
extern const u16 gCutscenePicture03[256] __attribute__((aligned(4))) = {
#include "intro/03_5b84bc_bitmap.pal.inc"
};
extern const u8 gCutscenePicture03Bitmap[] = {
#include "intro/03_5b84bc_bitmap.img.bin.lz.inc"
};

/* Picture 04: graphics/intro/04_5bc8e8_bitmap.png */
extern const u16 gCutscenePicture04[256] __attribute__((aligned(4))) = {
#include "intro/04_5bc8e8_bitmap.pal.inc"
};
extern const u8 gCutscenePicture04Bitmap[] = {
#include "intro/04_5bc8e8_bitmap.img.bin.lz.inc"
};

/* Picture 05: graphics/intro/05_5c1b38_bitmap.png */
extern const u16 gCutscenePicture05[256] __attribute__((aligned(4))) = {
#include "intro/05_5c1b38_bitmap.pal.inc"
};
extern const u8 gCutscenePicture05Bitmap[] = {
#include "intro/05_5c1b38_bitmap.img.bin.lz.inc"
};

/* Picture 06: graphics/intro/06_5c6950_bitmap.png */
extern const u16 gCutscenePicture06[256] __attribute__((aligned(4))) = {
#include "intro/06_5c6950_bitmap.pal.inc"
};
extern const u8 gCutscenePicture06Bitmap[] = {
#include "intro/06_5c6950_bitmap.img.bin.lz.inc"
};

/* Picture 07: graphics/intro/07_5cb360_bitmap.png */
extern const u16 gCutscenePicture07[256] __attribute__((aligned(4))) = {
#include "intro/07_5cb360_bitmap.pal.inc"
};
extern const u8 gCutscenePicture07Bitmap[] = {
#include "intro/07_5cb360_bitmap.img.bin.lz.inc"
};

/* Picture 08: graphics/intro/08_5d00cc_bitmap.png */
extern const u16 gCutscenePicture08[256] __attribute__((aligned(4))) = {
#include "intro/08_5d00cc_bitmap.pal.inc"
};
extern const u8 gCutscenePicture08Bitmap[] = {
#include "intro/08_5d00cc_bitmap.img.bin.lz.inc"
};

/* Picture 09: graphics/intro/09_5d6734_bitmap.png */
extern const u16 gCutscenePicture09[256] __attribute__((aligned(4))) = {
#include "intro/09_5d6734_bitmap.pal.inc"
};
extern const u8 gCutscenePicture09Bitmap[] = {
#include "intro/09_5d6734_bitmap.img.bin.lz.inc"
};

/* Picture 10: graphics/intro/10_5db968_bitmap.png */
extern const u16 gCutscenePicture10[256] __attribute__((aligned(4))) = {
#include "intro/10_5db968_bitmap.pal.inc"
};
extern const u8 gCutscenePicture10Bitmap[] = {
#include "intro/10_5db968_bitmap.img.bin.lz.inc"
};

/* Picture 11: graphics/intro/11_5dfcbc_bitmap.png */
extern const u16 gCutscenePicture11[256] __attribute__((aligned(4))) = {
#include "intro/11_5dfcbc_bitmap.pal.inc"
};
extern const u8 gCutscenePicture11Bitmap[] = {
#include "intro/11_5dfcbc_bitmap.img.bin.lz.inc"
};

/* Picture 12: graphics/intro/12_5e570c_bitmap.png */
extern const u16 gCutscenePicture12[256] __attribute__((aligned(4))) = {
#include "intro/12_5e570c_bitmap.pal.inc"
};
extern const u8 gCutscenePicture12Bitmap[] = {
#include "intro/12_5e570c_bitmap.img.bin.lz.inc"
};

/* Picture 13: graphics/intro/13_5eae44_bitmap.png */
extern const u16 gCutscenePicture13[256] __attribute__((aligned(4))) = {
#include "intro/13_5eae44_bitmap.pal.inc"
};
extern const u8 gCutscenePicture13Bitmap[] = {
#include "intro/13_5eae44_bitmap.img.bin.lz.inc"
};

/* Picture 14: graphics/intro/14_5f0568_bitmap.png */
extern const u16 gCutscenePicture14[256] __attribute__((aligned(4))) = {
#include "intro/14_5f0568_bitmap.pal.inc"
};
extern const u8 gCutscenePicture14Bitmap[] = {
#include "intro/14_5f0568_bitmap.img.bin.lz.inc"
};

/* Picture 15: graphics/intro/15_5f4ec4_bitmap.png */
extern const u16 gCutscenePicture15[256] __attribute__((aligned(4))) = {
#include "intro/15_5f4ec4_bitmap.pal.inc"
};
extern const u8 gCutscenePicture15Bitmap[] = {
#include "intro/15_5f4ec4_bitmap.img.bin.lz.inc"
};

/* Picture 16: graphics/intro/16_5fa3d4_bitmap.png */
extern const u16 gCutscenePicture16[256] __attribute__((aligned(4))) = {
#include "intro/16_5fa3d4_bitmap.pal.inc"
};
extern const u8 gCutscenePicture16Bitmap[] = {
#include "intro/16_5fa3d4_bitmap.img.bin.lz.inc"
};

/* Picture 17: graphics/intro/17_5ff97c_bitmap.png */
extern const u16 gCutscenePicture17[256] __attribute__((aligned(4))) = {
#include "intro/17_5ff97c_bitmap.pal.inc"
};
extern const u8 gCutscenePicture17Bitmap[] = {
#include "intro/17_5ff97c_bitmap.img.bin.lz.inc"
};

/* Picture 18: graphics/intro/18_603bb4_bitmap.png */
extern const u16 gCutscenePicture18[256] __attribute__((aligned(4))) = {
#include "intro/18_603bb4_bitmap.pal.inc"
};
extern const u8 gCutscenePicture18Bitmap[] = {
#include "intro/18_603bb4_bitmap.img.bin.lz.inc"
};

/* Picture 19: graphics/intro/19_608c50_bitmap.png */
extern const u16 gCutscenePicture19[256] __attribute__((aligned(4))) = {
#include "intro/19_608c50_bitmap.pal.inc"
};
extern const u8 gCutscenePicture19Bitmap[] = {
#include "intro/19_608c50_bitmap.img.bin.lz.inc"
};

/* Picture 20: graphics/intro/20_60d778_bitmap.png */
extern const u16 gCutscenePicture20[256] __attribute__((aligned(4))) = {
#include "intro/20_60d778_bitmap.pal.inc"
};
extern const u8 gCutscenePicture20Bitmap[] = {
#include "intro/20_60d778_bitmap.img.bin.lz.inc"
};

/* Picture 21: graphics/intro/21_611ddc_bitmap.png */
extern const u16 gCutscenePicture21[256] __attribute__((aligned(4))) = {
#include "intro/21_611ddc_bitmap.pal.inc"
};
extern const u8 gCutscenePicture21Bitmap[] = {
#include "intro/21_611ddc_bitmap.img.bin.lz.inc"
};

/* Picture 22: graphics/intro/22_616560_bitmap.png */
extern const u16 gCutscenePicture22[256] __attribute__((aligned(4))) = {
#include "intro/22_616560_bitmap.pal.inc"
};
extern const u8 gCutscenePicture22Bitmap[] = {
#include "intro/22_616560_bitmap.img.bin.lz.inc"
};

/* Picture 23: graphics/intro/23_61a154_bitmap.png */
extern const u16 gCutscenePicture23[256] __attribute__((aligned(4))) = {
#include "intro/23_61a154_bitmap.pal.inc"
};
extern const u8 gCutscenePicture23Bitmap[] = {
#include "intro/23_61a154_bitmap.img.bin.lz.inc"
};
