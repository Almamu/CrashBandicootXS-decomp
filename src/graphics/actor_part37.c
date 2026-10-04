#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern struct actor_pmf gStaticData_0817C4F8[];
extern void sub_802A7B8(void *self);

/* Per-state member-pointer dispatch, `(this->*gStaticData_0817C4F8
 * [this->state])()` (see `ACTOR_PMF_CALL`), then the standard
 * sub_802A7B8 step unless the state-2 animation has played through. */
void sub_8033E80(struct actor_self *self)
{
    s32 state;
    s32 step;

    ACTOR_PMF_CALL(self, gStaticData_0817C4F8);

    state = self->state;
    step = 1;
    if (state == 2 && self->animDone != 0) {
        step = 0;
    }
    if (step) {
        sub_802A7B8(self);
    }
}
