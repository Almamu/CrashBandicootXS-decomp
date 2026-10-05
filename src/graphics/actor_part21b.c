#include "core.h"
#include "actor_self.h"

/* Same "self" object family as actor_part20b.c - see
 * docs/matching/issue-58-0x08030334-actor.md. */

extern struct actor_pmf gAirshipFireballStateFuncs[];

/* `UpdateAirshipFireball`'s (actor_part20b.c) per-state member-pointer dispatch
 * without its tail: `(this->*gAirshipFireballStateFuncs[this->state])()`
 * (see `ACTOR_PMF_CALL`). */
void RunAirshipFireballState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gAirshipFireballStateFuncs);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
