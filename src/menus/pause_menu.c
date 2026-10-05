#include "core.h"
#include "audio.h"
#include "actor.h"
#include "bitmap_font.h"
#include "vram_pool.h"
#include "pause_menu.h"
#include "memory.h"
#include <agb_syscall.h>
#include "text.h"

extern void StopAmbientSfx(struct AudioContext *self);
extern void WaitForVBlank(void);
extern struct palette_cache *gPaletteCache;
extern struct AudioContext *gAudioContext;
extern struct vram_upload_cursor *gObjVramCursor;
extern u8 gSpriteBankTable[];
extern u8 gPauseMenuPalette[];
extern void SetPaletteCacheSource(struct palette_cache *self, u16 count, const u8 *records);
extern s32 ClaimPaletteSlot(struct palette_cache *self, s32 index);
extern void DestroyPaletteCache(struct palette_cache *self, u32 flags);
extern void *_call_via_r1(void *arg0, void *fn);
extern void ResetObjVram(struct vram_upload_cursor *self);
extern void *OperatorNew(s32 size);
extern struct pause_menu *InitPauseMenu(struct pause_menu *self);
extern s32 PauseMenuLoop(struct pause_menu *self);
extern void DestroyPauseMenu(struct pause_menu *self, u32 flags);

/* The composite pause/options screen's own constructor/driver
 * (docs/rom_map.md's "overlay_ui" section, "one composite pause/options
 * screen"). Runs the whole screen synchronously to completion: frees
 * pending heap bytes, resets the audio channel, clears palette color 0
 * and DISPCNT, swaps `gPaletteCache` for a fresh 16-slot tile cache
 * sized for this screen's icon graphics (seeding slot 15 from
 * `gPauseMenuPalette`), re-inits both icon managers (copying
 * `tileCount` between them and firing each one's slot-6 trampoline, the
 * same `_call_via_r1` pattern documented throughout `bitmap_font.h`),
 * builds the screen object (`InitPauseMenu`) and hands it to the blocking
 * cursor/confirm/cancel driver (`PauseMenuLoop`), then tears the screen
 * down (`DestroyPauseMenu`, flags=3) and restores the original tile cache
 * before returning `PauseMenuLoop`'s result.
 *
 * Matched in the second near-miss sweep (the old draft was 54 halfwords
 * off). The ROM never shares the 0x12c offset constant between the
 * `tileCount` reads; it rebuilds it for each one. Reading `tileCount`
 * through the `static inline` accessor `mgr_12c` stops CSE from sharing
 * it, which frees the register the draft spent on it and lets
 * &gPaletteCache/&gSmallFont/&gLargeFont/
 * &gObjVramCursor land in r6/r4/r5/r7 as in the ROM. The two
 * `tileCount` reads for the VRAM reservation are taken into locals
 * before `gObjVramCursor` is loaded, and the tile cache's base is
 * read into a local before `gPauseMenuPalette`'s address, both to
 * match the ROM's load order. The two `0`s still come from
 * inline-function parameters, which CSE shares into r8. */
extern struct palette_cache *InitPaletteCache(void *mem);

/* Fires an icon manager's slot-6 method (a gcc 2.x virtual call). */
#define ICON_SLOT6_CALL(mgr)                                                   \
    if (1)                                                                     \
    {                                                                          \
        struct icon_slot *_s = &(mgr)->record->slots[6];                       \
        ((void (*)(void *))_s->ptr)((u8 *)(mgr) + _s->offset);                 \
    } else (void)0

struct pause_gfx_pkg {
    u8 unused_00[8];
    const u8 *records;
    u8 unused_0c[2];
    u16 count;
};

static inline void init_icon_mgr(struct bitmap_font *mgr, u32 base)
{
    mgr->tileBase = base;
    ICON_SLOT6_CALL(mgr);
}

static inline void reserve_icon_vram(u32 n)
{
    gObjVramCursor->baseTile = n;
    ResetObjVram(gObjVramCursor);
}

/* Keeps CSE from sharing the 0x12c offset between reads (see above). */
static inline u32 mgr_12c(struct bitmap_font *m)
{
    return m->tileCount;
}

