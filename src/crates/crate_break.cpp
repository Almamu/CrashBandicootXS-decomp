#include "crate.hpp"
#include "spawners.hpp"
#include "crate_list.hpp"
#include "part_list.hpp"
#include "pickups.hpp"
#include "player.hpp"
#include "hud.hpp"

extern "C" {
#include "match.h"
#include "pickups.h"
#include "util.h"
#include "audio.h"
#include "player.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "sprite_bank.h"
}

/* The crate's hits, breaks and explosions (#664, include/crate.hpp,
 * part 7g): the player's hit on a crate (QueuePlayerCollision, resolved
 * later by ApplyCollision), what each kind of crate does when it breaks
 * or is set off, the explosions' blasts, the falls of the crates above a
 * broken one, and the crate list's update. ROM 0x0800D18C-0x0800FDC8,
 * the issue #12/#13 "physics/collision" cluster
 * (docs/matching/archive/issue-12-physics-collision.md). Built with
 * old_agbcp, as the C was with old_agbcc.
 *
 * The small inline helpers below each place a load or a computation
 * where the ROM has it (an inline's arguments are computed before its
 * body); the comment on each says which. The C idioms kept from the C,
 * each tried as plain C++ first: constants in locals (`u8 one = 1`, `u8
 * e = CRATE_KIND_TNT`, ...) and local copies (`u16 eid = id`, a `struct
 * player **` to gPlayer), which give the ROM's register choices and
 * reloads, and a few gotos for its block order. */

/* The player's collision queue as its class. */
static inline CollisionQueue *PlayerQueue()
{
    return (CollisionQueue *)&gPlayer->collisionQueue;
}

/* Returns its argument. Writing a position's y through it (instead of a
 * `pp` pointer local) lets gcse make the ROM's pointer copy: the store goes
 * through `add r0, sp, #N` and the copy (`adds r2, r0, #0`) is used after. */
static inline struct e08c_pos *PosPtr(struct e08c_pos *p)
{
    return p;
}

/* The response code for an object kind (used in `case 8`). As an inline,
 * `tgt->kind` is loaded before the table address, as in the ROM. */
static inline s32 HitResponse(s32 row, s32 k)
{
    return gCrateHitResponse[row][k];
}

/* The response code at the first lookup. There the ROM loads the table
 * address first, then `&obj->kind`, then adds `row * 28 + k * 4` to the
 * table: the table is an argument (all inline arguments are expanded
 * before the body), and the offset sum is written in that order. */
static inline s32 HitResponseAt(const s32 (*t)[7], u8 *row, s32 k)
{
    return *(const s32 *)((const u8 *)t + (*row * 28 + k * 4));
}

/* The ring lock byte as an int, for the first test in RingPush.
 * Written in place, both tests are the same expression and cse's jump
 * following sends a failed first test straight past the second one; the
 * ROM re-tests (its `bne` goes to the second load). */
static inline s32 PlayerCtrlMode(void)
{
    return gPlayer->ctrlMode;
}

/* The ring count as an int. The first `!= 0` test then loads it with the
 * same zero-extending load as the loop test, and cse reuses the value for
 * the loop's entry test. Written in place, shorten_compare narrows the test
 * to a QImode load and the loop test reloads it. */
static inline s32 PlayerRingCount(void)
{
    return gPlayer->listCount;
}

/* How far two boxes overlap on one axis: `a + b - c`. Written in place,
 * the loads come out in another order. */
static inline s32 Span(s32 a, s32 b, s32 c)
{
    return a + b - c;
}

/* Adds `bit` to the player's hit mask. As an inline, a constant bit is
 * loaded before the player. */
static inline void AddPlayerHit(Player *p, u32 bit)
{
    p->hitMask |= bit;
}

/* Whether the player is still invulnerable (its `deadline` is ahead).
 * Written in place, the tests around it are laid out differently. */
static inline s32 PlayerInvulnerable(void)
{
    return gPlayer->deadline > gRoomFrameCount;
}

/* Pushes `obj` onto the player's 5-slot ring of touched crates. */
static inline void RingPush(Crate *obj)
{
    if (PlayerCtrlMode() == 0 && gPlayer->listCount <= 4)
        gPlayer->list[gPlayer->listCount] = (Crate *)obj;
    if (gPlayer->ctrlMode == 0)
        gPlayer->listCount++;
}

/* GetSpriteFrameThirdBox inlined: the player's current hitbox quad. */
static inline const struct hitbox_quad *PlayerThirdBox(void)
{
    const struct sprite_frame_3box *info = (const struct sprite_frame_3box *)gPlayer->GetFrame();

    switch (info->frame.pieces[0] >> 4) {
    case 0:
        return &info->box[2];
    case 1:
    case 2:
    case 3:
        return &gEmptySpriteBox;
    case 4:
        return &info->box[2];
    case 5:
    case 6:
        return &gEmptySpriteBox;
    default:
        return &gEmptySpriteBox;
    }
}

/* The player's `busy` latch set: the player is loaded before the 1, as
 * in SetCrateBusy. */
static inline void SetPlayerBusy(void)
{
    Player *p = gPlayer;
    u8 one = 1;

    p->busy = one;
}

/* The player (in action state `idx`) against this crate. Its attack kind
 * (gActionCtrlStateAttackKinds; invincible with the top mask level) and
 * the crate's kind give the response (gCrateHitResponse).
 *
 * First, if the player's attack box (its frame's third box) overlaps the
 * crate, the hit is resolved at once: a switch is activated, a crate is
 * marked busy, broken (with the one above it, if the player's box still
 * reaches it) or set off, a checkpoint opened. A crate whose stack the
 * player already touched (the player's ring of five) gives no response,
 * and a crate broken in a stack bumps the player back and goes into the
 * ring.
 *
 * Otherwise the player's body box is tested against the crate (grown by
 * the crates of its stack): a falling crate crushes or pushes the player;
 * else the side the player came from (`edge`: 1/2 the sides, 4 below, 8
 * on top, from the player's previous position, with FindLineCrossing for
 * a diagonal approach) and how deep it is in decide where the player is
 * put back and which crate of the stack the hit goes to. The result is
 * queued on the player's collision queue, for ApplyCollision. In control
 * mode 1, a TNT crate the player touches is lit.
 *
 * The shapes the ROM needs are in the comments below; the C was matched
 * over three passes (docs/matching/archive/huge-naked-retry-3.md). */
