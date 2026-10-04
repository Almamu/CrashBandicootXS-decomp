#include "core.h"
#include "icon_manager.h"

struct counter_widget {
    u32 field_0;
    u8 field_4;
    u8 pad_5[3];
    s32 field_8;
};

extern void *gUnknown_03001300;
extern void *gUnknown_030012FC;
extern struct icon_manager *gUnknown_030012DC;
/* The six digit glyphs the widget draws. */
extern void *gStaticData_0817E714[6];
extern void sub_8006A90(void *arg0);
extern void sub_8006C28(void *arg0);
extern void sub_8006A48(void *arg0);
extern s32 sub_8028A30(struct icon_manager *mgr, u8 frame);
extern s32 sub_8037534(struct counter_widget *self);
/* `_call_via_r2`: calls `fn(self, arg)` (an icon_manager method). */
extern s32 _call_via_r2(void *self, void *arg, void *fn);

/* The counter widget's (src/audio/counter_selector.c) per-frame icon
 * draw loop: for each of the six digit slots (0-5) it sets the shared
 * overlay frame (`gUnknown_030012DC`: 1 or 2 from `sub_8037534`'s blink
 * state on the currently selected slot `field_8`, 0 elsewhere), measures
 * that slot's glyph with the icon manager's `slots[0]` method, centers
 * it horizontally, and draws it with `slots[2]` at a Y stepping by 0xa
 * from 0x32.
 *
 * Was NAKED ("many-register allocation ceiling"); calling the method
 * trampoline `_call_via_r2` directly with the glyph assigned inside the
 * first call's argument list (so it's loaded between `this` and the
 * method pointer, as the ROM does) matches outright - see
 * docs/matching/gax-toolchain-retry.md. */
void sub_80372BC(struct counter_widget *self)
{
    s32 y;
    s32 i;

    sub_8006A90(gUnknown_03001300);
    sub_8006C28(gUnknown_030012FC);
    y = 0x32;
    for (i = 0; i <= 5; i++) {
        void *glyph;
        s32 x;

        if (i == self->field_8)
            sub_8028A30(gUnknown_030012DC, sub_8037534(self));
        else
            sub_8028A30(gUnknown_030012DC, 0);
        x = (240 - _call_via_r2((u8 *)gUnknown_030012DC + gUnknown_030012DC->record->slots[0].offset,
                               glyph = gStaticData_0817E714[i],
                               gUnknown_030012DC->record->slots[0].ptr)) >> 1;
        gUnknown_030012DC->posX = x;
        gUnknown_030012DC->posY = y;
        _call_via_r2((u8 *)gUnknown_030012DC + gUnknown_030012DC->record->slots[2].offset, glyph,
                    gUnknown_030012DC->record->slots[2].ptr);
        y += 10;
    }
    sub_8006A48(gUnknown_03001300);
}

/* Resets several OAM-manager globals, then hand-fills
 * `gUnknown_030012B8`'s (`struct tile_asset_cache`, include/vram_pool.h)
 * `slots[0]`-`slots[3]` with 4 fixed 32-byte OBJ tiles copied from
 * `gStaticData_0817E72C`/`_74C`/`_76C`/`_78C`, and finally runs
 * `gUnknown_030012DC`'s/`gUnknown_030012E0`'s `record->slots[6]` method
 * (`_call_via_r1`) plus a VRAM reserve (`sub_8006C58`) for each, copying
 * `field_12c` into the other manager's `field_108`.
 *
 * Was NAKED: the ROM rematerializes the 0x108/0x12c/0x130 field-offset
 * constants after every call instead of keeping them in callee-saved
 * registers. Matched with the idiom from actor_part131.c's
 * `sub_8034CEC`: the two icon-manager steps as `static inline` helpers
 * taking the manager as a parameter (each expansion recomputes its own
 * offsets; the E0 base is read from DC before E0 itself), plus one
 * `zero` local shared by the `field_8`/`field_108` stores - the 0 the
 * ROM keeps in r8. Matches under both compilers. */
#include "vram_pool.h"
extern struct tile_asset_cache *gUnknown_030012B8;
extern struct icon_manager *gUnknown_030012E0;
extern const u16 gStaticData_0817E72C[16];
extern const u16 gStaticData_0817E74C[16];
extern const u16 gStaticData_0817E76C[16];
extern const u16 gStaticData_0817E78C[16];
extern void WaitForVBlank(void);
extern void sub_8006AAC(void *arg0);
extern void sub_8006EA8(struct tile_asset_cache *cache);
extern s32 sub_8006D50(struct tile_asset_cache *cache, s32 index);
extern void sub_8006C4C(void *cursor);
extern s32 sub_8006C58(void *cursor, s32 size);
extern void sub_8006C30(void *cursor);
extern void _call_via_r1(void *self, void *fn);

static inline void IconSetBase(struct icon_manager *m, u32 base)
{
    struct icon_slot *slot;

    m->field_108 = base;
    slot = &m->record->slots[6];
    _call_via_r1((u8 *)m + slot->offset, slot->ptr);
}

static inline void IconReserveVram(void *c, struct icon_manager *m)
{
    sub_8006C58(c, m->field_12c << 5);
}

void sub_8037388(void *unused)
{
    s32 i;

    sub_8006A90(gUnknown_03001300);
    sub_8006A48(gUnknown_03001300);
    WaitForVBlank();
    sub_8006AAC(gUnknown_03001300);
    sub_8006EA8(gUnknown_030012B8);
    sub_8006D50(gUnknown_030012B8, 0);
    sub_8006D50(gUnknown_030012B8, 1);
    sub_8006D50(gUnknown_030012B8, 2);
    sub_8006D50(gUnknown_030012B8, 3);
    {
        struct tile_asset_cache *cache = gUnknown_030012B8;
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

        sub_8028A30(gUnknown_030012DC, 0);
        sub_8028A30(gUnknown_030012E0, 0);
        ((u32 *)gUnknown_030012FC)[2] = zero;
        sub_8006C4C(gUnknown_030012FC);
        sub_8006C4C(gUnknown_030012FC);
        IconSetBase(gUnknown_030012DC, zero);
        IconReserveVram(gUnknown_030012FC, gUnknown_030012DC);
        {
            u32 base = gUnknown_030012DC->field_12c;

            IconSetBase(gUnknown_030012E0, base);
        }
        IconReserveVram(gUnknown_030012FC, gUnknown_030012E0);
    }
    sub_8006C30(gUnknown_030012FC);
}
