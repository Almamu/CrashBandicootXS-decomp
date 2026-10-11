#ifndef GUARD_CONSTANTS_ENTITY_PARAMS_H
#define GUARD_CONSTANTS_ENTITY_PARAMS_H

/*
 * The flags byte of a room entity's parameter record, byte 0 of
 * `params + paramOffsets[id]` (level_data.h). The code reads it through
 * three views: `struct entity_params.flags` (the spawners),
 * `struct crate_placement.flags` (Crate::Create, crate.hpp) and
 * `struct spawn_rec.flags` (Platform::Create, gobj_1a794.h). What a bit
 * means depends on the entity type (docs/flags/entity_params.md):
 *
 * - Bit 0: the bonus platform's exit facing (Platform::SetExitMirror);
 *   set on one bonus platform in the data.
 * - Bit 1: the entity faces the other way from its type's default. The
 *   start marker turns the player to face left, a basic crate is
 *   X-mirrored; an enemy, a boss or Dingodile's shield is X-mirrored
 *   (faces left, towards the player) unless it is set, so they read it
 *   inverted, `(flags >> ENTITY_PARAMS_BIT_FLIP_X ^ 1) & 1`. Set only on
 *   enemies in the data.
 * - Bit 2: Y-mirrored (basic crates, enemies, bosses). Never set in the
 *   data.
 * - Bits 3-4: nothing reads them and the data never sets them.
 * - Bits 5-7: a crate's placement flags, CRATE_PLACEMENT_FLAG_* (crate.hpp).
 *
 * `ENTITY_PARAMS_BIT_*` is the bit's number, for the code that shifts it
 * out (`TEST_BIT_SHR`, core.h), `ENTITY_PARAMS_FLAG_*` its mask.
 */

#define ENTITY_PARAMS_BIT_EXIT_MIRROR 0
#define ENTITY_PARAMS_BIT_FLIP_X 1
#define ENTITY_PARAMS_BIT_FLIP_Y 2
#define ENTITY_PARAMS_BIT_UNK3 3 /* unused: never read, never set in the data */
#define ENTITY_PARAMS_BIT_UNK4 4 /* unused: never read, never set in the data */

#define ENTITY_PARAMS_FLAG_EXIT_MIRROR (1 << ENTITY_PARAMS_BIT_EXIT_MIRROR)
#define ENTITY_PARAMS_FLAG_FLIP_X (1 << ENTITY_PARAMS_BIT_FLIP_X)
#define ENTITY_PARAMS_FLAG_FLIP_Y (1 << ENTITY_PARAMS_BIT_FLIP_Y)
#define ENTITY_PARAMS_FLAG_UNK3 (1 << ENTITY_PARAMS_BIT_UNK3)
#define ENTITY_PARAMS_FLAG_UNK4 (1 << ENTITY_PARAMS_BIT_UNK4)

#endif /* GUARD_CONSTANTS_ENTITY_PARAMS_H */
