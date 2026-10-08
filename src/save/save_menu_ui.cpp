#include "save_menu.hpp"

extern "C" {
#include "core.h"
#include "text.h"
#include "link.h"
#include "link_session.h"
#include "save.h"
#include "system.h"
#include "menus.h"
#include "gfx.h"
#include "globals.h"
}

/* SaveMenu's setup helpers and draw routines (include/save_menu.hpp,
 * #664's final cleanup). Built with old_agbcp (Makefile OLD_AGBCC_OBJS;
 * the C was agbcc): under it LoadBg matches without the C's r1 pin. */

/* Same shape as LoadLanguageSelectBg (src/frontend/language_select_setup.cpp) - reset
 * the DISPCNT shadow (`dispcnt`) and set its two bytes one at a time,
 * request a BG tile/map graphics package, set BG0's
 * control register from it - plus zeroing `frame`, which LoadLanguageSelectBg's
 * language_select doesn't have. */
void SaveMenu::LoadBg()
{
    struct bg_setup buf;
    u32 zero = 0;
    s32 a;
    s32 b;

    dispcnt = zero;
    a = 0x40;
    a |= ((u8 *)&dispcnt)[0];
    a &= -8;
    a |= 1;
    ((u8 *)&dispcnt)[0] = a;
    b = 1;
    b |= ((u8 *)&dispcnt)[1];
    b &= -3;
    b |= 0x10;
    ((u8 *)&dispcnt)[1] = b;

    InitBgSetup(&buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(&buf, &gMenuSkyBg);
    frame = 0;
    REG_BG0CNT = GetBgSetupControl(&buf);
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

/* Refreshes each of the 4 settings rows' aggregate stats from `handle`,
 * skipping any row IsSaveSlotEmpty reports as inactive/hidden. */
void SaveMenu::RefreshSlotSummaries(struct save_data *handle)
{
    struct settings_row_stats *row;
    struct save_slot buf;
    s32 i;

    i = 0;
    row = &rowStats[0];
    do {
        if (!IsSaveSlotEmpty(handle, i)) {
            ReadSaveSlot(handle, i, &buf);
            row->gems = CountClearGems(&buf.progress);
            row->relics = CountRelics(&buf.progress);
            row->lives = GetProgressLives(&buf.progress);
            row->crystals = CountCrystals(&buf.progress);
            row->percent = GetCompletionPercent(&buf.progress);
        }
        row++;
        i++;
    } while (i <= 3);
}

void SaveMenu::LoadData()
{
    s32 v = LoadSaveData(cartSave);
    if ((u32)(v - 1) <= 3) {
        ResetSaveData(cartSave);
        StoreSaveData(cartSave);
    }
}

/* Fills `dest` from `src` using the same five-function battery as the
 * loop in RefreshSlotSummaries above - `this` is passed but never used,
 * matching the ROM exactly. */
void SaveMenu::SummarizeProgress(struct settings_row_stats *dest, const struct game_progress *src)
{
    dest->gems = CountClearGems(src);
    dest->relics = CountRelics(src);
    dest->lives = GetProgressLives(src);
    dest->crystals = CountCrystals(src);
    dest->percent = GetCompletionPercent(src);
}

/* The next two (ROM 0x08004914-0x08004A50) were NAKED transcriptions
 * until the issue #4/#6/#8 retry
 * (docs/matching/archive/issue-4-6-8-naked-retry.md). */

/* `arg1`/`arg2` are plain coordinate values here (not pointers - the
 * ROM does raw integer arithmetic on them, `arg1+0x1d`/`arg2+0xc`),
 * used as the on-screen anchor for a centered numeric glyph (label
 * 0x25) into gSmallFont.
 *
 * The C wrote the draws as `record->slots[n]` calls (`ICON_TEXT_CALL`);
 * they are Font's virtual MeasureText and DrawText. */
void SaveMenu::DrawEmptySlotLabel(s32 arg1, s32 arg2, u8 arg3)
{
    s32 x = arg1 + 0x1d;
    s32 y = arg2 + 0xc;
    s32 w;

    if (arg3)
        gSmallFont->SetPalette(((flags >> 2) & 1) ? 1 : 2);
    else
        gSmallFont->SetPalette(0);
    w = gSmallFont->MeasureText((u8 *)GetUiText(0x25));
    gSmallFont->SetPos(x - w / 2, y);
    gSmallFont->DrawText((u8 *)GetUiText(0x25));
}

/* Draws a centred title (a GetUiText label) at Y=6 in gLargeFont; `this`
 * is unused. The same centred-label shape as PowerDialog::Draw's
 * (src/menus/power_dialog_draw.cpp), for a single label. */
void SaveMenu::DrawTitle(s32 labelIndex)
{
    s32 w;

    gLargeFont->SetPalette(0);
    w = gLargeFont->MeasureText((u8 *)GetUiText(labelIndex));
    gLargeFont->SetPos((0xf0 - w) >> 1, 6);
    gLargeFont->DrawText((u8 *)GetUiText(labelIndex));
}

/* The highlight's blink: palette 1 or 2, by bit 2 of the frame counter
 * `flags` (the other draws spell the same test out). */
s32 SaveMenu::GetBlinkPalette()
{
    if ((flags >> 2) & 1) {
        return 1;
    }
    return 2;
}

/* `this` is never read, but LinkInput passes it (the ROM's call sets it
 * up in r0). */
void SaveMenu::EndLinkTransfer()
{
    struct link_session *p = gLinkSession;
    ResetLinkSession(p);
    p->enabled = 0;
}

void SaveMenu::BeginLinkTransfer()
{
    ResetLinkSession(gLinkSession);
    gLinkSession->enabled = 1;
    ResetSaveData(linkSave);
}

void SaveMenu::DrawConfirmDelete()
{
    DrawTitle(0x1d);
    DrawSlots(cartSave, pendingSlot);
    DrawYesNoPrompt(0x26);
}

void SaveMenu::DrawDelete()
{
    DrawTitle(0x1d);
    DrawSlots(cartSave, cursor);
    DrawCancel(cursor == 4);
}

void SaveMenu::DrawOverwrite()
{
    DrawTitle(0x1e);
    DrawSlots(cartSave, pendingSlot);
    DrawYesNoPrompt(0x27);
}

void SaveMenu::DrawSave()
{
    DrawTitle(0x1e);
    DrawSlots(cartSave, cursor);
    DrawCancel(cursor == 4);
}

void SaveMenu::DrawMessage()
{
    DrawTitle(0x1c);
    DrawMessageLines(messageLine1, messageLine2);
}

void SaveMenu::DrawLoadLink()
{
    DrawTitle(0x1c);
    DrawSlots(linkSave, cursor);
    DrawCancel(cursor == 4);
}

void SaveMenu::DrawLoad()
{
    DrawTitle(0x1b);
    DrawSlots(cartSave, cursor);
    DrawCancel(cursor == 4);
}

void SaveMenu::Draw()
{
    gOamBuffer->Reset();
    gObjVramCursor->Rewind();
    if ((u32)state <= 0xa) {
        switch (state) {
        case 0:
            DrawMain();
            break;
        case 1:
            DrawLoad();
            break;
        case 2:
            DrawLoadLink();
            break;
        case 3:
        case 4:
            DrawMessage();
            break;
        case 5:
            DrawSave();
            break;
        case 6:
            DrawDelete();
            break;
        case 9:
            DrawOverwrite();
            break;
        case 7:
            DrawConfirmDelete();
            break;
        case 8:
            break;
        case 10:
            break;
        default:
            break;
        }
    }
    gOamBuffer->HideUnused();
}

void SaveMenu::DeleteSlot(s32 arg1)
{
    u8 buf[0x70];

    ReadSaveSlot(cartSave, arg1, buf);
    EraseSaveSlot(cartSave, arg1);
    if (StoreSaveData(cartSave)) {
        WriteSaveSlot(cartSave, arg1, buf);
    }
}
