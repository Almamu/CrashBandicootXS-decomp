#include "core.h"
#include "util.h"
#include "audio.h"
#include "player.h"
#include "bosses.h"
#include "enemies.h"
#include "objects.h"
#include "memory.h"
#include "crates.h"
#include "level.h"
#include "globals.h"

/* codegen: GetSpriteAttackBox/GetSpriteBodyBox take the destination as
 * their first argument (objects.h); this file was matched against the
 * same calls written as a struct return, which gives a different stack
 * frame. docs/headers_plan.md */
extern struct aabb GetSpriteAttackBox_s(void *part) asm("GetSpriteAttackBox");
extern struct aabb GetSpriteBodyBox_s(void *part) asm("GetSpriteBodyBox");

/* GitHub issue #24: 0x0801967C-0x0801A794, formerly
 * asm/code_3_2_17_188d0_1967c.s.
 *
 * Small actor-part "controller" classes, each with a method table at
 * self+0x0C (per-frame update in slot +0x0C, destructor in slot +0x4C)
 * and a constructor/destructor pair here: gCortexTargetVtable
 * (CreateCortexTargetCtrl/DestroyCortexTargetCtrl), 087E476C (CreateCortexCannonCtrl/DestroyCortexCannonCtrl, update
 * UpdateCortexCannon), 087E47D4 (CreateCortexBoss/DestroyCortexBoss), 087E483C
 * (CreateDingodileSharkCtrl/DestroyDingodileSharkCtrl, update UpdateDingodileShark), 087E48A4
 * (CreateDingodileProjectileCtrl/DestroyDingodileProjectileCtrl, update UpdateDingodileProjectile), 087E490C (destructor
 * DestroyDingodileShieldCtrl, update UpdateDingodileShield) and 087E4974 (update UpdateDingodile).
 *
 * The 087E4974 object is a boss-like state machine: UpdateDingodile steers
 * its part along the level (approach tables gDingodileStopXLeft/78/90/
 * A0, turning round at either level edge), counts hits the player lands
 * on it, and spawns helper parts through SpawnDingodileShieldOrRocket/SpawnDingodileShark;
 * SetDingodileState is its "enter state N" transition (animation + follow-up
 * spawns/sounds). The last state signals RequestRoomExit (the "entity ready"
 * barrier, see docs/rom_map.md) once the part falls off the bottom.
 * Its helpers use sprite bank 54: the purple energy ring of anim 3
 * (SpawnDingodileShieldOrRocket mode 0, kept 6 px in front of him,
 * alpha-blended and hurting the player on contact), the rocket of anim 7
 * (mode 1, sfx 0x29) that flies up and, at the top, drops the stalactite
 * of anims 8/9 (SpawnDingodileStalactite) that hurts him if it lands on
 * him, and a bank-4 shark (SpawnDingodileShark) that crosses the level.
 *
 * This file is compiled with tools/agbcc/bin/old_agbcc (see Makefile and
 * docs/matching/issue-24-boss-actor.md): the old compiler reproduces
 * this region's "constant before the byte it's combined with" ordering
 * without register pins. The code was C++: virtual calls are indirect
 * calls through libgcc's `_call_via_rN` helpers
 * (lib/libgcc/lib1funcs.s), and the AABB builders return their box by value.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: sub_8019718, GetDingodileHits. Matched
 * anyway.
 *
 * UpdateDingodileShield is a NAKED transcription (its C is kept under
 * NON_MATCHING); everything else is real C. */

struct vtable
{
    u8 unk_00[0x18];
    struct actor_method m18; // 0x18
    struct actor_method m20; // 0x20
    struct actor_method m28; // 0x28
    struct actor_method m30; // 0x30
    struct actor_method m38; // 0x38
    struct actor_method m40; // 0x40
    struct actor_method m48; // 0x48
    struct actor_method m50; // 0x50
    struct actor_method m58; // 0x58
    struct actor_method m60; // 0x60
    struct actor_method m68; // 0x68
};

