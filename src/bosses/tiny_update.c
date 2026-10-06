#include "core.h"
#include "util.h"
#include "audio.h"
#include "bosses.h"
#include "objects.h"
#include "memory.h"
#include "level.h"

/* GitHub issue #22, ROM 0x08018008-0x080187FC, formerly
 * asm/code_3_2_17_18008.s (details in
 * docs/matching/issue-22-0x08018008-hopper.md). Built with old_agbcc
 * (Makefile OLD_AGBCC_OBJS), like cortex.c right after it.
 *
 * UpdateTiny/SetTinyState are the per-frame update and "enter state"
 * methods of the gTinyVtable class (constructor CreateTiny,
 * cortex.c): a boss that hops its `part` along parabolic arcs
 * (the 257-entry i*i>>8 table at +0x48) between the gUnknown_030012EC
 * list's anchor objects, stomping them. PickTinyHopTarget picks the next anchor
 * from a per-round table, SpawnTinyFallingLeaves spawns a falling hazard. */

struct hop_vtable
{
    u8 unk_00[0x18];
    struct actor_method m18; // 0x18
    struct actor_method m20; // 0x20
    u8 unk_28[0x20];
    struct actor_method m48; // 0x48
    struct actor_method m50; // 0x50
    u8 unk_58[0x10];
    struct actor_method m68; // 0x68
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
    s32 rampYStart;                   // 0x54
    s32 rampYStep;                   // 0x58
    s32 rampYTarget;                   // 0x5C
    u8 unk_60[4];
    s32 speedY;                   // 0x64
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

struct hop_level
{
    u8 unk_00[0x10];
    struct { u8 unk_00[4]; s32 unk_04; u8 unk_08[8]; s32 width; s32 height; } *layer0;
};

/* gTinyVtable class */
struct tiny_tiger
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
 * dingodile.c). */
#define BOX_VALID(bx) (*(vs32 *)&(bx).w)

typedef void (*hop_fn1)(void *self, s32 a);
typedef void (*hop_fn1p)(void *self, void *a);
typedef void (*hop_fn2)(void *self, void *a, s32 b);
typedef void (*hop_fn3)(void *self, s32 a, s32 b, s32 c);

#define VCALL1(obj, m, a)                                                      \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));             \
    } while (0)
#define VCALL1P(obj, m, a)                                                     \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn1p)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a));         \
    } while (0)
#define VCALL2(obj, m, a, b)                                                   \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } while (0)
#define VCALL3(obj, m, a, b, c)                                                \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &(obj)->vt->m;                                 \
        ((hop_fn3)_m->fn)((u8 *)(obj) + _m->thisOffset, (a), (b), (c));        \
    } while (0)

extern void *gAudioContext;
extern void *gLevelState;
extern u8 ***gSpriteBankSet;
extern struct hop_player *gPlayer;
extern struct hop_list *gUnknown_030012EC;
extern void *gCollidableList;
extern struct hop_level *gLevelLayers;

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

