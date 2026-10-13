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
 * Both are built from graphics/cutscenes/NN_<what>.png by graphics.mk:
 * the bitmap by grit -gb -gB8 -p!, then gbagfx LZ77, then tools/bin2c.py
 * --lz (docs/data.md, "Resources"); the palette from the PNG's own
 * palette by tools/jasc_pal.py png-to-gba, then tools/bin2c.py --u16.
 * Most entries have bit 15 set, which the hardware ignores; the PNG keeps
 * it in the red value's lowest bit (docs/graphics.md, "Palettes"). Each
 * palette starts on a word boundary (the zero fill after the previous
 * bitmap is the ROM's alignment padding).
 */

/* Picture 00: graphics/cutscenes/00_space_station.png */
extern const u16 gCutscenePicture00[256] __attribute__((aligned(4))) = {
#include "cutscenes/00_space_station.pal.inc"
};
extern const u8 gCutscenePicture00Bitmap[] = {
#include "cutscenes/00_space_station.img.bin.lz.inc"
};

/* Picture 01: graphics/cutscenes/01_uka_uka_and_cortex.png */
extern const u16 gCutscenePicture01[256] __attribute__((aligned(4))) = {
#include "cutscenes/01_uka_uka_and_cortex.pal.inc"
};
extern const u8 gCutscenePicture01Bitmap[] = {
#include "cutscenes/01_uka_uka_and_cortex.img.bin.lz.inc"
};

/* Picture 02: graphics/cutscenes/02_cortex.png */
extern const u16 gCutscenePicture02[256] __attribute__((aligned(4))) = {
#include "cutscenes/02_cortex.pal.inc"
};
extern const u8 gCutscenePicture02Bitmap[] = {
#include "cutscenes/02_cortex.img.bin.lz.inc"
};

/* Picture 03: graphics/cutscenes/03_cortex_hologram.png */
extern const u16 gCutscenePicture03[256] __attribute__((aligned(4))) = {
#include "cutscenes/03_cortex_hologram.pal.inc"
};
extern const u8 gCutscenePicture03Bitmap[] = {
#include "cutscenes/03_cortex_hologram.img.bin.lz.inc"
};

/* Picture 04: graphics/cutscenes/04_minimizer_console.png */
extern const u16 gCutscenePicture04[256] __attribute__((aligned(4))) = {
#include "cutscenes/04_minimizer_console.pal.inc"
};
extern const u8 gCutscenePicture04Bitmap[] = {
#include "cutscenes/04_minimizer_console.img.bin.lz.inc"
};

/* Picture 05: graphics/cutscenes/05_minimizer_beam.png */
extern const u16 gCutscenePicture05[256] __attribute__((aligned(4))) = {
#include "cutscenes/05_minimizer_beam.pal.inc"
};
extern const u8 gCutscenePicture05Bitmap[] = {
#include "cutscenes/05_minimizer_beam.img.bin.lz.inc"
};

/* Picture 06: graphics/cutscenes/06_earth_shrinking.png */
extern const u16 gCutscenePicture06[256] __attribute__((aligned(4))) = {
#include "cutscenes/06_earth_shrinking.pal.inc"
};
extern const u8 gCutscenePicture06Bitmap[] = {
#include "cutscenes/06_earth_shrinking.img.bin.lz.inc"
};

/* Picture 07: graphics/cutscenes/07_cortex_on_monitors.png */
extern const u16 gCutscenePicture07[256] __attribute__((aligned(4))) = {
#include "cutscenes/07_cortex_on_monitors.pal.inc"
};
extern const u8 gCutscenePicture07Bitmap[] = {
#include "cutscenes/07_cortex_on_monitors.img.bin.lz.inc"
};

/* Picture 08: graphics/cutscenes/08_aku_aku_crash_and_coco.png */
extern const u16 gCutscenePicture08[256] __attribute__((aligned(4))) = {
#include "cutscenes/08_aku_aku_crash_and_coco.pal.inc"
};
extern const u8 gCutscenePicture08Bitmap[] = {
#include "cutscenes/08_aku_aku_crash_and_coco.img.bin.lz.inc"
};

/* Picture 09: graphics/cutscenes/09_cortex_holding_earth.png */
extern const u16 gCutscenePicture09[256] __attribute__((aligned(4))) = {
#include "cutscenes/09_cortex_holding_earth.pal.inc"
};
extern const u8 gCutscenePicture09Bitmap[] = {
#include "cutscenes/09_cortex_holding_earth.img.bin.lz.inc"
};

/* Picture 10: graphics/cutscenes/10_aku_aku.png */
extern const u16 gCutscenePicture10[256] __attribute__((aligned(4))) = {
#include "cutscenes/10_aku_aku.pal.inc"
};
extern const u8 gCutscenePicture10Bitmap[] = {
#include "cutscenes/10_aku_aku.img.bin.lz.inc"
};

/* Picture 11: graphics/cutscenes/11_cortex_on_screen.png */
extern const u16 gCutscenePicture11[256] __attribute__((aligned(4))) = {
#include "cutscenes/11_cortex_on_screen.pal.inc"
};
extern const u8 gCutscenePicture11Bitmap[] = {
#include "cutscenes/11_cortex_on_screen.img.bin.lz.inc"
};

/* Picture 12: graphics/cutscenes/12_coco_on_screen.png */
extern const u16 gCutscenePicture12[256] __attribute__((aligned(4))) = {
#include "cutscenes/12_coco_on_screen.pal.inc"
};
extern const u8 gCutscenePicture12Bitmap[] = {
#include "cutscenes/12_coco_on_screen.img.bin.lz.inc"
};

/* Picture 13: graphics/cutscenes/13_minimizer_overload.png */
extern const u16 gCutscenePicture13[256] __attribute__((aligned(4))) = {
#include "cutscenes/13_minimizer_overload.pal.inc"
};
extern const u8 gCutscenePicture13Bitmap[] = {
#include "cutscenes/13_minimizer_overload.img.bin.lz.inc"
};

/* Picture 14: graphics/cutscenes/14_bosses_shocked.png */
extern const u16 gCutscenePicture14[256] __attribute__((aligned(4))) = {
#include "cutscenes/14_bosses_shocked.pal.inc"
};
extern const u8 gCutscenePicture14Bitmap[] = {
#include "cutscenes/14_bosses_shocked.img.bin.lz.inc"
};

/* Picture 15: graphics/cutscenes/15_monster.png */
extern const u16 gCutscenePicture15[256] __attribute__((aligned(4))) = {
#include "cutscenes/15_monster.pal.inc"
};
extern const u8 gCutscenePicture15Bitmap[] = {
#include "cutscenes/15_monster.img.bin.lz.inc"
};

/* Picture 16: graphics/cutscenes/16_monster_and_crash.png */
extern const u16 gCutscenePicture16[256] __attribute__((aligned(4))) = {
#include "cutscenes/16_monster_and_crash.pal.inc"
};
extern const u8 gCutscenePicture16Bitmap[] = {
#include "cutscenes/16_monster_and_crash.img.bin.lz.inc"
};

/* Picture 17: graphics/cutscenes/17_crash_flying_to_station.png */
extern const u16 gCutscenePicture17[256] __attribute__((aligned(4))) = {
#include "cutscenes/17_crash_flying_to_station.pal.inc"
};
extern const u8 gCutscenePicture17Bitmap[] = {
#include "cutscenes/17_crash_flying_to_station.img.bin.lz.inc"
};

/* Picture 18: graphics/cutscenes/18_coco_at_enlarger.png */
extern const u16 gCutscenePicture18[256] __attribute__((aligned(4))) = {
#include "cutscenes/18_coco_at_enlarger.pal.inc"
};
extern const u8 gCutscenePicture18Bitmap[] = {
#include "cutscenes/18_coco_at_enlarger.img.bin.lz.inc"
};

/* Picture 19: graphics/cutscenes/19_earth_growing.png */
extern const u16 gCutscenePicture19[256] __attribute__((aligned(4))) = {
#include "cutscenes/19_earth_growing.pal.inc"
};
extern const u8 gCutscenePicture19Bitmap[] = {
#include "cutscenes/19_earth_growing.img.bin.lz.inc"
};

/* Picture 20: graphics/cutscenes/20_earth_beam_ring.png */
extern const u16 gCutscenePicture20[256] __attribute__((aligned(4))) = {
#include "cutscenes/20_earth_beam_ring.pal.inc"
};
extern const u8 gCutscenePicture20Bitmap[] = {
#include "cutscenes/20_earth_beam_ring.img.bin.lz.inc"
};

/* Picture 21: graphics/cutscenes/21_station_exploding.png */
extern const u16 gCutscenePicture21[256] __attribute__((aligned(4))) = {
#include "cutscenes/21_station_exploding.pal.inc"
};
extern const u8 gCutscenePicture21Bitmap[] = {
#include "cutscenes/21_station_exploding.img.bin.lz.inc"
};

/* Picture 22: graphics/cutscenes/22_reversed_beam.png */
extern const u16 gCutscenePicture22[256] __attribute__((aligned(4))) = {
#include "cutscenes/22_reversed_beam.pal.inc"
};
extern const u8 gCutscenePicture22Bitmap[] = {
#include "cutscenes/22_reversed_beam.img.bin.lz.inc"
};

/* Picture 23: graphics/cutscenes/23_legal_notice.png */
extern const u16 gCutscenePicture23[256] __attribute__((aligned(4))) = {
#include "cutscenes/23_legal_notice.pal.inc"
};
extern const u8 gCutscenePicture23Bitmap[] = {
#include "cutscenes/23_legal_notice.img.bin.lz.inc"
};
