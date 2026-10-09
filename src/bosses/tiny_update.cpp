#include "bg_layer.hpp"
#include "boss_ctrl.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "match.h"
#include "sprite_bank.h"
#include "util.h"
#include "memory.h"
#include "level.h"
#include "box_part.h"
#include "gfx_part.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #22, ROM 0x08018008-0x080187FC, formerly
 * asm/code_3_2_17_18008.s (details in
 * docs/matching/archive/issue-22-0x08018008-hopper.md). Built with
 * old_agbcp (Makefile OLD_AGBCC_OBJS), like cortex.cpp right after it.
 *
 * TinyCtrl's (include/boss_ctrl.hpp, gTinyVtable) per-frame Update and
 * "enter state" SetState: Tiny hops his `part` along parabolic arcs (the
 * 257-entry i*i>>8 table `squares`) between gTouchableList's anchors,
 * stomping them. PickHopTarget picks the next anchor from a per-round
 * table, SpawnFallingLeaves spawns a falling hazard. */

/* The player's HandleEvent (Player, include/player.hpp). */
static inline void HitPlayer(Player *pl)
{
    pl->HandleEvent(0, EVENT_HIT, 0);
}

/* `*flags &= mask` with an s32 mask: in place, g++ folds `~4` to the byte
 * constant 0xFB, where the ROM has -5 (`movs #5; negs`). */
static inline void ClearFlags(u8 *flags, s32 mask)
{
    *flags &= mask;
}

void TinyCtrl::Update(MovingSprite *part)
{
    struct aabb a;
    struct aabb b;
    s32 state;

    if (stomped != -1) {
        MovingSprite *pad = (MovingSprite *)gTouchableList->items[stomped];
        Ctrl *ctrl;

        delete pad->mover;
        ctrl = new StompedHopPadCtrl;
        pad->mover = ctrl;
        ctrl->Attach(pad);
        stomped = -1;
        gAudioContext->PlaySfx(SFX_UNKNOWN_39, 0x100);
    }

    if (this->state == 8) {
        GetSpriteAttackBox(&a, gPlayer);
        GetSpriteBodyBox(&b, part);
        if (a.w != 0 && AABB_VALID(b) && AabbOverlaps(&b, &a) && gPlayer->kind == 0x13)
            SetState(part, 9);
    } else if (gPlayer->dead == 0) {
        GetSpriteBodyBox(&a, gPlayer);
        if (a.w == 0) {
            GetSpriteAttackBox(&b, gPlayer);
            a = b;
        }
        GetSpriteAttackBox(&b, part);
        if (AABB_VALID(b) && a.w != 0 && AabbOverlaps(&b, &a))
            HitPlayer(gPlayer);
    }

    state = this->state;
    switch (state) {
    case 0:
        ClearFlags(&part->f.flags, ~4);
        {
            /* a named `one`: written as three plain `= 1`/`= 0` stores, the
             * 0 is materialized first (it is CSE'd from the dead `& 0` of
             * the byte store's expansion) */
            s32 one = 1;

            part->kind = one;
            anchor = one;
            count = 0;
        }
        SetState(part, 1);
        break;
    case 1:
    case 2:
    case 11:
    case 15:
        {
            s32 steps = --this->steps;
            s32 x = dx * steps / total + this->x;
            s32 t = Q8_DIV(steps, total);
            s32 y = Q8_MUL(0x100 - squares[0x100 - t], dy) + this->y;

            part->x = x;
            part->y = y;
            if (steps != 0)
                break;
            gAudioContext->PlaySfx(SFX_TINY_LAND, 0x100);
            if (this->state == 15) {
                if ((u8)gLevelState->HasTornadoSpin())
                    RequestRoomExit();
                SetState(part, 16);
            } else if (this->state == 1) {
                SetState(part, 6);
            } else if (this->state == 11) {
                count = steps;
                SetTargetAnim(part, 3);
                nextState = 12;
                SetState(part, 5);
            } else {
                gAudioContext->PlaySfx(SFX_UNKNOWN_3D, 0x100);
                SetState(part, 8);
            }
            break;
        }
    case 3:
    case 7:
    case 10:
    case 14:
        {
            s32 steps = --this->steps;
            s32 x = dx * steps / total + this->x;
            s32 t = Q8_DIV(steps, total);
            s32 y = Q8_MUL(squares[t], dy) + this->y;

            part->x = x;
            part->y = y;
            if (steps != 0)
                break;
            if (this->state == 14)
                SetState(part, 15);
            else if (this->state == 3)
                SetState(part, 1);
            else if (this->state == 7)
                SetState(part, 2);
            else
                SetState(part, 13);
            break;
        }
    case 13:
        if (nextState == 0) {
            s32 timer = --this->timer;

            if (timer == 0) {
                SetState(part, 11);
            } else {
                nextState = 0x46;
                SpawnFallingLeaves(part, timer);
            }
        }
        nextState--;
        break;
    case 6:
        if (part->animDone) {
            if (++count > 3) {
                count = 0;
                SetState(part, 7);
            } else {
                SetState(part, 3);
            }
        }
        break;
    case 8:
        if (timer != 0) {
            timer--;
            break;
        }
        timer--;
        SetTargetAnim(part, 0);
        nextState = 3;
        SetState(part, 5);
        break;
    case 5:
        if (part->animDone)
            SetState(part, nextState);
    case 4:
        if (--timer == 0)
            SetState(part, nextState);
        break;
    case 12:
        stomped = gTinyRoundAnchors[counter - 1];
        SetState(part, 3);
        break;
    case 9:
        if (counter > 2)
            SetState(part, 14);
        if (part->animDone) {
            SetState(part, 10);
            gAudioContext->PlaySfx(SFX_JUMP, 0x100);
        }
        break;
    }
}

