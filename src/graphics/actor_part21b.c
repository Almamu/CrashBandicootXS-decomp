#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part20b.c - see
 * docs/matching/issue-58-0x08030334-actor.md. */

extern struct actor_pmf gStaticData_0817C2B8[];

/* `sub_8030574`'s (actor_part20b.c) per-state member-pointer dispatch
 * without its tail: `(this->*gStaticData_0817C2B8[this->state])()`
 * (see `ACTOR_PMF_CALL`). */
void sub_8030648(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gStaticData_0817C2B8);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
