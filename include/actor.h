#ifndef __ACTOR_H__
#define __ACTOR_H__

/* Two things share this header (docs/headers_plan.md):
 *
 * - `struct actor`, the small on-screen entity of src/gfx/graphics.c;
 * - the actor subsystem (src/actor/): every function it defines, and the
 *   globals and data tables its files use. The prototypes are copied from
 *   the definitions. A .c file that needs a different local declaration
 *   for codegen keeps it as an asm-label alias with a `codegen:` comment.
 *
 * The bosses and vehicles built on `struct actor_self` are declared in
 * bosses.h and vehicle.h. */

#include "core.h"
#include "actor_self.h"
#include "vtable.h"

/* A small, moving on-screen object: position, a handful of flag bits, a
 * width/height pair (both raw and pre-halved/negated for centering), and
 * a pointer to a per-category data table (offset/text record pairs read
 * at several different fixed offsets by src/gfx/graphics.c's
 * IsEntityNearCamera/CheckEntityPlayerContact/IsEntityInsideRect/UpdateEntity/ResetEntity/etc. and
 * by power_dialog_draw.c's DestroyPowerDialog - none of that table's own shape is
 * understood yet, so it stays a raw `void *` here). Exactly 0x1c bytes -
 * confirmed by CreateEntity's `OperatorNew(0x1c)` allocation. The
 * fields at 0x0B, 0x0D-0x0F and 0x16-0x17 aren't understood beyond their
 * offset yet - named `unusedNN` rather than guessed. `id` and `kind`
 * have the same offsets and roles as `struct gobj`'s and `struct
 * player`'s. `struct power_dialog.icon` (in
 * power_dialog_draw.c) points at one of these. */
struct actor {
    s32 x; // 0x00 - Q8 fixed-point screen position
    s32 y; // 0x04 - Q8 fixed-point screen position
    // 0x08 - the spawn's bit index in the "gone" bitmap (MarkEntityGone); 0xFFFF: none
    u16 id;
    // 0x0A - the object kind sent to the player's hit method
    // on contact (CheckEntityPlayerContact, Get/SetEntityKind)
    u8 kind;
    u8 unused_0B;    // 0x0B
    u8 flags;        // 0x0C - bit 0 gone (MarkEntityGone), 1 unknown, 2 player contact enabled,
                     //        3 touched by the player, 4 always active (skips the camera tests);
                     //        sprite objects add 5 unknown, 6 vulnerable, 7 collision enabled
    u8 unused_0D[3]; // 0x0D-0x0F
    s16 halfW;       // 0x10 - -rawW/2, set by SetEntitySize/ResetEntity
    s16 halfH;       // 0x12 - -rawH/2, set by SetEntitySize/ResetEntity
    u8 rawW;         // 0x14
    u8 rawH;         // 0x15
    u8 unused_16[2]; // 0x16-0x17
    void *table;     // 0x18 - per-category data table, shape not yet known
};

COMPILE_TIME_ASSERT(actor_h, sizeof(struct actor) == 0x1c);

/* actor_anim.h has the full definitions; it can't be included here,
 * since several includers of this header define their own `struct
 * anim_box`/`struct sprite_frame` (docs/headers_plan.md, batch 7). */
struct anim_table_record;
struct category_vtable;
struct sub_effect_record;

struct actor_self_54;
struct actor_spawn;

/* The actor zone (src/actor/): the 3D actor object (`struct actor_self`),
 * its animation, spawning, category frame and backgrounds. */

/* src/actor/actor.c */
extern s32 IsTouchingPlayer(void *self);
extern void *InitActorPart(void *self, void *part, s32 b, s32 c, s32 d);
extern void UpdateActor(void *self);
extern void DrawActor(void *self);
extern void UpdateActorDepth(struct actor_self *self);
extern u8 GetActorRecordIndex(struct actor_self *self);
extern void SetActorState(struct actor_self *self, s32 a, s32 kind);
extern s32 GetActorZ(struct actor_self *self);
extern s32 GetActorY(struct actor_self *self);
extern s32 GetActorX(struct actor_self *self);
extern void *GetActorWorldBox(void *out, void *self);
extern u8 IsActorVisible(void *self);
extern void DestroyActor(void *self, s32 flags);
extern s32 IsSpawnCollected(void *self);
extern void MarkSpawnCollected(void *self);
extern void ClearCollectedSpawns(void);
extern void RestoreActorPaletteCycle(void);
extern void SaveActorPaletteCycle(void);
extern void UpdateActorPaletteCycle(void);
extern void SetActorPaletteCycle(s32 idx);
extern void EnableActorPaletteCycle(u8 flag);

