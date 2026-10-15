#include "game/entities/sprite_obj.hpp"
#include "engine/text/font.hpp"

extern "C" {
#include "engine/util/math_util.h"
#include "engine/sys/system.h"
#include "engine/gfx/gfx.h"
#include "globals.h"
#include "engine/gfx/sprite_bank.h"
}

using Engine::Text::Font;

/* N-Tranced's Font (0x08028E14-0x080294A8), its own: built from a
 * struct font_desc, with text codes 1-3 that pick the glyphs' palette,
 * and each glyph's tiles uploaded as it's drawn (engine/include/engine/text/font.hpp,
 * docs/n_tranced.md "Struct layouts"). DrawText is the class's key
 * method, so g++ emits gFontVtable here, and at the end the copies of the
 * inline methods (FontHeightToLines ... DestroyFont). */

/* N-Tranced's palette cache and sprite banks, not named yet: the bank
 * `index` of the set, the palette `id` of the bank table, and the slot of
 * a 16-colour palette (loading it if it isn't). */
extern "C" const struct sprite_bank *sub_08007D80(Engine::Gfx::SpriteBankSet *set, s32 index);
extern "C" const u16 *sub_08007A58(Engine::Gfx::PaletteCache *cache, u32 id);
extern "C" s32 sub_08007934(Engine::Gfx::PaletteCache *cache, const u16 *palette);

/* The palettes of text codes 2 and 3, and of the button glyphs (A, B, L,
 * R) */
extern "C" const u16 gFontCode2Palette[16];
extern "C" const u16 gFontCode3Palette[16];
extern "C" const u16 gFontButtonPalette[16];

Font::Font(const struct font_desc *desc)
{
    u32 zero;
    u32 i;
    u32 j;

    posX = posY = 0;
    marginX = 0;
    zero = 0;
    CpuSet(&zero, &oam_scratch,
           CPU_SET_32BIT | CPU_SET_SRC_FIXED | sizeof(oam_scratch) / sizeof(u32));
    lineHeight = desc->lineHeight;
    spaceWidth = desc->spaceWidth;
    glyphTiles = desc->glyphTiles;
    glyphTileStride = desc->glyphTiles * 32;
    objSize = desc->objSize;
    glyphRecords = desc->glyphRecords;
    tiles = desc->tiles;
    SetPalette(0);
    for (i = 0; i < ARRAY_COUNT(charLookup); i++) {
        charLookup[i] = 0;
        for (j = 0; j < desc->charCount; j++) {
            if (desc->chars[j] == i) {
                charLookup[i] = j;
                break;
            }
        }
    }
}

void Font::DrawText(const u8 *str)
{
    u8 c;

    for (c = *str; c != 0; c = *++str) {
        switch (c) {
        case ' ':
            posX += spaceWidth;
            break;
        case '\n':
            posX = marginX;
            posY += lineHeight;
            break;
        case 1:
            palette =
                sub_08007A58(gPaletteCache, sub_08007D80(gSpriteBankSet, 27)->anims->paletteId);
            break;
        case 2:
            palette = gFontCode2Palette;
            break;
        case 3:
            palette = gFontCode3Palette;
            break;
        default:
            DrawGlyph(c);
            break;
        }
    }
}

void Font::PutChar(u32 charByte)
{
    switch (charByte) {
    case ' ':
        posX += spaceWidth;
        break;
    case '\n':
        posX = marginX;
        posY += lineHeight;
        break;
    case 1:
        palette = sub_08007A58(gPaletteCache, sub_08007D80(gSpriteBankSet, 27)->anims->paletteId);
        break;
    case 2:
        palette = gFontCode2Palette;
        break;
    case 3:
        palette = gFontCode3Palette;
        break;
    default:
        DrawGlyph(charByte);
        break;
    }
}

void Font::DrawGlyph(u32 charByte)
{
    u32 glyph = charLookup[charByte];

    if (charByte - 'A' <= 1 || charByte == 'L' || charByte == 'R')
        oam_scratch.palette = sub_08007934(gPaletteCache, gFontButtonPalette);
    else
        oam_scratch.palette = sub_08007934(gPaletteCache, palette);
    oam_scratch.x = posX;
    oam_scratch.y = posY + glyphRecords[glyph].yOffset;
    oam_scratch.shape = glyphRecords[glyph].shape;
    oam_scratch.size = objSize;
    oam_scratch.tileNum = gObjVramCursor->baseTile << 2;
    gObjVramCursor->Upload((const u8 *)tiles + glyph * glyphTileStride, glyphTileStride);
    gOamBuffer->Add(&oam_scratch);
    posX += glyphRecords[glyph].width;
}

void Font::DrawChars(const u8 *str, s32 count)
{
    if (count > 0) {
        s32 remaining = count;

        do {
            PutChar(*str);
            str++;
            remaining--;
        } while (remaining != 0);
    }
}

screen_w_t Font::MeasureChars(const u8 *str, s32 count)
{
    screen_w_t total = 0;
    s32 i;

    for (i = 0; i < count; i++) {
        switch (str[i]) {
        case ' ':
            total += spaceWidth;
            break;
        case 1:
        case 2:
        case 3:
        case '\n':
            break;
        default:
            total += glyphRecords[charLookup[str[i]]].width;
            break;
        }
    }
    return total;
}

s32 Font::TextHeight(const u8 *str)
{
    s32 total = lineHeight;
    u8 c;

    for (c = *str; c != 0; c = *++str) {
        if (c == '\n') {
            total += lineHeight;
        }
    }
    return total;
}

screen_w_t Font::MeasureText(const u8 *text)
{
    u32 maxWidth = 0;
    u32 cur = 0;
    u8 c;

    for (; (c = *text) != 0; text++) {
        switch (c) {
        case ' ':
            cur += spaceWidth;
            break;
        case '\n':
            if (cur > maxWidth)
                maxWidth = cur;
            cur = 0;
            break;
        case 1:
        case 2:
        case 3:
            break;
        default:
            cur += glyphRecords[charLookup[c]].width;
            break;
        }
    }
    return CLAMP_MIN(cur, maxWidth);
}

void Font::SetPalette(u8 kind)
{
    switch (kind) {
    case 0:
        palette = sub_08007A58(gPaletteCache, sub_08007D80(gSpriteBankSet, 27)->anims->paletteId);
        break;
    case 1:
        palette = gFontCode2Palette;
        break;
    case 2:
        palette = gFontCode3Palette;
        break;
    }
}
