#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

/* The text subsystem (src/text/): the bitmap-font renderer and the
 * word-wrapping text box. `struct bitmap_font` and its vtable record are
 * in bitmap_font.h; the text rectangle is a `struct aabb` (aabb.h).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it with a `codegen:` comment instead of including
 * this header (docs/headers_plan.md); none of the text subsystem's
 * callers needs one. */

#include "core.h"
#include "aabb.h"
#include "bitmap_font.h"
#include "vtable.h"

/* The two font instances, built by InitLevelState (IWRAM, sym_iwram.txt). */
extern struct bitmap_font *gSmallFont;
extern struct bitmap_font *gLargeFont;

/* The fonts' vtables (src/data/entity_vtables_7e3bec.c): `record` of a
 * font built by InitFont, InitSmallFont and InitLargeFont. */
extern const struct vtable_slot gLargeFontVtable[9];
extern const struct vtable_slot gSmallFontVtable[9];
extern const struct vtable_slot gFontVtable[9];

/* The two fonts' character sets and glyph metrics
 * (src/data/hud_fonts_174be0.c) and tile data (data/data.s). */
extern const u8 gSmallFontChars[];
extern const struct icon_glyph_metrics gSmallFontGlyphs[79];
extern const u8 gLargeFontChars[];
extern const struct icon_glyph_metrics gLargeFontGlyphs[75];
extern const u8 gSmallFontTiles[];
extern const u8 gLargeFontTiles[];

/* src/text/font_measure.c */
extern s32 FontMeasureText(struct bitmap_font *self, u8 *text);

/* src/text/font.c */
extern void FontUploadTiles(struct bitmap_font *self);
extern void FontSetPalette(struct bitmap_font *self, u8 val);
extern void FontResetPalette(struct bitmap_font *self);
extern struct bitmap_font *InitFont(struct bitmap_font *self);
extern s32 FontHeightToLines(struct bitmap_font *self, s32 value);
extern u32 FontGetTileCount(struct bitmap_font *self);
extern void FontSetPos(struct bitmap_font *self, u32 x, u32 y);
extern void FontNewLineAt(struct bitmap_font *self, u32 y);
extern u32 FontGetMargin(struct bitmap_font *self);
extern void FontSetMargin(struct bitmap_font *self, u32 val);
extern u32 FontGetY(struct bitmap_font *self);
extern u32 FontGetX(struct bitmap_font *self);
extern void FontSetTileBase(struct bitmap_font *self, u32 val);
extern void DestroyFont(struct bitmap_font *self, u32 flags);

/* The two fonts' destructors (vtable slot 1). They are defined in
 * src/util/aabb_setup.c, which holds the ROM range they sit in. */
extern void DestroyLargeFont(void *self, u32 flags);
extern void DestroySmallFont(void *self, u32 flags);

/* src/text/font_glyph.c */
extern void FontDrawGlyph(struct bitmap_font *self, u8 charByte);
extern struct bitmap_font *InitSmallFont(struct bitmap_font *self);
extern struct bitmap_font *InitLargeFont(struct bitmap_font *self);
extern void FontPutChar(struct bitmap_font *self, u32 charByte);

/* src/text/font_draw_chars.c */
extern void FontDrawChars(struct bitmap_font *self, u8 *str, s32 count);

/* src/text/font_draw_text.c */
extern void FontDrawText(struct bitmap_font *self, u8 *str);
extern s32 FontMeasureChars(struct bitmap_font *self, u8 *str, s32 count);

/* src/text/font_height.c */
extern s32 FontTextHeight(struct bitmap_font *self, u8 *str);

/* src/text/text_box.c */
extern s32 GetWordLength(u8 *s);
extern s32 DrawWrappedTextInBox(u8 *text, struct bitmap_font *self, struct aabb *box, s32 mode);

/* src/text/wrapped_text.c */
extern s32 DrawWrappedText(u8 *text, struct bitmap_font *self, struct aabb *box, s32 limit, s32 mode);

#endif /* GUARD_TEXT_H */
