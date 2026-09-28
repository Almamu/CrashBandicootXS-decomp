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
extern s32 sub_803AD80(void *self, void *arg, void *fn);

/* The counter widget's (src/audio/counter_selector.c) per-frame icon
 * draw loop: for each of the six digit slots (0-5) it sets the shared
 * overlay frame (`gUnknown_030012DC`: 1 or 2 from `sub_8037534`'s blink
 * state on the currently selected slot `field_8`, 0 elsewhere), measures
 * that slot's glyph with the icon manager's `slots[0]` method, centers
 * it horizontally, and draws it with `slots[2]` at a Y stepping by 0xa
 * from 0x32.
 *
 * Was NAKED ("many-register allocation ceiling"); calling the method
 * trampoline `sub_803AD80` directly with the glyph assigned inside the
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
        x = (240 - sub_803AD80((u8 *)gUnknown_030012DC + gUnknown_030012DC->record->slots[0].offset,
                               glyph = gStaticData_0817E714[i],
                               gUnknown_030012DC->record->slots[0].ptr)) >> 1;
        gUnknown_030012DC->posX = x;
        gUnknown_030012DC->posY = y;
        sub_803AD80((u8 *)gUnknown_030012DC + gUnknown_030012DC->record->slots[2].offset, glyph,
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
 * (`sub_803AD7C`) plus a VRAM reserve (`sub_8006C58`) for each, copying
 * `field_12c` into the other manager's `field_108`.
 *
 * Still NAKED. The draft below matches through the tile-copy loop (the
 * `destA`/`destB` shape from actor_part88.c); the tail doesn't: the ROM
 * keeps the three manager/cursor global addresses in r4-r6 and a 0 in
 * r8, rematerializing the 0x108/0x12c/0x130 field-offset constants
 * after every call (deriving 0x130/0x108 from the previous constant
 * with `adds #40`/`subs #36`), whereas agbcc/old_agbcc CSE those
 * constants into callee-saved registers across the calls and push the
 * addresses to r8-sl. Pinning the addresses to r4-r6 just moves the
 * constants to r8-sl; do/while wrappers around the method calls don't
 * split the CSE blocks either. */
#if NON_MATCHING
#include "vram_pool.h"
extern struct tile_asset_cache *gUnknown_030012B8;
extern struct icon_manager *gUnknown_030012E0;
extern const u16 gStaticData_0817E72C[16];
extern const u16 gStaticData_0817E74C[16];
extern const u16 gStaticData_0817E76C[16];
extern const u16 gStaticData_0817E78C[16];
extern void sub_80006A8(void);
extern void sub_8006AAC(void *arg0);
extern void sub_8006EA8(struct tile_asset_cache *cache);
extern s32 sub_8006D50(struct tile_asset_cache *cache, s32 index);
extern void sub_8006C4C(void *cursor);
extern s32 sub_8006C58(void *cursor, s32 size);
extern void sub_8006C30(void *cursor);
extern void sub_803AD7C(void *self, void *fn);

