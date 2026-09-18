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

#if NON_MATCHING
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
 * NOT YET BYTE-MATCHING: reconstructed (semantics fully understood and
 * cross-checked - every field/global this touches is independently
 * confirmed by its callees sub_8004EC0/sub_8005100/sub_8005004/
 * sub_8006250) but parked here the same way sub_8006600
 * (src/graphics/oam_count.c) and settings_menu.c's/settings_menu6.c's
 * parked siblings are. Heavy register pinning (sl/r6/r9/r4/r5, matching
 * the ROM's own long-lived register choices for the heap flag, the
 * `&gUnknown_030012B8` cursor, the saved old cache, and the two icon-
 * manager addresses) got the instruction *order* and *count* extremely
 * close to the ROM's, but not exact: the ROM additionally keeps the
 * literal 0 live in r8 across roughly 100 intervening instructions (from
 * the first icon manager's `field_108 = 0` all the way to the final
 * `gUnknown_030012FC->field_08 = 0` reset) to avoid reloading it, and
 * shares a couple of shifted-constant computations (`0x84<<1`/`0x96<<1`)
 * between otherwise-separate statements - both are scheduling decisions
 * this compiler doesn't reach for from plain C at this call depth, and
 * attempts to force them (extra inline-asm r8 pins, hoisted constant
 * temporaries) either had no effect or introduced a stack spill the ROM
 * doesn't have. Real bytes stay in asm/code_3_1_10_7.s. */
s32 sub_8004D74(void)
{
    register s32 heapFlag asm("sl") = MEM_HEAP_BOTH;
    register struct tile_asset_cache **cacheAddr asm("r6") = &gUnknown_030012B8;
    register struct tile_asset_cache *oldCache asm("r9");
    struct tile_asset_cache *newCache;

    mem_free_bytes(heapFlag);
    sub_80019E8(gUnknown_030012BC);
    sub_80006A8();

    *(vu16 *)PLTT = 0;
    *(vu16 *)REG_ADDR_DISPCNT = 0;

    oldCache = *cacheAddr;
    {
        register void *tmp asm("r0") = sub_8026EDC(sizeof(struct tile_asset_cache));
        asm volatile("bl sub_8006FB4" : "+r" (tmp) :: "r1", "r2", "r3", "lr", "cc");
        newCache = tmp;
    }
    *cacheAddr = newCache;
    sub_8006EF0(newCache, *(u16 *)(gStaticData_084A5600 + 0xe), *(u8 **)(gStaticData_084A5600 + 8));
    sub_8006D50(*cacheAddr, 0xf);
    sub_803A94C(gStaticData_0816B2C0, (u8 *)*cacheAddr + (0x83 << 2), 0x10);

    {
        register struct icon_manager **dcAddr asm("r4") = &gUnknown_030012DC;
        register struct icon_manager **e0Addr asm("r5") = &gUnknown_030012E0;

        sub_8028A40(*dcAddr, 0);
        sub_8028A40(*e0Addr, 0);

        {
            struct icon_manager *mgr = *dcAddr;
            struct icon_record *rec;

            mgr->field_108 = 0;
            rec = mgr->record;
            sub_803AD7C((u8 *)mgr + rec->slots[6].offset, rec->slots[6].ptr);
        }

        {
            struct icon_manager *mgr = *e0Addr;
            struct icon_record *rec;

            mgr->field_108 = (*dcAddr)->field_12c;
            rec = mgr->record;
            sub_803AD7C((u8 *)mgr + rec->slots[6].offset, rec->slots[6].ptr);
        }

        gUnknown_030012FC->field_08 = (*dcAddr)->field_12c + (*e0Addr)->field_12c;
    }
    sub_8006C4C(gUnknown_030012FC);

    {
        register struct pause_screen_results *self asm("r4");
        register s32 result asm("r5");

        self = sub_8004EC0((struct pause_screen_results *)sub_8026EDC(0xd4));
        result = sub_8005100(self);
        if (self != NULL) {
            sub_8005004(self, 3);
        }

        gUnknown_030012FC->field_08 = 0;
        sub_8006C4C(gUnknown_030012FC);
        if (*cacheAddr != NULL) {
            sub_8006F94(*cacheAddr, 3);
        }
        *cacheAddr = oldCache;
        mem_free_bytes(heapFlag);

        return result;
    }
}
#endif /* NON_MATCHING */

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

