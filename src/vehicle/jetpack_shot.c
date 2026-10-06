#include "core.h"
#include "match.h"
#include "actor_self.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/archive/issue-56-0x0802f0dc-actor.md. */

/* A physics-step-and-collision-react updater: advances `self`'s
 * position by its `velX`/`velY` pair (with a fixed
 * gravity-like offset on Y) and a fixed Z step, then reacts to a
 * `FindShotTarget` collision probe - firing a trampoline on the hit
 * object if any (then falling into the same "own trampoline" tail as
 * below), else checking `IsTouchingAirship` (an AABB overlap test) and a
 * `depth` threshold before firing `self`'s own method-table
 * `destroy` ("hit/destroy") trampoline ( no `self` NULL-guard on this
 * specific call - unlike the other paths here) or, once past the
 * threshold, falling back to `UpdateActor` unconditionally (also no
 * NULL-guard). Two gaps were fixed to get this byte-exact:
 * (1) the `depth` threshold check and its two arms had to be
 * written as `if (cond) {...} else {UpdateActor(...);}` with the
 * *shared* tail code as a `merge:` label the `goto`s land on, not as
 * an early-return `else if` - this compiler places an `if`'s `else`
 * body last in program order but an early-returning `else if` chain's
 * next statement first, which put `UpdateActor`'s call block in the
 * wrong place relative to the shared tail even though every
 * individual instruction already matched; (2) the ROM makes an
 * inconsistent (`r2` vs `r3`) scratch-register choice for the `8`
 * immediate in the two otherwise-identical method-table lookups,
 * where this compiler always picks the same register for both - each
 * lookup gets a small inline-asm anchor spelling out the exact
 * `mov rN, #8` / `ldrsh` pair to force the ROM's register in each arm
 * independently (deliberately no clobber list on either asm block -
 * adding one, even for the register the asm text itself already
 * hardcodes, was enough added register pressure to make this compiler
 * spill a second copy of `self` into `r5`); relatedly, `self` is typed
 * as the parameter directly rather than this file's usual `void *selfArg`
 * parameter plus a local `self = selfArg;` copy -
 * with the copy, this compiler spills that same second `r5` copy of
 * `self` regardless of the asm blocks above, apparently treating a
 * separately-declared local (even one merely assigned, not
 * initialized, from the parameter) as needing a more conservative,
 * stack-like allocation across this function's several `goto`s than
 * the parameter register itself gets. Nothing else in this project
 * calls `UpdateJetpackShot` by name (only indirectly via a `void *`-typed
 * function-pointer table entry), so the parameter's own type here
 * doesn't need to match the usual `void *` convention. */
extern s32 _call_via_r2(void *pos, s32 arg1, void *table);

struct actor_falling {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 velX;           // 0x58
    s32 velY;           // 0x5C
};

void UpdateJetpackShot(struct actor_falling *self)
{
    struct actor_vtable *table;
    s32 off;

    self->base.x += self->velX;
    self->base.y += -0xc0 + self->velY;
    self->base.z += 0x400;

    {
        struct actor_self *hit = FindShotTarget((struct actor_self *)self);

        if (hit != 0) {
            struct actor_vtable *hitTable = hit->vtable;

            _call_via_r2((u8 *)hit + hitTable->m20.thisOffset, 2, hitTable->m20.fn);
            goto merge;
        } else if (IsTouchingAirship(self)) {
            DamageAirship(2);
            if (self == 0) {
                return;
            }
            table = self->base.vtable;
            asm volatile("mov r3, #8\n\tldrsh %0, [%1, r3]" : "=r"(off) : "r"(table));
            goto tail;
        } else if (self->base.depth > 0x8200) {
merge:
            if (self == 0) {
                return;
            }
            table = self->base.vtable;
            asm volatile("mov r2, #8\n\tldrsh %0, [%1, r2]" : "=r"(off) : "r"(table));
tail:
            _call_via_r2((u8 *)self + off, 3, table->destroy.fn);
        } else {
            UpdateActor(self);
        }
    }
}

asm(".align 2, 0");

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/archive/issue-56-0x0802f0dc-actor.md. */

/* An `InitActorPart`-based constructor for this cluster's `self` object:
 * forwards its first three real arguments plus one stack argument
 * straight to `InitActorPart`, then marks `self+0x54` = 1, sets
 * `self+0x50`'s event/trampoline table to `gJetpackShotVtable`, and
 * stashes its remaining two stack arguments into `self+0x58`/`self+0x5c`.
 * The same 7-argument `InitActorPart`-wrapper shape already left raw as
 * `CreateAirshipFireball` (docs/matching/archive/issue-58-0x08030334-actor.md). The ROM
 * wants `self`/the constant `1`/`e`/`f` pinned to `r4`/`r5`/`r6`/`r7`
 * respectively, all kept live across the `InitActorPart` call, with a
 * matching 4-register `push`/`pop`. Explicitly pinning `e`/`f` to their
 * target registers (`MATCH_HOLD_REG(s32, x, r6 / r7) = ...;`) either adds
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
void *CreateJetpackShot(void *selfArg, void *part, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    u8 *self = selfArg;
    MATCH_HOLD_REG(s32, one, r5) = 1;
    register s32 eReg = e;
    register s32 fReg = f;

    InitActorPart(self, part, b, c, d);
    *(s32 *)(self + 0x54) = one;
    *(void **)(self + 0x50) = (void *)gJetpackShotVtable;
    *(s32 *)(self + 0x58) = eReg;
    *(s32 *)(self + 0x5c) = fReg;

    return self;
}

asm(".align 2, 0");

/* Same "spawn/pre-attack" singleton family as wumpa.c - see that
 * file's header comment and docs/matching/archive/issue-56-0x0802f0dc-actor.md. */

/* Trivial constant predicate - always "true". */
s32 IsJetpackShotUnshootable(void)
{
    return 1;
}

asm(".align 2, 0");
