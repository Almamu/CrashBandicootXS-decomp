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

/* The direction bits (#843, docs/flags/direction.md): Sprite's `dir`
 * (the motion direction, ApplyVelocity; also the player's and the
 * camera's target's), MovingSprite's `hitAxes` and `hitMask` (the
 * terrain probes' hits: a probe in a direction that hits adds that
 * direction's bit, so `hitAxes & PART_DIR_DOWN` is "on the floor" and
 * `hitAxes & PART_DIR_UP` "hit the ceiling"), and EVENT_BUMP's side.
 * The X pair is also the X probe mode, the Y pair the Y probe mode
 * (ProbeTerrainAxes). */
#define PART_DIR_RIGHT 1
#define PART_DIR_LEFT 2
#define PART_DIR_UP 4
#define PART_DIR_DOWN 8
#define PART_DIR_X_MASK 3   // PART_DIR_RIGHT | PART_DIR_LEFT
#define PART_DIR_Y_MASK 0xc // PART_DIR_UP | PART_DIR_DOWN

#endif /* GUARD_BOX_PART_H */