void Crate::QueuePlayerCollision(s32 idx)
{
    struct {
        struct aabb a;
        struct aabb c;
        struct aabb b;
        u8 found;
        struct e08c_pos p1;
        struct e08c_pos p2;
        struct e08c_pos p3;
        struct e08c_pos pos;
    } f;
    s32 px;
    s32 py;
    s32 side;
    s32 attack;
    s32 edge;
    s32 dirX;
    s32 dirY;
    s32 f21;
    s32 hit;
    s32 f20;
    Crate *obj;
    Crate *tgt;
    s32 code;
    s32 dx;
    s32 dy;
    s32 n;
    s32 r; /* shared by both slope checks, so both get r2 as in the ROM */
    const struct hitbox_quad *q;
    struct e08c_pos *pp;
    const struct hitbox_quad *hb;
    struct aabb *bb;
    /* &state, kept for the `case 1`/`case 2` test. Declared last, it
     * is the last user variable on the stack, so it takes the slot right
     * after them (0x94) and the `attack * 4` gcse temp gets 0x98, as in the
     * ROM. Left to gcse, both are temps, numbered in expression-hash order
     * (`attack * 4` first), and the slots come out swapped. */
    u8 *st;

    {
        const struct hitbox_quad *pb = &bank->anims[tag].box[0];
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        px = Q8_TO_INT(x);
        py = Q8_TO_INT(y);
        offX = pb->offX;
        offY = pb->offY;
        w = pb->w;
        h = pb->h;
        SetAabbPos(&f.a, offX + px, offY + py);
        SetAabbSize(&f.a, w, h);
        if (mirrorBits.flipX < 0)
            f.a.x = px * 2 - (f.a.x + f.a.w);
        if (mirrorBits.flipY < 0)
            f.a.y = py * 2 - (f.a.y + f.a.h);
    }
    px = Q8_TO_INT(gPlayer->x);
    py = Q8_TO_INT(gPlayer->y);
    if (gLevelState->maskLevel == MASK_LEVEL_INVINCIBLE)
        attack = ATTACK_KIND_INVINCIBLE;
    else {
        attack = gActionCtrlStateAttackKinds[idx];
        if (kind == CRATE_KIND_REINFORCED && attack == ATTACK_KIND_BODY_SLAM && gPlayer->dir == 4)
            attack = ATTACK_KIND_JUMP;
    }
    {
        /* The state is loaded and masked before `st` is set, so `st` is a
         * copy of the load's address register, stored before the compare
         * (`ldrb; ands; str r1, [sp, #0x94]; cmp`). */
        s32 s = state & CRATE_STATE_MASK;

        st = &state;
        if (s == 1)
            goto tail;
    }
    if (fallDistance != 0)
        goto tail;
    {
        s32 empty;

        hb = PlayerThirdBox();
        empty = 0;
        if (hb->w == 0)
            if (hb->h == 0)
                empty = 1;
        if (empty)
            goto tail;
    }
    {
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        offX = hb->offX;
        offY = hb->offY;
        w = hb->w;
        h = hb->h;
        {
            s32 bx = offX + px, by = offY + py;

            SetAabbPos(BOX_ADDR(&f.b), bx, by);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (gPlayer->mirrorBits.flipX < 0)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayer->mirrorBits.flipY < 0)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    bb = BOX_ADDR(&f.b);
    if (!AabbOverlaps(&f.a, bb))
        goto tail;
    f.found = 0;
    if (attack <= ATTACK_KIND_SPIN)
        obj = ResolveStackHit(bb, &f.found);
    else
        obj = this;
    code = HitResponseAt(gCrateHitResponse, &obj->kind, attack);
    {
        s32 bnc = gPlayer->bounce;

        if (bnc > 4)
            code = 0;
    }
    if (f.found != 0 && PlayerRingCount() != 0) {
        s32 i;

        for (i = 0; i < gPlayer->listCount; i++) {
            Crate *e;

            if (gPlayer->ctrlMode == 0 && (i <= 4 || i < gPlayer->listCount))
                e = gPlayer->list[i];
            else
                e = 0;
            if (e != 0) {
                Crate *h = e->GetBelow();

                if (h != 0) {
                    while (h->GetBelow() != 0)
                        h = h->GetBelow();
                } else
                    h = e;
                for (; h != 0; h = h->GetAbove()) {
                    if (this == h) {
                        code = 0;
                        break;
                    }
                }
            }
        }
    }
    n = 0;
    switch (code) {
    case 0:
    case 1:
        if (obj->kind == CRATE_KIND_NITRO_SWITCH)
            obj->ActivateNitroSwitch();
        else if (obj->kind == CRATE_KIND_IRON_SWITCH)
            obj->ActivateIronSwitch();
        break;
    case 2:
        obj->state |= CRATE_STATE_BUSY;
        SetPlayerBusy();
        break;
    case 3:
        obj->BreakInStack(0, 0, 0);
        if (gPlayer->listCount == 0) {
            const struct sprite_anim *a = &gPlayer->bank->anims[gPlayer->tag];

            if (attack != ATTACK_KIND_SLIDE &&
                PlayerHitboxOverlapsAt((struct hitbox_quad *)&a->box[0], &f.a, px, py)) {
                Crate *e = obj->GetAbove();

                if (e != 0 && (e->state & CRATE_STATE_MASK) != 1) {
                    s32 c2 = gCrateHitResponse[e->kind][attack];

                    if (c2 == 3)
                        e->BreakInStack(0, 0, 0);
                    else if (c2 == 2) {
                        e->state |= CRATE_STATE_BUSY;
                        SetPlayerBusy();
                    } else if (c2 == 4)
                        e->Explode(1);
                }
            } else if (gPlayer->dir != 0)
                n += 2;
        }
        if (f.found == 0)
            return;
        if (Q8_TO_INT(gPlayer->x) < Q8_TO_INT(x)) {
            if (attack != ATTACK_KIND_SLIDE || obj->GetAbove() != 0) {
                gPlayer->HandleEvent(0, EVENT_BUMP, 1);
                AddPlayerHit(gPlayer, 1);
            }
        } else if (attack != ATTACK_KIND_SLIDE || obj->GetAbove() != 0) {
            gPlayer->HandleEvent(0, EVENT_BUMP, 2);
            AddPlayerHit(gPlayer, 2);
        }
        RingPush(obj);
        if (gPlayer->bounce == 0)
            gPlayer->bounce = 1;
        for (; n != 0; n--)
            RingPush(0);
        return;
    case 4:
        if ((obj->state & CRATE_STATE_MASK) == 0)
            obj->Explode(1);
        return;
    case 5:
        obj->OpenCheckpoint();
        return;
    }
tail:
    if (attack == ATTACK_KIND_BODY_SLAM) {
        if (gPlayer->dir == 4)
            attack = ATTACK_KIND_JUMP;
    } else if (attack == ATTACK_KIND_SLIDE)
        attack = ATTACK_KIND_TOUCH;
    if (touched != 0) {
        touched = 0;
        if (fallDistance == 0)
            return;
    }
    {
        /* Through a pointer local: the block copy then takes a copy of it
         * (`add r2, sp, #0x2c; adds r1, r2, #0`), as in the ROM. */
        struct aabb *pc = &f.c;

        *pc = f.a;
    }
    if (attack != ATTACK_KIND_INVINCIBLE)
        MarkStackTouched(&f.a);
    {
        s32 offX;
        s32 offY;
        u8 w;
        u8 h;

        q = &gPlayer->bank->anims[gPlayer->tag].box[0];
        offX = q->offX;
        offY = q->offY;
        w = q->w;
        h = q->h;
        {
            s32 bx = offX + px, by = offY + py;

            SetAabbPos(BOX_ADDR(&f.b), bx, by);
        }
        SetAabbSize(BOX_ADDR(&f.b), w, h);
        if (gPlayer->mirrorBits.flipX < 0)
            f.b.x = px * 2 - (f.b.x + f.b.w);
        if (gPlayer->mirrorBits.flipY < 0)
            f.b.y = py * 2 - (f.b.y + f.b.h);
    }
    bb = BOX_ADDR(&f.b);
    if (!AabbOverlapsInclusiveX(&f.a, bb))
        return;
    edge = 0;
    f21 = 0;
    if (f.b.y < f.a.y)
        f21 = 1;
    if (fallDistance != 0) {
        if (!AabbOverlapsInclusiveX(&f.c, &f.b))
            return;
        if (gCrateKindUnbreakable[kind] != 0) {
            /* `side` is dead in this path, but the ROM keeps a reload of
             * px (`ldr r1, [sp, #0x70]`) right after the call: a leftover
             * of its compare, deleted after reload. `side` being the
             * function's (the other path uses it) keeps the compare that
             * long; a block-local one is deleted before reload. */
            s32 prevX = gPlayer->GetPrevX();

            side = 2;
            if (px > prevX)
                side = 1;
            if (Q8_TO_INT(gPlayer->x) < Q8_TO_INT(x)) {
                dirX = 1;
                dx = Span(f.b.x, f.b.w, f.c.x) + 1;
            } else {
                dirX = 2;
                dx = Span(f.c.x, f.c.w, f.b.x) + 1;
            }
            if (Q8_TO_INT(gPlayer->y) > Q8_TO_INT(y)) {
                dirY = 4;
                dy = Span(f.c.y, f.c.h, f.b.y);
            } else {
                dirY = 8;
                dy = Span(f.b.y, f.b.h, f.c.y);
            }
            if (dx > 5 && f21 == 0) {
                if ((gLevelState->maskLevel == MASK_LEVEL_NONE && ((gPlayer->f.flags >> 6) & 1) &&
                     !PlayerInvulnerable()) ||
                    kind != CRATE_KIND_REINFORCED) {
                    gPlayer->f.flags |= 0x40;
                    SetMaskLevel(gLevelState, MASK_LEVEL_NONE);
                    gPlayer->HandleEvent(0, EVENT_HIT_CRUSH, 0);
                } else {
                    BreakInStack(0, 0, 0);
                    gPlayer->HandleEvent(0, EVENT_HIT, 0);
                }
                return;
            } else if (dx > 6 && dy > 1 && f21 != 0) {
                s32 py2;

                f.p1.x = gPlayer->x;
                py2 = gPlayer->y;
                pp = &f.p1;
                pp->y = py2 - INT_TO_Q8(dy - 1);
                gPlayer->speedY = 0;
                SetEntityPos(gPlayer, f.p1.x, pp->y);
                PlayerQueue()->posCommitted = 1;
                gPlayer->hitMask |= dirY;
                return;
            } else if (dx <= 6 && dy > 2) {
                s32 py2;

                f.p2.x = gPlayer->x;
                py2 = gPlayer->y;
                PosPtr(&f.p2)->y = py2;
                if (dirX == 2)
                    f.p2.x = INT_TO_Q8(dx) + f.p2.x;
                else if (dirX == 1)
                    f.p2.x -= INT_TO_Q8(dx);
                SetEntityPos(gPlayer, f.p2.x, PosPtr(&f.p2)->y);
                PlayerQueue()->posCommitted = 1;
                gPlayer->HandleEvent(0, EVENT_BUMP, dirX);
                gPlayer->hitMask |= dirX;
                return;
            } else {
                s32 py2;

                if (gPlayer->ctrlMode != 1)
                    return;
                f.p3.x = gPlayer->x;
                py2 = gPlayer->y;
                PosPtr(&f.p3)->y = py2;
                pp = &f.p3; /* shared with the p1 arm, where it gets r2 */
                if (dirY == 4)
                    pp->y = INT_TO_Q8(dy) + pp->y;
                else if (dirX == 8)
                    pp->y -= INT_TO_Q8(dy);
                SetEntityPos(gPlayer, f.p3.x, pp->y);
                PlayerQueue()->posCommitted = 1;
                gPlayer->HandleEvent(0, EVENT_BUMP, dirY);
                gPlayer->hitMask |= dirY;
                return;
            }
        } else {
            Crate *e;

            /* A `for` with the first call on `this`: its copy is
             * cross-jumped into the loop's call, so the ROM enters with
             * `mov r0, sl`. */
            for (e = GetBelow(); e != 0; e = e->GetBelow()) {
                if (gCrateKindUnbreakable[e->kind] != 0 && (e->state & CRATE_STATE_MASK) == 0)
                    return;
            }
            code = gCrateHitResponse[kind][5];
            dx = 0;
            dy = 0;
            dirX = 0;
            dirY = 0;
        }
    } else {
        s32 ax;
        s32 ay;

        ax = gPlayer->GetPrevX();
        ay = gPlayer->GetPrevY();
        side = 2;
        if (px > ax)
            side = 1;
        if (Q8_TO_INT(gPlayer->x) < Q8_TO_INT(x)) {
            dirX = 1;
            dx = Span(f.b.x, f.b.w, f.a.x) + 1;
        } else {
            dirX = 2;
            dx = Span(f.a.x, f.a.w, f.b.x) + 1;
        }
        if (Q8_TO_INT(gPlayer->y) > Q8_TO_INT(y)) {
            dirY = 4;
            dy = Span(f.a.y, f.a.h, f.b.y);
        } else {
            dirY = 8;
            dy = Span(f.b.y, f.b.h, f.a.y);
        }
        if (ay == py) {
            if (ax == px) {
                /* `else edge = dirX` after the other arm (not a `goto
                 * edge_x`): cross-jumping later merges it into edge_x, but
                 * its reload of dirX (r0) still advances reload's
                 * round-robin, so the next arm reloads dy into r1 as the
                 * ROM does. */
                if (dy <= 2) {
                    edge = 4;
                    if (f21 != 0)
                        edge = 8;
                } else
                    edge = dirX;
            }
            /* `else edge = dirX` (not a `goto edge_x`): its reload of dirX
             * gets r6, so cross-jumping sends this arm to the r6 copy of
             * `edge = dirX` (the one the first slope check ends in), as the
             * ROM's branch does. */
            else if (dy <= 2 && (dx > 3 || !gPlayer->HasRampYTarget())) {
                edge = 4;
                if (f21 != 0)
                    edge = 8;
            } else
                edge = dirX;
        } else if (ax == px) {
            edge = dirY;
            if (dy > 2) {
                edge = dirX;
                if (dy <= 7 && dx > 3)
                    edge = dirY;
            }
        } else if (ay <= py && f21 != 0) {
            ay += q->offY + q->h;
            /* An `if`/`else edge = dirX` (not a `goto edge_x`): the `else`
             * is the last code of this arm in the RTL. Its reload of dirX
             * takes r0 and moves reload's round-robin on, so the second
             * slope check's `ay += q->offY` loads into r1 with r0 as the
             * scratch, as in the ROM. Cross-jumping then merges the `else`
             * into edge_x, which also reloads dirX into r0. */
            if (ay <= f.a.y + f.a.h) {
                edge = 0;
                if (side == 1) {
                    if (f.b.x + f.b.w >= f.a.x && dx > 4)
                        edge = 8;
                } else if (f.b.x <= f.a.x + f.a.w && dx > 4)
                    edge = 8;
                if (edge == 0) {
                    py += q->offY + q->h;
                    /* The call is in each arm (cross-jumping merges the
                     * tails): the stack argument is stored before the join
                     * and px is passed from the register it was just
                     * computed in, as in the ROM. */
                    if (dirX == 1) {
                        ax += q->offX + q->w;
                        px = f.b.x + f.b.w;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x);
                    } else {
                        ax += q->offX;
                        px = f.b.x;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x + f.a.w);
                    }
                    if ((r < 0 && f21 != 0 && dx > 4) || (r > 0 && r <= f.a.y))
                        edge = 8;
                    else
                        edge = dirX;
                }
            } else
                edge = dirX;
        } else {
            ay += q->offY;
            if (ay < f.a.y)
                goto edge_x;
            {
                edge = 0;
                if (side == 1) {
                    if (f.b.x + f.b.w >= f.a.x && dx > 5)
                        edge = 4;
                } else if (f.b.x <= f.a.x + f.a.w && dx > 5)
                    edge = 4;
                if (edge == 0) {
                    py += q->offY;
                    if (dirX == 1) {
                        ax += q->offX + q->w;
                        px = f.b.x + f.b.w;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x);
                    } else {
                        ax += q->offX;
                        px = f.b.x;
                        r = FindLineCrossing(ax, ay, px, py, f.a.x + f.a.w);
                    }
                    if (gPlayer->ctrlMode == 1)
                        r += 2;
                    if ((r < 0 && f21 == 0 && dx > 5) || (r > 0 && r >= f.a.y + f.a.h))
                        edge = 4;
                    else
                        edge = dirX;
                }
            }
        }
        goto edge_done;
    edge_x:
        edge = dirX;
    edge_done:
        code = 0;
    }
    {
        s32 py2;

        f.pos.x = gPlayer->x;
        py2 = gPlayer->y;
        PosPtr(&f.pos)->y = py2;
    }
    LIMIT_MIN(dx, 0);
    LIMIT_MIN(dy, 0);
    hit = 0;
    f20 = gAttackKindBreakLimited[attack];
    tgt = this;
    switch (edge) {
    case 0:
    case 3:
    case 5:
    case 6:
    case 7:
        break;
    case 4:
        tgt = GetBottom();
        code = gCrateHitResponse[tgt->kind][attack];
        if (tgt->kind == CRATE_KIND_ARROW && attack == ATTACK_KIND_JUMP)
            code = 3;
        if (attack <= ATTACK_KIND_SLIDE || attack == ATTACK_KIND_INVINCIBLE ||
            (attack == ATTACK_KIND_SPIN && code <= 2)) {
            gPlayer->HandleEvent(0, EVENT_BUMP, 4);
            AddPlayerHit(gPlayer, 4);
            if (gPlayer->hitAxes != 8)
                PosPtr(&f.pos)->y = INT_TO_Q8(dy) + PosPtr(&f.pos)->y;
        }
        break;
    case 8:
        tgt = GetTop();
        code = HitResponse(tgt->kind, attack);
        if (attack == ATTACK_KIND_SPIN && tgt->kind != CRATE_KIND_NITRO &&
            gPlayer->listCount != 0) {
            gPlayer->speedY = 0;
            gPlayer->rampY.start = 0;
            gPlayer->rampY.step = 0;
            gPlayer->rampY.target = 0;
            code = 1;
        }
        if (code == 1 || code == 2) {
            PosPtr(&f.pos)->y -= INT_TO_Q8(dy - 1);
            PosPtr(&f.pos)->y &= ~0xff;
            SetEntityPos(gPlayer, f.pos.x, PosPtr(&f.pos)->y);
            PlayerQueue()->posCommitted = 1;
        } else if (code == 0 || code == 2)
            PosPtr(&f.pos)->y -= INT_TO_Q8(dy);
        PosPtr(&f.pos)->y &= ~0xff;
        break;
    case 1:
    case 2:
        hit = dirX;
        if (!AabbOverlapsInclusiveX(&f.c, &f.b)) {
            code = 0;
            ClearStackTouched();
            hit = 0;
        } else {
            if ((*st & CRATE_STATE_MASK) == 0) {
                if (hit == 2)
                    f.pos.x += INT_TO_Q8(dx);
                else if (hit == 1)
                    f.pos.x -= INT_TO_Q8(dx);
            }
            if (attack > ATTACK_KIND_JUMP) {
                code = gCrateHitResponse[kind][attack];
                if (attack == ATTACK_KIND_SPIN && code == 2)
                    code = 0;
                if (attack == ATTACK_KIND_BODY_SLAM && code == 3)
                    f.pos.x = gPlayer->x;
            } else if (dy <= 4 && dx > 3 && f21 != 0) {
                code = gCrateHitResponse[kind][attack];
                if (code > 1)
                    code = 0;
            } else if (gCrateHitResponse[kind][attack] == 4) {
                f.pos.x = gPlayer->x;
                code = gCrateHitResponse[kind][attack];
            }
        }
        if (code != 1 && hit != 0) {
            s32 ok = 1;
            Crate *next = GetAbove();
            Crate *prev = GetBelow();
            s32 vy = Q8_TO_INT(gPlayer->speedY);

            if (dirY == 8 && next == 0 && (vy >= dy - 1 || dy <= 2))
                ok = 0;
            else if (dirY == 4 && prev == 0 && (vy >= dy - 1 || dy <= 2))
                ok = 0;
            if (ok) {
                SetEntityPos(gPlayer, f.pos.x, PosPtr(&f.pos)->y);
                PlayerQueue()->posCommitted = 1;
            }
        }
        break;
    }
    if (gPlayer->ctrlMode == 1 && kind == CRATE_KIND_TNT && code <= 1 &&
        AabbOverlapsInclusiveX(&f.c, &f.b) == 1)
        tgt->LightTnt();
    PlayerQueue()->Add(tgt, attack, code, edge, dy, f.pos, hit, (struct byte_arg){ f20 },
                       (struct byte_arg){ f21 });
}

