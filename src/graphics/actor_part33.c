#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern struct actor_pmf gHovercraftCannonStateFuncs[];

/* Per-state member-pointer dispatch, `(this->*gHovercraftCannonStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`); returns 0 once the
 * state-2 animation has played through, 1 otherwise. */
s32 RunHovercraftCannonState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gHovercraftCannonStateFuncs);

    if (self->state == 2 && self->animDone != 0) {
        return 0;
    }
    return 1;
}
