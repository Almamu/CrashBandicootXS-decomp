#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/* The objects subsystem (src/objects/): the base level objects (sprite
 * objects, moving and ground sprites, platforms and their movers, the
 * controllers), their animation, the part lists and the collision queue.
 * Every function src/objects/ defines, with the prototype of its
 * definition, and the globals and tables its files use
 * (docs/headers_plan.md). InitCrateList (part_list.cpp) is in crates.h. A
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

struct actor;
struct box_part;
struct collect_part;
struct crate;
struct part_list;
struct gfx_part;
struct gfx_vec;
struct gobj;
struct hitbox_quad;
struct mover;
struct sprite_point;

/* A position pair: a collision candidate's (src/objects/collision_queue.cpp),
 * which ApplyCrateCollision takes by value. Copied as one 8-byte struct
 * (the ROM's paired `ldr; ldr; str; str`). */
struct e08c_pos {
    s32 x;
    s32 y;
};

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
    struct crate *neighbor; // 0x00 - the crate; its position is its first two words
    struct e08c_pos pos;    // 0x04
    s32 kind;               // 0x0C - ATTACK_KIND_* (constants/attack_kinds.h)
    s32 code;               // 0x10
    s32 edge;               // 0x14
    s32 depth;              // 0x18
    s32 hit;                // 0x1C
    struct byte_arg p20;    // 0x20 - passed on the stack as a byte (`strb`)
    struct byte_arg p21;    // 0x21
    u8 unk_22[2];
};

/*
 * The player's collision queue (`struct player.collisionQueue`, +0x108):
 * the crate collisions found during the frame, resolved once a frame by
 * ResolvePlayerCollisions. The player object is 0x350 bytes, so the queue
 * holds 16 candidates. collision_queue.cpp's `struct candidate_list` and
 * `struct collision_queue` and player_event.c's `struct ab9c_link` (the
 * head) were views of it (#574, batch 9e). It is the C view of
 * CollisionQueue (include/part_list.hpp), which checks the size.
 */
struct collision_queue {
    s32 count; // 0x00
    // 0x04 - ResetCollisionQueue clears it, QueueCratePlayerCollision (crate_break.cpp) sets
    // it, and while it is set ApplyCrateCollision leaves the player's
    // position alone
    u8 posCommitted;
    u8 unk_05[3];
    struct collision_candidate candidates[16]; // 0x08
};

/* An entry set: the {a, b} index pairs into a motion record table (two
 * records per state; `entries` is an array of pairs) and a Q8 scale,
 * 0x100 (1.0) in every set in the ROM, that SetCtrlTargetMotionX and
 * StartCtrlTargetMotionX multiply the motion vector by. A controller or
 * mover keeps one at +0x04 (SetCtrlAnimSet, gobj_1a794.h's `struct mover`
 * `set`). */
struct entry_set {
    const u32 (*entries)[2];
    s32 scale; // 0x04 - Q8
};

/* The controllers' base class: the C view of include/ctrl.hpp's C++
 * class Ctrl (src/objects/ctrl.cpp, method table gCtrlVtable;
 * InitCtrl/DestroyCtrl), for the C files. The two must keep the same
 * layout: ctrl.hpp checks the class's size against this struct's. Every
 * controller extends it: the
 * action controller (action_ctrl.hpp's class ActionCtrl), the swim and input
 * controllers (player_ctrl.h, player.h), the boss controllers (player.h's
 * `struct boss_ctrl`), the enemy controller (part_ctrl.h) and the effect
 * controller (effect_ctrl.cpp, which uses
 * include/ctrl.hpp's C++ classes). The knocked enemy controller
 * (CreateKnockedEnemyCtrl), the stomped hop pad and the one-shot
 * animation controllers are this base alone (OperatorNew(0x10)).
 *
 * The base's attach method (AttachCtrl, slot 3) stores the sprite object
 * it's attached to at +0x00; AttachSpriteCtrl passes the object. The
 * subclasses that override it keep their controlled part at +0x10
 * instead, which isn't part of the base. */
