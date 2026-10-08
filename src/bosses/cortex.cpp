#include "bg_layer.hpp"
#include "boss_ctrl.hpp"
#include "platform.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "match.h"
#include "sprite_bank.h"
#include "util.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #23: 0x080188D0-0x0801967C, formerly
 * asm/code_3_2_17_188d0.s (details in docs/matching/archive/issue-23-graphics.md).
 * Built with old_agbcp (Makefile OLD_AGBCC_OBJS).
 *
 * Small controllers (include/boss_ctrl.hpp, include/ctrl.hpp,
 * include/platform.hpp), in ROM order: the one-shot animation
 * controllers' constructors and destructors, Tiny's StartHop, destructor
 * and constructor, then most of the Neo Cortex fight (sprite bank 53: the
 * Tesla cannon at three angles in anims 3-5, the green/red crosshairs in
 * 0xF/0x10/0x12, the shots in 0xE/0x11 and the shrinking gems in 9-0xD):
 * the boss controller (CortexBossCtrl) spawns the cannon and the target
 * (crosshair), the target (CortexTargetCtrl) hops across the level and
 * then chases the player, firing a shot (CortexShotCtrl) at each stop. In
 * this fight the red/green/yellow gem spawners hand over to
 * SpawnCortexBossGem (bank 32's gems, CortexBossGemCtrl), which a fast
 * shot shrinks away, and the level's type-6 platforms get a
 * CortexBossPlatformMover.
 *
 * UNUSED - no caller anywhere in the ROM (checked the asm/ and expected/
 * sources, every .c file under src/, and every word-aligned Thumb pointer
 * in baserom.gba): UnusedOneShotAnimCtrl's constructor
 * (CreateUnusedOneShotAnimCtrl). Matched anyway. */

/* The player's HandleEvent (Player, include/player.hpp). */
static inline void HitPlayer(Player *pl, s32 event)
{
    pl->HandleEvent(0, event, 0);
}

/* Switches to animation `t` from its start. `t` is an s32: with a u8
 * parameter, SpawnCortexBossGem's tag 0 is loaded after the tag's address,
 * where the ROM loads it first. */
static inline void SetTag(MovingSprite *p, s32 t)
{
    p->tag = t;
    p->ResetFrameTimer();
    p->ResetFrameIndex();
    p->SetAnimDone(0);
}

/* Sets `frame` to `idx`, clamped to the animation's last step. */
static inline void ClampFrame(MovingSprite *p, s32 idx)
{
    u8 n = p->bank->anims[p->tag].frameCount;

    CLAMP_INDEX(idx, n);
    p->frame = idx;
}

OneShotAnimCtrl::OneShotAnimCtrl()
{
}

OneShotAnimCtrl::~OneShotAnimCtrl()
{
}

/* Marks the sprite object gone once its animation has played through,
 * as OneShotAnimCtrl::Update (tiny_hop_pad.cpp) does. */
void UnusedOneShotAnimCtrl::Update(MovingSprite *part)
{
    if (part->animDone)
        part->MarkGone();
}

/* UNUSED - see the top of the file. */
UnusedOneShotAnimCtrl::UnusedOneShotAnimCtrl()
{
}

UnusedOneShotAnimCtrl::~UnusedOneShotAnimCtrl()
{
}

/* Empty. TinyCtrl::SetState's case 9 (tiny_update.cpp) calls it directly,
 * between the hop set-up and the anim-7 call, with the same (self, part)
 * arguments as StartHop below; it is in no method table, so nothing shows
 * what it was for. */
void nullsub_19(void *self, void *part)
{
}

/* Starts a hop from (x, y) to the part's position, facing it. The ROM
 * clears and sets the mirror bit in two steps (`& -0x11`, then `| 0x10`),
 * through the byte; a bitfield store of 1 is one `orr`. */
void TinyCtrl::StartHop(MovingSprite *part)
{
    s32 px = part->x;

    if (x <= px) {
        u8 *p = &part->mirror;
        s32 m = -0x11;

        m &= *p;
        m |= 0x10;
        *p = m;
    } else {
        part->mirrorFlags.mirrorX = 0;
    }
    steps = 0x1A;
    total = 0x1A;
    dy = part->y - y;
    dx = part->x - x;
}

TinyCtrl::~TinyCtrl()
{
    delete[] squares;
}

/* The hop's squares table: i * i >> 8 for i = 0..0x100. */
TinyCtrl::TinyCtrl()
{
    s32 i;

    stomped = -1;
    squares = new s16[0x101];
    for (i = 0; i <= 0x100; i++)
        squares[i] = Q8_MUL(i, i);
}

/* State 0 spawns the cannon and the target and moves to state 1; state 1
 * picks the cannon's angle (animation) from the target's height, mirrors
 * both parts towards the target and sets both parts' frame from the
 * horizontal distance (0-5, scaled by the level width); state 2 counts to
 * 3 before moving on; state 3 sinks everything 0x80 per frame until it
 * leaves the bottom of the level, then signals RequestRoomExit. */
void CortexBossCtrl::Update(MovingSprite *part)
{
    switch (state) {
    case 0:
        SpawnCannon(part);
        SpawnTarget(part);
        part->f.b.visible = 0;
        goto mode1;
    case 1:
        {
            s32 y = target->y;
            s32 n;

            if (y <= 0x5000)
                SetTag(cannon, 5);
            else if (y <= 0x7800)
                SetTag(cannon, 4);
            else
                SetTag(cannon, 3);

            n = target->x - part->x;
            part->mirrorFlags.mirrorX = n >= 0;
            cannon->mirrorFlags.mirrorX = n >= 0;
            {
                s32 w = INT_TO_Q8(gLevelLayers->layer0->widthPx);

                n = (u32)(ABS_BRANCHLESS(n) * 12) / w;
            }
            LIMIT_MAX(n, 5);
            n = 5 - n;
            ClampFrame(part, n);
            ClampFrame(cannon, n);
            break;
        }
    case 2:
        if (++counter > 2) {
            SetState(part, 3);
            break;
        }
    mode1:
        SetState(part, 1);
        break;
    case 4:
        break;
    case 3:
        cannon->y += 0x80;
        part->y += 0x80;
        if (part->y >= INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x4000) {
            if ((u8)HasTurboRun(gLevelState))
                RequestRoomExit();
            SetState(part, 4);
        }
        break;
    }
}

/* The cannon: animation 3 of bank 53, still, facing as `part` does. */
void CortexBossCtrl::SpawnCannon(MovingSprite *part)
{
    MovingSprite *c = MovingSprite::Create(0xFFFF, 0, 0, 0);
    Ctrl *ctrl;

    c->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x27C);
    SetTag(c, 3);
    c->animating = 0;
    ctrl = new CortexCannonCtrl;
    c->palette = c->GetAnimPaletteSlot();
    c->mover = ctrl;
    ctrl->Attach(c);
    c->Pos() = part->Pos();
    c->mirrorFlags.mirrorX = part->mirrorFlags.mirrorX;
    c->f.b.active = 1;
    ForegroundList()->Add(c);
    cannon = c;
}

