#include "core.h"
#include "gobj_1a794.h"
#include "objects.h"
#include "globals.h"

/* codegen: SetSpritePrevPos takes (part, x, y) (objects.h), but
 * MovePlayerWithPlatform passes only the part and leaves r1/r2 as they
 * are. docs/headers_plan.md */
extern void SetSpritePrevPos_1(struct gobj *self) asm("SetSpritePrevPos");

/* GitHub issue #25, ROM 0x0801B208-0x0801B85C: the rest of struct gobj's
 * methods and all of struct mover's (see include/gobj_1a794.h and
 * docs/matching/archive/issue-25-level-objects.md).
 *
 * UNUSED - no caller anywhere in the ROM (checked asm/ .s files, src/ .c files
 * and the ROM for Thumb pointers): InitPlatform (the gobj constructor -
 * CreatePlatform inlines its body instead), SetPlatformMoverMotionYFromSet, SetPlatformMoverMotionXFromSet,
 * ClearPlatformMoverActive.
 *
 * Register pins and the few one-instruction-wide inline asm operands
 * below are load-bearing (docs/workflow.md step 7); each is commented. */

void UpdatePlatform(struct gobj *self)
{
    struct actor_method *m = &self->vtable->m38;

    if ((u8)_call_via_r1((u8 *)self + m->thisOffset, m->fn))
    {
        AdvanceSpriteAnim((struct box_part *)self);
        OBJ_CALL1(self, m60);
        if (self->type == 6 && self->frame > 0x12)
        {
            struct gobj **c = &gPlayer->carried;

            if (*c == self)
                *c = NULL;
        }
        if (self->mover)
            MOVER_CALL2(self->mover, m08, self);
    }
    else
    {
        OBJ_CALL1(self, m60);
        if (self->mover)
            MOVER_CALL2(self->mover, m08, self);
    }
}

s32 sub_801B29C(struct gobj *self)
{
    return (self->flags2 >> 4) & 1;
}

void sub_801B2A8(struct gobj *self, u8 value)
{
    u32 one = 1;
    u32 bit;
    s32 mask;

    /* hide the constant 1 from reload's cse, which would otherwise build
     * the mask below as `1 - 0x12` (docs/workflow.md step 7) */
    asm("" : "+r"(one));
    bit = (value & one) << 4;
    mask = ~0x10;
    self->flags2 = (mask & self->flags2) | bit;
}

s32 GetPlatformClassId(void)
{
    return 4;
}

void DestroyPlatform(struct gobj *self, s32 flags)
{
    self->vtable = (struct gobj_vtable *)gPlatformVtable;
    DestroyMovingSprite((struct actor *)self, flags);
}

void sub_801B2D8(struct gobj *self)
{
    s32 mask = ~0x40;

    self->flags = mask & self->flags;
}

struct gobj *InitPlatform(struct gobj *self)
{
    return GobjInit(self);
}

/* &gPlatformMoverMotionRecords[self->set->entries[1].a], with the scaled index
 * in r0 as the ROM computes it (docs/workflow.md step 7) */
static inline struct speed_ramp *MoverVec(struct mover *self)
{
    register u32 off asm("r0") = self->set->entries[1].a * sizeof(struct speed_ramp);
    register u32 base asm("r1") = (u32)gPlatformMoverMotionRecords;

    return (struct speed_ramp *)(off + base);
}

/* struct mover's per-frame step (method table +0x0C). On the first frame
 * each axis with a range starts moving (velocity record 1 of `set`,
 * sign-flipped by dirX/dirY); the distance travelled accumulates in
 * distX/distY and once it exceeds rangeX/rangeY the direction flips.
 * Kinds 5/6/7 add timed behaviour: kind 5 fires method +0x60 60 ticks
 * after the player lands and then wobbles the owner +-3px, kinds 6/7
 * freeze the owner's animation until its +0x38 trigger fires (kind 7 then
 * marks the owner gone in the gEntityFlags+0x108 bitmap, as
 * MarkEntityGone does). Finally MovePlayerWithPlatform drags the player along.
 *
 * Every `register ... asm()` below pins a value to the register the ROM
 * uses for it; unpinned, this compiler picks a different low register at
 * each site (docs/workflow.md step 7). The small u8/s32 constant
 * variables reproduce the ROM's load order (constant before the ldrb). */
