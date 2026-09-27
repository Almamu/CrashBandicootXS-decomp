#include "core.h"

/* GitHub issue #22, ROM 0x08018008-0x080187FC, formerly
 * asm/code_3_2_17_18008.s (details in
 * docs/matching/issue-22-0x08018008-hopper.md). Built with old_agbcc
 * (Makefile OLD_AGBCC_OBJS), like actor_part_188d0.c right after it.
 *
 * sub_8018008/sub_8018400 are the per-frame update and "enter state"
 * methods of the gStaticData_087E4564 class (constructor sub_80189EC,
 * actor_part_188d0.c): a boss that hops its `part` along parabolic arcs
 * (the 257-entry i*i>>8 table at +0x48) between the gUnknown_030012EC
 * list's anchor objects, stomping them. sub_801865C picks the next anchor
 * from a per-round table, sub_80186F0 spawns a falling hazard. */

asm(".set _call_via_r1, sub_803AD7C\n"
    ".set _call_via_r2, sub_803AD80\n"
    ".set _call_via_r3, sub_803AD84\n"
    ".set _call_via_r4, sub_803AD88\n"
    ".set __divsi3, sub_803ADB4\n");

struct hop_method
{
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct hop_vtable
{
    u8 unk_00[0x18];
    struct hop_method m18; // 0x18
    struct hop_method m20; // 0x20
    u8 unk_28[0x20];
    struct hop_method m48; // 0x48
    struct hop_method m50; // 0x50
    u8 unk_58[0x10];
    struct hop_method m68; // 0x68
};

struct hop_vobj
{
    u8 unk_00[0xC];
    struct hop_vtable *vt; // 0x0C
};

struct hop_anim_record
{
    u8 unk_00[0x16];
    u8 frameCount; // 0x16
    u8 unk_17[5];
};

struct hop_anim_bank
{
    struct hop_anim_record *records;
};

struct hop_part
{
    s32 x;                        // 0x00
    s32 y;                        // 0x04
    u16 id;                       // 0x08
    u8 unk_0A;                    // 0x0A
    u8 unk_0B;
    u8 flags;                     // 0x0C
    u8 unk_0D[0x13];
    struct hop_anim_bank *bank;   // 0x20
    u8 unk_24[5];
    u8 slot;                      // 0x29 - low nibble: palette slot
    u8 unk_2A[3];
    u8 tag;                       // 0x2D
    u8 unk_2E[2];
    s32 frame;                    // 0x30
    u8 unk_34[4];
    u8 animDone;                  // 0x38
    u8 unk_39[0xB];
    struct hop_vobj *ctrl;        // 0x44
    u8 unk_48[0xC];
    s32 unk_54;                   // 0x54
    s32 unk_58;                   // 0x58
    s32 unk_5C;                   // 0x5C
    u8 unk_60[4];
    s32 unk_64;                   // 0x64
};

struct hop_player
{
    s32 x;                        // 0x00
    s32 y;                        // 0x04
    u8 unk_08[2];
    u8 unk_0A;                    // 0x0A
    u8 unk_0B[0xD];
    struct hop_vtable *vt;        // 0x18
    u8 unk_1C[0xE8];
    u8 busy;                      // 0x104
};

struct hop_list
{
    u8 unk_00[4];
    s32 count;                    // 0x04
    u8 unk_08[4];
    struct hop_part **items;      // 0x0C
};

struct hop_box
{
    s32 x;
    s32 y;
    s32 w;
    s32 h;
};

struct hop_level
{
    u8 unk_00[0x10];
    struct { u8 unk_00[4]; s32 unk_04; u8 unk_08[8]; s32 width; s32 height; } *layer0;
};

/* gStaticData_087E4564 class */
struct hopper
{
    u8 unk_00[8];
    s32 state;                    // 0x08
    struct hop_vtable *vt;        // 0x0C
    s32 round;                    // 0x10
    u8 unk_14[8];
    s32 nextState;                // 0x1C
    s32 timer;                    // 0x20
    s32 stomped;                  // 0x24 - anchor to reset, or -1
    s32 target;                   // 0x28 - anchor index
    s32 count;                    // 0x2C
    s32 x;                        // 0x30 - hop start
    s32 y;                        // 0x34
    s32 steps;                    // 0x38
    s32 total;                    // 0x3C
    s32 dy;                       // 0x40
    s32 dx;                       // 0x44
    s16 *squares;                 // 0x48
};

/* The ROM re-reads a just-filled box's `w` word from its stack slot rather
 * than through the register holding the box's address (see
 * actor_part_1967c.c). */
#define BOX_VALID(bx) (*(vs32 *)&(bx).w)

typedef void (*hop_fn1)(void *self, s32 a);
typedef void (*hop_fn1p)(void *self, void *a);
typedef void (*hop_fn2)(void *self, void *a, s32 b);
typedef void (*hop_fn3)(void *self, s32 a, s32 b, s32 c);

#define VCALL1(obj, m, a)                                                      \
    do                                                                         \
    {                                                                          \
        struct hop_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));             \
    } while (0)
