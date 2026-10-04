#include "core.h"
#include "actor_self.h"

/*
 * ROM 0x0817C260-0x0817C2D0: three per-state member-function-pointer
 * tables (ACTOR_PMF_CALL, include/actor_self.h). Linked in ROM order
 * between data/data.s sections by ldscript.txt - see docs/data.md.
 */

extern void nullsub_28();
extern void nullsub_29();
extern void sub_802FE04();
extern void sub_802FE1C();
extern void sub_802FE58();
extern void sub_802FE78();
extern void sub_80300D8();
extern void sub_80300E0();
extern void sub_803013C();
extern void sub_8030188();
extern void sub_80301CC();
extern void sub_8030334();
extern void sub_803044C();
extern void sub_8030640();

/* Dispatched by UpdateJetpackPlane (actor_part46b.c) and sub_802FEA4
 * (actor_part_2fbf0.c). */
const struct actor_pmf gStaticData_0817C260[4] = {
    ACTOR_PMF(sub_802FE78),
    ACTOR_PMF(sub_802FE58),
    ACTOR_PMF(sub_802FE1C),
    ACTOR_PMF(sub_802FE04),
};

/* Dispatched by UpdateJetpackBomber and sub_8030234 (actor_part_2fbf0.c). */
const struct actor_pmf gStaticData_0817C280[7] = {
    ACTOR_PMF(nullsub_29),
    ACTOR_PMF(sub_80301CC),
    ACTOR_PMF(sub_8030188),
    ACTOR_PMF(sub_803013C),
    ACTOR_PMF(sub_80300E0),
    ACTOR_PMF(sub_80300D8),
    ACTOR_PMF(nullsub_28),
};

/* Dispatched by sub_8030574 (actor_part20b.c) and sub_8030648
 * (actor_part21b.c). */
const struct actor_pmf gStaticData_0817C2B8[3] = {
    ACTOR_PMF(sub_8030334),
    ACTOR_PMF(sub_803044C),
    ACTOR_PMF(sub_8030640),
};
