#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817A6B8-0x0817A728. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void PolarPlayerStateMount();
extern void PolarPlayerStateRun();
extern void PolarPlayerStateJump();
extern void PolarPlayerStateDash();
extern void PolarPlayerStateShocked();
extern void PolarPlayerStateCaught();
extern void sub_802BD24();
extern void PolarPlayerStateFinishLeap();
extern void PolarPlayerStateCarriedOff();
extern void PolarPlayerStateKnockedOff();
extern void PolarPlayerStateBoost();
extern void PolarPlayerStateLaunched();
extern void PolarPlayerStateFinish();
extern void PolarPlayerStateLand();

/* Per-state handlers of the actor object dispatched by UpdatePolarPlayer
 * (polar_player.c) and RunPolarPlayerState (polar_player_dispatch.c). */
const struct actor_pmf gPolarPlayerStateFuncs[14] = {
    ACTOR_PMF(PolarPlayerStateMount),
    ACTOR_PMF(PolarPlayerStateRun),
    ACTOR_PMF(PolarPlayerStateDash),
    ACTOR_PMF(PolarPlayerStateBoost),
    ACTOR_PMF(PolarPlayerStateJump),
    ACTOR_PMF(PolarPlayerStateLaunched),
    ACTOR_PMF(PolarPlayerStateKnockedOff),
    ACTOR_PMF(PolarPlayerStateCaught),
    ACTOR_PMF(PolarPlayerStateCarriedOff),
    ACTOR_PMF(PolarPlayerStateLand),
    ACTOR_PMF(PolarPlayerStateFinish),
    ACTOR_PMF(PolarPlayerStateFinishLeap),
    ACTOR_PMF(PolarPlayerStateShocked),
    ACTOR_PMF(sub_802BD24),
};
