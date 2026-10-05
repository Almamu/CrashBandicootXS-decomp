#include "core.h"
#include "actor_self.h"
#include "vehicle.h"


/* Per-state member-pointer dispatch, `(this->*gPolarPlayerStateFuncs
 * [this->state])()`. */
void RunPolarPlayerState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gPolarPlayerStateFuncs);
}

/* Pad to the next word with zeros, as the ROM does. */
asm(".align 2, 0");
