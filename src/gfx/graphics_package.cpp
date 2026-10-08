#include "sprite_obj.hpp"
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
 * OperatorDeleteArray). The functions keep C linkage; the BG setup and
 * the sprite box are plain structs (graphics_package.h, below). */

/* GitHub issue #30. Loads one BG: the palette into bank `paletteBank`,
 * the tiles into char block `charBlock`, and the tilemap into screen
 * block `screenBlock`, ORing the palette bank into every entry. Palettes
 * of more than 0x20 colors switch the BG to 256-color mode. Built with
 * old_agbcc - see docs/matching/archive/issue-30-old-agbcc.md. */
void LoadGraphicsPackage(struct bg_setup *self, const struct bg_package *pkg)
{
    u16 *map;
    u16 *src;
    u16 *dest;
    s32 pal;
    s32 x;
    s32 y;

    if (*(u32 *)pkg->paletteAsset >> 8 <= 0x20)
        self->ctrl.bits.colorMode = 0;
    else
        self->ctrl.bits.colorMode = 1;
    LoadTaggedAsset(pkg->paletteAsset, (void *)(PLTT + (self->paletteBank << 5)));
    LoadTaggedAsset(pkg->tileAsset, (void *)(VRAM + (self->charBlock << 14)));
    map = new u16[*(u32 *)pkg->mapAsset >> 9];
    LoadTaggedAsset(pkg->mapAsset, map);
    pal = self->paletteBank << 12;
    src = map;
    dest = (u16 *)(VRAM + (self->screenBlock << 11));
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
u16 GetBgSetupControl(struct bg_setup *self)
{
    return self->ctrl.raw;
}

/* Fills the BG setup buffer: char block, screen block and palette bank
 * verbatim, and a control value with priority `priority`, char base
 * `charBlock`, screen base `screenBlock` and size 0. */
struct bg_setup *InitBgSetup(struct bg_setup *self, u32 charBlock, u32 screenBlock, u32 paletteBank,
                             u32 priority)
{
    self->ctrl.raw = 0;
    self->ctrl.bits.priority = priority;
    self->charBlock = charBlock;
    self->ctrl.bits.charBase = charBlock;
    self->ctrl.bits.size = 0;
    self->screenBlock = screenBlock;
    self->ctrl.bits.screenBase = screenBlock;
    self->paletteBank = paletteBank;
    return self;
}

/* GitHub issue #30: the sprite-box fitter and its OAM writer. Built with
 * old_agbcc - see docs/matching/archive/issue-30-old-agbcc.md. */

/* A sprite box: position, requested size, its OAM template and the
 * preset box it was fitted to. */
struct gfx_box_obj {
    s32 x;                // 0x00
    s32 y;                // 0x04
    s32 width;            // 0x08
    s32 height;           // 0x0C
    struct oam_attrs oam; // 0x10 (gfx.h)
    s32 sizeIndex;        // 0x18
    s32 color;            // 0x1C - SetScaledSpriteColor
    s32 scaleX;           // 0x20 - Q8
    s32 scaleY;           // 0x24 - Q8
};

/* Picks the smallest-area box preset (gObjSizeWidths/674) that a
 * width x height box fits in at 50% zoom or better, puts its shape/size
 * and an area-derived tile number into the OAM template, and stores the
 * Q8 scale factors plus the affine mode (3 shrunk, 1 enlarged, 0 1:1). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void FitScaledSprite(struct gfx_box_obj *self, s32 width, s32 height)
{
    const s32 *widths;
    const s32 *heights;
    s32 best;
    s32 i;
    s32 idx;

    self->width = width;
    self->height = height;
    best = 0x1000;
    for (i = 0, widths = gObjSizeWidths, heights = gObjSizeHeights; i < 12; i++) {
        if (width <= widths[i] * 2 && height <= heights[i] * 2 && widths[i] * heights[i] < best) {
            best = widths[i] * heights[i];
            self->sizeIndex = i;
        }
    }
    idx = self->sizeIndex;
    self->oam.size = idx;
    self->oam.shape = idx >> 2;
    self->oam.tileNum = 0x400 - best / 32;
    self->scaleX = Q8_DIV(widths[idx], width);
    self->scaleY = Q8_DIV(heights[idx], height);
    if (self->scaleX < 0x100 || self->scaleY < 0x100)
        self->oam.affineMode = 3;
    else if (self->scaleX > 0x100 || self->scaleY > 0x100)
        self->oam.affineMode = 1;
    else
        self->oam.affineMode = 0;
}

/* Positions the OAM template for the box's affine mode (centred on the
 * preset box when scaled, or on the double-size area), allocates an
 * affine matrix holding the scale factors when affine, and queues the
 * entry into the shadow OAM buffer. */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void DrawScaledSprite(struct gfx_box_obj *self)
{
    OamBuffer *buf;
    s32 n;

    switch ((u32)self->oam.affineMode) {
    case 0:
        self->oam.x = self->x;
        self->oam.y = self->y;
        break;
    case 1:
        self->oam.x = self->x - (gObjSizeWidths[self->sizeIndex] - self->width) / 2;
        self->oam.y = self->y - (gObjSizeHeights[self->sizeIndex] - self->height) / 2;
        break;
    case 3:
        self->oam.x = self->x + self->width / 2 - gObjSizeWidths[self->sizeIndex];
        self->oam.y = self->y + self->height / 2 - gObjSizeHeights[self->sizeIndex];
        break;
    }
    if (self->oam.affineMode == 0) {
        self->oam.matrixBit3 = 0;
        self->oam.matrixBit4 = 0;
    } else {
        buf = gOamBuffer;
        n = buf->matrixCount++;
        self->oam.matrixLo = n;
        self->oam.matrixBit3 = n >> 3;
        self->oam.matrixBit4 = n >> 4;
        {
            /* Read through a u16 local: the ROM loads each scale with ldrh
             * ahead of its store address. */
            u16 param = self->scaleX;
            s32 i = n * 4;
            buf->table[i].attr[3] = param;
            buf->table[i + 1].attr[3] = 0;
            buf->table[i + 2].attr[3] = 0;
            param = self->scaleY;
            buf->table[i + 3].attr[3] = param;
        }
    }
    gOamBuffer->Add(&self->oam);
}

/* UNUSED - no caller anywhere in the ROM (no `bl` in expected/*.s, no
 * pointer to it in baserom.gba), like the other three setters below and
 * FitScaledSprite/DrawScaledSprite above, whose `struct gfx_box_obj` they
 * all take. Sets the sprite box's solid color `color` (a 256-color
 * index): stores it in `color` (+0x1c), puts its palette bank
 * (`color / 16`) in the OAM template's palette bits (attr2 bits 12-15),
 * and fills 64 OBJ tiles from tile 0x3C0 (`0x06017800`) with the color's
 * low nibble through a 16-bit fixed-source DMA3 transfer. */
void SetScaledSpriteColor(struct gfx_box_obj *self, s32 color)
{
    u32 nibble;
    u32 dest;

    self->color = color;
    self->oam.palette = color / 16;
    nibble = color & 0xf;
    nibble |= nibble << 4 | nibble << 8 | nibble << 12;
    dest = (u32)(OBJ_VRAM0 + 0x3C0 * TILE_SIZE_4BPP);
    DmaFill16(3, nibble, dest, 0x800);
}

/* UNUSED (see SetScaledSpriteColor). Sets the sprite box's OAM priority
 * (attr2 bits 10-11). */
void SetScaledSpritePriority(struct gfx_box_obj *self, u32 priority)
{
    self->oam.priority = priority;
}

/* UNUSED (see SetScaledSpriteColor). Sets the sprite box's position
 * (`x`/`y`, +0x00/+0x04). */
void SetScaledSpritePos(struct gfx_box_obj *self, u32 arg1, u32 arg2)
{
    self->x = arg1;
    self->y = arg2;
}

/* UNUSED (see SetScaledSpriteColor). Resets the OAM template's
 * attributes FitScaledSprite doesn't own: the object mode, mosaic, color
 * mode and shape bits of attr0 and the priority. */
void ResetScaledSpriteAttrs(struct gfx_box_obj *self)
{
    self->oam.objMode = 0;
    self->oam.mosaic = 0;
    self->oam.bpp = 0;
    self->oam.shape = 0;
    self->oam.priority = 0;
}
