#include "core.h"
#include "icon_manager.h"

struct counter_widget {
    u32 frame;
    u8 done;
    u8 pad_5[3];
    s32 language;
};

extern void *gOamBuffer;
extern void *gObjVramCursor;
extern struct icon_manager *gSmallFont;
/* The six language names. */
extern void *gLanguageNames[6];
extern void ResetOamBuffer(void *arg0);
extern void RewindObjVram(void *arg0);
extern void HideUnusedOamEntries(void *arg0);
extern s32 FontSetPalette(struct icon_manager *mgr, u8 frame);
extern s32 LanguageSelectBlink(struct counter_widget *self);
/* `_call_via_r2`: calls `fn(self, arg)` (an icon_manager method). */
extern s32 _call_via_r2(void *self, void *arg, void *fn);

/* The language menu's (src/audio/counter_selector.c) per-frame draw
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
void DrawLanguageSelect(struct counter_widget *self)
{
    s32 y;
    s32 i;

    ResetOamBuffer(gOamBuffer);
    RewindObjVram(gObjVramCursor);
    y = 0x32;
    for (i = 0; i <= 5; i++) {
        void *glyph;
        s32 x;

        if (i == self->language)
            FontSetPalette(gSmallFont, LanguageSelectBlink(self));
        else
            FontSetPalette(gSmallFont, 0);
        x = (240 - _call_via_r2((u8 *)gSmallFont + gSmallFont->record->slots[0].offset,
                               glyph = gLanguageNames[i],
                               gSmallFont->record->slots[0].ptr)) >> 1;
        gSmallFont->posX = x;
        gSmallFont->posY = y;
        _call_via_r2((u8 *)gSmallFont + gSmallFont->record->slots[2].offset, glyph,
                    gSmallFont->record->slots[2].ptr);
        y += 10;
    }
    HideUnusedOamEntries(gOamBuffer);
}

/* Resets several OAM-manager globals, then hand-fills
 * `gPaletteCache`'s (`struct palette_cache`, include/vram_pool.h)
 * `slots[0]`-`slots[3]` with 4 fixed 32-byte OBJ tiles copied from
 * `gStaticData_0817E72C`/`_74C`/`_76C`/`_78C`, and finally runs
 * `gSmallFont`'s/`gLargeFont`'s `record->slots[6]` method
 * (`_call_via_r1`) plus a VRAM reserve (`ReserveObjVram`) for each, copying
 * `tileCount` into the other manager's `tileBase`.
 *
 * Was NAKED: the ROM rematerializes the 0x108/0x12c/0x130 field-offset
 * constants after every call instead of keeping them in callee-saved
 * registers. Matched with the idiom from actor_part131.c's
 * `InitCredits`: the two icon-manager steps as `static inline` helpers
 * taking the manager as a parameter (each expansion recomputes its own
 * offsets; the E0 base is read from DC before E0 itself), plus one
 * `zero` local shared by the `field_8`/`tileBase` stores - the 0 the
 * ROM keeps in r8. Matches under both compilers. */
#include "vram_pool.h"
extern struct palette_cache *gPaletteCache;
extern struct icon_manager *gLargeFont;
extern const u16 gStaticData_0817E72C[16];
extern const u16 gStaticData_0817E74C[16];
extern const u16 gStaticData_0817E76C[16];
extern const u16 gStaticData_0817E78C[16];
extern void WaitForVBlank(void);
extern void CommitOamBuffer(void *arg0);
extern void FreeUnlockedPaletteSlots(struct palette_cache *cache);
extern s32 ClaimPaletteSlot(struct palette_cache *cache, s32 index);
extern void ResetObjVram(void *cursor);
extern s32 ReserveObjVram(void *cursor, s32 size);
extern void MarkObjVram(void *cursor);
extern void _call_via_r1(void *self, void *fn);

static inline void IconSetBase(struct icon_manager *m, u32 base)
{
    struct icon_slot *slot;

    m->tileBase = base;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserveVram(void *c, struct icon_manager *m)
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
            destA[i] = gStaticData_0817E72C[i];
            destA[i + 0x10] = gStaticData_0817E74C[i];
            destB[i] = gStaticData_0817E76C[i];
            destB[i + 0x10] = gStaticData_0817E78C[i];
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