/* The target: the green crosshair (animation 0xF), above and right of
 * `part`. */
void CortexBossCtrl::SpawnTarget(MovingSprite *part)
{
    MovingSprite *c = MovingSprite::Create(0xFFFF, 0, 0, 0);
    Ctrl *ctrl;
    s32 x, y;

    c->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x27C);
    SetTag(c, 0xF);
    c->palette = c->GetAnimPaletteSlot();
    ctrl = new CortexTargetCtrl(this);
    c->mover = ctrl;
    ctrl->Attach(c);
    x = part->x;
    y = part->y;
    x += 0x2000;
    y -= 0x4000;
    c->x = x;
    c->y = y;
    c->f.b.active = 1;
    ForegroundList()->Add(c);
    target = c;
}

/* A gem of the Neo Cortex fight (the red, green or yellow gem spawners'
 * hand-over): bank 32's gem `kind`, in the collidable list, where a fast
 * shot finds it. */
void SpawnCortexBossGem(u32 a0, u16 a1, u16 a2, u16 a3, s32 kind)
{
    MovingSprite *c = MovingSprite::Create(a0, a1, a2, a3);
    Ctrl *ctrl;

    c->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x180);
    switch (kind) {
    case 0:
        SetTag(c, 3);
        break;
    case 1:
        SetTag(c, 2);
        break;
    case 2:
        SetTag(c, 0);
        break;
    }
    c->palette = c->GetAnimPaletteSlot();
    ctrl = new CortexBossGemCtrl(kind);
    c->mover = ctrl;
    ctrl->Attach(c);
    c->kind = 0;
    c->f.b.visible = 0;
    c->f.b.active = 1;
    CollidableList()->Add(c);
}

/* Steps the hop towards (x, y), then runs the state: 1, 2 and 6 hop and
 * move on when the hop ends, 3 shows the red crosshair (0x12) before a
 * shot, 5 chases the player, blinking (0x10) as its timer runs out and
 * firing a fast shot when it does, 8 sends the target off the bottom of
 * the level. */