/* A queued hit resolved (ResolveCollisionCandidates, collision_queue.cpp):
 * `attack` and `code` as QueuePlayerCollision found them, the side `edge`,
 * its `depth`, the player's corrected position `pos` and the bump `hit`.
 * `limited` is gAttackKindBreakLimited's flag for the attack, `above` that
 * the player was above the crate, `forced` that another hit was resolved
 * before this one. A jump on a crate bounces the player (higher on an
 * arrow crate), a reinforced crate only turns busy; then by `code`: a
 * landing on top carries the player; a switch is activated; a TNT crate
 * lit, a bouncy wumpa crate bounced, or the crate marked busy; the crate
 * broken in its stack (a slot crate on its TNT face turns into TNT
 * first); set off; or a checkpoint opened. Unless the hit returned early,
 * the player is moved to `pos` (if nothing else committed its position
 * this frame) and bumped. */
void Crate::ApplyCollision(s32 attack, s32 code, s32 edge, s32 depth, struct e08c_pos pos, s32 hit,
                           bool limited, bool above, bool forcedIn)
{
    bool forced;

    if ((state & CRATE_STATE_MASK) != 0)
        goto commit;
    if (gPlayer->ctrlMode == 1 && code > 2) {
        pos.x = gPlayer->x;
        pos.y = gPlayer->y;
    }
    if ((u32)(code - 2) <= 1 || code == 5) {
        u8 k = kind;
        s32 d = gPlayer->dir;
        s32 d4 = d & 4;

        if (d4 == 0) {
            if (k != CRATE_KIND_REINFORCED) {
                if (attack == ATTACK_KIND_JUMP) {
                    if (k == CRATE_KIND_ARROW || k == CRATE_KIND_IRON_ARROW) {
                        PlaySfx(gAudioContext, SFX_ARROW_CRATE_BOUNCE, 0x100);
                        gPlayer->HandleEvent(0, EVENT_BOUNCE_HIGH, 8);
                    } else
                        gPlayer->HandleEvent(0, EVENT_BOUNCE, 8);
                    gPlayer->speedY = 0;
                    gPlayer->rampY.start = 0;
                    gPlayer->rampY.step = 0;
                    gPlayer->rampY.target = 0;
                } else if ((u32)(attack - ATTACK_KIND_BODY_SLAM) <= 1 &&
                           k == CRATE_KIND_IRON_ARROW) {
                    PlaySfx(gAudioContext, SFX_ARROW_CRATE_BOUNCE, 0x100);
                    gPlayer->HandleEvent(0, EVENT_BOUNCE_HIGH, 8);
                    gPlayer->speedY = 0;
                    gPlayer->rampY.start = 0;
                    gPlayer->rampY.step = 0;
                    gPlayer->rampY.target = 0;
                }
            } else if (code == 2) {
                state |= CRATE_STATE_BUSY;
                SetPlayerBusy();
                code = 1;
            }
        }
    }
    if (code == 3 && kind == CRATE_KIND_SLOT && (slotState & CRATE_SLOT_PHASE_MASK) == 3) {
        {
            u8 e = CRATE_KIND_TNT;

            kind = e;
            blastState = 0;
        }
        code = gCrateHitResponse[kind][attack];
    }
    forced = forcedIn;
    if (code == 1 && attack == ATTACK_KIND_SPIN && gPlayer->bounce == 1 &&
        !(gPlayer->dir & PART_DIR_Y_MASK)) {
        code = gCrateHitResponse[kind][attack];
        gPlayer->bounce = 2;
        gPlayer->bounce++;
        gPlayer->bounce++;
        gPlayer->bounce++;
        forced = true;
    }
    switch (code) {
    case 0:
    case 1:
        if (!(gPlayer->dir & 4) && above) {
            if ((edge == 8 && depth <= 1) || (depth <= 1 && code == 1) ||
                (depth <= 7 && code == 1 && edge == 8)) {
                gPlayer->carried = this;
                {
                    u8 m = 8;

                    gPlayer->hitAxes = m;
                }
                {
                    struct e08c_pos *pp = &pos;
                    s32 py = gPlayer->y;

                    pp->y = py - INT_TO_Q8(depth - 1);
                    hit = 0;
                    pp->x = gPlayer->x;
                }
            }
        }
        if (code != 1)
            goto commit;
        if ((u32)(edge - 1) <= 1 && attack <= ATTACK_KIND_TOUCH) {
            hit = 0;
            pos.x = gPlayer->x;
        }
        if (kind == CRATE_KIND_NITRO_SWITCH)
            ActivateNitroSwitch();
        else if (kind == CRATE_KIND_IRON_SWITCH)
            ActivateIronSwitch();
        goto commit;
    case 2:
        if (kind == CRATE_KIND_TNT)
            LightTnt();
        else if (kind == CRATE_KIND_BOUNCY_WUMPA)
            BounceWumpa();
        else {
            state |= CRATE_STATE_BUSY;
            SetPlayerBusy();
        }
        goto commit;
    case 3:
        if ((u32)(attack - ATTACK_KIND_BODY_SLAM) <= 1)
            BreakInStack(0, 0, 0);
        else if (fallDistance != 0)
            BreakInStack(0, 0, 4);
        else if (attack == ATTACK_KIND_JUMP)
            BreakInStack(0, limited, edge);
        else {
            Player **pp = &gPlayer;

            if ((*pp)->listCount != 0 && !forced)
                return;
            if (edge == 8 || edge == 4) {
                BreakInStack(0, 0, edge);
                if ((*pp)->ctrlMode == 0 && (*pp)->listCount <= 4)
                    (*pp)->list[(*pp)->listCount] = this;
                if (gPlayer->ctrlMode == 0)
                    gPlayer->listCount++;
            }
        }
        return;
    case 4:
        if ((state & CRATE_STATE_MASK) == 0)
            Explode(1);
        return;
    case 5:
        OpenCheckpoint();
        return;
    }
commit:
    {
        Player *p = gPlayer;
        CollisionQueue *q = &p->collisionQueue;

        if (q->posCommitted == 0)
            SetEntityPos(p, pos.x, pos.y);
    }
    if (hit != 0) {
        gPlayer->HandleEvent(0, EVENT_BUMP, hit);
        gPlayer->hitMask |= hit;
    }
}