/* src/actor/actor_anim.c */
extern s32 GetAnimFrameBaseOffset(struct actor_self *self);
extern s32 GetAnimFrameAttr(struct actor_self *self);
extern u8 *GetAnimFrameData(struct actor_self *self);
extern void SetActorAnim(struct actor_self *self, s32 idx);
extern void sub_803B25C(struct actor_self *self, u32 flags);
extern void DestroyPolarObstacle(struct actor_self *self, u32 flags);
extern s32 GetActorHp(struct actor_self_54 *self);
extern void DamageActor(void *self);

/* src/actor/actor_bg.c */
extern void ShakeActorBg(s32 arg0);
extern void SetActorBgLayerDepth(s32 arg0);
extern s32 GetActorBgLayerDepth(void);
extern void ActorCategoryEndStub(void);
extern void CommitActorBgScroll(void);
extern s32 GetActorBgCenterY(void);
extern s32 GetActorBgCenterX(void);

/* src/actor/actor_category_frame.c */
extern s32 RunActorCategoryFrame(void);
extern void *FindShotTarget(struct actor_self *self);

/* src/actor/actor_category_init.c */
extern s32 InitActorCategory(s32 category);

/* src/actor/actor_category_select.c */
extern void SelectActorCategory(s32 type, struct sub_effect_record *table, void *animTable,
                                u8 active, s32 variant, s32 checkpoint);

/* src/actor/actor_category_stats.c */
extern s32 CountCategoryCrates(s32 categoryIdx);
extern void AddActorMissedNitro(void);
extern s32 GetActorMissedNitros(void);
extern s32 GetActorCheckpoint(void);

/* src/actor/actor_factory.c */
extern struct actor_self *CreateActor(u8 kind, s32 x, s32 y, s32 z, void *spawn);
extern void ConstructAnimTableState(struct anim_table_record *table, s32 z);
extern struct actor_self *SpawnActor(struct actor_spawn *spawn, u8 useBonus, s32 zOffset);
extern struct actor_self *ConstructActorPart(struct actor_self *self, struct anim_table_record *rec,
                                             s32 z);

/* src/actor/actor_spawn.c */
extern s32 GetActorCategoryFrameCount(void);
extern s32 GetActorSpawnOffset(void);
extern void ResumeActorSpawns(void);
extern void PauseActorSpawns(void);
extern s32 GetActorSpawnNextTarget(s32 idx);
extern s32 GetActorSpawnZ(s32 idx);
extern s32 GetActorSpawnY(s32 idx);
extern s32 GetActorSpawnX(s32 idx);
extern s32 GetActorSpawnKindIndex(s32 idx);
extern s32 CanPauseActorCategory(void);
extern void ReloadActorCategoryGraphics(void);
extern void DestroyAllActors(void);
extern void UpdateActorCategoryBg2(void);
extern void SetActorCategoryExitStatus(s32 arg0);

/* src/actor/actor_vram_pool.c */
extern void SetupActorVramPool(void);

/* src/actor/bg_picture.c */
extern void LoadBgPicture(u8 *pic);
extern void FillBgPictureMap(u8 *nib, u16 *map, s32 cols, s32 rows);

/* src/actor/cell_anim.c */
extern void SetActorCheckpoint(s32 arg0);
extern s32 IsActorMaskAssistDue(void);
extern void UploadCellAnimFrame(void);
extern void InitCellAnim(s32 arg0, void *cellAnim, u32 animSize, s32 arg3);
extern void ResetCellAnimBg(void);
extern void ActorCategoryAttemptEndStub(void);
extern s32 GetCellAnimFreeTile(void);
extern void FlipCellAnimPage(void);
extern s32 GetCellAnimDistance(void);
extern void AdvanceCellAnim(void);
extern s32 GetCellAnimFrameStep(void);
extern s32 GetCellAnimSpeed(void);
extern void SetCellAnimSpeed(s32 arg0);
extern void FillCellAnimTilemap(s32 arg0, s32 w, s32 h);
extern void InitActorBgScroll(s32 arg0);
extern void UpdateActorBgScroll(s32 arg0, s32 arg1);

