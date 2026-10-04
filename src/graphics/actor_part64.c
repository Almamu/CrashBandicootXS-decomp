#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part57.c (constructed by
 * `sub_8033EF4`) - see docs/matching/issue-63-0x08033ef4-actor.md. */

extern struct actor_pmf gStaticData_0817C4F8[];

/* Per-state member-pointer dispatch, `(this->*gStaticData_0817C4F8
 * [this->state])()` (see `ACTOR_PMF_CALL`); returns 0 once the
 * state-2 animation has played through, 1 otherwise. */
s32 sub_8033FE4(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gStaticData_0817C4F8);

    if (self->state == 2 && self->animDone != 0) {
        return 0;
    }
    return 1;
}
