#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C1C0-0x0817C200. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void sub_802EDBC();
extern void sub_802EED0();
extern void sub_802EFD8();
extern void sub_802F570();
extern void sub_802F5AC();
extern void sub_802F5E4();
extern void sub_802F640();
extern void sub_802F69C();

/* Per-state handlers dispatched by sub_802E84C (actor_part128.c) and
 * sub_802F748 (actor_part44b.c). */
const struct actor_pmf gStaticData_0817C1C0[8] = {
    ACTOR_PMF(sub_802F69C),
    ACTOR_PMF(sub_802EDBC),
    ACTOR_PMF(sub_802EED0),
    ACTOR_PMF(sub_802EFD8),
    ACTOR_PMF(sub_802F5E4),
    ACTOR_PMF(sub_802F640),
    ACTOR_PMF(sub_802F5AC),
    ACTOR_PMF(sub_802F570),
};
