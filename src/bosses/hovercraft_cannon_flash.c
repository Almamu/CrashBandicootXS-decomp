#include "core.h"
#include "match.h"
#include "actor.h"
#include "bosses.h"

/* Same "self" object family as hovercraft_side_gun.c - see that file's header
 * comment and docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* Syncs `self`'s position from the singleton's own position plus a
 * fixed offset, sets `unshootable`, and - once the animation is done
 * (`animDone`) and `self` is non-NULL - calls the "destroy" virtual
 * with 3; otherwise calls `UpdateActor(self)`.
 *
 * The ROM computes a "should animate" 0/1 value into a register and
 * then re-checks it against zero before deciding whether to call
 * `UpdateActor`, even though the value is a compile-time constant on
 * each path - this compiler's dead-store/dead-branch elimination
 * always collapses that redundant compute-then-recheck step for a
 * plain `s32 doAnim`. A `MATCH_KEEP_VOLATILE(doAnim)`
 * right before the check makes the value opaque to the compiler,
 * forcing the recheck to materialize - the same class of gap already
 * closed for `DrawPolarCollectedWumpa` (issue #52) and the `| 0`-with-a-zero-
 * valued-term case in `DrawJetpackCheckpointText` (issue #71). */
extern s32 _call_via_r2(void *arg0, void *arg1, void *fn);

void UpdateHovercraftCannonFlash(void *selfArg)
{
    struct cannon_flash *self = selfArg;
    MATCH_HOLD_REG(s32, doAnim, r0);

    self->base.z = GetHovercraftZ() - 0x200;
    self->base.x = GetHovercraftX() + 0x2000;
    self->base.y = GetHovercraftY() + 0x3000;
    self->unshootable = 1;

    if (self->base.animDone != 0) {
        if (self != NULL) {
            struct actor_vtable *table = self->base.vtable;
            u8 *addr = (u8 *)self + table->destroy.thisOffset;
            void *fn = table->destroy.fn;

            _call_via_r2(addr, (void *)3, fn);
        }
        doAnim = 0;
    } else {
        doAnim = 1;
    }

    MATCH_KEEP_VOLATILE(doAnim);
    if (doAnim != 0) {
        UpdateActor(self);
    }
}

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in action_ctrl.cpp/hovercraft_parts.c/hovercraft_cannon.c. This is a
 * third, much smaller object kind (`struct cannon_flash`, vtable
 * `gHovercraftCannonFlashVtable`) whose only own fields are `hp` and the
 * `unshootable` flag. See docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* Constructor: forwards straight through to `InitActorPart`, then sets
 * `hp = 1` and the method table (`gHovercraftCannonFlashVtable`), resets
 * the state and animation (ACTOR_SET_STATE's stores, state 0), and sets
 * `unshootable`. Returns `self`. */
void *CreateHovercraftCannonFlash(void *selfArg, void *part, s32 b, s32 c, s32 d)
{
    struct cannon_flash *self = selfArg;
    MATCH_HOLD_REG(s32, one, r5) = 1;

    InitActorPart(self, part, b, c, d);
    self->hp = one;
    self->base.vtable = (struct actor_vtable *)gHovercraftCannonFlashVtable;
    {
        MATCH_HOLD_REG(s32, zero, r1) = 0;

        self->base.state = zero;
        self->base.stateTime = zero;
        self->base.animIndex = zero;
        {
            MATCH_HOLD_REG(u16, anim, r0) = self->base.anims[0].duration;
            MATCH_HOLD_REG(u8, zero2, r2) = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero2;
        }
        self->base.animTime = zero;
    }
    self->unshootable = one;

    return self;
}

/* Same "self" object family as hovercraft_launcher.c - see that file's header
 * comment and docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* Same position-sync/flag/trampoline shape as `UpdateHovercraftCannonFlash`
 * (above), but returns the "should animate" boolean directly
 * instead of calling `UpdateActor` itself - since the value only ever
 * needs to reach the return register (no re-check against zero the
 * way `UpdateHovercraftCannonFlash`'s own call-vs-no-call decision needs), the
 * compute-then-recheck gap that function hit doesn't apply here:
 * plain C matches byte-for-byte immediately. The same "update without
 * its UpdateActor tail" as RunHovercraftCannonState (hovercraft_cannon.c),
 * returning 0 once `self` has destroyed itself.
 * UNUSED - no caller anywhere in the ROM (checked every src/ .c file and
 * every word-aligned Thumb pointer in baserom.gba). */

s32 RunHovercraftCannonFlashState(void *selfArg)
{
    struct cannon_flash *self = selfArg;
    s32 doAnim;

    self->base.z = GetHovercraftZ() - 0x200;
    self->base.x = GetHovercraftX() + 0x2000;
    self->base.y = GetHovercraftY() + 0x3000;
    self->unshootable = 1;

    if (self->base.animDone != 0) {
        if (self != NULL) {
            struct actor_vtable *table = self->base.vtable;
            u8 *addr = (u8 *)self + table->destroy.thisOffset;
            void *fn = table->destroy.fn;

            _call_via_r2(addr, (void *)3, fn);
        }
        doAnim = 0;
    } else {
        doAnim = 1;
    }

    return doAnim;
}

/* Same "self" object family as hovercraft_launcher.c - see that file's header
 * comment and docs/matching/archive/issue-63-0x08033ef4-actor.md. */

/* Constant getter - returns `self`'s `unshootable` flag. */
u8 IsHovercraftCannonFlashUnshootable(void *selfArg)
{
    struct cannon_flash *self = selfArg;

    return self->unshootable;
}
