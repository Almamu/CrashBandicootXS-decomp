#ifndef GUARD_CONSTANTS_ACTOR_KINDS_H
#define GUARD_CONSTANTS_ACTOR_KINDS_H

/*
 * The actor kinds of the 3D stages (#843, docs/flags/actor_kinds.md): a
 * spawn record's `kind`/`altKind`/`bonusKind` (struct sub_effect_record,
 * actor_anim.h) and the index into the category family's animation table
 * (`gActorAnimTable` / `gJetpackAnimTable`), whose records hold the same
 * number in `index`. Each family has its own numbering: POLAR_KIND_* for
 * family 0 (categories 0-2, the polar runs, CreateActor and
 * gCategoryFamily0AnimTable) and JETPACK_KIND_* for family 1 (categories
 * 3-6, the jetpack and boss stages, CreateJetpackActor and
 * gCategoryFamily1AnimTable). The names come from each kind's
 * constructor and its record's frames.
 */

/* Both families' path points start here: no actor, a point a homing actor
 * (the polar penguin, the jetpack plane) flies to; GetActorSpawnKindIndex
 * returns the kind less this, the index into the speed tables. */
#define ACTOR_KIND_PATH_POINT 0x20

#define POLAR_KIND_PLAYER 0 /* record 0, the player (PolarPlayer); as a spawn kind, nothing */
#define POLAR_KIND_BASIC_CRATE 1
#define POLAR_KIND_RIDERLESS_POLAR 2
#define POLAR_KIND_CHECKPOINT_CRATE 3
#define POLAR_KIND_NITRO_CRATE 4
#define POLAR_KIND_TIME_CRATE_1 5 /* freezes the clock for 1 second */
#define POLAR_KIND_TIME_CRATE_2 6
#define POLAR_KIND_TIME_CRATE_3 7
#define POLAR_KIND_LIFE_CRATE 8
#define POLAR_KIND_AKU_AKU_CRATE 9
#define POLAR_KIND_FOUR_WUMPA_CRATE 10 /* looks like a basic crate, gives 4 wumpa */
#define POLAR_KIND_WUMPA 11            /* also the collected wumpa (SpawnPolarCollectedWumpa) */
#define POLAR_KIND_BOOST_PAD 12
#define POLAR_KIND_OBSTACLE 13
#define POLAR_KIND_OBSTACLE_2 14 /* the obstacle's second part, spawned with it */
#define POLAR_KIND_UNK15 15      /* never spawned: the record's index is 0, boost pad frames */
#define POLAR_KIND_ICICLE_A 16   /* PolarIcicle: kind and the next one are its left/right looks */
#define POLAR_KIND_ICICLE_A_RIGHT 17
#define POLAR_KIND_ICICLE_B 18
#define POLAR_KIND_ICICLE_B_RIGHT 19
#define POLAR_KIND_ICICLE_C 20
#define POLAR_KIND_ICICLE_C_RIGHT 21
#define POLAR_KIND_LAUNCHER 22
#define POLAR_KIND_ELECTRIC_FENCE 23
#define POLAR_KIND_PENGUIN 24
#define POLAR_KIND_GOAL 25
#define POLAR_KIND_GOAL_2 26 /* the goal's second part, spawned with it */
#define POLAR_KIND_AKU_AKU 27
#define POLAR_KIND_QUESTION_CRATE_WUMPA_1 28 /* also what a collected life crate becomes */
#define POLAR_KIND_QUESTION_CRATE_WUMPA_3 29
#define POLAR_KIND_QUESTION_CRATE_WUMPA_5 30
#define POLAR_KIND_QUESTION_CRATE_AKU_AKU 31
#define POLAR_KIND_PATH_SLOW (ACTOR_KIND_PATH_POINT + 0)   /* gPolarPenguinSpeeds 0x40 */
#define POLAR_KIND_PATH_MEDIUM (ACTOR_KIND_PATH_POINT + 1) /* 0x62 */
#define POLAR_KIND_PATH_FAST (ACTOR_KIND_PATH_POINT + 2)   /* 0x95 */
#define POLAR_KIND_QUESTION_CRATE_LIFE 35 /* a "?" crate (its frames) with an extra life */
#define POLAR_KIND_PALETTE_CYCLE_0 36     /* 36-39: no actor, SetActorPaletteCycle(kind - 36) */
#define POLAR_KIND_PALETTE_CYCLE_1 37
#define POLAR_KIND_PALETTE_CYCLE_2 38
#define POLAR_KIND_PALETTE_CYCLE_3 39
#define POLAR_KIND_CHECKPOINT_TEXT 40
#define POLAR_KIND_UNK62 62 /* SpawnActor skips it; no record, never in the data */

