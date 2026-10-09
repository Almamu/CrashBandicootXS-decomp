#include "save_menu.hpp"
#include "link_session.hpp"
#include "graphics_package.hpp"

extern "C" {
#include "core.h"
#include "actor.h"
#include "menus.h"
#include "graphics_package.h"
#include "objects.h"
#include "text.h"
#include "link.h"
#include "save.h"
#include "util.h"
#include "system.h"
#include "gfx.h"
#include "globals.h"
#include "math_util.h"
}

/* SaveMenu's link exchange, slot list drawing, setup helpers and per-state
 * draw routines (include/save_menu.hpp, #664's final cleanup). The second
 * half, from LoadBg on, was save_menu_ui.cpp until #771.
 *
 * The first half (ROM 0x08003B40-0x080041BC) were NAKED transcriptions
 * until the issue #4/#6/#8 retry
 * (docs/matching/archive/issue-4-6-8-naked-retry.md); DrawYesNoPrompt
 * followed in docs/matching/archive/early-rom-naked-retry.md, and
 * InitIcons in docs/matching/archive/hard-register-hold-retry.md. Built
 * with old_agbcc, now old_agbcp (Makefile OLD_AGBCC_OBJS), because
 * InitIcons only matches under it; the first half's other functions
 * compile identically under both compilers. */

/* State 3's link exchange: `new`s a save transfer (struct
 * save_transfer, 0x220 bytes) sending cartSave, then loops
 * VBlank-waiting while polling input (B cancels: 3), the link-reset flag
 * gLinkSessionReset and LinkSession::Update (the link-connection/handshake
 * driver, docs/rom_map.md's SIO/link-cable section) until the transfer's
 * state (SaveTransfer::Poll) settles. Returns that state; on 0 (done) it
 * copies the received save into linkSave.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The cancel test is `(u16)(keys & 2)`, whose known-zero
 * value the ROM reuses to clear `gLinkSessionReset`, and the
 * `SaveTransfer::GetData` result is taken before `linkSave` is loaded. */
s32 SaveMenu::LinkExchange()
{
    SaveTransfer *spinner = new SaveTransfer;
    s32 result;

    spinner->SetRecord(cartSave);
    spinner->Reset();
    do {
        WaitForVBlank();
        UpdateKeys(gInput);
        if ((u16)(gKeys.all & 2)) {
            result = 3;
        } else {
            if (gLinkSessionReset) {
                gLinkSessionReset = 0;
                spinner->Reset();
            }
            gLinkSession->Update();
            result = spinner->Poll();
        }
    } while (result == 1);
    if (result == 0) {
        s32 data = (s32)spinner->GetData();

        MemCopy32(linkSave, (void *)data, 0x200);
    }
    delete spinner;
    return result;
}

/* Draws `label1` (if non-zero) centered at Y=0x87, then `label2` (if
 * non-zero) centered at Y=0x91, both into gSmallFont.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers once the centre X gets its own local (`x = (0xf0 - w) >> 1`),
 * which is what puts it in r3 and the Y constant in ip. */
void SaveMenu::DrawMessageLines(s32 label1, s32 label2)
{
    s32 w, x;

    gSmallFont->SetPalette(0);
    if (label1) {
        w = gSmallFont->MeasureText((u8 *)label1);
        x = (0xf0 - w) >> 1;
        gSmallFont->SetPos(x, 0x87);
        gSmallFont->DrawText((u8 *)label1);
    }
    if (label2) {
        w = gSmallFont->MeasureText((u8 *)label2);
        x = (0xf0 - w) >> 1;
        gSmallFont->SetPos(x, 0x91);
        gSmallFont->DrawText((u8 *)label2);
    }
}

/* The "cancel" entry: label 0x23 centred at Y=0x87 in gSmallFont, in the
 * blink palette when highlighted.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers (same shape as DrawEmptySlotLabel, below). */