/* Clears the touched flag of every idle crate (state 0) in the stack,
 * above and then below this one. */
void Crate::ClearStackTouched()
{
    Crate *cur;

    for (cur = GetAbove(); cur != 0; cur = cur->GetAbove())
        if ((cur->state & CRATE_STATE_MASK) == 0)
            cur->touched = 0;
    for (cur = GetBelow(); cur != 0; cur = cur->GetBelow())
        if ((cur->state & CRATE_STATE_MASK) == 0)
            cur->touched = 0;
}

/* Sets the touched flag of every idle crate (state 0) in the stack. The
 * player's box `box` grows by its original height for each one, and
 * moves up by it for each crate above. */
void Crate::MarkStackTouched(struct aabb *box)
{
    s32 step = box->h;
    Crate *cur;

    cur = GetAbove();
    if (cur != 0) {
        u8 one = 1;
        do {
            if ((cur->state & CRATE_STATE_MASK) == 0) {
                cur->touched = one;
                box->y -= step;
                box->h += step;
            }
            cur = cur->GetAbove();
        } while (cur != 0);
    }
    cur = GetBelow();
    if (cur != 0) {
        u8 one = 1;
        do {
            if ((cur->state & CRATE_STATE_MASK) == 0) {
                cur->touched = one;
                box->h += step;
            }
            cur = cur->GetBelow();
        } while (cur != 0);
    }
}

/* The bouncy wumpa crate: the first bounce arms its 360-frame timer.
 * While it runs, each bounce (until the fifth, which breaks it) plays the
 * bounce animation and drops two wumpa fruit; once it has run out, a hit
 * breaks the crate. */
void Crate::BounceWumpa()
{
    if (bounceTimer == -0x2a) {
        bounceTimer = 0x168;
        paramB = 0;
    }
    if (bounceTimer > 0) {
        if (paramA == 0) {
            if (++paramB > 4) {
                BreakInStack(0, 0, 0);
            } else {
                state |= CRATE_STATE_BUSY;
                SetPlayerBusy();
                timer = 6;
                paramA = 1;
            }
            {
                s32 px = Q8_TO_INT(x);
                s32 py = Q8_TO_INT(y) - 6;

                gEntitySpawner->DropWumpa(px, py, 0, 0xe, true);
            }
            {
                s32 px = Q8_TO_INT(x) + 3;
                s32 py = Q8_TO_INT(y);

                gEntitySpawner->DropWumpa(px, py, 0, 0, true);
            }
        }
    } else {
        BreakInStack(0, 0, 0);
    }
}

/* The TNT crate lit: its 3-second countdown starts (kind TNT_LIT_3,
 * animation 0x14), with its palette and the tick sound. */
void Crate::LightTnt()
{
    const struct sprite_anim *anims;
    const struct sprite_anim *a;
    u32 slot;

    kind = CRATE_KIND_TNT_LIT_3;
    SetTag(0x14);
    f.b.active = 1;
    Crates()->LinkActive(this);
    anims = bank->anims;
    a = &anims[tag];
    slot = gPaletteCache->GetSlot(a->paletteId);
    palette = slot;
    PlaySfx(gAudioContext, SFX_TNT_TICK, 0x100);
    timer = 0x3c;
}

