#include "core.h"
#include "actor_self.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

/* A physics-step-and-collision-react updater: advances `self`'s
 * position by its `velX`/`velY` pair (with a fixed
 * gravity-like offset on Y) and a fixed Z step, then reacts to a
 * `sub_802A3AC` collision probe - firing a trampoline on the hit
 * object if any (then falling into the same "own trampoline" tail as
 * below), else checking `sub_8031378` (an AABB overlap test) and a
 * `depth` threshold before firing `self`'s own method-table
 * `m08` ("hit/destroy") trampoline ( no `self` NULL-guard on this
 * specific call - unlike the other paths here) or, once past the
 * threshold, falling back to `sub_802A7B8` unconditionally (also no
 * NULL-guard). Two gaps were fixed to get this byte-exact:
 * (1) the `depth` threshold check and its two arms had to be
 * written as `if (cond) {...} else {sub_802A7B8(...);}` with the
 * *shared* tail code as a `merge:` label the `goto`s land on, not as
 * an early-return `else if` - this compiler places an `if`'s `else`
 * body last in program order but an early-returning `else if` chain's
 * next statement first, which put `sub_802A7B8`'s call block in the
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
 * calls `sub_802F97C` by name (only indirectly via a `void *`-typed
 * function-pointer table entry), so the parameter's own type here
 * doesn't need to match the usual `void *` convention. */
extern void *sub_802A3AC(void *selfArg);
extern u8 sub_8031378(void *selfArg);
extern void sub_803146C(s32 delta);
extern s32 sub_803AD80(void *pos, s32 arg1, void *table);
extern void sub_802A7B8(void *selfArg);

struct actor_falling {
    struct actor_self base;
    s32 hp;             // 0x54
    s32 velX;           // 0x58
    s32 velY;           // 0x5C
};

void sub_802F97C(struct actor_falling *self)
{
    struct actor_vtable *table;
    s32 off;

    self->base.x += self->velX;
    self->base.y += -0xc0 + self->velY;
    self->base.z += 0x400;

    {
        struct actor_self *hit = sub_802A3AC(self);

        if (hit != 0) {
            struct actor_vtable *hitTable = hit->vtable;

            sub_803AD80((u8 *)hit + hitTable->m20.thisOffset, 2, hitTable->m20.fn);
            goto merge;
        } else if (sub_8031378(self)) {
            sub_803146C(2);
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
            sub_803AD80((u8 *)self + off, 3, table->m08.fn);
        } else {
            sub_802A7B8(self);
        }
    }
}

asm(".align 2, 0");
