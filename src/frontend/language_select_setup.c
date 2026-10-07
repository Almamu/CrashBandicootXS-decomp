#include "core.h"
#include "match.h"
#include "vram_pool.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "frontend.h"
#include "gfx.h"
#include "globals.h"


/* Resets `self`'s two byte flags, requests a BG tile/map graphics
 * package, and sets BG0's control register from it - a shared "load my
 * background" helper for the widget above. */
void LoadLanguageSelectBg(struct language_select *self)
{
    struct bg_setup buf;
    u32 zero = 0;
    s32 a;
    MATCH_HOLD_REG(s32, b, r2);
    MATCH_HOLD_REG(s32, mask, r1);

    *(u16 *)&self->dispcntLo = zero;
    a = 0x40;
    a |= self->dispcntLo;
    a &= -8;
    a |= 1;
    self->dispcntLo = a;
    b = 1;
    b |= self->dispcntHi;
    mask = -3;
    b &= mask;
    b |= 0x10;
    self->dispcntHi = b;

    InitBgSetup(&buf, 2, 0x1e, 1, 3);
    LoadGraphicsPackage(&buf, &gMenuSkyBg);
    REG_BG0CNT = GetBgSetupControl(&buf);
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
    REG_DISPCNT = *(u16 *)&self->dispcntLo;
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
    FadeBrightness(FADE_FLAG_IN, 1, 0);
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
