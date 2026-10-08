#include "frontend.hpp"

extern "C" {
#include "vram_pool.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* The rest of LanguageSelect's methods (#664, part 10b,
 * include/frontend.hpp; language_select.cpp has the others).
 *
 * old_agbcp (Makefile OLD_AGBCC_OBJS): LoadBg's DISPCNT bitfield stores
 * load each constant before the byte, as old_agbcc does. The C was built
 * with agbcc and pinned two registers to get that order. */

/* The screen's DISPCNT (mode 1, 1D OBJ mapping, BG0 and OBJ on) and its
 * background on BG0: the sky (gMenuSkyBg), unscrolled. */
void LanguageSelect::LoadBg()
{
    struct bg_setup buf;
    u32 zero = 0;

    dispcnt.raw = zero;
    dispcnt.bits.objMap1D = 1;
    dispcnt.bits.mode = 1;
    dispcnt.bits.bg0 = 1;
    dispcnt.bits.bg1 = 0;
    dispcnt.bits.obj = 1;

    InitBgSetup(&buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(&buf, &gMenuSkyBg);
    REG_BG0CNT = GetBgSetupControl(&buf);
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

s32 LanguageSelect::Blink()
{
    if ((frame >> 2) & 1) {
        return 1;
    }
    return 2;
}

void LanguageSelect::CommitFrame()
{
    REG_DISPCNT = dispcnt.raw;
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
}

/* The starfield goes with the screen. */
LanguageSelect::~LanguageSelect()
{
    delete starfield;
}

/* Loads the screen's graphics and background, starts the starfield and
 * fades in. */
LanguageSelect::LanguageSelect()
{
    FreeUnlockedPaletteSlots(gPaletteCache);
    InitGraphics();
    LoadBg();
    starfield = new Starfield;
    FadeBrightness(FADE_FLAG_IN, 1, 0);
    SetObjMapping1D();
    ShowObj();
    SetDispcntMode(1);
    CommitDispcnt();
}

/* gLanguageSelect's lifetime, around MainLoop's LanguageSelect::Run. */
void LanguageSelect::Close()
{
    FadeBrightness(0, 1, 0);
    delete gLanguageSelect;
    gLanguageSelect = 0;
    FreeUnlockedPaletteSlots(gPaletteCache);
}

void LanguageSelect::Open()
{
    FreeUnlockedPaletteSlots(gPaletteCache);
    gLanguageSelect = new LanguageSelect;
}