#define VCALL1P(obj, m, a)                                                     \
    do                                                                         \
    {                                                                          \
        struct hop_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn1p)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a));         \
    } while (0)
#define VCALL2(obj, m, a, b)                                                   \
    do                                                                         \
    {                                                                          \
        struct hop_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } while (0)
#define VCALL3(obj, m, a, b, c)                                                \
    do                                                                         \
    {                                                                          \
        struct hop_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn3)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c));        \
    } while (0)

extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern u8 ***gUnknown_030012D0;
extern struct hop_player *gUnknown_030012D8;
extern struct hop_list *gUnknown_030012EC;
extern void *gUnknown_030012F0;
extern struct hop_level *gUnknown_03001308;
extern u8 gStaticData_0816C308[];
extern u8 gStaticData_0816C30B[];

extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void *sub_8026EDC(u32 size);
extern struct hop_vobj *sub_801886C(void *mem);
extern struct hop_vobj *sub_80188D0(void *mem);
extern struct hop_box sub_8007C30(void *obj);
extern struct hop_box sub_8007CF8(void *obj);
extern u8 sub_8001688(struct hop_box *a, struct hop_box *b);
extern u8 sub_80231B4(void *self);
extern void sub_80241A4(void);
extern void sub_8018978(struct hopper *self, struct hop_part *part);
extern void nullsub_19(struct hopper *self, struct hop_part *part);
extern void sub_8021DFC(u32 arg0, u16 x, u16 y, u16 arg3);
extern struct hop_part *sub_8009ED0(u16 arg0, u16 arg1, u16 arg2, u16 arg3);
extern void sub_80087C0(struct hop_part *p);
extern void sub_80087B4(struct hop_part *p);
extern void sub_800872C(struct hop_part *p, s32 arg1);
extern s32 sub_800815C(struct hop_part *p);
extern void sub_8008E94(void *list, struct hop_part *p);

/* Byte read-modify-writes of the flags at +0x0C. old_agbcc materializes
 * the constant before loading the byte only when it arrives as an inline
 * helper's `s32` parameter (docs/matching/old-agbcc-retry.md). */
#define PART_FLAGS(p) (*((u8 *)(p) + 0xC))

static inline void OrFlags(struct hop_part *part, s32 bits)
{
    PART_FLAGS(part) |= bits;
}

static inline void AndFlags(struct hop_part *part, s32 mask)
{
    PART_FLAGS(part) &= mask;
}

/* Sets the palette-slot nibble at +0x29 (actor_part_188d0.c's
 * SetFrameNibble - the pinned registers are still needed under
 * old_agbcc). */
static inline void SetSlot(struct hop_part *part, s32 v)
{
    register s32 val asm("r0") = v;
    register u8 *p asm("r2") = &part->slot;
    register s32 m asm("r1");
    register s32 b asm("r3");

    val &= 0xF;
    asm volatile("mov %0, #0x10\n\tneg %0, %0" : "=r"(m));
    b = *p;
    m &= b;
    m |= val;
    *p = m;
}

