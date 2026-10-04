#include "core.h"

/* GitHub issue #24: 0x0801967C-0x0801A794, formerly
 * asm/code_3_2_17_188d0_1967c.s.
 *
 * Small actor-part "controller" classes, each with a method table at
 * self+0x0C (per-frame update in slot +0x0C, destructor in slot +0x4C)
 * and a constructor/destructor pair here: gStaticData_087E4704
 * (sub_80196F8/sub_80196E4), 087E476C (sub_8019758/sub_8019744, update
 * sub_8019730), 087E47D4 (sub_80197DC/sub_80197C8), 087E483C
 * (sub_801A724/sub_801A73C, update sub_801A64C), 087E48A4
 * (sub_801A768/sub_801A750, update sub_801A2A8), 087E490C (destructor
 * sub_801A780, update sub_801A114) and 087E4974 (update sub_80197F8).
 *
 * The 087E4974 object is a boss-like state machine: sub_80197F8 steers
 * its part along the level (approach tables gStaticData_0816C368/78/90/
 * A0, turning round at either level edge), counts hits the player lands
 * on it, and spawns helper parts through sub_8019EBC/sub_801A03C;
 * sub_8019CE4 is its "enter state N" transition (animation + follow-up
 * spawns/sounds). The last state signals sub_80241A4 (the "entity ready"
 * barrier, see docs/rom_map.md) once the part falls off the bottom.
 *
 * This file is compiled with tools/agbcc/bin/old_agbcc (see Makefile and
 * docs/matching/issue-24-boss-actor.md): the old compiler reproduces
 * this region's "constant before the byte it's combined with" ordering
 * without register pins. The code was C++: virtual calls are indirect
 * calls through libgcc's `_call_via_rN` helpers (aliased below to this
 * ROM's copies), and the AABB builders return their box by value.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: sub_8019718, sub_80197F4. Matched
 * anyway.
 *
 * sub_801A114 is a NAKED transcription (its C is kept under
 * NON_MATCHING); everything else is real C. */

struct vmethod
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct vtable
{
    u8 unk_00[0x18];
    struct vmethod m18; // 0x18
    struct vmethod m20; // 0x20
    struct vmethod m28; // 0x28
    struct vmethod m30; // 0x30
    struct vmethod m38; // 0x38
    struct vmethod m40; // 0x40
    struct vmethod m48; // 0x48
    struct vmethod m50; // 0x50
    struct vmethod m58; // 0x58
    struct vmethod m60; // 0x60
    struct vmethod m68; // 0x68
};

struct anim_rec
{
    u8 unk_00[0x16];
    u8 frameCount; // 0x16
    u8 unk_17[5];
};

struct box
{
    s32 unk_00;
    s32 unk_04;
    s32 valid; // 0x08
    s32 unk_0C;
};

/* The bitfield byte at part+0x28. `facing` is a signed field: the ROM
 * tests it with `lsl #27` / sign branch. */
struct part_f28
{
    u8 mode:2;
    u8 unk_2:2;
    s32 facing:1;
    u32 flag5:1;
    u8 unk_6:2;
} __attribute__((packed));

struct part
{
    s32 x;                 // 0x00
    s32 y;                 // 0x04
    u16 id;                // 0x08
    u8 unk_0A;             // 0x0A
    u8 unk_0B;
    union
    {
        u8 raw;
        struct
        {
            u8 gone:1;
            u8 bit1:1;
            u8 shown:1;
            u8 bit3:1;
            u8 active:1;
            u8 bit5:1;
            u8 hit:1;
            u8 bit7:1;
        } __attribute__((packed)) b;
    } __attribute__((packed)) fl; // 0x0C
    u8 unk_0D_0:2;         // 0x0D
    u8 blink:1;
    u8 unk_0D_3:5;
    u8 unk_0E[0xA];
    struct vtable *vt;     // 0x18
    u8 unk_1C[4];
    struct { struct anim_rec *recs; } *table; // 0x20
    u8 unk_24[4];
    struct part_f28 f28;   // 0x28
    u8 slot:4;             // 0x29
    u8 unk_29_4:4;
    u8 unk_2A[3];
    u8 tag;                // 0x2D
    u8 unk_2E[2];
    s32 frame;             // 0x30
    s32 unk_34;            // 0x34
    u8 animDone;           // 0x38
    u8 unk_39[0xB];
    void *ctl;             // 0x44
    s32 unk_48;            // 0x48
    s32 unk_4C;            // 0x4C
    s32 unk_50;            // 0x50
    s32 unk_54;            // 0x54
    s32 unk_58;            // 0x58
    s32 unk_5C;            // 0x5C
    s32 unk_60;            // 0x60
    s32 unk_64;            // 0x64
    u8 unk_68[0x9C];
    u8 busy;               // 0x104
};

