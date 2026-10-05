#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C1C0-0x0817C200. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void JetpackPlayerStateFly();
extern void JetpackPlayerStateRollLeft();
extern void JetpackPlayerStateRollRight();
extern void JetpackPlayerStateResume();
extern void JetpackPlayerStateBoost();
extern void JetpackPlayerStateFall();
extern void JetpackPlayerStateFinish();
extern void JetpackPlayerStateEnter();

/* Per-state handlers dispatched by UpdateJetpackPlayer (jetpack_spawn.c) and
 * RunJetpackPlayerState (jetpack_player.c). */
const struct actor_pmf gJetpackPlayerStateFuncs[8] = {
    ACTOR_PMF(JetpackPlayerStateEnter),
    ACTOR_PMF(JetpackPlayerStateFly),
    ACTOR_PMF(JetpackPlayerStateRollLeft),
    ACTOR_PMF(JetpackPlayerStateRollRight),
    ACTOR_PMF(JetpackPlayerStateFall),
    ACTOR_PMF(JetpackPlayerStateFinish),
    ACTOR_PMF(JetpackPlayerStateBoost),
    ACTOR_PMF(JetpackPlayerStateResume),
};
