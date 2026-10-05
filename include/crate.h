#ifndef GUARD_CRATE_H
#define GUARD_CRATE_H

#include "gobj_1a794.h"

/* The crate (CreateCrate, gCrateVtable): the object the issue #12
 * "physics/collision" cluster (ROM 0x0800D040-0x0800FC70,
 * src/system/game_loop6.c/7.c/47.c/48.c/49.c) turned out to be. `kind`
 * is the crate type CreateCrate picks (0 plain, 1 checkpoint, 2 Aku Aku,
 * 3 iron "!", 4 arrow, 5 outline, 6 nitro switch, 7 iron, 8 iron arrow,
 * 9 life, 10 nitro, 11 "?", 12 bouncy wumpa, 14 TNT, 16-18 time crates,
 * 19-21 lit TNT). Only the fields those functions touch are named;
 * the head (position, flags, anim table/tag, mirror bits) has the same
 * layout as `struct gobj`. See docs/matching/issue-12-physics-collision.md. */
struct crate_vtable
{
    u8 unk_00[0x18];
    struct method m18; // 0x18 - slot 3, the per-frame update (UpdateCrate)
    u8 unk_20[0x28];
    struct method m48; // 0x48 - returns the object's class id (3: box)
    struct method m50; // 0x50
    u8 unk_58[8];
    struct method m60; // 0x60
    struct method m68; // 0x68
};

struct crate;

/* UpdateSlotCrate's view of crate.u48: this compiler pads the struct to a
 * word, so a copy of it lives in one register and its bitfields are
 * updated with word-sized masks. */
struct phys_b48
{
    u8 phase:3;
    u8 cnt:3;
    u8 dir:2;
};


/* Bit view of crate.flags (a separate struct: this compiler pads
 * every struct to a word, so it can't be embedded). */
struct phys_flag_bits
{
    u8 gone:1;          // removed (see sub_80072D8)
    u8 unk_1:3;
    u8 bit4:1;          // set by BreakCrate (also `flags |= 0x10` elsewhere)
    u8 unk_5:3;
};

#define PHYS_GONE(obj) (((struct phys_flag_bits *)&(obj)->flags)->gone)
#define PHYS_FLAG4(obj) (((struct phys_flag_bits *)&(obj)->flags)->bit4)

/* A group of objects that trigger together (ActivateIronSwitchCrate builds it). */
struct crate_group
{
    s32 count;
    struct crate *items[0];
};

#define PHYS_NO_GROUP ((struct crate_group *)-1)
#define PHYS_HAS_GROUP(g) ((u32)(g) + 1 > 1)

struct crate
{
    s32 x;              // 0x00
    s32 y;              // 0x04
    u16 id;             // 0x08 - 0xffff: none
    u8 unk_0A[2];
    u8 flags;           // 0x0C - bit 0: removed, see PHYS_GONE
    u8 unk_0D[0xB];
    struct crate_vtable *vtable; // 0x18
    u8 unk_1C[4];
    struct anim_table *anim; // 0x20
    u8 unk_24[4];
    u32 unk_28_0:4;     // 0x28
    s32 flipX:1;        //      bit 4: X mirrored
    s32 flipY:1;        //      bit 5: Y mirrored
    u32 unk_28_6:2;
    u32 slot:4;         // 0x29 - palette/tile slot
    u32 unk_29_4:4;
    u32 unk_2A:16;
    u8 animating;       // 0x2C - nonzero while the keyframe timer runs (box_part.h)
    u8 tag;             // 0x2D
    u8 unk_2E[2];
    s32 frame;          // 0x30
    s32 stepTimer;      // 0x34 - ticks spent on the current step (FinishBrokenCrate blasts on its first tick)
    u8 animDone;        // 0x38 - set once the animation ends; UpdateCrate then resets `frame`
                        //        and clears the busy bit
    u8 unk_39[7];
    s32 fallTargetY;    // 0x40 - Q8 y the crate lands at (DropCratesAbove sets it; UpdateCrateFall
                        //        snaps `y` to it when the fall ends)
    s32 fallDistance;   // 0x44 - Q8 distance still to fall, 0: resting
    union {
        s32 n;
        struct crate_group *group; // NULL or PHYS_NO_GROUP: none
        struct phys_b48 b;
    } u48;              // 0x48
    s8 unk_4C;          // 0x4C
    u8 state;           // 0x4D - low 7 bits: state (1: committed), bit 7: busy
    u8 kind;            // 0x4E - index into the gStaticData_0816BB** tables
    u8 timer;           // 0x4F
    u8 unk_50;          // 0x50
    u8 unk_51;          // 0x51
    u8 unk_52[2];
    s32 unk_54;         // 0x54
    u8 touched;         // 0x58
    u8 groupAllocated;  // 0x59 - ActivateIronSwitchCrate allocated `u48.group` (freed when it fires)
};

