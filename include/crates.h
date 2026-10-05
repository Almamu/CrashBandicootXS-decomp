#ifndef GUARD_CRATES_H
#define GUARD_CRATES_H

/* The crates subsystem (src/crates/): the crate object (`struct crate`,
 * crate.h), its collision and breaking, the slot crate, and the crate
 * list (the bucketed grid of crates and other collidable parts).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The crate list's `struct pool_manager` (and `pool_init`, the same
 * object as ResetCrateList sees it) is defined in each file that walks
 * it; the objects batch merges the copies with src/objects/part_list.c's.
 * InitCrateList is in that file. */

#include "core.h"
#include "aabb.h"
#include "byte_arg.h"
#include "vtable.h"

struct actor;
struct box_part;
struct crate;
struct part_list;
struct pool_init;
struct pool_item;
struct pool_manager;

/* The position ApplyCrateCollision takes by value (a collision
 * candidate's, src/objects/collision_queue.c). */
struct e08c_pos
{
    s32 x;
    s32 y;
};

/* A sprite frame's hitbox quad, `{s16 xOff, s16 yOff, u8 w, u8 h}` at
 * +4..+9 of its 28-byte record (BreakCrateTouchedByPlayer,
 * PlayerAnimWouldTouchCrate). sub_800CEAC is handed a pointer to the quad
 * itself. graphics.c's `struct anim_box` is the same layout. */
struct hitbox_quad {
    s16 xOff;
    s16 yOff;
    u8 w;
    u8 h;
};

/* The method table (src/data/entity_vtables_7e3bec.c). */
extern const struct vtable_slot gCrateVtable[13];

/* The crate tables, indexed by crate kind (src/data/object_tables_16bb6c.c). */
extern const u8 gSlotCrateTimers[4];
extern const u8 gCrateKindCounted[22];
extern const u8 gCrateKindBreakable[22];
extern const u8 gCrateKindExplosive[22];
extern const u8 gCrateKindUnbreakable[22];
extern const s32 gCrateHitResponse[22][7];
extern const u8 gStaticData_0816BF00[8];

/* Set when the crate list changes (sym_iwram.txt). */
extern u8 gCrateListChanged;

/* src/crates/crate_break.c */
extern void QueueCratePlayerCollision(struct crate *self, s32 idx);
extern void ApplyCrateCollision(struct crate *self, s32 kind, s32 code, s32 edge, s32 depth,
                                struct e08c_pos pos, s32 hit, struct byte_arg p20,
                                struct byte_arg p21, struct byte_arg pforced);
extern void ClearCrateStackTouched(struct crate *obj);
extern void MarkCrateStackTouched(struct crate *obj, struct aabb *ctx);
extern void BounceWumpaCrate(struct crate *self);
extern void LightTntCrate(struct crate *self);
extern void OpenCheckpointCrate(struct crate *self);
extern void BreakCrateInStack(struct crate *self, u32 arg1, u32 arg2, u32 dir);
extern void BreakCrate(struct crate *self, u32 arg1);
extern void OpenMysteryCrate(struct crate *self, u32 arg1);
extern void OpenSlotCrate(struct crate *self, u32 arg1);
extern void DropCratesAbove(struct crate *self);
extern void ExplodeCrate(struct crate *self, u8 near);
extern void BlastNearbyCrates(struct crate *self, s32 dist);
extern void UpdateCrates(void);
extern void DetonateNitroCrates(void);
extern void ActivateNitroSwitchCrate(struct crate *self);
extern void ActivateIronSwitchCrate(struct crate *self);
extern void SolidifyOutlineCrates(struct crate *self);
extern void SolidifyOutlineCrate(struct crate *self);
extern void BreakCratesInArea(s32 x, s32 y, s32 dist, s32 height);
extern void FinishBrokenCrate(struct crate *self);
extern void UpdateTntCountdown(struct crate *self);
extern void UpdateSlotCrate(struct crate *self);
extern void UpdateCrateFall(struct crate *self);

/* src/crates/crate.c */
extern u32 IsCrateInsideRect(void *self, struct aabb *box);
extern void ResolvePlayerCollisions(void);
extern void *GetCrateBelow(void *self);
extern void *GetCrateAbove(void *self);
extern void SetCrateBelow(void *self, void *val);
extern void SetCrateAbove(void *self, void *val);
extern u32 GetCrateClassId(void);
extern void DestroyCrate(struct actor *self, u32 arg1);
extern struct actor *InitCrate(struct actor *self);
extern s32 FindLineCrossingYMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound);
extern s32 FindLineCrossingXMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound);

