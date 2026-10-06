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
 * The crate list (`struct pool_manager`, below) is set up by InitCrateList,
 * which src/objects/part_list.c holds for ROM order. */

#include "core.h"
#include "aabb.h"
#include "gfx.h"
#include "objects.h"
#include "byte_arg.h"
#include "vtable.h"

struct actor;
struct box_part;
struct crate;
struct part_list;
struct pool_item;

/* One entry of the crate list's free list: a grid node not in use. */
struct pool_link {
    struct pool_node *node;     // 0x00
    struct pool_link *next;     // 0x04
};

/* One node of the crate list's grid (0x14 bytes): a listed part, the
 * next node in its bucket, and the free-list entry it was taken from.
 * UpdateCrateList also files a large part in bucket 255 under a second
 * node (`link`), and DrawCrateList/UpdateCrateList mark the nodes they
 * have handled. */
struct pool_node {
    struct box_part *data;      // 0x00
    struct pool_node *next;     // 0x04
    struct pool_link *wrap;     // 0x08
    struct pool_node *link;     // 0x0C
    u8 mark;                    // 0x10
    u8 mark2;                   // 0x11
};

/* codegen: `struct pool_node` with untyped fields, the view
 * PoolResetFreeList (crate_list_reset.c, part_list.c) zeroes the nodes
 * through. Through the real `struct pool_node *` fields, gcc takes the
 * zeroing stores as possible writes to `m->nodeArray` and reloads it
 * (docs/headers_plan.md, "Codegen findings"). Both files had a copy. */
struct pool_init_node {
    void *data;
    void *next;
    struct pool_link *wrap;
    void *link;
    u8 mark;
};

/* The crate list (`gCrateList`): a fixed-slot pool of the crates and
 * other collidable parts, set up by InitCrateList. `slotArray` holds the
 * active objects (bounded by `activeCount`, up to `capacity`);
 * `nodeArray` is a flat array of `capacity` grid nodes; `gridHead`/
 * `gridTail` are a 256-bucket spatial hash grid, each bucket a
 * singly-linked list of nodes (head set once when a bucket leaves empty,
 * tail always updated for O(1) append - see AddCrateGridNode);
 * `freeListArray` is `capacity` free-list entries threaded into a
 * singly-linked list, `freeListHead` pointing at its first still-free
 * entry. ResetCrateList and InitCrateList saw it as `struct pool_init`. */
struct pool_manager {
    s32 activeCount;                    // 0x000
    s32 capacity;                       // 0x004
    struct box_part **slotArray;        // 0x008
    struct pool_node *nodeArray;        // 0x00C
    struct pool_node *gridHead[256];    // 0x010
    struct pool_node *gridTail[256];    // 0x410
    struct pool_link *freeListArray;    // 0x810
    struct pool_link *freeListHead;     // 0x814
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
extern u32 IsCrateInsideRect(struct crate *self, struct aabb *box);
extern void ResolvePlayerCollisions(void);
extern struct crate *GetCrateBelow(struct crate *self);
extern struct crate *GetCrateAbove(struct crate *self);
extern void SetCrateBelow(struct crate *self, struct crate *val);
extern void SetCrateAbove(struct crate *self, struct crate *val);
extern u32 GetCrateClassId(void);
extern void DestroyCrate(struct actor *self, u32 arg1);
extern struct actor *InitCrate(struct actor *self);
extern s32 FindLineCrossingYMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound);
extern s32 FindLineCrossingXMajor(s32 y, s32 count, s32 dx, s32 dy, s32 yStep, s32 bound);

/* src/crates/crate_create.c */
extern void *CreateCrate(u16 id, u16 x, u16 y, u16 slot, u8 type);

/* src/crates/crate_draw.c */
extern void DrawCrate(struct crate *self);

/* src/crates/crate_grid_collide.c */
extern void CollideCrateGrid(struct pool_manager *m, struct aabb box, s32 unused, struct box_part *other);
extern void CollideCrateGridPartWithPlayer(struct part_list *list, struct aabb box, struct box_part *part);

/* src/crates/crate_grid_link.c */
extern void LinkCrateToActiveBucket(struct pool_manager *manager, struct box_part *obj);

/* src/crates/crate_grid_unlink.c */
extern void UnlinkCrateFromGrid(struct pool_manager *manager, struct pool_item *item);