/* MovingSprite::StartMotionY inlined: the Y speed `vel` and its ramp. As
 * an inline, `vel` is in a register before the stores; written in place,
 * the stores come out in another order. */
static inline void PuffSetMotion(MovingSprite *puff, s32 vel, s32 step, s32 target)
{
    puff->speedY = vel;
    puff->rampY.start = vel;
    puff->rampY.step = step;
    puff->rampY.target = target;
}

/* The checkpoint crate: a puff of smoke rises from it, it plays its open
 * animation and sound, its entity is marked activated, it counts as
 * broken, and the checkpoint moves to the player. */
void Crate::OpenCheckpoint()
{
    MovingSprite *puff;
    u8 one;

    {
        s32 px = Q8_TO_INT(x) - 10;
        s32 py = Q8_TO_INT(y);

        puff = gEntitySpawner->SpawnEffectPart(0x2a, 0, px, py, 0);
    }
    puff->f.b.visible = 0;
    puff->mirrorBits.flipX = 0;
    PuffSetMotion(puff, -0x180, 8, -0x10);
    SetTag(0x1b);
    PlaySfx(gAudioContext, SFX_CHECKPOINT, 0x100);
    {
        u16 eid = id;

        if (eid != 0xffff)
            SetEntityIdActivated(gEntityFlags, eid);
    }
    if (gCrateKindCounted[kind])
        AddBrokenCrate(gLevelState);
    SetCheckpointAtPlayer(gLevelState, paramA != 0);
    state &= CRATE_STATE_MASK;
    gPlayer->busy = 0;
    one = 1;
    state = (state & CRATE_STATE_BUSY) | one;
}

/* Breaks the crate at the end of the stack in direction `dir` (4: down,
 * 8: up) that isn't committed yet, or this one for any other `dir`. With
 * `arg2`, only the first break of the player's countdown goes ahead.
 *
 * The walk is written as the ROM's goto loops: the natural `for`
 * loops get rotated and their exit blocks laid out differently. */
void Crate::BreakInStack(u8 flag, bool once, u32 dir)
{
    Crate *p;
    Crate *q;

    if (once) {
        if (gPlayer->countdown != 0)
            return;
        gPlayer->countdown = 1;
        gPlayer->countdown++;
    }
    if (dir == 4) {
        p = GetBelow();
        if (p == 0 || (p->state & CRATE_STATE_MASK) == 1)
            goto none;
    prev:
        q = p->GetBelow();
        if (q == 0 || (q->state & CRATE_STATE_MASK) == 1)
            goto last;
        p = q;
        goto prev;
    }
    if (dir != 8)
        goto other;
    p = GetAbove();
    if (p != 0 && (p->state & CRATE_STATE_MASK) != 1)
        goto next;
none:
    q = this;
    goto found;
last:
    q = p;
    goto found;
next:
    q = p->GetAbove();
    if (q == 0 || (q->state & CRATE_STATE_MASK) == 1)
        goto last;
    p = q;
    goto next;
found:
    if (gCrateKindUnbreakable[q->kind] == 0)
        q->Break(flag);
    return;
other:
    Break(flag);
}

/* Breaks the crate (unless it is committed already): its break animation
 * and palette, the broken crate count, its entity's gone bit, the crates
 * above it falling, and then what its kind does. `arg1` set: broken by a
 * blast, so a crate's contents aren't given out. `chained`: another crate
 * stands on it (the drop's flag).
 *
 * The constant 1 of the state store is a local `one` that the bitmap
 * shift reuses (the ROM's r8), and the switch cases are in the ROM's
 * block order with an explicit empty case 22. */
void Crate::Break(u32 arg1)
{
    u8 flag = arg1;
    bool chained;
    u8 one;

    if ((state & CRATE_STATE_MASK) == 1)
        return;
    chained = 0;
    if (GetAbove() != 0 && flag == 0)
        chained = 1;
    f.b.active = 1;
    Crates()->LinkActive(this);
    state &= CRATE_STATE_MASK;
    gPlayer->busy = 0;
    one = 1;
    state = (state & CRATE_STATE_BUSY) | one;
    SetTag(0x1d);
    {
        const struct sprite_anim *anims = bank->anims;
        const struct sprite_anim *a = &anims[tag];
        u32 slot = gPaletteCache->GetSlot(a->paletteId);

        palette = slot;
    }
    ClampFrame(3);
    if (gCrateKindCounted[kind])
        AddBrokenCrate(gLevelState);
    /* ENTITY_SET_GONE_BIT_OF(id, one) (entity_bits.h) in a plain
     * block: the macro's do/while(0) changes this object. */
    {
        s32 eid = id;
        struct entity_flags *base = gEntityFlags;
        s32 word = eid / 32;
        s32 off = word * 4;
        u32 *slot = base->bits0Copy;

        slot = (u32 *)((u8 *)slot + off);
        *slot |= one << (eid - word * 32);
    }
    DropAbove();
    switch (kind) {
    case CRATE_KIND_AKU_AKU:
        if (flag == 0)
            OpenAkuAku();
        break;
    case CRATE_KIND_LIFE:
        if (flag == 0)
            OpenLife(chained);
        break;
    case CRATE_KIND_IRON_SWITCH:
        ActivateIronSwitch();
        break;
    case CRATE_KIND_NITRO_SWITCH:
        ActivateNitroSwitch();
        break;
    case CRATE_KIND_MYSTERY:
        if (flag == 0)
            OpenMystery(chained);
        break;
    case CRATE_KIND_ARROW:
    case CRATE_KIND_BOUNCY_WUMPA:
    case CRATE_KIND_REINFORCED:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        break;
    case CRATE_KIND_NITRO:
    case CRATE_KIND_TNT:
    case CRATE_KIND_TNT_LIT_1:
    case CRATE_KIND_TNT_LIT_2:
    case CRATE_KIND_TNT_LIT_3:
        Explode(0);
        break;
    case CRATE_KIND_SLOT:
        if (flag == 0)
            OpenSlot(chained);
        break;
    case CRATE_KIND_TIME_1:
        FreezeLevelClock(gLevelState, 1);
        break;
    case CRATE_KIND_TIME_2:
        FreezeLevelClock(gLevelState, 2);
        break;
    case CRATE_KIND_TIME_3:
        FreezeLevelClock(gLevelState, 3);
        break;
    case CRATE_KIND_BASIC:
        {
            s32 px = Q8_TO_INT(x);
            s32 py = Q8_TO_INT(y) + 3;

            gEntitySpawner->DropWumpa(px, py, 0, 3, chained);
        }
        if (flag == 0)
            PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        break;
    case 22:
        break;
    }
}

/* PlaySfx(gAudioContext, sfx, 0x100). As an inline, the sound's id is
 * loaded before the volume. */
static inline void Sfx(s32 sfx)
{
    PlaySfx(gAudioContext, sfx, 0x100);
}

/* The "?" crate: its contents (paramB; 9: picked at random) - wumpa
 * fruit (1-6, more for the higher values, each case falling into the
 * next), an Aku Aku mask (7), an extra life (8) or a single wumpa (10).
 * Cases 7 and 8 are OpenAkuAku and OpenLife inlined. */
void Crate::OpenMystery(bool flag)
{
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    if (paramB == 9) {
        u8 r = (u16)rand() >> 8;

        if (r <= 0x56)
            paramB = 1;
        else if (r <= 0xd3)
            paramB = 4;
        else if (r <= 0xec)
            paramB = 7;
        else
            paramB = 8;
    }
    switch (paramB) {
    case 10:
        {
            s32 px = Q8_TO_INT(x);
            s32 py = Q8_TO_INT(y);

            gEntitySpawner->DropWumpa(px, py, 0, 0xff, false);
        }
        break;
    case 8:
        Sfx(3);
        {
            u16 eid = id;

            if (eid != 0xffff) {
                if ((u8)IsEntityIdActivated(gEntityFlags, eid) == 0)
                    SetEntityIdActivated(gEntityFlags, id);
            }
        }
        {
            s32 px = Q8_TO_INT(x);
            s32 py = Q8_TO_INT(y) + 3;

            gEntitySpawner->DropExtraLife(px, py, 0, 3, flag);
        }
        break;
    case 7:
        {
            Player *p = gPlayer;

            if (p->f.flags >> 7) {
                p->HandleEvent(0, EVENT_MASK_GAIN, 0);
                Sfx(1);
            }
        }
        break;
    case 6:
        {
            s32 px = Q8_TO_INT(x) - 1;
            s32 py = Q8_TO_INT(y) + 3;

            gEntitySpawner->DropWumpa(px, py, 1, 3, flag);
        }
    case 5:
        {
            s32 px = Q8_TO_INT(x) + 1;
            s32 py = Q8_TO_INT(y) + 1;

            gEntitySpawner->DropWumpa(px, py, 0, 3, flag);
        }
    case 4:
        {
            s32 px = Q8_TO_INT(x) - 3;
            s32 py = Q8_TO_INT(y) + 3;

            gEntitySpawner->DropWumpa(px, py, 1, 1, flag);
        }
    case 3:
        {
            s32 px = Q8_TO_INT(x) + 3;
            s32 py = Q8_TO_INT(y) + 2;

            gEntitySpawner->DropWumpa(px, py, 0, 1, flag);
        }
    case 2:
        {
            s32 px = Q8_TO_INT(x) + 5;
            s32 py = Q8_TO_INT(y) + 2;

            gEntitySpawner->DropWumpa(px, py, 0, 2, flag);
        }
    case 1:
    default:
        {
            s32 px = Q8_TO_INT(x) - 5;
            s32 py = Q8_TO_INT(y) + 3;

            gEntitySpawner->DropWumpa(px, py, 1, 2, flag);
        }
        break;
    }
}