void UpdatePlatformMover(struct mover *self, struct gobj *objArg)
{
    register struct gobj *obj asm("r4") = objArg;
    s32 kind;

    if (self->lastX == 0 && self->rangeX > 0)
    {
        register struct speed_ramp *e asm("r3");

        self->lastX = obj->x >> 8;
        e = MoverVec(self);
        if (self->dirX)
        {
            s32 x = e->start;
            s32 y = e->step;
            s32 z = e->target;

            obj->speedX = x;
            obj->rampX.start = x;
            obj->rampX.step = y;
            obj->rampX.target = z;
        }
        else
        {
            s32 x = -e->start;
            s32 z = -e->target;
            s32 y = e->step;

            obj->speedX = x;
            obj->rampX.start = x;
            obj->rampX.step = y;
            obj->rampX.target = z;
        }
    }
    if (self->lastY == 0 && self->rangeY > 0)
    {
        register struct speed_ramp *e asm("r3");

        self->lastY = obj->y >> 8;
        e = MoverVec(self);
        if (self->dirY)
        {
            s32 x = e->start;
            s32 y = e->step;
            s32 z = e->target;

            obj->speedY = x;
            obj->rampY.start = x;
            obj->rampY.step = y;
            obj->rampY.target = z;
        }
        else
        {
            s32 x = -e->start;
            s32 z = -e->target;
            s32 y = e->step;

            obj->speedY = x;
            obj->rampY.start = x;
            obj->rampY.step = y;
            obj->rampY.target = z;
        }
    }

    {
        s32 d = self->distX;

        if (d != -1)
        {
            s32 v = (obj->x >> 8) - self->lastX;
            s32 sign;

            ABS32(v, sign);
            self->distX = d + v;
        }
        else
        {
            register s32 v asm("r2") = obj->speedX;
            s32 sign;

            ABS32(v, sign);
            if (v >= gPlatformMoverMotionRecords[self->set->entries[1].a].target)
                self->distX = 0;
        }
    }
    {
        s32 d = self->distY;

        if (d != -1)
        {
            s32 v = (obj->y >> 8) - self->lastY;
            s32 sign;

            ABS32(v, sign);
            self->distY = d + v;
        }
        else
        {
            register s32 v asm("r2") = obj->speedY;
            s32 sign;

            ABS32(v, sign);
            if (v >= gPlatformMoverMotionRecords[self->set->entries[1].a].target)
                self->distY = 0;
        }
    }

    if (self->distX > self->rangeX && self->rangeX != 0)
    {
        register struct speed_ramp *e asm("r3");
        register u8 *dp asm("r0") = &self->dirX;
        u8 one = 1;
        register u32 cur asm("r1") = *dp;
        u8 d = one ^ cur;

        *dp = d;

        e = MoverVec(self);
        if (d)
        {
            s32 x = e->start;
            s32 y = e->step;
            s32 z = e->target;

            obj->rampX.start = x;
            obj->rampX.step = y;
            obj->rampX.target = z;
        }
        else
        {
            s32 x = -e->start;
            s32 z = -e->target;
            s32 y = e->step;

            obj->rampX.start = x;
            obj->rampX.step = y;
            obj->rampX.target = z;
        }
        self->distX = -1;
    }
    if (self->distY > self->rangeY && self->rangeY != 0)
    {
        register struct speed_ramp *e asm("r3");
        register u8 *dp asm("r0") = &self->dirY;
        u8 one = 1;
        register u32 cur asm("r5") = *dp;
        u8 d = one ^ cur;

        *dp = d;

        e = MoverVec(self);
        if (d)
        {
            s32 x = e->start;
            s32 y = e->step;
            s32 z = e->target;

            obj->rampY.start = x;
            obj->rampY.step = y;
            obj->rampY.target = z;
        }
        else
        {
            s32 x = -e->start;
            s32 z = -e->target;
            s32 y = e->step;

            obj->rampY.start = x;
            obj->rampY.step = y;
            obj->rampY.target = z;
        }
        self->distY = -1;
    }

    kind = self->kind;
    if (kind == 5 && self->timer > 0 && gRoomFrameCount - self->timer == 60)
    {
        MOVER_CALL3(self, m60, obj, 3);
        self->timer = -1;
    }
    else if (kind == 5 && self->timer > 0)
    {
        u32 now = gRoomFrameCount;

        if (__umodsi3(now - self->timer, 30) <= 4)
        {
            if (!(now & 1))
                obj->y += -0x300;
            else
            {
                s32 y = obj->y;
                register s32 k asm("r2") = 0x300;

                asm("" : "+r"(k));
                obj->y = y + k;
            }
            goto done;
        }
        goto check_rest;
    }
    else
    {
    check_rest:
        if ((kind == 7 && obj->frame <= 1 && !self->active)
         || (kind == 6 && obj->frame <= 1 && gRoomFrameCount < self->time))
            goto clamp;
        if (kind == 7 && obj->animDone)
        {
            register u32 bit asm("r0") = 1;

            obj->flags = bit | obj->flags;
            {
                register s32 none asm("r0") = 0xFFFF;
                register u32 cur asm("r2") = obj->id;

                if (cur != none)
                {
                    register s32 id asm("r3") = *(vu16 *)&obj->id;
                    u8 *base = (u8 *)gEntityFlags;
                    register s32 word asm("r0") = id;
                    s32 off;
                    u32 *slot;

                    word /= 32;
                    off = word * 4;
                    slot = (u32 *)(base + 0x108);
                    slot = (u32 *)((u8 *)slot + off);
                    word = id - word * 32;
                    *slot |= 1 << word;
                }
            }
        }
        else if (kind == 6 && obj->animDone)
        {
            self->time = gRoomFrameCount + 120;
        clamp:
            {
                register s32 f asm("r3") = 0;
                struct anim_table *anim = obj->anim;
                register u8 *tp asm("r2") = &obj->tag;
                struct anim_rec *recs = anim->records;
                register u32 tag asm("r5") = *tp;
                s32 n = recs[tag].frames;

                if (f >= n)
                    f = n - 1;
                obj->frame = f;
            }
        }
    }
done:
    MovePlayerWithPlatform(self, obj);
    self->lastX = obj->x >> 8;
    self->lastY = obj->y >> 8;
}

