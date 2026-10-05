#include "core.h"
#include "actor_self.h"
#include "bosses.h"
#include "vehicle.h"

/*
 * ROM 0x0817C260-0x0817C2D0: three per-state member-function-pointer
 * tables (ACTOR_PMF_CALL, include/actor_self.h). Linked in ROM order
 * between data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* Dispatched by UpdateJetpackPlane and RunJetpackPlaneState
 * (jetpack_plane.c). */
const struct actor_pmf gJetpackPlaneStateFuncs[4] = {
    ACTOR_PMF(JetpackPlaneStateFly),
    ACTOR_PMF(sub_802FE58),
    ACTOR_PMF(sub_802FE1C),
    ACTOR_PMF(JetpackPlaneStateFall),
};

/* Dispatched by UpdateJetpackBomber and RunJetpackBomberState (jetpack_plane.c). */
const struct actor_pmf gJetpackBomberStateFuncs[7] = {
    ACTOR_PMF(JetpackBomberStateIdle),
    ACTOR_PMF(JetpackBomberStateHome),
    ACTOR_PMF(JetpackBomberStateBobVertical),
    ACTOR_PMF(JetpackBomberStateSwingHorizontal),
    ACTOR_PMF(JetpackBomberStateCircle),
    ACTOR_PMF(JetpackBomberStateDrop),
    ACTOR_PMF(JetpackBomberStateDying),
};

/* Dispatched by UpdateAirshipFireball (airship_fireball.c) and RunAirshipFireballState
 * (airship_fireball.c). */
const struct actor_pmf gAirshipFireballStateFuncs[3] = {
    ACTOR_PMF(AirshipFireballStateOrbit),
    ACTOR_PMF(AirshipFireballStateSpiralIn),
    ACTOR_PMF(AirshipFireballStateExplode),
};