/* Sets the animation (no reset). */
static inline void SetTag(MovingSprite *p, s32 tag)
{
    p->tag = tag;
}

/* Sets the frame, clamped to the current animation's frame count. */
static inline void SetFrame(MovingSprite *part, s32 frame)
{
    const struct sprite_bank *bank = part->bank;
    u8 *tag = &part->tag;
    const struct sprite_anim *records = bank->anims;
    s32 count = records[*tag].frameCount;

    CLAMP_INDEX(frame, count);
    part->frame = frame;
}

void TinyCtrl::SetState(MovingSprite *part, s32 next)
{
    switch (next) {
    case 13:
        nextState = 0;
        timer = 4;
    case 11:
        anchor = gTinyRoundAnchors[counter - 1];
    case 1:
    case 2:
        {
            /* pinned, as in the C (under agbcp and old_agbcp): unpinned,
             * `pad` lands in r0 where the ROM has r2, and pinning it
             * alone moves `x` off r1. #662 round 2: `pad`/`x` at function
             * scope or in nested blocks, TouchableList(), an inline
             * returning the pad, and `pad->x` read again all leave `pad`
             * in r0. */
            MATCH_HOLD_REG(MovingSprite *, pad, r2);
            MATCH_HOLD_REG(s32, x, r1);

            if (next == 1)
                SetTargetAnim(part, 2);
            else
                SetTargetAnim(part, 1);
            SetFrame(part, 4);
            pad = (MovingSprite *)gTouchableList->items[anchor];
            x = pad->x;
            this->x = x;
            if (next == 11 || next == 13)
                part->x = x;
            this->y = pad->y - 0x2400;
            goto hop;
        }
    case 3:
    case 7:
        {
            MovingSprite *pad;
            s32 ax;
            s32 ay;
            s32 y;

            anchor = PickHopTarget();
            SetTargetAnim(part, 4);
            pad = (MovingSprite *)gTouchableList->items[anchor];
            ax = pad->x;
            ay = pad->y;
            y = ay - 0x2400;
            this->x = (ax + part->x) >> 1;
            if (y >= part->y)
                y = part->y - 0x5900;
            else
                y = ay - 0x7D00;
            this->y = y;
            goto hop;
        }
    case 10:
        this->y = -0x3000;
        this->x = part->x;
    hop:
        StartHop(part);
        break;
    case 8:
        SetTargetAnim(part, 6);
        timer = 0xB4;
        break;
    case 9:
        gAudioContext->PlaySfx(SFX_BOSS_HIT, 0x100);
        if (++counter > 2) {
            s32 x = part->x;

            this->y = part->y - 0x6400;
            this->x = x + 0x6400;
            StartHop(part);
        }
        TinyHitStub(this, part);
        SetTargetAnim(part, 7);
        break;
    case 14:
        {
            MovingSprite *pad = (MovingSprite *)gTouchableList->items[2];
            s32 x = pad->x;
            s32 y = pad->y - 0x1800;

            if (!(u8)gLevelState->HasTornadoSpin())
                SpawnTornadoSpinPower(0xFFFF, Q8_TO_INT(x), Q8_TO_INT(y), 0);
            break;
        }
    case 15:
        {
            s32 x = part->x;

            this->x = x;
            this->y = INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x4000;
            this->x = x + 0x6400;
            StartHop(part);
            break;
        }
    }
    SetMode(next);
}