void sub_8018400(struct hopper *self, struct hop_part *part, s32 next);
s32 sub_801865C(struct hopper *self);
void sub_80186F0(struct hopper *self, struct hop_part *part, s32 n);

void sub_8018008(struct hopper *self, struct hop_part *part)
{
    struct hop_box a;
    struct hop_box b;
    s32 state;

    if (self->stomped != -1)
    {
        struct hop_part *anchor = gUnknown_030012EC->items[self->stomped];
        struct hop_vobj *ctrl;

        if (anchor->ctrl != NULL)
            VCALL1(anchor->ctrl, m48, 3);
        ctrl = sub_801886C(sub_8026EDC(0x10));
        anchor->ctrl = ctrl;
        VCALL1P(ctrl, m18, anchor);
        self->stomped = -1;
        PlaySfx(gUnknown_030012BC, 0x39, 0x100);
    }

    if (self->state == 8)
    {
        a = sub_8007C30(gUnknown_030012D8);
        b = sub_8007CF8(part);
        if (a.w != 0 && BOX_VALID(b) && sub_8001688(&b, &a)
            && gUnknown_030012D8->unk_0A == 0x13)
            sub_8018400(self, part, 9);
    }
    else if (gUnknown_030012D8->busy == 0)
    {
        a = sub_8007CF8(gUnknown_030012D8);
        if (a.w == 0)
        {
            b = sub_8007C30(gUnknown_030012D8);
            a = b;
        }
        b = sub_8007C30(part);
        if (BOX_VALID(b) && a.w != 0 && sub_8001688(&b, &a))
        {
            struct hop_player *pl = gUnknown_030012D8;
            struct hop_method *m = &pl->vt->m68;
            void *t = (u8 *)pl + m->thisOffset;
            ((hop_fn3)m->fn)(t, 0, 1, 0);
        }
    }

    state = self->state;
    switch (state)
    {
    case 0:
        AndFlags(part, ~4);
        {
            /* a named `one`: written as three plain `= 1`/`= 0` stores, the
             * 0 is materialized first (it is CSE'd from the dead `& 0` of
             * the byte store's expansion) */
            s32 one = 1;

            part->unk_0A = one;
            self->target = one;
            self->count = 0;
        }
        sub_8018400(self, part, 1);
        break;
    case 1:
    case 2:
    case 11:
    case 15:
    {
        s32 steps = --self->steps;
        s32 x = self->dx * steps / self->total + self->x;
        s32 t = (steps << 8) / self->total;
        s32 y = ((0x100 - self->squares[0x100 - t]) * self->dy >> 8) + self->y;

        part->x = x;
        part->y = y;
        if (steps != 0)
            break;
        PlaySfx(gUnknown_030012BC, 0x2A, 0x100);
        if (self->state == 15)
        {
            if (sub_80231B4(gUnknown_030012C0))
                sub_80241A4();
            sub_8018400(self, part, 16);
        }
        else if (self->state == 1)
        {
            sub_8018400(self, part, 6);
        }
        else if (self->state == 11)
        {
            self->count = steps;
            VCALL2(self, m50, part, 3);
            self->nextState = 12;
            sub_8018400(self, part, 5);
        }
        else
        {
            PlaySfx(gUnknown_030012BC, 0x3D, 0x100);
            sub_8018400(self, part, 8);
        }
        break;
    }
    case 3:
    case 7:
    case 10:
    case 14:
    {
        s32 steps = --self->steps;
        s32 x = self->dx * steps / self->total + self->x;
        s32 t = (steps << 8) / self->total;
        s32 y = (self->squares[t] * self->dy >> 8) + self->y;

        part->x = x;
        part->y = y;
        if (steps != 0)
            break;
        {
            /* The ROM tests 14 and 3 on a low-register copy of `state`
             * (sb), then re-copies it for 7; a plain if-chain reloads sb
             * into a new register for every test. */
            s32 s = state;

            asm("" : "+r"(s));
            if (s == 14)
                sub_8018400(self, part, 15);
            else if (s == 3)
                sub_8018400(self, part, 1);
            else if (state == 7)
                sub_8018400(self, part, 2);
            else
                sub_8018400(self, part, 13);
        }
        break;
    }
    case 13:
        if (self->nextState == 0)
        {
            s32 timer = --self->timer;

            if (timer == 0)
            {
                sub_8018400(self, part, 11);
            }
            else
            {
                self->nextState = 0x46;
                sub_80186F0(self, part, timer);
            }
        }
        self->nextState--;
        break;
    case 6:
        if (part->animDone)
        {
            if (++self->count > 3)
            {
                self->count = 0;
                sub_8018400(self, part, 7);
            }
            else
            {
                sub_8018400(self, part, 3);
            }
        }
        break;
    case 8:
        if (self->timer != 0)
        {
            self->timer--;
            break;
        }
        self->timer--;
        VCALL2(self, m50, part, 0);
        self->nextState = 3;
        sub_8018400(self, part, 5);
        break;
    case 5:
        if (part->animDone)
            sub_8018400(self, part, self->nextState);
    case 4:
        if (--self->timer == 0)
            sub_8018400(self, part, self->nextState);
        break;
    case 12:
        self->stomped = gStaticData_0816C308[self->round - 1];
        sub_8018400(self, part, 3);
        break;
    case 9:
        if (self->round > 2)
            sub_8018400(self, part, 14);
        if (part->animDone)
        {
            sub_8018400(self, part, 10);
            PlaySfx(gUnknown_030012BC, 0xD, 0x100);
        }
        break;
    }
}

