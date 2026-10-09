#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/* The objects subsystem (src/objects/): the base level objects (sprite
 * objects, moving and ground sprites, platforms and their movers, the
 * controllers), their animation, the part lists and the collision queue.
 * Every function src/objects/ defines, with the prototype of its
 * definition, and the globals and tables its files use
 * (docs/headers_plan.md). InitCrateList (crate_list_update.cpp) is in crates.h. A
 * .c file that needs a different local declaration for codegen keeps it
 * as an asm-label alias with a `codegen:` comment.
 *
 * The objects' structs live in their type headers (box_part.h,
 * gfx_part.h, gobj_1a794.h, ...); this header only declares their tags. */

#include "core.h"
#include "aabb.h"
#include "byte_arg.h"
#include "vtable.h"
#include "constants/events.h"

struct vec2;
struct hitbox_quad;
struct sprite_point;

/*
 * One queued collision of the player with a crate, 0x24 bytes:
 * AddCollisionCandidate (QueueCratePlayerCollision) appends it, and
 * ResolveCollisionCandidates hands it to ApplyCrateCollision, whose
 * parameter names the fields take. The two views of collision_queue.cpp,
 * `struct candidate` and `struct collision_candidate`, were merged here
 * (`unk_10`..`unk_1C`/`field10`..`field1c` are `code`/`edge`/`depth`/`hit`,
 * `unk_20`/`field20` and `unk_21`/`field21` are `p20`/`p21`; #574,
 * batch 9e).
 */
struct collision_candidate {
    struct Crate *neighbor; // 0x00 - the crate (class Crate, crate.hpp)
    struct vec2 pos;        // 0x04
    s32 kind;               // 0x0C - ATTACK_KIND_* (constants/attack_kinds.h)
    s32 code;               // 0x10
    s32 edge;               // 0x14
    s32 depth;              // 0x18
    s32 hit;                // 0x1C
    struct byte_arg p20;    // 0x20 - passed on the stack as a byte (`strb`)
    struct byte_arg p21;    // 0x21
};

/* An entry set: the {a, b} index pairs into a motion record table (two
 * records per state; `entries` is an array of pairs) and a Q8 scale,
 * 0x100 (1.0) in every set in the ROM, that SetCtrlTargetMotionX and
 * StartCtrlTargetMotionX multiply the motion vector by. A controller or
 * mover keeps one at +0x04 (SetCtrlAnimSet, Ctrl's `animSet`). */
struct entry_set {
    const u32 (*entries)[2];
    s32 scale; // 0x04 - Q8
};

/* A sprite object's per-axis speed ramp (MovingSprite's rampX/rampY): each frame ApplySpriteVelocity steps speedX/speedY by `step`
 * toward `target` without overshooting. The Start...MotionX/Y setters also
 * load `start` into the speed; the Set... ones keep the current speed.
 *
 * It is also the 12-byte motion record those setters copy one axis from
 * (gCtrlMotionRecords, the player's and the bosses' motion records,
 * gPlatformMoverMotionRecords): the `{a, b}` index pairs of an entry set
 * pick the X and the Y record of a state. objects.h's `struct motion_rec`
 * (`a`/`b`/`c`) and gobj_1a794.h's `struct vec3` (`x`/`y`/`z`) were
 * views of it (#574, batch 9e). */
struct speed_ramp {
    s32 start;
    s32 step;
    s32 target;
};

/* src/objects/ctrl.cpp: Ctrl's methods (include/ctrl.hpp)
 * under their C names (cxx_symbols.txt), for the C callers. */

/* src/objects/moving_sprite_collide.cpp */

/* src/objects/sprite.cpp */
extern void *GetSpriteAttackBox(void *dest, void *pt);
extern void *GetSpriteBodyBox(void *dest, void *pt);

/* src/objects/sprite_obj.cpp */

/* src/objects/sprite_obj.cpp */
extern void OffsetFromHitboxEdge(void *dest, s32 kind, void *rec);
extern void OffsetToHitboxEdge(void *dest, s32 kind, void *rec);
extern void OffsetToHitboxEdgeStart(void *dest, s32 kind, void *rec);

/* The controllers' motion records (src/data/motion_records_16b304.c),
 * read by StartCtrlTargetMotionYFromSet/StartCtrlTargetMotionXFromSet and
 * ApplyActionCtrlMotion. player.h has the player's two tables. */
extern const struct speed_ramp gCtrlMotionRecords[44];

/* The platform mover's entry set (src/data/entry_set_16c418.c), whose
 * entries are gDingodileMotionEntries[4..7] (bosses.h), and its motion
 * records (src/data/velocity_16c460.c). */
extern const struct entry_set gPlatformMoverMotionSet;
extern const struct speed_ramp gPlatformMoverMotionRecords[3];

/* The empty box GetSpriteFrameBodyBox and friends return for a frame
 * without one (src/data/obj_sizes_16b2e0.c). */
extern const struct hitbox_quad gEmptySpriteBox;
/* The anchor point GetSpriteFrameAnchor returns for a frame without one
 * (same file): {0, 0}. */
extern const struct sprite_point gEmptySpritePoint;

/* sym_iwram.txt */
extern s32 gLastSpriteVelY;

#endif /* __OBJECTS_H__ */
