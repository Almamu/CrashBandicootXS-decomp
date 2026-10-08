#ifndef __BITMAP_FONT_H__
#define __BITMAP_FONT_H__

/* `struct bitmap_font` is the game's bitmap-font text renderer
 * (formerly `struct icon_manager`), the C view of the C++ class Font
 * (include/font.hpp, src/text/), for the C files. There are two fonts,
 * gSmallFont (InitSmallFont) and gLargeFont (InitLargeFont), both built
 * in InitLevelState; menus, the credits and the dialogs draw all their
 * text with them. `record` is the font's vtable (gFontVtable/
 * gSmallFontVtable/gLargeFontVtable): slot 1 the destructor (DestroyFont/
 * DestroySmallFont/DestroyLargeFont), then FontMeasureText,
 * FontMeasureChars, FontDrawText, FontDrawChars, FontDrawGlyph,
 * FontPutChar and FontUploadTiles. */

struct vtable_slot;

/* A single glyph's draw metrics - `bitmap_font.glyphRecords` is an
 * array of these, 12 bytes apart, indexed by `bitmap_font.charLookup`.
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

/* The bitmap font (see the top of this file). gLargeFont/gSmallFont
 * are its two instances.
 *
 * The leading `unused_00`/`unused_10c` regions and part of `unused_118`
 * were opaque when this struct was first written; GitHub issue #46's chunk
 * (src/hud/hud_slide.cpp) reads and writes them directly and
 * fills in the real shape below. */
struct bitmap_font {
    /* A 6-byte OAM-shaped draw-request scratch buffer, rebuilt fresh by
     * `FontDrawGlyph` for every glyph drawn (byte 0/1, halfword at 2,
     * halfword at 4) then handed to `AddOamEntry`; zeroed 8 bytes at a
     * time (a fixed-source `CpuSet` fill) by the widget
     * constructors. Bytes 6-7 are never written by anything in this
     * chunk. */
    u8 oam_scratch[8];
    /* Reverse char-byte -> glyph-index lookup table, built once by the
     * widget constructors (`InitSmallFont`/`InitLargeFont`)
     * from a small font-glyph-order table: `charLookup[c]` is the index
     * `i` (1-0x4F) such that the font table's `i`th byte equals `c`, or
     * 0 if `c` isn't in the font (or equals the font table's own
     * leading count byte). */
    u8 charLookup[0x100];
    /* Base value combined with `glyphIndex * glyphTileStride` (masked to 10
     * bits) into the OAM-scratch draw request's halfword at +4 -
     * plausibly a base tile/palette selector for the glyph sheet. */
    u32 tileBase;
    /* `struct icon_glyph_metrics` array, 12 bytes/entry, indexed by
     * `charLookup[c]`. */
    struct icon_glyph_metrics *glyphRecords;
    u32 posX;
    u32 posY;
    /* Left-margin X: `posX` is reset to this on a newline character. */
    u32 marginX;
    /* Line height: added to `posY` on a newline character; also used
     * as a plain divisor by `FontHeightToLines`/`DrawWrappedTextInBox`
     * (src/text/text_box.c). */
    s32 lineHeight;
    /* Advance width contributed by a literal space character, in place
     * of a `glyphRecords` lookup. */
    s32 spaceWidth;
    /* Per-glyph stride multiplier feeding `tileBase`'s combine (see
     * above) - `tileBase + glyphIndex * glyphTileStride`. */
    s32 glyphTileStride;
    /* Pointer to a small per-widget data table (`gSmallFontTiles`-
     * family) `FontUploadTiles` reads its first word from and shifts
     * right 13 into `tileCount`, then hands straight to `LoadTaggedAsset`
     * as the asset to upload. */
    void *tiles;
    /* Copied byte-for-byte from gSmallFont's copy into
     * gLargeFont's copy by InitLanguageSelectGraphics - meaning not understood
     * yet. */
    u32 tileCount;
    const struct vtable_slot *record; // 0x130 - Font's vtable (vtable.h)
};

COMPILE_TIME_ASSERT(bitmap_font_h, sizeof(struct bitmap_font) == 0x134);

#endif /* __BITMAP_FONT_H__ */