static inline void SetFrame(struct hop_part *part, s32 frame)
{
    struct hop_anim_bank *bank = part->bank;
    u8 *tag = &part->tag;
    struct hop_anim_record *records = bank->records;
    s32 count = records[*tag].frameCount;

    if (frame >= count)
        frame = count - 1;
    part->frame = frame;
}

void sub_8018400(struct hopper *self, struct hop_part *part, s32 next)
{
    switch (next)
    {
    case 13:
        self->nextState = 0;
        self->timer = 4;
    case 11:
        self->target = gStaticData_0816C308[self->round - 1];
    case 1:
    case 2:
    {
        /* pinned: unpinned, `anchor` lands in r0 and `x` in r1 */
        register struct hop_part *anchor asm("r2");
        register s32 x asm("r1");

        if (next == 1)
            VCALL2(self, m50, part, 2);
        else
            VCALL2(self, m50, part, 1);
        SetFrame(part, 4);
        anchor = gUnknown_030012EC->items[self->target];
        x = anchor->x;
        self->x = x;
        if (next == 11 || next == 13)
            part->x = x;
        self->y = anchor->y - 0x2400;
        goto hop;
    }
    case 3:
    case 7:
    {
        struct hop_part *anchor;
        s32 ax;
        s32 ay;
        s32 y;

        self->target = sub_801865C(self);
        VCALL2(self, m50, part, 4);
        anchor = gUnknown_030012EC->items[self->target];
        ax = anchor->x;
        ay = anchor->y;
        y = ay - 0x2400;
        self->x = (ax + part->x) >> 1;
        if (y >= part->y)
            y = part->y - 0x5900;
        else
            y = ay - 0x7D00;
        self->y = y;
        goto hop;
    }
    case 10:
        self->y = -0x3000;
        self->x = part->x;
    hop:
        sub_8018978(self, part);
        break;
    case 8:
        VCALL2(self, m50, part, 6);
        self->timer = 0xB4;
        break;
    case 9:
        PlaySfx(gUnknown_030012BC, 0x15, 0x100);
        if (++self->round > 2)
        {
            s32 x = part->x;

            self->y = part->y - 0x6400;
            self->x = x + 0x6400;
            sub_8018978(self, part);
        }
        nullsub_19(self, part);
        VCALL2(self, m50, part, 7);
        break;
    case 14:
    {
        struct hop_part *anchor = gUnknown_030012EC->items[2];
        s32 x = anchor->x;
        s32 y = anchor->y - 0x1800;

        if (!sub_80231B4(gUnknown_030012C0))
            sub_8021DFC(0xFFFF, x >> 8, y >> 8, 0);
        break;
    }
    case 15:
    {
        s32 x = part->x;

        self->x = x;
        self->y = (gUnknown_03001308->layer0->height << 8) + 0x4000;
        self->x = x + 0x6400;
        sub_8018978(self, part);
        break;
    }
    }
    VCALL1(self, m20, next);
}

