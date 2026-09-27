#include "core.h"
#include "actor_self.h"


extern struct actor_pmf gStaticData_0817A6B8[];

ACTOR_CALL_VIA_ALIASES

/* Per-state member-pointer dispatch, `(this->*gStaticData_0817A6B8
 * [this->state])()`. */
void sub_802C208(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gStaticData_0817A6B8);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
