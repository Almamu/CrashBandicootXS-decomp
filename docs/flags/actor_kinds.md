# The 3D stages' actor kinds (#843)

A 3D stage (an actor category, actor_anim.h) places its actors from a
spawn table. Each record has a `kind`, plus an `altKind` for time trials
and a `bonusKind`. The kind is also the index into the category family's
animation table, and that record's `index` holds the same number. The two
families number their kinds separately. The names are in
include/constants/actor_kinds.h, which actor_anim.h includes. They come
from the constructor each factory calls and from the record's frames
(src/game/vehicles/*/data/*_anim_family.cpp).

## Family 0: the polar runs (categories 0-2), `POLAR_KIND_*`

| Kind | Name | Built by (CreateActor) / evidence |
|---|---|---|
| 0 | `PLAYER` | record 0, PolarPlayer (ConstructAnimTableState). SpawnActor skips it as a spawn kind; it is a time-trial "nothing" |
| 1 | `BASIC_CRATE` | PolarBasicCrate. In a time trial, SpawnActor turns kinds 3, 8, 28-31 and 35 into it |
| 2 | `RIDERLESS_POLAR` | RiderlessPolar |
| 3 | `CHECKPOINT_CRATE` | PolarCheckpointCrate |
| 4 | `NITRO_CRATE` | PolarNitroCrate; DetonateNearby sets off the other kind-4 actors |
| 5-7 | `TIME_CRATE_1`..`_3` | PolarTimeCrate, freezes the clock 1-3 seconds |
| 8 | `LIFE_CRATE` | PolarLifeCrate (life crate frames); once collected, a kind-28 crate |
| 9 | `AKU_AKU_CRATE` | PolarAkuAkuCrate |
| 10 | `FOUR_WUMPA_CRATE` | PolarFourWumpaCrate |
| 11 | `WUMPA` | PolarWumpa; SpawnPolarCollectedWumpa's record. Skipped in a time trial |
| 12 | `BOOST_PAD` | PolarBoostPad |
| 13, 14 | `OBSTACLE`, `OBSTACLE_2` | PolarObstacle; kind 13 also builds its second part from record 14 |
| 15 | `UNK15` | no factory case, not in the data; the record's `index` is 0 |
| 16-21 | `ICICLE_A`, `_A_RIGHT`, `_B`, `_B_RIGHT`, `_C`, `_C_RIGHT` | PolarIcicle (16, 18, 20); on the right it uses the next record's animations |
| 22 | `LAUNCHER` | PolarLauncher |
| 23 | `ELECTRIC_FENCE` | PolarElectricFence |
| 24 | `PENGUIN` | PolarPenguin |
| 25, 26 | `GOAL`, `GOAL_2` | PolarGoal, and its second part |
| 27 | `AKU_AKU` | SpawnPolarAkuAku's record |
| 28-31 | `QUESTION_CRATE_WUMPA_1`, `_WUMPA_3`, `_WUMPA_5`, `_AKU_AKU` | PolarQuestionCrate: 1, 3 or 5 wumpa, or the mask |
| 32-34 | `PATH_SLOW`, `PATH_MEDIUM`, `PATH_FAST` | no actor: the points a penguin flies to, at gPolarPenguinSpeeds[kind - 0x20] (0x40, 0x62, 0x95) |
| 35 | `QUESTION_CRATE_LIFE` | PolarLifeCrate with "?" crate frames |
| 36-39 | `PALETTE_CYCLE_0`..`_3` | no actor: SetActorPaletteCycle(kind - 36) |
| 40 | `CHECKPOINT_TEXT` | CreatePolarCheckpointText's record |
| 62 | `UNK62` | SpawnActor skips it; no record, not in the data |

CountCategoryCrates counts kinds 1, 3, 4, 8, 9, 10, 28-31 and 35.

## Family 1: the jetpack and boss stages (categories 3-6), `JETPACK_KIND_*`

| Kind | Name | Built by (CreateJetpackActor, SpawnJetpackActor) / evidence |
|---|---|---|
| 0 | `PLAYER` | record 0; skipped as a spawn kind |
| 1 | `PLANE` | JetpackPlane |
| 2 | `SHOT` | SpawnJetpackShot's record |
| 3 | `CANNONBALL` | the JetpackCannonball spawner's record |
| 4-9 | `BOMBER_IDLE`, `_HOME`, `_BOB_VERTICAL`, `_SWING_HORIZONTAL`, `_CIRCLE`, `_DROP` | JetpackBomber; the kind picks its first `JETPACK_BOMBER_STATE_*` |
| 10 | `HOVERCRAFT` | Hovercraft::Spawn |
| 11-14 | `HOVERCRAFT_CANNON`, `_LAUNCHER`, `_SIDE_GUN`, `_CANNON_FLASH` | the hovercraft parts' spawners' records |
| 15 | `UNK15` | no factory case, not in the data; the record's `index` is 0 |
| 16-18 | `AIRSHIP_0`..`_2` | Airship::Spawn(kind - 16), the attack set |
| 19 | `HEALTH_CRATE` | JetpackHealthCrate |
| 20-23 | `QUESTION_CRATE_WUMPA_1`, `_WUMPA_3`, `_WUMPA_5`, `_LIFE` | JetpackQuestionCrate; a collected or time-trial 23 becomes 20 |
| 24-26 | `TIME_CRATE_1`..`_3` | JetpackTimeCrate |
| 27 | `PARACHUTE_NITRO` | JetpackParachuteNitro |
| 28 | `ROCKET` | JetpackRocket |
| 29 | `STOPWATCH` | JetpackTimeCrate that starts the time trial (not counted broken); spawned only once IsCrystalSaved |
| 30 | `UNK30` | rocket frames; CountCategoryCrates counts it, no factory case, not in the data |
| 31 | `RING_LEFT` | JetpackRing, with its right half (43); only kind 31 counts as passed |
| 32-37 | `PATH_0`..`_5` | no actor: the points a plane flies to, at gJetpackPlaneHopSpeeds[kind - 0x20] |
| 38, 39 | `AIRSHIP_FIREBALL`, `HOVERCRAFT_FIREBALL` | their spawners' records |
| 40-42 | `BALLOON_RED`, `_YELLOW`, `_RED_CROSS` | the balloon crates' balloons: time crate, question crate, health crate (named by how they look in their palette banks 5, 6 and 7) |
| 43 | `RING_RIGHT` | the ring's right half |
| 44-46 | `COLLECTED_WUMPA`, `EXPLOSION`, `CHECKPOINT_TEXT` | their spawners' records |
| 62 | `UNK62` | SpawnJetpackActor skips it; no record, not in the data |

CountCategoryCrates counts kinds 19-23, 27 and 30.

`ACTOR_KIND_PATH_POINT` (0x20) is both families' first path point;
GetActorSpawnKindIndex returns a kind less it.

## Also here: CountCrateEntities' room entity types

LevelEntityFlags::CountCrateEntities (entity_flags.cpp) switches on room
entity types, so its cases are the generated `ENTITY_*` names
(constants/entities.h). It counts every crate type from
`ENTITY_BASIC_CRATE` to `ENTITY_TIME_CRATE_3`, except the iron switch,
outline, nitro switch, iron and iron arrow crates. An outline crate counts
as the kind it turns into.

## The spawn tables

The seven spawn tables (`gCategory0SpawnTable`..`gCategory6SpawnTable`,
data/levels/spawns/*.json, generated into
src/game/vehicles/polar/data/polar_cell_anim.cpp, polar_spawn_tables.cpp, jetpack_cell_anim.cpp
and jetpack_sky_pictures.cpp; docs/levels.md "Actor categories") spell each spawn's `kind`, `altKind` and
`bonusKind` with their family's names: `POLAR_KIND_*` in categories 0-2,
`JETPACK_KIND_*` in 3-6 (without the prefix in the JSON). The polar runs start with a checkpoint crate,
the jetpack stages with the stopwatch, the hovercraft's table with the
hovercraft. A `PLAYER` (0) alternative means nothing spawns there in that mode.
The 4 bytes after the last spawn (`end` in `ACTOR_SPAWN_TABLE`) stay
numbers: they belong to no spawn.

HovercraftLauncher::StateLaunch's three CreateJetpackActor calls build
`BOMBER_HOME`, `BOMBER_BOB_VERTICAL` or `BOMBER_CIRCLE`, picked by a
`RandRange(3)` roll that stays a number.

## Left as numbers

- **PolarIcicle's `kind * 4 - 0x40`.** That is `(kind - POLAR_KIND_ICICLE_A) * 4`, spelled the ROM's way. It has a comment.
- **CountCrateEntities' outline lookup.** The `s16` eight bytes into the record is `crate_placement`'s `solidKind`, still read through a raw cast.
