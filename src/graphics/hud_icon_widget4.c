#include "core.h"
#include "bitmap_font.h"

/* Sits between FontMeasureText (src/graphics/hud_icon_widget_8994.c) and
 * InitFont (src/graphics/hud_icon_widget_8a78.c) - FontUploadTiles/
 * FontSetPalette/FontResetPalette, GitHub issue #46. Same `struct bitmap_font`
 * as hud_icon_widget.c/hud_icon_widget2.c/hud_icon_widget3.c/
 * hud_icon_widget5.c. */

extern void LoadTaggedAsset(void *asset, void *dest);
extern u8 *gPaletteCache;
extern void ***gSpriteBankSet;
extern s32 GetPaletteSlot(u8 *cache, s32 recordId);

/* Uploads `tiles`'s referenced tile data to the OBJ VRAM slot
 * selected by `tileBase`, recording the resulting tile-count-derived
 * shift (`>>13` of the asset's own header word) into `tileCount`. */
void FontUploadTiles(struct bitmap_font *self)
{
    void *asset = self->tiles;

    self->tileCount = *(u32 *)asset >> 13;
    LoadTaggedAsset(asset, (void *)(0x06010000 + (self->tileBase << 5)));
}

/* Sets the low nibble of `oam_scratch[5]` from `val`'s low byte - a
 * priority/attribute nibble selector, exact meaning not established. */
void FontSetPalette(struct bitmap_font *self, u8 val)
{
    u32 shifted;
    register u8 mask asm("r2");
    register u8 field asm("r3");

    shifted = val << 4;
    mask = 0xF;
    asm volatile("" : "+r"(mask));
    field = self->oam_scratch[5];
    mask &= field;
    mask |= shifted;
    self->oam_scratch[5] = mask;
}

/* Looks up a tile-cache slot for the byte at
 * `(**gSpriteBankSet)[0x1A4]`'s own `+0x14` field (see
 * docs/rom_map.md's `gSpriteBankTable` investigation) via
 * `GetPaletteSlot`, and folds the result into the same `oam_scratch[5]`
 * nibble FontSetPalette sets above. */
void FontResetPalette(struct bitmap_font *self, u32 unused)
{
    u8 *cache = gPaletteCache;
    void *rec = *(void **)((u8 *)(**gSpriteBankSet) + (0xD2 << 1));
    u8 field = ((u8 *)rec)[0x14];
    s32 slot = GetPaletteSlot(cache, field);
    u32 shifted = slot << 4;
    register u8 mask asm("r1");
    register u8 b asm("r2");

    mask = 0xF;
    asm volatile("" : "+r"(mask));
    b = self->oam_scratch[5];
    mask &= b;
    mask |= shifted;
    self->oam_scratch[5] = mask;
}
