#include "core.h"
#include "audio.h"
#include "actor.h"
#include "icon_manager.h"
#include "vram_pool.h"
#include "pause_screen_results.h"
#include "memory.h"

extern void sub_80019E8(struct AudioContext *self);
extern void sub_80006A8(void);
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
extern void sub_803A94C(const void *src, void *dst, u32 cnt);
extern void sub_8028A40(struct icon_manager *self, u32 unused);
extern void *sub_803AD7C(void *arg0, void *fn);
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
 * same `sub_803AD7C` pattern documented throughout `icon_manager.h`),
 * builds the screen object (`sub_8004EC0`) and hands it to the blocking
 * cursor/confirm/cancel driver (`sub_8005100`), then tears the screen
 * down (`sub_8005004`, flags=3) and restores the original tile cache
 * before returning `sub_8005100`'s result.
 *
 * Written as NAKED asm, not plain C: hits the same class of gcc-2.9
 * register/scheduling nondeterminism already documented at length for
 * `sub_8006600` (src/graphics/oam_count.c) - see
 * docs/matching/issue-7-0x08004d74-overlay-ui.md for the previous pass's
 * specific gap (the ROM keeps a literal 0 live in r8 across ~100
 * intervening instructions and shares shifted-constant computations this
 * compiler never reached for from plain C, even with heavy register
 * pinning). Every instruction below is transcribed directly from and
 * checked against the ROM's own disassembly, like this project's other
 * hard-compiler-limitation cases (src/system/link_cable.c's
 * `sub_8001CB8`/`sub_8001DB4`, `src/audio/gax_swi.c`'s `sub_80392C4`). */
NAKED s32 sub_8004D74(void)
{
    asm(
    "push {r4, r5, r6, r7, lr}\n\t"
    "mov r7, sl\n\t"
    "mov r6, sb\n\t"
    "mov r5, r8\n\t"
    "push {r5, r6, r7}\n\t"
    "mov r0, #0xc0\n\t"
    "lsl r0, r0, #0x18\n\t"
    "mov sl, r0\n\t"
    "bl mem_free_bytes\n\t"
    "ldr r0, 3f\n\t"
    "ldr r0, [r0]\n\t"
    "bl sub_80019E8\n\t"
    "bl sub_80006A8\n\t"
    "mov r0, #0xa0\n\t"
    "lsl r0, r0, #0x13\n\t"
    "mov r1, #0\n\t"
    "strh r1, [r0]\n\t"
    "mov r0, #0x80\n\t"
    "lsl r0, r0, #0x13\n\t"
    "strh r1, [r0]\n\t"
    "ldr r6, 4f\n\t"
    "ldr r1, [r6]\n\t"
    "mov sb, r1\n\t"
    "mov r0, #0x8c\n\t"
    "lsl r0, r0, #2\n\t"
    "bl sub_8026EDC\n\t"
    "bl sub_8006FB4\n\t"
    "str r0, [r6]\n\t"
    "ldr r2, 5f\n\t"
    "ldrh r1, [r2, #0xe]\n\t"
    "ldr r2, [r2, #8]\n\t"
    "bl sub_8006EF0\n\t"
    "ldr r0, [r6]\n\t"
    "mov r1, #0xf\n\t"
    "bl sub_8006D50\n\t"
    "ldr r1, [r6]\n\t"
    "ldr r0, 6f\n\t"
    "mov r2, #0x83\n\t"
    "lsl r2, r2, #2\n\t"
    "add r1, r1, r2\n\t"
    "mov r2, #0x10\n\t"
    "bl sub_803A94C\n\t"
    "ldr r4, 7f\n\t"
    "ldr r0, [r4]\n\t"
    "bl sub_8028A40\n\t"
    "ldr r5, 8f\n\t"
    "ldr r0, [r5]\n\t"
    "bl sub_8028A40\n\t"
    "ldr r0, [r4]\n\t"
    "mov r3, #0\n\t"
    "mov r8, r3\n\t"
    "mov r2, #0x84\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "str r3, [r1]\n\t"
    "mov r3, #0x98\n\t"
    "lsl r3, r3, #1\n\t"
    "add r1, r0, r3\n\t"
    "ldr r1, [r1]\n\t"
    "add r1, #0x40\n\t"
    "mov r3, #0\n\t"
    "ldrsh r2, [r1, r3]\n\t"
    "add r0, r0, r2\n\t"
    "ldr r1, [r1, #4]\n\t"
    "bl sub_803AD7C\n\t"
    "ldr r0, [r4]\n\t"
    "mov r1, #0x96\n\t"
    "lsl r1, r1, #1\n\t"
    "add r0, r0, r1\n\t"
    "ldr r2, [r0]\n\t"
    "ldr r0, [r5]\n\t"
    "mov r3, #0x84\n\t"
    "lsl r3, r3, #1\n\t"
    "add r1, r0, r3\n\t"
    "str r2, [r1]\n\t"
    "mov r2, #0x98\n\t"
    "lsl r2, r2, #1\n\t"
    "add r1, r0, r2\n\t"
    "ldr r1, [r1]\n\t"
    "add r1, #0x40\n\t"
    "mov r3, #0\n\t"
    "ldrsh r2, [r1, r3]\n\t"
    "add r0, r0, r2\n\t"
    "ldr r1, [r1, #4]\n\t"
    "bl sub_803AD7C\n\t"
    "ldr r0, [r4]\n\t"
    "mov r1, #0x96\n\t"
    "lsl r1, r1, #1\n\t"
    "add r0, r0, r1\n\t"
    "ldr r1, [r0]\n\t"
    "ldr r0, [r5]\n\t"
    "mov r2, #0x96\n\t"
    "lsl r2, r2, #1\n\t"
    "add r0, r0, r2\n\t"
    "ldr r2, [r0]\n\t"
    "ldr r7, 9f\n\t"
    "ldr r0, [r7]\n\t"
    "add r1, r1, r2\n\t"
    "str r1, [r0, #8]\n\t"
    "bl sub_8006C4C\n\t"
    "mov r0, #0xd4\n\t"
    "bl sub_8026EDC\n\t"
    "bl sub_8004EC0\n\t"
    "add r4, r0, #0\n\t"
    "bl sub_8005100\n\t"
    "add r5, r0, #0\n\t"
    "cmp r4, #0\n\t"
    "beq 1f\n\t"
    "add r0, r4, #0\n\t"
    "mov r1, #3\n\t"
    "bl sub_8005004\n\t"
    "1:\n\t"
    "ldr r0, [r7]\n\t"
    "mov r3, r8\n\t"
    "str r3, [r0, #8]\n\t"
    "bl sub_8006C4C\n\t"
    "ldr r0, [r6]\n\t"
    "cmp r0, #0\n\t"
    "beq 2f\n\t"
    "mov r1, #3\n\t"
    "bl sub_8006F94\n\t"
    "2:\n\t"
    "mov r0, sb\n\t"
    "str r0, [r6]\n\t"
    "mov r0, sl\n\t"
    "bl mem_free_bytes\n\t"
    "add r0, r5, #0\n\t"
    "pop {r3, r4, r5}\n\t"
    "mov r8, r3\n\t"
    "mov sb, r4\n\t"
    "mov sl, r5\n\t"
    "pop {r4, r5, r6, r7}\n\t"
    "pop {r1}\n\t"
    "bx r1\n\t"
    ".align 2, 0\n"
    "3: .4byte gUnknown_030012BC\n"
    "4: .4byte gUnknown_030012B8\n"
    "5: .4byte gStaticData_084A5600\n"
    "6: .4byte gStaticData_0816B2C0\n"
    "7: .4byte gUnknown_030012DC\n"
    "8: .4byte gUnknown_030012E0\n"
    "9: .4byte gUnknown_030012FC\n"
    );
}

extern void *sub_801E644(void *buf, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void LoadGraphicsPackage(void *buf, void *asset);
extern s32 sub_801E640(void *buf);
extern void *gUnknown_030012C0;
extern void ***gUnknown_030012D0;
extern void *sub_80236EC(void *arg0);
extern void sub_800599C(struct pause_screen_results *self);
extern struct actor *sub_8008904(struct actor *part);
extern s32 sub_8000E1C(s32 max);
extern u8 gStaticData_0816B284[];
extern u8 gStaticData_0816B298[];

/* Same "recurring screen-constructor shape" docs/rom_map.md's overlay_ui
 * section documents for sub_8004EC0/sub_80063D8/sub_8005D44: `self`
 * (allocated by the caller, `sub_8004D74`, as a fresh 0xd4-byte
 * `struct pause_screen_results`) gets `sub_801E644` init, a local
 * BLDCNT/BLDY/DISPCNT setup (`field_c8`/`field_cc`/`field_d0`, the same
 * fields `sub_8006250` applies), `LoadGraphicsPackage`, a row-stats
 * handle from `gUnknown_030012C0`, then hands off to `sub_800599C` to
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
        self->field_10 = sub_80236EC(gUnknown_030012C0);
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

    self->field_c4 = (u16)sub_8000E1C(0x78) + 0x78;

    self->field_14 = gStaticData_0816B298;
    self->field_18 = zero;
    {
        s32 v;
        if (*((u8 *)gUnknown_030012C0 + 0x8c) != 0) {
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

extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
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
            sub_803AD80((u8 *)_icon + *(s16 *)_p, (void *)3, *(void **)(_p + 4)); \
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