/* The actor zone's globals (sym_iwram.txt). */
extern struct anim_table_record *gActorAnimTable;
extern s32 gActorBg0VOffset;
extern s32 gActorBgHeight;
extern s32 gActorBgScrollEaseShift;
extern s32 gActorBgScrollMaxX;
extern s32 gActorBgScrollMaxY;
extern s32 gActorBgScrollType;
extern s32 gActorBgScrollX;
extern s32 gActorBgScrollY;
/* gActorBgShake: a Y bias UpdateActorBgScroll subtracts from the BG
 * scroll (ShakeActorBg sets it, CommitActorBgScroll clears it). */
extern s32 gActorBgShake;
extern s32 gActorBgWidth;
extern s32 gActorCategory;
extern s32 gActorCategoryDeaths;
extern s32 gActorCategoryExitStatus;
extern s32 gActorCategoryFrameCount;
extern const struct category_vtable *gActorCategoryVtable;
extern s32 gActorCheckpointMissedNitros;
extern s32 gActorDrawCount;
extern struct actor_self **gActorDrawList;
extern s32 gActorFarClipDepth;
/* A frame-tick counter, incremented every category-load-loop tick
 * (InitActorCategory) and snapshotted at the top of SelectActorCategory
 * for the bookkeeping the gActorSpawnTable accessors use. */
extern s32 gActorMissedNitros;
extern s32 gActorNearClipDepth;
extern u8 gActorPaletteCycleEnabled;
extern s32 gActorPaletteCycleFrame;
extern s32 gActorPaletteCycleTarget;
extern s32 gActorPaletteCycleTimer;
extern s32 gActorSpawnIndex;
extern s32 gActorSpawnOffset;
extern struct sub_effect_record *gActorSpawnTable;
extern u8 gActorSpawnsPaused;
extern void *gCategorySpriteSheet; /* the decompressed category sprite sheet */
extern void *gCellAnim;
extern s32 gCellAnimCols;
extern s32 gCellAnimDistance;
extern s32 gCellAnimFrameSize;
extern s32 gCellAnimFrameStep;
extern u8 gCellAnimHasBanks;
extern s32 gCellAnimLength;
extern u8 gCellAnimPage;
extern s32 gCellAnimRows;
extern s32 gCellAnimSpeed;
extern s32 gCellAnimTileBytes;
extern s32 gCellAnimTime;
extern u8 gCellAnimUploaded;
extern void *gCollectedSpawns[];
extern s32 gSavedActorPaletteCycleFrame;
extern s32 gSavedActorPaletteCycleTarget;
/* The deaths since the checkpoint that count towards the category's
 * `retryBossDeaths` (InitActorCategory): being carried off by the yeti
 * (exit status 2) in a type-0 category, any death in the others. */
extern s32 gActorCategoryBossDeaths;
/* The projection distance (Q8): DrawActor scales x/y by
 * gActorFocalLength / depth, and InitCellAnim starts the cell animation
 * this far from the checkpoint. Set by InitActorBgScroll. */
extern s32 gActorFocalLength;
/* The depth of the BG2 boss layer (the yeti, airship or hovercraft):
 * actors deeper than it get OBJ priority 2 and draw behind it (bit 15 of
 * their sortKey). InitActorBgScroll resets it to gActorFarClipDepth; the
 * bosses set it to their own distance (SetActorBgLayerDepth). */
extern s32 gActorBgLayerDepth;
/* UpdateActorBgScroll's input ranges: an input of +-range scrolls the BG
 * to either end of [0, gActorBgScrollMaxX/Y]. Set by InitActorBgScroll. */
extern s32 gActorBgScrollRangeX;
extern s32 gActorBgScrollRangeY;
/* SelectActorCategory's `active` argument (the deaths reached the
 * category's `bonusKindDeaths`): spawns use their record's `bonusKind`. */
extern u8 gActorSpawnUseBonus;

/* src/iwram/iwram_data.c */
extern s32 gActorCheckpoint;
extern s32 gCollectedSpawnCount;
extern void (*gDrawMirroredTilemapFunc)(u8 *pal, s32 lowBlock, s32 w, s32 h);
extern void (*gHeapSortActorsByKeyFunc)(s32 n, struct actor_self **list);

/* src/data/palette_cycle_175760.c */
extern const u16 gActorPaletteCycleFrames[32][14 * 16];
extern const s32 gActorPaletteCycleStartFrames[4];
extern const s32 gActorPaletteCycleTargetFrames[4];

/* src/data/entity_vtables_7e3bec.c */
extern const struct vtable_slot gStaticData_087E4F54[4];

#endif /* !__ACTOR_H__ */
