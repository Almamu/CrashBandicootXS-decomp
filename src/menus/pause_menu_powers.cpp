#include "menus.hpp"
#include "font.hpp"

extern "C" {
#include "core.h"
#include "text.h"
#include "system.h"
}

/* PauseMenu::DrawPowersPage (menus.hpp; C++ since the #664 cleanup): the
 * icons of the powers the player has (flag bits 0x20, 0x80, 0x40, 0x10),
 * or, with none, a centred "no powers" label (text 0x3a).
 *
 * Built with old_agbcp (Makefile OLD_AGBCC_OBJS; agbcc as C, where 12
 * register pins made agbcc load the mask before the flag byte): the old
 * compiler's constant-before-ldrb order is the ROM's. */
void PauseMenu::DrawPowersPage()
{
    s32 none = 1;

    if (progress->flags & 0x20) {
        powerIcons[0]->DrawWithOffset(0, 0);
        none = 0;
    }
    if (progress->flags & 0x80) {
        powerIcons[1]->DrawWithOffset(0, 0);
        none = 0;
    }
    if (progress->flags & 0x40) {
        powerIcons[2]->DrawWithOffset(0, 0);
        none = 0;
    }
    if (progress->flags & 0x10) {
        powerIcons[3]->DrawWithOffset(0, 0);
        none = 0;
    }

    if (none) {
        u8 *label = (u8 *)GetUiText(0x3a);
        u32 width = gSmallFont->MeasureText(label);

        gSmallFont->SetPos(0xc2 - (width >> 1), 0x64);
        gSmallFont->DrawText(label);
    }
}
