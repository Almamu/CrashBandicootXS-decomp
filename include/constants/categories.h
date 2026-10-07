#ifndef GUARD_CONSTANTS_CATEGORIES_H
#define GUARD_CONSTANTS_CATEGORIES_H

/*
 * Actor categories: the index of gActorCategories, the `catIndex` of a
 * ROOM_KIND_CATEGORY stage and InitActorCategory's argument. Each is the
 * stage of one level (gLevelTable's room lists): the polar rides of
 * frostbite cavern, snow crash and snow job, the jetpack levels, and
 * n. gin's fight.
 */
#define CATEGORY_FROSTBITE_CAVERN 0
#define CATEGORY_SNOW_CRASH 1
#define CATEGORY_SNOW_JOB 2
#define CATEGORY_ROCKET_RACKET 3
#define CATEGORY_BLIMP_BONANZA 4
#define CATEGORY_NO_FLY_ZONE 5
#define CATEGORY_N_GIN 6
#define CATEGORY_COUNT 7

/*
 * `struct category_descriptor.type`: the gActorCategoryVtables index,
 * which picks the stage's player and boss.
 */
#define CATEGORY_TYPE_POLAR 0      /* the polar ride; the yeti (CreateYeti) */
#define CATEGORY_TYPE_JETPACK 1    /* the jetpack (CreateJetpackPlayer); the airship (CreateAirship) */
#define CATEGORY_TYPE_HOVERCRAFT 2 /* the jetpack; n. gin's hovercraft (CreateHovercraft) */
#define CATEGORY_TYPE_COUNT 3

/*
 * SetActorCategoryExitStatus values: what RunActorCategoryFrame returns to
 * InitActorCategory's loop at the end of the frame.
 */
#define CATEGORY_EXIT_NONE 0 /* keep playing */
/* The stage is finished (PolarPlayerStateFinishLeap,
 * JetpackPlayerStateFinish): leave the loop. */
#define CATEGORY_EXIT_CLEARED 1
/* Retry from the checkpoint, counting a death and a boss death
 * (gActorCategoryBossDeaths); a time trial counts neither
 * (PolarPlayerStateCarriedOff). */
#define CATEGORY_EXIT_BOSS_DEATH 2
/* Retry from the checkpoint, counting a death, and a boss death too
 * unless the type is CATEGORY_TYPE_POLAR; a time trial counts neither
 * (PolarPlayerStateKnockedOff, JetpackPlayerStateFall). */
#define CATEGORY_EXIT_DEATH 3

#endif /* GUARD_CONSTANTS_CATEGORIES_H */
