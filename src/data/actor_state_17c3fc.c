#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C3FC-0x0817C444. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void AirshipStateInactive();
extern void nullsub_32();
extern void nullsub_33();
extern void AirshipStateApproach();
extern void AirshipStateFireballs();
extern void AirshipStateCannon();
extern void AirshipStateExplode();
extern void AirshipStateFall();
extern void JetpackBalloonStatePop();
extern void JetpackBalloonStateFloatAway();
extern void JetpackBalloonCrateStateFall();
extern void JetpackBalloonCrateStateHang();

/* Per-state step functions of the weapon-kind tracker, called through
 * _call_via_r0 as `gAirshipStateFuncs[gAirshipState]` by
 * UpdateAirship (airship.c). */
void (*const gAirshipStateFuncs[6])() = {
    AirshipStateInactive,
    AirshipStateApproach,
    AirshipStateFireballs,
    AirshipStateCannon,
    AirshipStateExplode,
    AirshipStateFall,
};

/* Per-state handlers dispatched by RunJetpackBalloonState (actor_part125.c). */
const struct actor_pmf gJetpackBalloonStateFuncs[3] = {
    ACTOR_PMF(nullsub_32),
    ACTOR_PMF(JetpackBalloonStateFloatAway),
    ACTOR_PMF(JetpackBalloonStatePop),
};

/* Per-state handlers dispatched by UpdateJetpackBalloonCrate and RunJetpackBalloonCrateState
 * (actor_part129.c). */
const struct actor_pmf gJetpackBalloonCrateStateFuncs[3] = {
    ACTOR_PMF(JetpackBalloonCrateStateHang),
    ACTOR_PMF(JetpackBalloonCrateStateFall),
    ACTOR_PMF(nullsub_33),
};
