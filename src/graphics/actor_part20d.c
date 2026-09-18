#include "core.h"

/* Same large per-instance "self" object family as actor_part20.c/
 * actor_part20b.c - see actor_part20.c's header comment and
 * docs/matching/issue-58-0x08030334-actor.md. */

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E525C[];

/* An `InitActorPart`-based constructor: forwards all 4 of its own real
 * arguments (the last stack-passed) straight to `InitActorPart`, then
 * marks `self+0x54 = 2`, sets `self+0x50`'s event/trampoline table to
 * `gStaticData_087E525C`, and stashes its own `b`/`c` arguments a
 * second time into `self+0x58`/`self+0x5c`, `self+0x64 = 0`,
 * `self+0x60 = 0x95`, `self+0x68 (byte) = 0`. Returns `self` - the same
 * shape as the already-matched `sub_8033BB8` (actor_part32.c) and the
 * still-parked `sub_802FA04` (actor_part45c.c), except this one's `d`
 * argument is itself stack-passed (a 5th real argument total) rather
 * than the 4th register argument. Pinning `d` to `r0` *after* the other
 * register pins (rather than alongside them) is what gets this
 * compiler to fetch the stack argument in the same position the ROM's
 * own build does - declaring it earlier reorders the fetch ahead of the
 * `r6`/`r8` parameter homes, which is the "4-instruction scheduling
 * permutation" this function previously resisted. */
void *sub_80305F8(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 bReg asm("r6") = b;
    register s32 cReg asm("r8") = c;
    register s32 dReg asm("r0") = d;
    register s32 health asm("r5") = 2;

    InitActorPart(self, a, b, c, dReg);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E525C;
    *(s32 *)(self + 0x58) = bReg;
    *(s32 *)(self + 0x5c) = cReg;
    *(s32 *)(self + 0x64) = 0;
    *(s32 *)(self + 0x60) = 0x95;
    self[0x68] = 0;

    return self;
}

asm(".align 2, 0");
