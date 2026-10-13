#include "menus.hpp"
#include "vram_dma_queue.hpp"
#include "font.hpp"
#include "frame_limiter.hpp"

extern "C" {
#include "core.h"
#include "gba/defines.h"
#include "text.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
}

/* PauseMenu's time trial and crystals pages, its page title and its
 * frame commit (menus.hpp; C++ since the #664 cleanup). The font draws
 * are Font's virtual MeasureText and DrawText (the C spelled them out as
 * hand-written method-table slot calls); see
 * docs/matching/archive/issue-4-6-8-naked-retry.md. */

/* The time trial page: the medal, if earned, and the trial's time
 * centred over it. */
void PauseMenu::DrawTimeTrialPage()
{
    u32 w;

    if (trialEarned)
        trialIcon->DrawWithOffset(0, 0);
    w = gSmallFont->MeasureText(timeText);
    gSmallFont->SetPos(gPauseTimeTrialIconPos.x - (w >> 1) - 2, gPauseTimeTrialIconPos.y - 0x23);
    gSmallFont->DrawText(timeText);
}

/* The crystals page: the crystal and the crystals found out of 20. */
void PauseMenu::DrawCrystalsPage()
{
    crystalIcon->DrawWithOffset(0, 0);
    gSmallFont->SetPos(gPauseCrystalIconPos.x - 0x2c, gPauseCrystalIconPos.y - 8);
    DrawFraction(crystalCount, crystalTotal);
}

/* The info page's title (gPauseMenuPageTitles[page]), centred at
 * (0xc2, 0x2c). */
void PauseMenu::DrawPageTitle()
{
    const u8 *label = GetUiText(gPauseMenuPageTitles[page]);
    /* `w` is needed: inlined, the instructions differ */
    u32 w = gSmallFont->MeasureText(label);

    gSmallFont->SetPos(DISPLAY_ART_X + 0xc2 - (w >> 1), DISPLAY_ART_Y + 0x2c);
    gSmallFont->DrawText(label);
}

/* Waits for VBlank and commits the frame: the palette cache, the OAM
 * buffer, the VRAM DMA queue, the backdrop colour, and the BLDCNT, BLDY
 * and DISPCNT shadows. */
void PauseMenu::CommitFrame()
{
    FrameLimiter::WaitForVBlank();
    gPaletteCache->Upload();
    gOamBuffer->Commit();
    VramDmaQueue::Flush();
    BACKDROP_COLOR = 0;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    *(vu16 *)REG_ADDR_BLDY = bldy.evy;
    *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
}
