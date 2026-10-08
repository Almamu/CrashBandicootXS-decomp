#include "save_menu.hpp"

extern "C" {
#include "core.h"
#include "match.h"
#include "actor.h"
#include "pause_menu.h"
#include "vram_pool.h"
#include "text.h"
#include "link.h"
#include "save.h"
#include "util.h"
#include "system.h"
#include "gfx.h"
#include "objects.h"
#include "globals.h"
#include "math_util.h"
}

/* SaveMenu's link exchange and slot list drawing (include/save_menu.hpp,
 * #664's final cleanup; ROM 0x08003B40-0x080041BC). The functions were
 * NAKED transcriptions until the issue #4/#6/#8 retry
 * (docs/matching/archive/issue-4-6-8-naked-retry.md); DrawYesNoPrompt
 * followed in docs/matching/archive/early-rom-naked-retry.md, and
 * InitIcons in docs/matching/archive/hard-register-hold-retry.md. Built
 * with old_agbcc, now old_agbcp (Makefile OLD_AGBCC_OBJS), because
 * InitIcons only matches under it; every other function here compiles
 * identically under both compilers. Their siblings DrawEmptySlotLabel and
 * DrawTitle are in src/save/save_menu_ui.cpp. */

/* State 3's link exchange: `new`s a save transfer (struct
 * settings_sync_pump, 0x220 bytes) sending cartSave, then loops
 * VBlank-waiting while polling input (B cancels: 3), the link-reset flag
 * gLinkSessionReset and UpdateLinkSession (the link-connection/handshake
 * driver, docs/rom_map.md's SIO/link-cable section) until the transfer's
 * state (PollSaveTransfer) settles. Returns that state; on 0 (done) it
 * copies the received save into linkSave.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The cancel test is `(u16)(keys & 2)`, whose known-zero
 * value the ROM reuses to clear `gLinkSessionReset`, and the
 * `GetSaveTransferData` result is taken before `linkSave` is loaded. */
