#include "core.h"
#include "actor_self.h"
#include "bosses.h"

/*
 * ROM 0x0817C4C8-0x0817C510. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

/* Per-kind animation step functions of the singleton object, called
 * through _call_via_r0 as `gHovercraftStateFuncs[gHovercraftState]` by
 * RunHovercraftState (hovercraft.c). */
void (*const gHovercraftStateFuncs[6])() = {
    HovercraftStateInactive,
    HovercraftStateApproach,
    HovercraftStateCloseIn,
    HovercraftStateFallBack,
    nullsub_37,
    HovercraftStateFall,
};

/* Per-state handlers dispatched by UpdateHovercraftCannon (hovercraft_cannon.c) and
 * RunHovercraftCannonState (hovercraft_cannon.c). */
const struct actor_pmf gHovercraftCannonStateFuncs[3] = {
    ACTOR_PMF(HovercraftCannonStateWait),
    ACTOR_PMF(HovercraftCannonStateFire),
    ACTOR_PMF(HovercraftCannonStateDestroyed),
};

/* Per-state handlers dispatched by UpdateHovercraftLauncher (hovercraft_launcher.c) and
 * RunHovercraftLauncherState (hovercraft_launcher.c). */
const struct actor_pmf gHovercraftLauncherStateFuncs[3] = {
    ACTOR_PMF(HovercraftLauncherStateWait),
    ACTOR_PMF(HovercraftLauncherStateLaunch),
    ACTOR_PMF(HovercraftLauncherStateDestroyed),
};
