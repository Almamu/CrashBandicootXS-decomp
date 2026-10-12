#include "platform.hpp"
#include "player.hpp"
#include "level_state.hpp"

extern "C" {
#include "gfx.h"
#include "util.h"
#include "crates.h"
#include "level.h"
}

/* Platform::ResolveCollision (#664, include/platform.hpp), ROM
 * 0x0801AB98-0x0801B208: ResolvePlatformCollision, the player-vs-platform
 * collision CheckPlayerContact runs (see
 * docs/matching/archive/issue-25-level-objects.md for what it computes). */

/* Returns its argument: reading `pos` through it keeps the address in a
 * register, as the ROM does. */
static inline struct vec2 *PosPtr(struct vec2 *p)
{
    return p;
}

/* x + w - o with the three operands evaluated first */
static inline s32 Span(s32 x, s32 w, s32 o)
{
    return x + w - o;
}

void Platform::ResolveCollision(void *)
{
    struct vec2 pos;
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
    const struct hitbox_quad *box;
    s32 result;
    s32 flags;
    s32 r;
    struct aabb a = GetAnimHitbox();

    px = Q8_TO_INT(gPlayer->x);
    py = Q8_TO_INT(gPlayer->y);
    /* The player's box. Its address stays in r4 for the overlap tests,
     * while both boxes' fields are read through aabb.h's accessors, at
     * their own sp offsets, as in the ROM. Read as `b.y`, a field goes
     * through a fresh copy of `&b` that cse1 ties to r4's pseudo
     * (`ldr r1, [r4, #4]`); an inlined accessor's argument is the
     * constant `fp + 16` itself (#662 round 4, aabb.h). Until then the C
     * hid r4's value with an empty asm and filled the box through an
     * explicit-destination alias of GetSpriteHitbox. */
    struct aabb b = gPlayer->GetAnimHitbox();

    box = &gPlayer->bank->anims[gPlayer->tag].box[0];
    if (AabbOverlaps(&a, &b)) {
        result = 0;
        above = 0;
        if (AabbY(&b) < AabbY(&a))
            above = 1;
        tx = gPlayer->GetPrevX();
        ty = gPlayer->GetPrevY();
        side = PART_DIR_LEFT;
        if (px > tx)
            side = PART_DIR_RIGHT;
        if (Q8_TO_INT(gPlayer->x) < Q8_TO_INT(x)) {
            hdir = PART_DIR_RIGHT;
            ox = Span(AabbX(&b), AabbW(&b), AabbX(&a)) + 1;
        } else {
            hdir = PART_DIR_LEFT;
            ox = Span(AabbX(&a), AabbW(&a), AabbX(&b)) + 1;
        }
        if (Q8_TO_INT(gPlayer->y) > Q8_TO_INT(y)) {
            vdir = PART_DIR_UP;
            oy = Span(AabbY(&a), AabbH(&a), AabbY(&b));
        } else {
            vdir = PART_DIR_DOWN;
            oy = Span(AabbY(&b), AabbH(&b), AabbY(&a));
        }
        if (type != PLATFORM_TYPE_MOVING && type != PLATFORM_TYPE_FALLING &&
            type != PLATFORM_TYPE_FLIP) {
            if (ty == py && tx == px) {
                result = hdir;
                if (oy <= 2)
                    result = PART_DIR_DOWN;
            } else if (ty == py && GetPrevY() == Q8_TO_INT(y) && oy > 2)
                result = hdir;
            else if (tx == px && GetPrevX() == Q8_TO_INT(x))
                result = vdir;
        }
        if (ty <= py && above) {
            if (result == 0) {
                ty += box->offY + box->h;
                if (ty <= AabbY(&a) + AabbH(&a)) {
                    if (side == PART_DIR_RIGHT) {
                        if (AabbX(&b) + AabbW(&b) >= AabbX(&a) && ox > 2)
                            result = PART_DIR_DOWN;
                    } else {
                        if (AabbX(&b) <= AabbX(&a) + AabbW(&a) && ox > 2)
                            result = PART_DIR_DOWN;
                    }
                    if (result == 0) {
                        py += box->offY + box->h;
                        if (hdir == PART_DIR_RIGHT) {
                            tx += box->offX + box->w;
                            px = AabbX(&b) + AabbW(&b);
                            r = FindLineCrossing(tx, ty, px, py, AabbX(&a));
                        } else {
                            tx += box->offX;
                            px = AabbX(&b);
                            r = FindLineCrossing(tx, ty, px, py, AabbX(&a) + AabbW(&a));
                        }
                        if ((r < 0 && above && oy <= 1) || (r > 0 && r <= AabbY(&a)))
                            result = PART_DIR_DOWN;
                        else
                            result = hdir;
                    }
                } else
                    result = hdir;
            }
        } else {
            if (result == 0) {
                ty += box->offY;
                if (ty >= AabbY(&a)) {
                    if (side == PART_DIR_RIGHT) {
                        if (AabbX(&b) + AabbW(&b) >= AabbX(&a) && ox > 3)
                            result = PART_DIR_UP;
                    } else {
                        if (AabbX(&b) <= AabbX(&a) + AabbW(&a) && ox > 3)
                            result = PART_DIR_UP;
                    }
                    if (result == 0) {
                        py += box->offY;
                        if (hdir == PART_DIR_RIGHT) {
                            tx += box->offX + box->w;
                            px = AabbX(&b) + AabbW(&b);
                            r = FindLineCrossing(tx, ty, px, py, AabbX(&a));
                        } else {
                            tx += box->offX;
                            px = AabbX(&b);
                            r = FindLineCrossing(tx, ty, px, py, AabbX(&a) + AabbW(&a));
                        }
                        if ((r < 0 && !above && ox > 3) || (r > 0 && r >= AabbY(&a) + AabbH(&a)))
                            result = PART_DIR_UP;
                        else
                            result = hdir;
                    }
                } else
                    result = hdir;
            }
        }

        pos.x = gPlayer->x;
        pos.y = gPlayer->y;
        LIMIT_MIN(ox, 0);
        LIMIT_MIN(oy, 0);
        flags = 0;
        switch (result) {
        case 0: /* empty case: makes gcc use the ROM's jump table */
            break;
        case PART_DIR_UP:
            {
                Player *q = gPlayer;

                if (!(q->hitAxes & PART_DIR_DOWN)) {
                    q->HandleEvent(0, EVENT_BUMP, PART_DIR_UP);
                    PosPtr(&pos)->y = INT_TO_Q8(oy) + PosPtr(&pos)->y;
                }
            }
            break;
        case PART_DIR_DOWN:
            PosPtr(&pos)->y = (PosPtr(&pos)->y - INT_TO_Q8(oy - 1)) & ~0xFF;
            break;
        case PART_DIR_RIGHT:
        case PART_DIR_LEFT:
            flags = hdir;
            if (flags == PART_DIR_LEFT)
                pos.x = INT_TO_Q8(ox) + pos.x;
            else if (flags == PART_DIR_RIGHT)
                pos.x -= INT_TO_Q8(ox);
            break;
        }
        if (result == PART_DIR_DOWN || oy <= 1) {
            Player *q = gPlayer;

            if (!(q->dir & PART_DIR_UP) && above) {
                q->carried = this;

                u8 m = PART_DIR_DOWN;

                q->hitAxes = m;

                s32 y = gPlayer->y;

                PosPtr(&pos)->y = y - INT_TO_Q8(oy - 1);
                flags = 0;
                pos.x = gPlayer->x;
            }
        }
        SetEntityPos(gPlayer, pos.x, PosPtr(&pos)->y);
        if (flags) {
            gPlayer->HandleEvent(0, EVENT_BUMP, flags);
            gPlayer->hitMask |= flags;
        }
        if (result == PART_DIR_DOWN) {
            s32 t = type;

            if (t == PLATFORM_TYPE_MOVING || t == PLATFORM_TYPE_FALLING || t == PLATFORM_TYPE_FLIP)
                Mover()->active = 1;
            else {
                s32 d = Q8_TO_INT(x) - Q8_TO_INT(gPlayer->x);
                s32 sign;

                MAKE_ABS_BRANCHLESS(d, sign);
                if (d <= 7) {
                    switch (t) {
                    case PLATFORM_TYPE_WARP_EXIT:
                        gPlayer->HandleEvent(0, EVENT_WARP_EXIT, 0);
                        break;
                    case PLATFORM_TYPE_WARP_BONUS:
                        if (!gLevelState->IsBonusRoundDone() && !gLevelState->timeTrial)
                            gPlayer->HandleEvent(0, EVENT_WARP_BONUS_ROUND, 0);
                        break;
                    case PLATFORM_TYPE_WARP_GEM_PATH:
                        if (!gLevelState->IsGemPathDone() && !gLevelState->timeTrial)
                            gPlayer->HandleEvent(0, EVENT_WARP_GEM_PATH, 0);
                        break;
                    }
                }
            }
        }
    } else {
        a.y -= 4;
        a.h += 4;
        switch (type) {
        case PLATFORM_TYPE_STILL:
        case PLATFORM_TYPE_ROCK:
            if (AabbOverlaps(&a, &b)) {
                Player *q = gPlayer;

                q->carried = this;

                u8 m = PART_DIR_DOWN;

                q->hitAxes = m;
            }
            break;
        case PLATFORM_TYPE_WARP_EXIT:
            if (AabbOverlaps(&a, &b)) {
                s32 d = Q8_TO_INT(x) - Q8_TO_INT(gPlayer->x);
                s32 sign;

                MAKE_ABS_BRANCHLESS(d, sign);
                if (d <= 7)
                    gPlayer->HandleEvent(0, EVENT_WARP_EXIT, 0);
            }
            break;
        case PLATFORM_TYPE_WARP_BONUS:
            if (!gLevelState->IsBonusRoundDone() && !gLevelState->timeTrial &&
                AabbOverlaps(&a, &b)) {
                s32 d = Q8_TO_INT(x) - Q8_TO_INT(gPlayer->x);
                s32 sign;

                MAKE_ABS_BRANCHLESS(d, sign);
                if (d <= 7)
                    gPlayer->HandleEvent(0, EVENT_WARP_BONUS_ROUND, 0);
            }
            break;
        case PLATFORM_TYPE_WARP_GEM_PATH:
            if (!gLevelState->IsGemPathDone() && !gLevelState->timeTrial && AabbOverlaps(&a, &b)) {
                s32 d = Q8_TO_INT(x) - Q8_TO_INT(gPlayer->x);
                s32 sign;

                MAKE_ABS_BRANCHLESS(d, sign);
                if (d <= 7)
                    gPlayer->HandleEvent(0, EVENT_WARP_GEM_PATH, 0);
            }
            break;
        case PLATFORM_TYPE_MOVING:
        case PLATFORM_TYPE_FALLING:
        case PLATFORM_TYPE_FLIP:
            if (!AabbOverlaps(&a, &b))
                Mover()->active = 0;
            break;
        }
    }
}