s32 RunPauseMenu(void)
{
    struct palette_cache *oldCache;
    struct pause_menu *screen;
    s32 result;

    mem_free_bytes(MEM_HEAP_BOTH);
    StopAmbientSfx(gAudioContext);
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;

    oldCache = gPaletteCache;
    gPaletteCache = InitPaletteCache(OperatorNew(sizeof(struct palette_cache)));
    SetPaletteCacheSource(gPaletteCache, ((struct pause_gfx_pkg *)gSpriteBankTable)->count,
                ((struct pause_gfx_pkg *)gSpriteBankTable)->records);
    ClaimPaletteSlot(gPaletteCache, 0xf);
    {
        u8 *dst = (u8 *)gPaletteCache;

        CpuSet(gPauseMenuPalette, dst + (0x83 << 2), 0x10);
    }

    FontResetPalette(gSmallFont);
    FontResetPalette(gLargeFont);
    init_icon_mgr(gSmallFont, 0);
    init_icon_mgr(gLargeFont, mgr_12c(gSmallFont));
    {
        u32 a = mgr_12c(gSmallFont);
        u32 b = mgr_12c(gLargeFont);

        gObjVramCursor->baseTile = a + b;
        ResetObjVram(gObjVramCursor);
    }

    screen = InitPauseMenu(OperatorNew(0xd4));
    result = PauseMenuLoop(screen);
    if (screen != NULL)
        DestroyPauseMenu(screen, 3);

    reserve_icon_vram(0);
    if (gPaletteCache != NULL)
        DestroyPaletteCache(gPaletteCache, 3);
    gPaletteCache = oldCache;
    mem_free_bytes(MEM_HEAP_BOTH);
    return result;
}

