#ifndef GUARD_BOX_PART_H
#define GUARD_BOX_PART_H

#include "aabb.h"
#include "gfx.h"

/* The sprite objects' flag and direction bits, as the C++ code tests
 * them through the bytes (entity.hpp's `union EntityFlags` and Sprite's
 * `dir`). The C view this header was named after, `struct box_part` (a
 * MovingSprite, include/sprite_obj.hpp), its `struct keyframe` records
 * and `struct part_list` (PartList) went with their last users (#656). */

/* The entity flags bytes at 0x0C/0x0D: actor.h's ENTITY_FLAG_* and
 * ENTITY_FLAG2_* (#843; PART_FLAG_GONE/PART_FLAG_TOUCHED were two of
 * them). */

/* Sprite's `dir` (the player's PLAYER_DIR_*, player.hpp, are the same
 * byte's): the direction
 * bits, 1 right, 2 left, 4 up, 8 down. The X pair is also the X probe
 * mode, the Y pair the Y probe mode (ProbeGroundSpriteTerrain). */
#define PART_DIR_X_MASK 3
#define PART_DIR_Y_MASK 0xc

#endif /* GUARD_BOX_PART_H */
