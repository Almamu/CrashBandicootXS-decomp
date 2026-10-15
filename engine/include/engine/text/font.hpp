#ifndef GUARD_FONT_HPP
#define GUARD_FONT_HPP

/* The bitmap fonts as C++ (#664, docs/cplusplus.md, step 10b): the classes
 * behind gFontVtable, gSmallFontVtable and gLargeFontVtable (engine/src/engine/text/,
 * engine/src/engine/util/aabb_setters.cpp). gSmallFont/gLargeFont are the two instances.
 * There is no C view (bitmap_font.h's `struct bitmap_font` went in #754);
 * bitmap_font.h keeps the glyph metrics.
 *
 * No `#pragma interface`: g++ emits Font's vtable in engine/src/engine/text/font.cpp,
 * which has its key method, MeasureText (its first non-inline virtual
 * one; the destructor is inline), and SmallFont's and LargeFont's in
 * engine/src/engine/util/aabb_setters.cpp, which has their destructors. games/xs/ldscript.txt
 * places them at their ROM addresses (docs/cplusplus.md, "Emitting the
 * vtables"); cxx_symbols.txt maps the mangled names onto the C names.
 *
 * The three classes are in the namespace Engine::Text (#1033,
 * docs/cplusplus.md "Namespaces"): `DrawText__Q36Engine4Text4FontPCUc`,
 * `_vt.Q36Engine4Text4Font` (in the section
 * `.gnu.linkonce.d._vt.Q36Engine4Text4Font`, which games/xs/ldscript.txt names) &
 * co. text.h's C-linkage wrapped-text functions stay global. */

extern "C" {
#include "core.h"
#include "engine/gfx/gfx.h"
#include "engine/text/text.h"
#include <agb_syscall.h>
#include <libgcc.h>
}

namespace Engine
{
namespace Text
{

/* A bitmap font (0x134 bytes): draws text as one OBJ per glyph through
 * AddOamEntry, with a cursor (posX, posY) that the draw methods advance.
 *
 * The constructor, the destructor and the accessors are inline: the
 * subclasses' constructors and destructors and the callers expand them,
 * and font.cpp, which gets Font's vtable, also gets an out-of-line copy
 * of each at its end, in the reverse of their declaration order, as the
 * ROM has them (InitFont, FontHeightToLines ... DestroyFont). Nothing
 * calls those copies. */
#ifdef GAME_N2
/* N-Tranced's glyph record: XS's struct icon_glyph_metrics, but the Y
 * offset is a word (its DrawGlyph, 0x08029158, loads it whole). */
struct font_glyph {
    s32 width;   // 0x00 - the advance
    s32 shape;   // 0x04 - the OBJ shape
    s32 yOffset; // 0x08 - added to posY
};

/* What N-Tranced's Font is built from (its constructor's argument). */
struct font_desc {
    screen_h_t lineHeight;                 // 0x00
    screen_w_t spaceWidth;                 // 0x04
    u32 glyphTiles;                        // 0x08 - tiles per glyph
    u32 objSize;                           // 0x0C - the glyphs' OBJ size
    const void *tiles;                     // 0x10
    const u8 *chars;                       // 0x14 - the characters, by glyph
    u32 charCount;                         // 0x18
    const struct font_glyph *glyphRecords; // 0x1C
};
#endif

class Font
{
public:
    // 0x000 - the glyph's OAM entry, rebuilt by DrawGlyph for every glyph and
    //         handed to AddOamEntry; the constructor zeroes it
    struct oam_attrs oam_scratch;
#ifdef GAME_N2
    /* N-Tranced's font is 0x13C bytes, built from a struct font_desc (its
     * constructor, 0x08028E14): this word is new, so everything up to
     * glyphTileStride is 4 bytes later (lineHeight at 0x120); then two
     * more words, and no tileCount. */
    u32 objSize; // 0x08 - the glyphs' OBJ size (attribute 1's size bits)
#endif
    // 0x008 - char -> glyph index (1-0x4F; 0: not in the font), built by the
    //         subclasses' constructors from the font's character set
    u8 charLookup[0x100];
    // 0x108 - the OBJ tile base: a glyph's tile is tileBase + glyph * glyphTileStride
    u32 tileBase;
#ifdef GAME_N2
    const struct font_glyph *glyphRecords;
#else
    const struct icon_glyph_metrics *glyphRecords; // 0x10C
#endif
    screen_ux_t posX;      // 0x110 - the cursor, in screen pixels
    screen_uy_t posY;      // 0x114
    screen_ux_t marginX;   // 0x118 - posX after a newline
    screen_h_t lineHeight; // 0x11C - posY's step on a newline
    screen_w_t spaceWidth; // 0x120 - a space's advance
    s32 glyphTileStride;   // 0x124
#ifdef GAME_N2
    // (glyphTileStride is the bytes of a glyph's tiles: DrawGlyph uploads
    // each glyph's tiles as it draws it)
    u32 glyphTiles;    // 0x12C - tiles per glyph
    const void *tiles; // 0x130 - the glyphs' tiles, glyphTileStride bytes each
    // 0x134 - the 16-colour palette the glyphs are drawn with (SetPalette,
    // and text codes 1-3)
    const u16 *palette;
#else
    const void *tiles; // 0x128 - the tagged tile asset
    // 0x12C - set by UploadTiles: the tile asset's first word >> 13
    u32 tileCount;
#endif
    // 0x130: the vtable pointer, gFontVtable or a subclass's

