#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching/issue-56-0x0802f0dc-actor.md,
 * "Parked, not matched: sub_802FA04" for the full account; compiled
 * only under `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c99c_2fa04.s) is used otherwise.
 * An `InitActorPart`-based constructor for this cluster's `self`
 * object: forwards its first three real arguments plus one stack
 * argument straight to `InitActorPart`, then marks `self+0x54` = 1,
 * sets `self+0x50`'s event/trampoline table to `gStaticData_087E517C`,
 * and stashes its remaining two stack arguments into `self+0x58`/
 * `self+0x5c`. Every load/store and call confirmed correct - the same
 * 7-argument `InitActorPart`-wrapper shape already left raw as
 * `sub_80305F8` (docs/matching/issue-58-0x08030334-actor.md); parked
 * because this compiler always pushes only as many high registers
 * (r4-r7) as it independently decides it needs for its own constant/
 * stack-argument evaluation order, never matching the ROM's specific
 * `r4=self,r5=1,r6=e,r7=f` assignment (and the matching 4-register
 * push/pop) without either an extra, incorrect push/pop of `r8` (a
 * relay attempt) or losing the `d` stack argument's value outright to
 * a register collision with an explicit `r7` pin. */
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E517C[];

void *sub_802FA04(void *selfArg, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    u8 *self = selfArg;

    InitActorPart(self, a, b, c, d);
    *(s32 *)(self + 0x54) = 1;
    *(void **)(self + 0x50) = gStaticData_087E517C;
    *(s32 *)(self + 0x58) = e;
    *(s32 *)(self + 0x5c) = f;
    return self;
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
