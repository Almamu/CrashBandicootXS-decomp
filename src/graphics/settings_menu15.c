#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "pause_screen_results.h"
#include "memory.h"

extern void StopAmbientSfx(struct AudioContext *self);
extern void WaitForVBlank(void);
extern struct tile_asset_cache *gUnknown_030012B8;
extern struct AudioContext *gUnknown_030012BC;
extern struct icon_manager *gUnknown_030012DC;
extern struct icon_manager *gUnknown_030012E0;
extern struct vram_upload_cursor *gUnknown_030012FC;
extern u8 gStaticData_084A5600[];
extern u8 gStaticData_0816B2C0[];
extern void sub_8006EF0(struct tile_asset_cache *self, u16 count, const u8 *records);
extern s32 sub_8006D50(struct tile_asset_cache *self, s32 index);
extern void sub_8006F94(struct tile_asset_cache *self, u32 flags);
extern void CpuSet(const void *src, void *dst, u32 cnt);
extern void sub_8028A40(struct icon_manager *self);
extern void *_call_via_r1(void *arg0, void *fn);
extern void sub_8006C4C(struct vram_upload_cursor *self);
extern void *sub_8026EDC(s32 size);
extern struct pause_screen_results *sub_8004EC0(struct pause_screen_results *self);
extern s32 sub_8005100(struct pause_screen_results *self);
extern void sub_8005004(struct pause_screen_results *self, u32 flags);

/* The composite pause/options screen's own constructor/driver
 * (docs/rom_map.md's "overlay_ui" section, "one composite pause/options
 * screen"). Runs the whole screen synchronously to completion: frees
 * pending heap bytes, resets the audio channel, clears palette color 0
 * and DISPCNT, swaps `gUnknown_030012B8` for a fresh 16-slot tile cache
 * sized for this screen's icon graphics (seeding slot 15 from
 * `gStaticData_0816B2C0`), re-inits both icon managers (copying
 * `field_12c` between them and firing each one's slot-6 trampoline, the
 * same `_call_via_r1` pattern documented throughout `icon_manager.h`),
 * builds the screen object (`sub_8004EC0`) and hands it to the blocking
 * cursor/confirm/cancel driver (`sub_8005100`), then tears the screen
 * down (`sub_8005004`, flags=3) and restores the original tile cache
 * before returning `sub_8005100`'s result.
 *
 * Matched in the second near-miss sweep (the old draft was 54 halfwords
 * off). The ROM never shares the 0x12c offset constant between the
 * `field_12c` reads; it rebuilds it for each one. Reading `field_12c`
 * through the `static inline` accessor `mgr_12c` stops CSE from sharing
 * it, which frees the register the draft spent on it and lets
 * &gUnknown_030012B8/&gUnknown_030012DC/&gUnknown_030012E0/
 * &gUnknown_030012FC land in r6/r4/r5/r7 as in the ROM. The two
 * `field_12c` reads for the VRAM reservation are taken into locals
 * before `gUnknown_030012FC` is loaded, and the tile cache's base is
 * read into a local before `gStaticData_0816B2C0`'s address, both to
 * match the ROM's load order. The two `0`s still come from
 * inline-function parameters, which CSE shares into r8. */
extern struct tile_asset_cache *sub_8006FB4(void *mem);

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

static inline void init_icon_mgr(struct icon_manager *mgr, u32 base)
{
    mgr->field_108 = base;
    ICON_SLOT6_CALL(mgr);
}

static inline void reserve_icon_vram(u32 n)
{
    gUnknown_030012FC->field_08 = n;
    sub_8006C4C(gUnknown_030012FC);
}

/* Keeps CSE from sharing the 0x12c offset between reads (see above). */
static inline u32 mgr_12c(struct icon_manager *m)
{
    return m->field_12c;
}

s32 sub_8004D74(void)
{
    struct tile_asset_cache *oldCache;
    struct pause_screen_results *screen;
    s32 result;

    mem_free_bytes(MEM_HEAP_BOTH);
    StopAmbientSfx(gUnknown_030012BC);
    WaitForVBlank();
    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;

    oldCache = gUnknown_030012B8;
    gUnknown_030012B8 = sub_8006FB4(sub_8026EDC(sizeof(struct tile_asset_cache)));
    sub_8006EF0(gUnknown_030012B8, ((struct pause_gfx_pkg *)gStaticData_084A5600)->count,
                ((struct pause_gfx_pkg *)gStaticData_084A5600)->records);
    sub_8006D50(gUnknown_030012B8, 0xf);
    {
        u8 *dst = (u8 *)gUnknown_030012B8;

        CpuSet(gStaticData_0816B2C0, dst + (0x83 << 2), 0x10);
    }

    sub_8028A40(gUnknown_030012DC);
    sub_8028A40(gUnknown_030012E0);
    init_icon_mgr(gUnknown_030012DC, 0);
    init_icon_mgr(gUnknown_030012E0, mgr_12c(gUnknown_030012DC));
    {
        u32 a = mgr_12c(gUnknown_030012DC);
        u32 b = mgr_12c(gUnknown_030012E0);

        gUnknown_030012FC->field_08 = a + b;
        sub_8006C4C(gUnknown_030012FC);
    }

    screen = sub_8004EC0(sub_8026EDC(0xd4));
    result = sub_8005100(screen);
    if (screen != NULL)
        sub_8005004(screen, 3);

    reserve_icon_vram(0);
    if (gUnknown_030012B8 != NULL)
        sub_8006F94(gUnknown_030012B8, 3);
    gUnknown_030012B8 = oldCache;
    mem_free_bytes(MEM_HEAP_BOTH);
    return result;
}