void SaveMenu::DrawCancel(u8 highlight)
{
    s32 w;

    if (highlight)
        gSmallFont->SetPalette(((flags >> 2) & 1) ? 1 : 2);
    else
        gSmallFont->SetPalette(0);
    w = gSmallFont->MeasureText((u8 *)GetUiText(0x23));
    gSmallFont->SetPos((0xf0 - w) >> 1, 0x87);
    gSmallFont->DrawText((u8 *)GetUiText(0x23));
}

/* Draws `value`'s label centered at Y=0x87, then draws a
 * highlighted/plain pair of fixed labels (0x29/0x2a, purpose
 * unconfirmed) swapping Y=0x87 vs Y=0x91 depending on `cursor`
 * - each pair member's slot gets a `gMenuCursorText` draw at its
 * *previous* position right before the real label, which reads as a
 * clear/overwrite step rather than a width probe (the return value is
 * never used).
 *
 * Once a NAKED transcription. The ROM keeps the record offset 0x130 in
 * r8 and the Y constant 0x87 in sb; with the 0x87 written as a literal
 * at each SetPos, as here, gcc shares it the ROM's way (the C kept it in
 * a `y` pinned to r9; #662 round 2). */
void SaveMenu::DrawYesNoPrompt(s32 value)
{
    s32 w;

    gSmallFont->SetPalette(0);
    w = gSmallFont->MeasureText((u8 *)GetUiText(value));
    gSmallFont->SetPos(0xa0 - w, 0x87);
    gSmallFont->DrawText((u8 *)GetUiText(value));
    gSmallFont->SetPalette(((flags >> 2) & 1) ? 1 : 2);
    if (!cursor) {
        gSmallFont->SetPos(0xa8, 0x87);
        gSmallFont->DrawText((u8 *)gMenuCursorText);
        gSmallFont->SetPos(0xb0, 0x87);
        gSmallFont->DrawText((u8 *)GetUiText(0x29));
    } else {
        gSmallFont->SetPos(0xa8, 0x91);
        gSmallFont->DrawText((u8 *)gMenuCursorText);
        gSmallFont->SetPos(0xb0, 0x91);
        gSmallFont->DrawText((u8 *)GetUiText(0x2a));
    }
    gSmallFont->SetPalette(0);
    if (!cursor) {
        gSmallFont->SetPos(0xb0, 0x91);
        gSmallFont->DrawText((u8 *)GetUiText(0x2a));
    } else {
        gSmallFont->SetPos(0xb0, 0x87);
        gSmallFont->DrawText((u8 *)GetUiText(0x29));
    }
}

/* Moves a slot list icon to (x, y) (whole pixels) and draws it. */
static inline void place_row_obj(Sprite *o, s32 x, s32 y)
{
    o->x = INT_TO_Q8(x);
    o->y = INT_TO_Q8(y);
    o->DrawWithOffset(0, 0);
}

/* The shared highlight/dim state call: selected rows draw in the
 * palette `flags` bit 2 picks (1 or 2), others in palette 0. */
#define SET_HIGHLIGHT(mgrExpr, flag)                                            \
    if (flag)                                                                   \
        (mgrExpr)->SetPalette(((flags >> 2) & 1) ? 1 : 2);                      \
    else                                                                        \
        (mgrExpr)->SetPalette(0)

/* Draws this settings row's three numeric stat values -
 * `statPtr->gems`/`crystals`/`relics` of the row's own `struct
 * settings_row_stats` (`statPtr` is `(&currentStats)[rowIdx]`,
 * i.e. `currentStats` and `rowStats[0..3]` read as one contiguous
 * 5-element array - RefreshSlotSummaries/SummarizeProgress,
 * below in this file, already establish `rowStats` as
 * this same array shape) - as plain decimal strings into
 * `rowObjA[rowIdx]`/`rowObjC[rowIdx]`/`rowObjB[rowIdx]`
 * respectively (each drawn with gSmallFont's DrawText, and each preceded
 * by the same highlight/dim SetPalette call this file's other row-label
 * functions
 * establish - DrawSlots's own `flag` parameter selects which row
 * is "selected", matching that shared idiom). A fourth value
 * (`statPtr->percent`) is formatted as `"NN%"` by `itoa`-ing then
 * manually scanning for the NUL terminator and overwriting it with a
 * literal `%` byte (re-terminating one byte later) - measured once via
 * gLargeFont's MeasureText to get its pixel width, then drawn
 * with its DrawText, right-aligned against `label1` using the measured
 * width (`posX = label1 - width + 0x1f`) - the standard
 * "measure, then right-align" idiom this ROM region uses throughout.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The fifth argument is a packed one-byte struct
 * (`struct byte_arg`): the callee reads it with `ldrb` and the caller
 * stores it with `strb`. Each block keeps running `x`/`y` locals, and
 * the third block re-derives `y` the same way the second does, which
 * reproduces the ROM spilling it. See docs/matching/archive/issue-4-6-8-naked-retry.md. */