/* The slot crate, by the face it stopped on: nothing (0), an extra life
 * (1), a "?" crate's contents (2) or an explosion (3).
 *
 * The empty `case 0` gives the ROM's `==1`/`<=1`/`==2`/`==3` compare
 * order. */
void Crate::OpenSlot(bool flag)
{
    PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
    switch (slotState & CRATE_SLOT_PHASE_MASK) {
    case 0:
        break;
    case 1:
        PlaySfx(gAudioContext, SFX_CRATE_BREAK, 0x100);
        {
            u16 eid = id;

            if (eid != 0xffff) {
                if ((u8)IsEntityIdActivated(gEntityFlags, eid) == 0)
                    SetEntityIdActivated(gEntityFlags, id);
            }
        }
        {
            s32 px = Q8_TO_INT(x);
            s32 py = Q8_TO_INT(y) + 3;

            gEntitySpawner->DropExtraLife(px, py, 0, 3, flag);
        }
        break;
    case 2:
        OpenMystery(flag);
        break;
    case 3:
        state &= CRATE_STATE_BUSY;
        Explode(1);
        break;
    }
}

/* The crates stacked on this one start to fall: each one's target is the
 * one below's (this crate's own target, or its position), and its speed
 * steps down by 2 (4 when this crate is falling itself). An explosive
 * crate that falls more than 0x16 pixels explodes when it lands, unless
 * it is the top of the stack (a nitro crate there still does, if it is
 * idle).
 *
 * Matching notes: the gCrateKindExplosive pointer is a local set before
 * the loop (only then does the ROM's reload-register choice come out),
 * the bank is loaded into a local before the animation's offset is
 * computed, `spread` is built in two steps, the
 * step delta is widened into its own int before the add, and
 * `n->fallTargetY` is written in both arms of an if/else. */
void Crate::DropAbove()
{
    s8 delta = -2;
    const struct sprite_bank *b = bank;
    const struct sprite_anim *a = &b->anims[tag];
    s32 base = INT_TO_Q8(a->box[0].h + 1);
    Crate *n = GetAbove();
    s32 spread;
    s32 carry;
    const u8 *tbl = gCrateKindExplosive;

    if (fallDistance != 0)
        delta = -4;
    if (n == 0 || this == 0)
        return;
    if (fallDistance != 0)
        n->fallTargetY = fallTargetY;
    else
        n->fallTargetY = y;
    spread = n->fallTargetY;
    spread -= n->y;
    LIMIT_MIN(spread, 0);
    carry = 0;
    while (n != 0) {
        s32 t;

        if (n->fallDistance != 0) {
            n->fallDistance = spread + carry;
            n->fallTargetY = n->fallTargetY + carry;
        } else {
            n->fallDistance = spread;
            n->fallTargetY = n->y + base;
        }
        t = n->fallSpeed;
        LIMIT_MAX(t, 0);
        {
            s32 d = delta;

            n->fallSpeed = t + d;
        }
        n->f.flags |= 0x10;
        Crates()->LinkActive(n);
        if (tbl[n->kind] && blastState == 0 && n->fallDistance > 0x1600) {
            Crate *next = n->GetAbove();
            Crate *prev = n->GetBelow();

            if (next == 0 && prev != 0) {
                if (n->kind != CRATE_KIND_NITRO)
                    goto advance;
                if (n->state & CRATE_STATE_MASK)
                    goto advance;
            }
            n->timer = 0;
            n->blastState = 1;
        }
    advance:
        n = n->GetAbove();
        if (carry == 0)
            carry = base;
    }
}

/* An explosive crate explodes (unless it is committed already): its
 * explosion animation (a nitro crate's is tag 1), the broken crate count,
 * its entity's gone bit, the sound, and the crates above it falling. A
 * vulnerable player within 0x1D pixels on both axes, or anywhere when
 * `near` is set, is hit by it (EVENT_HIT_EXPLOSION). A TNT crate is left
 * as TNT_LIT_1. */
void Crate::Explode(u8 near)
{
    u8 one;

    if ((state & CRATE_STATE_MASK) == 1)
        return;

    timer = 0;
    state &= CRATE_STATE_MASK;
    gPlayer->busy = 0;
    f.flags |= 0x10;
    Crates()->LinkActive(this);
    one = 1;
    state = (state & CRATE_STATE_BUSY) | one;
    if (kind == CRATE_KIND_NITRO) {
        tag = one;
        ResetFrameTimer();
        ResetFrameIndex();
        SetAnimDone(0);
    } else
        SetTag(0x21);
    if (gCrateKindCounted[kind])
        AddBrokenCrate(gLevelState);
    ENTITY_SET_GONE_BIT(id);
    PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
    DropAbove();

    if ((gPlayer->f.flags >> 6) & 1 && !PlayerInvulnerable()) {
        GroundSprite *p;
        s32 t1 = Q8_TO_INT(gPlayer->x) - Q8_TO_INT(x);
        s32 dx = ABS_BRANCHLESS(t1);

        if (dx <= 0x1d) {
            s32 t2 = Q8_TO_INT(gPlayer->y) - Q8_TO_INT(y);
            s32 dy = ABS_BRANCHLESS(t2);

            if (dy <= 0x1d)
                goto call;
        }
        if (near) {
        call:
            p = gPlayer;
            p->HandleEvent(0, EVENT_HIT_EXPLOSION, 0);
        }
    }
    if (kind != CRATE_KIND_NITRO)
        kind = CRATE_KIND_TNT_LIT_1;
}

/* The blast of an exploding crate (FinishBrokenCrate, at steps 3 and 6
 * of the explosion): every idle crate within `dist` pixels (|dx| + |dy|)
 * explodes, if it is explosive, breaks, if it is breakable, or is
 * activated, if it is a switch; every wumpa fruit within `dist` is picked
 * up. The crate is then marked as having blasted.
 *
 * The crate list is read through gCrateList's C view here, in
 * UpdateCrates and in BreakCratesInArea: the loop tests through
 * Crates() (an inline call) aren't copied in front of the loops, which
 * changes their layout. */
void Crate::BlastNearby(s32 dist)
{
    s32 i = 0;

    if (i < gCrateList->count) {
        /* gCrateKindExplosive as an integer: `kind + table` is the ROM's
         * operand order; indexing it (or `kind + pointer`) adds the other
         * way round. */
        u32 commit = (u32)gCrateKindExplosive;

        do {
            Crate *o = gCrateList->slots[i];

            if (o->GetClassId() == 3) {
                s32 t1 = Q8_TO_INT(o->x) - Q8_TO_INT(x);
                s32 dx = ABS_BRANCHLESS(t1);
                s32 t2 = Q8_TO_INT(o->y) - Q8_TO_INT(y);
                s32 dy = ABS_BRANCHLESS(t2);

                if (dx + dy <= dist && (o->state & CRATE_STATE_MASK) == 0) {
                    u32 k = o->kind;

                    if (*(u8 *)(k + commit))
                        o->Explode(0);
                    else if (gCrateKindBreakable[k])
                        o->BreakInStack(1, 0, 0);
                    else if (k == CRATE_KIND_IRON_SWITCH)
                        o->ActivateIronSwitch();
                    else if (k == CRATE_KIND_NITRO_SWITCH)
                        o->ActivateNitroSwitch();
                }
            }
            i++;
        } while (i < gCrateList->count);
    }

    i = 0;
    if (i < gTouchableList->count) {
        do {
            Wumpa *o = (Wumpa *)gTouchableList->items[i];

            if (o->GetClassId() == 2) {
                s32 t1 = Q8_TO_INT(o->x) - Q8_TO_INT(x);
                s32 dx = ABS_BRANCHLESS(t1);
                s32 t2 = Q8_TO_INT(o->y) - Q8_TO_INT(y);
                s32 dy = ABS_BRANCHLESS(t2);

                if (dx + dy <= dist) {
                    o->PickUp(1);
                    o->f.flags |= 0x10;
                }
            }
            i++;
        } while (i < gTouchableList->count);
    }
    blastState = 0xff;
}

/* The crate list's update pass (run_room.cpp): DetonateNitroCrates, then
 * each crate of the list is updated, and a crate that is gone is removed
 * and deleted; all of it again while the list keeps changing. */
void UpdateCrates(void)
{
    s32 i;

    DetonateNitroCrates();
    do {
        gCrateListChanged = 0;
        for (i = 0; i < gCrateList->count; i++) {
            Entity *o = gCrateList->slots[i];

            if (o->GetClassId() == 3) {
                if (o->f.flags & 1) {
                    Crates()->RemoveAt(i);
                    delete o;
                    i--;
                } else {
                    o->Update();
                }
            }
        }
    } while (gCrateListChanged);
}