extern void sub_8008890(void *arg0, s32 arg1, s32 arg2);
extern s32 sub_8026F38(s32 arg0);
extern void sub_800570C(struct pause_screen_results *self);
extern struct icon_pos gStaticData_0816B21C[];
extern void sub_8005E5C(struct pause_screen_results *self, void *label1, void *label2);

#if NON_MATCHING
/* Shows whichever of `icons9c[1..4]` has a matching bit set in
 * `self->field_10`'s flag byte (bits 1/4/8/2 - a different bit set
 * than sub_800570C's, same handle), always shows `icons9c[0]`
 * unconditionally, then draws a fixed "x/28"-shaped fraction readout:
 * first `gStaticData_0816B21C[0]`'s position (offset by -0x14/-4) with
 * `self->field_2f`'s buffer at a fixed slot, then repositions to
 * (0xb4, 0x80) and calls `sub_8005E5C` with `self->field_32`/
 * `self->field_49` (the count/total buffers `sub_8005B80` -
 * src/graphics/settings_menu6.c - already fills for this same icon
 * row).
 *
 * NOT YET BYTE-MATCHING: same register-pressure class of difficulty as
 * sub_8005E5C above - the ROM keeps `self` in r5 and evolves a single
 * register (r6) through three different offset meanings
 * (0x110->0x130->0x114) via incremental arithmetic on its own prior
 * value, needing only r4-r6 total; every restructuring tried here
 * (direct field stores, hoisted x/y locals, an explicit r5 pin on
 * `self`) always needs one register more (r7 or r8) than the ROM does.
 * Real bytes stay in asm/code_3_1_10_7_57e0.s. */
void sub_80057E0(struct pause_screen_results *self)
{
    if (*((u8 *)self->field_10 + 2) & 1) {
        sub_8008890(self->icons9c[1], 0, 0);
    }
    if (*((u8 *)self->field_10 + 2) & 4) {
        sub_8008890(self->icons9c[2], 0, 0);
    }
    if (*((u8 *)self->field_10 + 2) & 8) {
        sub_8008890(self->icons9c[3], 0, 0);
    }
    if (*((u8 *)self->field_10 + 2) & 2) {
        sub_8008890(self->icons9c[4], 0, 0);
    }
    sub_8008890(self->icons9c[0], 0, 0);

    {
        struct icon_record *rec;

        gUnknown_030012DC->posX = gStaticData_0816B21C[0].x - 0x14;
        gUnknown_030012DC->posY = gStaticData_0816B21C[0].y - 4;

        rec = gUnknown_030012DC->record;
        sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, self->buf2f, rec->slots[2].ptr);
    }

    gUnknown_030012DC->posX = 0xb4;
    gUnknown_030012DC->posY = 0x80;
    sub_8005E5C(self, self->buf32, self->buf49);
}
#endif /* NON_MATCHING */

extern struct icon_pos gStaticData_0816B258[];

#if NON_MATCHING
/* Same shape as sub_80057E0 above for the `iconsB0[3]` row: hides all
 * three icons unconditionally (no per-bit gating this time), then
 * draws three fixed "x/20"-shaped fraction readouts at
 * `gStaticData_0816B258[2]/[1]/[0]`'s positions (offset -4/+0xe, same
 * pattern as sub_80057E0's single readout) with `self->field_38`/
 * `field_3b`/`field_3e`, then a final one at (0xb4, 0x80) via
 * `sub_8005E5C` with `self->field_35`/`field_4c` (the total/threshold
 * buffers `sub_8005C58` - src/graphics/settings_menu6.c - fills for
 * this row).
 *
 * NOT YET BYTE-MATCHING: same register-pressure class of difficulty as
 * sub_80057E0/sub_8005E5C above - parked the same way, real bytes stay
 * in asm/code_3_1_10_7_57e0.s. */
