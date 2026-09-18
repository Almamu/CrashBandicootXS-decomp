#include "core.h"

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in actor_part57.c/actor_part28.c/actor_part32.c. This is a
 * third, much smaller object kind (vtable `gStaticData_087E558C`) that
 * reuses `self+0x58` as a plain one-shot flag rather than a health
 * countdown. See docs/matching/issue-63-0x08033ef4-actor.md. */

extern void *InitActorPart(void *selfArg, void *part, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E558C[];

/* Constructor: forwards straight through to `InitActorPart`, then sets
 * health (`+0x54=1`), the event table (`+0x50=&gStaticData_087E558C`),
 * resets state/frame-counter/table-index/anim/accumulator, and sets the
 * one-shot flag (`+0x58=1`). Returns `self`. */
void *sub_80342D4(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 one asm("r5") = 1;

    InitActorPart(self, part, b, c, d);
    *(s32 *)(self + 0x54) = one;
    *(void **)(self + 0x50) = gStaticData_087E558C;
    {
        register s32 zero asm("r1") = 0;

        *(s32 *)(self + 0x28) = zero;
        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)*(void **)self;
            register u8 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero2;
        }
        *(s32 *)(self + 8) = zero;
    }
    self[0x58] = one;

    return self;
}

asm(".align 2, 0");
