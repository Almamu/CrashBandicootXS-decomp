#include "core.h"
#include "gba/dma_macros.h"
#include "system.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "text.h"
#include "frontend.h"
#include "audio.h"
#include "gfx.h"
#include "globals.h"

/* The language menu shown at boot (OpenLanguageSelect/RunLanguageSelect/
 * CloseLanguageSelect, called from MainLoop): up/down cycles `language`
 * through the six entries of gLanguageNames ("english", "français",
 * "deutsch", "español", "italiano", "nederlands", drawn with gSmallFont
 * over a starfield), A or START confirms, and MainLoop stores the
 * result in gLanguage, which picks the gUiText<Lang>/cutscene tables.
 * Sits at the very start of the address range docs/audio.md calls the
 * GAX2 engine, but is game-side code that merely uses PlaySfx.
 * `struct language_select` is in frontend.h. */

/* Loads a "tagged" asset (see LoadTaggedAsset, src/system/asset.c)
 * into a freshly allocated buffer, then queues a DMA3 transfer from that
 * buffer out to `dest` - `unused` (r0) is never read. */
void LoadTaggedAssetBuffered(void *unused, const void *asset, void *dest)
{
    u32 val = *(const u32 *)asset;
    struct dma_regs *dma;
    void *buf;
    u32 cnt;

    val >>= 8;
    buf = OperatorNewArray(val);
    LoadTaggedAsset(asset, buf);
    dma = (struct dma_regs *)REG_ADDR_DMA3SAD;
    dma->src = (u32)buf;
    dma->dst = (u32)dest;
    val >>= 1;
    dma->cnt = val | 0x80000000;
    cnt = dma->cnt;
    if (buf != NULL) {
        OperatorDeleteArray(buf);
    }
}

void nullsub_7(void)
{
}