/* Sets the palette-slot nibble at +0x29 (cortex.c's
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

void UpdateTiny(struct tiny_tiger *self, struct hop_part *part)
{
    struct aabb a;
    struct aabb b;
    s32 state;

    if (self->stomped != -1)
    {
        struct hop_part *anchor = gUnknown_030012EC->items[self->stomped];
        struct hop_vobj *ctrl;

        if (anchor->ctrl != NULL)
            VCALL1(anchor->ctrl, m48, 3);
        ctrl = CreateStompedHopPadCtrl(OperatorNew(0x10));
        anchor->ctrl = ctrl;
        VCALL1P(ctrl, m18, anchor);
        self->stomped = -1;
        PlaySfx(gAudioContext, 0x39, 0x100);
    }

    if (self->state == 8)
    {
        GetSpriteAttackBox(&a, gPlayer);
        GetSpriteBodyBox(&b, part);
        if (a.w != 0 && BOX_VALID(b) && AabbOverlaps(&b, &a)
            && gPlayer->unk_0A == 0x13)
            SetTinyState(self, part, 9);
    }
    else if (gPlayer->busy == 0)
    {
        GetSpriteBodyBox(&a, gPlayer);
        if (a.w == 0)
        {
            GetSpriteAttackBox(&b, gPlayer);
            a = b;
        }
        GetSpriteAttackBox(&b, part);
        if (BOX_VALID(b) && a.w != 0 && AabbOverlaps(&b, &a))
        {
            struct hop_player *pl = gPlayer;
            struct actor_method *m = &pl->vt->m68;
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
        SetTinyState(self, part, 1);
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
        PlaySfx(gAudioContext, 0x2A, 0x100);
        if (self->state == 15)
        {
            if ((u8)HasTornadoSpin(gLevelState))
                RequestRoomExit();
            SetTinyState(self, part, 16);
        }
        else if (self->state == 1)
        {
            SetTinyState(self, part, 6);
        }
        else if (self->state == 11)
        {
            self->count = steps;
            VCALL2(self, m50, part, 3);
            self->nextState = 12;
            SetTinyState(self, part, 5);
        }
        else
        {
            PlaySfx(gAudioContext, 0x3D, 0x100);
            SetTinyState(self, part, 8);
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
                SetTinyState(self, part, 15);
            else if (s == 3)
                SetTinyState(self, part, 1);
            else if (state == 7)
                SetTinyState(self, part, 2);
            else
                SetTinyState(self, part, 13);
        }
        break;
    }
    case 13:
        if (self->nextState == 0)
        {
            s32 timer = --self->timer;

            if (timer == 0)
            {
                SetTinyState(self, part, 11);
            }
            else
            {
                self->nextState = 0x46;
                SpawnTinyFallingLeaves(self, part, timer);
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
                SetTinyState(self, part, 7);
            }
            else
            {
                SetTinyState(self, part, 3);
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
        SetTinyState(self, part, 5);
        break;
    case 5:
        if (part->animDone)
            SetTinyState(self, part, self->nextState);
    case 4:
        if (--self->timer == 0)
            SetTinyState(self, part, self->nextState);
        break;
    case 12:
        self->stomped = gTinyRoundAnchors[self->round - 1];
        SetTinyState(self, part, 3);
        break;
    case 9:
        if (self->round > 2)
            SetTinyState(self, part, 14);
        if (part->animDone)
        {
            SetTinyState(self, part, 10);
            PlaySfx(gAudioContext, 0xD, 0x100);
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

void SetTinyState(struct tiny_tiger *self, struct hop_part *part, s32 next)
{
    switch (next)
    {
    case 13:
        self->nextState = 0;
        self->timer = 4;
    case 11:
        self->target = gTinyRoundAnchors[self->round - 1];
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

        self->target = PickTinyHopTarget(self);
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
        StartTinyHop((struct gfx_offset_ctrl *)self, (struct gfx_part *)part);
        break;
    case 8:
        VCALL2(self, m50, part, 6);
        self->timer = 0xB4;
        break;
    case 9:
        PlaySfx(gAudioContext, 0x15, 0x100);
        if (++self->round > 2)
        {
            s32 x = part->x;

            self->y = part->y - 0x6400;
            self->x = x + 0x6400;
            StartTinyHop((struct gfx_offset_ctrl *)self, (struct gfx_part *)part);
        }
        nullsub_19(self, part);
        VCALL2(self, m50, part, 7);
        break;
    case 14:
    {
        struct hop_part *anchor = gUnknown_030012EC->items[2];
        s32 x = anchor->x;
        s32 y = anchor->y - 0x1800;

        if (!(u8)HasTornadoSpin(gLevelState))
            SpawnTornadoSpinPower(0xFFFF, x >> 8, y >> 8, 0);
        break;
    }
    case 15:
    {
        s32 x = part->x;

        self->x = x;
        self->y = (gLevelLayers->layer0->height << 8) + 0x4000;
        self->x = x + 0x6400;
        StartTinyHop((struct gfx_offset_ctrl *)self, (struct gfx_part *)part);
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
 * of this round's/current anchor's gTinyHopTargets row. */
s32 PickTinyHopTarget(struct tiny_tiger *self)
{
    s32 nearest = 0;
    s32 best = 0xFFFFFF;
    s32 i;

    for (i = 0; i < gUnknown_030012EC->count; i++)
    {
        struct hop_part *anchor = gUnknown_030012EC->items[i];
        s32 px = gPlayer->x;
        s32 py = gPlayer->y;
        s32 ax = anchor->x;
        s32 ay = anchor->y;
        s32 d = Abs(ax - px) + Abs(ay - py);

        if (d < best)
        {
            best = d;
            nearest = i;
        }
    }
    return gTinyHopTargets[self->target * 5 + nearest + self->round * 25];
}

/* Spawns a falling hazard (a gCollidableList part driven by a
 * CreateOneShotAnimCtrl object) at the `n`th third of the way from `part` towards
 * the player. */
void SpawnTinyFallingLeaves(struct tiny_tiger *self, struct hop_part *part, s32 n)
{
    /* `p` pinned to r4: unpinned, it and `ctrl` swap r4/r5 */
    register struct hop_part *p asm("r4") = CreateMovingSprite(0xFFFF, 0, 0, 0);
    struct hop_vobj *ctrl;
    s32 x;
    s32 zero;

    p->bank = (void *)(**gSpriteBankSet + 0x294);
    {
        /* The ROM loads the tag (5, in r0) before its address, and
         * materializes the 0 it later stores to +0x64/+0x54 here, keeping
         * it in r8 across the calls (like the 0xF of SpawnCortexTarget,
         * cortex.c); no plain-C placement of that 0 does this. */
        register s32 t asm("r0") = 5;
        u8 *tp;

        asm("" : "+r"(t));
        tp = &p->tag;
        zero = 0;
        asm("" : "+r"(zero));
        *tp = t;
    }
    ResetSpriteFrameTimer(p);
    ResetSpriteFrameIndex(p);
    SetSpriteAnimDone(p, 0);
    ctrl = CreateOneShotAnimCtrl(OperatorNew(0x10));
    SetSlot(p, GetSpriteAnimPaletteSlot((struct actor *)p));
    p->ctrl = ctrl;
    VCALL1P(ctrl, m18, p);
    OrFlags(p, 0x10);
    AddToPartList(gCollidableList, p);
    {
        /* the ROM materializes 0x80 before re-reading `zero` */
        s32 k = 0x80;

        p->speedY = zero;
        p->rampYStart = zero;
        p->rampYStep = k;
        p->rampYTarget = k;
    }
    {
        s32 x0 = part->x;
        s32 y;

        x = x0 + (gPlayer->x - x0) * (n - 1) / 3;
        y = gLevelLayers->layer0->unk_04 << 8;
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
    PlaySfx(gAudioContext, 0x13, 0x100);
}
