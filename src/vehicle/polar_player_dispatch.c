#include "core.h"
#include "actor_self.h"
#include "vehicle.h"

/* Per-state member-pointer dispatch, `(this->*gPolarPlayerStateFuncs
 * [this->state])()`. */
void RunPolarPlayerState(struct actor_self *self)
{
    ACTOR_PMF_CALL(self, gPolarPlayerStateFuncs);
}
