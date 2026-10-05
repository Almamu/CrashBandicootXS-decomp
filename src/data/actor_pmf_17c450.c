#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C450-0x0817C460. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void HovercraftFireballStateExplode();
extern void HovercraftFireballStateFly();

/* Per-state handlers dispatched by UpdateHovercraftFireball and RunHovercraftFireballState
 * (actor_part130.c). */
const struct actor_pmf gHovercraftFireballStateFuncs[2] = {
    ACTOR_PMF(HovercraftFireballStateFly),
    ACTOR_PMF(HovercraftFireballStateExplode),
};
