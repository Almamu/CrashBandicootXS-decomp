#include "core.h"
#include "match.h"
#include "bitmap_font.h"
#include "text.h"
#include "link.h"
#include "save.h"
#include "system.h"
#include "menus.h"
#include "gfx.h"
#include "globals.h"


/* Same shape as LoadLanguageSelectBg (src/frontend/language_select_setup.c) - reset
 * the DISPCNT shadow (`dispcnt`) and set its two bytes one at a time,
 * request a BG tile/map graphics package, set BG0's
 * control register from it - plus zeroing `frame`, which LoadLanguageSelectBg's
 * language_select doesn't have. */
void LoadSaveMenuBg(struct save_menu *self)
{
    struct bg_setup buf;
    u32 zero = 0;
    s32 a;
    MATCH_HOLD_REG(s32, b, r1);

    self->dispcnt = zero;
    a = 0x40;
    a |= ((u8 *)&self->dispcnt)[0];
    a &= -8;
    a |= 1;
    ((u8 *)&self->dispcnt)[0] = a;
    b = 1;
    b |= ((u8 *)&self->dispcnt)[1];
    b &= -3;
    b |= 0x10;
    ((u8 *)&self->dispcnt)[1] = b;

    InitBgSetup(&buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(&buf, &gMenuSkyBg);
    self->frame = 0;
    REG_BG0CNT = GetBgSetupControl(&buf);
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

/* Refreshes each of the 4 settings rows' aggregate stats from `handle`,
 * skipping any row IsSaveSlotEmpty reports as inactive/hidden. */
void RefreshSaveSlotSummaries(struct save_menu *self, void *handle)
{
    struct settings_row_stats *row;
    u8 buf[0x70];
    s32 i;

    i = 0;
    row = &self->rowStats[0];
    do {
        if (!IsSaveSlotEmpty(handle, i)) {
            ReadSaveSlot(handle, i, buf);
            row->gems = CountClearGems(buf);
            row->relics = CountRelics(buf);
            row->lives = GetProgressLives(buf);
            row->crystals = CountCrystals(buf);
            row->percent = GetCompletionPercent(buf);
        }
        row++;
        i++;
    } while (i <= 3);
}

void LoadSaveMenuData(struct save_menu *self)
{
    s32 v = LoadSaveData(self->cartSave);
    if ((u32)(v - 1) <= 3) {
        ResetSaveData(self->cartSave);
        StoreSaveData(self->cartSave);
    }
}

/* Fills `dest` from `src` using the same five-function battery as the
 * loop in RefreshSaveSlotSummaries above - `self` (the screen widget) is passed but
 * never used, matching the ROM exactly. */
void SummarizeProgress(struct save_menu *self, struct settings_row_stats *dest, void *src)
{
    dest->gems = CountClearGems(src);
    dest->relics = CountRelics(src);
    dest->lives = GetProgressLives(src);
    dest->crystals = CountCrystals(src);
    dest->percent = GetCompletionPercent(src);
}

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

static inline void set_icon_mgr_pos(struct bitmap_font *m, u32 x, u32 y)
{
    m->posX = x;
    m->posY = y;
}

/* Calls `record->slots[n]` on an icon manager with `label` (slot 0
 * measures and returns the pixel width, slot 2 draws) - a gcc 2.x
 * virtual call through libgcc's `_call_via_r2`. A statement macro so
 * `this` is computed before the label argument, as in the ROM. */
#define ICON_TEXT_CALL(mgrExpr, n, label)                                       \
    ({                                                                          \
        struct bitmap_font *_m = (mgrExpr);                                    \
        struct icon_slot *_s = &_m->record->slots[n];                           \
        _call_via_r2((u8 *)_m + _s->offset, (void *)(label), _s->ptr);           \
    })

/* Sits right after the screen-init BG-load/per-row-stats cluster
 * (`src/save/save_menu_ui.o`, ROM `0x080047F8`-`0x08004914`) and
 * before the settings-row flag-test/wrapper cluster
 * (`src/save/save_menu_ui.c`, ROM `0x08004A50` onward). Both
 * functions were NAKED transcriptions until the issue #4/#6/#8 retry
 * (docs/matching/archive/issue-4-6-8-naked-retry.md); they match as plain C
 * under both compilers. */

/* `arg1`/`arg2` are plain coordinate values here (not pointers - the
 * ROM does raw integer arithmetic on them, `arg1+0x1d`/`arg2+0xc`),
 * used as the on-screen anchor for a centered numeric glyph (label
 * 0x25) into gSmallFont.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers, the draws written as `record->slots[n]` virtual calls
 * (`ICON_TEXT_CALL`). See docs/matching/archive/issue-4-6-8-naked-retry.md. */
void DrawEmptySlotLabel(struct save_menu *self, s32 arg1, s32 arg2, u8 arg3)
{
    s32 x = arg1 + 0x1d;
    s32 y = arg2 + 0xc;
    s32 w;

    if (arg3)
        FontSetPalette(gSmallFont, ((self->flags >> 2) & 1) ? 1 : 2);
    else
        FontSetPalette(gSmallFont, 0);
    w = ICON_TEXT_CALL(gSmallFont, 0, GetUiText(0x25));
    set_icon_mgr_pos(gSmallFont, x - w / 2, y);
    ICON_TEXT_CALL(gSmallFont, 2, GetUiText(0x25));
}

/* Draws a centered label (from the runtime string table via
 * GetUiText) into gLargeFont's icon pair - `self` is unused.
 * Matches DrawPowerDialog's (src/menus/power_dialog_draw.c) centered-icon shape
 * exactly, just for a single label rather than flanking a number.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers with the same `ICON_TEXT_CALL` virtual-call macro and no
 * pins. See docs/matching/archive/issue-4-6-8-naked-retry.md. */
void DrawSaveMenuTitle(struct save_menu *self, s32 labelIndex)
{
    s32 w;

    FontSetPalette(gLargeFont, 0);
    w = ICON_TEXT_CALL(gLargeFont, 0, GetUiText(labelIndex));
    set_icon_mgr_pos(gLargeFont, (0xf0 - w) >> 1, 6);
    ICON_TEXT_CALL(gLargeFont, 2, GetUiText(labelIndex));
}

/* Bit-2 flag test repeated throughout this chunk's functions - matches
 * GetSaveMenuBlinkPalette's own trivial body: caller-visible "1" (set) vs "2"
 * (clear). */
s32 GetSaveMenuBlinkPalette(struct save_menu *self)
{
    if ((self->flags >> 2) & 1) {
        return 1;
    }
    return 2;
}

/* `self` is never read, but SaveMenuLinkInput passes it (the ROM's call
 * sets it up in r0). */
void EndLinkSaveTransfer(struct save_menu *self)
{
    struct link_session *p = gLinkSession;
    ResetLinkSession(p);
    p->enabled = 0;
}

void BeginLinkSaveTransfer(struct save_menu *self)
{
    ResetLinkSession(gLinkSession);
    gLinkSession->enabled = 1;
    ResetSaveData(self->linkSave);
}

void DrawSaveMenuConfirmDelete(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1d);
    DrawSaveSlots(self, self->cartSave, self->pendingSlot);
    DrawYesNoPrompt(self, 0x26);
}

void DrawSaveMenuDelete(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1d);
    DrawSaveSlots(self, self->cartSave, self->cursor);
    DrawSaveMenuCancel(self, self->cursor == 4);
}

