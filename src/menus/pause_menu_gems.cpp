#include "menus.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "text.h"
}

/* PauseMenu's DrawGemsPage and DrawRelicsPage (menus.hpp; C++ since the
 * #664 cleanup), between DrawPowersPage (pause_menu_powers.cpp) and
 * InitInfo (pause_menu_info.cpp) in the ROM.
 *
 * Built with old_agbcp (Makefile OLD_AGBCC_OBJS; old_agbcc as C): its
 * mask-before-ldrb order shows in DrawGemsPage's flag tests. */

/* The gems page: the four coloured gems the player has (flag bits
 * 1/4/8/2), the clear gem, the clear gems found and the gems found out
 * of the total. */
void PauseMenu::DrawGemsPage()
{
    if (progress->flags & 1)
        gemIcons[1]->DrawWithOffset(0, 0);
    if (progress->flags & 4)
        gemIcons[2]->DrawWithOffset(0, 0);
    if (progress->flags & 8)
        gemIcons[3]->DrawWithOffset(0, 0);
    if (progress->flags & 2)
        gemIcons[4]->DrawWithOffset(0, 0);
    gemIcons[0]->DrawWithOffset(0, 0);

    gSmallFont->SetPos(gPauseGemIconPos[0].x - 0x14, gPauseGemIconPos[0].y - 4);
    gSmallFont->DrawText(clearGemCount);
    gSmallFont->SetPos(0xb4, 0x80);
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
    gSmallFont->SetPos(0xb4, 0x80);
    DrawFraction(relicCount, relicTotal);
}
