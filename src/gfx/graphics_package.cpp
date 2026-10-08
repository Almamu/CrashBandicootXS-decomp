#include "sprite_obj.hpp"
#include "graphics_package.hpp"
extern "C" {
#include "core.h"
#include "math_util.h"
#include "graphics_package.h"
#include "gfx.h"
#include "gba/gba.h"
#include "system.h"
#include "globals.h"
}

/* C++ since the #664 cleanup: LoadGraphicsPackage's tilemap buffer is a
 * `new u16[]`/`delete[]` (the C called OperatorNewArray and
 * OperatorDeleteArray). The BG setup and the sprite box are classes since
 * #753 (BgSetup, ScaledSprite; include/graphics_package.hpp). */

/* GitHub issue #30. Loads one BG: the palette into bank `paletteBank`,
 * the tiles into char block `charBlock`, and the tilemap into screen
 * block `screenBlock`, ORing the palette bank into every entry. Palettes
 * of more than 0x20 colors switch the BG to 256-color mode. Built with
 * old_agbcc - see docs/matching/archive/issue-30-old-agbcc.md. */
void BgSetup::Load(const struct bg_package *pkg)
{
    u16 *map;
    u16 *src;
    u16 *dest;
    s32 pal;
    s32 x;
    s32 y;

    if (*(u32 *)pkg->paletteAsset >> 8 <= 0x20)
        ctrl.bits.colorMode = 0;
    else
        ctrl.bits.colorMode = 1;
    LoadTaggedAsset(pkg->paletteAsset, (void *)(PLTT + (paletteBank << 5)));
    LoadTaggedAsset(pkg->tileAsset, (void *)(VRAM + (charBlock << 14)));
    map = new u16[*(u32 *)pkg->mapAsset >> 9];
    LoadTaggedAsset(pkg->mapAsset, map);
    pal = paletteBank << 12;
    src = map;
    dest = (u16 *)(VRAM + (screenBlock << 11));
    for (y = 0; y < (s32)pkg->height; y++) {
        u16 *next = dest + 0x20;
        for (x = 0; x < (s32)pkg->width; x++)
            dest[x] = pal | src[x];
        src += pkg->width;
        dest = next;
    }
    delete[] map;
}

/* GitHub issue #30. Both built with old_agbcc - see
 * docs/matching/archive/issue-30-old-agbcc.md. */

/* The BG control value InitBgSetup built, for REG_BGnCNT. */
u16 BgSetup::GetControl()
{
    return ctrl.raw;
}

/* Fills the BG setup buffer: char block, screen block and palette bank
 * verbatim, and a control value with priority `priority`, char base
 * `charBlock`, screen base `screenBlock` and size 0. */
BgSetup::BgSetup(u32 charBlock, u32 screenBlock, u32 paletteBank, u32 priority)
{
    ctrl.raw = 0;
    ctrl.bits.priority = priority;
    this->charBlock = charBlock;
    ctrl.bits.charBase = charBlock;
    ctrl.bits.size = 0;
    this->screenBlock = screenBlock;
    ctrl.bits.screenBase = screenBlock;
    this->paletteBank = paletteBank;
}

/* GitHub issue #30: the sprite-box fitter and its OAM writer. Built with
 * old_agbcc - see docs/matching/archive/issue-30-old-agbcc.md. */

