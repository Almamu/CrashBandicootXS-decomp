#ifndef __ICON_MANAGER_H__
#define __ICON_MANAGER_H__

/* One (OAM-slot-offset, pointer) pair, as used by sub_803AD80/
 * sub_803AD84 to draw a single OAM entry. `struct icon_record` is an
 * array of these, 8 bytes apart, starting at offset 0x10 - sub_8006600
 * (src/graphics/oam_count.c, parked) uses slots 0 and 2 (a wide icon spanning
 * two OAM entries); sub_8000EE4 (src/graphics/text_layout.c, parked) uses slots
 * 1, 3, and 5 (per-glyph and newline-marker OAM entries). */
struct icon_slot {
    s16 offset;
    u8 unused_2[2];
    void *ptr;
};

struct icon_record {
    u8 unused_00[0x10];
    /* A 7th slot (index 6, offset 0x40) is read by sub_8037388 - extends
     * the 6-slot record sub_8006600/sub_8000EE4 already established. */
    struct icon_slot slots[7];
};

/* A single glyph's draw metrics - `icon_manager.glyphRecords` is an
 * array of these, 12 bytes apart, indexed by `icon_manager.charLookup`.
 * Established by GitHub issue #46's chunk (`sub_80285C4`/`sub_8028900`/
 * `MeasureText`, src/graphics/hud_icon_widget.c): `width` is the glyph's
 * horizontal advance (added to `posX` after each draw, and what
 * `MeasureText`/`sub_8028900` sum to measure a run of text); `field_4`
 * feeds a small (2-bit, `<<6` into a byte) shape/size selector;
 * `field_8` is a tile-row contribution combined with `posY`'s low byte
 * into the OAM-scratch draw request `sub_80285C4` builds. */
struct icon_glyph_metrics {
    s32 width;
    s32 field_4;
    u8 field_8;
    u8 unused_9[3];
};

COMPILE_TIME_ASSERT(sizeof(struct icon_glyph_metrics) == 0xC);

/* An OAM "icon" positioner: screen X/Y for the icon, then a pointer to
 * a small record describing which OAM slot(s) to draw it into.
 * gUnknown_030012E0/gUnknown_030012DC (src/graphics/oam_count.c) are two
 * instances of this, used for a left/right icon pair flanking a number
 * in sub_8006600; sub_8000EE4 takes one as its render-target object.
 *
 * The leading `unused_00`/`unused_10c` regions and part of `unused_118`
 * were opaque when this struct was first written (oam_count.c/
 * text_layout.c, both still not byte-matched); GitHub issue #46's chunk
 * (src/graphics/hud_icon_widget.c) reads and writes them directly and
 * fills in the real shape below. */
struct icon_manager {
    /* A 6-byte OAM-shaped draw-request scratch buffer, rebuilt fresh by
     * `sub_80285C4` for every glyph drawn (byte 0/1, halfword at 2,
     * halfword at 4) then handed to `sub_8006AC8`; zeroed 8 bytes at a
     * time (`sub_803A94C`, fixed-source CpuSet fill) by the widget
     * constructors. Bytes 6-7 are never written by anything in this
     * chunk. */
    u8 oam_scratch[8];
    /* Reverse char-byte -> glyph-index lookup table, built once by the
     * widget constructors (`InitHudIconWidgetA`/`InitHudIconWidgetB`)
     * from a small font-glyph-order table: `charLookup[c]` is the index
     * `i` (1-0x4F) such that the font table's `i`th byte equals `c`, or
     * 0 if `c` isn't in the font (or equals the font table's own
     * leading count byte). */
    u8 charLookup[0x100];
    /* Base value combined with `glyphIndex * field_124` (masked to 10
     * bits) into the OAM-scratch draw request's halfword at +4 -
     * plausibly a base tile/palette selector for the glyph sheet. */
    u32 field_108;
    /* `struct icon_glyph_metrics` array, 12 bytes/entry, indexed by
     * `charLookup[c]`. */
    struct icon_glyph_metrics *glyphRecords;
    u32 posX;
    u32 posY;
    /* Left-margin X: `posX` is reset to this on a newline character. */
    u32 field_118;
    /* Line height: added to `posY` on a newline character; also used
     * as a plain divisor by `sub_8028AC4`/`sub_8001214`
     * (src/util/word_util.c). */
    s32 field_11c;
    /* Advance width contributed by a literal space character, in place
     * of a `glyphRecords` lookup. */
    s32 spaceWidth;
    /* Per-glyph stride multiplier feeding `field_108`'s combine (see
     * above) - `field_108 + glyphIndex * field_124`. */
    s32 field_124;
    /* Pointer to a small per-widget data table (`gStaticData_085A4E70`-
     * family) `UploadHudTile` reads its first word from and shifts
     * right 13 into `field_12c`, then hands straight to `LoadTaggedAsset`
     * as the asset to upload. */
    void *field_128;
    /* Copied byte-for-byte from gUnknown_030012DC's copy into
     * gUnknown_030012E0's copy by sub_8037388 - meaning not understood
     * yet. */
    u32 field_12c;
    struct icon_record *record;
};

COMPILE_TIME_ASSERT(sizeof(struct icon_manager) == 0x134);

#endif /* __ICON_MANAGER_H__ */
