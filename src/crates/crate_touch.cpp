#include "crate.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
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
 * The two stack boxes are one frame struct. The ROM recomputes the
 * player box's address (`add r0, sp, #16`) for each of the first two
 * builder calls and only holds it in r6 from the first overlap test on;
 * BOX_ADDR keeps each of those uses its own value (match.h,
 * docs/matching/archive/sp-box-retry.md), as in crate_hit.cpp. The
 * first build's x/y are computed before its call.
 *
 * #662 round 2: the two builder sites are cse1 inside one basic block
 * (the second `&f.b` is replaced by the first's register), which no
 * cse/gcse flag changes; an inline box builder, a reference to `f.b`
 * and agbcp do worse. With them kept, `pb`'s site is gcse's: PRE
 * computes `&f.b` early in the player block's mirror tests and the last
 * overlap test rematerializes `add r1, sp, #16` instead of using r6.
 * -fno-gcse frees that one site (the object matches), as it does
 * crate_hit.cpp's, but not crate_break.cpp's; not worth a flag while
 * the builder sites stay. */
u8 Crate::PlayerAnimWouldTouch(s32 action)
{
    struct {
        struct aabb a;
        struct aabb b;
    } f;
    struct aabb *pb;
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
    pb = BOX_ADDR(&f.b);
    if (AabbOverlapsInclusiveX(&f.a, pb))
        return 0;
    {
        const struct hitbox_quad *q = &gPlayer->bank->anims[action].box[0];
        s32 offX, offY;
        u8 w, h;

        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        SetAabbPos(pb, offX + px, offY + py);
        SetAabbSize(pb, w, h);
        if (gPlayer->mirrorBits.flipX < 0)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayer->mirrorBits.flipY < 0)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    if (AabbOverlapsInclusiveX(&f.a, pb) != 1)
        return 0;
    return 1;
}
