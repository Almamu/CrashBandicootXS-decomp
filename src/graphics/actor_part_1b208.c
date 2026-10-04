#include "core.h"
#include "gobj_1a794.h"

/* GitHub issue #25, ROM 0x0801B208-0x0801B85C: the rest of struct gobj's
 * methods and all of struct mover's (see include/gobj_1a794.h and
 * docs/matching/issue-25-level-objects.md).
 *
 * UNUSED - no caller anywhere in the ROM (checked asm/ .s files, src/ .c files
 * and the ROM for Thumb pointers): sub_801B2E4 (the gobj constructor -
 * sub_801A878 inlines its body instead), sub_801B6EC, sub_801B734,
 * sub_801B854.
 *
 * Register pins and the few one-instruction-wide inline asm operands
 * below are load-bearing (docs/workflow.md step 7); each is commented. */

void sub_801B208(struct gobj *self)
{
    struct method *m = &self->vtable->m38;

    if ((u8)_call_via_r1((u8 *)self + m->thisOffset, m->fn))
    {
        sub_8008044(self);
        OBJ_CALL1(self, m60);
        if (self->type == 6 && self->frame > 0x12)
        {
            struct gobj **c = &gUnknown_030012D8->carried;

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

u8 sub_801B29C(struct gobj *self)
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

s32 sub_801B2C0(void)
{
    return 4;
}

void sub_801B2C4(struct gobj *self, s32 flags)
{
    self->vtable = (struct gobj_vtable *)gStaticData_087E49DC;
    sub_8009F1C(self, flags);
}

void sub_801B2D8(struct gobj *self)
{
    s32 mask = ~0x40;

    self->flags = mask & self->flags;
}

struct gobj *sub_801B2E4(struct gobj *self)
{
    return GobjInit(self);
}

/* &gStaticData_0816C460[self->set->entries[1].a], with the scaled index
 * in r0 as the ROM computes it (docs/workflow.md step 7) */
static inline struct vec3 *MoverVec(struct mover *self)
{
    register u32 off asm("r0") = self->set->entries[1].a * sizeof(struct vec3);
    register u32 base asm("r1") = (u32)gStaticData_0816C460;

    return (struct vec3 *)(off + base);
}

/* struct mover's per-frame step (method table +0x0C). On the first frame
 * each axis with a range starts moving (velocity record 1 of `set`,
 * sign-flipped by dirX/dirY); the distance travelled accumulates in
 * distX/distY and once it exceeds rangeX/rangeY the direction flips.
 * Kinds 5/6/7 add timed behaviour: kind 5 fires method +0x60 60 ticks
 * after the player lands and then wobbles the owner +-3px, kinds 6/7
 * freeze the owner's animation until its +0x38 trigger fires (kind 7 then
 * marks the owner gone in the gEntityFlags+0x108 bitmap, as
 * sub_80072D8 does). Finally sub_801B624 drags the player along.
 *
 * Every `register ... asm()` below pins a value to the register the ROM
 * uses for it; unpinned, this compiler picks a different low register at
 * each site (docs/workflow.md step 7). The small u8/s32 constant
 * variables reproduce the ROM's load order (constant before the ldrb). */
void sub_801B304(struct mover *self, struct gobj *objArg)
{
    register struct gobj *obj asm("r4") = objArg;
    s32 kind;

    if (self->lastX == 0 && self->rangeX > 0)
    {
        register struct vec3 *e asm("r3");

        self->lastX = obj->x >> 8;
        e = MoverVec(self);
        if (self->dirX)
        {
            s32 x = e->x;
            s32 y = e->y;
            s32 z = e->z;

            obj->speedX = x;
            obj->velA.x = x;
            obj->velA.y = y;
            obj->velA.z = z;
        }
        else
        {
            s32 x = -e->x;
            s32 z = -e->z;
            s32 y = e->y;

            obj->speedX = x;
            obj->velA.x = x;
            obj->velA.y = y;
            obj->velA.z = z;
        }
    }
    if (self->lastY == 0 && self->rangeY > 0)
    {
        register struct vec3 *e asm("r3");

        self->lastY = obj->y >> 8;
        e = MoverVec(self);
        if (self->dirY)
        {
            s32 x = e->x;
            s32 y = e->y;
            s32 z = e->z;

            obj->speedY = x;
            obj->velB.x = x;
            obj->velB.y = y;
            obj->velB.z = z;
        }
        else
        {
            s32 x = -e->x;
            s32 z = -e->z;
            s32 y = e->y;

            obj->speedY = x;
            obj->velB.x = x;
            obj->velB.y = y;
            obj->velB.z = z;
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
            if (v >= gStaticData_0816C460[self->set->entries[1].a].z)
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
            if (v >= gStaticData_0816C460[self->set->entries[1].a].z)
                self->distY = 0;
        }
    }

    if (self->distX > self->rangeX && self->rangeX != 0)
    {
        register struct vec3 *e asm("r3");
        register u8 *dp asm("r0") = &self->dirX;
        u8 one = 1;
        register u32 cur asm("r1") = *dp;
        u8 d = one ^ cur;

        *dp = d;

        e = MoverVec(self);
        if (d)
        {
            s32 x = e->x;
            s32 y = e->y;
            s32 z = e->z;

            obj->velA.x = x;
            obj->velA.y = y;
            obj->velA.z = z;
        }
        else
        {
            s32 x = -e->x;
            s32 z = -e->z;
            s32 y = e->y;

            obj->velA.x = x;
            obj->velA.y = y;
            obj->velA.z = z;
        }
        self->distX = -1;
    }
    if (self->distY > self->rangeY && self->rangeY != 0)
    {
        register struct vec3 *e asm("r3");
        register u8 *dp asm("r0") = &self->dirY;
        u8 one = 1;
        register u32 cur asm("r5") = *dp;
        u8 d = one ^ cur;

        *dp = d;

        e = MoverVec(self);
        if (d)
        {
            s32 x = e->x;
            s32 y = e->y;
            s32 z = e->z;

            obj->velB.x = x;
            obj->velB.y = y;
            obj->velB.z = z;
        }
        else
        {
            s32 x = -e->x;
            s32 z = -e->z;
            s32 y = e->y;

            obj->velB.x = x;
            obj->velB.y = y;
            obj->velB.z = z;
        }
        self->distY = -1;
    }

    kind = self->kind;
    if (kind == 5 && self->timer > 0 && gUnknown_0300082C - self->timer == 60)
    {
        MOVER_CALL3(self, m60, obj, 3);
        self->timer = -1;
    }
    else if (kind == 5 && self->timer > 0)
    {
        u32 now = gUnknown_0300082C;

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
         || (kind == 6 && obj->frame <= 1 && gUnknown_0300082C < self->time))
            goto clamp;
        if (kind == 7 && obj->unk_38)
        {
            register u32 bit asm("r0") = 1;

            obj->flags = bit | obj->flags;
            {
                register s32 none asm("r0") = 0xFFFF;
                register u32 cur asm("r2") = obj->id;

                if (cur != none)
                {
                    register s32 id asm("r3") = *(vu16 *)&obj->id;
                    u8 *base = gEntityFlags;
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
        else if (kind == 6 && obj->unk_38)
        {
            self->time = gUnknown_0300082C + 120;
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
    sub_801B624(self, obj);
    self->lastX = obj->x >> 8;
    self->lastY = obj->y >> 8;
}

/* While the mover is active (the player is standing on its owner), move
 * the player by the owner's displacement since last frame, mark the owner
 * as the player's `carried` object, and fold the owner's velocity signs
 * into the player's +0x24 direction bits. Pins as in sub_801B304. */
void sub_801B624(struct mover *self, struct gobj *obj)
{
    if (self->active && self->kind != 6)
    {
        struct gobj **pp = &gUnknown_030012D8;
        struct gobj *p = *pp;
        register u32 f asm("r1") = p->flags;
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
                struct gobj *q = *pp;
                register s32 px asm("r1") = q->x >> 8;
                register s32 dx asm("r5") = (obj->x >> 8) - self->lastX;
                register s32 py asm("r2") = q->y >> 8;
                register s32 dy asm("r3") = (obj->y >> 8) - self->lastY;

                px += dx;
                py += dy;
                q->x = px << 8;
                q->y = py << 8;
                sub_8009EA8(q);
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
            gUnknown_030012D8->dir = dir;
            if (self->kind == 5 && self->timer == 0)
                self->timer = gUnknown_0300082C;
        }
    }
}

void sub_801B6EC(struct mover *self, struct gobj *part, s32 index)
{
    struct vec3 *e = &gStaticData_0816C460[self->set->entries[index].b];

    if ((s32)(part->mirror << 26) < 0)
    {
        s32 x = -e->x;
        s32 z = -e->z;
        s32 y = e->y;

        part->velB.x = x;
        part->velB.y = y;
        part->velB.z = z;
    }
    else
    {
        s32 x = e->x;
        s32 y = e->y;
        s32 z = e->z;

        part->velB.x = x;
        part->velB.y = y;
        part->velB.z = z;
    }
}

void sub_801B734(struct mover *self, struct gobj *part, s32 index)
{
    struct vec3 *e = &gStaticData_0816C460[self->set->entries[index].a];

    if ((s32)(part->mirror << 27) < 0)
    {
        s32 x = -e->x;
        s32 z = -e->z;
        s32 y = e->y;

        part->velA.x = x;
        part->velA.y = y;
        part->velA.z = z;
    }
    else
    {
        s32 x = e->x;
        s32 y = e->y;
        s32 z = e->z;

        part->velA.x = x;
        part->velA.y = y;
        part->velA.z = z;
    }
}

void sub_801B77C(struct mover *self, struct gobj *part, s32 index)
{
    sub_800B6D0(self, part, &gStaticData_0816C460[self->set->entries[index].b]);
}

void sub_801B7A0(struct mover *self, struct gobj *part, s32 index)
{
    sub_800B7B0(self, part, &gStaticData_0816C460[self->set->entries[index].a]);
}

void sub_801B7C4(struct mover *self, s32 flags)
{
    self->vtable = (struct mover_vtable *)gStaticData_087E4A54;
    sub_800B8A8(self, flags);
}

/* struct mover's constructor. The 5th argument arrives on the stack as a
 * genuine byte, which the ROM reads with `add r0, sp, #0x18; ldrb` - this
 * compiler would load the whole word and mask it, so only the address is
 * computed in asm and the byte load itself is plain C. */
struct mover *sub_801B7D8(struct mover *self, s32 distX, s32 distY, u32 dirXArg, u8 dirY, s32 kind)
{
    register u8 *dyp asm("r0");
    u8 dy;
    u8 dirX;

    asm("add %0, sp, #0x18" : "=r"(dyp));
    dirX = dirXArg;
    dy = *dyp;

    sub_800B8C8(self);
    self->vtable = (struct mover_vtable *)gStaticData_087E4A54;
    if ((u32)(kind - 6) <= 1)
    {
        distY = 0;
        distX = 0;
    }
    self->lastX = 0;
    self->distX = distX;
    self->lastY = 0;
    self->distY = distY;
    self->set = (void *)gStaticData_0816C458;
    self->active = 0;
    self->kind = kind;
    self->timer = 0;
    self->rangeX = distX * 2;
    self->rangeY = distY * 2;
    self->dirX = dirX;
    self->dirY = dy;
    self->time = gUnknown_0300082C + 0x78;
    return self;
}

void sub_801B854(struct mover *self)
{
    self->active = 0;
}
