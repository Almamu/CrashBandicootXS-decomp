#include "core.h"
#include "gobj_1a794.h"
#include "gfx.h"
#include "objects.h"

/* codegen: GetSpriteHitbox returns the box by value (objects.h); this
 * file was matched against the same call written with the destination
 * as an explicit first argument, and through the struct return gcc adds
 * a temporary on the stack. docs/headers_plan.md */
extern void GetSpriteHitbox_p(struct aabb *dest, void *part) asm("GetSpriteHitbox");

/* GitHub issue #25, ROM 0x0801AB98-0x0801B208: ResolvePlatformCollision, the
 * player-vs-object collision resolver (see include/gobj_1a794.h and
 * docs/matching/issue-25-level-objects.md for what it computes).
 *
 * Built with old_agbcc (OLD_AGBCC_OBJS). Closed in the last-five NAKED
 * retry (docs/matching/last5-naked-retry.md); what it took, beyond the
 * gap4 structure (empty `case 0`, the `Span` inline, the shared
 * `set_hdir` arm):
 * - A hard-register hold on r8 up to the first overlap test, so `self`
 *   can't take r8 and `result` gets it (the ROM has self in sb).
 * - Both FindLineCrossing calls pass `px` as the third argument, reassigned
 *   in each arm (`px = b.x + b.w` / `px = b.x`), and `r` is one
 *   function-level local shared by both classify blocks.
 * - `&b` goes through a `pb` local hidden from cse, so it stays in r4
 *   from the first box build into the no-overlap switch.
 * - The player's position is written as `pos`, then read and written
 *   back only through the `PosPtr` inline (no `pp` pointer local): the
 *   ROM's pointer is a gcse copy inserted after the `&gPlayer`
 *   one.
 * - The vtable call is a `static inline` (`Call68`) calling through the
 *   method's function pointer, not the r4-pinned OBJ_CALL68 macro.
 * - `flags = hdir` in case 1/2 (the commit's hit flag), `(oy << 8) +
 *   y` operand order, `u8 m = 8` for the stand mode, the inline
 *   sign-mask abs, and a hold on r5 over the `result == 0` test so the
 *   reload there takes r0. */

typedef void (*ab98_fn3)(void *self, s32 a, s32 b, s32 c);

/* The player's hit handler (vtable +0x68), called with three arguments. */
static inline void Call68(struct player *obj, s32 a, s32 b, s32 c)
{
    const struct actor_method *m = &obj->vtable->handleEvent;

    ((ab98_fn3)m->fn)((u8 *)obj + m->thisOffset, a, b, c);
}

/* Returns its argument: reading `pos` through it keeps the address in a
 * register, as the ROM does. */
static inline struct pos2 *PosPtr(struct pos2 *p)
{
    return p;
}

/* x + w - o with the three operands evaluated first */
static inline s32 Span(s32 x, s32 w, s32 o)
{
    return x + w - o;
}