void SaveMenu::DrawSlotStats(s32 label1, s32 label2, s32 rowIdx, struct byte_arg flagArg)
{
    u8 flag = flagArg.v;
    u8 buf[8];
    struct settings_row_stats *stats = &(&currentStats)[rowIdx];
    s32 x, y, i;
    s32 w;

    x = label1 + 0x2b;
    y = label2 + 5;
    place_row_obj(rowObjA[rowIdx], x, y);
    x += 0xd;
    y = label2;
    itoa(stats->gems, buf, 10);
    SET_HIGHLIGHT(gSmallFont, flag);
    gSmallFont->SetPos(x, y);
    gSmallFont->DrawText((u8 *)buf);

    x = label1 + 7;
    y = label2 + 0x1e;
    place_row_obj(rowObjC[rowIdx], x, y);
    x += 9;
    y -= 7;
    itoa(stats->crystals, buf, 10);
    SET_HIGHLIGHT(gSmallFont, flag);
    gSmallFont->SetPos(x, y);
    gSmallFont->DrawText((u8 *)buf);

    x = label1 + 0x2b;
    y = label2 + 0x1e;
    place_row_obj(rowObjB[rowIdx], x, y);
    x += 0xd;
    y -= 7;
    itoa(stats->relics, buf, 10);
    SET_HIGHLIGHT(gSmallFont, flag);
    gSmallFont->SetPos(x, y);
    gSmallFont->DrawText((u8 *)buf);

    itoa(stats->percent, buf, 10);
    i = 0;
    y = label2 - 2;
    for (; i < 7; i++) {
        if (buf[i] == 0) {
            buf[i] = '%';
            buf[i + 1] = 0;
            break;
        }
    }
    w = gLargeFont->MeasureText((u8 *)buf);
    SET_HIGHLIGHT(gLargeFont, flag);
    gLargeFont->SetPos(label1 - w + 0x1f, y);
    gLargeFont->DrawText((u8 *)buf);
}

/* An inlined copy of DrawEmptySlotLabel (below in this file): the
 * row's highlighted/dimmed 0x25 glyph centred at (arg1 + 0x1d, arg2 + 0xc). */
static inline void draw_row_mark(SaveMenu *self, s32 arg1, s32 arg2, u8 arg3)
{
    s32 x = arg1 + 0x1d;
    s32 y = arg2 + 0xc;
    s32 w;

    if (arg3)
        gSmallFont->SetPalette(((self->flags >> 2) & 1) ? 1 : 2);
    else
        gSmallFont->SetPalette(0);
    w = gSmallFont->MeasureText((u8 *)GetUiText(0x25));
    gSmallFont->SetPos(x - w / 2, y);
    gSmallFont->DrawText((u8 *)GetUiText(0x25));
}

#define DRAW_ROW(i, labelX, labelY)                                             \
    if (handle->IsSlotEmpty((i))) {                                             \
        draw_row_mark(this, (labelX), (labelY), selectedIndex == (i));          \
    } else {                                                                    \
        struct byte_arg sel;                                                    \
        sel.v = selectedIndex == (i);                                           \
        DrawSlotStats((labelX), (labelY), (i) + 1, sel);                    \
    }

