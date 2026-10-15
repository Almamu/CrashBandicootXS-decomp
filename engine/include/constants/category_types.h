#ifndef GUARD_CONSTANTS_CATEGORY_TYPES_H
#define GUARD_CONSTANTS_CATEGORY_TYPES_H

/*
 * `struct category_descriptor.type`: the ActorCategory::vtables index,
 * which picks the stage's player and boss. The categories themselves
 * (CATEGORY_*, CATEGORY_COUNT) are data: constants/categories.h, which
 * tools/levels.py generates from levels.json's "categories" and which
 * includes this header (docs/levels.md, "Actor categories").
 */
#define CATEGORY_TYPE_POLAR 0      /* the polar ride; the yeti (CreateYeti) */
#define CATEGORY_TYPE_JETPACK 1    /* the jetpack (JetpackPlayer::Create); the airship (CreateAirship) */
#define CATEGORY_TYPE_HOVERCRAFT 2 /* the jetpack; n. gin's hovercraft (CreateHovercraft) */
#define CATEGORY_TYPE_COUNT 3

/*
 * ActorCategory::SetExitStatus values: what ActorCategory::RunFrame returns to
 * ActorCategory::Init's loop at the end of the frame.
 */
#define CATEGORY_EXIT_NONE 0 /* keep playing */
/* The stage is finished (PolarPlayerStateFinishLeap,
 * JetpackPlayerStateFinish): leave the loop. */
#define CATEGORY_EXIT_CLEARED 1
/* Retry from the checkpoint, counting a death and a boss death
 * (ActorCategoryStats::bossDeaths); a time trial counts neither
 * (PolarPlayerStateCarriedOff). */
#define CATEGORY_EXIT_BOSS_DEATH 2
/* Retry from the checkpoint, counting a death, and a boss death too
 * unless the type is CATEGORY_TYPE_POLAR; a time trial counts neither
 * (PolarPlayerStateKnockedOff, JetpackPlayerStateFall). */
#define CATEGORY_EXIT_DEATH 3

#endif /* GUARD_CONSTANTS_CATEGORY_TYPES_H */
