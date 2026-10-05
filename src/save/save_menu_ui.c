#include "core.h"
#include "save_menu.h"
#include "bitmap_font.h"
#include "text.h"

/* A small "load my background" sub-widget - the same field_c/field_d
 * bit-flags-pair idiom as `struct language_select`
 * (src/frontend/language_select_setup.c's LoadLanguageSelectBg), just at offsets
 * 0x1c/0x1d here - this chunk doesn't include whatever embeds it in a
 * bigger object, so it gets its own minimal type. */
struct bg_widget {
    u32 field_0;
    u8 unused_04[0x1c - 4];
    u8 field_1c;
    u8 field_1d;
};

extern void *InitBgSetup(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern s32 GetBgSetupControl(void *buf);
extern u8 gMenuSkyBg[];

/* Same shape as LoadLanguageSelectBg (src/frontend/language_select_setup.c) - reset
 * two bit-flag bytes, request a BG tile/map graphics package, set BG0's
 * control register from it - plus zeroing `field_0`, which LoadLanguageSelectBg's
 * language_select doesn't have. */
void LoadSaveMenuBg(struct bg_widget *self)
{
    u8 buf[0x10];
    u32 zero = 0;
    s32 a;
    register s32 b asm("r1");

    *(u16 *)&self->field_1c = zero;
    a = 0x40;
    a |= self->field_1c;
    a &= -8;
    a |= 1;
    self->field_1c = a;
    b = 1;
    b |= self->field_1d;
    b &= -3;
    b |= 0x10;
    self->field_1d = b;

    InitBgSetup(buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(buf, gMenuSkyBg);
    self->field_0 = 0;
    REG_BG0CNT = GetBgSetupControl(buf);
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

extern s32 CountClearGems(void *arg0);
extern s32 CountRelics(void *arg0);
extern s32 GetProgressLives(void *arg0);
extern s32 CountCrystals(void *arg0);
extern s32 GetCompletionPercent(void *arg0);
extern u8 IsSaveSlotEmpty(void *handle, s32 rowIndex);
extern void ReadSaveSlot(void *handle, s32 rowIndex, void *buf);

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

extern s32 LoadSaveData(void *arg0);
extern s32 StoreSaveData(void *arg0);
extern void ResetSaveData(void *arg0);

void LoadSaveMenuData(struct save_menu *self)
{
    s32 v = LoadSaveData(self->field_8c);
    if ((u32)(v - 1) <= 3) {
        ResetSaveData(self->field_8c);
        StoreSaveData(self->field_8c);
    }
}

/* Fills `dest` from `src` using the same five-function battery as the
 * loop in RefreshSaveSlotSummaries above - `self` (the screen widget) is passed but
 * never used, matching the ROM exactly. */
void SummarizeProgress(void *self, struct settings_row_stats *dest, void *src)
{
    dest->gems = CountClearGems(src);
    dest->relics = CountRelics(src);
    dest->lives = GetProgressLives(src);
    dest->crystals = CountCrystals(src);
    dest->percent = GetCompletionPercent(src);
}

extern s32 GetUiText(s32 arg0);
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
 * (docs/matching/issue-4-6-8-naked-retry.md); they match as plain C
 * under both compilers. */

/* `arg1`/`arg2` are plain coordinate values here (not pointers - the
 * ROM does raw integer arithmetic on them, `arg1+0x1d`/`arg2+0xc`),
 * used as the on-screen anchor for a centered numeric glyph (label
 * 0x25) into gSmallFont.
 *
 * Once a NAKED transcription; it matches as plain C under both
 * compilers, the draws written as `record->slots[n]` virtual calls
 * (`ICON_TEXT_CALL`). See docs/matching/issue-4-6-8-naked-retry.md. */
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
 * pins. See docs/matching/issue-4-6-8-naked-retry.md. */
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

extern void ResetLinkSession(void *arg0);
extern void *gLinkSession;

void EndLinkSaveTransfer(void)
{
    void *p = gLinkSession;
    ResetLinkSession(p);
    *((u8 *)p + 5) = 0;
}

void BeginLinkSaveTransfer(struct save_menu *self)
{
    ResetLinkSession(gLinkSession);
    *((u8 *)gLinkSession + 5) = 1;
    ResetSaveData(self->field_90);
}

extern void DrawSaveMenuTitle(struct save_menu *self, s32 labelIndex);
extern void DrawSaveSlots(struct save_menu *self, void *handle, s32 arg2);
extern void DrawYesNoPrompt(struct save_menu *self, s32 labelIndex);
extern void DrawSaveMenuCancel(struct save_menu *self, u8 highlight);
extern void DrawSaveMenuMessageLines(struct save_menu *self, s32 label1, s32 label2);

void DrawSaveMenuConfirmDelete(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1d);
    DrawSaveSlots(self, self->field_8c, self->field_24);
    DrawYesNoPrompt(self, 0x26);
}

void DrawSaveMenuDelete(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1d);
    DrawSaveSlots(self, self->field_8c, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

void DrawSaveMenuOverwrite(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1e);
    DrawSaveSlots(self, self->field_8c, self->field_24);
    DrawYesNoPrompt(self, 0x27);
}

void DrawSaveMenuSave(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1e);
    DrawSaveSlots(self, self->field_8c, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

void DrawSaveMenuMessage(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1c);
    DrawSaveMenuMessageLines(self, self->field_14, self->field_18);
}

void DrawSaveMenuLoadLink(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1c);
    DrawSaveSlots(self, self->field_90, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

void DrawSaveMenuLoad(struct save_menu *self)
{
    DrawSaveMenuTitle(self, 0x1b);
    DrawSaveSlots(self, self->field_8c, self->field_10);
    DrawSaveMenuCancel(self, self->field_10 == 4);
}

extern void ResetOamBuffer(void *arg0);
extern void HideUnusedOamEntries(void *arg0);
extern void RewindObjVram(void *arg0);
extern void DrawSaveMenuMain(struct save_menu *self);
extern void *gOamBuffer;
extern void *gObjVramCursor;

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

extern void EraseSaveSlot(void *arg0, s32 arg1);
extern void WriteSaveSlot(void *arg0, s32 arg1, void *buf);

void DeleteSaveSlot(struct save_menu *self, s32 arg1)
{
    u8 buf[0x70];

    ReadSaveSlot(self->field_8c, arg1, buf);
    EraseSaveSlot(self->field_8c, arg1);
    if (StoreSaveData(self->field_8c)) {
        WriteSaveSlot(self->field_8c, arg1, buf);
    }
}
asm(".align 2, 0");
