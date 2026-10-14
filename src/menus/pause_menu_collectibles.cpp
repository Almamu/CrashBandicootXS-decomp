#include "menus.hpp"
#include "engine/text/font.hpp"

extern "C" {
#include "core.h"
#include "engine/text/text.h"
#include "engine/sys/system.h"
}

using Game::Ui::PauseMenu;

/* PauseMenu's collectible pages (menus.hpp; C++ since the #664 cleanup):
 * DrawPowersPage, DrawGemsPage and DrawRelicsPage, between Draw's helpers
 * (pause_menu_draw.cpp) and InitInfo (pause_menu_info.cpp) in the ROM.
 * They were pause_menu_powers.cpp and pause_menu_gems.cpp until #771. */

/* PauseMenu::DrawPowersPage: the
 * icons of the powers the player has (PROGRESS_FLAG_*),
 * or, with none, a centred "no powers" label (text 0x3a).
 *
 * Built with old_agbcp (Makefile OLD_AGBCC_OBJS; agbcc as C, where 12
 * register pins made agbcc load the mask before the flag byte): the old
 * compiler's constant-before-ldrb order is the ROM's. */
void PauseMenu::DrawPowersPage()
{
    s32 none = 1;

    if (progress->flags & PROGRESS_FLAG_SUPER_BODY_SLAM) {
        powerIcons[0]->DrawWithOffset(0, 0);
        none = 0;
    }
    if (progress->flags & PROGRESS_FLAG_DOUBLE_JUMP) {
        powerIcons[1]->DrawWithOffset(0, 0);
        none = 0;
    }
    if (progress->flags & PROGRESS_FLAG_TORNADO_SPIN) {
        powerIcons[2]->DrawWithOffset(0, 0);
        none = 0;
    }
    if (progress->flags & PROGRESS_FLAG_TURBO_RUN) {
        powerIcons[3]->DrawWithOffset(0, 0);
        none = 0;
    }

    if (none) {
        const u8 *label = GetUiText(0x3a);
        /* `width` is needed: inlined, the instructions differ */
        u32 width = gSmallFont->MeasureText(label);

        gSmallFont->SetPos(DISPLAY_ART_X + 0xc2 - (width >> 1), DISPLAY_ART_Y + 0x64);
        gSmallFont->DrawText(label);
    }
}

/* DrawGemsPage and DrawRelicsPage: built with old_agbcp (old_agbcc as C),
 * whose mask-before-ldrb order shows in DrawGemsPage's flag tests. */

/* The gems page: the four coloured gems the player has (PROGRESS_FLAG_*),
 * the clear gem, the clear gems found and the gems found out
 * of the total. */
void PauseMenu::DrawGemsPage()
{
    if (progress->flags & PROGRESS_FLAG_RED_GEM)
        gemIcons[1]->DrawWithOffset(0, 0);
    if (progress->flags & PROGRESS_FLAG_GREEN_GEM)
        gemIcons[2]->DrawWithOffset(0, 0);
    if (progress->flags & PROGRESS_FLAG_BLUE_GEM)
        gemIcons[3]->DrawWithOffset(0, 0);
    if (progress->flags & PROGRESS_FLAG_YELLOW_GEM)
        gemIcons[4]->DrawWithOffset(0, 0);
    gemIcons[0]->DrawWithOffset(0, 0);

    gSmallFont->SetPos(gPauseGemIconPos[0].x - 0x14, gPauseGemIconPos[0].y - 4);
    gSmallFont->DrawText(clearGemCount);
    gSmallFont->SetPos(DISPLAY_ART_X + 0xb4, DISPLAY_ART_Y + 0x80);
    DrawFraction(gemCount, gemTotal);
}

/* The relics page: the three relic icons, each kind's count under it,
 * and the relics found out of the total. */
void PauseMenu::DrawRelicsPage()
{
    s32 i;

    for (i = 0; i <= 2; i++)
        relicIcons[i]->DrawWithOffset(0, 0);

    gSmallFont->SetPos(gPauseRelicIconPos[2].x - 4, gPauseRelicIconPos[2].y + 0xe);
    gSmallFont->DrawText(sapphireCount);
    gSmallFont->SetPos(gPauseRelicIconPos[1].x - 4, gPauseRelicIconPos[1].y + 0xe);
    gSmallFont->DrawText(goldCount);
    gSmallFont->SetPos(gPauseRelicIconPos[0].x - 4, gPauseRelicIconPos[0].y + 0xe);
    gSmallFont->DrawText(platinumCount);
    gSmallFont->SetPos(DISPLAY_ART_X + 0xb4, DISPLAY_ART_Y + 0x80);
    DrawFraction(relicCount, relicTotal);
}
