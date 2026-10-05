#include "core.h"
#include "graphics_package.h"

extern void LoadTaggedAsset(void *asset, void *dest);
extern void *OperatorNewArray(u32 size);
extern void OperatorDeleteArray(void *ptr);

/* GitHub issue #30. Loads one BG: the palette into bank `paletteBank`,
 * the tiles into char block `charBlock`, and the tilemap into screen
 * block `screenBlock`, ORing the palette bank into every entry. Palettes
 * of more than 0x20 colors switch the BG to 256-color mode. Built with
 * old_agbcc - see docs/matching/issue-30-old-agbcc.md. */
void LoadGraphicsPackage(struct bg_setup *self, struct bg_package *pkg)
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
