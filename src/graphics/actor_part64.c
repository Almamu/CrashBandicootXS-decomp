#include "core.h"
#include "actor_self.h"

/* Same "self" object family as action_ctrl.c (constructed by
 * `CreateHovercraftLauncher`) - see docs/matching/issue-63-0x08033ef4-actor.md. */

extern struct actor_pmf gHovercraftLauncherStateFuncs[];

/* Per-state member-pointer dispatch, `(this->*gHovercraftLauncherStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`); returns 0 once the
 * state-2 animation has played through, 1 otherwise. */
s32 RunHovercraftLauncherState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gHovercraftLauncherStateFuncs);

    if (self->state == 2 && self->animDone != 0) {
        return 0;
    }
    return 1;
}