void sub_80058C0(struct pause_screen_results *self)
{
    struct settings_icon_actor **p = self->iconsB0;
    s32 i;
    struct icon_record *rec;

    for (i = 2; i >= 0; i--) {
        sub_8008890(*p, 0, 0);
        p++;
    }

    gUnknown_030012DC->posX = gStaticData_0816B258[2].x - 4;
    gUnknown_030012DC->posY = gStaticData_0816B258[2].y + 0xe;
    rec = gUnknown_030012DC->record;
    sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, self->buf38, rec->slots[2].ptr);

    gUnknown_030012DC->posX = gStaticData_0816B258[1].x - 4;
    gUnknown_030012DC->posY = gStaticData_0816B258[1].y + 0xe;
    rec = gUnknown_030012DC->record;
    sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, self->buf3b, rec->slots[2].ptr);

    gUnknown_030012DC->posX = gStaticData_0816B258[0].x - 4;
    gUnknown_030012DC->posY = gStaticData_0816B258[0].y + 0xe;
    rec = gUnknown_030012DC->record;
    sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, self->buf3e, rec->slots[2].ptr);

    gUnknown_030012DC->posX = 0xb4;
    gUnknown_030012DC->posY = 0x80;
    sub_8005E5C(self, self->buf35, self->buf4c);
}
#endif /* NON_MATCHING */

extern void sub_8006A90(void *arg0);
extern void sub_8006C28(struct vram_upload_cursor *self);
extern struct oam_shadow_buffer *gUnknown_03001300;
extern void sub_8006A48(struct oam_shadow_buffer *arg0);
extern void sub_800556C(struct pause_screen_results *self);
extern void sub_80061E8(struct pause_screen_results *self);
extern void sub_800619C(struct pause_screen_results *self);
extern void sub_8006124(struct pause_screen_results *self);

#if NON_MATCHING
/* The composite pause/options screen's per-frame "draw the current
 * settings row" step: draws `self->field_70` (the current level's name
 * label) centered into `gUnknown_030012E0`'s slot pair, then - only
 * when `self->field_74` is set (levels 0-0x13, see sub_800599C) -
 * draws `field_74` followed immediately by `self->buf78` (" N") at a
 * fixed position, forming a "LEVEL N"-shaped composite label.
 * Unconditionally right-aligns `self->buf41` (the completion
 * percentage string) at a fixed row. Calls the per-row list renderer
 * (`sub_800556C`) and an unread sibling (`sub_80061E8`), then
 * dispatches on `self->field_24` (the same state sub_8005304 cycles -
 * cases 0-4 map to `sub_800619C`/`sub_800570C`/`sub_80057E0`/
 * `sub_80058C0`/`sub_8006124`, one per icon-row group), and finally
 * hides `self->field_c0` (the row-cursor icon) if its blink countdown
 * (`field_c4`) has reached 0.
 *
 * NOT YET BYTE-MATCHING: same register-pressure class of difficulty as
 * sub_80057E0/sub_8005E5C - several sequential `sub_803AD80` draws
 * with hand-scheduled constant/offset register reuse (including an
 * `ip`-register spill in the field_74 branch) this compiler doesn't
 * reach for from plain C. Real bytes stay in asm/code_3_1_10_7_53f4.s. */
