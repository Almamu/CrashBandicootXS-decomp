#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817A6B8-0x0817A728. Linked in ROM order between data/data.s
 * sections by ldscript.txt - see docs/data.md.
 */

extern void sub_802B8E8();
extern void sub_802B990();
extern void sub_802BA5C();
extern void sub_802BAD0();
extern void sub_802BB4C();
extern void sub_802BBE4();
extern void sub_802BD24();
extern void sub_802BD64();
extern void sub_802BDD0();
extern void sub_802BE34();
extern void sub_802BE80();
extern void sub_802BED8();
extern void sub_802BF30();
extern void sub_802BFA0();

/* Per-state handlers of the actor object dispatched by sub_802B364
 * (actor_part127.c) and sub_802C208 (actor_part19e.c). */
const struct actor_pmf gStaticData_0817A6B8[14] = {
    ACTOR_PMF(sub_802B8E8),
    ACTOR_PMF(sub_802B990),
    ACTOR_PMF(sub_802BAD0),
    ACTOR_PMF(sub_802BE80),
    ACTOR_PMF(sub_802BA5C),
    ACTOR_PMF(sub_802BED8),
    ACTOR_PMF(sub_802BE34),
    ACTOR_PMF(sub_802BBE4),
    ACTOR_PMF(sub_802BDD0),
    ACTOR_PMF(sub_802BFA0),
    ACTOR_PMF(sub_802BF30),
    ACTOR_PMF(sub_802BD64),
    ACTOR_PMF(sub_802BB4C),
    ACTOR_PMF(sub_802BD24),
};
