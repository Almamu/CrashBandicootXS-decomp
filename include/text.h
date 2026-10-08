#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

/* The text subsystem (src/text/): the bitmap-font renderer and the
 * word-wrapping text box. The fonts are C++ classes (include/font.hpp);
 * `struct bitmap_font`, their C view, and its vtable record are in
 * bitmap_font.h; the text rectangle is a `struct aabb` (aabb.h).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it with a `codegen:` comment instead of including
 * this header (docs/headers_plan.md); none of the text subsystem's
 * callers needs one. */

#include "core.h"
#include "aabb.h"
#include "bitmap_font.h"

/* The two font instances, built by InitLevelState (IWRAM, sym_iwram.txt):
 * a SmallFont and a LargeFont (include/font.hpp) to the C++ files. */
#ifdef __cplusplus
extern class Font *gSmallFont;
extern class Font *gLargeFont;
#else
extern struct bitmap_font *gSmallFont;
extern struct bitmap_font *gLargeFont;
#endif

/* The two fonts' character sets and glyph metrics
 * (src/data/hud_fonts_174be0.c) and tile data (data/data.s). */
extern const u8 gSmallFontChars[];
extern const struct icon_glyph_metrics gSmallFontGlyphs[79];
extern const u8 gLargeFontChars[];
extern const struct icon_glyph_metrics gLargeFontGlyphs[75];
extern const u8 gSmallFontTiles[];
extern const u8 gLargeFontTiles[];

/* Font's methods (include/font.hpp) under their C names
 * (cxx_symbols.txt), for the C callers: src/text/font.cpp */
extern void FontSetPalette(struct bitmap_font *self, u8 val);

/* src/text/text_box.c */
extern s32 GetWordLength(u8 *s);
#ifdef __cplusplus
extern s32 DrawWrappedTextInBox(u8 *text, class Font *self, struct aabb *box, s32 mode);
#else
extern s32 DrawWrappedTextInBox(u8 *text, struct bitmap_font *self, struct aabb *box, s32 mode);
#endif

/* src/text/wrapped_text.cpp (C linkage) */
#ifdef __cplusplus
extern s32 DrawWrappedText(u8 *text, class Font *self, struct aabb *box, s32 limit, s32 mode);
#else
extern s32 DrawWrappedText(u8 *text, struct bitmap_font *self, struct aabb *box, s32 limit,
                           s32 mode);
#endif

#endif /* GUARD_TEXT_H */