/* Picks the smallest-area box preset (gObjSizeWidths/674) that a
 * width x height box fits in at 50% zoom or better, puts its shape/size
 * and an area-derived tile number into the OAM template, and stores the
 * Q8 scale factors plus the affine mode (3 shrunk, 1 enlarged, 0 1:1). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void ScaledSprite::Fit(s32 width, s32 height)
{
    const s32 *widths;
    const s32 *heights;
    s32 best;
    s32 i;
    s32 idx;

    this->width = width;
    this->height = height;
    best = 0x1000;
    for (i = 0, widths = gObjSizeWidths, heights = gObjSizeHeights; i < 12; i++) {
        if (width <= widths[i] * 2 && height <= heights[i] * 2 && widths[i] * heights[i] < best) {
            best = widths[i] * heights[i];
            sizeIndex = i;
        }
    }
    idx = sizeIndex;
    oam.size = idx;
    oam.shape = idx >> 2;
    oam.tileNum = 0x400 - best / 32;
    scaleX = Q8_DIV(widths[idx], width);
    scaleY = Q8_DIV(heights[idx], height);
    if (scaleX < 0x100 || scaleY < 0x100)
        oam.affineMode = 3;
    else if (scaleX > 0x100 || scaleY > 0x100)
        oam.affineMode = 1;
    else
        oam.affineMode = 0;
}

/* Positions the OAM template for the box's affine mode (centred on the
 * preset box when scaled, or on the double-size area), allocates an
 * affine matrix holding the scale factors when affine, and queues the
 * entry into the shadow OAM buffer. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void ScaledSprite::Draw()
{
    OamBuffer *buf;
    s32 n;

    switch ((u32)oam.affineMode) {
    case 0:
        oam.x = x;
        oam.y = y;
        break;
    case 1:
        oam.x = x - (gObjSizeWidths[sizeIndex] - width) / 2;
        oam.y = y - (gObjSizeHeights[sizeIndex] - height) / 2;
        break;
    case 3:
        oam.x = x + width / 2 - gObjSizeWidths[sizeIndex];
        oam.y = y + height / 2 - gObjSizeHeights[sizeIndex];
        break;
    }
    if (oam.affineMode == 0) {
        oam.matrixBit3 = 0;
        oam.matrixBit4 = 0;
    } else {
        buf = gOamBuffer;
        n = buf->matrixCount++;
        oam.matrixLo = n;
        oam.matrixBit3 = n >> 3;
        oam.matrixBit4 = n >> 4;
        {
            /* Read through a u16 local: the ROM loads each scale with ldrh
             * ahead of its store address. */
            u16 param = scaleX;
            s32 i = n * 4;
            buf->table[i].attr[3] = param;
            buf->table[i + 1].attr[3] = 0;
            buf->table[i + 2].attr[3] = 0;
            param = scaleY;
            buf->table[i + 3].attr[3] = param;
        }
    }
    gOamBuffer->Add(&oam);
}

/* UNUSED - no caller anywhere in the ROM (no `bl` in expected/*.s, no
 * pointer to it in baserom.gba), like the other three setters below and
 * Fit/Draw above (ScaledSprite's other methods). Sets the sprite box's solid color `color` (a 256-color
 * index): stores it in `color` (+0x1c), puts its palette bank
 * (`color / 16`) in the OAM template's palette bits (attr2 bits 12-15),
 * and fills 64 OBJ tiles from tile 0x3C0 (`0x06017800`) with the color's
 * low nibble through a 16-bit fixed-source DMA3 transfer. */
void ScaledSprite::SetColor(s32 color)
{
    u32 nibble;
    u32 dest;

    this->color = color;
    oam.palette = color / 16;
    nibble = color & 0xf;
    nibble |= nibble << 4 | nibble << 8 | nibble << 12;
    dest = (u32)(OBJ_VRAM0 + 0x3C0 * TILE_SIZE_4BPP);
    DmaFill16(3, nibble, dest, 0x800);
}

/* UNUSED (see SetScaledSpriteColor). Sets the sprite box's OAM priority
 * (attr2 bits 10-11). */
void ScaledSprite::SetPriority(u32 priority)
{
    oam.priority = priority;
}

/* UNUSED (see SetScaledSpriteColor). Sets the sprite box's position
 * (`x`/`y`, +0x00/+0x04). */
void ScaledSprite::SetPos(u32 x, u32 y)
{
    this->x = x;
    this->y = y;
}

/* UNUSED (see SetScaledSpriteColor). Resets the OAM template's
 * attributes FitScaledSprite doesn't own: the object mode, mosaic, color
 * mode and shape bits of attr0 and the priority. */
void ScaledSprite::ResetAttrs()
{
    oam.objMode = 0;
    oam.mosaic = 0;
    oam.bpp = 0;
    oam.shape = 0;
    oam.priority = 0;
}
