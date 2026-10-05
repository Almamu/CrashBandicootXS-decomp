#include "core.h"

/* Same "self" object family as actor_part61.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

/* Syncs `self`'s position fields from the singleton's own position plus
 * a fixed offset, sets the one-shot flag (`+0x58=1`), and - if
 * `self+0x12` is set and `self` is non-NULL - fires the `self+0x50`
 * event table's slot-3 trampoline; otherwise calls `UpdateActor(self)`.
 *
 * The ROM computes a "should animate" 0/1 value into a register and
 * then re-checks it against zero before deciding whether to call
 * `UpdateActor`, even though the value is a compile-time constant on
 * each path - this compiler's dead-store/dead-branch elimination
 * always collapses that redundant compute-then-recheck step for a
 * plain `s32 doAnim`. An empty `asm volatile("" : "+r"(doAnim))`
 * right before the check makes the value opaque to the compiler,
 * forcing the recheck to materialize - the same class of gap already
 * closed for `DrawPolarCollectedWumpa` (issue #52) and the `| 0`-with-a-zero-
 * valued-term case in `DrawJetpackCheckpointText` (issue #71). */
extern s32 GetHovercraftZ(void);
extern s32 GetHovercraftX(void);
extern s32 GetHovercraftY(void);
extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);
extern void UpdateActor(void *self);

void UpdateHovercraftCannonFlash(void *selfArg)
{
    u8 *self = selfArg;
    register s32 doAnim asm("r0");

    *(s32 *)(self + 0x24) = GetHovercraftZ() - 0x200;
    *(s32 *)(self + 0x1c) = GetHovercraftX() + 0x2000;
    *(s32 *)(self + 0x20) = GetHovercraftY() + 0x3000;
    self[0x58] = 1;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *table = *(u8 **)(self + 0x50);
            u8 *addr = self + *(s16 *)(table + 8);
            void *fn = *(void **)(table + 0xc);

            _call_via_r2(addr, (void *)3, fn);
        }
        doAnim = 0;
    } else {
        doAnim = 1;
    }

    asm volatile("" : "+r"(doAnim));
    if (doAnim != 0) {
        UpdateActor(self);
    }
}

asm(".align 2, 0");

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in action_ctrl.c/actor_part28.c/actor_part32.c. This is a
 * third, much smaller object kind (vtable `gHovercraftCannonFlashVtable`) that
 * reuses `self+0x58` as a plain one-shot flag rather than a health
 * countdown. See docs/matching/issue-63-0x08033ef4-actor.md. */

extern void *InitActorPart(void *selfArg, void *part, s32 b, s32 c, s32 d);
extern u8 gHovercraftCannonFlashVtable[];

/* Constructor: forwards straight through to `InitActorPart`, then sets
 * health (`+0x54=1`), the event table (`+0x50=&gHovercraftCannonFlashVtable`),
 * resets state/frame-counter/table-index/anim/accumulator, and sets the
 * one-shot flag (`+0x58=1`). Returns `self`. */
void *CreateHovercraftCannonFlash(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register s32 one asm("r5") = 1;

    InitActorPart(self, part, b, c, d);
    *(s32 *)(self + 0x54) = one;
    *(void **)(self + 0x50) = gHovercraftCannonFlashVtable;
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

/* Same "self" object family as actor_part63.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

/* Same position-sync/flag/trampoline shape as `UpdateHovercraftCannonFlash`
 * (actor_part68.c), but returns the "should animate" boolean directly
 * instead of calling `UpdateActor` itself - since the value only ever
 * needs to reach the return register (no re-check against zero the
 * way `UpdateHovercraftCannonFlash`'s own call-vs-no-call decision needs), the
 * compute-then-recheck gap that function hit doesn't apply here:
 * plain C matches byte-for-byte immediately. */

s32 sub_8034314(void *selfArg)
{
    u8 *self = selfArg;
    s32 doAnim;

    *(s32 *)(self + 0x24) = GetHovercraftZ() - 0x200;
    *(s32 *)(self + 0x1c) = GetHovercraftX() + 0x2000;
    *(s32 *)(self + 0x20) = GetHovercraftY() + 0x3000;
    self[0x58] = 1;

    if (self[0x12] != 0) {
        if (self != NULL) {
            u8 *table = *(u8 **)(self + 0x50);
            u8 *addr = self + *(s16 *)(table + 8);
            void *fn = *(void **)(table + 0xc);

            _call_via_r2(addr, (void *)3, fn);
        }
        doAnim = 0;
    } else {
        doAnim = 1;
    }

    return doAnim;
}

asm(".align 2, 0");

/* Same "self" object family as actor_part63.c - see that file's header
 * comment and docs/matching/issue-63-0x08033ef4-actor.md. */

/* Constant getter - returns `self`'s one-shot flag (`self+0x58`). */
u8 IsHovercraftCannonFlashUnshootable(void *selfArg)
{
    u8 *self = selfArg;

    return self[0x58];
}

asm(".align 2, 0");
