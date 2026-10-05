#include "core.h"
#include "pause_options_screen.h"

/* A small "load my background" sub-widget - the same field_c/field_d
 * bit-flags-pair idiom as `struct counter_widget`
 * (src/audio/counter_selector_setup.c's LoadLanguageSelectBg), just at offsets
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
extern u8 gStaticData_0816C484[];

/* Same shape as LoadLanguageSelectBg (src/audio/counter_selector_setup.c) - reset
 * two bit-flag bytes, request a BG tile/map graphics package, set BG0's
 * control register from it - plus zeroing `field_0`, which LoadLanguageSelectBg's
 * counter_widget doesn't have. */
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
    LoadGraphicsPackage(buf, gStaticData_0816C484);
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
void RefreshSaveSlotSummaries(struct pause_options_screen *self, void *handle)
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

void LoadSaveMenuData(struct pause_options_screen *self)
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