void DestroyCompanyLogos(void *self, u32 flags)
{
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* The company-logo actor's destructor (RunCompanyLogos, title_screen.c):
 * slot 1 of gLogoActorVtable, so no `bl` reaches it - it is
 * called through the vtable with the deleting flags 3. It frees the two
 * VRAM tile blocks InitLogoActor allocated, drops back to the base
 * gActorVtable, unlinks the actor from the actor ring and frees it on
 * flags bit 0 - the same shape as DestroyActor. */


void DestroyLogoActor(struct actor_self *self, u32 flags)
{
    self->vtable = (struct actor_vtable *)gLogoActorVtable;
    FreeVramTileBlock(gLogoActorTiles[0]);
    FreeVramTileBlock(gLogoActorTiles[1]);
    self->vtable = (struct actor_vtable *)gActorVtable;
    self->next->prev = self->prev;
    self->prev->next = self->next;
    if (flags & 1) {
        mem_free((u8 *)self);
    }
}

/* Runs the widget: resets it, draws/flushes once, then polls input each
 * frame (dispatching newly-pressed keys to LanguageSelectInput) until it signals
 * done via `field_4`, returning the final selected value in `field_8`. */
s32 RunLanguageSelect(void)
{
    gLanguageSelect->language = 0;
    gLanguageSelect->frame = 0;
    gLanguageSelect->done = 0;
    DrawLanguageSelect(gLanguageSelect);
    WaitForVBlank();
    CommitLanguageSelectFrame(gLanguageSelect);

    while (gLanguageSelect->done == 0) {
        u16 keys;
        u16 *addr;

        UpdateKeys(gInput);
        addr = &gKeys.half.held;
        keys = *(u16 *)((u8 *)addr + 2);
        LanguageSelectInput(gLanguageSelect, keys);
        DrawLanguageSelect(gLanguageSelect);
        WaitForVBlank();
        CommitLanguageSelectFrame(gLanguageSelect);
        UpdateStarfield(gLanguageSelect->starfield);
    }

    return gLanguageSelect->language;
}

/* Dispatches one frame's newly-pressed `flags` for the widget above:
 * bit 3 or bit 0 confirms/cancels (sets `field_4` to end the loop, sfx
 * 0x49); bit 6/bit 7 decrement/increment the 0-5 `field_8` value
 * (wrapping around, sfx 0x46). `field_0` is a free-running frame
 * counter, incremented every call regardless. */
void LanguageSelectInput(struct language_select *self, u32 flags)
{
    if (flags & 8) {
        self->done = 1;
        goto confirm;
    } else if (flags & 1) {
        self->done = 1;
    confirm:
        PlaySfx(gAudioContext, 0x49, 0x100);
    } else if (flags & 0x40) {
        self->language--;
        if (self->language < 0) {
            self->language = 5;
        }
        PlaySfx(gAudioContext, 0x46, 0x100);
    } else if (flags & 0x80) {
        self->language++;
        if (self->language > 5) {
            self->language = 0;
        }
        PlaySfx(gAudioContext, 0x46, 0x100);
    }
    self->frame = (self->frame + 1) & 0xff;
}

/* `_call_via_r2`: calls `fn(self, arg)` (an bitmap_font method). */
extern s32 _call_via_r2(void *self, void *arg, void *fn);

/* The language menu's (src/frontend/language_select.c) per-frame draw
 * loop: for each of the six language names (0-5) it sets the shared
 * overlay frame (`gSmallFont`: 1 or 2 from `LanguageSelectBlink`'s blink
 * state on the currently selected entry `language`, 0 elsewhere), measures
 * that slot's glyph with the icon manager's `slots[0]` method, centers
 * it horizontally, and draws it with `slots[2]` at a Y stepping by 0xa
 * from 0x32.
 *
 * Was NAKED ("many-register allocation ceiling"); calling the method
 * trampoline `_call_via_r2` directly with the glyph assigned inside the
 * first call's argument list (so it's loaded between `this` and the
 * method pointer, as the ROM does) matches outright - see
 * docs/matching/gax-toolchain-retry.md. */
void DrawLanguageSelect(struct language_select *self)
{
    s32 y;
    s32 i;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    y = 0x32;
    for (i = 0; i <= 5; i++) {
        const u8 *glyph;
        s32 x;

        if (i == self->language)
            FontSetPalette(gSmallFont, LanguageSelectBlink(self));
        else
            FontSetPalette(gSmallFont, 0);
        x = (240 - _call_via_r2((u8 *)gSmallFont + gSmallFont->record->slots[0].offset,
                               (void *)(glyph = gLanguageNames[i]),
                               gSmallFont->record->slots[0].ptr)) >> 1;
        gSmallFont->posX = x;
        gSmallFont->posY = y;
        _call_via_r2((u8 *)gSmallFont + gSmallFont->record->slots[2].offset, (void *)glyph,
                    gSmallFont->record->slots[2].ptr);
        y += 10;
    }
    HideUnusedOamEntries(gOamBuffer);
}

/* Resets several OAM-manager globals, then hand-fills
 * `gPaletteCache`'s (`struct palette_cache`, include/vram_pool.h)
 * `slots[0]`-`slots[3]` with 4 fixed 32-byte OBJ tiles copied from
 * `gLanguageSelectPalette0`..`gLanguageSelectPalette3`, and finally runs
 * `gSmallFont`'s/`gLargeFont`'s `record->slots[6]` method
 * (`_call_via_r1`) plus a VRAM reserve (`ReserveObjVram`) for each, copying
 * `tileCount` into the other manager's `tileBase`.
 *
 * Was NAKED: the ROM rematerializes the 0x108/0x12c/0x130 field-offset
 * constants after every call instead of keeping them in callee-saved
 * registers. Matched with the idiom from credits.c's
 * `InitCredits`: the two icon-manager steps as `static inline` helpers
 * taking the manager as a parameter (each expansion recomputes its own
 * offsets; the E0 base is read from DC before E0 itself), plus one
 * `zero` local shared by the `field_8`/`tileBase` stores - the 0 the
 * ROM keeps in r8. Matches under both compilers. */
extern void _call_via_r1(void *self, void *fn);

static inline void IconSetBase(struct bitmap_font *m, u32 base)
{
    struct icon_slot *slot;

    m->tileBase = base;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserveVram(void *c, struct bitmap_font *m)
{
    ReserveObjVram(c, m->tileCount << 5);
}

void InitLanguageSelectGraphics(void *unused)
{
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
    {
        struct palette_cache *cache = gPaletteCache;
        u16 *destA = (u16 *)cache->slots[0];
        u16 *destB = (u16 *)cache->slots[2];

        for (i = 0; i < 16; i++) {
            destA[i] = gLanguageSelectPalette0[i];
            destA[i + 0x10] = gLanguageSelectPalette1[i];
            destB[i] = gLanguageSelectPalette2[i];
            destB[i + 0x10] = gLanguageSelectPalette3[i];
        }
    }
    {
        u32 zero = 0;

        FontSetPalette(gSmallFont, 0);
        FontSetPalette(gLargeFont, 0);
        ((u32 *)gObjVramCursor)[2] = zero;
        ResetObjVram(gObjVramCursor);
        ResetObjVram(gObjVramCursor);
        IconSetBase(gSmallFont, zero);
        IconReserveVram(gObjVramCursor, gSmallFont);
        {
            u32 base = gSmallFont->tileCount;

            IconSetBase(gLargeFont, base);
        }
        IconReserveVram(gObjVramCursor, gLargeFont);
    }
    MarkObjVram(gObjVramCursor);
}