struct anim_rec
{
    u8 unk_00[0x16];
    u8 frameCount; // 0x16
    u8 unk_17[5];
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
    u8 kind;               // 0x0A - object kind passed to the hit handlers (0x13: attacking player)
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
    s32 rampXStart;            // 0x48
    s32 rampXStep;            // 0x4C
    s32 rampXTarget;            // 0x50
    s32 rampYStart;            // 0x54
    s32 rampYStep;            // 0x58
    s32 rampYTarget;            // 0x5C
    s32 speedX;            // 0x60
    s32 speedY;            // 0x64
};

#define PART_OFFSET(f) ((u32)&((struct part *)0)->f)
COMPILE_TIME_ASSERT(dingodile_c, PART_OFFSET(vt) == 0x18);
COMPILE_TIME_ASSERT(dingodile_c, PART_OFFSET(f28) == 0x28);
COMPILE_TIME_ASSERT(dingodile_c, PART_OFFSET(tag) == 0x2D);
COMPILE_TIME_ASSERT(dingodile_c, PART_OFFSET(frame) == 0x30);
COMPILE_TIME_ASSERT(dingodile_c, PART_OFFSET(ctl) == 0x44);

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

/* gDingodileVtable's class (per-frame update UpdateDingodile, destructor
 * DestroyDingodile in the next asm file). */
struct dingodile_boss
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

/* gDingodileShieldVtable's class (0x28 bytes; constructor CreateDingodileShieldCtrl in
 * the next asm file, destructor DestroyDingodileShieldCtrl, update UpdateDingodileShield). */
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

/* gDingodileSharkVtable's class (constructor CreateDingodileSharkCtrl, destructor
 * DestroyDingodileSharkCtrl, update UpdateDingodileShark). */
struct obj_483c
{
    u8 unk_00[8];
    s32 state;         // 0x08
    struct vtable *vt; // 0x0C
};

/* gDingodileProjectileVtable's class (constructor CreateDingodileProjectileCtrl, destructor
 * DestroyDingodileProjectileCtrl, update UpdateDingodileProjectile). */
struct obj_48a4
{
    u8 unk_00[8];
    s32 state;           // 0x08
    struct vtable *vt;   // 0x0C
    u8 unk_10[0xC];
    struct part *target; // 0x1C
};

typedef void (*method1_fn)(void *self, s32 a);
typedef void (*method2_fn)(void *self, void *a, s32 b);
typedef void (*method3_fn)(void *self, s32 a, s32 b, s32 c);
typedef u8 (*query_fn)(void *self);

#define VCALL1(obj, m, a)                                                      \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &((struct vobj *)(obj))->vt->m;                   \
        ((method1_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));          \
    } while (0)
#define VCALL2(obj, m, a, b)                                                   \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &((struct vobj *)(obj))->vt->m;                   \
        ((method2_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (void *)(a), (s32)(b)); \
    } while (0)

/* VCALL1 as a plain block: `do { } while (0)` is not neutral under
 * agbcc (its loop notes change allocation), and some call sites only
 * match without it. */
#define VCALL1_B(obj, m, a)                                                    \
    {                                                                          \
        struct actor_method *_m = &((struct vobj *)(obj))->vt->m;                   \
        ((method1_fn)_m->fn)((u8 *)(obj) + _m->thisOffset, (s32)(a));          \
    }

/* VCALL2 split in two, for call sites that share one indirect call:
 * load `this`/function/first argument here, then `goto` the call. */
#define PREP_VCALL2(obj, m, a_)                                                \
    do                                                                         \
    {                                                                          \
        struct actor_method *_m = &((struct vobj *)(obj))->vt->m;                   \
        t = (u8 *)(obj) + _m->thisOffset;                                      \
        fn = _m->fn;                                                           \
        a = (a_);                                                              \
    } while (0)

/* The ROM re-reads a just-filled box's `w` (a box with no width is
 * empty) straight from its stack slot rather than through the register
 * already holding the box's address; a volatile read is what stops
 * gcc's CSE from rewriting the address. */
#define BOX_VALID(bx) (*(vs32 *)&(bx).w)

/* Right edge of the level, in Q8 units. */
static inline s32 LevelRight(void)
{
    return gLevelLayers->layer0->widthPx << 8;
}