void sub_80053F4(struct pause_screen_results *self)
{
    struct icon_record *rec;
    u32 width;
    s32 half;

    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);

    rec = gUnknown_030012E0->record;
    width = sub_803AD80((u8 *)gUnknown_030012E0 + rec->slots[0].offset, self->field_70, rec->slots[0].ptr);
    half = (0xf0 - width) >> 1;

    gUnknown_030012E0->posX = half;
    gUnknown_030012E0->posY = 0xe;

    rec = gUnknown_030012E0->record;
    sub_803AD80((u8 *)gUnknown_030012E0 + rec->slots[2].offset, self->field_70, rec->slots[2].ptr);

    if (self->field_74 != NULL) {
        gUnknown_030012E0->posX = 0x20;
        gUnknown_030012E0->posY = 0x26;

        rec = gUnknown_030012E0->record;
        sub_803AD80((u8 *)gUnknown_030012E0 + rec->slots[2].offset, self->field_74, rec->slots[2].ptr);

        rec = gUnknown_030012E0->record;
        sub_803AD80((u8 *)gUnknown_030012E0 + rec->slots[2].offset, self->buf78, rec->slots[2].ptr);
    }

    rec = gUnknown_030012E0->record;
    width = sub_803AD80((u8 *)gUnknown_030012E0 + rec->slots[0].offset, self->buf41, rec->slots[0].ptr);

    gUnknown_030012E0->posX = 0x8c - width;
    gUnknown_030012E0->posY = 0x88;

    rec = gUnknown_030012E0->record;
    sub_803AD80((u8 *)gUnknown_030012E0 + rec->slots[2].offset, self->buf41, rec->slots[2].ptr);

    sub_800556C(self);
    sub_80061E8(self);

    switch (self->field_24) {
    case 0:
        sub_800619C(self);
        break;
    case 1:
        sub_800570C(self);
        break;
    case 2:
        sub_80057E0(self);
        break;
    case 3:
        sub_80058C0(self);
        break;
    case 4:
        sub_8006124(self);
        break;
    }

    if (self->field_c4 == 0) {
        sub_8008890(self->field_c0, 0, 0);
    }

    sub_8006A48(gUnknown_03001300);
}
#endif /* NON_MATCHING */

extern void sub_80007AC(void *arg0);
extern void *gUnknown_03001304;
extern u32 gUnknown_030007E0;
extern void sub_8006084(struct pause_screen_results *self);
extern s32 sub_800609C(struct pause_screen_results *self);
extern void sub_8005EF4(struct pause_screen_results *self);
extern void sub_8005FBC(struct pause_screen_results *self);
extern void sub_8006250(struct pause_screen_results *self);
extern void sub_8005304(struct pause_screen_results *self);
extern void PlaySfx(struct AudioContext *self, u32 id, u32 volumeParam);

#if NON_MATCHING
/* The composite pause/options screen's blocking cursor/confirm/cancel
 * driver (docs/rom_map.md's overlay_ui section) - runs until the user
 * confirms or cancels, redrawing every frame via sub_80053F4/
 * sub_8006250/sub_8005304 (the same per-row draw/apply-registers/
 * icon-cycle trio every settings row already uses).
 *
 * `field_cc`'s low 5 bits are a blend/fade level (see sub_8004EC0 and
 * sub_8006250): first ramps it down to 0 one frame at a time (the
 * screen's fade-in), then the main input loop - L/R adjust the
 * currently-selected row's slider (sub_800609C/sub_8006084, playing a
 * confirm-ish SFX and arming a short flash via field_68), the D-pad
 * bumps the selected row's value up/down with an initial-press vs
 * held-repeat distinction (sub_8005EF4/FBC), and A confirms only when
 * the selected row's type tag is 4 or 5 (an "editable" row - anything
 * else just plays a cancel SFX and keeps looping), B cancels
 * outright. On confirm, ramps the fade level back up to 0x10 (the
 * screen's fade-out) before returning the confirmed row's type tag;
 * on cancel, returns 0 without ramping back up (`field_cc` is instead
 * force-set to 0x40 in the low byte and DISPCNT reapplied once).
 *
 * NOT YET BYTE-MATCHING: fully understood (every field/global here is
 * independently confirmed by sub_8004EC0/sub_8005304/sub_8006250's own
 * matched bytes) but by far the largest and most control-flow-heavy
 * function in this chunk - parked without attempting the same class of
 * register-pressure/scheduling fight already documented (at length) for
 * sub_80057E0/sub_8005E5C/sub_80053F4 above; the effort-to-payoff ratio
 * for hand-tuning a function this size wasn't worth it this pass. Real
 * bytes stay in asm/code_3_1_10_7_5100.s. */