struct ctrl {
    void *owner;                      // 0x00 - the attached sprite object (AttachCtrl)
    const struct entry_set *animSet;  // 0x04 - SetCtrlAnimSet
    s32 state;                        // 0x08 - GetCtrlMode/SetCtrlMode
    const struct vtable_slot *vtable; // 0x0C - gCtrlVtable or a subclass's
};

COMPILE_TIME_ASSERT(objects_h, sizeof(struct ctrl) == 0x10);

/* A sprite object's per-axis speed ramp (struct gobj.rampX/rampY, struct
 * player's): each frame ApplySpriteVelocity steps speedX/speedY by `step`
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

/* src/objects/collision_queue.cpp */
extern void ResolveCollisionCandidates(struct collision_queue *self);
extern void AddCollisionCandidate(struct collision_queue *self, struct crate *neighbor, s32 kind,
                                  s32 code, s32 edge, s32 depth, struct e08c_pos pos, s32 hit,
                                  s32 p20, s32 p21);
extern void DestroyCollisionQueue(struct collision_queue *self, s32 flags);
extern void ResetCollisionQueue(struct collision_queue *self);

/* src/objects/ctrl.cpp: Ctrl's methods (include/ctrl.hpp)
 * under their C names (cxx_symbols.txt), for the vtables and the C
 * callers. */
extern void StartCtrlTargetMotionYFromSet(void *self, void *part, s32 index);
extern void SetCtrlTargetMotionX(void *self, void *part, s32 *vec);
extern void StartCtrlTargetMotionX(void *self, void *part, s32 *vec);
extern void StartCtrlTargetMotionXFromSet(void *self, void *part, s32 index);
extern void CtrlHandleEvent(void);
extern u8 SetCtrlTargetAnim(void *unused, void *part, s32 newVal);
extern void AttachCtrl(void *self, s32 val);
extern void DestroyCtrl(void *self, s32 flags);
extern void InitCtrl(void *self);
extern s32 GetCtrlMode(void *self);

/* src/objects/effect_ctrl.cpp: EffectCtrl's methods (include/ctrl.hpp) under
 * their C names (cxx_symbols.txt), for gEffectCtrlVtable and the C
 * callers. */
extern void UpdateEffectCtrl(void *self, void *part);
extern void EffectCtrlHandleEvent(void *self, void *sender, s32 event, s32 arg);
extern void ResetEffectCtrl(void *self);
extern void DestroyEffectCtrl(void *self, s32 flags);
extern void *InitEffectCtrl(void *self);

/* src/objects/ground_sprite.cpp */
extern void DrawGroundSprite(void *self);
extern s32 GetGroundSpriteClassId(void);
extern void *CreateGroundSprite(u16 id, u16 x, u16 y, u16 unused);
extern void DestroyGroundSprite(struct actor *self, u32 unused);
extern void ResetGroundSprite(void *self);
extern struct actor *InitGroundSprite(struct actor *self);
extern u8 IsGroundSpriteGrounded(void *self);
extern void ClearGroundSpriteGrounded(void *self);
extern void SetGroundSpriteGrounded(void *self);
extern u8 IsGroundSpriteFloorProbeEnabled(void *self);
extern void DisableGroundSpriteFloorProbe(void *self);
extern void EnableGroundSpriteFloorProbe(void *self);
extern void ClearSpriteObjFlag5(void *self);
extern void SetSpriteObjFlag5(void *self);
extern u8 GetSpriteObjFlag5(void *self);
extern s32 GetMovingSpriteCtrl(void *self);

/* src/objects/ground_sprite_collide.cpp */
extern u8 CollideGroundSprite(struct box_part *self);
extern s32 ProbeGroundSpriteTerrain(struct box_part *self);
extern u8 ProbeGroundSpriteFloor(struct box_part *self, struct hitbox_quad *quad, u8 *outFlag);

/* src/objects/ground_sprite_update.cpp */
extern void UpdateGroundSprite(struct gobj *self);
extern void AnchorGroundSpriteHitbox(struct gobj *self);

