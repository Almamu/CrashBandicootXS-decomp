#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern struct actor_pmf gHovercraftLauncherStateFuncs[];
extern void UpdateActor(void *self);

/* Per-state member-pointer dispatch, `(this->*gHovercraftLauncherStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`), then the standard
 * UpdateActor step unless the state-2 animation has played through. */
void UpdateHovercraftLauncher(struct actor_self *self)
{
    s32 state;
    s32 step;

    ACTOR_PMF_CALL(self, gHovercraftLauncherStateFuncs);

    state = self->state;
    step = 1;
    if (state == 2 && self->animDone != 0) {
        step = 0;
    }
    if (step) {
        UpdateActor(self);
    }
}
