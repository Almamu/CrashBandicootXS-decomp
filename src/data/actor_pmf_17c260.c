#include "core.h"
#include "actor_self.h"
#include "vehicle.h"

/*
 * ROM 0x0817C260-0x0817C2B8: two per-state member-function-pointer
 * tables (ACTOR_PMF_CALL, include/actor_self.h); the third, the airship
 * fireball's, follows in actor_pmf_17c2b8.cpp. Linked in ROM order
 * between data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* Dispatched by UpdateJetpackPlane and RunJetpackPlaneState
 * (jetpack_plane.c). */
const struct actor_pmf gJetpackPlaneStateFuncs[4] = {
    ACTOR_PMF(JetpackPlaneStateFly),
    ACTOR_PMF(JetpackPlaneStateFollow),
    ACTOR_PMF(JetpackPlaneStateKnockedOut),
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
