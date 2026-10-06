#include "core.h"
#include "memory.h"
#include "actor_anim.h"
#include "actor_self.h"
#include "actor.h"
#include "bosses.h"
#include "vehicle.h"
#include "level_state.h"
#include "globals.h"

/* 0x0802AC28-0x0802B364 (GitHub issue #51), formerly
 * asm/code_3_2_20_8b7c_ac28.s: the actor factory on top of the shared
 * `struct actor_self` object (include/actor_self.h).
 *
 * - ConstructAnimTableState (category vtable slot 0, see docs/rom_map.md)
 *   installs the category's animation table (`struct anim_table_record`,
 *   include/actor_anim.h) as gActorAnimTable and builds the player with
 *   ConstructActorPart, which also resets the gPolarPauseLocked-030014A4
 *   player-state globals.
 * - SpawnActor (vtable slot 1) turns a level spawn record into a
 *   CreateActor call, picking the record's alternate kind in the
 *   gLevelState+0x8C mode (with several kinds folded to 1) or its
 *   bonus kind when asked to, and skipping kinds 0/32-34/62.
 * - CreateActor is the per-kind `new`: a switch whose case bodies are
 *   the inlined constructors of each actor class (allocate, run the
 *   base constructor, install the method table at +0x50); kinds 36-39
 *   only select a palette-cycle preset (SetActorPaletteCycle).
 * - CreatePolarCheckpointText/SpawnPolarCollectedWumpa/SpawnPolarAkuAku construct three fixed records
 *   (40, 11, 27) the same way.
 *
 * Matches under either compiler (nothing here tells them apart); built
 * with the current agbcc like the rest of this zone. See
 * docs/matching/archive/issue-51-actor-2ac28.md. */

/* An inline wrapper rather than a macro: the ROM materializes the size
 * before the heap flags, i.e. evaluates it as an argument of its own. */
static inline struct actor_self *AllocActor(u32 size)
{
    return (struct actor_self *)mem_alloc(size, MEM_HEAP_IWRAM);
}

/* `&gActorAnimTable[i]` with the index scaled before the table pointer
 * is loaded, as the ROM's inlined constructors compute it (plain
 * `&gActorAnimTable[i]` loads the pointer first). */
#define REC_AT(i) ((struct anim_table_record *)((i) * sizeof(struct anim_table_record) + (u32)gActorAnimTable))

/* The inlined `new Foo(x, y, z)` bodies of CreateActor's switch: allocate,
 * run the base constructor, install the class's method table. */
#define NEW_CB34_ACTOR(size, rec, vt)                                          \
    {                                                                          \
        struct actor_self *self = AllocActor(size);                             \
        InitPolarCrate(self, (rec), x, y, z);                                     \
        self->vtable = (struct actor_vtable *)(vt);                            \
        return self;                                                           \
    }

#define NEW_CB34_TRACKED_ACTOR(rec, vt)                                        \
    {                                                                          \
        struct actor_tracked *self = (struct actor_tracked *)AllocActor(0x58);  \
        InitPolarCrate(&self->base, (rec), x, y, z);                              \
        self->base.vtable = (struct actor_vtable *)(vt);                       \
        self->spawn = spawn;                                                   \
        return &self->base;                                                    \
    }

#define NEW_BASE_ACTOR(rec, vt)                                                \
    {                                                                          \
        struct actor_self *self = AllocActor(0x54);                             \
        InitActorPart(self, (rec), x, y, z);                                   \
        self->vtable = (struct actor_vtable *)(vt);                            \
        return self;                                                           \
    }

/* Restarts animation sequence `idx` without touching the state. */
#define SET_ANIM(self, idx)                                                    \
    {                                                                          \
        (self)->animIndex = (idx);                                             \
        (self)->animTimer = (self)->anims[idx].duration;                       \
        (self)->animDone = 0;                                                  \
        (self)->animTime = 0;                                                  \
    }

/* A spawner-linked actor (CreateActor kinds 8/35): remembers the spawn
 * record that created it. */
struct actor_tracked
{
    struct actor_self base;
    void *spawn;                // 0x54
};

struct actor_self *CreateActor(u8 kind, s32 x, s32 y, s32 z, void *spawn)
{
    x += gActorAnimTable[kind].spawnX;
    y += gActorAnimTable[kind].spawnY;