/* Per docs/rom_map.md's "narrowed down which screen overlay_ui is"
 * section: one of 4 settings rows, `handle`/`selectedIndex` from the
 * 6-wrapper-caller family (DrawConfirmDelete etc.,
 * below). When `handle->IsSlotEmpty(i)`
 * reports row `i` selected, draws a highlighted numeric glyph
 * (label 0x25) centered at the row's fixed position; otherwise draws
 * the row's normal label pair via DrawSlotStats (above in this file),
 * flagged if `selectedIndex == i`. The four rows' fixed anchors: row 0
 * = (0x43,0x2d)/labels(0x26,0x21)/idx 1; row 1 = (0x43,0x5f)/
 * (0x26,0x53)/idx 2; row 2 = (0xa3,0x2d)/(0x86,0x21)/idx 3; row 3 =
 * (0xa3,0x5f)/(0x86,0x53)/idx 4.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The "selected" branch is an inlined copy of DrawEmptySlotLabel
 * (below), `draw_row_mark` above. */
void SaveMenu::DrawSlots(SaveData *handle, s32 selectedIndex)
{
    DRAW_ROW(0, 0x26, 0x21);
    DRAW_ROW(1, 0x26, 0x53);
    DRAW_ROW(2, 0x86, 0x21);
    DRAW_ROW(3, 0x86, 0x53);
}

/* Reserves a font's tiles in OBJ VRAM (as power_dialog.cpp's). */
static inline void IconReserve(Font **m)
{
    ObjVramCursor *c = gObjVramCursor;

    c->Reserve((*m)->tileCount << 5);
}

#define SET_ROW_OBJ_POS(objExpr, px, py)                                        \
    {                                                                           \
        Sprite *_o = (objExpr);                                                 \
        _o->x = (px) << 8;                                                      \
        _o->y = (py) << 8;                                                      \
    }

/* The animation number as a value: an inline call, so the store's value
 * is computed before its address (the ROM's `movs r0, #1` ahead of
 * `adds r1, r4, #0x2d`); a plain `frame` is loaded after the address. */
static inline s32 Opaque(s32 v)
{
    return v;
}

static inline void new_row_icon(UiSprite **slot, u32 tblOff, u32 frame)
{
    UiSprite *icon;

    icon = new UiSprite;
    *slot = icon;
    icon->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + tblOff);
    icon->tag = Opaque(frame);
    icon->ResetFrameTimer();
    icon->ResetFrameIndex();
    icon->SetAnimDone(0);
    {
        s32 lo = (*slot)->GetAnimPaletteSlot();
        u8 *p = (u8 *)*slot + 0x29; // the palette nibble's byte
        s32 m = -16;

        lo &= 15;
        *p = (*p & m) | lo;
    }
    {
        u16 half = 0x80;

        (*slot)->affine = half;
    }
}

/* The screen's graphics setup (InitSaveMenuIcons): resets the OAM shadow
 * buffer and the tile cache, loads four 16-colour palettes into cache
 * slots 0-3, uploads both fonts' tiles (the same SetTileBase/IconReserve
 * sequence as PowerDialog::Show), builds the three 5-entry icon arrays
 * `rowObjA/B/C` (`new UiSprite`s; sprite bank offsets 0x180/0x18c/0x1bc,
 * frames 1/2/0, half size: `affine` 0x80) and places five of them.
 *
 * Was raw asm (asm/code_3_1_10_4.s) with a NON_MATCHING draft; closed
 * in docs/matching/archive/hard-register-hold-retry.md.
 * #662 round 3: `tag` is a plain field store. old_agbcp's read-modify-
 * write byte store leaves a dead zero mask, which loop.c hoists out of
 * the loop as a movable; that zero lives across the whole loop, gets no
 * register (r4-r10 are taken) and is rematerialized by reload at frame
 * 0's store, after the address, into r1: the ROM's frame-0 sequence, and
 * the reload that moves the reload-register rotation on to r3 for the
 * 15's copy from sl. The C had a `u8 *` store with the frame-0 address
 * pinned to r0 and an r1 hold for that rotation. The 0x80 is a `u16`
 * local, which puts its load (r9) right after the 15's in the loop
 * pre-header, ahead of the rowObj pointers' copies, as in the ROM;
 * `affine = 0x80` loads it after them. */