void ResolvePlatformCollision(struct gobj *selfArg, void *unused)
{
    struct gobj *self = selfArg;
    struct aabb a;
    struct aabb b;
    struct pos2 pos;
    s32 ox;
    s32 oy;
    s32 hdir;
    s32 vdir;
    s32 side;
    s32 above;
    s32 px;
    s32 py;
    s32 tx;
    s32 ty;
    struct hitbox_quad *box;
    s32 result;
    s32 flags;
    struct aabb *pb;
    s32 r;
    register s32 hold8 asm("r8");

    /* Emits nothing: keeps r8 live up to the first overlap test, so
     * `self` goes to sb and `result` gets r8, as in the ROM. */
    asm("" : "=r"(hold8));
    GetSpriteHitbox_p(&a, self);
    {
        s32 t = gPlayer->x;

        px = t >> 8;
    }
    py = gPlayer->y >> 8;
    pb = &b;
    /* Emits nothing: hides `pb`'s value from cse, so `&b` stays in a
     * register (r4) instead of being re-added to sp at each use. */
    asm("" : "+r"(pb));
    GetSpriteHitbox_p(pb, gPlayer);
    {
        struct player *q = gPlayer;
        struct act_anim_bank *anim = q->anim;
        u32 tag = q->tag;

        box = (struct hitbox_quad *)&anim->records[tag].offX;
    }
    /* Emits nothing: end of the r8 hold. */
    asm("" : : "r"(hold8));
    if (AabbOverlaps(&a, pb))
    {
        result = 0;
        above = 0;
        if (b.y < a.y)
            above = 1;
        tx = GetSpritePrevX((struct gfx_part *)gPlayer);
        ty = GetSpritePrevY((struct gfx_part *)gPlayer);
        side = 2;
        if (px > tx)
            side = 1;
        if ((gPlayer->x >> 8) < (self->x >> 8))
        {
            hdir = 1;
            ox = Span(b.x, b.w, a.x) + 1;
        }
        else
        {
            hdir = 2;
            ox = Span(a.x, a.w, b.x) + 1;
        }
        if ((gPlayer->y >> 8) > (self->y >> 8))
        {
            vdir = 4;
            oy = Span(a.y, a.h, b.y);
        }
        else
        {
            vdir = 8;
            oy = Span(b.y, b.h, a.y);
        }
        if (self->type != 1 && self->type != 5 && self->type != 6)
        {
            if (ty == py)
            {
                if (tx == px)
                {
                    result = hdir;
                    if (oy <= 2)
                        result = 8;
                    goto classified;
                }
                if (GetSpritePrevY((struct gfx_part *)self) == (self->y >> 8) && oy > 2)
                {
                    result = hdir;
                    goto classified;
                }
            }
            if (tx == px && GetSpritePrevX((struct gfx_part *)self) == (self->x >> 8))
                result = vdir;
        }
    classified:
        if (ty <= py && above)
        {
            if (result == 0)
            {
                ty += box->offY + box->h;
                if (ty <= a.y + a.h)
                {
                    if (side == 1)
                    {
                        if (b.x + b.w >= a.x && ox > 2)
                            result = 8;
                    }
                    else
                    {
                        if (b.x <= a.x + a.w && ox > 2)
                            result = 8;
                    }
                    if (result == 0)
                    {
                        py += box->offY + box->h;
                        if (hdir == 1)
                        {
                            tx += box->offX + box->w;
                            px = b.x + b.w;
                            r = FindLineCrossing(tx, ty, px, py, a.x);
                        }
                        else
                        {
                            tx += box->offX;
                            px = b.x;
                            r = FindLineCrossing(tx, ty, px, py, a.x + a.w);
                        }
                        if ((r < 0 && above && oy <= 1) || (r > 0 && r <= a.y))
                            result = 8;
                        else
                            result = hdir;
                    }
                }
                else
                    goto set_hdir;
            }
        }
        else
        {
            register s32 hold5 asm("r5");

            /* Emits nothing: r5 is live across the `result == 0` test,
             * so its reload of `result` takes r0 as in the ROM. */
            asm("" : "=r"(hold5));
            if (result == 0)
            {
                asm("" : : "r"(hold5));
                ty += box->offY;
                if (ty >= a.y)
                {
                    if (side == 1)
                    {
                        if (b.x + b.w >= a.x && ox > 3)
                            result = 4;
                    }
                    else
                    {
                        if (b.x <= a.x + a.w && ox > 3)
                            result = 4;
                    }
                    if (result == 0)
                    {
                        py += box->offY;
                        if (hdir == 1)
                        {
                            tx += box->offX + box->w;
                            px = b.x + b.w;
                            r = FindLineCrossing(tx, ty, px, py, a.x);
                        }
                        else
                        {
                            tx += box->offX;
                            px = b.x;
                            r = FindLineCrossing(tx, ty, px, py, a.x + a.w);
                        }
                        if ((r < 0 && !above && ox > 3) || (r > 0 && r >= a.y + a.h))
                            result = 4;
                        else
                            result = hdir;
                    }
                }
                else
                {
                set_hdir:
                    result = hdir;
                }
            }
        }

        pos.x = gPlayer->x;
        pos.y = gPlayer->y;
        if (ox < 0)
            ox = 0;
        if (oy < 0)
            oy = 0;
        flags = 0;
        switch (result)
        {
        case 0: /* empty case: makes gcc use the ROM's jump table */
            break;
        case 4:
            {
                struct player *q = gPlayer;

                if (!(q->hitAxes & 8))
                {
                    Call68(q, 0, 0xC, 4);
                    PosPtr(&pos)->y = (oy << 8) + PosPtr(&pos)->y;
                }
            }
            break;
        case 8:
            PosPtr(&pos)->y = (PosPtr(&pos)->y - ((oy - 1) << 8)) & ~0xFF;
            break;
        case 1:
        case 2:
            flags = hdir;
            if (flags == 2)
                pos.x = (ox << 8) + pos.x;
            else if (flags == 1)
                pos.x -= ox << 8;
            break;
        }
        if (result == 8 || oy <= 1)
        {
            struct player *q = gPlayer;

            if (!(q->dir & 4) && above)
            {
                q->carried = self;
                {
                    u8 m = 8;

                    q->hitAxes = m;
                }
                {
                    s32 y = gPlayer->y;

                    PosPtr(&pos)->y = y - ((oy - 1) << 8);
                }
                flags = 0;
                pos.x = gPlayer->x;
            }
        }
        SetEntityPos((struct actor *)gPlayer, pos.x, PosPtr(&pos)->y);
        if (flags)
        {
            Call68(gPlayer, 0, 0xC, flags);
            gPlayer->hitMask |= flags;
        }
        if (result == 8)
        {
            s32 type = self->type;

            if (type == 1 || type == 5 || type == 6)
                self->mover->active = 1;
            else
            {
                s32 d = (self->x >> 8) - (gPlayer->x >> 8);
                s32 sign;

                sign = d >> 31;
                d ^= sign;
                d -= sign;
                if (d <= 7)
                {
                    switch (type)
                    {
                    case 2:
                        Call68(gPlayer, 0, 0x11, 0);
                        break;
                    case 3:
                        if (!IsBonusRoundDone(gLevelState) && !gLevelState->timeTrial)
                            Call68(gPlayer, 0, 0xF, 0);
                        break;
                    case 4:
                        if (!IsGemPathDone(gLevelState) && !gLevelState->timeTrial)
                            Call68(gPlayer, 0, 0x10, 0);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        a.y -= 4;
        a.h += 4;
        switch (self->type)
        {
        case 0:
        case 7:
            if (AabbOverlaps(&a, pb))
            {
                struct player *q = gPlayer;

                q->carried = self;
                {
                    u8 m = 8;

                    q->hitAxes = m;
                }
            }
            break;
        case 2:
            if (AabbOverlaps(&a, pb))
            {
                s32 d = (self->x >> 8) - (gPlayer->x >> 8);
                s32 sign;

                sign = d >> 31;
                d ^= sign;
                d -= sign;
                if (d <= 7)
                    Call68(gPlayer, 0, 0x11, 0);
            }
            break;
        case 3:
            if (!IsBonusRoundDone(gLevelState) && !gLevelState->timeTrial
                && AabbOverlaps(&a, pb))
            {
                s32 d = (self->x >> 8) - (gPlayer->x >> 8);
                s32 sign;

                sign = d >> 31;
                d ^= sign;
                d -= sign;
                if (d <= 7)
                    Call68(gPlayer, 0, 0xF, 0);
            }
            break;
        case 4:
            if (!IsGemPathDone(gLevelState) && !gLevelState->timeTrial
                && AabbOverlaps(&a, pb))
            {
                s32 d = (self->x >> 8) - (gPlayer->x >> 8);
                s32 sign;

                sign = d >> 31;
                d ^= sign;
                d -= sign;
                if (d <= 7)
                    Call68(gPlayer, 0, 0x10, 0);
            }
            break;
        case 1:
        case 5:
        case 6:
            if (!AabbOverlaps(&a, pb))
                self->mover->active = 0;
            break;
        }
    }
}