/* src/crates/crate_create.c */
extern void *CreateCrate(u16 id, u16 x, u16 y, u16 slot, u8 type);

/* src/crates/crate_draw.c */
extern void DrawCrate(void *self);

/* src/crates/crate_grid_collide.c */
extern void CollideCrateGrid(struct pool_manager *m, struct aabb box, s32 unused, struct box_part *other);
extern void CollideCrateGridPartWithPlayer(struct part_list *list, struct aabb box, struct box_part *part);

/* src/crates/crate_grid_link.c */
extern void LinkCrateToActiveBucket(struct pool_manager *manager, void *obj);

/* src/crates/crate_grid_unlink.c */
extern void UnlinkCrateFromGrid(struct pool_manager *manager, struct pool_item *item);

/* src/crates/crate_hit.c */
extern u8 sub_800CEAC(void *self, struct hitbox_quad *quad, struct aabb *box, s32 xOffset, s32 yOffset);
extern struct box_part *sub_800CF70(struct box_part *self, struct aabb *box, u8 *foundFlag);
extern void BreakCrateTouchedByPlayer(struct box_part *self);

/* src/crates/crate_list.c */
extern void CollideCrateGridPartWithObject(struct part_list *list, struct aabb box, struct box_part *part, struct box_part *other);
extern void RemoveCrateFromList(struct pool_manager *manager, void *target);
extern void RemoveCrateListAt(struct pool_manager *manager, s32 index);
extern void *AddCrateGridNode(struct pool_manager *manager, void *data, s32 bucket, s32 extra);
extern void LinkCrateInGrid(struct pool_manager *manager, void *obj);
extern void AddCrateToList(struct pool_manager *manager, void *obj);
extern void DestroyCrateList(struct pool_manager *manager, s32 flags);

/* src/crates/crate_list_draw.c */
extern void DrawCrateList(void *manager);

/* src/crates/crate_list_reset.c */
extern void ResetCrateList(struct pool_init *m);

/* src/crates/crate_list_update.c */
extern void UpdateCrateList(struct pool_manager *manager);

/* src/crates/crate_player_collide.c */
extern void CollidePlayerWithCrates(struct pool_manager *m, s32 unused);

/* src/crates/crate_reset.c */
extern s32 FindLineCrossing(s32 pos, s32 count, s32 a, s32 b, s32 limit);
extern void ResetCrate(void *self);

/* src/crates/crate_stack.c */
extern void OpenLifeCrate(struct actor *self, u32 arg1);
extern u8 IsCrateKindBreakable(void *arg0, u32 idx);
extern void *GetTopCrate(void *self);
extern void *GetBottomCrate(void *self);
extern s32 CollideCrateWithPlayer(void *self, u32 idx, s32 testX, s32 testY);

/* src/crates/crate_time_trial.c */
extern void ConvertCratesForTimeTrial(void);
extern void OpenAkuAkuCrate(struct crate *crate);

/* src/crates/crate_touch.c */
extern u8 PlayerAnimWouldTouchCrate(struct box_part *self, s32 action);

/* src/crates/crate_update.c */
extern void UpdateCrate(struct crate *self);

/* src/crates/slot_crate.c */
extern u32 GetSlotCrateStage(void *self);
extern void DecrementSlotCrateStage(void *self);
extern void SetSlotCrateStage(void *self, u32 state);
extern void ClearSlotCrateStage(void *self);
extern u32 GetSlotCrateSpins(void *self);
extern void DecrementSlotCrateSpins(void *self);
extern void SetSlotCrateSpins(void *self, u32 state);
extern void SetSlotCratePhase(void *self, u32 state);
extern u32 GetSlotCratePhase(void *self);
extern void SetCrateKind(void *self, u8 val);
extern u8 GetCrateKind(void *self);
extern void SetCrateFallDistance(void *self, s32 val);
extern s32 GetCrateFallDistance(void *self);
extern void SetCrateState(void *self, u32 val);
extern u32 GetCrateState(void *self);
extern void SetCrateFallSpeed(void *self, u8 val);
extern s32 GetCrateFallSpeed(void *self);
extern u32 IsCrateBusy(void *self);
extern void SetCrateBusy(void *self);
extern void ClearCrateBusy(void *self);
extern void SetCrateTouched(void *self, u8 val);
extern u8 GetCrateParamB(void *self);
extern u8 GetCrateParamA(void *self);
extern void SetCrateSolidKind(void *self, u32 val);
extern void SetCrateTrialKind(void *self, s32 val);
extern s32 GetCrateTrialKind(void *self);

#endif /* GUARD_CRATES_H */