s32 SaveMenu::LinkExchange()
{
    struct settings_sync_pump *spinner = new settings_sync_pump;
    s32 result;

    SetSaveTransferRecord(spinner, cartSave);
    ResetSaveTransfer(spinner);
    do {
        WaitForVBlank();
        UpdateKeys(gInput);
        if ((u16)(gKeys.all & 2)) {
            result = 3;
        } else {
            if (gLinkSessionReset) {
                gLinkSessionReset = 0;
                ResetSaveTransfer(spinner);
            }
            UpdateLinkSession(gLinkSession);
            result = PollSaveTransfer(spinner);
        }
    } while (result == 1);
    if (result == 0) {
        s32 data = (s32)GetSaveTransferData(spinner);

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
 * compilers (same shape as DrawEmptySlotLabel, src/save/save_menu_ui.cpp). */
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
 * Once a NAKED transcription; it matches as plain C under both
 * compilers. The ROM keeps the record offset 0x130 in r8 and the Y
 * constant 0x87 in sb: `y` is pinned to r9 and set after the
 * manager pointer is loaded (the unpinned draft swapped the two, as
 * global-alloc ranks 0x87 slightly above 0x130). */
void SaveMenu::DrawYesNoPrompt(s32 value)
{
    s32 w;
    MATCH_HOLD_REG(s32, y, r9); // r9, as in the ROM (see above); still needed in C++

    gSmallFont->SetPalette(0);
    w = gSmallFont->MeasureText((u8 *)GetUiText(value));
    {
        s32 x = 0xa0 - w;
        Font *m = gSmallFont;
        y = 0x87;
        m->SetPos(x, y);
    }
    gSmallFont->DrawText((u8 *)GetUiText(value));
    gSmallFont->SetPalette(((flags >> 2) & 1) ? 1 : 2);
    if (!cursor) {
        gSmallFont->SetPos(0xa8, y);
        gSmallFont->DrawText((u8 *)gMenuCursorText);
        gSmallFont->SetPos(0xb0, y);
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
 * src/save/save_menu_ui.cpp, already establish `rowStats` as
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

/* An inlined copy of DrawEmptySlotLabel (src/save/save_menu_ui.cpp): the
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
    if (IsSaveSlotEmpty(handle, (i))) {                                             \
        draw_row_mark(this, (labelX), (labelY), selectedIndex == (i));          \
    } else {                                                                    \
        struct byte_arg sel;                                                    \
        sel.v = selectedIndex == (i);                                           \
        DrawSlotStats((labelX), (labelY), (i) + 1, sel);                    \
    }

/* Per docs/rom_map.md's "narrowed down which screen overlay_ui is"
 * section: one of 4 settings rows, `handle`/`selectedIndex` from the
 * 6-wrapper-caller family (DrawConfirmDelete etc.,
 * src/save/save_menu_ui.cpp). When `IsSaveSlotEmpty(handle, i)`
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
 * (src/save/save_menu_ui.cpp), `draw_row_mark` above. */
void SaveMenu::DrawSlots(struct save_data *handle, s32 selectedIndex)
{
    DRAW_ROW(0, 0x26, 0x21);
    DRAW_ROW(1, 0x26, 0x53);
    DRAW_ROW(2, 0x86, 0x21);
    DRAW_ROW(3, 0x86, 0x53);
}

/* Reserves a font's tiles in OBJ VRAM (as power_dialog.cpp's). */
static inline void IconReserve(Font **m)
{
    struct vram_upload_cursor *c = gObjVramCursor;

    ReserveObjVram(c, (*m)->tileCount << 5);
}

#define SET_ROW_OBJ_POS(objExpr, px, py)                                        \
    {                                                                           \
        Sprite *_o = (objExpr);                                                 \
        _o->x = (px) << 8;                                                      \
        _o->y = (py) << 8;                                                      \
    }

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
    /* Plain `u8 *` stores here and for `affine` below: old_agbcp's
     * read-modify-write field store leaves a dead zero mask that the loop
     * pass counts as a movable, which keeps 0x80 out of the loop
     * pre-header (as old_agbcc's did in the C). */
    if (frame)
        *(u8 *)&icon->tag = Opaque(frame);
    else {
        /* The ROM computes the address first, in r0, and reloads the 0
         * into r1 after it; this pin reproduces that for frame 0. */
        MATCH_HOLD_REG(u8 *, fp, r0) = &icon->tag;
        *fp = 0;
    }
    icon->ResetFrameTimer();
    icon->ResetFrameIndex();
    icon->SetAnimDone(0);
    {
        s32 lo = (*slot)->GetAnimPaletteSlot();
        u8 *p = (u8 *)*slot + 0x29; // the palette nibble's byte
        s32 m = -16;

        if (frame == 0) {
            /* Hard-register hold (no code): keeping r1 live here makes
             * reload skip it when it copies the 15 from sl, so the third
             * icon takes r3 as in the ROM. The ROM reloaded the 0 above,
             * which moved the round-robin on; the pinned store doesn't. */
            MATCH_HOLD_REG(s32, hold, r1);
            MATCH_HOLD(hold);
            lo &= 15;
            MATCH_USE(hold);
        } else
            lo &= 15;
        *p = (*p & m) | lo;
    }
    *(u16 *)&(*slot)->affine = 0x80;
}

/* The screen's graphics setup (InitSaveMenuIcons): resets the OAM shadow
 * buffer and the tile cache, loads four 16-colour palettes into cache
 * slots 0-3, uploads both fonts' tiles (the same SetTileBase/IconReserve
 * sequence as PowerDialog::Show), builds the three 5-entry icon arrays
 * `rowObjA/B/C` (`new UiSprite`s; sprite bank offsets 0x180/0x18c/0x1bc,
 * frames 1/2/0, half size: `affine` 0x80) and places five of them.
 *
 * Was raw asm (asm/code_3_1_10_4.s) with a NON_MATCHING draft; closed
 * in docs/matching/archive/hard-register-hold-retry.md. The plain-pointer
 * stores in new_row_icon fix the loop pre-header (see
 * docs/matching/archive/early-rom-naked-retry-2.md); the frame-0 address
 * pin and the r1 hold fix the last 6 halfwords. The C also needed three
 * MATCH_BARRIER()s of insn-count padding to keep the rowObj pointers'
 * stack slots in the ROM's order; the C++ doesn't. */
void SaveMenu::InitIcons()
{
    u16 (*pal)[16];
    UiSprite **a, **b, **c;
    s32 i;

    ResetOamBuffer(gOamBuffer);
    HideUnusedOamEntries(gOamBuffer);
    WaitForVBlank();
    CommitOamBuffer(gOamBuffer);
    FreeUnlockedPaletteSlots(gPaletteCache);
    ClaimPaletteSlot(gPaletteCache, 0);
    ClaimPaletteSlot(gPaletteCache, 1);
    ClaimPaletteSlot(gPaletteCache, 2);
    ClaimPaletteSlot(gPaletteCache, 3);
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
    ResetObjVram(gObjVramCursor);
    ResetObjVram(gObjVramCursor);
    gSmallFont->SetTileBase(0);
    IconReserve(&gSmallFont);
    {
        u32 v = gSmallFont->tileCount;

        gLargeFont->SetTileBase(v);
    }
    IconReserve(&gLargeFont);
    MarkObjVram(gObjVramCursor);

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
