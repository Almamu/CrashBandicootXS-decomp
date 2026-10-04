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
extern void sub_8033048();
extern void sub_803395C();
extern void sub_80339DC();
extern void sub_8033BFC();
extern void sub_8033C28();
extern void sub_8033CF8();
extern void sub_8033F48();
extern void sub_8033F74();

/* Per-kind animation step functions of the singleton object, called
 * through _call_via_r0 as `gStaticData_0817C4C8[gUnknown_030015B0]` by
 * sub_8032B6C (actor_part130.c). */
void (*const gStaticData_0817C4C8[6])() = {
    nullsub_36,
    sub_803395C,
    sub_8032C0C,
    sub_8032EA0,
    nullsub_37,
    sub_8033048,
};

/* Per-state handlers dispatched by sub_8033B44 (actor_part31.c) and
 * sub_8033C84 (actor_part33.c). */
const struct actor_pmf gStaticData_0817C4E0[3] = {
    ACTOR_PMF(sub_8033C28),
    ACTOR_PMF(sub_80339DC),
    ACTOR_PMF(sub_8033BFC),
};

/* Per-state handlers dispatched by sub_8033E80 (actor_part37.c) and
 * sub_8033FE4 (actor_part64.c). */
const struct actor_pmf gStaticData_0817C4F8[3] = {
    ACTOR_PMF(sub_8033F74),
    ACTOR_PMF(sub_8033CF8),
    ACTOR_PMF(sub_8033F48),
};