/* The fields of the player object (gPlayer, a `struct gobj`)
 * this cluster uses: a 5-slot ring of recently touched boxes. */
struct phys_player
{
    s32 x;              // 0x00
    s32 y;              // 0x04
    u8 unk_08[0x10];
    struct gobj_vtable *vtable; // 0x18
    u8 unk_1C[8];
    u8 dir;             // 0x24 - bit 2: blocks the landing checks
    u8 unk_25[0x2F];
    s32 velX;           // 0x54
    s32 velY;           // 0x58
    s32 velZ;           // 0x5C
    u8 unk_60[4];
    s32 speedY;         // 0x64
    u8 hitAxes;         // 0x68 - struct gobj.hitAxes; 8: standing (on `carried`)
    u8 unk_69[0xB];
    u32 hitMask;        // 0x74
    u8 unk_78[8];
    u8 busy;            // 0x80
    u8 unk_81[7];
    u8 ctrlMode;        // 0x88 - struct gobj.ctrlMode; 1: crates fall at quarter speed, touched
                        //        enemies just vanish; nonzero stops the ring recording
    u8 unk_89[8];
    u8 handled;         // 0x91
    u8 bounce;          // 0x92
    u8 unk_93;
    u8 ringCount;       // 0x94
    u8 unk_95[3];
    struct crate *ring[5]; // 0x98
    struct crate *carried; // 0xAC
    u8 unk_B0[0x5C];
    u8 unk_10C;         // 0x10C - nonzero: sub_800E08C leaves the position alone
};

#define PHYS_PLAYER ((struct phys_player *)gPlayer)

/* gCrateList: the active-object list these functions scan. */
struct crate_list
{
    s32 count;
    s32 capacity;
    struct crate **items;
};

/* gUnknown_030012EC, the second object list BlastNearbyCrates scans. */
struct phys_obj_list2
{
    s32 unk_00;
    s32 count;
    s32 unk_08;
    struct crate **items;
};

typedef s32 (*phys_method_fn)(void *self);

/* Calls method `m` (a gcc 2.x {s16 thisOffset; fn} vtable slot) on `obj`. */
static inline s32 PhysCall(void *obj, struct method *m)
{
    return ((phys_method_fn)m->fn)((u8 *)obj + m->thisOffset);
}
#define PHYS_CALL(obj, m) PhysCall((obj), &(obj)->vtable->m)

typedef void (*phys_method1_fn)(void *self, s32 arg);

static inline void PhysCall1(void *obj, struct method *m, s32 arg)
{
    ((phys_method1_fn)m->fn)((u8 *)obj + m->thisOffset, arg);
}
#define PHYS_CALL1(obj, m, arg) PhysCall1((obj), &(obj)->vtable->m, (arg))

typedef void (*phys_method3_fn)(void *self, s32 a, s32 b, s32 c);

static inline void PhysCall3(void *obj, struct method *m, s32 a, s32 b, s32 c)
{
    ((phys_method3_fn)m->fn)((u8 *)obj + m->thisOffset, a, b, c);
}

/* Switches `self` to animation tag `tag` and refreshes its sprite - the
 * three-call idiom every state change in this cluster uses. */
static inline void PhysSetTag(struct crate *self, u8 tag)
{
    self->tag = tag;
    ResetSpriteFrameTimer(self);
    ResetSpriteFrameIndex(self);
    SetSpriteAnimDone(self, 0);
}

/* Sets `frame` to `idx`, clamped to the current tag's frame count. `idx`
 * being a parameter matters: the inlined copy keeps the constant
 * argument in its own register, which the callers' later zero/constant
 * stores reuse (BreakCrate, UpdateCrate). */
static inline void PhysSetFrame(struct crate *obj, s32 idx)
{
    u8 n = obj->anim->records[obj->tag].frames;

    if (idx >= n)
        idx = n - 1;
    obj->frame = idx;
}

/* Sets bit `id` of the gEntityFlags+0x108 bitmap - the same
 * sequence (and the same do/while(0) trick) as actor_part_16048.c's
 * SET_ID_BIT. */
#define PHYS_SET_ID_BIT(idExpr)                                                \
    do                                                                         \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = gEntityFlags;                                         \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= 1 << (_id - _word * 32);                                     \
    } while (0)

#endif // GUARD_CRATE_H