/* Bottom edge of the level, in Q8 units. */
static inline s32 LevelBottom(void)
{
    return gLevelLayers->layer0->heightPx << 8;
}

static inline s32 AtLevelEdge(struct part_f28 *f, s32 x)
{
    if (f->facing)
        return x <= 0x2000;
    else
        return x >= LevelRight() - 0x2000;
}

/* Close enough to the player (on the side it is facing) to react. */
static inline void Approach(struct dingodile_boss *self, struct part *other, s32 d)
{
    if (d <= 0x1FFF)
        SetDingodileState(self, other, 5);
}

static inline void SetTag(struct part *p, u8 tag)
{
    p->tag = tag;
}

/* (u16)(width + n), computed the way the ROM does it: in the upper
 * halfword, then shifted back down. */
#define LayerWidthPlus(n) (((gLevelLayers->layer0->widthPx << 16) + ((n) << 16)) >> 16)

static inline void MarkCollected(struct part *p)
{
    p->fl.b.gone = 1;
    if (p->id != 0xFFFF)
    {
        s32 id = p->id;
        struct entity_flags *ls = gEntityFlags;
        s32 w = id;

        w /= 32;
        ls->bits0Copy[w] |= 1 << (id - w * 32);
    }
}

void sub_801967C(void *self, u8 flag)
{
    s32 i;
    s32 n = gUnknown_030012EC->count;

    for (i = 0; i < n; i++)
    {
        struct part *p = (struct part *)gUnknown_030012EC->items[i];

        if (flag)
            p->kind = 1;
        else
            p->kind = flag;
    }
}

void SetCortexTargetDest(struct obj_4704 *self, s32 *origin, s32 x, s32 y)
{
    u8 v;

    self->x = x;
    self->y = y;
    self->dx = x - origin[0];
    self->dy = y - origin[1];
    v = *(self->src->index + gCortexTargetHopSteps);
    self->unk_28 = v;
    self->unk_24 = v;
}

void DestroyCortexTargetCtrl(struct obj_4704 *self, s32 flags)
{
    self->vt = (struct vtable *)gCortexTargetVtable;
    DestroyCtrl(self, flags);
}

struct obj_4704 *CreateCortexTargetCtrl(struct obj_4704 *self, void *src)
{
    InitCtrl(self);
    self->vt = (struct vtable *)gCortexTargetVtable;
    self->unk_24 = 0;
    self->src = src;
    return self;
}

/* UNUSED - no caller or pointer anywhere in the ROM. */
void sub_8019718(struct vobj *self, s32 unused, s32 arg)
{
    VCALL1(self, m20, arg);
}

void UpdateCortexCannon(struct obj_476c *self, struct part *other)
{
    if (self->state == 0)
        other->fl.b.shown = 0;
}

void DestroyCortexCannonCtrl(struct obj_476c *self, s32 flags)
{
    self->vt = (struct vtable *)gCortexCannonVtable;
    DestroyCtrl(self, flags);
}

struct obj_476c *CreateCortexCannonCtrl(struct obj_476c *self)
{
    InitCtrl(self);
    self->vt = (struct vtable *)gCortexCannonVtable;
    return self;
}

void SetCortexBossState(struct obj_476c *self, s32 unused, s32 arg)
{
    if (arg == 3)
    {
        VCALL1(self->part->ctl, m20, 9);
        if (!(u8)HasTurboRun(gLevelState))
            SpawnBodySlamPower(0xFFFF, 0x8C, 0x98, 0);
    }
    VCALL1(self, m20, arg);
}

void DestroyCortexBoss(struct vobj *self, s32 flags)
{
    self->vt = (struct vtable *)gCortexBossVtable;
    DestroyBossCtrl(self, flags);
}

struct vobj *CreateCortexBoss(struct vobj *self)
{
    CreateBossCtrl(self);
    self->vt = (struct vtable *)gCortexBossVtable;
    return self;
}

/* UNUSED - no caller or pointer anywhere in the ROM. */
s32 GetDingodileHits(struct dingodile_boss *self)
{
    return self->hits;
}

