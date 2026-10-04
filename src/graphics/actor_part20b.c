#include "core.h"
#include "actor_self.h"

/* Same large per-instance "self" object family as actor_part20.c - see
 * that file's header comment and docs/matching/issue-58-0x08030334-actor.md. */

extern struct actor_pmf gStaticData_0817C2B8[];
extern void UpdateActor(void *self);

/* Per-state member-pointer dispatch, `(this->*gStaticData_0817C2B8
 * [this->state])()` (see `ACTOR_PMF_CALL`), then either the "destroy"
 * virtual call once the state-2 animation has played through, or the
 * standard UpdateActor step. */
void sub_8030574(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gStaticData_0817C2B8);

    if (self->state == 2 && self->animDone != 0) {
        if (self != NULL) {
            ACTOR_VCALL(self, destroy, 3);
        }
    } else {
        UpdateActor(self);
    }
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