#define PART_OFFSET(f) ((u32)&((struct part *)0)->f)
COMPILE_TIME_ASSERT(PART_OFFSET(vt) == 0x18);
COMPILE_TIME_ASSERT(PART_OFFSET(f28) == 0x28);
COMPILE_TIME_ASSERT(PART_OFFSET(tag) == 0x2D);
COMPILE_TIME_ASSERT(PART_OFFSET(frame) == 0x30);
COMPILE_TIME_ASSERT(PART_OFFSET(ctl) == 0x44);
COMPILE_TIME_ASSERT(PART_OFFSET(busy) == 0x104);

/* Every class in this file keeps its method table at +0x0C. */
struct vobj
{
    u8 unk_00[0xC];
    struct vtable *vt; // 0x0C
};

struct obj_4704
{
    u8 unk_00[0xC];
    struct vtable *vt; // 0x0C
    u8 unk_10[4];
    s32 x;             // 0x14
    s32 y;             // 0x18
    s32 dx;            // 0x1C
    s32 dy;            // 0x20
    s32 unk_24;        // 0x24
    s32 unk_28;        // 0x28
    u8 unk_2C[0x10];
    struct { u8 unk_00[0x10]; s32 index; } *src; // 0x3C
};

struct obj_476c
{
    u8 unk_00[8];
    s32 state;         // 0x08
    struct vtable *vt; // 0x0C
    u8 unk_10[0x10];
    struct part *part; // 0x20
};

/* gStaticData_087E4974's class (per-frame update sub_80197F8, destructor
 * sub_801A824 in the next asm file). */
struct boss
{
    u8 unk_00[8];
    s32 state;         // 0x08
    struct vtable *vt; // 0x0C
    s32 hits;          // 0x10
    u8 unk_14[8];
    s32 step;          // 0x1C - approach-table index
    s32 timer;         // 0x20
    s32 nextState;     // 0x24
    s32 passes;        // 0x28
    struct part *part; // 0x2C
};

/* gStaticData_087E490C's class (0x28 bytes; constructor sub_801A794 in
 * the next asm file, destructor sub_801A780, update sub_801A114). */
struct obj_490c
{
    u8 unk_00[8];
    s32 state;           // 0x08
    struct vtable *vt;   // 0x0C
    u8 unk_10[0xC];
    s32 blinkTimer;      // 0x1C
    s32 blinksLeft;      // 0x20
    struct part *target; // 0x24
};

/* gStaticData_087E483C's class (constructor sub_801A724, destructor
 * sub_801A73C, update sub_801A64C). */
struct obj_483c
{
    u8 unk_00[8];
    s32 state;         // 0x08
    struct vtable *vt; // 0x0C
};

/* gStaticData_087E48A4's class (constructor sub_801A768, destructor
 * sub_801A750, update sub_801A2A8). */
struct obj_48a4
{
    u8 unk_00[8];
    s32 state;           // 0x08
    struct vtable *vt;   // 0x0C
    u8 unk_10[0xC];
    struct part *target; // 0x1C
};

struct part_list
{
    u8 unk_00[4];
    s32 count;            // 0x04
    u8 unk_08[4];
    struct part **items;  // 0x0C
};

struct level_layer
{
    u8 unk_00[0x10];
    s32 width;  // 0x10
    s32 height; // 0x14
};

struct collect_info
{
    u8 unk_00[8];
    u16 *offset;               // 0x08
    u8 *bits;                  // 0x0C
};

struct level_state
{
    struct collect_info *info; // 0x00
    u8 unk_04[0x104];
    u32 bitmap[1];             // 0x108
};

extern struct part_list *gUnknown_030012EC;
extern struct part_list *gUnknown_030012F0;
extern struct part *gUnknown_030012D8;
extern void ***gUnknown_030012D0;
extern struct level_state *gEntityFlags;
extern void *gUnknown_030012BC;
extern void *gLevelState;
extern struct { u8 unk_00[0x10]; struct level_layer *layer; } *gLevelLayers;
extern u8 gStaticData_0816C358[];
extern s32 gStaticData_0816C368[];
extern s32 gStaticData_0816C378[];
extern s32 gStaticData_0816C390[];
extern s32 gStaticData_0816C3A0[];
extern s32 gStaticData_0816C3B8[];
extern u8 gStaticData_0816C3E8[];
extern u8 gStaticData_0816C3F4[];
extern u8 gStaticData_087E4704[];
extern u8 gStaticData_087E476C[];
extern u8 gStaticData_087E47D4[];
extern u8 gStaticData_087E483C[];
extern u8 gStaticData_087E48A4[];
extern u8 gStaticData_087E490C[];

