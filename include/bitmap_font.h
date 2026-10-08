#ifndef __BITMAP_FONT_H__
#define __BITMAP_FONT_H__

/* The glyph metrics of the game's bitmap fonts (class Font,
 * include/font.hpp, src/text/; formerly `struct icon_manager`). Font has
 * no C view: `struct bitmap_font` went with its last C reader (#754). */

/* A single glyph's draw metrics - Font's `glyphRecords` is an array of
 * these, 12 bytes apart, indexed by its `charLookup`.
 * Established by GitHub issue #46's chunk (`FontDrawGlyph`/`FontMeasureChars`/
 * `FontMeasureText`, src/hud/hud_slide.cpp): `width` is the glyph's
 * horizontal advance (added to `posX` after each draw, and what
 * `FontMeasureText`/`FontMeasureChars` sum to measure a run of text); `shape`
 * feeds a small (2-bit, `<<6` into a byte) shape/size selector;
 * `yOffset` is a tile-row contribution combined with `posY`'s low byte
 * into the OAM-scratch draw request `FontDrawGlyph` builds. */
struct icon_glyph_metrics {
    s32 width;
    s32 shape;
    u8 yOffset;
    u8 unused_9[3];
};

COMPILE_TIME_ASSERT(bitmap_font_h, sizeof(struct icon_glyph_metrics) == 0xC);

#endif /* __BITMAP_FONT_H__ */
