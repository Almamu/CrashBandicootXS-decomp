#include "core.h"
#include "actor_self.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

extern struct actor_pmf gStaticData_0817C1C0[];

/* Per-state member-pointer dispatch, `(this->*gStaticData_0817C1C0
 * [this->state])()` (see `ACTOR_PMF_CALL`). */
void sub_802F748(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gStaticData_0817C1C0);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