/* Picks the hop target: the anchor nearest the player selects a column
 * of this round's/current anchor's gTinyHopTargets row. */
s32 TinyCtrl::PickHopTarget()
{
    s32 nearest = 0;
    s32 best = 0xFFFFFF;
    s32 i;

    for (i = 0; i < gTouchableList->count; i++) {
        MovingSprite *pad = (MovingSprite *)gTouchableList->items[i];
        s32 px = gPlayer->x;
        s32 py = gPlayer->y;
        s32 ax = pad->x;
        s32 ay = pad->y;
        s32 d = ABS_BRANCHLESS(ax - px) + ABS_BRANCHLESS(ay - py);

        if (d < best) {
            best = d;
            nearest = i;
        }
    }
    return gTinyHopTargets[anchor * 5 + nearest + counter * 25];
}

/* Spawns a falling hazard (a gCollidableList part driven by a
 * OneShotAnimCtrl) at the `n`th third of the way from `part` towards
 * the player. */
void TinyCtrl::SpawnFallingLeaves(MovingSprite *part, s32 n)
{
    MovingSprite *p = MovingSprite::Create(0xFFFF, 0, 0, 0);
    Ctrl *ctrl;
    s32 x;

    p->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x294);
    /* through an s32: a constant stored straight is loaded after the
     * address, where the ROM loads it first */
    SetTag(p, 5);
    p->ResetFrameTimer();
    p->ResetFrameIndex();
    p->SetAnimDone(0);
    ctrl = new OneShotAnimCtrl;
    p->palette = p->GetAnimPaletteSlot();
    p->mover = ctrl;
    ctrl->Attach(p);
    p->f.flags |= 0x10;
    CollidableList()->Add(p);
    {
        /* the ROM materializes 0x80 before storing the 0s */
        s32 k = 0x80;

        p->speedY = 0;
        p->rampY.start = 0;
        p->rampY.step = k;
        p->rampY.target = k;
    }
    {
        s32 x0 = part->x;
        s32 y;

        x = x0 + (gPlayer->x - x0) * (n - 1) / 3;
        y = INT_TO_Q8(gLevelLayers->layer0->y);
        p->x = x;
        p->y = y;
    }
    p->kind = 1;
    {
        /* two masks, not folded to -0x45; the -5 is derived from the 1 */
        s32 m = -5;

        m &= p->f.flags;
        m &= -0x41;
        p->f.flags = m;
    }
    gAudioContext->PlaySfx(SFX_UNKNOWN_13, 0x100);
}