void SaveMenu::InitIcons()
{
    u16 (*pal)[16];
    UiSprite **a, **b, **c;
    s32 i;

    gOamBuffer->Reset();
    gOamBuffer->HideUnused();
    WaitForVBlank();
    gOamBuffer->Commit();
    gPaletteCache->FreeUnlockedSlots();
    gPaletteCache->ClaimSlot(0);
    gPaletteCache->ClaimSlot(1);
    gPaletteCache->ClaimSlot(2);
    gPaletteCache->ClaimSlot(3);
    pal = (u16 (*)[16])gPaletteCache->slots;
    for (i = 0; i < 16; i++) {
        pal[0][i] = gSaveMenuPalette0[i];
        pal[1][i] = gSaveMenuPalette1[i];
        pal[2][i] = gSaveMenuPalette2[i];
        pal[3][i] = gSaveMenuPalette3[i];
    }
    gSmallFont->SetPalette(0);
    gLargeFont->SetPalette(0);
    gObjVramCursor->baseTile = 0;
    gObjVramCursor->Reset();
    gObjVramCursor->Reset();
    gSmallFont->SetTileBase(0);
    IconReserve(&gSmallFont);
    {
        u32 v = gSmallFont->tileCount;

        gLargeFont->SetTileBase(v);
    }
    IconReserve(&gLargeFont);
    gObjVramCursor->Mark();

    a = rowObjA;
    b = rowObjB;
    c = rowObjC;
    for (i = 0; i < 5; i++) {
        new_row_icon(&a[i], 0xc0 << 1, 1);
        new_row_icon(&b[i], 0xc6 << 1, 2);
        new_row_icon(&c[i], 0xde << 1, 0);
    }
    SET_ROW_OBJ_POS(rowObjA[0], 0x14, 0x28);
    SET_ROW_OBJ_POS(rowObjB[0], 0x14, 0x50);
    SET_ROW_OBJ_POS(rowObjB[1], 0x14, 0x3c);
    SET_ROW_OBJ_POS(rowObjB[2], 0x14, 0x4b);
    SET_ROW_OBJ_POS(rowObjC[0], 0x78, 0x50);
}

/* SaveMenu's setup helpers and draw routines (save_menu_ui.cpp until
 * #771). Built with old_agbcp (the C was agbcc): under it LoadBg matches
 * without the C's r1 pin. */

/* Same shape as LoadLanguageSelectBg (src/frontend/language_select_setup.cpp) - reset
 * the DISPCNT shadow (`dispcnt`) and set its two bytes one at a time,
 * request a BG tile/map graphics package, set BG0's
 * control register from it - plus zeroing `frame`, which LoadLanguageSelectBg's
 * language_select doesn't have. */
void SaveMenu::LoadBg()
{
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

    BgSetup buf(2, 0x1e, 1, 3);
    buf.Load(&gMenuSkyBg);
    frame = 0;
    REG_BG0CNT = buf.GetControl();
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

/* Refreshes each of the 4 settings rows' aggregate stats from `handle`,
 * skipping any row SaveData::IsSlotEmpty reports as inactive/hidden. */
void SaveMenu::RefreshSlotSummaries(SaveData *handle)
{
    struct settings_row_stats *row;
    struct save_slot buf;
    s32 i;

    i = 0;
    row = &rowStats[0];
    do {
        if (!handle->IsSlotEmpty(i)) {
            handle->ReadSlot(i, &buf);
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
    s32 v = cartSave->Load();
    if ((u32)(v - 1) <= 3) {
        cartSave->Reset();
        cartSave->Store();
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
    LinkSession *p = gLinkSession;
    p->Reset();
    p->enabled = 0;
}

void SaveMenu::BeginLinkTransfer()
{
    gLinkSession->Reset();
    gLinkSession->enabled = 1;
    linkSave->Reset();
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

    cartSave->ReadSlot(arg1, buf);
    cartSave->EraseSlot(arg1);
    if (cartSave->Store()) {
        cartSave->WriteSlot(arg1, buf);
    }
}
