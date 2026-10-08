#include "font.hpp"

extern "C" {
#include "math_util.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
#include "sprite_bank.h"
}

/* GitHub issue #46: Font's MeasureText, UploadTiles and palette setters
 * (include/font.hpp), then the out-of-line copies of its inline methods.
 * MeasureText is the class's key method, its first non-inline virtual
 * one, so g++ emits gFontVtable here, and with it a copy of every inline
 * method of the class at the end of the object, in the reverse of their
 * declaration order: InitFont (the constructor), FontHeightToLines ...
 * FontSetTileBase (the accessors) and DestroyFont (the destructor), as
 * the ROM has them. Nothing calls those copies: the subclasses' and the
 * callers' code expands them. This was two C files, font_measure.c and
 * font.c; the copies are why they are one object.
 *
 * Built with old_agbcp (the Makefile's OLD_AGBCC_OBJS), which
 * MeasureText needs. */

/* Width of the widest line of `text`: sums each line's advance widths
 * (space = `spaceWidth`, other characters = their glyph's width),
 * resetting at each newline and keeping the running maximum. */
s32 Font::MeasureText(u8 *text)
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
        default:
            cur += glyphRecords[charLookup[c]].width;
            break;
        }
    }
    return CLAMP_MIN(cur, maxWidth);
}

/* Uploads the font's tiles to the OBJ VRAM slot selected by `tileBase`,
 * recording the asset's tile count (its header word `>> 13`) in
 * `tileCount`. */
void Font::UploadTiles()
{
    const void *asset = tiles;

    tileCount = *(const u32 *)asset >> 13;
    LoadTaggedAsset(asset, OBJ_VRAM0 + (tileBase << 5));
}

/* Sets the palette of the glyphs' OBJs (attr2's top nibble, the high
 * half of `oam_scratch[5]`). */
void Font::SetPalette(u8 palette)
{
    u32 shifted = palette << 4;

    oam_scratch[5] = (oam_scratch[5] & 0xF) | shifted;
}

/* codegen: GetPaletteSlot returns u8 (gfx.h); with the u8 return
 * ResetPalette adds `lsl #0x18; lsr #0x14` where the ROM has one
 * `lsl #4`. docs/headers_plan.md */
extern "C" s32 GetPaletteSlot_s32(u8 *cache, s32 recordId) asm("GetPaletteSlot");

/* Sets the glyphs' palette back to the palette-cache slot of sprite bank
 * 35's first animation's palette (see docs/rom_map.md's
 * `gSpriteBankTable` investigation), the same `oam_scratch[5]` nibble
 * SetPalette sets. */
void Font::ResetPalette()
{
    u8 *cache = (u8 *)gPaletteCache;
    const struct sprite_anim *anim = gSpriteBankSet->table->banks[35].anims;
    u8 field = anim->paletteId;
    s32 slot = GetPaletteSlot_s32(cache, field);
    u32 shifted = slot << 4;

    oam_scratch[5] = (oam_scratch[5] & 0xF) | shifted;
}