/* src/objects/moving_sprite.cpp */
extern s32 ApplySpriteVelocity(void *self);
extern void SetSpritePrevPos(struct gfx_part *self, s32 x, s32 y);
extern void GetSpritePrevPos(struct gfx_vec *dest, struct gfx_part *self);
extern s32 GetSpritePrevY(struct gfx_part *self);
extern s32 GetSpritePrevX(struct gfx_part *self);
extern s32 GetMovingSpriteClassId(void);
extern void *CreateMovingSprite(u16 id, u16 x, u16 y, u16 unused);
extern void DestroyMovingSprite(struct actor *self, u32 flags);
extern void ResetMovingSprite(void *self);
extern struct actor *InitMovingSprite(struct actor *part);
extern void UpdateMovingSprite(struct actor *self);

/* src/objects/moving_sprite_collide.cpp */
extern void HitMovingSprite(struct gobj *self, s32 a, s32 b, s32 c);
extern s32 ClassifySpriteContact(void *part, void *region);
extern s32 CollideMovingSprite(struct gobj *self);
extern s32 GetGroundSpriteHitMask(struct gobj *self);
extern s32 HasGroundSpriteHitMask(struct gobj *self);
extern void ClearGroundSpriteHitMask(struct gobj *self);
extern void AddGroundSpriteHitMask(struct gobj *self, s32 val);
extern void SetGroundSpriteHitAxes(struct gobj *self, u8 val);
extern u8 GetGroundSpriteHitAxes(struct gobj *self);
extern void SetSpriteSpeedY(struct gobj *self, s32 val);
extern void SetSpriteSpeedX(struct gobj *self, s32 val);
extern s32 GetSpriteSpeedX(struct gobj *self);
extern s32 GetSpriteSpeedY(struct gobj *self);
extern struct mover *GetSpriteCtrl(struct gobj *self);
extern void AttachSpriteCtrl(struct gobj *self, struct mover *rec);
extern void StartSpriteMotionY(struct gobj *self, s32 a, s32 b, s32 c);
extern void SetSpriteMotionY(struct gobj *self, s32 a, s32 b, s32 c);
extern void StartSpriteMotionX(struct gobj *self, s32 a, s32 b, s32 c);
extern void SetSpriteMotionX(struct gobj *self, s32 a, s32 b, s32 c);
extern u8 GetGroundSpriteProbeTries(struct gobj *self);

/* src/objects/part_collide.cpp */
extern void CollidePartWithObject(struct part_list *list, struct aabb box, struct box_part *part,
                                  struct box_part *other);

/* src/objects/part_list.cpp */
extern void DrawPartList(struct part_list *manager);
extern void RemoveFromPartList(struct part_list *manager, void *target);
extern void RemovePartListAt(struct part_list *manager, s32 index);
extern void AddToPartList(struct part_list *manager, void *value);
extern void DestroyPartList(struct part_list *manager, s32 flags);
extern struct part_list *InitPartList(struct part_list *manager, s32 count);

/* src/objects/part_list_cull.cpp */
extern void CullPartList(struct part_list *manager);
extern void ClearPartList(struct part_list *manager);
extern void CollidePartsOfClass(struct part_list *manager, s32 classId);

/* src/objects/platform.c */
extern void UpdatePlatform(struct gobj *self);
extern s32 GetPlatformExitMirror(struct gobj *self);
extern void SetPlatformExitMirror(struct gobj *self, u8 value);
extern s32 GetPlatformClassId(void);
extern void DestroyPlatform(struct gobj *self, s32 flags);
extern void ClearPlatformVulnerable(struct gobj *self);
extern struct gobj *InitPlatform(struct gobj *self);
extern void UpdatePlatformMover(struct mover *self, struct gobj *obj);
extern void MovePlayerWithPlatform(struct mover *self, struct gobj *obj);
extern void SetPlatformMoverMotionYFromSet(struct mover *self, struct gobj *part, s32 index);
extern void SetPlatformMoverMotionXFromSet(struct mover *self, struct gobj *part, s32 index);
extern void StartPlatformMoverMotionYFromSet(struct mover *self, struct gobj *part, s32 index);
extern void StartPlatformMoverMotionXFromSet(struct mover *self, struct gobj *part, s32 index);
extern void DestroyPlatformMover(struct mover *self, s32 flags);
extern struct mover *CreatePlatformMover(struct mover *self, s32 distX, s32 distY, u32 dirX,
                                         u8 dirY, s32 kind);
extern void ClearPlatformMoverActive(struct mover *self);

