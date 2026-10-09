#include "menus.hpp"
#include "font.hpp"

extern "C" {
#include "text.h"
#include "util.h"
#include "system.h"
#include "level.h"
#include "globals.h"
}

/* Draws the dialog: the power's title centred at row 0x2d (gLargeFont's
 * MeasureText, then DrawText), the description wrapped into its box
 * (DrawWrappedTextInBox) and UI text 0x2e centred at row 0x90
 * (gSmallFont). Parked as NAKED for a long time over a register-letter
 * gap; closed by computing the centered X into its own local before
 * passing it to the inline setter (Font::SetPos; passing the expression
 * straight in swapped the X/Y and 0x130/240 registers) - see
 * docs/matching/archive/strag3-naked-retry.md. */
void PowerDialog::Draw()
{
    struct aabb box;
    s32 w;
    s32 n;
    u32 x;

    gOamBuffer->Reset();
    gObjVramCursor->Rewind();
    icon->DrawWithOffset(0, 0);
    w = gLargeFont->MeasureText((u8 *)titleText);
    x = (u32)(240 - w) >> 1;
    gLargeFont->SetPos(x, 0x2d);
    gLargeFont->DrawText((u8 *)titleText);
    SetAabbPos(&box, 0x10, 0x6a);
    SetAabbSize(&box, 0xd0, 0x35);
    DrawWrappedTextInBox((u8 *)descText, gSmallFont, &box, 0);
    n = GetUiText(0x2e);
    w = gSmallFont->MeasureText((u8 *)n);
    x = (u32)(240 - w) >> 1;
    gSmallFont->SetPos(x, 0x90);
    gSmallFont->DrawText((u8 *)n);
    gOamBuffer->HideUnused();
}

void PowerDialog::Animate()
{
    frame++;
    icon->AdvanceAnim();
}

void PowerDialog::CommitFrame()
{
    WaitForVBlank();
    gPaletteCache->Upload();
    gOamBuffer->Commit();
    FlushVramDmaQueue();
    *(vu16 *)REG_ADDR_BG0HOFS = frame >> 3;
    *(vu16 *)PLTT = 0;
    *(vu32 *)REG_ADDR_BLDCNT = blend.raw;
    *(vu16 *)REG_ADDR_BLDY = bldy.evy;
    *(vu16 *)REG_ADDR_DISPCNT = dispcnt.raw;
}

PowerDialog::~PowerDialog()
{
    delete icon;
}

/* The four powers' dialogs (game_frame.cpp): each power's name and
 * description text and its icon. */
void ShowTurboRunDialog(void)
{
    PowerDialog::Show(0x3F, 0x43, 1);
}

void ShowTornadoSpinDialog(void)
{
    PowerDialog::Show(0x3E, 0x42, 0);
}

void ShowDoubleJumpDialog(void)
{
    PowerDialog::Show(0x3D, 0x41, 2);
}

void ShowSuperBodySlamDialog(void)
{
    PowerDialog::Show(0x3C, 0x40, 3);
}