void CortexTargetCtrl::Update(MovingSprite *part)
{
    /* Pinned: the ROM keeps `n` in r5, `part` in r6 and `this` in r7.
     * Unpinned, global allocation ranks `this` first (45 refs) and gives it
     * r6, `n` r7 and `part` r5; no spelling tried (an inline for the step,
     * locals for `n`, the blink flag through a pointer, ...) changed that.
     * The C pinned `part` and `n` and needed 11 more pins and an `asm` for
     * case 5's blink flag and frame clamps, which the C++ doesn't. */
    MATCH_HOLD_REG(s32, n, r5);

    if ((n = stepsLeft) != 0) {
        /* the ROM loads `dx` before `steps`, and stores both coordinates
         * after the second division */
        s32 total;
        s32 t;
        s32 nx, ny;

        stepsLeft = --n;
        t = dx * n;
        total = steps;
        nx = x - t / total;
        ny = y - dy * n / total;
        part->x = nx;
        part->y = ny;
    }

    switch (state) {
    case 0:
        part->f.b.visible = 0;
        SetState(part, 1);
        break;
    case 1:
    case 2:
    case 6:
        if (stepsLeft != 0)
            break;
        goto next;
    case 3:
        nextState = 4;
        SetTargetAnim(part, 0x12);
        SetState(part, 7);
        break;
    case 4:
        FireShot(part, 0);
        SetState(part, 2);
        SetTargetAnim(part, 0xF);
        break;
    case 7:
        if (part->animDone) {
        next:
            SetState(part, nextState);
        }
        break;
    case 5:
        {
            s32 left;

            if (blinking && ++blink > 9) {
                blink = 0;
                ClampFrame(part, part->frame ^ 1);
            }
            if ((left = stepsLeft) != 0)
                break;
            if (--timer == 0) {
                SetState(part, 1);
                FireShot(part, 1);
                break;
            }
            if (timer == gCortexTargetBlinkStartTimes[boss->counter]) {
                gAudioContext->PlaySfx(SFX_CORTEX_TARGET_BLINK, 0x100);
                part->animating = left;
                SetTargetAnim(part, 0x10);
                blinking = 1;
                blink = left;
            }
            if (timer == gCortexTargetBlinkStopTimes[boss->counter]) {
                part->animating = left;
                SetTargetAnim(part, 0x10);
                blinking = left;
                ClampFrame(part, 1);
            }
            SetDest(part, gPlayer->x, gPlayer->y - 0xA00);
            {
                /* the round first, then the table's address */
                s32 round = boss->counter;

                stepsLeft = steps = gCortexTargetChaseSteps[round];
            }
            break;
        }
    case 8:
        nextState = 10;
        SetState(part, 6);
        break;
    case 9:
        SetState(part, 8);
        break;
    case 10:
        break;
    }
}

/* Advances the target's height pattern for the boss's round: 0 - high
 * mirrors the horizontal direction, 1 - alternate high/low, 2 - alternate
 * high/low and flip `top` every second step. The switch is on a copy of
 * the parameter: the ROM runs the last compare (`== 2`) on a copy of the
 * round in another register, as the C's hand-written compare tree did. */
static inline void StepHeight(CortexTargetCtrl *t, s32 pattern)
{
    s32 p = pattern;

    switch (p) {
    case 0:
        t->high = t->dirLeft;
        break;
    case 1:
        t->high ^= 1;
        break;
    case 2:
        if ((t->high ^= 1) == 0)
            t->top ^= 1;
        break;
    }
}

void CortexTargetCtrl::SetState(MovingSprite *part, s32 next)
{
    switch (next) {
    case 8:
        SetDest(part, (u32)INT_TO_Q8(gLevelLayers->layer0->widthPx) >> 1,
                INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x2000);
        break;
    case 1:
        SetPlatformsKind(0);
        part->animating = next;
        SetTargetAnim(part, 0xF);
        dirLeft = next;
        high = next;
        top = 0;
        SetDest(part, INT_TO_Q8(gLevelLayers->layer0->widthPx) - 0x400, 0x9800);
        nextState = 2;
        break;
    case 2:
        {
            s32 x, y;

            nextState = 3;
            x = this->x;
            if (dirLeft) {
                if (x - 0x1800 <= 0x400)
                    dirLeft = 0;
            } else if (x + 0x1C00 >= INT_TO_Q8(gLevelLayers->layer0->widthPx)) {
                nextState = 5;
            }
            StepHeight(this, boss->counter);
            if (dirLeft)
                x -= 0x1800;
            else
                x += 0x1800;
            if (high) {
                u8 t = top; // loaded before the 0x9800

                y = 0x9800;
                if (t)
                    y = 0x3E00;
            } else {
                y = 0x8200;
            }
            SetDest(part, x, y);
            break;
        }
    case 5:
        blinking = 0;
        SetPlatformsKind(1);
        timer = 0x14;
        break;
    }
    SetMode(next);
}

