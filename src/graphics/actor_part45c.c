#include "core.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E517C[];

/* An `InitActorPart`-based constructor for this cluster's `self` object:
 * forwards its first three real arguments plus one stack argument
 * straight to `InitActorPart`, then marks `self+0x54` = 1, sets
 * `self+0x50`'s event/trampoline table to `gStaticData_087E517C`, and
 * stashes its remaining two stack arguments into `self+0x58`/`self+0x5c`.
 * The same 7-argument `InitActorPart`-wrapper shape already left raw as
 * `sub_80305F8` (docs/matching/issue-58-0x08030334-actor.md). The ROM
 * wants `self`/the constant `1`/`e`/`f` pinned to `r4`/`r5`/`r6`/`r7`
 * respectively, all kept live across the `InitActorPart` call, with a
 * matching 4-register `push`/`pop`. Explicitly pinning `e`/`f` to their
 * target registers (`register s32 x asm("r6"|"r7") = ...;`) either adds
 * a spurious extra `r8` push/pop (when the pin forces a relay) or -
 * for `r7` specifically - drops that register from the compiler's own
 * push/pop list outright (a genuine agbcc/gcc 2.9 Thumb-prologue bug,
 * not just a missed optimization). The fix: pin only the constant `1`
 * to `r5`; leave `self`, `d` and both `e`/`f` completely unpinned
 * (`self` as a plain `u8 *` local, `d` used directly as the call's
 * stack argument, `e`/`f` as plain `register` locals with no explicit
 * hardware register). With that much natural register pressure, this
 * compiler's own allocator picks `r4`/`r6`/`r7` for `self`/`e`/`f` on
 * its own - correctly including all of `r4`-`r7` in the push/pop list
 * - and, in this exact declaration order (`self`, then `one`, then
 * `eReg`, then `fReg`), schedules the loads in the ROM's own
 * self/d/e/f/one order. */
void *sub_802FA04(void *selfArg, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    u8 *self = selfArg;
    register s32 one asm("r5") = 1;
    register s32 eReg = e;
    register s32 fReg = f;

    InitActorPart(self, a, b, c, d);
    *(s32 *)(self + 0x54) = one;
    *(void **)(self + 0x50) = gStaticData_087E517C;
    *(s32 *)(self + 0x58) = eReg;
    *(s32 *)(self + 0x5c) = fReg;

    return self;
}

asm(".align 2, 0");