extern void *InitBgSetup(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern s32 GetBgSetupControl(void *buf);
extern void *gLevelState;
extern void ***gSpriteBankSet;
extern void *PackSaveData(void *arg0);
extern void InitPauseMenuInfo(struct pause_menu *self);
extern struct actor *InitUiSpriteObj(struct actor *part);
extern s32 RandRange(s32 max);
extern u8 gPauseMenuBg[];
extern u8 gPauseMenuRows[];

/* Same "recurring screen-constructor shape" docs/rom_map.md's overlay_ui
 * section documents for InitPauseMenu/InitPowerDialog/InitPauseTimeTrialPage: `self`
 * (allocated by the caller, `RunPauseMenu`, as a fresh 0xd4-byte
 * `struct pause_menu`) gets `InitBgSetup` init, a local
 * BLDCNT/BLDY/DISPCNT setup (`field_c8`/`field_cc`/`field_d0`, the same
 * fields `CommitPauseMenuFrame` applies), `LoadGraphicsPackage`, a row-stats
 * handle from `gLevelState`, then hands off to `InitPauseMenuInfo` to
 * build the results sub-widgets. Afterwards builds one more icon (the
 * row-cursor/highlight icon at `field_c0`) directly, seeds the settings-
 * row bookkeeping fields (`field_14`/`field_18`/`field_1c`/`field_20`/
 * `field_24`/`field_28`), and applies BG0CNT/BG0HOFS before returning
 * `self` unchanged. */
struct pause_menu *InitPauseMenu(struct pause_menu *self)
{
    register s32 zero asm("r6");

    InitBgSetup(self, 0, 0x1f, 0, 3);

    {
        register u32 *c8Addr asm("r4") = &self->field_c8;
        register s32 orTen asm("r5");
        register s32 one asm("r3");
        u8 v;

        zero = 0;
        *c8Addr = zero;
        v = 0xc0;
        v |= *(u8 *)c8Addr;
        v |= 0x20;
        one = 1;
        v |= one;
        v |= 2;
        v |= 4;
        v |= 8;
        orTen = 0x10;
        v |= orTen;
        *(u8 *)c8Addr = v;

        {
            register u8 *addr asm("r2") = &self->field_cc;
            register s32 mask asm("r0") = -0x20;
            register u8 byte asm("r1") = *addr;
            mask &= byte;
            mask |= orTen;
            *addr = mask;

            {
                register u32 bldcntAddr asm("r1") = REG_ADDR_BLDCNT;
                register u32 bldcntVal asm("r0") = *c8Addr;
                register u32 bldyVal asm("r0");

                asm volatile("str %1, [%0]\n\tadd %0, %0, #4" : "+r" (bldcntAddr) : "r" (bldcntVal));
                {
                    register u8 byte2 asm("r2") = *addr;
                    bldyVal = ((u32)byte2 << 27) >> 27;
                }
                *(vu16 *)bldcntAddr = bldyVal;
            }
        }

        {
            register void *addr asm("r2") = &self->field_d0;
            register s32 v2 asm("r0");
            register s32 r1v asm("r1");

            *(u16 *)addr = zero;
            v2 = 0x40;
            r1v = *(u8 *)addr;
            v2 |= r1v;
            r1v = -8;
            v2 &= r1v;
            *(u8 *)addr = v2;
        }

        {
            u8 *hi = (u8 *)self + 0xd1;
            register u8 byte2 asm("r2") = *hi;
            one |= byte2;
            one |= orTen;
            *hi = one;
        }

        LoadGraphicsPackage(self, gPauseMenuBg);
        self->field_10 = PackSaveData(gLevelState);
        InitPauseMenuInfo(self);

        {
            register struct settings_icon_actor **field_c0_addr asm("r4") = (struct settings_icon_actor **)((u8 *)c8Addr - 8);
            struct settings_icon_actor *icon = (struct settings_icon_actor *)InitUiSpriteObj((struct actor *)OperatorNew(0x40));

            *field_c0_addr = icon;
            {
                register u8 *base asm("r1") = (u8 *)(**gSpriteBankSet);
                asm volatile("mov r3, #0x8a\n\tlsl r3, r3, #2\n\tadd %0, %0, r3" : "+r" (base) :: "r3");
                icon->field_20 = (void **)base;
            }
            {
                register s32 _ret asm("r0") = GetSpriteAnimPaletteSlot((struct actor *)icon);
                register u8 *_addr asm("r2") = &(*field_c0_addr)->field_29;
                register s32 _mask asm("r1");
                register u8 _byte asm("r3");
                _mask = 0xf;
                _ret &= _mask;
                asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r" (_mask));
                _byte = *_addr;
                _mask &= _byte;
                _mask |= _ret;
                *_addr = _mask;
            }
            {
                struct actor *base = &(*field_c0_addr)->base;
                base->x = 0xee << 7;
                base->y = 0xbc << 7;
            }
        }
    }

    self->field_c4 = (u16)RandRange(0x78) + 0x78;

    self->field_14 = gPauseMenuRows;
    self->field_18 = zero;
    {
        s32 v;
        if (*((u8 *)gLevelState + 0x8c) != 0) {
            v = 5;
            asm volatile(".pool");
        } else {
            v = 4;
        }
        self->field_1c = v;
    }
    self->field_20 = 0x10;
    self->field_24 = 0;
    self->field_28 = 0xb4;

    REG_BG0CNT = GetBgSetupControl(self);
    *(vu32 *)REG_ADDR_BG0HOFS = 0;

    return self;
}

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void OperatorDelete(void *arg0);

/* Same "re-probe an actor's own category-table slot 0x50/0x54" shape
 * already established by DestroyPowerDialog (src/menus/power_dialog_draw.c) -
 * width-re-measures a single icon's currently-drawn text in place
 * (the return value is discarded), skipping a NULL slot entirely. A
 * `#define`, not a helper function, so it inlines identically at each
 * of the six call sites below - matching DestroyPowerDialog's own inlined
 * shape rather than adding a real call the ROM doesn't make. */
#define REFRESH_ICON_WIDGET(iconExpr) \
    do { \
        struct settings_icon_actor *_icon = (iconExpr); \
        if (_icon != NULL) { \
            u8 *_p = (u8 *)_icon->base.table + 0x50; \
            _call_via_r2((u8 *)_icon + *(s16 *)_p, (void *)3, *(void **)(_p + 4)); \
        } \
    } while (0)

/* Refreshes every icon field/array the results screen owns (field_c0,
 * field_bc, iconsB0[3], icons9c[5], icons8c[4], field_88 - in that
 * order) via `REFRESH_ICON_WIDGET` above, then frees `self` if bit 0 of
 * `flags` is set - the same trailing shape DestroyPowerDialog uses for its
 * own single-icon `arg0`. */
void DestroyPauseMenu(struct pause_menu *selfArg, u32 flagsArg)
{
    register struct pause_menu *self asm("r6") = selfArg;
    register u32 flags asm("sl") = flagsArg;
    struct settings_icon_actor **icons9cBase;
    register struct settings_icon_actor **icons8cBase asm("r8") = NULL;
    register struct settings_icon_actor **field88Addr asm("r9") = NULL;
    struct settings_icon_actor **p;
    s32 i;

    REFRESH_ICON_WIDGET(self->field_c0);
    REFRESH_ICON_WIDGET(self->field_bc);

    icons9cBase = self->icons9c;
    icons8cBase = self->icons8c;
    field88Addr = &self->field_88;
    p = self->iconsB0;

    for (i = 2; i >= 0; i--) {
        REFRESH_ICON_WIDGET(*p);
        p++;
    }
    p = icons9cBase;
    for (i = 4; i >= 0; i--) {
        REFRESH_ICON_WIDGET(*p);
        p++;
    }
    p = icons8cBase;
    for (i = 3; i >= 0; i--) {
        REFRESH_ICON_WIDGET(*p);
        p++;
    }

    REFRESH_ICON_WIDGET(*field88Addr);

    if (flags & 1) {
        OperatorDelete(self);
    }
}
/* Trailing byte-padding gotcha (see docs/matching.md/
 * matching_decomp_alignment_fix memory): the ROM pads the gap before
 * the next function (PauseMenuLoop) with zero bytes (an explicit
 * `.align 2, 0` in the original assembly), but this compiler's own
 * default inter-function padding is a `mov r8, r8` NOP-equivalent
 * instead. */
asm(".align 2, 0");
