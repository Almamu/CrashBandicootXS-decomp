#include "menus.hpp"
#include "font.hpp"
#include "audio.hpp"

extern "C" {
#include "system.h"
#include <agb_syscall.h>
#include "text.h"
#include "util.h"
#include "level.h"
#include "globals.h"
}

/* The pause menu (PauseMenu, menus.hpp; docs/rom_map.md's "overlay_ui"
 * section). Run runs the whole screen to completion: frees the pending
 * heap bytes, stops the ambient sounds, clears palette color 0 and
 * DISPCNT, swaps gPaletteCache for a new palette cache with the menu's
 * palette in slot 15, sets up both fonts (each one's tiles uploaded, the
 * two fonts' tiles reserved in OBJ VRAM), builds the menu, runs its loop
 * (Loop, pause_menu_loop.cpp) and deletes it, then restores the old palette
 * cache and returns the loop's result.
 *
 * The ROM builds the font's 0x12c offset again for each `tileCount` read:
 * reading it through the inline Font::GetTileCount stops CSE from sharing
 * it. The
 * two reads for the VRAM reservation are taken into locals before
 * `gObjVramCursor` is loaded, and the palette cache into a local before
 * `gPauseMenuPalette`'s address, for the ROM's load order. */

static inline void reserve_icon_vram(u32 n)
{
    gObjVramCursor->baseTile = n;
    gObjVramCursor->Reset();
}

s32 PauseMenu::Run()
{
    PaletteCache *oldCache;
    PauseMenu *screen;
    s32 result;

    mem_free_bytes(MEM_HEAP_BOTH);
    gAudioContext->StopAmbientSfx();
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;

    oldCache = gPaletteCache;
    gPaletteCache = (PaletteCache *)new PaletteCache;
    gPaletteCache->SetSource(gSpriteBankTable.paletteCount, gSpriteBankTable.palettes);
    gPaletteCache->ClaimSlot(0xf);
    {
        PaletteCache *cache = gPaletteCache;

        CpuSet(gPauseMenuPalette, cache->slots[15], 0x10);
    }

    gSmallFont->ResetPalette();
    gLargeFont->ResetPalette();
    gSmallFont->SetTileBase(0);
    gLargeFont->SetTileBase(gSmallFont->GetTileCount());
    {
        u32 a = gSmallFont->GetTileCount();
        u32 b = gLargeFont->GetTileCount();

        gObjVramCursor->baseTile = a + b;
        gObjVramCursor->Reset();
    }

    screen = new PauseMenu;
    result = screen->Loop();
    delete screen;

    reserve_icon_vram(0);
    delete gPaletteCache;
    gPaletteCache = oldCache;
    mem_free_bytes(MEM_HEAP_BOTH);
    return result;
}

/* The constructor: alpha blending at full fade (BLDY 16), BG0 and the
 * OBJs on, the background, the save block, the info pages (InitInfo),
 * the blinking eyes (sprite bank 0x228, at (238, 188) / 2), then the
 * rows (five in a time trial: "restart trial") and BG0's registers. */
PauseMenu::PauseMenu() : bg(0, 0x1f, 0, 3)
{
    blend.raw = 0;
    blend.bits.effect = 3;
    blend.bits.bdFirst = 1;
    blend.bits.bg0First = 1;
    blend.bits.bg1First = 1;
    blend.bits.bg2First = 1;
    blend.bits.bg3First = 1;
    blend.bits.objFirst = 1;
    bldy.evy = 16;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    *(vu16 *)REG_ADDR_BLDY = bldy.evy;
    dispcnt.raw = 0;
    dispcnt.bits.objMap1D = 1;
    dispcnt.bits.mode = 0;
    dispcnt.bits.bg0 = 1;
    dispcnt.bits.obj = 1;
    bg.Load(&gPauseMenuBg);
    progress = PackSaveData(gLevelState);
    InitInfo();
    {
        UiSprite *s;

        blinkEyes = new UiSprite;
        s = blinkEyes;
        SetIconBank(s, 0x8a << 2);
        blinkEyes->palette = s->GetAnimPaletteSlot();
    }
    blinkEyes->SetPixelPos(119, 94);
    blinkTimer = (u16)RandRange(0x78) + 0x78;
    rows = gPauseMenuRows;
    cursor = 0;
    if (gLevelState->timeTrial != 0)
        rowCount = 5;
    else
        rowCount = 4;
    rowSpacing = 0x10;
    page = 0;
    pageTimer = 0xb4;
    REG_BG0CNT = bg.GetControl();
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
}

/* The destructor deletes the icons: the eyes, the time trial's medal, the
 * relics', gems' and powers' icons and the crystal. Each loop has its own
 * counter: with one shared counter, the counter and the strength-reduced
 * pointer swap r4 and r5. */
PauseMenu::~PauseMenu()
{
    delete blinkEyes;
    delete trialIcon;
    for (s32 i = 0; i < 3; i++)
        delete relicIcons[i];
    for (s32 j = 0; j < 5; j++)
        delete gemIcons[j];
    for (s32 k = 0; k < 4; k++)
        delete powerIcons[k];
    delete crystalIcon;
}
