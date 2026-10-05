#ifndef __BITMAP_FONT_H__
#define __BITMAP_FONT_H__

/* `struct bitmap_font` is the game's bitmap-font text renderer
 * (formerly `struct icon_manager`). There are two fonts, gSmallFont (InitSmallFont)
 * and gLargeFont (InitLargeFont), both built in InitLevelState; menus,
 * the credits and the dialogs draw all their text with them. Calls go
 * through the font's vtable (`record`, gFontVtable/gSmallFontVtable/
 * gLargeFontVtable): `record->slots[n]` is vtable slot n + 2, so
 * slots[0] FontMeasureText, [1] FontMeasureChars, [2] FontDrawText,
 * [3] FontDrawChars, [4] FontDrawGlyph, [5] FontPutChar and
 * [6] FontUploadTiles; `destroy` is slot 1 (DestroyFont/
 * DestroySmallFont/DestroyLargeFont). */

/* One (OAM-slot-offset, pointer) pair, as used by _call_via_r2/
 * _call_via_r3 to draw a single OAM entry. `struct icon_record` is an
 * array of these, 8 bytes apart, starting at offset 0x10 - DrawPowerDialog
 * (src/graphics/oam_count.c, parked) uses slots 0 and 2 (a wide icon spanning
 * two OAM entries); sub_8000EE4 (src/graphics/text_layout.c, parked) uses slots
 * 1, 3, and 5 (per-glyph and newline-marker OAM entries). */
struct icon_slot {
    s16 offset;
    u8 unused_2[2];
    void *ptr;
};

struct icon_record {
    u8 unused_00[8];
    /* The object's destructor entry (gcc 2.x {this-adjust, fn} method
     * record): DestroyLevelState tears both icon managers down by calling it
     * with the "delete" flags 3. */
    struct icon_slot destroy;
    /* A 7th slot (index 6, offset 0x40) is read by InitLanguageSelectGraphics - extends
     * the 6-slot record DrawPowerDialog/sub_8000EE4 already established. */
    struct icon_slot slots[7];
};

/* A single glyph's draw metrics - `bitmap_font.glyphRecords` is an
 * array of these, 12 bytes apart, indexed by `bitmap_font.charLookup`.
 * Established by GitHub issue #46's chunk (`FontDrawGlyph`/`FontMeasureChars`/
 * `FontMeasureText`, src/graphics/hud_icon_widget.c): `width` is the glyph's
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

COMPILE_TIME_ASSERT(sizeof(struct icon_glyph_metrics) == 0xC);

/* The bitmap font (see the top of this file). gLargeFont/gSmallFont
 * are its two instances; sub_8000EE4 takes one as its render-target
 * object.
 *
 * The leading `unused_00`/`unused_10c` regions and part of `unused_118`
 * were opaque when this struct was first written (oam_count.c/
 * text_layout.c, both still not byte-matched); GitHub issue #46's chunk
 * (src/graphics/hud_icon_widget.c) reads and writes them directly and
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
     * as a plain divisor by `FontHeightToLines`/`sub_8001214`
     * (src/util/word_util.c). */
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
    struct icon_record *record;
};

COMPILE_TIME_ASSERT(sizeof(struct bitmap_font) == 0x134);

#endif /* __BITMAP_FONT_H__ */
