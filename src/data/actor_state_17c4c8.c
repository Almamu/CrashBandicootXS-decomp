#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C4C8-0x0817C510. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_36();
extern void nullsub_37();
extern void sub_8032C0C();
extern void sub_8032EA0();
extern void HovercraftStateFall();
extern void HovercraftStateApproach();
extern void HovercraftCannonStateFire();
extern void HovercraftCannonStateDestroyed();
extern void HovercraftCannonStateWait();
extern void HovercraftLauncherStateLaunch();
extern void HovercraftLauncherStateDestroyed();
extern void HovercraftLauncherStateWait();

/* Per-kind animation step functions of the singleton object, called
 * through _call_via_r0 as `gHovercraftStateFuncs[gHovercraftState]` by
 * RunHovercraftState (actor_part130.c). */
void (*const gHovercraftStateFuncs[6])() = {
    nullsub_36,
    HovercraftStateApproach,
    sub_8032C0C,
    sub_8032EA0,
    nullsub_37,
    HovercraftStateFall,
};

/* Per-state handlers dispatched by UpdateHovercraftCannon (actor_part31.c) and
 * RunHovercraftCannonState (actor_part33.c). */
const struct actor_pmf gHovercraftCannonStateFuncs[3] = {
    ACTOR_PMF(HovercraftCannonStateWait),
    ACTOR_PMF(HovercraftCannonStateFire),
    ACTOR_PMF(HovercraftCannonStateDestroyed),
};

/* Per-state handlers dispatched by UpdateHovercraftLauncher (actor_part37.c) and
 * RunHovercraftLauncherState (actor_part64.c). */
const struct actor_pmf gHovercraftLauncherStateFuncs[3] = {
    ACTOR_PMF(HovercraftLauncherStateWait),
    ACTOR_PMF(HovercraftLauncherStateLaunch),
    ACTOR_PMF(HovercraftLauncherStateDestroyed),
};
