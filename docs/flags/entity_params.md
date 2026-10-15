# A room entity's parameter flags (#843)

Each entity a room places has a parameter record, at `params +
paramOffsets[id]` in the room's entity list (level_data.h,
`gEntityFlags->list`). Its byte 0 is a flags byte, and the code reads it
through two views:

- `struct entity_params.flags` (level_data.h): the enemy, boss, basic-crate
  and start-marker spawners, and Platform::Create (`p.platform`, which was
  gobj_1a794.h's `struct spawn_rec`);
- `struct crate_placement.flags` (crate.hpp): Crate::Create.

What a bit means depends on the entity type. The names are in
engine/include/constants/entity_params.h (`ENTITY_PARAMS_*`), which level_data.h
includes, except for the crate placement bits, which stay in crate.hpp.
The counts below are from games/xs/data/levels/*/room.json.

| Bit | Name | Who reads it | Set in the data |
|---|---|---|---|
| 0 (0x01) | `ENTITY_PARAMS_FLAG_EXIT_MIRROR` | Platform::Create, on the bonus platform (`SetExitMirror(rec->flags & ...)`) | 1 bonus platform of 13 |
| 1 (0x02) | `ENTITY_PARAMS_FLAG_FLIP_X` | SpawnBasicCrate (X-mirrored), SpawnStartMarker (the player faces left); inverted by the enemy spawners (SetMirror), the boss spawners (SetMirrorFromRecord) and Dingodile's shield/rocket | only on enemies: 29 (blowgun tribesman, frog, moray eel, patrolling jungle enemy, shark, venus flytrap) |
| 2 (0x04) | `ENTITY_PARAMS_FLAG_FLIP_Y` | SpawnBasicCrate, the enemy and boss spawners, Dingodile's shield/rocket (Y-mirrored) | never |
| 3 (0x08) | `ENTITY_PARAMS_FLAG_UNK3` | nothing | never |
| 4 (0x10) | `ENTITY_PARAMS_FLAG_UNK4` | nothing | never |
| 5 (0x20) | `CRATE_PLACEMENT_FLAG_TRIAL_KIND` (crate.hpp) | Crate::Create: the crate has a time-trial kind | 66 crates |
| 6 (0x40) | `CRATE_PLACEMENT_FLAG_ASSIST_AKU_AKU`; on a checkpoint crate `CRATE_PLACEMENT_FLAG_CHECKPOINT_MIRROR_X` | Crate::Create | 6 "?" crates (5 with bit 5), 5 checkpoint crates |
| 7 (0x80) | `CRATE_PLACEMENT_FLAG_ASSIST_CHECKPOINT` | Crate::Create | 4 "?" crates |

`ENTITY_PARAMS_BIT_*` and `CRATE_PLACEMENT_BIT_*` are the bit numbers, for
the sites that shift the bit out (`TEST_BIT_SHR`, `TEST_BIT_SIGN`, core.h).

## Bit 1, the facing

The sprites' `mirrorX` (Sprite's MirrorFlags) set means facing left: the
swim controller sets it on a turn to the left, and a patrolling enemy with
it set turns back at the left end of its range (EnemyCtrl::UpdatePatrol).
The player and the crates start unmirrored, so bit 1 mirrors them
directly. An enemy faces left, towards the player, by default, so the
enemy and boss spawners store the inverse, `(flags >> ENTITY_PARAMS_BIT_FLIP_X
^ 1) & 1`: with bit 1 set, the enemy faces right. Either way the bit
turns the entity around from its type's default, hence `FLIP_X`.

Dingodile's SpawnShieldOrRocket reads the flags of the room's record 0,
not its own.

## The crate placement's options (byte 1)

| Bit | Name | Meaning |
|---|---|---|
| 0 (0x01) | `CRATE_PLACEMENT_OPTION_ASSIST_LIFE` | an assisted "?" or slot crate becomes a life crate (1 "?" crate) |
| 1 (0x02) | `CRATE_PLACEMENT_OPTION_SLOT_MYSTERY` | the slot crate may stop on its "?" face |
| 2 (0x04) | `CRATE_PLACEMENT_OPTION_SLOT_LIFE` | ... on its extra-life face |
| 3 (0x08) | `CRATE_PLACEMENT_OPTION_SLOT_TNT` | ... on its TNT face |
| 4-7 | | never set or read |

Every slot crate in the data has options 6, the "?" and life faces.

## Crate::paramA on a slot crate

Crate::Create turns the three options into a face mask in `paramA`:

| Mask | Name | Phase | Animation | OpenSlot |
|---|---|---|---|---|
| 1 | `CRATE_SLOT_FACE_MYSTERY` | 2 | 0xB | OpenMystery |
| 2 | `CRATE_SLOT_FACE_LIFE` | 1 | 9 | DropExtraLife |
| 4 | `CRATE_SLOT_FACE_TNT` | 3 | 0xD | Explode |

Crate::UpdateSlot skips a phase whose face bit is clear.

The other kinds use paramA as a plain value (crate.hpp): the checkpoint
crate's respawn mirror (below), the bouncy wumpa crate's "bouncing" flag,
and the iron switch's and outline crates' group id and step count. Those
stay unnamed.

## The respawn's X mirror

| Value | Name | Where |
|---|---|---|
| 1 | `CHECKPOINT_FLAG_MIRROR_X` (level_state.hpp) | `LevelProgress::checkpointFlags` |

A checkpoint crate's placement bit 6 goes into its `paramA`
(`TEST_BIT_SIGN(flags, CRATE_PLACEMENT_BIT_CHECKPOINT_MIRROR_X)`); breaking
it calls `SetCheckpointAtPlayer(paramA != 0)`. Coming back from a bonus
round, RunRoom calls SetCheckpoint with the bonus platform's GetExitMirror
(bit 0 above). Both keep the value in `checkpointFlags`, and PlayRoom
copies it into the player's `mirrorX`, so the player respawns facing left.
UpdateGameFrame clears it at the level's start. The code only stores and
copies the byte, so the name documents the value; no site spells it.
