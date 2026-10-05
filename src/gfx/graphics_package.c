#include "core.h"
#include "graphics_package.h"
#include "gfx.h"
#include "gba/gba.h"
#include "system.h"

/* GitHub issue #30. Loads one BG: the palette into bank `paletteBank`,
 * the tiles into char block `charBlock`, and the tilemap into screen
 * block `screenBlock`, ORing the palette bank into every entry. Palettes
 * of more than 0x20 colors switch the BG to 256-color mode. Built with
 * old_agbcc - see docs/matching/issue-30-old-agbcc.md. */
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
    for (y = 0; y < (s32)pkg->height; y++)
    {
        u16 *next = dest + 0x20;
        for (x = 0; x < (s32)pkg->width; x++)
            dest[x] = pal | src[x];
        src += pkg->width;
        dest = next;
    }
    if (map != NULL)
        OperatorDeleteArray(map);
}
/* Zero-fill the trailing halfword, as the ROM does. */
asm(".align 2, 0");

/* GitHub issue #30. Both built with old_agbcc - see
 * docs/matching/issue-30-old-agbcc.md. */

/* The BG control value InitBgSetup built, for REG_BGnCNT. */
u16 GetBgSetupControl(struct bg_setup *self)
{
    return self->ctrl.raw;
}

/* Fills the BG setup buffer: char block, screen block and palette bank
 * verbatim, and a control value with priority `priority`, char base
 * `charBlock`, screen base `screenBlock` and size 0. */
struct bg_setup *InitBgSetup(struct bg_setup *self, u32 charBlock, u32 screenBlock, u32 paletteBank, u32 priority)
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
/* Zero-fill the trailing halfword, as the ROM does. */
asm(".align 2, 0");

/* GitHub issue #30: the sprite-box fitter and its OAM writer. Built with
 * old_agbcc - see docs/matching/issue-30-old-agbcc.md. */

/* gfx.h's `struct oam_attrs` with u16 storage units for attributes 0-1
 * (with gfx.h's u32 units, a DrawScaledSprite store changes).
 * One hardware OAM entry (attr0/attr1/attr2 plus the interleaved affine
 * parameter). matrixNum is split: in affine mode its bits 3-4 double as
 * the h/v-flip bits. */
struct oam_attrs_u16 {
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

/* A sprite box: position, requested size, its OAM template and the
 * preset box it was fitted to. */
struct gfx_box_obj {
    s32 x;                  // 0x00
    s32 y;                  // 0x04
    s32 width;              // 0x08
    s32 height;             // 0x0C
    struct oam_attrs_u16 oam; // 0x10
    s32 sizeIndex;          // 0x18
    u8 unk_1C[4];
    s32 scaleX;             // 0x20 - Q8
    s32 scaleY;             // 0x24 - Q8
};

extern struct oam_shadow_buffer *gOamBuffer;

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
            buf->table[i].attr[3] = param;
            buf->table[i + 1].attr[3] = 0;
            buf->table[i + 2].attr[3] = 0;
            param = self->scaleY;
            buf->table[i + 3].attr[3] = param;
        }
    }
    AddOamEntry(gOamBuffer, &self->oam);
}

/* Fills one 4bpp VRAM tile (`0x06017800`) with a solid color index via a
 * word-sized DMA3 transfer from a stack scratch halfword: `arg1`'s low
 * nibble is replicated into all four nibbles of a 16-bit pattern, and
 * also (rounded down to a multiple of 0x10) packed into the low 4 bits
 * of the "self" scratch buffer's byte +0x15 (alongside its own +0x1c
 * 32-bit field, set verbatim to `arg1`) - the same graphics-package
 * scratch buffer `GetBgSetupControl`/`InitBgSetup` write, extended here with
 * two more fields. */