extern void *sub_801E644(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern s32 sub_801E640(void *buf);
extern void *gLevelState;
extern void ***gUnknown_030012D0;
extern void *sub_80236EC(void *arg0);
extern void sub_800599C(struct pause_screen_results *self);
extern struct actor *sub_8008904(struct actor *part);
extern s32 RandRange(s32 max);
extern u8 gStaticData_0816B284[];
extern u8 gStaticData_0816B298[];

/* Same "recurring screen-constructor shape" docs/rom_map.md's overlay_ui
 * section documents for sub_8004EC0/sub_80063D8/sub_8005D44: `self`
 * (allocated by the caller, `sub_8004D74`, as a fresh 0xd4-byte
 * `struct pause_screen_results`) gets `sub_801E644` init, a local
 * BLDCNT/BLDY/DISPCNT setup (`field_c8`/`field_cc`/`field_d0`, the same
 * fields `sub_8006250` applies), `LoadGraphicsPackage`, a row-stats
 * handle from `gLevelState`, then hands off to `sub_800599C` to
 * build the results sub-widgets. Afterwards builds one more icon (the
 * row-cursor/highlight icon at `field_c0`) directly, seeds the settings-
 * row bookkeeping fields (`field_14`/`field_18`/`field_1c`/`field_20`/
 * `field_24`/`field_28`), and applies BG0CNT/BG0HOFS before returning
 * `self` unchanged. */
struct pause_screen_results *sub_8004EC0(struct pause_screen_results *self)
{
    register s32 zero asm("r6");

    sub_801E644(self, 0, 0x1f, 0, 3);

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

        LoadGraphicsPackage(self, gStaticData_0816B284);
        self->field_10 = sub_80236EC(gLevelState);
        sub_800599C(self);

        {
            register struct settings_icon_actor **field_c0_addr asm("r4") = (struct settings_icon_actor **)((u8 *)c8Addr - 8);
            struct settings_icon_actor *icon = (struct settings_icon_actor *)sub_8008904((struct actor *)sub_8026EDC(0x40));

            *field_c0_addr = icon;
            {
                register u8 *base asm("r1") = (u8 *)(**gUnknown_030012D0);
                asm volatile("mov r3, #0x8a\n\tlsl r3, r3, #2\n\tadd %0, %0, r3" : "+r" (base) :: "r3");
                icon->field_20 = (void **)base;
            }
            {
                register s32 _ret asm("r0") = sub_800815C((struct actor *)icon);
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

    self->field_14 = gStaticData_0816B298;
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

    REG_BG0CNT = sub_801E640(self);
    *(vu32 *)REG_ADDR_BG0HOFS = 0;

    return self;
}

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void sub_8026ED0(void *arg0);

/* Same "re-probe an actor's own category-table slot 0x50/0x54" shape
 * already established by sub_8006770 (src/graphics/oam_count.c) -
 * width-re-measures a single icon's currently-drawn text in place
 * (the return value is discarded), skipping a NULL slot entirely. A
 * `#define`, not a helper function, so it inlines identically at each
 * of the six call sites below - matching sub_8006770's own inlined
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
 * `flags` is set - the same trailing shape sub_8006770 uses for its
 * own single-icon `arg0`. */
void sub_8005004(struct pause_screen_results *selfArg, u32 flagsArg)
{
    register struct pause_screen_results *self asm("r6") = selfArg;
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
        sub_8026ED0(self);
    }
}
/* Trailing byte-padding gotcha (see docs/matching.md/
 * matching_decomp_alignment_fix memory): the ROM pads the gap before
 * the next function (sub_8005100) with zero bytes (an explicit
 * `.align 2, 0` in the original assembly), but this compiler's own
 * default inter-function padding is a `mov r8, r8` NOP-equivalent
 * instead. */
asm(".align 2, 0");