void DrawSaveMenuOverwrite(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1e);
    DrawSaveSlots(self, self->cartSave, self->pendingSlot);
    DrawYesNoPrompt(self, 0x27);
}

void DrawSaveMenuSave(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1e);
    DrawSaveSlots(self, self->cartSave, self->cursor);
    DrawSaveMenuCancel(self, self->cursor == 4);
}

void DrawSaveMenuMessage(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1c);
    DrawSaveMenuMessageLines(self, self->messageLine1, self->messageLine2);
}

void DrawSaveMenuLoadLink(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1c);
    DrawSaveSlots(self, self->linkSave, self->cursor);
    DrawSaveMenuCancel(self, self->cursor == 4);
}

void DrawSaveMenuLoad(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1b);
    DrawSaveSlots(self, self->cartSave, self->cursor);
    DrawSaveMenuCancel(self, self->cursor == 4);
}

void DrawSaveMenu(struct save_menu *self)
{
    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    if ((u32)self->state <= 0xa) {
        switch (self->state) {
        case 0:
            DrawSaveMenuMain(self);
            break;
        case 1:
            DrawSaveMenuLoad(self);
            break;
        case 2:
            DrawSaveMenuLoadLink(self);
            break;
        case 3:
        case 4:
            DrawSaveMenuMessage(self);
            break;
        case 5:
            DrawSaveMenuSave(self);
            break;
        case 6:
            DrawSaveMenuDelete(self);
            break;
        case 9:
            DrawSaveMenuOverwrite(self);
            break;
        case 7:
            DrawSaveMenuConfirmDelete(self);
            break;
        case 8:
            break;
        case 10:
            break;
        default:
            break;
        }
    }
    HideUnusedOamEntries(gOamBuffer);
}

void DeleteSaveSlot(struct save_menu *self, s32 arg1)
{
    u8 buf[0x70];

    ReadSaveSlot(self->cartSave, arg1, buf);
    EraseSaveSlot(self->cartSave, arg1);
    if (StoreSaveData(self->cartSave)) {
        WriteSaveSlot(self->cartSave, arg1, buf);
    }
}
asm(".align 2, 0");
