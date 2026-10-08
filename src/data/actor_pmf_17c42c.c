#include "core.h"
#include "actor_self.h"
#include "vehicle.h"

/*
 * ROM 0x0817C42C-0x0817C444, split from actor_state_17c3fc.c (#664 part
 * 11f, which moved the balloon's table before it to C++). Linked in ROM
 * order between data/data.s sections by ldscript.txt - see docs/data.md.
 */

/* Per-state handlers dispatched by UpdateJetpackBalloonCrate and RunJetpackBalloonCrateState
 * (jetpack_crates.c). */
const struct actor_pmf gJetpackBalloonCrateStateFuncs[3] = {
    ACTOR_PMF(JetpackBalloonCrateStateHang),
    ACTOR_PMF(JetpackBalloonCrateStateFall),
    ACTOR_PMF(JetpackBalloonCrateStateDestroyed),
};
