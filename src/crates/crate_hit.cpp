#include "crate.hpp"
#include "player.hpp"

extern "C" {
#include "util.h"
#include "globals.h"
#include "player.h"
}

/* Crate::PlayerAnimWouldTouch (#664, include/crate.hpp). */

/* Whether the player's box for animation `action` would touch the crate
 * when its box for the current animation doesn't (PlayerHasRoomForAnim
 * asks it for each crate). Outline and kind 0xA crates never do. The
 * boxes are the animations' first boxes, mirrored around their
 * positions; the touching test counts touching edges
 * (AabbOverlapsInclusiveX).
 *
 * The boxes are filled and mirrored through util.h's SetAabb and
 * aabb.h's FlipAabbX/FlipAabbY. As inline arguments, `&a` and `&b`
 * reach the inlined bodies as the constants `sp` and `sp + 16`
 * (integrate.c's const_equiv_map for a parameter whose argument is a
 * frame address), so each builder call computes its own
 * `add r0, sp, #16` and the mirror reads are sp-relative, as in the ROM;
 * only the overlap tests' `&b` is a pseudo, which takes r6. Called
 * directly with `&f.b` (a member of a frame struct, as the C had it),
 * each argument is a pseudo that cse1 ties to the first one, held in r6
 * across the calls; the C kept each use its own value with an empty
 * `"+r"` asm (BOX_ADDR) until #662 round 4. */
u8 Crate::PlayerAnimWouldTouch(s32 action)
{
    struct aabb a;
    struct aabb b;
    s32 px, py;
    const struct sprite_anim *anim;
    u8 k = kind;

    if (k == CRATE_KIND_OUTLINE || k == 0xa)
        return 0;
    {
        const struct hitbox_quad *q;
        s32 offX, offY;
        u8 w, h;

        anim = &bank->anims[tag];
        q = &anim->box[0];
        px = Q8_TO_INT(x);
        py = Q8_TO_INT(y);
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        SetAabb(&a, offX + px, offY + py, w, h);
        if (mirrorBits.flipX < 0)
            FlipAabbX(&a, px);
        if (mirrorBits.flipY < 0)
            FlipAabbY(&a, py);
    }
    {
        Player *p = gPlayer;
        const struct hitbox_quad *q;
        s32 offX, offY;
        u8 w, h;

        px = Q8_TO_INT(p->x);
        py = Q8_TO_INT(p->y);
        anim = &p->bank->anims[p->tag];
        q = &anim->box[0];
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        SetAabb(&b, offX + px, offY + py, w, h);
        if (gPlayer->mirrorBits.flipX < 0)
            FlipAabbX(&b, px);
        if (gPlayer->mirrorBits.flipY < 0)
            FlipAabbY(&b, py);
    }
    if (AabbOverlapsInclusiveX(&a, &b))
        return 0;
    {
        const struct hitbox_quad *q = &gPlayer->bank->anims[action].box[0];
        s32 offX, offY;
        u8 w, h;

        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        SetAabb(&b, offX + px, offY + py, w, h);
        if (gPlayer->mirrorBits.flipX < 0)
            FlipAabbX(&b, px);
        if (gPlayer->mirrorBits.flipY < 0)
            FlipAabbY(&b, py);
    }
    if (AabbOverlapsInclusiveX(&a, &b) != 1)
        return 0;
    return 1;
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
 * is skipped. `px`/`py` are shared by both boxes. The boxes go through
 * SetAabb and FlipAabbX/FlipAabbY, as in PlayerAnimWouldTouch (above). */
void Crate::BreakIfTouchedByPlayer()
{
    struct aabb a;
    struct aabb b;
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
        SetAabb(&a, offX + px, offY + py, w, h);
        if (mirrorBits.flipX < 0)
            FlipAabbX(&a, px);
        if (mirrorBits.flipY < 0)
            FlipAabbY(&a, py);
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
        SetAabb(&b, offX + px, offY + py, w, h);
        if (gPlayer->mirrorBits.flipX < 0)
            FlipAabbX(&b, px);
        if (gPlayer->mirrorBits.flipY < 0)
            FlipAabbY(&b, py);
    }
    if (AabbOverlaps(&a, &b)) {
        if (gCrateKindExplosive[kind] == 1)
            Explode(1);
        else
            BreakInStack(0, 0, 0);
    }
}
