#include "core.h"
#include "actor.h"

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

/* An OAM-backed part (same layout as actor_part_188d0.c's gfx_part). */
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
    u8 unk_8C;                  // 0x8C
    u8 unk_8D[3];
    s32 unk_90[5];              // 0x90-0xA0
    u8 unk_A4[0x38];
    struct level_obj *level;    // 0xDC
    u8 unk_E0[0xD8];
    struct slot_part *slotA;    // 0x1B8
    struct slot_part *slotB;    // 0x1BC
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

extern void *gUnknown_030012B8;
extern struct entity_list *gUnknown_030012EC;
extern struct collision_map *gEntityFlags;

extern void SetMaskLevel(struct level_state *self, s32 arg1);
extern void sub_80087C0(struct slot_part *part);
extern void sub_80087B4(struct slot_part *part);
extern void sub_800872C(struct slot_part *part, s32 arg);
extern void sub_8006D08(void *cache, s32 palette, u8 record);
extern void sub_8010804(void);
extern s32 _call_via_r1(void *self, void *fn);
extern void sub_8011448(struct actor *self, s32 arg1);

static inline void SetPartTag(struct slot_part *part, s32 tag)
{
    part->tag = tag;
}

#define ACTOR_METHOD(a, m) \
    _call_via_r1((u8 *)(a) + ((struct entity_vtable *)(a)->table)->m.thisOffset, \
                ((struct entity_vtable *)(a)->table)->m.fn)

/* Level restart: resets the level state's +0x8c/+0x90 fields, then
 * (unless the level object is already in state 3) retags and restarts
 * both slot parts, reloads slot B's tiles, runs sub_8010804, and
 * re-registers every entity in gUnknown_030012EC whose +0x48 method
 * returns 2: those whose +0x28 method fails are despawned, the rest are
 * flagged seen and marked in the collision map's seen bitmap. */
void sub_8022D50(struct level_state *self)
{
    struct slot_part *part;
    s32 i;

    SetMaskLevel(self, 0);
    self->unk_8C = 1;
    self->unk_90[0] = 0;
    self->unk_90[1] = 0;
    self->unk_90[2] = 0;
    self->unk_90[3] = 0;
    self->unk_90[4] = 0;
    if (self->level->state == 3)
        return;

    part = self->slotA;
    if (part != NULL)
    {
        SetPartTag(part, 7);
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
    }
    part = self->slotB;
    if (part != NULL)
    {
        SetPartTag(part, 0xc);
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
        sub_8006D08(gUnknown_030012B8, self->slotB->frameNibble,
                    self->slotB->anim->records[self->slotB->tag].tileRecord);
    }
    sub_8010804();

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
                    sub_8011448(e, 1);
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
