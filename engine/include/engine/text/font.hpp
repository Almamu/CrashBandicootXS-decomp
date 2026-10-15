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
class Font
{
public:
    // 0x000 - the glyph's OAM entry, rebuilt by DrawGlyph for every glyph and
    //         handed to AddOamEntry; the constructor zeroes it
    struct oam_attrs oam_scratch;
#ifdef GAME_N2
    /* N-Tranced's font is 0x13C bytes, built from a descriptor (its
     * constructor, 0x08028E14, copies the descriptor's words into it):
     * this word (the descriptor's fourth) is new, so everything up to
     * glyphTileStride is 4 bytes later (lineHeight at 0x120: TextHeight,
     * its accessors 0x08029414-0x08029484); then two more words, and no
     * tileCount (the constructor doesn't zero one). */
    u32 unk_08;
#endif
    // 0x008 - char -> glyph index (1-0x4F; 0: not in the font), built by the
    //         subclasses' constructors from the font's character set
    u8 charLookup[0x100];
    // 0x108 - the OBJ tile base: a glyph's tile is tileBase + glyph * glyphTileStride
    u32 tileBase;
    const struct icon_glyph_metrics *glyphRecords; // 0x10C
    screen_ux_t posX;                              // 0x110 - the cursor, in screen pixels
    screen_uy_t posY;                              // 0x114
    screen_ux_t marginX;                           // 0x118 - posX after a newline
    screen_h_t lineHeight;                         // 0x11C - posY's step on a newline
    screen_w_t spaceWidth;                         // 0x120 - a space's advance
    s32 glyphTileStride;                           // 0x124
#ifdef GAME_N2
    u32 unk_12C;       // the descriptor's third word (glyphTileStride is it << 5)
    const void *tiles; // 0x130 - the descriptor's fifth word
    // 0x134 - a pointer its 0x08029390 sets by its u8 argument (0-2)
    const void *unk_134;
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

    /* FontSetTileBase */
    void SetTileBase(u32 base)
    {
        tileBase = base;
        UploadTiles();
    }

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
        return __udivsi3(height, lineHeight);
    }

    /* InitFont: resets the cursor, the margin and tileCount and zeroes the
     * OAM entry. */
    Font()
    {
        u32 zero;

        posX = posY = 0;
        marginX = 0;
#ifndef GAME_N2
        tileCount = 0;
#endif
        zero = 0;
        CpuSet(&zero, &oam_scratch,
               CPU_SET_32BIT | CPU_SET_SRC_FIXED | sizeof(oam_scratch) / sizeof(u32));
    }
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