#define JETPACK_KIND_PLAYER 0 /* record 0, the player; as a spawn kind, nothing */
#define JETPACK_KIND_PLANE 1
#define JETPACK_KIND_SHOT 2
#define JETPACK_KIND_CANNONBALL 3
#define JETPACK_KIND_BOMBER_IDLE 4 /* 4-9: JetpackBomber, the kind picks its first state */
#define JETPACK_KIND_BOMBER_HOME 5
#define JETPACK_KIND_BOMBER_BOB_VERTICAL 6
#define JETPACK_KIND_BOMBER_SWING_HORIZONTAL 7
#define JETPACK_KIND_BOMBER_CIRCLE 8
#define JETPACK_KIND_BOMBER_DROP 9
#define JETPACK_KIND_HOVERCRAFT 10 /* Hovercraft::Spawn */
#define JETPACK_KIND_HOVERCRAFT_CANNON 11
#define JETPACK_KIND_HOVERCRAFT_LAUNCHER 12
#define JETPACK_KIND_HOVERCRAFT_SIDE_GUN 13
#define JETPACK_KIND_HOVERCRAFT_CANNON_FLASH 14
#define JETPACK_KIND_UNK15 15    /* never spawned: the record's index is 0 */
#define JETPACK_KIND_AIRSHIP_0 16 /* 16-18: Airship::Spawn(kind - 16), the attack set */
#define JETPACK_KIND_AIRSHIP_1 17
#define JETPACK_KIND_AIRSHIP_2 18
#define JETPACK_KIND_HEALTH_CRATE 19
#define JETPACK_KIND_QUESTION_CRATE_WUMPA_1 20 /* also a collected or time-trial life crate */
#define JETPACK_KIND_QUESTION_CRATE_WUMPA_3 21
#define JETPACK_KIND_QUESTION_CRATE_WUMPA_5 22
#define JETPACK_KIND_QUESTION_CRATE_LIFE 23
#define JETPACK_KIND_TIME_CRATE_1 24
#define JETPACK_KIND_TIME_CRATE_2 25
#define JETPACK_KIND_TIME_CRATE_3 26
#define JETPACK_KIND_PARACHUTE_NITRO 27
#define JETPACK_KIND_ROCKET 28
#define JETPACK_KIND_STOPWATCH 29 /* starts the time trial; only once the crystal is saved */
#define JETPACK_KIND_UNK30 30     /* rocket frames; CountCategoryCrates counts it, nothing spawns it */
#define JETPACK_KIND_RING_LEFT 31 /* the ring the player passes; its right half is 43 */
#define JETPACK_KIND_PATH_0 (ACTOR_KIND_PATH_POINT + 0) /* 0x20-0x25: gJetpackPlaneHopSpeeds */
#define JETPACK_KIND_PATH_1 (ACTOR_KIND_PATH_POINT + 1)
#define JETPACK_KIND_PATH_2 (ACTOR_KIND_PATH_POINT + 2)
#define JETPACK_KIND_PATH_3 (ACTOR_KIND_PATH_POINT + 3)
#define JETPACK_KIND_PATH_4 (ACTOR_KIND_PATH_POINT + 4)
#define JETPACK_KIND_PATH_5 (ACTOR_KIND_PATH_POINT + 5)
#define JETPACK_KIND_AIRSHIP_FIREBALL 38
#define JETPACK_KIND_HOVERCRAFT_FIREBALL 39
#define JETPACK_KIND_BALLOON_RED_YELLOW 40 /* the balloon crates' balloons */
#define JETPACK_KIND_BALLOON_ORANGE_BLUE 41
#define JETPACK_KIND_BALLOON_YELLOW_BLUE 42
#define JETPACK_KIND_RING_RIGHT 43
#define JETPACK_KIND_COLLECTED_WUMPA 44
#define JETPACK_KIND_EXPLOSION 45
#define JETPACK_KIND_CHECKPOINT_TEXT 46
#define JETPACK_KIND_UNK62 62 /* SpawnJetpackActor skips it; no record, never in the data */

#endif /* GUARD_CONSTANTS_ACTOR_KINDS_H */