s32 sub_8005100(struct pause_screen_results *self)
{
    s32 result;

    if (self->field_cc & 0x1f) {
        do {
            u8 v = self->field_cc;
            s32 low5 = ((v & 0x1f) - 1) & 0x1f;
            self->field_cc = (v & -0x20) | low5;
            sub_80053F4(self);
            sub_8006250(self);
            sub_8005304(self);
        } while (self->field_cc & 0x1f);
    }

    for (;;) {
        u32 raw, newPress;
        s32 rowType;

        sub_80053F4(self);
        sub_8006250(self);
        sub_8005304(self);
        sub_80007AC(gUnknown_03001304);

        raw = gUnknown_030007E0;
        newPress = raw >> 0x10;

        if (newPress & 0x40) {
            sub_800609C(self);
            self->field_68 = 0x1e;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }
        raw = gUnknown_030007E0;
        newPress = raw >> 0x10;
        if (newPress & 0x80) {
            sub_8006084(self);
            self->field_68 = 0x1e;
            PlaySfx(gUnknown_030012BC, 0x46, 0x100);
        }

        raw = gUnknown_030007E0;
        newPress = raw >> 0x10;
        if (newPress & 0x20) {
            sub_8005EF4(self);
            self->field_68 = 0x1e;
        } else if (raw & 0x20) {
            if (self->field_68 == 0) {
                sub_8005EF4(self);
                self->field_68 = 5;
            } else {
                self->field_68 = self->field_68 - 1;
            }
        }

        raw = gUnknown_030007E0;
        newPress = raw >> 0x10;
        if (newPress & 0x10) {
            sub_8005FBC(self);
            self->field_68 = 0x1e;
        } else if (raw & 0x10) {
            if (self->field_68 == 0) {
                sub_8005FBC(self);
                self->field_68 = 5;
            } else {
                self->field_68 = self->field_68 - 1;
            }
        }

        newPress = *((u16 *)&gUnknown_030007E0 + 1);
        if (!(newPress & 1)) {
            /* Neither confirm nor cancel this frame: cancel-check below
             * falls straight through when B isn't pressed either. */
            newPress = *((u16 *)&gUnknown_030007E0 + 1);
            if (newPress & 8) {
                PlaySfx(gUnknown_030012BC, 0x49, 0x100);
                result = 0;
                break;
            }
            continue;
        }

        rowType = *(s32 *)((u8 *)self->field_14 + self->field_18 * 8 + 4);
        if ((u32)(rowType - 4) > 1) {
            PlaySfx(gUnknown_030012BC, 0x48, 0x100);
            continue;
        }

        PlaySfx(gUnknown_030012BC, 0x49, 0x100);
        result = rowType;
        break;
    }

    if ((self->field_cc & 0x1f) != 0x10) {
        do {
            u8 v = self->field_cc;
            s32 low5 = ((v & 0x1f) + 1) & 0x1f;
            self->field_cc = (v & -0x20) | low5;
            sub_80053F4(self);
            sub_8006250(self);
            sub_8005304(self);
        } while ((self->field_cc & 0x1f) != 0x10);
    }

    self->field_d0 = 0;
    *(u8 *)&self->field_d0 |= 0x40;
    sub_8006250(self);
    return result;
}
#endif /* NON_MATCHING */

