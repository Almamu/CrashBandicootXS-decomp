#include "core.h"

/* GitHub issue #30: the sprite-box fitter and its OAM writer. Built with
 * old_agbcc - see docs/matching/issue-30-old-agbcc.md. */

/* One hardware OAM entry (attr0/attr1/attr2 plus the interleaved affine
 * parameter). matrixNum is split: in affine mode its bits 3-4 double as
 * the h/v-flip bits. */
struct oam_attrs {
    u16 y:8;            // 0x00
    u16 affineMode:2;
    u16 objMode:2;
    u16 mosaic:1;
    u16 bpp:1;
    u16 shape:2;
    u32 x:9;            // 0x02 - u32: with u16, DrawScaledSprite's stores schedule differently
    u16 matrixNumLo:3;
    u16 hFlip:1;
    u16 vFlip:1;
    u16 size:2;
    u16 tileNum:10;     // 0x04
    u16 priority:2;
    u16 paletteNum:4;
    s16 affineParam;    // 0x06
};

/* Same 0x40C-byte OAM shadow buffer `src/graphics/graphics.c` already
 * names `struct oam_shadow_buffer`. */
struct oam_shadow_buffer {
    s32 count;
    s32 base;
    s32 matrixCount;        // 0x08
    struct oam_attrs oam[0x80];
};

/* A sprite box: position, requested size, its OAM template and the
 * preset box it was fitted to. */
struct gfx_box_obj {
    s32 x;                  // 0x00
    s32 y;                  // 0x04
    s32 width;              // 0x08
    s32 height;             // 0x0C
    struct oam_attrs oam;   // 0x10
    s32 sizeIndex;          // 0x18
    u8 unk_1C[4];
    s32 scaleX;             // 0x20 - Q8
    s32 scaleY;             // 0x24 - Q8
};

extern struct oam_shadow_buffer *gOamBuffer;
extern void AddOamEntry(struct oam_shadow_buffer *buf, struct oam_attrs *oam);

extern s32 gObjSizeWidths[12];
extern s32 gObjSizeHeights[12];

/* Picks the smallest-area box preset (gObjSizeWidths/674) that a
 * width x height box fits in at 50% zoom or better, puts its shape/size
 * and an area-derived tile number into the OAM template, and stores the
 * Q8 scale factors plus the affine mode (3 shrunk, 1 enlarged, 0 1:1). */
/* UNUSED - no caller anywhere in the ROM (no Thumb `bl` to it and no
 * pointer to it in baserom.gba, nor any reference in asm/ or src/). */
void FitScaledSprite(struct gfx_box_obj *self, s32 width, s32 height)
{
    s32 *widths;
    s32 *heights;
    s32 best;
    s32 i;
    s32 idx;

    self->width = width;
    self->height = height;
    best = 0x1000;
    for (i = 0, widths = gObjSizeWidths, heights = gObjSizeHeights; i < 12; i++)
    {
        if (width <= widths[i] * 2 && height <= heights[i] * 2 && widths[i] * heights[i] < best)
        {
            best = widths[i] * heights[i];
            self->sizeIndex = i;
        }
    }
    idx = self->sizeIndex;
    self->oam.size = idx;
    self->oam.shape = idx >> 2;
    self->oam.tileNum = 0x400 - best / 32;
    self->scaleX = (widths[idx] << 8) / width;
    self->scaleY = (heights[idx] << 8) / height;
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
    struct oam_shadow_buffer *buf;
    s32 n;

    switch ((u32)self->oam.affineMode)
    {
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
    if (self->oam.affineMode == 0)
    {
        self->oam.hFlip = 0;
        self->oam.vFlip = 0;
    }
    else
    {
        buf = gOamBuffer;
        n = buf->matrixCount++;
        self->oam.matrixNumLo = n;
        self->oam.hFlip = n >> 3;
        self->oam.vFlip = n >> 4;
        {
            /* Read through a u16 local: the ROM loads each scale with ldrh
             * ahead of its store address. */
            u16 param = self->scaleX;
            s32 i = n * 4;
            buf->oam[i].affineParam = param;
            buf->oam[i + 1].affineParam = 0;
            buf->oam[i + 2].affineParam = 0;
            param = self->scaleY;
            buf->oam[i + 3].affineParam = param;
        }
    }
    AddOamEntry(gOamBuffer, &self->oam);
}