extern void sub_800B8A8(void *self, s32 flags);
extern void sub_800B8C8(void *self);
extern void sub_8017A78(void *self, s32 flags);
extern void sub_8017A8C(void *self);
extern void sub_800CA60(void *self, s32 flags);
extern void sub_800CA74(void *self);
extern u8 sub_80231BC(void *arg0);
extern u8 sub_80231C4(void *arg0);
extern void sub_8021D80(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void sub_8021EF4(u32 arg0, s32 arg1, s32 arg2, s32 arg3);
/* These three return their box by value (gcc passes the hidden result
 * pointer in r0 and returns it). */
extern struct box sub_8007C30(struct part *obj);
extern struct box sub_8007CF8(struct part *obj);
extern struct box sub_8007B98(struct part *obj);
extern u8 sub_8001688(struct box *a, struct box *b);
extern void sub_80241A4(void);
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern struct part *sub_8009ED0(u32 arg0, u16 x, u16 y, u32 arg3);
extern void sub_80087C0(struct part *p);
extern void sub_80087B4(struct part *p);
extern void sub_800872C(struct part *p, s32 arg1);
extern void *sub_8026EDC(u32 size);
extern s32 sub_800815C(struct part *p);
extern void sub_8008E94(struct part_list *list, struct part *p);
extern void sub_801A7AC(struct boss *self, struct part *other, s32 arg2);

void sub_8019CE4(struct boss *self, struct part *other, s32 next);
void sub_8019EBC(struct boss *self, s32 mode, u16 x, u16 y, struct part *arg);
void sub_801A03C(struct boss *self, u16 x, u16 y, u8 facing);
void sub_801A584(struct obj_48a4 *self, u16 x, u16 y);
struct obj_490c *sub_801A794(void *mem);
struct vobj *sub_801A724(void *mem);
struct obj_48a4 *sub_801A768(void *mem);

/* C++ virtual calls: gcc 2.x reads the method-table entry's `this`
 * adjustment and function pointer, then makes an indirect call - which
 * Thumb code emits as `bl _call_via_rN` (N = the register holding the
 * function pointer). This ROM's copies of those libgcc helpers are the
 * sub_803AD78..sub_803AD94 trampolines (src/system/reg_trampolines.c). */
asm(".set _call_via_r1, sub_803AD7C\n"
    ".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n"
    ".set _call_via_r4, sub_803AD88\n");

typedef void (*method1_fn)(void *self, s32 a);
typedef void (*method2_fn)(void *self, void *a, s32 b);
typedef void (*method3_fn)(void *self, s32 a, s32 b, s32 c);
typedef u8 (*query_fn)(void *self);

#define VCALL1(obj, m, a)                                                      \
    do                                                                         \
    {                                                                          \
        struct vmethod *_m = &((struct vobj *)(obj))->vt->m;                   \
        ((method1_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));          \
    } while (0)
#define VCALL2(obj, m, a, b)                                                   \
    do                                                                         \
    {                                                                          \
        struct vmethod *_m = &((struct vobj *)(obj))->vt->m;                   \
        ((method2_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } while (0)

/* VCALL1 as a plain block: `do { } while (0)` is not neutral under
 * agbcc (its loop notes change allocation), and some call sites only
 * match without it. */
#define VCALL1_B(obj, m, a)                                                    \
    {                                                                          \
        struct vmethod *_m = &((struct vobj *)(obj))->vt->m;                   \
        ((method1_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));          \
    }

/* VCALL2 split in two, for call sites that share one indirect call:
 * load `this`/function/first argument here, then `goto` the call. */
#define PREP_VCALL2(obj, m, a_)                                                \
    do                                                                         \
    {                                                                          \
        struct vmethod *_m = &((struct vobj *)(obj))->vt->m;                   \
        t = (u8 *)(obj) + _m->thisOffset;                                      \
        fn = _m->fn;                                                           \
        a = (a_);                                                              \
    } while (0)

/* The ROM re-reads a just-filled box's `valid` word straight from its
 * stack slot rather than through the register already holding the
 * box's address; a volatile read is what stops gcc's CSE from
 * rewriting the address. */
#define BOX_VALID(bx) (*(vs32 *)&(bx).valid)

/* Right edge of the level, in Q8 units. */
static inline s32 LevelRight(void)
{
    return gLevelLayers->layer->width << 8;
}

/* Bottom edge of the level, in Q8 units. */
static inline s32 LevelBottom(void)
{
    return gLevelLayers->layer->height << 8;
}

static inline s32 AtLevelEdge(struct part_f28 *f, s32 x)
{
    if (f->facing)
        return x <= 0x2000;
    else
        return x >= LevelRight() - 0x2000;
}

/* Close enough to the player (on the side it is facing) to react. */
static inline void Approach(struct boss *self, struct part *other, s32 d)
{
    if (d <= 0x1FFF)
        sub_8019CE4(self, other, 5);
}

static inline void SetTag(struct part *p, u8 tag)
{
    p->tag = tag;
}

/* (u16)(width + n), computed the way the ROM does it: in the upper
 * halfword, then shifted back down. */
#define LayerWidthPlus(n) (((gLevelLayers->layer->width << 16) + ((n) << 16)) >> 16)

static inline void MarkCollected(struct part *p)
{
    p->fl.b.gone = 1;
    if (p->id != 0xFFFF)
    {
        s32 id = p->id;
        struct level_state *ls = gEntityFlags;
        s32 w = id;

        w /= 32;
        ls->bitmap[w] |= 1 << (id - w * 32);
    }
}

void sub_801967C(void *self, u8 flag)
{
    s32 i;
    s32 n = gUnknown_030012EC->count;

    for (i = 0; i < n; i++)
    {
        struct part *p = gUnknown_030012EC->items[i];

        if (flag)
            p->unk_0A = 1;
        else
            p->unk_0A = flag;
    }
}

void sub_80196B8(struct obj_4704 *self, s32 *origin, s32 x, s32 y)
{
    u8 v;

    self->x = x;
    self->y = y;
    self->dx = x - origin[0];
    self->dy = y - origin[1];
    v = *(self->src->index + gStaticData_0816C358);
    self->unk_28 = v;
    self->unk_24 = v;
}

void sub_80196E4(struct obj_4704 *self, s32 flags)
{
    self->vt = (struct vtable *)gStaticData_087E4704;
    sub_800B8A8(self, flags);
}

struct obj_4704 *sub_80196F8(struct obj_4704 *self, void *src)
{
    sub_800B8C8(self);
    self->vt = (struct vtable *)gStaticData_087E4704;
    self->unk_24 = 0;
    self->src = src;
    return self;
}

/* UNUSED - no caller or pointer anywhere in the ROM. */
void sub_8019718(struct vobj *self, s32 unused, s32 arg)
{
    VCALL1(self, m20, arg);
}

void sub_8019730(struct obj_476c *self, struct part *other)
{
    if (self->state == 0)
        other->fl.b.shown = 0;
}

void sub_8019744(struct obj_476c *self, s32 flags)
{
    self->vt = (struct vtable *)gStaticData_087E476C;
    sub_800B8A8(self, flags);
}

struct obj_476c *sub_8019758(struct obj_476c *self)
{
    sub_800B8C8(self);
    self->vt = (struct vtable *)gStaticData_087E476C;
    return self;
}

void sub_8019770(struct obj_476c *self, s32 unused, s32 arg)
{
    if (arg == 3)
    {
        VCALL1(self->part->ctl, m20, 9);
        if (!sub_80231C4(gLevelState))
            sub_8021D80(0xFFFF, 0x8C, 0x98, 0);
    }
    VCALL1(self, m20, arg);
}

void sub_80197C8(struct vobj *self, s32 flags)
{
    self->vt = (struct vtable *)gStaticData_087E47D4;
    sub_8017A78(self, flags);
}

struct vobj *sub_80197DC(struct vobj *self)
{
    sub_8017A8C(self);
    self->vt = (struct vtable *)gStaticData_087E47D4;
    return self;
}

/* UNUSED - no caller or pointer anywhere in the ROM. */
s32 sub_80197F4(struct boss *self)
{
    return self->hits;
}

void sub_80197F8(struct boss *self, struct part *other)
{
    struct box hurt;
    struct box box;
    u32 state;

    if (other->f28.facing)
    {
        s32 x = other->x;
        s32 y = other->y;
        struct part *p = self->part;

        p->x = x + 0x600;
        p->y = y;
    }
    else
    {
        s32 x = other->x;
        s32 y = other->y;
        struct part *p = self->part;

        p->x = x - 0x600;
        p->y = y;
    }
    hurt = sub_8007CF8(other);
    if (self->state == 8 && gUnknown_030012D8->unk_0A == 0x13)
    {
        box = sub_8007C30(gUnknown_030012D8);
        if (BOX_VALID(box) && sub_8001688(&box, &hurt))
        {
            self->hits++;
            sub_8019CE4(self, other, 11);
        }
    }

    state = self->state;
    switch (state)
    {
    case 0:
        sub_8019CE4(self, other, 1);
        self->step = 0;
        self->passes = 0;
        other->unk_0A = 0;
        other->fl.b.shown = 0;
        other->fl.b.hit = 0;
        break;
    case 1:
    case 14:
    {
        struct part_f28 *f = &other->f28;
        s32 x = other->x;

        if (state == 1)
        {
            if (f->facing)
            {
                s32 *tbl = gStaticData_0816C368;
                if (self->hits > 0)
                    tbl = gStaticData_0816C378;
                if (x <= tbl[self->step])
                {
                    Approach(self, other, gUnknown_030012D8->x - x);
                    self->step++;
                    break;
                }
            }
            else
            {
                s32 *tbl = gStaticData_0816C390;
                if (self->hits > 0)
                    tbl = gStaticData_0816C3A0;
                if (x >= tbl[self->step])
                {
                    Approach(self, other, x - gUnknown_030012D8->x);
                    self->step++;
                    break;
                }
            }
        }
        if (!AtLevelEdge(f, x))
            break;
        if (state == 1)
        {
            s32 lim = 1;
            if (self->hits > 0)
                lim = 2;
            if (++self->passes >= lim)
            {
                sub_8019CE4(self, other, 6);
                break;
            }
        }
        goto turn;
    }
    case 3:
    case 13:
        if (other->animDone)
        {
            u32 prev = state;
            s32 n;

            sub_8019CE4(self, other, 1);
            if (other->f28.facing)
            {
                s32 x = other->x >> 8;
                s32 y = other->y >> 8;

                x += 6;
                other->x = x << 8;
                other->y = y << 8;
            }
            else
            {
                s32 x = other->x >> 8;
                s32 y = other->y >> 8;

                x -= 6;
                other->x = x << 8;
                other->y = y << 8;
            }
            n = 8;
            if (n >= other->table->recs[other->tag].frameCount)
                n = other->table->recs[other->tag].frameCount - 1;
            other->frame = n;
            {
                s32 v = other->f28.facing;

                other->f28.facing = v ? 0 : 1;
            }
            if (prev == 3)
            {
                if (self->passes == 0)
                {
                    self->timer = 0x73;
                    if (self->hits > 1)
                        self->nextState = 15;
                    else
                        self->nextState = 1;
                    sub_8019CE4(self, other, 2);
                }
                else
                {
                    self->timer = 0x64;
                    self->nextState = 1;
                    sub_8019CE4(self, other, 2);
                }
                self->step = 0;
            }
            else
            {
                sub_8019CE4(self, other, 14);
            }
        }
        break;
    case 15:
        if (other->f28.facing)
            sub_801A03C(self, 0, 0x2D, 0);
        else
            sub_801A03C(self, LayerWidthPlus(0x28), 0x2D, 1);
        goto idle;
    case 4:
        if (!other->animDone)
            break;
        goto idle;
    case 5:
    case 6:
        if (other->frame == 0x14 && other->unk_34 == 0)
        {
            if (other->f28.facing)
                sub_8019EBC(self, 1, (other->x >> 8) + 6, (other->y >> 8) - 0x32, other);
            else
                sub_8019EBC(self, 1, (other->x >> 8) - 6, (other->y >> 8) - 0x32, other);
        }
        if (!other->animDone)
            break;
        if (self->state == 6)
        {
            other->fl.b.hit = 1;
        turn:
            sub_8019CE4(self, other, 3);
            break;
        }
        sub_8019CE4(self, other, 1);
        {
            s32 n = 8;
            if (n >= other->table->recs[other->tag].frameCount)
                n = other->table->recs[other->tag].frameCount - 1;
            other->frame = n;
        }
        break;
    case 2:
        if (--self->timer == 0)
            sub_8019CE4(self, other, self->nextState);
        break;
    case 7:
        sub_801A7AC(self, other, 0);
        VCALL2(self, m50, other, 5);
        sub_8019CE4(self, other, 8);
        break;
    case 8:
        if (--self->timer == 0)
            sub_8019CE4(self, other, 9);
        break;
    case 9:
    case 10:
        if (!other->animDone)
            break;
        if (state == 9)
        {
        idle:
            sub_8019CE4(self, other, 1);
            break;
        }
        sub_8019CE4(self, other, 12);
        break;
    case 11:
        if (self->timer != 0)
        {
            self->timer--;
            break;
        }
        if (self->hits > 2)
            sub_8019CE4(self, other, 16);
        if (other->animDone)
            sub_8019CE4(self, other, 10);
        break;
    case 12:
    {
        s32 lim = 0x4000;
        if (self->hits > 0)
            lim = 0x2000;
        if (other->f28.facing)
        {
            if (other->x > lim)
                break;
        }
        else
        {
            s32 x = other->x;

            if (x < LevelRight() - lim)
                break;
        }
        sub_8019CE4(self, other, 13);
        break;
    }
    case 16:
    {
        s32 y = other->y;

        if (y >= LevelBottom() + 0x2000)
        {
            sub_801A7AC(self, other, 0);
            if (sub_80231BC(gLevelState))
                sub_80241A4();
            sub_8019CE4(self, other, 17);
        }
        break;
    }
    case 17:
        break;
    }
}

/* "Enter state `next`": tells the object (slot +0x20), then plays the
 * matching animation/spawns. The three states that differ only in the
 * slot +0x50 animation id share one indirect call, as in the ROM - the
 * call's argument registers are pinned so each case loads them itself
 * before jumping to it. */
void sub_8019CE4(struct boss *self, struct part *other, s32 next)
{
    register void *t asm("r0");
    register void *a asm("r1");
    register s32 b asm("r2");
    register void *fn asm("r3");

    VCALL1(self, m20, next);
    switch (next)
    {
    case 16:
        if (!sub_80231BC(gLevelState))
            sub_8021EF4(0xFFFF, 0xA0, 0xA9, 0);
        sub_801A7AC(self, other, 3);
        break;
    case 12:
        sub_801A03C(self, gLevelLayers->layer->width, 0x28, 1);
        sub_801A03C(self, 0, 0x46, 0);
        sub_801A03C(self, LayerWidthPlus(0x46), 0x64, 1);
    case 1:
    case 14:
        VCALL2(self, m50, other, 0);
        sub_801A7AC(self, other, 1);
        break;
    case 3:
    case 13:
        PREP_VCALL2(self, m50, other);
        b = 6;
        goto call;
    case 4:
        PREP_VCALL2(self, m50, other);
        b = 1;
        goto call;
    case 6:
        self->passes = 0;
    case 5:
        PREP_VCALL2(self, m50, other);
        b = 4;
    call:
        ((method2_fn)fn)(t, a, b);
    case 2:
        sub_801A7AC(self, other, 0);
        break;
    case 8:
        VCALL1(self->part->ctl, m20, 2);
        self->timer = 0xD2;
        break;
    case 9:
    case 10:
        VCALL1(self->part->ctl, m20, 1);
        VCALL2(self, m50, other, 2);
        break;
    case 11:
        self->timer = 0x64;
        if (self->hits > 2)
            self->timer = 1;
        PlaySfx(gUnknown_030012BC, 0x15, 0x100);
        other->fl.b.shown = 0;
        sub_801A7AC(self, other, 2);
        VCALL2(self, m50, other, 1);
        break;
    }
}

void sub_8019EBC(struct boss *self, s32 mode, u16 x, u16 y, struct part *arg)
{
    struct part *p = sub_8009ED0(0xFFFF, x, y, 0);
    struct vobj *ctl;
    u8 *bits;

    p->fl.b.shown = 0;
    p->table = (void *)((u8 *)**gUnknown_030012D0 + 0x288);
    switch (mode)
    {
    case 0:
        {
            s32 kind = 1;

            p->f28.mode = kind;
            SetTag(p, 3);
            sub_80087C0(p);
            sub_80087B4(p);
            sub_800872C(p, 0);
            p->unk_0A = kind;
        }
        ctl = (struct vobj *)sub_801A794(sub_8026EDC(0x28));
        ((struct obj_490c *)ctl)->target = arg;
        self->part = p;
        break;
    case 1:
        PlaySfx(gUnknown_030012BC, 0x29, 0x100);
        SetTag(p, 7);
        sub_80087C0(p);
        sub_80087B4(p);
        sub_800872C(p, 0);
        p->unk_0A = 4;
        ctl = (struct vobj *)sub_801A768(sub_8026EDC(0x20));
        ((struct obj_48a4 *)ctl)->target = arg;
        break;
    default:
        ctl = NULL;
        break;
    }
    p->slot = sub_800815C(p);
    p->ctl = ctl;
    VCALL1(ctl, m18, p);
    bits = &gEntityFlags->info->bits[*gEntityFlags->info->offset];
    p->f28.facing = ((*bits >> 1) ^ 1) & 1;
    p->f28.flag5 = (*bits >> 2) & 1;
    p->fl.b.active = 1;
    if (mode == 0)
        sub_8008E94(gUnknown_030012EC, p);
    else
        sub_8008E94(gUnknown_030012F0, p);
}

/* Spawns one of the boss's floor-tile parts (record index 1 of the
 * `+0x30` table, `unk_0A` 6) at (x, y), with a gStaticData_087E483C
 * controller, facing `facing`, and registers it with gUnknown_030012F0.
 *
 * The two virtual calls are written as plain blocks, not VCALL1's
 * `do { } while (0)` (whose loop notes swap the part/controller
 * registers), and `facing` goes into the 1-bit field unmasked (an
 * explicit `& 1` makes the tag store reuse the held constant 1). */
void sub_801A03C(struct boss *self, u16 x, u16 y, u8 facing)
{
    struct part *p = sub_8009ED0(0xFFFF, x, y, 0);
    struct vobj *ctl;

    p->table = (void *)((u8 *)**gUnknown_030012D0 + 0x30);
    SetTag(p, 1);
    sub_80087C0(p);
    sub_80087B4(p);
    sub_800872C(p, 0);
    p->unk_0A = 6;
    ctl = sub_801A724(sub_8026EDC(0x8C));
    p->slot = sub_800815C(p);
    p->ctl = ctl;
    VCALL1_B(ctl, m18, p)
    p->f28.facing = facing;
    p->fl.b.active = 1;
    VCALL1_B(ctl, m18, p)
    sub_8008E94(gUnknown_030012F0, p);
}

/* gStaticData_087E490C's per-frame update (this controller is created
 * by sub_8019EBC mode 0; `other` is the part it drives): while the
 * player isn't busy
 * (gUnknown_030012D8+0x104) and `other` reports a hit (its own table
 * slot +0x28), overlaps `other`'s box with the player's hurt box (falling
 * back to the player's plain box) and on contact fires the player's slot
 * +0x68 method with `other->unk_0A`. Then: state 0 writes BLDCNT/
 * BLDALPHA (1st target OBJ, 2nd target BG0-3+OBJ, EVA=EVB=16) and moves
 * to state 5; states 1/2 arm a two-blink countdown and move to 3/4;
 * states 3/4 toggle `other`'s blink bit every 20 frames until the
 * blinks run out (then state 5).
 *
 * Was NAKED (~159 halfwords off as C). The ROM leaves r4-r6 unused for
 * the long-lived values (`self` r7, `&b` r8, `other` r9,
 * &gUnknown_030012D8 r10); the draft's allocation order was already the
 * ROM's, but it started at r5. Holding r5 and r6 across the box builders
 * (docs/matching/hard-register-hold-retry.md) makes global-alloc skip
 * them. The state-0 BLDCNT accumulator lives in r5 in the ROM: a
 * block-scoped r5 variable, initialised through the constant-init asm so
 * the orr chain is neither folded nor reordered, reproduces it. */
void sub_801A114(struct obj_490c *self, struct part *other)
{
    struct box a;
    struct box b;
    register s32 hr5 asm("r5");
    register s32 hr6 asm("r6");

    {
        struct vmethod *m = &other->vt->m28;
        if (((query_fn)m->fn)((u8 *)other + m->thisOffset))
        {
            if (!gUnknown_030012D8->busy)
            {
                /* Hard-register hold (no code): r5 and r6 stay live
                 * across the box builders, so no long-lived pseudo gets
                 * them. */
                asm("" : "=r"(hr5));
                asm("" : "=r"(hr6));
                a = sub_8007C30(other);
                b = sub_8007CF8(gUnknown_030012D8);
                if (!BOX_VALID(b))
                {
                    struct box *pb = &b;

                    *pb = sub_8007C30(gUnknown_030012D8);
                }
                /* End of the hold. */
                asm("" : : "r"(hr5));
                asm("" : : "r"(hr6));
                if (sub_8001688(&b, &a))
                {
                    struct part *pl = gUnknown_030012D8;
                    struct vmethod *m2 = &pl->vt->m68;

                    ((method3_fn)m2->fn)((u8 *)pl + m2->thisOffset, 0, other->unk_0A, 0);
                }
            }
        }
    }

    switch (self->state)
    {
    case 0:
        {
            register u32 acc asm("r5");
            u32 w;

            /* Constant-init (emits the `movs r5, #0x10`): a plain
             * assignment is folded into the orr chain. */
            asm("" : "=r"(acc) : "0"(BLDCNT_TGT1_OBJ));
            acc |= BLDCNT_TGT2_BG0;
            acc |= BLDCNT_TGT2_BG1;
            acc |= BLDCNT_TGT2_BG2;
            acc |= BLDCNT_TGT2_BG3;
            w = acc | BLDCNT_TGT2_OBJ;
            w |= 0x100000;
            w |= 0x10000000;
            *(vu32 *)REG_ADDR_BLDCNT = w;
        }
        VCALL1(self, m20, 5);
        break;
    case 1:
        self->blinksLeft = 2;
        self->blinkTimer = 0;
        VCALL1(self, m20, 3);
        break;
    case 2:
        self->blinksLeft = 2;
        self->blinkTimer = 0;
        VCALL1(self, m20, 4);
        break;
    case 3:
    case 4:
        if (self->blinkTimer == 0)
        {
            self->blinkTimer = 0x14;
            other->blink = !other->blink;
            if (self->blinksLeft == 0)
                VCALL1(self, m20, 5);
            self->blinksLeft--;
        }
        self->blinkTimer--;
        break;
    case 5:
        break;
    }
}

void sub_801A2A8(struct obj_48a4 *self, struct part *other)
{
    struct box a;
    struct box b;

    a = sub_8007C30(other);
    if (a.valid)
    {
        if (self->state != 4 && self->state != 6)
        {
            struct part *t = self->target;
            if ((t->fl.raw >> 6) & 1)
            {
                b = sub_8007B98(t);
                if (sub_8001688(&a, &b))
                {
                    VCALL1(self->target->ctl, m20, 7);
                    VCALL1(self, m20, 6);
                    VCALL2(self, m50, other, 8);
                    PlaySfx(gUnknown_030012BC, 0x39, 0x100);
                }
            }
        }
        if (!gUnknown_030012D8->busy)
        {
            b = sub_8007CF8(gUnknown_030012D8);
            if (!BOX_VALID(b))
            {
                struct box *pb = &b;

                *pb = sub_8007C30(gUnknown_030012D8);
            }
            if (sub_8001688(&a, &b))
            {
                struct part *pl = gUnknown_030012D8;
                struct vmethod *m2 = &pl->vt->m68;

                ((method3_fn)m2->fn)((u8 *)pl + m2->thisOffset, 0, other->unk_0A, 0);
                if (self->state == 3)
                {
                    VCALL1(self, m20, 6);
                    VCALL2(self, m50, other, 8);
                    PlaySfx(gUnknown_030012BC, 0x39, 0x100);
                }
            }
        }
    }

    switch (self->state)
    {
    case 0:
        VCALL2(self, m30, other, gStaticData_0816C3E8);
        VCALL1(self, m20, 1);
        break;
    case 1:
        if (other->y <= 0x800)
        {
            other->unk_64 = 0;
            other->unk_54 = 0;
            other->unk_58 = 0;
            other->unk_5C = 0;
            VCALL2(self, m50, other, 8);
            other->slot = sub_800815C(other);
            sub_801A584(self, other->x >> 8, other->y >> 8);
            VCALL1(self, m20, 2);
        }
        break;
    case 5:
        VCALL2(self, m50, other, 9);
        VCALL2(self, m30, other, gStaticData_0816C3F4);
        VCALL1(self, m20, 3);
        break;
    case 3:
    case 4:
    {
        s32 y = other->y;

        if (y >= LevelBottom() - 0x2000)
        {
            VCALL1(self, m20, 6);
            VCALL2(self, m50, other, 8);
            PlaySfx(gUnknown_030012BC, 0x39, 0x100);
        }
        break;
    }
    case 6:
        other->unk_64 = 0;
        other->unk_54 = 0;
        other->unk_58 = 0;
        other->unk_5C = 0;
    case 2:
        if (other->animDone)
            MarkCollected(other);
        break;
    }
}

void sub_801A584(struct obj_48a4 *self, u16 x, u16 y)
{
    struct part *p = sub_8009ED0(0xFFFF, x, y, 0);
    struct obj_48a4 *c;

    p->fl.b.shown = 0;
    p->table = (void *)((u8 *)**gUnknown_030012D0 + 0x288);
    SetTag(p, 8);
    sub_80087C0(p);
    sub_80087B4(p);
    sub_800872C(p, 0);
    p->unk_0A = 1;
    c = sub_8026EDC(0x20);
    sub_8017A8C(c);
    c->vt = (struct vtable *)gStaticData_087E48A4;
    c->target = self->target;
    VCALL1(c, m20, 5);
    p->slot = sub_800815C(p);
    p->ctl = c;
    VCALL1(c, m18, p);
    p->fl.b.active = 1;
    sub_8008E94(gUnknown_030012F0, p);
}

void sub_801A64C(struct obj_483c *self, struct part *other)
{
    switch (self->state)
    {
    case 0:
        if (other->f28.facing)
        {
            s32 a = -gStaticData_0816C3B8[9];
            s32 c = -gStaticData_0816C3B8[11];
            s32 b = gStaticData_0816C3B8[10];
            other->unk_60 = a;
            other->unk_48 = a;
            other->unk_4C = b;
            other->unk_50 = c;
        }
        else
        {
            s32 a = gStaticData_0816C3B8[9];
            s32 b = gStaticData_0816C3B8[10];
            s32 c = gStaticData_0816C3B8[11];
            other->unk_60 = a;
            other->unk_48 = a;
            other->unk_4C = b;
            other->unk_50 = c;
        }
        break;
    case 1:
        if (other->f28.facing)
        {
            if (other->x + 0x2800 <= 0)
                MarkCollected(other);
        }
        else
        {
            s32 x = other->x;

            if (x >= LevelRight() + 0x2800)
                MarkCollected(other);
        }
        break;
    }
}

struct vobj *sub_801A724(void *mem)
{
    struct vobj *self = mem;

    sub_800CA74(self);
    self->vt = (struct vtable *)gStaticData_087E483C;
    return self;
}

void sub_801A73C(struct vobj *self, s32 flags)
{
    self->vt = (struct vtable *)gStaticData_087E483C;
    sub_800CA60(self, flags);
}

void sub_801A750(struct obj_48a4 *self, s32 flags)
{
    self->vt = (struct vtable *)gStaticData_087E48A4;
    self->target = NULL;
    sub_8017A78(self, flags);
}

struct obj_48a4 *sub_801A768(void *mem)
{
    struct obj_48a4 *self = mem;

    sub_8017A8C(self);
    self->vt = (struct vtable *)gStaticData_087E48A4;
    return self;
}

void sub_801A780(struct vobj *self, s32 flags)
{
    self->vt = (struct vtable *)gStaticData_087E490C;
    sub_8017A78(self, flags);
}
