/* The sprite box (ScaledSprite; include/engine/gfx/graphics_package.hpp): the fitter,
 * its OAM writer and the template accessors. Split from
 * gfx/graphics_package.cpp (#767), same flags (old_agbcc). */

#include "sprite_obj.hpp"
#include "engine/gfx/graphics_package.hpp"
extern "C" {
#include "core.h"
#include "engine/util/math_util.h"
#include "engine/gfx/gfx.h"
#include "gba/gba.h"
#include "engine/sys/system.h"
#include "globals.h"
}

using Engine::Gfx::OamBuffer;
using Engine::Gfx::ScaledSprite;

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
    if (scaleX < INT_TO_Q8(1) || scaleY < INT_TO_Q8(1))
        oam.affineMode = OAM_AFFINE_MODE_DOUBLE;
    else if (scaleX > INT_TO_Q8(1) || scaleY > INT_TO_Q8(1))
        oam.affineMode = OAM_AFFINE_MODE_AFFINE;
    else
        oam.affineMode = OAM_AFFINE_MODE_NORMAL;
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

    switch (oam.affineMode) {
    case OAM_AFFINE_MODE_NORMAL:
        oam.x = x;
        oam.y = y;
        break;
    case OAM_AFFINE_MODE_AFFINE:
        oam.x = x - (gObjSizeWidths[sizeIndex] - width) / 2;
        oam.y = y - (gObjSizeHeights[sizeIndex] - height) / 2;
        break;
    case OAM_AFFINE_MODE_DOUBLE:
        oam.x = x + width / 2 - gObjSizeWidths[sizeIndex];
        oam.y = y + height / 2 - gObjSizeHeights[sizeIndex];
        break;
    }
    if (oam.affineMode == OAM_AFFINE_MODE_NORMAL) {
        oam.matrixBit3 = 0;
        oam.matrixBit4 = 0;
    } else {
        buf = gOamBuffer;
        n = buf->matrixCount++;
        oam.matrixLo = n;
        oam.matrixBit3 = n >> 3;
        oam.matrixBit4 = n >> 4;

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
    /* `dest` is needed: inlined, the instructions differ */
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
void ScaledSprite::SetPos(screen_ux_t x, screen_uy_t y)
{
    this->x = x;
    this->y = y;
}

/* UNUSED (see SetScaledSpriteColor). Resets the OAM template's
 * attributes FitScaledSprite doesn't own: the object mode, mosaic, color
 * mode and shape bits of attr0 and the priority. */
void ScaledSprite::ResetAttrs()
{
    oam.objMode = OAM_OBJ_MODE_NORMAL;
    oam.mosaic = 0;
    oam.bpp = 0;
    oam.shape = 0;
    oam.priority = 0;
}