/* src/crates/crate_hit.c */
extern u8 sub_800CEAC(struct crate *self, struct hitbox_quad *quad, struct aabb *box, s32 xOffset, s32 yOffset);
extern struct crate *sub_800CF70(struct crate *self, struct aabb *box, u8 *foundFlag);
extern void BreakCrateTouchedByPlayer(struct box_part *self);

/* src/crates/crate_list.c */
extern void CollideCrateGridPartWithObject(struct part_list *list, struct aabb box, struct box_part *part, struct box_part *other);
extern void RemoveCrateFromList(struct pool_manager *manager, struct box_part *target);
extern void RemoveCrateListAt(struct pool_manager *manager, s32 index);
extern void *AddCrateGridNode(struct pool_manager *manager, struct box_part *data, s32 bucket, s32 extra);
extern void LinkCrateInGrid(struct pool_manager *manager, struct box_part *obj);
extern void AddCrateToList(struct pool_manager *manager, struct box_part *obj);
extern void DestroyCrateList(struct pool_manager *manager, s32 flags);

/* src/crates/crate_list_draw.c */
extern void DrawCrateList(struct pool_manager *manager);

/* src/crates/crate_list_reset.c */
extern void ResetCrateList(struct pool_manager *m);

/* src/objects/part_list.c (for ROM order) */
extern struct pool_manager *InitCrateList(struct pool_manager *m, s32 count);

/* src/crates/crate_list_update.c */
extern void UpdateCrateList(struct pool_manager *manager);

/* src/crates/crate_player_collide.c */
extern void CollidePlayerWithCrates(struct pool_manager *m, s32 unused);

/* src/crates/crate_reset.c */
extern s32 FindLineCrossing(s32 pos, s32 count, s32 a, s32 b, s32 limit);
extern void ResetCrate(struct crate *self);

/* src/crates/crate_stack.c */
extern void OpenLifeCrate(struct actor *self, u32 arg1);
extern u8 IsCrateKindBreakable(void *arg0, u32 idx);
extern struct crate *GetTopCrate(struct crate *self);
extern struct crate *GetBottomCrate(struct crate *self);
extern s32 CollideCrateWithPlayer(struct crate *self, u32 idx, s32 testX, s32 testY);

/* src/crates/crate_time_trial.c */
extern void ConvertCratesForTimeTrial(void);
extern void OpenAkuAkuCrate(struct crate *crate);

/* src/crates/crate_touch.c */
extern u8 PlayerAnimWouldTouchCrate(struct box_part *self, s32 action);

/* src/crates/crate_update.c */
extern void UpdateCrate(struct crate *self);

/* src/crates/slot_crate.c */
extern u32 GetSlotCrateStage(struct crate *self);
extern void DecrementSlotCrateStage(struct crate *self);
extern void SetSlotCrateStage(struct crate *self, u32 state);
extern void ClearSlotCrateStage(struct crate *self);
extern u32 GetSlotCrateSpins(struct crate *self);
extern void DecrementSlotCrateSpins(struct crate *self);
extern void SetSlotCrateSpins(struct crate *self, u32 state);
extern void SetSlotCratePhase(struct crate *self, u32 state);
extern u32 GetSlotCratePhase(struct crate *self);
extern void SetCrateKind(struct crate *self, u8 val);
extern u8 GetCrateKind(struct crate *self);
extern void SetCrateFallDistance(struct crate *self, s32 val);
extern s32 GetCrateFallDistance(struct crate *self);
extern void SetCrateState(struct crate *self, u32 val);
extern u32 GetCrateState(struct crate *self);
extern void SetCrateFallSpeed(struct crate *self, u8 val);
extern s32 GetCrateFallSpeed(struct crate *self);
extern u32 IsCrateBusy(struct crate *self);
extern void SetCrateBusy(struct crate *self);
extern void ClearCrateBusy(struct crate *self);
extern void SetCrateTouched(struct crate *self, u8 val);
extern u8 GetCrateParamB(struct crate *self);
extern u8 GetCrateParamA(struct crate *self);
extern void SetCrateSolidKind(struct crate *self, u32 val);
extern void SetCrateTrialKind(struct crate *self, s32 val);
extern s32 GetCrateTrialKind(struct crate *self);

#endif /* GUARD_CRATES_H */
