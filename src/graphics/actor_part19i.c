#include "core.h"

/* Sits right after actor_part19d.c's `sub_802C904` and before the
 * still-raw remainder of `asm/code_3_2_20_28568_c99c.s` (starting at
 * `sub_802CC9C`) - directly adjacent to `actor_part19d.c` now, closing
 * part of GitHub issue #53's `0x0802C99C`-`0x0802D3A8` chunk. Same
 * `self` object and conventions documented in `actor_part19g.c` (the
 * `self+0xc` state field, the `+0x10`/`+0x12`/`+8` anim-reset idiom,
 * the `+0x30` type-byte indirection, and the `InitActorPart`-based
 * constructor family already matched throughout this ROM region). */

extern void *gUnknown_03000884;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern u8 sub_802A6EC(void *self);
extern void sub_8022FEC(void *self);
extern void sub_8022EA8(void *arg0, s32 arg1);
extern void sub_802C078(void *arg0, s32 delta);
extern void sub_802C4C8(void *selfArg);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 sub_803ADB4(s32 arg0, s32 arg1);

extern u8 gStaticData_087E4F94[];
extern u8 gStaticData_087E4EB4[];
extern u8 gStaticData_087E4ED4[];
extern u8 gStaticData_087E4EF4[];
extern u8 gStaticData_087E4F14[];
extern u8 gStaticData_087E4F34[];
extern u8 gStaticData_087E4F54[];
extern u8 gStaticData_087E4F74[];

/* Extends the type-byte event dispatch family (`sub_8031D7C`/etc, per
 * docs/rom_map.md; the `sub_802C540` shape in actor_part19g.c) with
 * values `5`-`7`. On proximity (`sub_802A6EC`), plays a sound, ties the
 * lap counter, then dispatches `sub_8022EA8` with a tier argument keyed
 * off `self+0x30`'s type byte (`5`->1, `6`->2, `7`->anything else
 * dispatches nothing) before the shared "used" state transition;
 * tail-calls `sub_802C4C8`. */
void sub_802C99C(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0xc) != 0x12 && sub_802A6EC(self)) {
        s32 typeByte;

        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_8022FEC(gUnknown_030012C0);

        typeByte = **(u8 **)(self + 0x30);

        switch (typeByte) {
        case 5:
            sub_8022EA8(gUnknown_030012C0, 1);
            break;
        case 6:
            sub_8022EA8(gUnknown_030012C0, 2);
            break;
        case 7:
            sub_8022EA8(gUnknown_030012C0, 3);
            break;
        }

        *(s32 *)(self + 0xc) = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xd8);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
            *(s32 *)(self + 8) = zero2;
        }
    }

    sub_802C4C8(self);
}

/* Unconditional (no `sub_802A6EC` proximity guard) "used"-state
 * transition: plays a sound, ties the lap counter, clears `self+0x44`,
 * then the usual state-0x12/anim-reset block. No tail call - the
 * caller drives whatever comes after directly. */
void sub_802CA28(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0xc) != 0x12) {
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        sub_8022FEC(gUnknown_030012C0);
        {
            register s32 zero2 asm("r2") = 0;

            *(s32 *)(self + 0x44) = zero2;
            *(s32 *)(self + 0xc) = 0x12;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xd8);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = zero2;
        }
    }
}

/* Same proximity-gated "used" transition shape as `sub_802C540`/
 * `sub_802C614` (actor_part19g.c), forwarding a fixed accumulator
 * delta of `4` to `sub_802C078(gUnknown_03000884, ...)`; tail-calls
 * `sub_802C4C8`. */
void sub_802CA6C(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0xc) != 0x12 && sub_802A6EC(self)) {
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_8022FEC(gUnknown_030012C0);
        sub_802C078(gUnknown_03000884, 4);
        *(s32 *)(self + 0xc) = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xd8);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
            *(s32 *)(self + 8) = zero2;
        }
    }

    sub_802C4C8(self);
}

