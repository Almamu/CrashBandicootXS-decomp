#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C1C0-0x0817C200. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void JetpackPlayerStateFly();
extern void JetpackPlayerStateRollLeft();
extern void JetpackPlayerStateRollRight();
extern void sub_802F570();
extern void sub_802F5AC();
extern void sub_802F5E4();
extern void sub_802F640();
extern void sub_802F69C();

/* Per-state handlers dispatched by UpdateJetpackPlayer (actor_part128.c) and
 * sub_802F748 (actor_part44b.c). */
const struct actor_pmf gJetpackPlayerStateFuncs[8] = {
    ACTOR_PMF(sub_802F69C),
    ACTOR_PMF(JetpackPlayerStateFly),
    ACTOR_PMF(JetpackPlayerStateRollLeft),
    ACTOR_PMF(JetpackPlayerStateRollRight),
    ACTOR_PMF(sub_802F5E4),
    ACTOR_PMF(sub_802F640),
    ACTOR_PMF(sub_802F5AC),
    ACTOR_PMF(sub_802F570),
};
