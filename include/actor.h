#ifndef __ACTOR_H__
#define __ACTOR_H__

/* Two things share this header (docs/headers_plan.md):
 *
 * - the entity's flags (`union EntityFlags`), which class Entity
 *   (include/entity.hpp) and the C view of the player (player.h) share,
 *   and the `struct actor` tag the C prototypes of the entity's methods
 *   take (gfx.h);
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
#include "constants/categories.h"

/* The entity flags byte at +0x0C and the one after it, +0x0D (Entity's
 * `f`, struct player's). */
union EntityFlags {
    u8 flags; // 0x0C
    struct {
        u8 gone:1;       // removed (SetGone); the lists drop it
        u8 unk_1:1;      // Get/Set/ClearFlag1
        u8 visible:1;    // in contact with the player (Is/Enable/DisableContact)
        u8 bit3:1;       // touched by the player or another object; SetTargetAnim clears it
        u8 active:1;     // always active: skips the camera tests (updated off screen too)
        u8 unk_5:1;      // Get/Set/ClearSpriteObjFlag5
        u8 vulnerable:1; // the player's attacks hit it
        u8 collides:1;   // Is/Enable/DisableCollision
        u8 floorProbe:1; // 0x0D - a ground sprite probes the floor (Enable/DisableFloorProbe)
        u8 grounded:1;   // a ground sprite stands on the floor (ProbeFloor)
        u8 blink:1;      // hidden this frame (a blinking part; Is/ToggleHidden)
        u8 solid:1;      // pushes the player out (Is/Set/ClearSolid)
        u8 exitMirror:1; // a bonus platform's exit facing (Platform::Get/SetExitMirror)
        u8 unk_0D_5:3;
    } b; // (ARM structs are 4-byte sized: the union spans 0x0C-0x0F)
    struct {
        u8 flags;  // 0x0C
        u8 flags2; // 0x0D
    } bytes;
};

/* The entity (class Entity, include/entity.hpp: 0x1C bytes, built by
 * CreateEntity). No C file reads its fields: the C callers of its methods
 * (SetEntityPos, SetEntityPixelPos; gfx.h) only pass the pointer, so the
 * C side has the tag alone. */
struct actor;

/* actor_anim.h has the full definitions; it can't be included here,
 * since several includers of this header define their own `struct
 * anim_box`/`struct sprite_frame` (docs/headers_plan.md, batch 7). */
struct anim_table_record;
struct category_vtable;
struct sub_effect_record;

/* A level spawn record as SpawnActor and SpawnJetpackActor (vehicle.h)
 * take it: the actor kinds, then the position in tile units (<< 8 to Q8).
 * It is the tail of a spawn table's struct sub_effect_record (actor_anim.h)
 * from its `kind` on, and the next record's `depth` (actor_factory.cpp's
 * and jetpack_spawn.cpp's copies were merged here, #656). */
struct actor_spawn {
    u8 kind;      // 0x00 - the normal kind
    u8 altKind;   // 0x01 - in time trial mode
    u8 bonusKind; // 0x02 - when the bonus kinds are on (`useBonus`, `alt`)
    u8 unk_03;
    s32 x; // 0x04
    s32 y; // 0x08
    s32 z; // 0x0C
};

/* The actor zone (src/actor/): the 3D actor object (`struct actor_self`),
 * its animation, spawning, category frame and backgrounds. */

/* src/actor/actor.cpp: C linkage, and the C names of ActorSelf's methods
 * (actor_self.hpp) */
extern s32 IsTouchingPlayer(void *self);
extern s32 IsSpawnCollected(void *self);
extern void MarkSpawnCollected(void *self);
extern void ClearCollectedSpawns(void);
extern void RestoreActorPaletteCycle(void);
extern void SaveActorPaletteCycle(void);
extern void UpdateActorPaletteCycle(void);
extern void SetActorPaletteCycle(s32 idx);
extern void EnableActorPaletteCycle(u8 flag);

/* src/actor/actor_anim.cpp: the C names of AnimPart's methods
 * (actor_self.hpp) the yeti's C files call */
extern s32 GetAnimFrameBaseOffset(struct actor_self *self);
extern void SetActorAnim(struct actor_self *self, s32 idx);

/* src/actor/actor_bg.c */
extern void ShakeActorBg(s32 arg0);
extern void SetActorBgLayerDepth(s32 arg0);
extern s32 GetActorBgLayerDepth(void);
extern void ActorCategoryEndStub(void);
extern void CommitActorBgScroll(void);
extern s32 GetActorBgCenterY(void);
extern s32 GetActorBgCenterX(void);

/* src/actor/actor_category_frame.cpp */
extern s32 RunActorCategoryFrame(void);

/* src/actor/actor_category_init.c */
extern s32 InitActorCategory(s32 category);

/* src/actor/actor_category_select.cpp */
extern void SelectActorCategory(s32 type, struct sub_effect_record *table, void *animTable,
                                u8 active, s32 variant, s32 checkpoint);

/* src/actor/actor_category_stats.c */
extern s32 CountCategoryCrates(s32 categoryIdx);
extern void AddActorMissedNitro(void);
extern s32 GetActorMissedNitros(void);
extern s32 GetActorCheckpoint(void);

/* src/actor/actor_factory.cpp */
#ifdef __cplusplus
extern class ActorSelf *CreateActor(u8 kind, s32 x, s32 y, s32 z, void *spawn);
extern class ActorSelf *SpawnActor(struct actor_spawn *spawn, u8 useBonus, s32 zOffset);
#else
extern struct actor_self *CreateActor(u8 kind, s32 x, s32 y, s32 z, void *spawn);
extern struct actor_self *SpawnActor(struct actor_spawn *spawn, u8 useBonus, s32 zOffset);
#endif
extern void ConstructAnimTableState(struct anim_table_record *table, s32 z);

/* src/actor/actor_spawn.cpp */
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
#ifdef __cplusplus
extern class ActorSelf **gActorDrawList;
#else
extern struct actor_self **gActorDrawList;
#endif
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
#ifdef __cplusplus
extern void (*gHeapSortActorsByKeyFunc)(s32 n, class ActorSelf **list);
#else
extern void (*gHeapSortActorsByKeyFunc)(s32 n, struct actor_self **list);
#endif

/* src/data/palette_cycle_175760.c */
extern const u16 gActorPaletteCycleFrames[32][14 * 16];
extern const s32 gActorPaletteCycleStartFrames[4];
extern const s32 gActorPaletteCycleTargetFrames[4];

#endif /* !__ACTOR_H__ */
