#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C3FC-0x0817C444. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_31();
extern void nullsub_32();
extern void nullsub_33();
extern void AirshipStateApproach();
extern void AirshipStateFireballs();
extern void AirshipStateCannon();
extern void AirshipStateExplode();
extern void AirshipStateFall();
extern void sub_8031954();
extern void sub_80319A0();
extern void sub_8032274();
extern void sub_8032290();

/* Per-state step functions of the weapon-kind tracker, called through
 * _call_via_r0 as `gAirshipStateFuncs[gAirshipState]` by
 * UpdateAirship (actor_part23f.c). */
void (*const gAirshipStateFuncs[6])() = {
    nullsub_31,
    AirshipStateApproach,
    AirshipStateFireballs,
    AirshipStateCannon,
    AirshipStateExplode,
    AirshipStateFall,
};

/* Per-state handlers dispatched by sub_8031A08 (actor_part125.c). */
const struct actor_pmf gStaticData_0817C414[3] = {
    ACTOR_PMF(nullsub_32),
    ACTOR_PMF(sub_80319A0),
    ACTOR_PMF(sub_8031954),
};

/* Per-state handlers dispatched by UpdateJetpackBalloonCrate and sub_80322F4
 * (actor_part129.c). */
const struct actor_pmf gStaticData_0817C42C[3] = {
    ACTOR_PMF(sub_8032290),
    ACTOR_PMF(sub_8032274),
    ACTOR_PMF(nullsub_33),
};
