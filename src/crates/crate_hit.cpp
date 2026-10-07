#include "crate.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include "util.h"
#include "globals.h"
#include "player.h"
}

/* The player's hits on a crate and its stack (#664, include/crate.hpp):
 * QueueCratePlayerCollision's two helpers and the body slam's break. */

/* Whether the player's box `quad`, at (xOffset, yOffset) and mirrored
 * around it as the player is, overlaps `box`. While the player is
 * `bumped` (a crate's side stopped it), the box is 2 pixels wider on each
 * side. The crate itself isn't used. The wide box's x is `x += xOffset;
 * x -= 2;`, two statements, as the ROM adds and subtracts. */
u8 Crate::PlayerHitboxOverlapsAt(struct hitbox_quad *quad, struct aabb *box, s32 xOffset,
                                 s32 yOffset)
{
    struct aabb b;

    if (gPlayer->bumped) {
        s32 x = quad->offX;
        s32 y = quad->offY;
        u8 h = quad->h;
        s32 w;

        x += xOffset;
        x -= 2;
        y += yOffset;
        w = quad->w + 4;
        SetAabbPos(&b, x, y);
        SetAabbSize(&b, w, h);
    } else {
        s32 x = quad->offX;
        s32 y = quad->offY;
        u8 w = quad->w;
        u8 h = quad->h;

        SetAabbPos(&b, x + xOffset, y + yOffset);
        SetAabbSize(&b, w, h);
    }
    if (gPlayer->mirrorBits.flipX < 0)
        b.x = xOffset * 2 - (b.x + b.w);
    if (gPlayer->mirrorBits.flipY < 0)
        b.y = yOffset * 2 - (b.y + b.h);
    if (AabbOverlaps(box, &b))
        return 1;
    return 0;
}

/* Which crate of the stack the player's `box` hits: the crate, or the
 * one it stands on (`below`) when their boxes overlap. `*foundFlag` is
 * set when the crate is in a stack at all. The crate below is skipped
 * when it is committed (state 1); its box is mirrored by this crate's
 * mirror bits. */
Crate *Crate::ResolveStackHit(struct aabb *box, u8 *foundFlag)
{
    Crate *hit = this;
    Crate *next = hit->GetAbove();
    Crate *prev = hit->GetBelow();

    if (next == 0 && prev == 0)
        return hit;
    *foundFlag = 1;
    if (prev == 0 || (prev->state & CRATE_STATE_MASK) == 1)
        return hit;
    {
        const struct hitbox_quad *q = &prev->bank->anims[prev->tag].box[0];
        struct aabb b;
        s32 px = Q8_TO_INT(prev->x);
        s32 py = Q8_TO_INT(prev->y);
        s32 offX = q->offX;
        s32 offY = q->offY;
        u8 w = q->w;
        u8 h = q->h;

        SetAabbPos(&b, offX + px, offY + py);
        SetAabbSize(&b, w, h);
        if (hit->mirrorBits.flipX < 0)
            b.x = px * 2 - (b.x + b.w);
        if (hit->mirrorBits.flipY < 0)
            b.y = py * 2 - (b.y + b.h);
        if (AabbOverlaps(&b, box))
            hit = prev;
    }
    return hit;
}

/* A crate the player's box overlaps (both from their animations' first
 * boxes, mirrored around their positions) explodes, if it is an
 * explosive kind, or breaks in its stack; a committed crate (state 1)
 * is skipped. The two boxes are one frame struct, and `px`/`py` are
 * shared by both. */
void Crate::BreakIfTouchedByPlayer()
{
    struct {
        struct aabb a;
        struct aabb b;
    } f;
    s32 px;
    s32 py;

    if ((state & CRATE_STATE_MASK) == 1)
        return;
    {
        const struct hitbox_quad *q = &bank->anims[tag].box[0];
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        px = Q8_TO_INT(x);
        py = Q8_TO_INT(y);
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        SetAabbPos(&f.a, offX + px, offY + py);
        SetAabbSize(&f.a, w, h);
        if (mirrorBits.flipX < 0)
            f.a.x = px * 2 - (f.a.x + f.a.w);
        if (mirrorBits.flipY < 0)
            f.a.y = py * 2 - (f.a.y + f.a.h);
    }
    {
        Player *p = gPlayer;
        const struct hitbox_quad *q;
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        px = Q8_TO_INT(p->x);
        py = Q8_TO_INT(p->y);
        q = &p->bank->anims[p->tag].box[0];
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        {
            s32 bx = offX + px;
            s32 by = offY + py;

            SetAabbPos(BOX_ADDR(&f.b), bx, by);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (gPlayer->mirrorBits.flipX < 0)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayer->mirrorBits.flipY < 0)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    if (AabbOverlaps(&f.a, BOX_ADDR(&f.b))) {
        if (gCrateKindExplosive[kind] == 1)
            Explode(1);
        else
            BreakInStack(0, 0, 0);
    }
}