/* Fires a shot from the target's position: a slow one (kind 0, animation
 * 0xE) or a fast one (1, 0x11). */
void CortexTargetCtrl::FireShot(MovingSprite *part, s32 kind)
{
    MovingSprite *c = MovingSprite::Create(0xFFFF, 0, 0, 0);
    CortexShotCtrl *ctrl;

    c->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x27C);
    switch (kind) {
    case 0:
        SetTag(c, 0xE);
        break;
    case 1:
        SetTag(c, 0x11);
        break;
    }
    c->palette = c->GetAnimPaletteSlot();
    ctrl = new CortexShotCtrl(boss);
    ctrl->fast = kind == 1;
    c->mover = ctrl;
    ctrl->Attach(c);
    /* the inline's parameters (two loads of `part`, where the block copy
     * of SpawnCannon makes one) rank `part` above `this`, which the ROM
     * gives r6 and r7 */
    c->SetPos(part->x, part->y);
    c->f.b.visible = 0;
    c->kind = 1;
    c->f.b.active = 1;
    ForegroundList()->Add(c);
    if (kind == 1)
        gAudioContext->PlaySfx(SFX_CORTEX_SHOT_FAST, 0x100);
    else
        gAudioContext->PlaySfx(SFX_CORTEX_SHOT, 0x100);
}

/* While the part's `kind` is 1, the shot hurts the player it touches, or
 * (a fast one) shrinks the first gems it touches and tells the boss; it
 * goes once its animation is done. */
void CortexShotCtrl::Update(MovingSprite *part)
{
    struct aabb a;
    struct aabb b;

    if (part->kind == 1) {
        GetSpriteBodyBox(&a, gPlayer);
        if (a.w == 0) {
            GetSpriteAttackBox(&b, gPlayer);
            a = b;
        }
        GetSpriteAttackBox(&b, part);
        if (AabbOverlaps(&a, &b)) {
            if (gPlayer->dead == 0)
                HitPlayer(gPlayer, EVENT_HIT_CORTEX_SHOT);
            part->kind = 0;
        } else if (fast) {
            s32 n = gCollidableList->count;
            s32 i;

            for (i = 0; i < n; i++) {
                MovingSprite *e = (MovingSprite *)gCollidableList->items[i];
                struct aabb c = e->GetAnimHitbox();

                if (AabbOverlaps(&c, &b)) {
                    boss->SetMode(2);
                    /* through the inline: with a plain `e->kind = 1`,
                     * reload picks other registers for `part` (r8) in
                     * the next store and in MarkGone */
                    e->SetKind(1);
                    part->kind = 0;
                }
            }
        }
    }
    if (part->animDone)
        part->MarkGone();
}

/* Runs the platform's animation up to frame 0x1A, or to frame 10 while its
 * `kind` is 1, and stops it there. */
void CortexBossPlatformMover::Update(MovingSprite *part)
{
    s32 target;

    if (state == 0) {
        ClampFrame(part, 0x1A);
        state = 1;
    }
    target = 0x1A;
    if (part->kind == 1)
        target = 10;
    if (part->frame == target) {
        part->animating = 0;
    } else {
        /* a named 1: the ROM loads it before `animating`'s address, then
         * forms `animDone`'s from that address */
        s32 one = 1;

        part->animating = one;
        if (part->animDone)
            ClampFrame(part, 0);
    }
}

/* Once a fast shot has hit it (`kind` 1), the gem plays its shrink
 * animation (bank 53's 0xC, 0xB or 0xD) and goes when it is done. */
void CortexBossGemCtrl::Update(MovingSprite *part)
{
    switch (state) {
    case 0:
        if (part->kind == 1) {
            part->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x27C);
            switch (kind) {
            case 0:
                SetTargetAnim(part, 0xC);
                break;
            case 1:
                SetTargetAnim(part, 0xB);
                break;
            case 2:
                SetTargetAnim(part, 0xD);
                break;
            }
            part->palette = part->GetAnimPaletteSlot();
            SetMode(1);
        }
        break;
    case 1:
        if (part->animDone)
            part->MarkGone();
        break;
    }
}

CortexBossGemCtrl::~CortexBossGemCtrl()
{
}

CortexBossGemCtrl::CortexBossGemCtrl(s32 kind)
{
    this->kind = kind;
}

CortexBossPlatformMover::~CortexBossPlatformMover()
{
}

/* A still type-6 platform mover. */
CortexBossPlatformMover::CortexBossPlatformMover() : PlatformMover(0, 0, false, false, 6)
{
}

CortexShotCtrl::~CortexShotCtrl()
{
}

CortexShotCtrl::CortexShotCtrl(CortexBossCtrl *boss)
{
    this->boss = boss;
}