/* Every idle nitro crate of the crate list explodes. */
void DetonateNitroCrates(void)
{
    s32 i = 0;

    if (i < Crates()->count) {
        do {
            Crate *o = Crates()->slots[i];

            if (o->GetClassId() == 3 && o->kind == CRATE_KIND_NITRO) {
                if ((o->state & CRATE_STATE_MASK) == 0)
                    o->Explode(0);
            }
            i++;
        } while (i < Crates()->count);
    }
}

/* The nitro switch crate, once: its pressed animation and palette, every
 * nitro crate in the room explodes, the crate counter shows, and the
 * switch counts as pressed. */
void Crate::ActivateNitroSwitch()
{
    if (pressed == 0) {
        s32 one;
        const struct sprite_anim *anims;
        const struct sprite_anim *a;
        u32 slot;

        state |= CRATE_STATE_BUSY;
        {
            Player *player = gPlayer;
            one = 1;
            player->busy = one;
        }
        SetTag(0x23);
        anims = bank->anims;
        a = &anims[tag];
        slot = gPaletteCache->GetSlot(a->paletteId);
        palette = slot;
        DetonateNitroCrates();
        gHud->ShowCrates();
        PlaySfx(gAudioContext, SFX_EXPLOSION, 0x100);
        pressed = 1;
        PressSwitchCrate(gLevelState);
    }
}

/* The iron switch crate, once: its pressed animation and palette, its
 * entity marked activated, and the group of the room's idle outline
 * crates with its group id (paramA) collected (up to 32; each marked
 * activated), which SolidifyOutlineCrates then turns solid one step at a
 * time. */
void Crate::ActivateIronSwitch()
{
    Crate *found[32];
    s32 n = 0;
    s32 i;

    if (group == PHYS_NO_GROUP)
        return;
    if (group != 0)
        return;

    f.flags |= 0x10;
    Crates()->LinkActive(this);
    state |= CRATE_STATE_BUSY;
    SetPlayerBusy();
    SetTag(0x22);
    {
        const struct sprite_anim *anims = bank->anims;
        const struct sprite_anim *a = &anims[tag];
        u32 slot = gPaletteCache->GetSlot(a->paletteId);

        palette = slot;
    }
    MarkEntityIdActivated(gEntityFlags, id);

    i = 0;
    if (i < Crates()->count) {
        do {
            Crate *o = Crates()->slots[i];

            if (o->GetClassId() == 3 && (o->state & CRATE_STATE_MASK) == 0) {
                if (o->kind == CRATE_KIND_OUTLINE && o->paramA == paramA) {
                    found[n] = o;
                    n++;
                    n &= 0x1f;
                    MarkEntityIdActivated(gEntityFlags, o->id);
                }
            }
            i++;
        } while (i < Crates()->count);
    }

    if (n != 0) {
        struct crate_group *g = (struct crate_group *)OperatorNewArray((n + 1) * 4);

        groupAllocated = 1;
        g->count = n;
        /* Stored through the block as an array of words (`g[i + 1]`):
         * written as `g->items[i]`, the loop's code differs from the ROM's. */
        for (i = 0; i < n; i++)
            ((Crate **)g)[i + 1] = found[i];
        group = g;
    } else {
        group = PHYS_NO_GROUP;
    }
    paramA = 0;
    timer = fallSpeed;
}

/* One step of an activated iron switch crate (when its timer runs out):
 * the step counter (paramA) goes up, and the outline crates of its group
 * whose step (paramB) it has reached turn solid, with one sound. After
 * the last step the group is freed and the switch becomes an iron crate. */
void Crate::SolidifyOutlines()
{
    if (timer != 0)
        return;

    if (++paramA >= paramB) {
        struct crate_group *g = group;

        if (PHYS_HAS_GROUP(g)) {
            if (g != 0)
                OperatorDeleteArray(g);
            groupAllocated = 0;
        }
        group = PHYS_NO_GROUP;
        {
            u8 k = CRATE_KIND_IRON;
            kind = k;
        }
    } else {
        struct crate_group *g = group;

        if (PHYS_HAS_GROUP(g)) {
            s32 i;
            s32 n = g->count;
            Crate **items = g->items;
            s32 played = FALSE;

            for (i = 0; i < n; i++) {
                Crate *o = items[i];

                if (o->kind == CRATE_KIND_OUTLINE && paramA >= o->paramB) {
                    o->SolidifyOutline();
                    if (!played) {
                        PlaySfx(gAudioContext, SFX_OUTLINE_CRATES_SOLIDIFY, 0x100);
                        played = TRUE;
                    }
                }
            }
        }
        timer = fallSpeed;
    }
}

/* An outline crate turns into the crate it outlines (`solidKind`, an
 * entity type): its kind, its animation and its palette. */
void Crate::SolidifyOutline()
{
    kind = solidKind - ENTITY_BASIC_CRATE;
    switch (kind) {
    case CRATE_KIND_BASIC:
        SetTag(0x1f);
        break;
    case CRATE_KIND_CHECKPOINT:
        SetTag(0x1a);
        break;
    case CRATE_KIND_AKU_AKU:
        SetTag(0x17);
        break;
    case CRATE_KIND_ARROW:
        SetTag(0x18);
        break;
    case CRATE_KIND_NITRO_SWITCH:
        SetTag(4);
        break;
    case CRATE_KIND_IRON:
        SetTag(0x20);
        break;
    case CRATE_KIND_IRON_ARROW:
        SetTag(2);
        break;
    case CRATE_KIND_NITRO:
        SetTag(5);
        break;
    case CRATE_KIND_BOUNCY_WUMPA:
        bounceTimer = -0x2a;
        SetTag(0x19);
        break;
    case CRATE_KIND_REINFORCED:
        SetTag(6);
        break;
    case CRATE_KIND_TNT:
        SetTag(0x11);
        break;
    case CRATE_KIND_TIME_1:
        SetTag(0xe);
        break;
    case CRATE_KIND_TIME_2:
        SetTag(0xf);
        break;
    case CRATE_KIND_TIME_3:
        SetTag(0x10);
        break;
    }
    palette = GetAnimPaletteSlot();
}

/* Every idle crate of the crate list within `dist` pixels (|dx| + |dy|)
 * of (x, y), and less than `height` pixels above or below it, explodes,
 * if it is explosive, or opens or breaks, if it is breakable (the
 * player's super body slam, action_ctrl_hang.cpp). */
void BreakCratesInArea(s32 x, s32 y, s32 dist, s32 height)
{
    s32 i = 0;

    if (i < gCrateList->count) {
        const u8 *commit = gCrateKindExplosive;

        do {
            Crate *o = gCrateList->slots[i];

            if (o->GetClassId() == 3) {
                s32 t1 = Q8_TO_INT(o->x) - x;
                s32 dx = ABS_BRANCHLESS(t1);
                s32 t2 = Q8_TO_INT(o->y) - y;
                s32 dy = ABS_BRANCHLESS(t2);

                if (dx + dy <= dist && dy < height && (o->state & CRATE_STATE_MASK) == 0) {
                    /* `kind + table`, as in BlastNearbyCrates */
                    if (*(u8 *)(o->kind + (u32)commit))
                        o->Explode(0);
                    else if (gCrateKindBreakable[o->kind]) {
                        if (o->kind == CRATE_KIND_CHECKPOINT)
                            o->OpenCheckpoint();
                        else
                            o->BreakInStack(0, 0, 0);
                    }
                }
            }
            i++;
        } while (i < gCrateList->count);
    }
}

/* Entry `i` of the player's ring of touched crates (GetPlayerListEntry
 * inlined): none while the ring is locked (`ctrlMode`). */
static inline Crate *RingAt(Player *p, s32 i)
{
    if (p->ctrlMode == 0 && (i <= 4 || i < p->listCount))
        return (Crate *)p->list[i];
    return 0;
}

/* A committed (breaking) crate's tick: an explosive one blasts at steps
 * 3 and 6 of its animation. When the animation ends, the crate leaves its
 * stack and, unless it is a checkpoint crate, is marked gone (and removed
 * from the player's ring of touched crates, which empties). */
void Crate::FinishBroken()
{
    if (gCrateKindExplosive[kind] && stepTimer == 0) {
        if (frame == 3)
            BlastNearby(0x14);
        else if (frame == 6)
            BlastNearby(0x28);
    }

    if (animDone) {
        Crate *prev = GetBelow();
        Crate *next = GetAbove();
        s32 i;

        if (prev != 0 && next != 0) {
            next->SetBelow(prev);
            prev->SetAbove(next);
        } else if (next != 0) {
            next->SetBelow(0);
        } else if (prev != 0) {
            prev->SetAbove(0);
        }

        if (kind == CRATE_KIND_CHECKPOINT)
            return;
        gCrateListChanged = 1;
        MarkGone();
        i = 0;
        if (i < gPlayer->listCount) {
            do {
                if (RingAt(gPlayer, i) == this)
                    gPlayer->listCount = 0;
                i++;
            } while (i < gPlayer->listCount);
        }
    } else if (kind != CRATE_KIND_CHECKPOINT) {
        gCrateListChanged = 1;
    }
}

