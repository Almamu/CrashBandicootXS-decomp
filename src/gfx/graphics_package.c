#include "core.h"
#include "math_util.h"
#include "match.h"
#include "graphics_package.h"
#include "gfx.h"
#include "gba/gba.h"
#include "system.h"
#include "globals.h"

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
    map = OperatorNewArray(*(u32 *)pkg->mapAsset >> 9 << 1);
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
    if (map != NULL)
        OperatorDeleteArray(map);
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

/* gfx.h's `struct oam_attrs` with u16 storage units for attributes 0-1
 * (with gfx.h's u32 units, a DrawScaledSprite store changes).
 * One hardware OAM entry (attr0/attr1/attr2 plus the interleaved affine
 * parameter). matrixNum is split: in affine mode its bits 3-4 double as
 * the h/v-flip bits. */
struct oam_attrs_u16 {
    u16 y:8; // 0x00
    u16 affineMode:2;
    u16 objMode:2;
    u16 mosaic:1;
    u16 bpp:1;
    u16 shape:2;
    u32 x:9; // 0x02 - u32: with u16, DrawScaledSprite's stores schedule differently
    u16 matrixNumLo:3;
    u16 hFlip:1;
    u16 vFlip:1;
    u16 size:2;
    u16 tileNum:10; // 0x04
    u16 priority:2;
    u16 paletteNum:4;
    s16 affineParam; // 0x06
};

/* A sprite box: position, requested size, its OAM template and the
 * preset box it was fitted to. */
struct gfx_box_obj {
    s32 x;                    // 0x00
    s32 y;                    // 0x04
    s32 width;                // 0x08
    s32 height;               // 0x0C
    struct oam_attrs_u16 oam; // 0x10
    s32 sizeIndex;            // 0x18
    s32 color;                // 0x1C - SetScaledSpriteColor
    s32 scaleX;               // 0x20 - Q8
    s32 scaleY;               // 0x24 - Q8
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
    struct oam_shadow_buffer *buf;
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
        self->oam.hFlip = 0;
        self->oam.vFlip = 0;
    } else {
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
            buf->table[i].attr[3] = param;
            buf->table[i + 1].attr[3] = 0;
            buf->table[i + 2].attr[3] = 0;
            param = self->scaleY;
            buf->table[i + 3].attr[3] = param;
        }
    }
    AddOamEntry(gOamBuffer, &self->oam);
}

/* UNUSED - no caller anywhere in the ROM (no `bl` in expected/*.s, no
 * pointer to it in baserom.gba), like the other three setters below and
 * FitScaledSprite/DrawScaledSprite above, whose `struct gfx_box_obj` they
 * all take (as a byte pointer). Sets the sprite box's solid color `arg1`
 * (a 256-color index): stores it in `color` (+0x1c), puts its palette
 * bank (`arg1 >> 4`) in the OAM template's palette bits (+0x15, attr2
 * bits 12-15), and fills OBJ tile 0x3C0 (`0x06017800`) with the color's
 * low nibble through a 16-bit fixed-source DMA3 transfer. */
void SetScaledSpriteColor(u8 *selfArg, s32 arg1)
{
    MATCH_HOLD_REG(u8 *, self, r4) = selfArg;
    MATCH_HOLD_REG(s32, val, r3);
    MATCH_HOLD_REG(s32, aligned, r2);
    MATCH_HOLD_REG(u32, mask, r1);
    MATCH_HOLD_REG(u32, acc, r0);
    u16 buf;
    u32 dadVal;
    vu32 *dma;

    val = arg1;
    ((struct gfx_box_obj *)self)->color = val;
    aligned = val;
    if (val < 0) {
        aligned += 0xf;
    }
    aligned >>= 4;
    aligned <<= 4;

    mask = 0xf;
    acc = mask;
    MATCH_KEEP(acc);
    acc &= self[0x15];
    acc |= aligned;
    self[0x15] = acc;

    mask &= val;
    acc = mask << 4;
    aligned = mask << 8;
    acc |= aligned;
    aligned = mask << 0xc;
    acc |= aligned;
    mask |= acc;

    dadVal = (u32)(OBJ_VRAM0 + 0x3C0 * TILE_SIZE_4BPP);
    buf = mask;
    dma = (vu32 *)REG_ADDR_DMA3SAD;
    dma[0] = (u32)&buf;
    dma[1] = dadVal;
    dma[2] = 0x81000400;
    dma[2];
}

/* UNUSED (see SetScaledSpriteColor). Sets the sprite box's OAM priority:
 * `arg1`'s low 2 bits into bits 2-3 of the template's byte +0x15 (attr2
 * bits 10-11).
 *
 * The two `& 3`/`neg`-mask constants land in the ROM's own registers
 * (both in r2, one right after the other - a fresh `mov r2,#0xd`
 * reload, not a reuse of the earlier `#3` value) once the second mask
 * is materialized via an opaque `mov #N; neg` asm idiom (the pause
 * menu's icon constructors used the same one while they were C) instead
 * of a plain C `~0xc`/`-0xd`, which this compiler folds differently. */
void SetScaledSpritePriority(u8 *self, u32 arg1)
{
    MATCH_HOLD_REG(s32, mask1, r2);
    MATCH_HOLD_REG(u32, shifted, r1);
    MATCH_HOLD_REG(s32, mask2, r2);
    MATCH_HOLD_REG(u8, byte, r3);

    mask1 = 3;
    shifted = arg1 & mask1;
    shifted = shifted << 2;
    asm volatile("mov %0, #0xd\n\tneg %0, %0" : "=r"(mask2));
    byte = self[0x15];
    mask2 &= byte;
    mask2 |= shifted;
    self[0x15] = mask2;
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
 * mode and shape bits of attr0 (byte +0x11 masked with ~0xc, ~0x10,
 * ~0x20 and 0x3f, i.e. bits 10-15 cleared) and the priority (byte +0x15
 * masked with ~0xc). */
void ResetScaledSpriteAttrs(u8 *self)
{
    MATCH_HOLD_REG(s32, mask, r3) = -0xd;
    MATCH_HOLD_REG(s32, b, r1);

    b = mask;
    MATCH_KEEP(b);
    b &= self[0x11];
    b &= -0x11;
    b &= -0x21;
    b &= 0x3f;
    self[0x11] = b;
    mask &= self[0x15];
    self[0x15] = mask;
}
