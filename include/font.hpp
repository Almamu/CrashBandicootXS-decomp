#ifndef GUARD_FONT_HPP
#define GUARD_FONT_HPP

/* The bitmap fonts as C++ (#664, docs/cplusplus.md, step 10b): the classes
 * behind gFontVtable, gSmallFontVtable and gLargeFontVtable (src/text/,
 * src/util/aabb_setup.cpp). bitmap_font.h's struct bitmap_font is their C
 * view, for the files that are still C, and gSmallFont/gLargeFont are
 * the two instances.
 *
 * No `#pragma interface`: g++ emits Font's vtable in src/text/font.cpp,
 * which has its key method, MeasureText (its first non-inline virtual
 * one; the destructor is inline), and SmallFont's and LargeFont's in
 * src/util/aabb_setup.cpp, which has their destructors. ldscript.txt
 * places them at their ROM addresses (docs/cplusplus.md, "Emitting the
 * vtables"); cxx_symbols.txt maps the mangled names onto the C names. */

extern "C" {
#include "core.h"
#include "text.h"
#include <agb_syscall.h>
#include <libgcc.h>
}

/* A bitmap font (0x134 bytes): draws text as one OBJ per glyph through
 * AddOamEntry, with a cursor (posX, posY) that the draw methods advance.
 *
 * The constructor, the destructor and the accessors are inline: the
 * subclasses' constructors and destructors and the callers expand them,
 * and font.cpp, which gets Font's vtable, also gets an out-of-line copy
 * of each at its end, in the reverse of their declaration order, as the
 * ROM has them (InitFont, FontHeightToLines ... DestroyFont). Nothing
 * calls those copies. */
class Font
{
public:
    u8 oam_scratch[8];                             // 0x000 - the glyph's OAM entry
    u8 charLookup[0x100];                          // 0x008 - char -> glyph index
    u32 tileBase;                                  // 0x108
    const struct icon_glyph_metrics *glyphRecords; // 0x10C
    u32 posX;                                      // 0x110
    u32 posY;                                      // 0x114
    u32 marginX;                                   // 0x118 - posX after a newline
    s32 lineHeight;                                // 0x11C
    s32 spaceWidth;                                // 0x120
    s32 glyphTileStride;                           // 0x124
    const void *tiles;                             // 0x128 - the tagged tile asset
    u32 tileCount;                                 // 0x12C
    // 0x130: the vtable pointer, gFontVtable or a subclass's

    /* 1 DestroyFont */
    virtual ~Font()
    {
    }

    virtual s32 MeasureText(u8 *text);            // 2 FontMeasureText
    virtual s32 MeasureChars(u8 *str, s32 count); // 3 FontMeasureChars
    virtual void DrawText(u8 *str);               // 4 FontDrawText
    virtual void DrawChars(u8 *str, s32 count);   // 5 FontDrawChars
    virtual void DrawGlyph(u8 charByte);          // 6 FontDrawGlyph
    virtual void PutChar(u32 charByte);           // 7 FontPutChar
    virtual void UploadTiles();                   // 8 FontUploadTiles
    void SetPalette(u8 palette);                  // FontSetPalette
    void ResetPalette();                          // FontResetPalette
    s32 TextHeight(u8 *str);                      // FontTextHeight

    /* FontSetTileBase */
    void SetTileBase(u32 base)
    {
        tileBase = base;
        UploadTiles();
    }

    /* FontGetX */
    u32 GetX()
    {
        return posX;
    }

    /* FontGetY */
    u32 GetY()
    {
        return posY;
    }

    /* FontSetMargin */
    void SetMargin(u32 x)
    {
        marginX = x;
    }

    /* FontGetMargin */
    u32 GetMargin()
    {
        return marginX;
    }

    /* FontNewLineAt */
    void NewLineAt(u32 y)
    {
        posX = marginX;
        posY = y;
    }

    /* FontSetPos */
    void SetPos(u32 x, u32 y)
    {
        posX = x;
        posY = y;
    }

    /* FontGetTileCount */
    u32 GetTileCount()
    {
        return tileCount;
    }

    /* The lines of text that fit in `height` pixels (FontHeightToLines). */
    s32 HeightToLines(s32 height)
    {
        return __udivsi3(height, lineHeight);
    }

    /* InitFont: resets the cursor, the margin and tileCount and zeroes the
     * OAM entry. */
    Font()
    {
        u32 zero;

        posX = posY = 0;
        marginX = 0;
        tileCount = 0;
        zero = 0;
        CpuSet(&zero, this, CPU_SET_32BIT | CPU_SET_SRC_FIXED | 2);
    }
};

COMPILE_TIME_ASSERT(font_hpp, sizeof(Font) == sizeof(struct bitmap_font));

/* gSmallFont: 9-pixel lines, 4-pixel spaces, 2 tiles per glyph
 * (InitSmallFont, src/text/font_glyph.cpp; ~SmallFont, aabb_setup.cpp). */
class SmallFont : public Font
{
public:
    SmallFont();          // InitSmallFont
    virtual ~SmallFont(); // 1 DestroySmallFont
};

/* gLargeFont: 16-pixel lines, 6-pixel spaces, 4 tiles per glyph and
 * 16x16 OBJs (InitLargeFont, src/text/font_glyph.cpp; ~LargeFont,
 * aabb_setup.cpp). */
class LargeFont : public Font
{
public:
    LargeFont();          // InitLargeFont
    virtual ~LargeFont(); // 1 DestroyLargeFont
};

#endif /* !GUARD_FONT_HPP */
