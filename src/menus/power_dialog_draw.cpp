#include "menus.hpp"
#include "font.hpp"

extern "C" {
#include "vram_pool.h"
#include "text.h"
#include "util.h"
#include "system.h"
#include "level.h"
#include "globals.h"
}

/* gLevelTable's time-trial thresholds (`level_info.times`, level.h):
 * CountSapphireRelics/CountGoldRelics/CountPlatinumRelics each count how
 * many of a caller's 20 records fall between two adjacent thresholds
 * (times[0]/[1] for one function, times[1]/[2] for the next, and just
 * times[2] alone for the simplest one).
 * CountSapphireRelics/CountGoldRelics read through inline asm rather than plain
 * struct field access on purpose: gcc's CSE otherwise shares the
 * "table[i]" address between the two threshold reads even though the
 * ROM recomputes it fresh for each one (see docs/matching.md, "Matching
 * decompilation"). */

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

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
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
    HideUnusedOamEntries(gOamBuffer);
}

void PowerDialog::Animate()
{
    frame++;
    icon->AdvanceAnim();
}

void PowerDialog::CommitFrame()
{
    WaitForVBlank();
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
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

s32 GetProgressLives(void *arg0)
{
    return PACKED_STATS_LIVES(*(u8 *)arg0);
}

/* The save block's counts (the pause menu's pages, the save menu's rows,
 * the game-over screen's totals). The relics are the time-trial medals:
 * a platinum relic for a time within the level's times[2], gold within
 * times[1], sapphire within times[0]. */
s32 CountPlatinumRelics(void *arg0)
{
    struct menu_save *save = (struct menu_save *)arg0;
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = save->levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[2]) {
                count++;
            }
        }
    }
    return count;
}

s32 CountGoldRelics(void *arg0)
{
    struct menu_save *save = (struct menu_save *)arg0;
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = save->levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[1] && val > gLevelTable[i].times[2]) {
                count++;
            }
        }
    }
    return count;
}

s32 CountSapphireRelics(void *arg0)
{
    struct menu_save *save = (struct menu_save *)arg0;
    s32 count;
    s32 i;
    u32 val;

    count = 0;
    for (i = 0; i < 20; i++) {
        val = save->levels[i].h.time;
        if (val != 0) {
            if (val <= gLevelTable[i].times[0] && val > gLevelTable[i].times[1]) {
                count++;
            }
        }
    }
    return count;
}

s32 CountRelics(void *arg0)
{
    s32 total;
    s32 b;
    s32 c;

    total = CountSapphireRelics(arg0);
    b = CountGoldRelics(arg0);
    c = CountPlatinumRelics(arg0);
    total += b;
    total += c;
    return total;
}

s32 CountGems(void *arg0)
{
    struct menu_save *save = (struct menu_save *)arg0;
    s32 total;
    s32 i;
    s32 result;
    u8 flags;

    total = 0;
    for (i = 0; i < 20; i++)
        total += save->levels[i].b.flag1 + save->levels[i].b.flag2;
    total += save->levels[24].b.flag1 + save->levels[24].b.flag2;
    flags = save->flags;
    result = total + (((u32)flags << 31) >> 31);
    result += ((u32)flags << 29) >> 31;
    result += ((u32)flags << 28) >> 31;
    result += ((u32)flags << 30) >> 31;
    return result;
}

s32 CountClearGems(void *arg0)
{
    struct menu_save *save = (struct menu_save *)arg0;
    s32 total;
    s32 i;

    total = 0;
    for (i = 0; i < 20; i++)
        total += save->levels[i].b.flag1 + save->levels[i].b.flag2;
    total += save->levels[24].b.flag1 + save->levels[24].b.flag2;
    return total;
}

s32 CountCrystals(void *arg0)
{
    struct menu_save *save = (struct menu_save *)arg0;
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 20; i++)
        count += save->levels[i].b.cleared;
    return count;
}
