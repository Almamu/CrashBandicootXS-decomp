#include "core.h"
#include "memory.h"
#include "actor_anim.h"
#include "actor_self.h"

/* 0x0802AC28-0x0802B364 (GitHub issue #51), formerly
 * asm/code_3_2_20_8b7c_ac28.s: the actor factory on top of the shared
 * `struct actor_self` object (include/actor_self.h).
 *
 * - ConstructAnimTableState (category vtable slot 0, see docs/rom_map.md)
 *   installs the category's animation table (`struct anim_table_record`,
 *   include/actor_anim.h) as gActorAnimTable and builds the player with
 *   ConstructActorPart, which also resets the gUnknown_03001480-030014A4
 *   player-state globals.
 * - SpawnActor (vtable slot 1) turns a level spawn record into a
 *   CreateActor call, picking the record's alternate kind in the
 *   gLevelState+0x8C mode (with several kinds folded to 1) or its
 *   bonus kind when asked to, and skipping kinds 0/32-34/62.
 * - CreateActor is the per-kind `new`: a switch whose case bodies are
 *   the inlined constructors of each actor class (allocate, run the
 *   base constructor, install the method table at +0x50); kinds 36-39
 *   only select a palette-cycle preset (sub_802ABC8).
 * - sub_802B12C/sub_802B174/sub_802B1A8 construct three fixed records
 *   (40, 11, 27) the same way.
 *
 * Matches under either compiler (nothing here tells them apart); built
 * with the current agbcc like the rest of this zone. See
 * docs/matching/issue-51-actor-2ac28.md. */

extern struct anim_table_record *gActorAnimTable;
extern struct actor_self *gActorList;
extern void *gLevelState;

extern u8 gStaticData_087E4E14[];
extern u8 gStaticData_087E4E34[];
extern u8 gStaticData_087E4E54[];
extern u8 gStaticData_087E4E94[];
extern u8 gStaticData_087E4EB4[];
extern u8 gStaticData_087E4ED4[];
extern u8 gStaticData_087E4EF4[];
extern u8 gStaticData_087E4F14[];
extern u8 gStaticData_087E4F34[];
extern u8 gStaticData_087E4F54[];
extern u8 gStaticData_087E4F74[];

