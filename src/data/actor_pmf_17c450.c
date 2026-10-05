#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C450-0x0817C460. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void sub_8032A1C();
extern void sub_8032A24();

/* Per-state handlers dispatched by UpdateHovercraftFireball and RunHovercraftFireballState
 * (actor_part130.c). */
const struct actor_pmf gHovercraftFireballStateFuncs[2] = {
    ACTOR_PMF(sub_8032A24),
    ACTOR_PMF(sub_8032A1C),
};