/* Same shape as `sub_802CA6C` above, accumulator delta `1` instead of
 * `4`. */
void sub_802CAD0(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0xc) != 0x12 && sub_802A6EC(self)) {
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_8022FEC(gUnknown_030012C0);
        sub_802C078(gUnknown_03000884, 1);
        *(s32 *)(self + 0xc) = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xd8);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
            *(s32 *)(self + 8) = zero2;
        }
    }

    sub_802C4C8(self);
}

/* `InitActorPart`-based constructor: forwards `a`/`b`/`c`/`lastArg`
 * straight through, installs `self+0x50 = gStaticData_087E4F94`, then
 * classifies a "kind" (`self+0xc`) from a `sub_803ADB4`-scaled
 * function of `b` (clamped to `[0, 5]`) plus up to two `+6` bumps keyed
 * off `c`'s own range - selecting one of up to 18 per-kind anim
 * records from the part table (`self[0]`, stride `0xc`) to seed
 * `self+0x10`/`self+0x12`/`self+8`, the same idiom as `sub_802D648`
 * (`src/graphics/actor_part58.c`). */
void *sub_802CB34(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    u8 *self = selfArg;
    s32 idx;

    InitActorPart(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4F94;

    idx = sub_803ADB4((b >> 8) + 0x3c, 0x14);

    if (idx < 0) {
        idx = 0;
    }
    if (idx > 5) {
        idx = 5;
    }

    {
        s32 cShifted = c >> 8;

        if (cShifted <= 0x2b) {
            idx += 6;
        }
        if (cShifted <= 6) {
            idx += 6;
        }
    }

    *(s32 *)(self + 0xc) = idx;
    {
        register u8 *table asm("r1") = *(u8 **)self;
        u16 anim = *(u16 *)(idx * 0xc + table);
        register u8 zero1 asm("r1") = 0;
        register s32 zero2 asm("r2") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero1;
        *(s32 *)(self + 8) = zero2;
    }

    return self;
}

/* Thin `sub_802CB34`-forwarding constructor, `self+0x50` overridden to
 * `gStaticData_087E4EB4`. */
void *sub_802CB9C(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    u8 *self = selfArg;

    sub_802CB34(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4EB4;
    return self;
}

/* Same shape as `sub_802CB9C`, `self+0x50 = gStaticData_087E4ED4`. */
void *sub_802CBC0(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    u8 *self = selfArg;

    sub_802CB34(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4ED4;
    return self;
}

/* Same shape as `sub_802CB9C`, `self+0x50 = gStaticData_087E4EF4`. */
void *sub_802CBE4(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    u8 *self = selfArg;

    sub_802CB34(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4EF4;
    return self;
}

/* Same shape as `sub_802CB9C`, `self+0x50 = gStaticData_087E4F14`. */
void *sub_802CC08(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    u8 *self = selfArg;

    sub_802CB34(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4F14;
    return self;
}

/* Same shape as `sub_802CB9C`, `self+0x50 = gStaticData_087E4F34`, plus
 * a 6th argument stashed straight into `self+0x54`. */
void *sub_802CC2C(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg, s32 arg6)
{
    u8 *self = selfArg;

    sub_802CB34(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4F34;
    *(s32 *)(self + 0x54) = arg6;
    return self;
}

/* Same shape as `sub_802CB9C`, `self+0x50 = gStaticData_087E4F54`. */
void *sub_802CC54(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    u8 *self = selfArg;

    sub_802CB34(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4F54;
    return self;
}

/* Same shape as `sub_802CB9C`, `self+0x50 = gStaticData_087E4F74`. */
void *sub_802CC78(void *selfArg, s32 a, s32 b, s32 c, s32 lastArg)
{
    u8 *self = selfArg;

    sub_802CB34(self, a, b, c, lastArg);
    *(u8 **)(self + 0x50) = gStaticData_087E4F74;
    return self;
}

asm(".align 2, 0");