static inline s32 Abs(s32 v)
{
    s32 sign = v >> 31;

    return (v ^ sign) - sign;
}

/* Picks the hop target: the anchor nearest the player selects a column
 * of this round's/current anchor's gStaticData_0816C30B row. */
s32 sub_801865C(struct hopper *self)
{
    s32 nearest = 0;
    s32 best = 0xFFFFFF;
    s32 i;

    for (i = 0; i < gUnknown_030012EC->count; i++)
    {
        struct hop_part *anchor = gUnknown_030012EC->items[i];
        s32 px = gUnknown_030012D8->x;
        s32 py = gUnknown_030012D8->y;
        s32 ax = anchor->x;
        s32 ay = anchor->y;
        s32 d = Abs(ax - px) + Abs(ay - py);

        if (d < best)
        {
            best = d;
            nearest = i;
        }
    }
    return gStaticData_0816C30B[self->target * 5 + nearest + self->round * 25];
}

/* Spawns a falling hazard (a gUnknown_030012F0 part driven by a
 * sub_80188D0 object) at the `n`th third of the way from `part` towards
 * the player. */
void sub_80186F0(struct hopper *self, struct hop_part *part, s32 n)
{
    /* `p` pinned to r4: unpinned, it and `ctrl` swap r4/r5 */
    register struct hop_part *p asm("r4") = sub_8009ED0(0xFFFF, 0, 0, 0);
    struct hop_vobj *ctrl;
    s32 x;
    s32 zero;

    p->bank = (void *)(**gUnknown_030012D0 + 0x294);
    {
        /* The ROM loads the tag (5, in r0) before its address, and
         * materializes the 0 it later stores to +0x64/+0x54 here, keeping
         * it in r8 across the calls (like the 0xF of sub_8018CB0,
         * actor_part_188d0.c); no plain-C placement of that 0 does this. */
        register s32 t asm("r0") = 5;
        u8 *tp;

        asm("" : "+r"(t));
        tp = &p->tag;
        zero = 0;
        asm("" : "+r"(zero));
        *tp = t;
    }
    sub_80087C0(p);
    sub_80087B4(p);
    sub_800872C(p, 0);
    ctrl = sub_80188D0(sub_8026EDC(0x10));
    SetSlot(p, sub_800815C(p));
    p->ctrl = ctrl;
    VCALL1P(ctrl, m18, p);
    OrFlags(p, 0x10);
    sub_8008E94(gUnknown_030012F0, p);
    {
        /* the ROM materializes 0x80 before re-reading `zero` */
        s32 k = 0x80;

        p->unk_64 = zero;
        p->unk_54 = zero;
        p->unk_58 = k;
        p->unk_5C = k;
    }
    {
        s32 x0 = part->x;
        s32 y;

        x = x0 + (gUnknown_030012D8->x - x0) * (n - 1) / 3;
        y = gUnknown_03001308->layer0->unk_04 << 8;
        p->x = x;
        p->y = y;
    }
    p->unk_0A = 1;
    {
        /* two masks, not folded to -0x45; the -5 is derived from the 1 */
        s32 m = -5;

        m &= p->flags;
        m &= -0x41;
        p->flags = m;
    }
    PlaySfx(gUnknown_030012BC, 0x13, 0x100);
}
