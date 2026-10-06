#ifndef __OBJECTS_H__
#define __OBJECTS_H__

/* The objects subsystem (src/objects/): the base level objects (sprite
 * objects, moving and ground sprites, platforms and their movers, the
 * controllers), their animation, the part lists and the collision queue.
 * Every function src/objects/ defines, with the prototype of its
 * definition, and the globals and tables its files use
 * (docs/headers_plan.md). InitCrateList (part_list.c) is in crates.h. A
 * .c file that needs a different local declaration for codegen keeps it
 * as an asm-label alias with a `codegen:` comment.
 *
 * The objects' structs live in their type headers (box_part.h,
 * gfx_part.h, gobj_1a794.h, ...); this header only declares their tags. */

#include "core.h"
#include "aabb.h"
#include "byte_arg.h"
#include "vtable.h"

struct actor;
struct box_part;
struct candidate_list;
struct cbf4_other;
struct collect_part;
struct collision_queue;
struct part_list;
struct gfx_part;
struct gfx_vec;
struct gobj;
struct hitbox_quad;
struct mover;
struct sprite_box;

/* A position pair: a collision candidate's (src/objects/collision_queue.c),
 * which ApplyCrateCollision takes by value. Copied as one 8-byte struct
 * (the ROM's paired `ldr; ldr; str; str`). */
struct e08c_pos
{
    s32 x;
    s32 y;
};

/* An entry set: the {a, b} index pairs into a motion record table (two
 * records per state; `entries` is an array of pairs) and a word the code
 * doesn't read, 0x100 in every set in the ROM. A controller or mover
 * keeps one at +0x04 (SetCtrlAnimSet, gobj_1a794.h's `struct mover`
 * `set`). */
struct entry_set {
    const u32 (*entries)[2];
    u32 unk_04;
};

/* One 12-byte motion record: the parameters StartCtrlTargetMotionX and
 * the other motion starters read for one axis. The `{a, b}` index pairs
 * of the entry sets (src/data/entry_set_16b92c.c) pick two per state. */
struct motion_rec {
    s32 a;
    s32 b;
    s32 c;
};

/* src/objects/collision_queue.c */
extern void ResolveCollisionCandidates(struct candidate_list *self);
extern void AddCollisionCandidate(struct collision_queue *self, void *neighbor, s32 kind, s32 field10, s32 field14, s32 field18,
                                  struct e08c_pos pos, s32 field1c, s32 field20, s32 field21);
extern void DestroyCollisionQueue(void *self, s32 flags);
extern void ResetCollisionQueue(void *self);

/* src/objects/ctrl.c */
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

/* src/objects/effect_ctrl.c */
extern void UpdateEffectCtrl(void *self, struct cbf4_other *other);
extern void EffectCtrlHandleEvent(void *self);
extern void nullsub_3(void *self);
extern void DestroyEffectCtrl(void *self, s32 flags);
extern void *InitEffectCtrl(void *self);

/* src/objects/ground_sprite.c */
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

/* src/objects/ground_sprite_collide.c */
extern u8 CollideGroundSprite(struct box_part *self);
extern s32 ProbeGroundSpriteTerrain(struct box_part *self);
extern u8 ProbeGroundSpriteFloor(struct box_part *self, struct hitbox_quad *quad, u8 *outFlag);

/* src/objects/ground_sprite_update.c */
extern void UpdateGroundSprite(struct gobj *self);
extern void sub_800A590(struct gobj *self);

/* src/objects/moving_sprite.c */
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

/* src/objects/moving_sprite_collide.c */
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

/* src/objects/part_collide.c */
extern void CollidePartWithObject(struct part_list *list, struct aabb box, struct box_part *part, struct box_part *other);

/* src/objects/part_list.c */
extern void DrawPartList(struct part_list *manager);
extern void RemoveFromPartList(struct part_list *manager, void *target);
extern void RemovePartListAt(struct part_list *manager, s32 index);
extern void AddToPartList(struct part_list *manager, void *value);
extern void DestroyPartList(struct part_list *manager, s32 flags);
extern struct part_list *InitPartList(struct part_list *manager, s32 count);

/* src/objects/part_list_cull.c */
extern void CullPartList(void *manager);
extern void ClearPartList(void *manager);
extern void CollidePartsOfClass(void *manager, s32 classId);