extern s32 sub_8028A30(struct icon_manager *self, s32 val);

/* One 8-byte record of `self->field_14`'s per-row array: a runtime
 * string-table label id, then a type tag (`sub_800556C` branches on
 * `==4`/`==5`/else; `sub_8005100`'s confirm check uses the same tag). */
struct pause_screen_row_record {
    s32 labelId;
    s32 typeTag;
};

#if NON_MATCHING
/* The composite pause/options screen's per-row list renderer - draws
 * `self->field_1c` rows (from `self->field_14`'s record array),
 * highlighting whichever matches `self->field_18` (the selected
 * index), each centered horizontally and stacked vertically by
 * `self->field_20` pixels starting at y=0x4a. Three layout variants
 * per row, keyed by the record's type tag (docs/rom_map.md's
 * overlay_ui section, "sub_800556C branches on a per-row type tag"):
 * a plain centered label (any other tag), or - for tags 4/5 - the
 * label additionally offset left by half of a second string's width
 * (`self->buf57` for tag 4, `self->buf4f` for tag 5 - the " <NN%>"
 * scratch buffers sub_800599C/sub_8005EF4/FBC fill), with that second
 * string drawn immediately after at the same position (auto-advancing
 * - "label <NN%>" on one line).
 *
 * NOT YET BYTE-MATCHING: fully understood but parked without attempting
 * the register-pressure fight already documented at length above - the
 * ROM additionally keeps the running row-Y coordinate in a stack slot
 * (not a register) across the whole loop, a scheduling choice this
 * compiler doesn't reach for from a plain loop-local. Real bytes stay
 * in asm/code_3_1_10_7_53f4.s. */
void sub_800556C(struct pause_screen_results *self)
{
    struct pause_screen_row_record *records = (struct pause_screen_row_record *)self->field_14;
    s32 y = 0x4a;
    s32 i;

    for (i = 0; i < self->field_1c; i++) {
        s32 label;
        struct icon_record *rec;
        u32 width;
        s32 x;

        if (i == self->field_18) {
            sub_8028A30(gUnknown_030012DC, 0xf);
        } else {
            sub_8028A40(gUnknown_030012DC, 0);
        }

        label = sub_8026F38(records[i].labelId);
        rec = gUnknown_030012DC->record;
        width = sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[0].offset, (void *)label, rec->slots[0].ptr);
        x = 0x32 - (width >> 1);

        if (records[i].typeTag == 4) {
            u32 w2;

            rec = gUnknown_030012DC->record;
            w2 = sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[0].offset, self->buf57, rec->slots[0].ptr);
            x -= (w2 >> 1);

            gUnknown_030012DC->posX = x;
            gUnknown_030012DC->posY = y;

            rec = gUnknown_030012DC->record;
            sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, (void *)label, rec->slots[2].ptr);

            rec = gUnknown_030012DC->record;
            sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, self->buf57, rec->slots[2].ptr);
        } else if (records[i].typeTag == 5) {
            u32 w2;

            rec = gUnknown_030012DC->record;
            w2 = sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[0].offset, self->buf4f, rec->slots[0].ptr);
            x -= (w2 >> 1);

            gUnknown_030012DC->posX = x;
            gUnknown_030012DC->posY = y;

            rec = gUnknown_030012DC->record;
            sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, (void *)label, rec->slots[2].ptr);

            rec = gUnknown_030012DC->record;
            sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, self->buf4f, rec->slots[2].ptr);
        } else {
            gUnknown_030012DC->posX = x;
            gUnknown_030012DC->posY = y;

            rec = gUnknown_030012DC->record;
            sub_803AD80((u8 *)gUnknown_030012DC + rec->slots[2].offset, (void *)label, rec->slots[2].ptr);
        }

        y += self->field_20;
    }

    sub_8028A40(gUnknown_030012DC, 0);
}
#endif /* NON_MATCHING */