    switch (kind)
    {
    case 9:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gPolarAkuAkuCrateVtable);
    case 3:
        return CreatePolarCheckpointCrate(AllocActor(0x54), gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case 5:
    case 6:
    case 7:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gPolarTimeCrateVtable);
    case 1:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gPolarBasicCrateVtable);
    case 4:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gPolarNitroCrateVtable);
    case 22:
        return CreatePolarLauncher(AllocActor(0x54), gActorAnimTable + kind, x, y, z);
    case 12:
        return CreatePolarBoostPad((struct actor_once *)(AllocActor(0x58)), gActorAnimTable + kind, x, y, z);
    case 24:
        return CreatePolarPenguin(AllocActor(0x68), gActorAnimTable + kind, x, y, z, spawn);
    case 28:
    case 29:
    case 30:
    case 31:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gPolarQuestionCrateVtable);
    case 35:
        if ((u8)IsSpawnCollected(spawn))
        {
            NEW_CB34_ACTOR(0x54, gActorAnimTable + 28, gPolarQuestionCrateVtable);
        }
        NEW_CB34_TRACKED_ACTOR(REC_AT(kind), gPolarLifeCrateVtable);
    case 8:
        if ((u8)IsSpawnCollected(spawn))
        {
            NEW_CB34_ACTOR(0x54, gActorAnimTable + 28, gPolarQuestionCrateVtable);
        }
        NEW_CB34_TRACKED_ACTOR(REC_AT(kind), gPolarLifeCrateVtable);
    case 10:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gStaticData_087E4F54);
    case 11:
        NEW_BASE_ACTOR(REC_AT(kind), gPolarWumpaVtable);
    case 23:
        return CreatePolarElectricFence(AllocActor(0x54), gActorAnimTable + kind, x, y, z);
    case 16:
    case 18:
    case 20:
        return CreatePolarIcicle(AllocActor(0x54), (u8 *)(gActorAnimTable + kind), x, y, z);
    case 25:
    {
        struct actor_self *self = CreatePolarGoal(AllocActor(0x54), gActorAnimTable + 26, gActorAnimTable[26].spawnX, y, z);

        SET_ANIM(self, 1);
        return CreatePolarGoal(AllocActor(0x54), gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    }
    case 13:
    {
        struct actor_self *self = sub_802CE38(AllocActor(0x54), gActorAnimTable + 14, gActorAnimTable[14].spawnX, y, z);

        SET_ANIM(self, 1);
        return sub_802CE38(AllocActor(0x54), gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    }
    case 2:
        NEW_BASE_ACTOR(REC_AT(kind), gRiderlessPolarVtable);
    case 36:
    case 37:
    case 38:
    case 39:
        SetActorPaletteCycle(kind - 36);
        break;
    }
    return NULL;
}

void CreatePolarCheckpointText(s32 x, s32 y, s32 z)
{
    struct actor_self *self = AllocActor(0x54);

    InitActorPart(self, &gActorAnimTable[40], x, y, z);
    self->vtable = (struct actor_vtable *)gPolarCheckpointTextVtable;
}

void SpawnPolarCollectedWumpa(s32 x, s32 y, s32 z)
{
    CreatePolarCollectedWumpa(AllocActor(0x60), &gActorAnimTable[11], x, y, z);
}

struct actor_self *SpawnPolarAkuAku(s32 x, s32 y, s32 z, s32 arg)
{
    return CreatePolarAkuAku(AllocActor(0x54), &gActorAnimTable[27], x, y, z, arg);
}

void ConstructAnimTableState(struct anim_table_record *table, s32 z)
{
    gActorAnimTable = table;
    gActorList = NULL;
    gActorList = ConstructActorPart(AllocActor(0x54), gActorAnimTable, z);
}

struct actor_spawn
{
    u8 kind;
    u8 altKind;
    u8 bonusKind;
    u8 unk_03;
    s32 x;
    s32 y;
    s32 z;
};

struct actor_self *SpawnActor(struct actor_spawn *spawn, u8 useBonus, s32 zOffset)
{
    u8 kind = spawn->kind;

    if (gLevelState->timeTrial != 0)
    {
        kind = spawn->altKind;
        if (kind == 11)
            return NULL;
        if (kind == 3 || kind == 8 || kind == 28 || kind == 29 || kind == 30 || kind == 31 || kind == 35)
            kind = 1;
    }
    else if (useBonus)
    {
        kind = spawn->bonusKind;
    }
    if (kind == 0 || kind == 32 || kind == 33 || kind == 34 || kind == 62)
        return NULL;
    return CreateActor(kind, spawn->x << 8, spawn->y << 8, (spawn->z << 8) + zOffset, spawn);
}

struct actor_self *ConstructActorPart(struct actor_self *self, struct anim_table_record *rec, s32 z)
{
    InitActorPart(self, rec, 0, z != 0 ? 0x2800 : -0x5000, z);
    self->vtable = (struct actor_vtable *)gPolarPlayerVtable;
    AllocPolarPlayerTiles(self);
    if (self->z != 0)
    {
        ACTOR_SET_STATE(self, 0xD, 0xC);
    }
    else
    {
        s32 idx = 8;

        self->animIndex = idx;
        self->animTimer = self->anims[idx].duration;
        self->animDone = 0;
        self->animTime = 0;
    }
    gPolarPlayerVelY = 0;
    gRiderlessPolar = 0;
    gPolarAkuAku = 0;
    gPolarSteerEnabled = 0;
    gPolarInvulnTimer = 0;
    gPolarSteerTime = 0;
    gPolarFinishTimer = 0;
    gPolarPlayerInactive = 1;
    gPolarFadeStarted = 0;
    gPolarPlayerHalted = 0;
    gPolarQueuedWumpa = 0;
    gPolarWumpaDispenseTimer = 0;
    gPolarPauseLocked = 0;
    return self;
}