/* A lit TNT crate's countdown, each time its timer runs out: 3, 2 (each
 * with its animation and tick), then it explodes (if idle). */
void Crate::UpdateTntCountdown()
{
    u8 k;

    if (timer != 0)
        return;

    k = kind;
    switch (k) {
    case CRATE_KIND_TNT_LIT_3:
        SetTag(0x13);
        PlaySfx(gAudioContext, SFX_TNT_TICK, 0x100);
        kind = CRATE_KIND_TNT_LIT_2;
        timer = 0x3c;
        break;
    case CRATE_KIND_TNT_LIT_2:
        SetTag(0x12);
        PlaySfx(gAudioContext, SFX_TNT_TICK, 0x100);
        kind = CRATE_KIND_TNT_LIT_1;
        timer = 0x3c;
        break;
    case CRATE_KIND_TNT_LIT_1:
        if ((state & CRATE_STATE_MASK) == 0)
            Explode(0);
        break;
    }
}

/* The slot crate's tick (slotState: see crate.h's CRATE_SLOT_*). An idle
 * one (stage 0) starts at stage 1 when the player comes within 0x4F by
 * 0x3F pixels. Each time its timer runs out, it turns to its next face:
 * once spinning, the faces it may stop on (paramA's mask), counting down
 * the stage's spins and moving on a stage after them (after stage 3 it
 * turns to iron); else its idle animations. The timer is reloaded from
 * gSlotCrateTimers by stage.
 *
 * The phase test (`w1`), the loop (`lw`) and the `0x38` switch (`w2`)
 * each have their own local; `lw`'s phase is cleared and set in place
 * (`lw &= ...; lw |= nx`), which keeps it in r1. The count update is
 * written as separate in-place
 * steps on a fresh local (`t = (r - 1) << 24; cw &= 0xc7; t >>= 21;
 * cw |= t`), which ties the `& 0xc7` to the reloaded word's register and
 * the shift to `t`'s, as the ROM does; a single `(w & 0xc7) | (t << 3)`
 * expression left 7 halfwords off. */
void Crate::UpdateSlot()
{
    s32 w;
    s32 ph0;
    s32 w1;

    w = slotState;
    if (!(w & CRATE_SLOT_STAGE_MASK)) {
        Player *pl = gPlayer;
        s32 d;

        d = Q8_TO_INT(pl->x);
        d -= Q8_TO_INT(x);
        MAKE_ABS(d);
        if (d <= 0x4f) {
            d = Q8_TO_INT(pl->y);
            d -= Q8_TO_INT(y);
            MAKE_ABS(d);
            if (d <= 0x3f) {
                w &= CRATE_SLOT_CLEAR_STAGE;
                w |= 0x40;
                w &= CRATE_SLOT_CLEAR_SPINS;
                w |= 0x10;
                slotState = w;
            }
        }
    }
    if (timer != 0)
        return;
    ph0 = slotState & CRATE_SLOT_PHASE_MASK;
    ph0 &= CRATE_SLOT_PHASE_STARTED;
    w1 = slotState;
    if (ph0 && tag == 8) {
        s32 done = 0;

        do {
            s32 ph;
            s32 nx;
            s32 lw;

            lw = slotState;
            nx = ((lw & CRATE_SLOT_PHASE_MASK) + 1) & 3;
            ph = nx;
            lw &= CRATE_SLOT_CLEAR_PHASE;
            lw |= nx;
            slotState = lw;
            switch (ph) {
            case 0:
                SetTag(7);
                if (slotState & CRATE_SLOT_STAGE_MASK) {
                    u8 r = GetSlotSpins();

                    if (r != 0) {
                        u32 t = (r - 1) << 24;
                        s32 cw = slotState;

                        cw &= CRATE_SLOT_CLEAR_SPINS;
                        t >>= 21;
                        cw |= t;
                        slotState = cw;
                    }
                    w = slotState;
                    if (!(w & CRATE_SLOT_SPINS_MASK)) {
                        s32 w2 = (w & CRATE_SLOT_CLEAR_SPINS) | 0x10;

                        slotState = w2;
                        switch (
                            (s32)((u32)(w2 & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT)) {
                        case 1:
                            slotState = (w2 & CRATE_SLOT_CLEAR_STAGE) | 0x80;
                            break;
                        case 2:
                            slotState = (w2 & CRATE_SLOT_CLEAR_STAGE) | 0xc0;
                            break;
                        case 3:
                            SetTag(0x20);
                            kind = CRATE_KIND_IRON;
                            break;
                        }
                    }
                }
                goto out;
            case 1:
                if (paramA & 2) {
                    SetTag(9);
                    goto out;
                }
                break;
            case 2:
                if (paramA & 1) {
                    SetTag(0xb);
                    goto out;
                }
                break;
            case 3:
                if (paramA & 4) {
                    SetTag(0xd);
                    done = 1;
                }
                break;
            }
        } while (!done);
    out:
        {
            const struct sprite_anim *anims = bank->anims;
            const struct sprite_anim *a = &anims[tag];
            u32 slot = gPaletteCache->GetSlot(a->paletteId);

            palette = slot;
        }
        {
            s32 d = (s32)((u32)(slotState & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT);

            timer = gSlotCrateTimers[d];
        }
    } else {
        {
            s32 p = (w1 & CRATE_SLOT_PHASE_MASK) | CRATE_SLOT_PHASE_STARTED;

            w1 = p | (w1 & CRATE_SLOT_CLEAR_PHASE);
        }
        slotState = w1;
        timer = 1;
        if (tag == 0xc)
            SetTag(0xa);
        else if (tag == 0xa)
            SetTag(8);
        else {
            switch ((s32)((u32)(slotState & CRATE_SLOT_STAGE_MASK) >> CRATE_SLOT_STAGE_SHIFT)) {
            case 0:
            case 1:
                SetTag(0xc);
                break;
            case 2:
                SetTag(0xa);
                break;
            case 3:
                SetTag(8);
                break;
            }
        }
        {
            const struct sprite_anim *anims = bank->anims;
            const struct sprite_anim *a = &anims[tag];
            u32 slot = gPaletteCache->GetSlot(a->paletteId);

            palette = slot;
        }
        if (slotState & CRATE_SLOT_STAGE_MASK)
            PlaySfx(gAudioContext, SFX_SLOT_CRATE_SPIN, 0x100);
    }
}

/* The crate's fall (fallDistance, Q8), one step per unit of speed: down
 * 0x100 a step (0x40 in the player's slow mode), or up while the speed is
 * negative. When it lands, it snaps to its target, and an explosive crate
 * explodes (if it was set to, or is a nitro crate) or, a TNT crate, lights
 * unless it is the top of a stack; the TNT crates below the one under it
 * light too. The speed then ramps up to 5, or clears once the fall is
 * done.
 *
 * Matching notes (docs/matching/archive/issue-12-13-25-naked-retry.md): the
 * speed byte is re-read through `fallSpeed` each time (GCSE keeps
 * its address in sb), `speed--` is written in both step arms, the
 * neighbour walk skips the first neighbour, and one temporary `t` both
 * carries `fallTargetY` into `y` and re-reads `x` at the bottom of the loop
 * (the ROM's r1). */
void Crate::UpdateFall()
{
    s32 remaining = fallDistance;
    s32 speed;
    s32 acc;
    s32 t;

    if (remaining == 0)
        return;
    gCrateListChanged = 1;
    speed = fallSpeed;
    if (speed == 0)
        speed = 1;
    acc = 0;
    t = x;
    if (speed < 0) {
        do {
            acc -= 0x100;
            remaining += 0x100;
            speed++;
        } while (speed != 0);
    } else {
        do {
            if (remaining > 0) {
                if (gPlayer->ctrlMode == 1) {
                    acc += 0x40;
                    remaining -= 0x40;
                    speed--;
                } else {
                    acc += 0x100;
                    remaining -= 0x100;
                    speed--;
                }
            } else {
                Crate *n;
                u8 k;

                speed = 1;
                t = fallTargetY;
                y = t;
                acc = 0;
                fallDistance = remaining;
                if (gCrateKindExplosive[k = kind]) {
                    if (blastState != 0 || k == CRATE_KIND_NITRO) {
                        if ((state & CRATE_STATE_MASK) == 0)
                            Explode(0);
                    } else if (k == CRATE_KIND_TNT) {
                        Crate *next = GetAbove();
                        Crate *prev = GetBelow();

                        if (next != 0 || prev == 0)
                            LightTnt();
                    }
                }
                n = GetBelow();
                speed--;
                if (n != 0) {
                    n = n->GetBelow();
                    while (n != 0) {
                        if (n->kind == CRATE_KIND_TNT)
                            n->LightTnt();
                        n = n->GetBelow();
                    }
                }
            }
            t = x;
        } while (speed != 0);
    }
    {
        s32 ny = y + acc;

        x = t;
        y = ny;
    }
    fallDistance = remaining;
    if (remaining == 0)
        fallSpeed = 0;
    else {
        if (++fallSpeed == 0)
            ++fallSpeed;
        LIMIT_MAX(fallSpeed, 5);
    }
}