void sub_8037388(void *unused)
{
    s32 i;
    struct icon_slot *slot;

    sub_8006A90(gUnknown_03001300);
    sub_8006A48(gUnknown_03001300);
    sub_80006A8();
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
    sub_8028A30(gUnknown_030012DC, 0);
    sub_8028A30(gUnknown_030012E0, 0);
    ((u32 *)gUnknown_030012FC)[2] = 0;
    sub_8006C4C(gUnknown_030012FC);
    sub_8006C4C(gUnknown_030012FC);
    gUnknown_030012DC->field_108 = 0;
    slot = &gUnknown_030012DC->record->slots[6];
    sub_803AD7C((u8 *)gUnknown_030012DC + slot->offset, slot->ptr);
    sub_8006C58(gUnknown_030012FC, gUnknown_030012DC->field_12c << 5);
    gUnknown_030012E0->field_108 = gUnknown_030012DC->field_12c;
    slot = &gUnknown_030012E0->record->slots[6];
    sub_803AD7C((u8 *)gUnknown_030012E0 + slot->offset, slot->ptr);
    sub_8006C58(gUnknown_030012FC, gUnknown_030012E0->field_12c << 5);
    sub_8006C30(gUnknown_030012FC);
}
#else
NAKED void sub_8037388(void *unused)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "ldr r4, 1f\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A90\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006A48\n\t"
        "bl sub_80006A8\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006AAC\n\t"
        "ldr r4, 2f\n\t"
        "ldr r0, [r4]\n\t"
        "bl sub_8006EA8\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0\n\t"
        "bl sub_8006D50\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #1\n\t"
        "bl sub_8006D50\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #2\n\t"
        "bl sub_8006D50\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #3\n\t"
        "bl sub_8006D50\n\t"
        "ldr r0, [r4]\n\t"
        "mov r7, #0\n\t"
        "add r2, r0, #0\n\t"
        "add r2, #0x6c\n\t"
        "ldr r6, 3f\n\t"
        "add r1, r0, #0\n\t"
        "add r1, #0x2c\n\t"
        "ldr r5, 4f\n\t"
        "ldr r4, 5f\n\t"
        "ldr r3, 6f\n\t"
    "_080373E2:\n\t"
        "ldrh r0, [r5]\n\t"
        "strh r0, [r1]\n\t"
        "ldrh r0, [r6]\n\t"
        "strh r0, [r1, #0x20]\n\t"
        "ldrh r0, [r3]\n\t"
        "strh r0, [r2]\n\t"
        "ldrh r0, [r4]\n\t"
        "strh r0, [r2, #0x20]\n\t"
        "add r2, #2\n\t"
        "add r6, #2\n\t"
        "add r1, #2\n\t"
        "add r5, #2\n\t"
        "add r4, #2\n\t"
        "add r3, #2\n\t"
        "add r7, #1\n\t"
        "cmp r7, #0xf\n\t"
        "ble _080373E2\n\t"
        "mov r0, #0\n\t"
        "mov r8, r0\n\t"
        "ldr r4, 7f\n\t"
        "ldr r0, [r4]\n\t"
        "mov r1, #0\n\t"
        "bl sub_8028A30\n\t"
        "ldr r6, 8f\n\t"
        "ldr r0, [r6]\n\t"
        "mov r1, #0\n\t"
        "bl sub_8028A30\n\t"
        "ldr r5, 9f\n\t"
        "ldr r0, [r5]\n\t"
        "mov r1, r8\n\t"
        "str r1, [r0, #8]\n\t"
        "bl sub_8006C4C\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8006C4C\n\t"
        "ldr r0, [r4]\n\t"
        "mov r2, #0x84\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r0, r2\n\t"
        "mov r3, r8\n\t"
        "str r3, [r1]\n\t"
        "add r2, #0x28\n\t"
        "add r1, r0, r2\n\t"
        "ldr r1, [r1]\n\t"
        "add r1, #0x40\n\t"
        "mov r3, #0\n\t"
        "ldrsh r2, [r1, r3]\n\t"
        "add r0, r0, r2\n\t"
        "ldr r1, [r1, #4]\n\t"
        "bl sub_803AD7C\n\t"
        "ldr r0, [r5]\n\t"
        "ldr r1, [r4]\n\t"
        "mov r2, #0x96\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "ldr r1, [r1]\n\t"
        "lsl r1, r1, #5\n\t"
        "bl sub_8006C58\n\t"
        "ldr r0, [r4]\n\t"
        "mov r3, #0x96\n\t"
        "lsl r3, r3, #1\n\t"
        "add r0, r0, r3\n\t"
        "ldr r2, [r0]\n\t"
        "ldr r0, [r6]\n\t"
        "sub r3, #0x24\n\t"
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
        "ldr r0, [r5]\n\t"
        "ldr r1, [r6]\n\t"
        "mov r2, #0x96\n\t"
        "lsl r2, r2, #1\n\t"
        "add r1, r1, r2\n\t"
        "ldr r1, [r1]\n\t"
        "lsl r1, r1, #5\n\t"
        "bl sub_8006C58\n\t"
        "ldr r0, [r5]\n\t"
        "bl sub_8006C30\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
        ".align 2, 0\n"
    "1: .4byte gUnknown_03001300\n"
    "2: .4byte gUnknown_030012B8\n"
    "3: .4byte gStaticData_0817E74C\n"
    "4: .4byte gStaticData_0817E72C\n"
    "5: .4byte gStaticData_0817E78C\n"
    "6: .4byte gStaticData_0817E76C\n"
    "7: .4byte gUnknown_030012DC\n"
    "8: .4byte gUnknown_030012E0\n"
    "9: .4byte gUnknown_030012FC\n"
    );
}
#endif
