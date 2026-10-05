#include "core.h"
#include "vram_pool.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "frontend.h"

extern struct oam_shadow_buffer *gOamBuffer;
extern struct palette_cache *gPaletteCache;

extern void FreeUnlockedPaletteSlots(struct palette_cache *self);
extern void CommitOamBuffer(struct oam_shadow_buffer *arg0);
extern void UploadPaletteCache(struct palette_cache *arg0);
extern void FlushVramDmaQueue(void);

extern u8 gMenuSkyBg[];

extern void *OperatorNew(s32 size);
extern void OperatorDelete(void *self);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern void *InitBgSetup(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 GetBgSetupControl(void *buf);
extern void FadeBrightness(u8 flags, s32 frameDelay, u8 sync);
extern void SetObjMapping1D(void);
extern void ShowObj(void);
extern void SetDispcntMode(s32 val);
extern void CommitDispcnt(void);

/* Resets `self`'s two byte flags, requests a BG tile/map graphics
 * package, and sets BG0's control register from it - a shared "load my
 * background" helper for the widget above. */
void LoadLanguageSelectBg(struct language_select *self)
{
    u8 buf[0x10];
    u32 zero = 0;
    s32 a;
    register s32 b asm("r2");
    register s32 mask asm("r1");

    *(u16 *)&self->field_c = zero;
    a = 0x40;
    a |= self->field_c;
    a &= -8;
    a |= 1;
    self->field_c = a;
    b = 1;
    b |= self->field_d;
    mask = -3;
    b &= mask;
    b |= 0x10;
    self->field_d = b;

    InitBgSetup(buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(buf, gMenuSkyBg);
    REG_BG0CNT = GetBgSetupControl(buf);
    *(vu32 *)REG_ADDR_BG0HOFS = zero;
}

s32 LanguageSelectBlink(struct language_select *self)
{
    if ((self->frame >> 2) & 1) {
        return 1;
    }
    return 2;
}

void CommitLanguageSelectFrame(struct language_select *self)
{
    REG_DISPCNT = *(u16 *)&self->field_c;
    *(vu32 *)REG_ADDR_BG0HOFS = 0;
    UploadPaletteCache(gPaletteCache);
    CommitOamBuffer(gOamBuffer);
    FlushVramDmaQueue();
}

void DestroyLanguageSelect(struct language_select *self, u32 flags)
{
    void *starfield = self->starfield;

    if (starfield != NULL) {
        DestroyStarfield(starfield, 3);
    }
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Loads the widget's graphics, sets up its (16-tile) map/palette upload
 * request, and kicks off the fade/screen machinery. Returns `self`. */
void *InitLanguageSelect(struct language_select *self)
{
    FreeUnlockedPaletteSlots(gPaletteCache);
    InitLanguageSelectGraphics(self);
    LoadLanguageSelectBg(self);
    self->starfield = InitStarfield(OperatorNew(0x14));
    FadeBrightness(0x80, 1, 0);
    SetObjMapping1D();
    ShowObj();
    SetDispcntMode(1);
    CommitDispcnt();
    return self;
}

void CloseLanguageSelect(void)
{
    FadeBrightness(0, 1, 0);
    if (gLanguageSelect != NULL) {
        DestroyLanguageSelect(gLanguageSelect, 3);
    }
    gLanguageSelect = NULL;
    FreeUnlockedPaletteSlots(gPaletteCache);
}

void OpenLanguageSelect(void)
{
    FreeUnlockedPaletteSlots(gPaletteCache);
    gLanguageSelect = InitLanguageSelect(OperatorNew(0x14));
}