/* src/objects/platform_collide.c */
extern void ResolvePlatformCollision(struct gobj *self, void *unused);

/* src/objects/platform_contact.c */
extern s32 CheckPlatformContact(struct gobj *self);

/* src/objects/platform_create.c */
extern struct gobj *CreatePlatform(u16 id, u16 x, u16 y, u16 index, s32 kind);

/* src/objects/player_contact.cpp */
extern void CheckPlayerContact(void *part);
extern void ResolvePlayerContact(void *part);

/* src/objects/sprite.cpp */
extern void DrawSpriteAt(void *self, void *part, s32 x, s32 y);
extern void DrawSprite(void *self, void *part);
extern void DestroySpriteRenderer(void *self, u32 flags);
extern void InitSpriteRenderer(void);
extern void ResetSpriteObj(void *self);
extern struct aabb GetSpriteBounds(struct box_part *part);
extern struct aabb GetSpriteHitbox(struct box_part *part);
extern void *GetSpriteAttackBox(void *dest, void *pt);
extern void *GetSpriteBodyBox(void *dest, void *pt);
extern s32 CheckSpritePickup(struct collect_part *part);
extern s32 IsSpriteObjOnScreen(struct box_part *part);
extern s32 SpriteObjOverlapsRect(struct actor *part, struct aabb *region);
extern void AdvanceSpriteAnim(struct box_part *part);

/* src/objects/sprite_anim.cpp */
extern u8 GetSpriteAnimFrameCount(struct actor *part);
extern u8 GetSpriteAnimDuration(struct actor *part);
extern void ResetSpriteFrameIndex(void *part);
extern void SetSpriteFrameTimer(void *part, s32 val);
extern void ResetSpriteFrameTimer(void *part);
extern void SetSpriteAnimIndex(void *part, u8 val);
extern void SetSpriteAnim(void *part, u8 idx);
extern void IncSpriteFrameIndex(void *part);
extern void IncSpriteFrameTimer(void *part);
extern void SetSpriteMoveAxes(void *part, u8 val);
extern u8 GetSpriteMoveAxes(void *part);
extern s32 GetSpriteFrameIndex(struct box_part *part);
extern s32 GetSpriteFrameTimer(struct box_part *part);
extern u8 GetSpriteAnim(void *part);
extern s32 GetSpriteGfxMode(void *part);
extern void SetSpriteGfxMode(void *part, s32 value);
extern s32 GetSpriteFlipX(void *part);
extern s32 GetSpriteFlipY(void *part);
extern u8 GetSpriteAnimDone(void *part);
extern s32 GetSpriteMosaic(void *part);
extern s32 GetSpriteOamPalette(void *part);
extern s32 GetSpriteColorMode(void *part);
extern u16 GetSpriteAffine(struct box_part *part);
extern void SetSpriteAffine(struct box_part *part, u16 val);
extern void DrawSpriteWithOffset(struct actor *part, s32 dx, s32 dy);
extern void SetSpritePriority(void *part, s32 value);
extern s32 GetSpritePriority(void *part);
extern void DestroyUiSpriteObj(struct actor *part, u32 flags);
extern struct actor *InitUiSpriteObj(struct actor *part);
extern void UpdatePartList(struct part_list *list);
extern void CollidePartList(struct part_list *list, struct aabb box, s32 unused,
                            struct box_part *other);
extern void CollidePartWithPlayer(struct part_list *list, struct aabb box, struct box_part *part);

