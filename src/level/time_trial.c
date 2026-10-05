#include "core.h"
#include "actor.h"
#include "pickups.h"

/* GitHub issue #34, UpdateGameFrame-MainLoop cluster (docs/rom_map.md).
 * Built with old_agbcc - see docs/matching/game-loop-old-agbcc.md. */

struct anim_record
{
    u8 unk_00[0x14];
    u8 tileRecord;              // 0x14
    u8 unk_15[7];
};

struct anim_table
{
    struct anim_record *records;
};

/* An OAM-backed part (same layout as cortex.c's gfx_part). */
struct slot_part
{
    struct actor base;          // 0x00
    u8 unk_1C[4];
    struct anim_table *anim;    // 0x20
    u8 unk_24[5];
    u8 frameNibble:4;           // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;                     // 0x2D
};

struct vmethod
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct entity_vtable
{
    u8 unk_00[0x28];
    struct vmethod m28;         // 0x28
    u8 unk_30[0x18];
    struct vmethod m48;         // 0x48
};

struct entity
{
    struct actor base;          // 0x00 (+0x08: id, +0x0C: flags)
    u8 unk_1C[0];
};

struct level_obj
{
    u8 unk_00[8];
    s32 state;                  // 0x08
};

struct level_state
{
    u8 unk_00[0x8C];
    u8 timeTrial;                  // 0x8C
    u8 unk_8D[3];
    s32 unk_90[5];              // 0x90-0xA0
    u8 unk_A4[0x38];
    struct level_obj *level;    // 0xDC
    u8 unk_E0[0xD8];
    struct slot_part *bonusPlatform;    // 0x1B8
    struct slot_part *gemPlatform;    // 0x1BC
};

struct entity_list
{
    u8 unk_00[4];
    s32 count;                  // 0x04
    u8 unk_08[4];
    struct actor **items;       // 0x0C
};

struct collision_map
{
    u8 unk_000[0x108];
    u32 seen[1];                // 0x108
};

extern void *gPaletteCache;
extern struct entity_list *gUnknown_030012EC;
extern struct collision_map *gEntityFlags;

extern void SetMaskLevel(struct level_state *self, s32 arg1);
extern void ResetSpriteFrameTimer(struct slot_part *part);
extern void ResetSpriteFrameIndex(struct slot_part *part);
extern void SetSpriteAnimDone(struct slot_part *part, s32 arg);
extern void LoadPaletteSlot(void *cache, s32 palette, u8 record);
extern void ConvertCratesForTimeTrial(void);
extern s32 _call_via_r1(void *self, void *fn);

static inline void SetPartTag(struct slot_part *part, s32 tag)
{
    part->tag = tag;
}

#define ACTOR_METHOD(a, m) \
    _call_via_r1((u8 *)(a) + ((struct entity_vtable *)(a)->table)->m.thisOffset, \
                ((struct entity_vtable *)(a)->table)->m.fn)

/* Level restart: resets the level state's +0x8c/+0x90 fields, then
 * (unless the level object is already in state 3) retags and restarts
 * both slot parts, reloads slot B's tiles, runs ConvertCratesForTimeTrial, and
 * re-registers every entity in gUnknown_030012EC whose +0x48 method
 * returns 2: those whose +0x28 method fails are despawned, the rest are
 * flagged seen and marked in the collision map's seen bitmap. */
void StartTimeTrial(struct level_state *self)
{
    struct slot_part *part;
    s32 i;

    SetMaskLevel(self, 0);
    self->timeTrial = 1;
    self->unk_90[0] = 0;
    self->unk_90[1] = 0;
    self->unk_90[2] = 0;
    self->unk_90[3] = 0;
    self->unk_90[4] = 0;
    if (self->level->state == 3)
        return;

    part = self->bonusPlatform;
    if (part != NULL)
    {
        SetPartTag(part, 7);
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
    }
    part = self->gemPlatform;
    if (part != NULL)
    {
        SetPartTag(part, 0xc);
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        LoadPaletteSlot(gPaletteCache, self->gemPlatform->frameNibble,
                    self->gemPlatform->anim->records[self->gemPlatform->tag].tileRecord);
    }
    ConvertCratesForTimeTrial();

    i = 0;
    if (i < gUnknown_030012EC->count)
    {
        do
        {
            struct actor *e = gUnknown_030012EC->items[i];
            struct actor *a = e;
            struct vmethod *m = &((struct entity_vtable *)e->table)->m48;

            if (_call_via_r1((u8 *)e + m->thisOffset, m->fn) == 2)
            {
                if ((u8)ACTOR_METHOD(e, m28))
                    PickUpWumpa((struct orbit_part *)e, 1);
                else
                {
                    a->flags |= 1;
                    if (a->field_08 != 0xffff)
                    {
                        s32 id = a->field_08;
                        gEntityFlags->seen[id / 32] |= 1 << (id % 32);
                    }
                }
            }
        } while (++i < gUnknown_030012EC->count);
    }
}
