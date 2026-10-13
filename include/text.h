#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

/* The text subsystem (src/text/): the bitmap-font renderer and the
 * word-wrapping text box. The fonts are C++ classes (include/font.hpp)
 * with no C view; their glyph metrics are in bitmap_font.h; the text
 * rectangle is a `struct aabb` (aabb.h).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it with a `codegen:` comment instead of including
 * this header (docs/headers.md); none of the text subsystem's
 * callers needs one. */

#include "core.h"
#include "aabb.h"
#include "bitmap_font.h"

/* The two font instances, built by InitLevelState (IWRAM, sym_iwram.txt):
 * a SmallFont and a LargeFont (include/font.hpp). No C file uses them. */
#ifdef __cplusplus
extern class Font *gSmallFont;
extern class Font *gLargeFont;
#endif

/* The two fonts' character sets and glyph metrics
 * (src/data/hud_fonts_174be0.cpp) and tile data (data/data.s). */
extern const u8 gSmallFontChars[];
extern const struct icon_glyph_metrics gSmallFontGlyphs[79];
extern const u8 gLargeFontChars[];
extern const struct icon_glyph_metrics gLargeFontGlyphs[75];

/* src/text/text_box.cpp and src/text/wrapped_text.cpp (C linkage). Their
 * callers are all C++. */
extern s32 GetWordLength(const u8 *s);

/* DrawWrappedText's `mode` (#843, docs/flags/text.md): when the words
 * reach the screen. */
#define WRAPPED_TEXT_MODE_BUFFERED 0 // never: the caller commits the OAM buffer
#define WRAPPED_TEXT_MODE_PER_WORD 1 // after every word (FrameLimiter::WaitForVBlank, Commit): typed out
#define WRAPPED_TEXT_MODE_PER_LINE 2 // after every line wrap
#ifdef __cplusplus
extern s32 DrawWrappedTextInBox(const u8 *text, class Font *self, struct aabb *box, s32 mode);
extern s32 DrawWrappedText(const u8 *text, class Font *self, struct aabb *box, s32 limit, s32 mode);
#endif

#endif /* GUARD_TEXT_H */