/* src/objects/sprite_obj.cpp */
extern s32 SpriteHitboxOverlaps(struct actor *part, void *region);
extern s32 GetSpriteAnimPaletteSlot(struct actor *part);
extern void OffsetFromHitboxEdge(void *dest, s32 kind, void *rec);
extern void OffsetToHitboxEdge(void *dest, s32 kind, void *rec);
extern void OffsetToHitboxEdgeStart(void *dest, s32 kind, void *rec);
extern s32 IsSpriteObjInsideRect(struct actor *part, void *box);
extern s32 IsSpriteObjNearCamera(struct actor *part);
extern s32 ApplySpriteObjVelocity(void);
extern void DrawSpriteObj(void *part);
extern void UpdateSpriteObj(struct actor *part);
extern void *GetSpriteObjHitbox(struct actor *part);
extern s32 GetSpriteTileBase(void *part);
extern void *GetSpriteFrame(struct gfx_part *part);
extern s32 GetSpriteObjPriority(void);
extern void *CreateSpriteObj(u16 id, u16 x, u16 y, u16 unused);
extern s32 GetSpriteObjClassId(void);
extern void DestroySpriteObj(struct actor *self, u32 flags);
extern struct actor *InitSpriteObj(struct actor *self);
extern void *GetSpriteFrameAnchor(void *part);
extern void *GetSpriteFrameThirdBox(void *part);
extern void *GetSpriteFrameAttackBox(void *part);
extern void *GetSpriteFrameBodyBox(void *part);
extern void *GetSpriteAnimRecord(struct actor *part);
extern void SetSpriteFrameIndex(struct actor *part, s32 frame);
extern u8 GetSpriteScreenSpace(void *part);
extern void SetSpriteScreenSpace(void *part, u8 val);
extern s32 IsSpriteHidden(void *part);
extern void ToggleSpriteHidden(void *part);
extern s32 IsPartSolid(void *part);
extern void ClearPartSolid(void *part);
extern void SetPartSolid(void *part);
extern s32 IsSpriteObjVulnerable(struct actor *part);
extern void ClearSpriteObjVulnerable(struct actor *part);
extern void SetSpriteObjVulnerable(struct actor *part);
extern void ResetSpriteAnimIndex(void *part);
extern s32 IsSpriteObjCollisionEnabled(struct actor *part);
extern void DisableSpriteObjCollision(struct actor *part);
extern void EnableSpriteObjCollision(struct actor *part);
extern u8 GetSpriteAnimating(void *part);
extern void SetSpriteAnimating(void *part, u8 val);
extern void SetSpriteFlipX(void *part, u8 value);
extern void SetSpriteFlipY(void *part, u8 value);
extern void SetSpriteAnimDone(void *part, u8 val);
extern u8 GetSpriteAnimPaletteId(struct actor *part);
extern s32 GetSpritePalette(void *part);
extern void SetSpritePalette(void *part, s32 value);
extern void SetSpriteAnimTable(void *part, void *val);
extern void *GetSpriteAnimTable(void *part);
extern u8 IsSpriteAnimLooping(struct actor *part);

/* src/objects/step_probe.cpp */
extern s32 ProbeHitboxEdgeTerrain(struct box_part *self, s32 mode, struct hitbox_quad *quad);

/* The controllers' motion records (src/data/motion_records_16b304.c),
 * read by StartCtrlTargetMotionYFromSet/StartCtrlTargetMotionXFromSet and
 * ApplyActionCtrlMotion. player.h has the player's two tables. */
extern const struct speed_ramp gCtrlMotionRecords[44];

/* The platform mover's entry set (src/data/entry_set_16c418.c), whose
 * entries are gDingodileMotionEntries[4..7] (bosses.h), and its motion
 * records (src/data/velocity_16c460.c). */
extern const struct entry_set gPlatformMoverMotionSet;
extern const struct speed_ramp gPlatformMoverMotionRecords[3];

/* The method tables (src/data/entity_vtables_7e3bec.c) */
extern const struct vtable_slot gCtrlVtable[13];
extern const struct vtable_slot gEffectCtrlVtable[13];
extern const struct vtable_slot gGroundSpriteVtable[15];
extern const struct vtable_slot gMovingSpriteVtable[15];
extern const struct vtable_slot gPlatformVtable[15];
extern const struct vtable_slot gPlatformMoverVtable[13];
extern const struct vtable_slot gSpriteObjVtable[13];
extern const struct vtable_slot gUiSpriteObjVtable[13];

/* The empty box GetSpriteFrameBodyBox and friends return for a frame
 * without one (src/data/obj_sizes_16b2e0.c). */
extern const struct hitbox_quad gEmptySpriteBox;
/* The anchor point GetSpriteFrameAnchor returns for a frame without one
 * (same file): {0, 0}. */
extern const struct sprite_point gEmptySpritePoint;

/* sym_iwram.txt */
extern s32 gLastSpriteVelY;

#endif /* __OBJECTS_H__ */