/* src/objects/platform.c */
extern void UpdatePlatform(struct gobj *self);
extern s32 sub_801B29C(struct gobj *self);
extern void sub_801B2A8(struct gobj *self, u8 value);
extern s32 GetPlatformClassId(void);
extern void DestroyPlatform(struct gobj *self, s32 flags);
extern void sub_801B2D8(struct gobj *self);
extern struct gobj *InitPlatform(struct gobj *self);
extern void UpdatePlatformMover(struct mover *self, struct gobj *obj);
extern void MovePlayerWithPlatform(struct mover *self, struct gobj *obj);
extern void SetPlatformMoverMotionYFromSet(struct mover *self, struct gobj *part, s32 index);
extern void SetPlatformMoverMotionXFromSet(struct mover *self, struct gobj *part, s32 index);
extern void StartPlatformMoverMotionYFromSet(struct mover *self, struct gobj *part, s32 index);
extern void StartPlatformMoverMotionXFromSet(struct mover *self, struct gobj *part, s32 index);
extern void DestroyPlatformMover(struct mover *self, s32 flags);
extern struct mover *CreatePlatformMover(struct mover *self, s32 distX, s32 distY, u32 dirX, u8 dirY, s32 kind);
extern void ClearPlatformMoverActive(struct mover *self);

/* src/objects/platform_collide.c */
extern void ResolvePlatformCollision(struct gobj *self, void *unused);

/* src/objects/platform_contact.c */
extern s32 CheckPlatformContact(struct gobj *self);

/* src/objects/platform_create.c */
extern struct gobj *CreatePlatform(u16 id, u16 x, u16 y, u16 index, s32 kind);

/* src/objects/player_contact.c */
extern void CheckPlayerContact(void *part);
extern void ResolvePlayerContact(void *part);

/* src/objects/sprite.c */
extern void DrawSpriteAt(void *self, void *part, s32 x, s32 y);
extern void DrawSprite(void *self, void *part);
extern void DestroySpriteRenderer(void *self, u32 flags);
extern void nullsub_2(void);
extern void ResetSpriteObj(void *self);
extern struct aabb GetSpriteBounds(struct box_part *part);
extern struct aabb GetSpriteHitbox(struct box_part *part);
extern void *GetSpriteAttackBox(void *dest, void *pt);
extern void *GetSpriteBodyBox(void *dest, void *pt);
extern s32 CheckSpritePickup(struct collect_part *part);
extern s32 IsSpriteObjOnScreen(struct box_part *part);
extern s32 SpriteObjOverlapsRect(struct actor *part, struct aabb *region);
extern void AdvanceSpriteAnim(struct box_part *part);

/* src/objects/sprite_anim.c */
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
extern void CollidePartList(struct part_list *list, struct aabb box, s32 unused, struct box_part *other);
extern void CollidePartWithPlayer(struct part_list *list, struct aabb box, struct box_part *part);

/* src/objects/sprite_obj.c */
extern s32 SpriteHitboxOverlaps(struct actor *part, void *region);
extern s32 GetSpriteAnimPaletteSlot(struct actor *part);
extern void sub_8008188(void *dest, s32 kind, void *rec);
extern void sub_8008200(void *dest, s32 kind, void *rec);
extern void sub_8008278(void *dest, s32 kind, void *rec);
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

/* src/objects/step_probe.c */
extern s32 sub_8009BE0(struct box_part *self, s32 mode, struct hitbox_quad *quad);

/* The controllers' motion records (src/data/motion_records_16b304.c),
 * read by StartCtrlTargetMotionYFromSet/StartCtrlTargetMotionXFromSet and
 * ApplyActionCtrlMotion. player.h has the player's two tables. */
extern const struct motion_rec gCtrlMotionRecords[44];

/* The platform mover's entry set (src/data/entry_set_16c418.c), whose
 * entries are gDingodileMotionEntries[4..7] (bosses.h). */
extern const struct entry_set gPlatformMoverMotionSet;

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
extern const struct sprite_box gEmptySpriteBox;

/* sym_iwram.txt */
extern s32 gLastSpriteVelY;

#endif /* __OBJECTS_H__ */