    /* 1 DestroyFont */
    virtual ~Font()
    {
    }

#ifdef GAME_N2
    /* N-Tranced's vtable (0x087F9AC4) has three more slots: DrawText
     * (0x08028F04), MeasureText and TextHeight; the rest aren't virtual
     * (its DrawChars calls PutChar directly) */
    virtual void DrawText(const u8 *str);           // 2
    virtual screen_w_t MeasureText(const u8 *text); // 3
    virtual s32 TextHeight(const u8 *str);          // 4
    screen_w_t MeasureChars(const u8 *str, s32 count);
    void DrawChars(const u8 *str, s32 count);
    void PutChar(u32 charByte);
    void DrawGlyph(u32 charByte);
    void SetPalette(u8 kind);
    Font(const struct font_desc *desc);
#else
    virtual screen_w_t MeasureText(const u8 *text);            // 2 FontMeasureText
    virtual screen_w_t MeasureChars(const u8 *str, s32 count); // 3 FontMeasureChars
    virtual void DrawText(const u8 *str);                      // 4 FontDrawText
    virtual void DrawChars(const u8 *str, s32 count);          // 5 FontDrawChars
    virtual void DrawGlyph(u8 charByte);                       // 6 FontDrawGlyph
    virtual void PutChar(u32 charByte);                        // 7 FontPutChar
    virtual void UploadTiles();                                // 8 FontUploadTiles
    void SetPalette(u8 palette);                               // FontSetPalette
    void ResetPalette();                                       // FontResetPalette
    s32 TextHeight(const u8 *str);                             // FontTextHeight
#endif

#ifndef GAME_N2
    /* FontSetTileBase */
    void SetTileBase(u32 base)
    {
        tileBase = base;
        UploadTiles();
    }
#endif

    /* FontGetX */
    screen_ux_t GetX()
    {
        return posX;
    }

    /* FontGetY */
    screen_uy_t GetY()
    {
        return posY;
    }

    /* FontSetMargin */
    void SetMargin(screen_ux_t x)
    {
        marginX = x;
    }

    /* FontGetMargin */
    screen_ux_t GetMargin()
    {
        return marginX;
    }

    /* FontNewLineAt */
    void NewLineAt(screen_uy_t y)
    {
        posX = marginX;
        posY = y;
    }

    /* FontSetPos */
    void SetPos(screen_ux_t x, screen_uy_t y)
    {
        posX = x;
        posY = y;
    }

#ifndef GAME_N2
    /* FontGetTileCount */
    u32 GetTileCount()
    {
        return tileCount;
    }
#endif

    /* The lines of text that fit in `height` pixels (FontHeightToLines). */
    s32 HeightToLines(screen_h_t height)
    {
#ifdef GAME_N2
        return __divsi3(height, lineHeight); // N-Tranced's divides signed
#else
        return __udivsi3(height, lineHeight);
#endif
    }

#ifndef GAME_N2
    /* InitFont: resets the cursor, the margin and tileCount and zeroes the
     * OAM entry. */
    Font()
    {
        u32 zero;

        posX = posY = 0;
        marginX = 0;
        tileCount = 0;
        zero = 0;
        CpuSet(&zero, &oam_scratch,
               CPU_SET_32BIT | CPU_SET_SRC_FIXED | sizeof(oam_scratch) / sizeof(u32));
    }
#endif
};

LAYOUT_ASSERT(font_hpp, sizeof(Font) == 0x134);
N2_LAYOUT_ASSERT(font_hpp, sizeof(Font) == 0x13C);

/* gSmallFont: 9-pixel lines, 4-pixel spaces, 2 tiles per glyph
 * (InitSmallFont, engine/src/engine/text/font_glyph.cpp; ~SmallFont, aabb_setters.cpp). */
class SmallFont : public Font
{
public:
    SmallFont();          // InitSmallFont
    virtual ~SmallFont(); // 1 DestroySmallFont
};

/* gLargeFont: 16-pixel lines, 6-pixel spaces, 4 tiles per glyph and
 * 16x16 OBJs (InitLargeFont, engine/src/engine/text/font_glyph.cpp; ~LargeFont,
 * aabb_setters.cpp). */
class LargeFont : public Font
{
public:
    LargeFont();          // InitLargeFont
    virtual ~LargeFont(); // 1 DestroyLargeFont
};

} // namespace Text
} // namespace Engine

#endif /* !GUARD_FONT_HPP */
