#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern u8 gStaticData_087E54E4[];

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern void sub_8029E28(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);
extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern void *sub_80338C4(void);

/* Constructor: forwards to `InitActorPart`, then sets `self`'s health
 * (`+0x54=15`), event/trampoline table (`+0x50=&gStaticData_087E54E4`),
 * and caches its own `b`/`c` constructor args at `+0x58`/`+0x5c`;
 * finally resets state (`+0x28=0`) and the death flag (`+0x6c=0`).
 * Returns `self`. */
void *sub_8033BB8(void *selfArg, s32 a, s32 b, s32 cParam, s32 d)
{
    u8 *self = selfArg;
    register s32 bReg asm("r6") = b;
    register s32 c asm("r8") = cParam;
    register s32 dReg asm("r0") = d;
    register s32 health asm("r5") = 15;

    InitActorPart(self, a, b, cParam, dReg);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E54E4;
    *(s32 *)(self + 0x58) = bReg;
    *(s32 *)(self + 0x5c) = c;
    *(s32 *)(self + 0x28) = 0;
    self[0x6c] = 0;

    return self;
}

/* Plays a fixed sound cue (`sub_8029E28(0x400)`), then - if `self`'s
 * `+0x12` flag is set - fires the `self+0x50` event table's slot-3
 * trampoline at `self` offset by the table's `+8` halfword. */
void sub_8033BFC(void *selfArg)
{
    u8 *self = selfArg;

    sub_8029E28(0x400);

    if (self[0x12] != 0 && self != NULL) {
        register u8 *table asm("r1") = *(u8 **)(self + 0x50);
        register u8 *addr asm("r0");
        void *fn;

        {
            register s32 eight asm("r2") = 8;

            addr = self + *(s16 *)((u8 *)table + eight);
        }
        fn = *(void **)(table + 0xc);

        sub_803AD80(addr, (void *)3, fn);
    }
}

/* Sets `self`'s position fields (`+0x1c`/`+0x20`/`+0x24`) from the
 * singleton's own position plus a fixed offset, and - while `self+0x34`
 * is still under its `0x4AFF` threshold - switches `self` to state 1/
 * table-index 1, seeding `+0x64`/`+0x68` from `gUnknown_030015DC`'s
 * table and resetting the anim-frame pair from `self`'s part table's
 * `+0xc` field. */
void sub_8033C28(void *selfArg)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x1c) = sub_8033900() + 0x2000;
    *(s32 *)(self + 0x20) = sub_80338F4() + 0x3000;
    *(s32 *)(self + 0x24) = sub_80338E8() - 0x100;

    if (*(s32 *)(self + 0x34) <= 0x4AFF) {
        u8 *table = sub_80338C4();

        *(s32 *)(self + 0x64) = *(s32 *)(table + 0x10);
        {
            register s32 zero asm("r2") = 0;

            *(s32 *)(self + 0x68) = zero;
            {
                register s32 one asm("r0") = 1;

                *(s32 *)(self + 0x28) = one;
                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = one;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero2;
                }
                *(s32 *)(self + 8) = zero;
            }
        }
    }
}