/* While the mover is active (the player is standing on its owner), move
 * the player by the owner's displacement since last frame, mark the owner
 * as the player's `carried` object, and fold the owner's velocity signs
 * into the player's +0x24 direction bits. Pins as in UpdatePlatformMover. */
void MovePlayerWithPlatform(struct mover *self, struct gobj *obj)
{
    if (self->active && self->kind != 6)
    {
        struct player **pp = &gPlayer;
        struct player *p = *pp;
        register u32 f asm("r1") = p->flags.all;
        register u32 top asm("r0") = f >> 7;

        if (top)
        {
            u8 dir;

            {
                register struct gobj **c asm("r0") = &p->carried;

                *c = obj;
                {
                    register u32 v asm("r1") = 8;

                    /* p->unk_68, addressed off &p->carried as the ROM does */
                    *((u8 *)c - (0xAC - 0x68)) = v;
                }
            }
            {
                struct player *q = *pp;
                register s32 px asm("r1") = q->x >> 8;
                register s32 dx asm("r5") = (obj->x >> 8) - self->lastX;
                register s32 py asm("r2") = q->y >> 8;
                register s32 dy asm("r3") = (obj->y >> 8) - self->lastY;

                px += dx;
                py += dy;
                q->x = px << 8;
                q->y = py << 8;
                SetSpritePrevPos_1((struct gobj *)q);
            }
            dir = (*pp)->dir;
            if (obj->speedX > 0)
                dir |= 1;
            else if (obj->speedX < 0)
                dir |= 2;
            if (obj->speedY > 0)
                dir |= 8;
            else if (obj->speedY < 0)
                dir |= 4;
            gPlayer->dir = dir;
            if (self->kind == 5 && self->timer == 0)
                self->timer = gRoomFrameCount;
        }
    }
}

void SetPlatformMoverMotionYFromSet(struct mover *self, struct gobj *part, s32 index)
{
    const struct speed_ramp *e = &gPlatformMoverMotionRecords[self->set->entries[index].b];

    if ((s32)(part->mirror << 26) < 0)
    {
        s32 x = -e->start;
        s32 z = -e->target;
        s32 y = e->step;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
    else
    {
        s32 x = e->start;
        s32 y = e->step;
        s32 z = e->target;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
}

void SetPlatformMoverMotionXFromSet(struct mover *self, struct gobj *part, s32 index)
{
    const struct speed_ramp *e = &gPlatformMoverMotionRecords[self->set->entries[index].a];

    if ((s32)(part->mirror << 27) < 0)
    {
        s32 x = -e->start;
        s32 z = -e->target;
        s32 y = e->step;

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
    else
    {
        s32 x = e->start;
        s32 y = e->step;
        s32 z = e->target;

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
}

void StartPlatformMoverMotionYFromSet(struct mover *self, struct gobj *part, s32 index)
{
    StartCtrlTargetMotionY(self, part, &gPlatformMoverMotionRecords[self->set->entries[index].b]);
}

void StartPlatformMoverMotionXFromSet(struct mover *self, struct gobj *part, s32 index)
{
    StartCtrlTargetMotionX(self, part, (s32 *)&gPlatformMoverMotionRecords[self->set->entries[index].a]);
}

void DestroyPlatformMover(struct mover *self, s32 flags)
{
    self->vtable = (struct mover_vtable *)gPlatformMoverVtable;
    DestroyCtrl(self, flags);
}

/* struct mover's constructor. The 5th argument arrives on the stack as a
 * genuine byte, which the ROM reads with `add r0, sp, #0x18; ldrb` - this
 * compiler would load the whole word and mask it, so only the address is
 * computed in asm and the byte load itself is plain C. */
struct mover *CreatePlatformMover(struct mover *self, s32 distX, s32 distY, u32 dirXArg, u8 dirY, s32 kind)
{
    register u8 *dyp asm("r0");
    u8 dy;
    u8 dirX;

    asm("add %0, sp, #0x18" : "=r"(dyp));
    dirX = dirXArg;
    dy = *dyp;

    InitCtrl(self);
    self->vtable = (struct mover_vtable *)gPlatformMoverVtable;
    if ((u32)(kind - 6) <= 1)
    {
        distY = 0;
        distX = 0;
    }
    self->lastX = 0;
    self->distX = distX;
    self->lastY = 0;
    self->distY = distY;
    self->set = (void *)&gPlatformMoverMotionSet;
    self->active = 0;
    self->kind = kind;
    self->timer = 0;
    self->rangeX = distX * 2;
    self->rangeY = distY * 2;
    self->dirX = dirX;
    self->dirY = dy;
    self->time = gRoomFrameCount + 0x78;
    return self;
}

void ClearPlatformMoverActive(struct mover *self)
{
    self->active = 0;
}