void sub_801E8F8(u8 *selfArg, s32 arg1)
{
    register u8 *self asm("r4") = selfArg;
    register s32 val asm("r3");
    register s32 aligned asm("r2");
    register u32 mask asm("r1");
    register u32 acc asm("r0");
    u16 buf;
    u32 dadVal;
    vu32 *dma;

    val = arg1;
    *(s32 *)(self + 0x1c) = val;
    aligned = val;
    if (val < 0) {
        aligned += 0xf;
    }
    aligned >>= 4;
    aligned <<= 4;

    mask = 0xf;
    acc = mask;
    asm("" : "+r"(acc));
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

    dadVal = 0x06017800;
    buf = mask;
    dma = (vu32 *)REG_ADDR_DMA3SAD;
    dma[0] = (u32)&buf;
    dma[1] = dadVal;
    dma[2] = 0x81000400;
    dma[2];
}

/* Packs `arg1`'s low 2 bits into bits 2-3 of the same "self" scratch
 * buffer's byte +0x15 that `sub_801E8F8` writes bits 0-3 of (a second,
 * narrower bitfield update on the same byte - callers of this family
 * build up the same 0x10-byte scratch buffer field by field before
 * `LoadGraphicsPackage`).
 *
 * The two `& 3`/`neg`-mask constants land in the ROM's own registers
 * (both in r2, one right after the other - a fresh `mov r2,#0xd`
 * reload, not a reuse of the earlier `#3` value) once the second mask
 * is materialized via the same `mov #N; neg` opaque-asm idiom as
 * `UPDATE_ICON_FRAME_NIBBLE` (src/menus/pause_menu_pages_init.c) instead of
 * a plain C `~0xc`/`-0xd`, which this compiler folds differently. */
void sub_801E950(u8 *self, u32 arg1)
{
    register s32 mask1 asm("r2");
    register u32 shifted asm("r1");
    register s32 mask2 asm("r2");
    register u8 byte asm("r3");

    mask1 = 3;
    shifted = arg1 & mask1;
    shifted = shifted << 2;
    asm volatile("mov %0, #0xd\n\tneg %0, %0" : "=r"(mask2));
    byte = self[0x15];
    mask2 &= byte;
    mask2 |= shifted;
    self[0x15] = mask2;
}

/* Writes the graphics-package "self" scratch buffer's +0x00/+0x04 pair
 * verbatim (a second, narrower constructor alongside `InitBgSetup`'s
 * five-argument one - same buffer, different subset of fields). */
void sub_801E964(u8 *self, u32 arg1, u32 arg2)
{
    *(u32 *)(self + 0) = arg1;
    *(u32 *)(self + 4) = arg2;
}

/* Clears bits 4-9 (masked via -0x11/-0x21/0x3f, i.e. a combined
 * ~0x10 & ~0x20 & 0x3f = 0xf) of the same scratch buffer's byte +0x11,
 * and clears bit 2 (mask -0xd, i.e. ~4) of byte +0x15 - two independent
 * "reset before rebuild" bitfield clears on the same buffer
 * `sub_801E8F8`/`sub_801E950` write. */
void sub_801E96C(u8 *self)
{
    register s32 mask asm("r3") = -0xd;
    register s32 b asm("r1");

    b = mask;
    asm("" : "+r"(b));
    b &= self[0x11];
    b &= -0x11;
    b &= -0x21;
    b &= 0x3f;
    self[0x11] = b;
    mask &= self[0x15];
    self[0x15] = mask;
}
/* This object is the last thing linked before the still-raw
 * asm/code_3_2_17_1e990.s continuation, which starts at a 4-byte-aligned
 * ROM address (0x0801E990) two bytes past sub_801E96C's own end
 * (0x0801E98E) - the ROM pads that gap with zero bytes (a real
 * `.align 2, 0` in the original assembly), not this compiler's default
 * Thumb NOP-fill (`0x46C0`) for an implicit end-of-object alignment. See
 * the matching_decomp_alignment_fix technique. */
asm(".align 2, 0");
