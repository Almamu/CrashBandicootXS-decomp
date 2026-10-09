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