extern struct actor_self *InitActorPart(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802CB34(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802D764(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802CF0C(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802D648(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802D0C8(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z, void *spawn);
extern struct actor_self *sub_802CDE4(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802D1B8(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802D5D4(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802CE38(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802C3E8(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z);
extern struct actor_self *sub_802D528(struct actor_self *self, struct anim_table_record *rec, s32 x, s32 y, s32 z, s32 arg);
extern u8 sub_802AA80(void *spawn);
extern void sub_802ABC8(s32 idx);
extern void sub_802B864(struct actor_self *self);

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
        sub_802CB34(self, (rec), x, y, z);                                     \
        self->vtable = (struct actor_vtable *)(vt);                            \
        return self;                                                           \
    }

#define NEW_CB34_TRACKED_ACTOR(rec, vt)                                        \
    {                                                                          \
        struct actor_tracked *self = (struct actor_tracked *)AllocActor(0x58);  \
        sub_802CB34(&self->base, (rec), x, y, z);                              \
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
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gStaticData_087E4EF4);
    case 3:
        return sub_802D764(AllocActor(0x54), gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    case 5:
    case 6:
    case 7:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gStaticData_087E4EB4);
    case 1:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gStaticData_087E4F74);
    case 4:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gStaticData_087E4F14);
    case 22:
        return sub_802CF0C(AllocActor(0x54), gActorAnimTable + kind, x, y, z);
    case 12:
        return sub_802D648(AllocActor(0x58), gActorAnimTable + kind, x, y, z);
    case 24:
        return sub_802D0C8(AllocActor(0x68), gActorAnimTable + kind, x, y, z, spawn);
    case 28:
    case 29:
    case 30:
    case 31:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gStaticData_087E4ED4);
    case 35:
        if (sub_802AA80(spawn))
        {
            NEW_CB34_ACTOR(0x54, gActorAnimTable + 28, gStaticData_087E4ED4);
        }
        NEW_CB34_TRACKED_ACTOR(REC_AT(kind), gStaticData_087E4F34);
    case 8:
        if (sub_802AA80(spawn))
        {
            NEW_CB34_ACTOR(0x54, gActorAnimTable + 28, gStaticData_087E4ED4);
        }
        NEW_CB34_TRACKED_ACTOR(REC_AT(kind), gStaticData_087E4F34);
    case 10:
        NEW_CB34_ACTOR(0x54, REC_AT(kind), gStaticData_087E4F54);
    case 11:
        NEW_BASE_ACTOR(REC_AT(kind), gStaticData_087E4E94);
    case 23:
        return sub_802CDE4(AllocActor(0x54), gActorAnimTable + kind, x, y, z);
    case 16:
    case 18:
    case 20:
        return sub_802D1B8(AllocActor(0x54), gActorAnimTable + kind, x, y, z);
    case 25:
    {
        struct actor_self *self = sub_802D5D4(AllocActor(0x54), gActorAnimTable + 26, gActorAnimTable[26].spawnX, y, z);

        SET_ANIM(self, 1);
        return sub_802D5D4(AllocActor(0x54), gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    }
    case 13:
    {
        struct actor_self *self = sub_802CE38(AllocActor(0x54), gActorAnimTable + 14, gActorAnimTable[14].spawnX, y, z);

        SET_ANIM(self, 1);
        return sub_802CE38(AllocActor(0x54), gActorAnimTable + kind, gActorAnimTable[kind].spawnX, y, z);
    }
    case 2:
        NEW_BASE_ACTOR(REC_AT(kind), gStaticData_087E4E14);
    case 36:
    case 37:
    case 38:
    case 39:
        sub_802ABC8(kind - 36);
        break;
    }
    return NULL;
}

void sub_802B12C(s32 x, s32 y, s32 z)
{
    struct actor_self *self = AllocActor(0x54);

    InitActorPart(self, &gActorAnimTable[40], x, y, z);
    self->vtable = (struct actor_vtable *)gStaticData_087E4E34;
}

void sub_802B174(s32 x, s32 y, s32 z)
{
    sub_802C3E8(AllocActor(0x60), &gActorAnimTable[11], x, y, z);
}

struct actor_self *sub_802B1A8(s32 x, s32 y, s32 z, s32 arg)
{
    return sub_802D528(AllocActor(0x54), &gActorAnimTable[27], x, y, z, arg);
}

extern struct actor_self *ConstructActorPart(struct actor_self *self, struct anim_table_record *rec, s32 z);

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

    if (((u8 *)gLevelState)[0x8C] != 0)
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

extern s32 gUnknown_030014A4;
extern s32 gUnknown_03001490;
extern s32 gUnknown_03001494;
extern u8 gUnknown_030014A3;
extern s32 gUnknown_0300149C;
extern s32 gUnknown_03001498;
extern s32 gUnknown_0300148C;
extern u8 gUnknown_030014A0;
extern u8 gUnknown_030014A2;
extern u8 gUnknown_030014A1;
extern s32 gUnknown_03001488;
extern s32 gUnknown_03001484;
extern u8 gUnknown_03001480;

struct actor_self *ConstructActorPart(struct actor_self *self, struct anim_table_record *rec, s32 z)
{
    InitActorPart(self, rec, 0, z != 0 ? 0x2800 : -0x5000, z);
    self->vtable = (struct actor_vtable *)gStaticData_087E4E54;
    sub_802B864(self);
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
    gUnknown_030014A4 = 0;
    gUnknown_03001490 = 0;
    gUnknown_03001494 = 0;
    gUnknown_030014A3 = 0;
    gUnknown_0300149C = 0;
    gUnknown_03001498 = 0;
    gUnknown_0300148C = 0;
    gUnknown_030014A0 = 1;
    gUnknown_030014A2 = 0;
    gUnknown_030014A1 = 0;
    gUnknown_03001488 = 0;
    gUnknown_03001484 = 0;
    gUnknown_03001480 = 0;
    return self;
}