void UpdateDingodile(struct dingodile_boss *self, struct part *other)
{
    struct aabb hurt;
    struct aabb box;
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
    hurt = GetSpriteBodyBox_s(other);
    if (self->state == 8 && gPlayer->kind == 0x13)
    {
        box = GetSpriteAttackBox_s(gPlayer);
        if (BOX_VALID(box) && AabbOverlaps(&box, &hurt))
        {
            self->hits++;
            SetDingodileState(self, other, 11);
        }
    }

    state = self->state;
    switch (state)
    {
    case 0:
        SetDingodileState(self, other, 1);
        self->step = 0;
        self->passes = 0;
        other->kind = 0;
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
                const s32 *tbl = gDingodileStopXLeft;
                if (self->hits > 0)
                    tbl = gDingodileStopXLeftHurt;
                if (x <= tbl[self->step])
                {
                    Approach(self, other, gPlayer->x - x);
                    self->step++;
                    break;
                }
            }
            else
            {
                const s32 *tbl = gDingodileStopXRight;
                if (self->hits > 0)
                    tbl = gDingodileStopXRightHurt;
                if (x >= tbl[self->step])
                {
                    Approach(self, other, x - gPlayer->x);
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
                SetDingodileState(self, other, 6);
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

            SetDingodileState(self, other, 1);
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
                    SetDingodileState(self, other, 2);
                }
                else
                {
                    self->timer = 0x64;
                    self->nextState = 1;
                    SetDingodileState(self, other, 2);
                }
                self->step = 0;
            }
            else
            {
                SetDingodileState(self, other, 14);
            }
        }
        break;
    case 15:
        if (other->f28.facing)
            SpawnDingodileShark(self, 0, 0x2D, 0);
        else
            SpawnDingodileShark(self, LayerWidthPlus(0x28), 0x2D, 1);
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
                SpawnDingodileShieldOrRocket(self, 1, (other->x >> 8) + 6, (other->y >> 8) - 0x32, other);
            else
                SpawnDingodileShieldOrRocket(self, 1, (other->x >> 8) - 6, (other->y >> 8) - 0x32, other);
        }
        if (!other->animDone)
            break;
        if (self->state == 6)
        {
            other->fl.b.hit = 1;
        turn:
            SetDingodileState(self, other, 3);
            break;
        }
        SetDingodileState(self, other, 1);
        {
            s32 n = 8;
            if (n >= other->table->recs[other->tag].frameCount)
                n = other->table->recs[other->tag].frameCount - 1;
            other->frame = n;
        }
        break;
    case 2:
        if (--self->timer == 0)
            SetDingodileState(self, other, self->nextState);
        break;
    case 7:
        StartDingodileMotion(self, (struct gobj *)other, 0);
        VCALL2(self, m50, other, 5);
        SetDingodileState(self, other, 8);
        break;
    case 8:
        if (--self->timer == 0)
            SetDingodileState(self, other, 9);
        break;
    case 9:
    case 10:
        if (!other->animDone)
            break;
        if (state == 9)
        {
        idle:
            SetDingodileState(self, other, 1);
            break;
        }
        SetDingodileState(self, other, 12);
        break;
    case 11:
        if (self->timer != 0)
        {
            self->timer--;
            break;
        }
        if (self->hits > 2)
            SetDingodileState(self, other, 16);
        if (other->animDone)
            SetDingodileState(self, other, 10);
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
        SetDingodileState(self, other, 13);
        break;
    }
    case 16:
    {
        s32 y = other->y;

        if (y >= LevelBottom() + 0x2000)
        {
            StartDingodileMotion(self, (struct gobj *)other, 0);
            if ((u8)HasSuperBodySlam(gLevelState))
                RequestRoomExit();
            SetDingodileState(self, other, 17);
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
void SetDingodileState(struct dingodile_boss *self, struct part *other, s32 next)
{
    register void *t asm("r0");
    register void *a asm("r1");
    register s32 b asm("r2");
    register void *fn asm("r3");

    VCALL1(self, m20, next);
    switch (next)
    {
    case 16:
        if (!(u8)HasSuperBodySlam(gLevelState))
            SpawnTurboRunPower(0xFFFF, 0xA0, 0xA9, 0);
        StartDingodileMotion(self, (struct gobj *)other, 3);
        break;
    case 12:
        SpawnDingodileShark(self, gLevelLayers->layer0->widthPx, 0x28, 1);
        SpawnDingodileShark(self, 0, 0x46, 0);
        SpawnDingodileShark(self, LayerWidthPlus(0x46), 0x64, 1);
    case 1:
    case 14:
        VCALL2(self, m50, other, 0);
        StartDingodileMotion(self, (struct gobj *)other, 1);
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
        StartDingodileMotion(self, (struct gobj *)other, 0);
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
        PlaySfx(gAudioContext, 0x15, 0x100);
        other->fl.b.shown = 0;
        StartDingodileMotion(self, (struct gobj *)other, 2);
        VCALL2(self, m50, other, 1);
        break;
    }
}

void SpawnDingodileShieldOrRocket(struct dingodile_boss *self, s32 mode, u16 x, u16 y, struct part *arg)
{
    struct part *p = CreateMovingSprite(0xFFFF, x, y, 0);
    struct vobj *ctl;
    u8 *bits;

    p->fl.b.shown = 0;
    p->table = (void *)(SPRITE_BANK_BASE + 0x288);
    switch (mode)
    {
    case 0:
        {
            s32 kind = 1;

            p->f28.mode = kind;
            SetTag(p, 3);
            ResetSpriteFrameTimer(p);
            ResetSpriteFrameIndex(p);
            SetSpriteAnimDone(p, 0);
            p->kind = kind;
        }
        ctl = (struct vobj *)CreateDingodileShieldCtrl(OperatorNew(0x28));
        ((struct obj_490c *)ctl)->target = arg;
        self->part = p;
        break;
    case 1:
        PlaySfx(gAudioContext, 0x29, 0x100);
        SetTag(p, 7);
        ResetSpriteFrameTimer(p);
        ResetSpriteFrameIndex(p);
        SetSpriteAnimDone(p, 0);
        p->kind = 4;
        ctl = (struct vobj *)CreateDingodileProjectileCtrl(OperatorNew(0x20));
        ((struct obj_48a4 *)ctl)->target = arg;
        break;
    default:
        ctl = NULL;
        break;
    }
    p->slot = GetSpriteAnimPaletteSlot((struct actor *)p);
    p->ctl = ctl;
    VCALL1(ctl, m18, p);
    bits = &((u8 *)gEntityFlags->list->params)[*gEntityFlags->list->paramOffsets];
    p->f28.facing = ((*bits >> 1) ^ 1) & 1;
    p->f28.flag5 = (*bits >> 2) & 1;
    p->fl.b.active = 1;
    if (mode == 0)
        AddToPartList(gUnknown_030012EC, p);
    else
        AddToPartList(gCollidableList, p);
}

/* Spawns one of the boss's floor-tile parts (record index 1 of the
 * `+0x30` table, `kind` 6) at (x, y), with a gDingodileSharkVtable
 * controller, facing `facing`, and registers it with gCollidableList.
 *
 * The two virtual calls are written as plain blocks, not VCALL1's
 * `do { } while (0)` (whose loop notes swap the part/controller
 * registers), and `facing` goes into the 1-bit field unmasked (an
 * explicit `& 1` makes the tag store reuse the held constant 1). */
void SpawnDingodileShark(struct dingodile_boss *self, u16 x, u16 y, u8 facing)
{
    struct part *p = CreateMovingSprite(0xFFFF, x, y, 0);
    struct vobj *ctl;

    p->table = (void *)(SPRITE_BANK_BASE + 0x30);
    SetTag(p, 1);
    ResetSpriteFrameTimer(p);
    ResetSpriteFrameIndex(p);
    SetSpriteAnimDone(p, 0);
    p->kind = 6;
    ctl = CreateDingodileSharkCtrl(OperatorNew(0x8C));
    p->slot = GetSpriteAnimPaletteSlot((struct actor *)p);
    p->ctl = ctl;
    VCALL1_B(ctl, m18, p)
    p->f28.facing = facing;
    p->fl.b.active = 1;
    VCALL1_B(ctl, m18, p)
    AddToPartList(gCollidableList, p);
}

/* gDingodileShieldVtable's per-frame update (this controller is created
 * by SpawnDingodileShieldOrRocket mode 0; `other` is the part it drives): while the
 * player isn't dead
 * (gPlayer->dead) and `other` reports a hit (its own table
 * slot +0x28), overlaps `other`'s box with the player's hurt box (falling
 * back to the player's plain box) and on contact fires the player's slot
 * +0x68 method with `other->kind`. Then: state 0 writes BLDCNT/
 * BLDALPHA (1st target OBJ, 2nd target BG0-3+OBJ, EVA=EVB=16) and moves
 * to state 5; states 1/2 arm a two-blink countdown and move to 3/4;
 * states 3/4 toggle `other`'s blink bit every 20 frames until the
 * blinks run out (then state 5).
 *
 * Was NAKED (~159 halfwords off as C). The ROM leaves r4-r6 unused for
 * the long-lived values (`self` r7, `&b` r8, `other` r9,
 * &gPlayer r10); the draft's allocation order was already the
 * ROM's, but it started at r5. Holding r5 and r6 across the box builders
 * (docs/matching/hard-register-hold-retry.md) makes global-alloc skip
 * them. The state-0 BLDCNT accumulator lives in r5 in the ROM: a
 * block-scoped r5 variable, initialised through the constant-init asm so
 * the orr chain is neither folded nor reordered, reproduces it. */
void UpdateDingodileShield(struct obj_490c *self, struct part *other)
{
    struct aabb a;
    struct aabb b;
    register s32 hr5 asm("r5");
    register s32 hr6 asm("r6");

    {
        struct actor_method *m = &other->vt->m28;
        if (((query_fn)m->fn)((u8 *)other + m->thisOffset))
        {
            if (!gPlayer->dead)
            {
                /* Hard-register hold (no code): r5 and r6 stay live
                 * across the box builders, so no long-lived pseudo gets
                 * them. */
                asm("" : "=r"(hr5));
                asm("" : "=r"(hr6));
                a = GetSpriteAttackBox_s(other);
                b = GetSpriteBodyBox_s(gPlayer);
                if (!BOX_VALID(b))
                {
                    struct aabb *pb = &b;

                    *pb = GetSpriteAttackBox_s(gPlayer);
                }
                /* End of the hold. */
                asm("" : : "r"(hr5));
                asm("" : : "r"(hr6));
                if (AabbOverlaps(&b, &a))
                {
                    struct player *pl = gPlayer;
                    const struct actor_method *m2 = &pl->vtable->handleEvent;

                    ((method3_fn)m2->fn)((u8 *)pl + m2->thisOffset, 0, other->kind, 0);
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

void UpdateDingodileProjectile(struct obj_48a4 *self, struct part *other)
{
    struct aabb a;
    struct aabb b;

    a = GetSpriteAttackBox_s(other);
    if (a.w)
    {
        if (self->state != 4 && self->state != 6)
        {
            struct part *t = self->target;
            if ((t->fl.raw >> 6) & 1)
            {
                b = GetSpriteHitbox((struct box_part *)t);
                if (AabbOverlaps(&a, &b))
                {
                    VCALL1(self->target->ctl, m20, 7);
                    VCALL1(self, m20, 6);
                    VCALL2(self, m50, other, 8);
                    PlaySfx(gAudioContext, 0x39, 0x100);
                }
            }
        }
        if (!gPlayer->dead)
        {
            b = GetSpriteBodyBox_s(gPlayer);
            if (!BOX_VALID(b))
            {
                struct aabb *pb = &b;

                *pb = GetSpriteAttackBox_s(gPlayer);
            }
            if (AabbOverlaps(&a, &b))
            {
                struct player *pl = gPlayer;
                const struct actor_method *m2 = &pl->vtable->handleEvent;

                ((method3_fn)m2->fn)((u8 *)pl + m2->thisOffset, 0, other->kind, 0);
                if (self->state == 3)
                {
                    VCALL1(self, m20, 6);
                    VCALL2(self, m50, other, 8);
                    PlaySfx(gAudioContext, 0x39, 0x100);
                }
            }
        }
    }

    switch (self->state)
    {
    case 0:
        VCALL2(self, m30, other, gDingodileRocketRiseMotion);
        VCALL1(self, m20, 1);
        break;
    case 1:
        if (other->y <= 0x800)
        {
            other->speedY = 0;
            other->rampYStart = 0;
            other->rampYStep = 0;
            other->rampYTarget = 0;
            VCALL2(self, m50, other, 8);
            other->slot = GetSpriteAnimPaletteSlot((struct actor *)other);
            SpawnDingodileStalactite(self, other->x >> 8, other->y >> 8);
            VCALL1(self, m20, 2);
        }
        break;
    case 5:
        VCALL2(self, m50, other, 9);
        VCALL2(self, m30, other, gDingodileStalactiteFallMotion);
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
            PlaySfx(gAudioContext, 0x39, 0x100);
        }
        break;
    }
    case 6:
        other->speedY = 0;
        other->rampYStart = 0;
        other->rampYStep = 0;
        other->rampYTarget = 0;
    case 2:
        if (other->animDone)
            MarkCollected(other);
        break;
    }
}

void SpawnDingodileStalactite(struct obj_48a4 *self, u16 x, u16 y)
{
    struct part *p = CreateMovingSprite(0xFFFF, x, y, 0);
    struct obj_48a4 *c;

    p->fl.b.shown = 0;
    p->table = (void *)(SPRITE_BANK_BASE + 0x288);
    SetTag(p, 8);
    ResetSpriteFrameTimer(p);
    ResetSpriteFrameIndex(p);
    SetSpriteAnimDone(p, 0);
    p->kind = 1;
    c = OperatorNew(0x20);
    CreateBossCtrl(c);
    c->vt = (struct vtable *)gDingodileProjectileVtable;
    c->target = self->target;
    VCALL1(c, m20, 5);
    p->slot = GetSpriteAnimPaletteSlot((struct actor *)p);
    p->ctl = c;
    VCALL1(c, m18, p);
    p->fl.b.active = 1;
    AddToPartList(gCollidableList, p);
}

void UpdateDingodileShark(struct obj_483c *self, struct part *other)
{
    switch (self->state)
    {
    case 0:
        if (other->f28.facing)
        {
            s32 a = -gDingodileMotionRecords[3][0];
            s32 c = -gDingodileMotionRecords[3][2];
            s32 b = gDingodileMotionRecords[3][1];
            other->speedX = a;
            other->rampXStart = a;
            other->rampXStep = b;
            other->rampXTarget = c;
        }
        else
        {
            s32 a = gDingodileMotionRecords[3][0];
            s32 b = gDingodileMotionRecords[3][1];
            s32 c = gDingodileMotionRecords[3][2];
            other->speedX = a;
            other->rampXStart = a;
            other->rampXStep = b;
            other->rampXTarget = c;
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

struct vobj *CreateDingodileSharkCtrl(void *mem)
{
    struct vobj *self = mem;

    CreateEnemyCtrl((struct part_ctrl *)self);
    self->vt = (struct vtable *)gDingodileSharkVtable;
    return self;
}

void DestroyDingodileSharkCtrl(struct vobj *self, s32 flags)
{
    self->vt = (struct vtable *)gDingodileSharkVtable;
    DestroyEnemyCtrl((struct part_ctrl *)self, flags);
}

void DestroyDingodileProjectileCtrl(struct obj_48a4 *self, s32 flags)
{
    self->vt = (struct vtable *)gDingodileProjectileVtable;
    self->target = NULL;
    DestroyBossCtrl(self, flags);
}

struct obj_48a4 *CreateDingodileProjectileCtrl(void *mem)
{
    struct obj_48a4 *self = mem;

    CreateBossCtrl(self);
    self->vt = (struct vtable *)gDingodileProjectileVtable;
    return self;
}

void DestroyDingodileShieldCtrl(struct vobj *self, s32 flags)
{
    self->vt = (struct vtable *)gDingodileShieldVtable;
    DestroyBossCtrl(self, flags);
}
