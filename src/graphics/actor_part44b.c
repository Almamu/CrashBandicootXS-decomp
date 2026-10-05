#include "core.h"
#include "actor_self.h"

/* Same "spawn/pre-attack" singleton family as actor_part39.c - see that
 * file's header comment and docs/matching/issue-56-0x0802f0dc-actor.md. */

extern struct actor_pmf gJetpackPlayerStateFuncs[];

/* Per-state member-pointer dispatch, `(this->*gJetpackPlayerStateFuncs
 * [this->state])()` (see `ACTOR_PMF_CALL`). */
void RunJetpackPlayerState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gJetpackPlayerStateFuncs);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
