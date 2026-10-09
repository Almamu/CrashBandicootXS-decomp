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
#include "crates.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "math_util.h"
}

/* Dingodile's fight (GitHub issue #24), ROM 0x080197F4-0x0801A794,
 * formerly asm/code_3_2_17_188d0_1967c.s. Built with old_agbcp (Makefile
 * OLD_AGBCC_OBJS; docs/matching/archive/issue-24-boss-actor.md). Until
 * #769 it started with the end of the Neo Cortex fight's controllers
 * (cortex.cpp since).
 *
 * The small boss controllers of include/boss_ctrl.hpp, in ROM order:
 * Dingodile (DingodileCtrl), his shield (DingodileShieldCtrl), rocket and
 * stalactite (DingodileProjectileCtrl) and shark (DingodileSharkCtrl).
 *
 * Dingodile walks his part along the level (the approach tables
 * gDingodileStopXLeft/Right and their Hurt variants), turning round at
 * either level edge, counts the hits the player lands on him, and spawns
 * helper parts through SpawnShieldOrRocket/SpawnShark; SetState is his
 * "enter state N" transition (animation + follow-up spawns/sounds). The
 * last state signals RequestRoomExit once the part falls off the bottom.
 * His helpers use sprite bank 54: the purple energy ring of anim 3
 * (SpawnShieldOrRocket mode 0, kept 6 px in front of him, alpha-blended
 * and hurting the player on contact), the rocket of anim 7 (mode 1,
 * SFX_DINGODILE_ROCKET) that flies up and, at the top, drops the
 * stalactite of anims 8/9 (SpawnStalactite) that hurts him if it lands
 * on him, and a bank-4 shark (SpawnShark) that crosses the level.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: DingodileCtrl::GetHits. Matched
 * anyway. */

/* Right edge of the level, in Q8 units. */
static inline s32 LevelRight(void)
{
    return INT_TO_Q8(gLevelLayers->layer0->widthPx);
}

/* Bottom edge of the level, in Q8 units. */
static inline s32 LevelBottom(void)
{
    return INT_TO_Q8(gLevelLayers->layer0->heightPx);
}

/* (u16)(width + n), computed the way the ROM does it: in the upper
 * halfword, then shifted back down. */
#define LayerWidthPlus(n) (((gLevelLayers->layer0->widthPx << 16) + ((n) << 16)) >> 16)

/* The player's HandleEvent (Player, include/player.hpp). */
static inline void HitPlayer(MovingSprite *by)
{
    gPlayer->HandleEvent(0, by->kind, 0);
}

/* GetSpriteAttackBox (objects.h) as a struct return: assigned to a box
 * that already holds the body box, it fills a temporary and copies it
 * (the ROM's frame and its ldmia/stmia copy). */
extern "C" struct aabb GetSpriteAttackBox_s(void *part) asm("GetSpriteAttackBox");

static inline void SetTag(MovingSprite *p, u8 tag)
{
    p->tag = tag;
}

s32 DingodileCtrl::GetHits()
{
    return counter;
}

/* Close enough to the player (on the side he is facing) to react. */
static inline void Approach(DingodileCtrl *self, MovingSprite *part, s32 d)
{
    if (d <= 0x1FFF)
        self->SetState(part, 5);
}

static inline s32 AtLevelEdge(Sprite::MirrorBits *f, s32 x)
{
    if (f->flipX < 0)
        return x <= 0x2000;
    else
        return x >= LevelRight() - 0x2000;
}

void DingodileCtrl::Update(MovingSprite *part)
{
    struct aabb hurt;
    struct aabb box;
    u32 state;

    if (part->mirrorBits.flipX < 0) {
        s32 x = part->x;
        s32 y = part->y;
        MovingSprite *p = shield;

        p->x = x + 0x600;
        p->y = y;
    } else {
        s32 x = part->x;
        s32 y = part->y;
        MovingSprite *p = shield;

        p->x = x - 0x600;
        p->y = y;
    }
    GetSpriteBodyBox(&hurt, part);
    if (this->state == 8 && gPlayer->kind == 0x13) {
        GetSpriteAttackBox(&box, gPlayer);
        if (AABB_VALID(box) && AabbOverlaps(&box, &hurt)) {
            counter++;
            SetState(part, 11);
        }
    }

    state = this->state;
    switch (state) {
    case 0:
        SetState(part, 1);
        step = 0;
        passes = 0;
        part->kind = 0;
        part->f.b.visible = 0;
        part->f.b.vulnerable = 0;
        break;
    case 1:
    case 14:
        {
            Sprite::MirrorBits *f = &part->mirrorBits;
            s32 x = part->x;

            if (state == 1) {
                if (f->flipX < 0) {
                    const s32 *tbl = gDingodileStopXLeft;
                    if (counter > 0)
                        tbl = gDingodileStopXLeftHurt;
                    if (x <= tbl[step]) {
                        Approach(this, part, gPlayer->x - x);
                        step++;
                        break;
                    }
                } else {
                    const s32 *tbl = gDingodileStopXRight;
                    if (counter > 0)
                        tbl = gDingodileStopXRightHurt;
                    if (x >= tbl[step]) {
                        Approach(this, part, x - gPlayer->x);
                        step++;
                        break;
                    }
                }
            }
            if (!AtLevelEdge(f, x))
                break;
            if (state == 1) {
                s32 lim = 1;
                if (counter > 0)
                    lim = 2;
                if (++passes >= lim) {
                    SetState(part, 6);
                    break;
                }
            }
            goto turn;
        }
    case 3:
    case 13:
        if (part->animDone) {
            u32 prev = state;
            s32 n;

            SetState(part, 1);
            if (part->mirrorBits.flipX < 0) {
                s32 x = Q8_TO_INT(part->x);
                s32 y = Q8_TO_INT(part->y);

                x += 6;
                part->x = INT_TO_Q8(x);
                part->y = INT_TO_Q8(y);
            } else {
                s32 x = Q8_TO_INT(part->x);
                s32 y = Q8_TO_INT(part->y);

                x -= 6;
                part->x = INT_TO_Q8(x);
                part->y = INT_TO_Q8(y);
            }
            n = 8;
            CLAMP_INDEX(n, part->bank->anims[part->tag].frameCount);
            part->frame = n;
            {
                s32 v = part->mirrorBits.flipX;

                part->mirrorBits.flipX = v ? 0 : 1;
            }
            if (prev == 3) {
                if (passes == 0) {
                    timer = 0x73;
                    if (counter > 1)
                        nextState = 15;
                    else
                        nextState = 1;
                    SetState(part, 2);
                } else {
                    timer = 0x64;
                    nextState = 1;
                    SetState(part, 2);
                }
                step = 0;
            } else {
                SetState(part, 14);
            }
        }
        break;
    case 15:
        if (part->mirrorBits.flipX < 0)
            SpawnShark(0, 0x2D, 0);
        else
            SpawnShark(LayerWidthPlus(0x28), 0x2D, 1);
        goto idle;
    case 4:
        if (!part->animDone)
            break;
        goto idle;
    case 5:
    case 6:
        if (part->frame == 0x14 && part->stepTimer == 0) {
            if (part->mirrorBits.flipX < 0)
                SpawnShieldOrRocket(1, Q8_TO_INT(part->x) + 6, Q8_TO_INT(part->y) - 0x32, part);
            else
                SpawnShieldOrRocket(1, Q8_TO_INT(part->x) - 6, Q8_TO_INT(part->y) - 0x32, part);
        }
        if (!part->animDone)
            break;
        if (this->state == 6) {
            part->f.b.vulnerable = 1;
        turn:
            SetState(part, 3);
            break;
        }
        SetState(part, 1);
        {
            s32 n = 8;
            CLAMP_INDEX(n, part->bank->anims[part->tag].frameCount);
            part->frame = n;
        }
        break;
    case 2:
        if (--timer == 0)
            SetState(part, nextState);
        break;
    case 7:
        StartMotion(part, 0);
        SetTargetAnim(part, 5);
        SetState(part, 8);
        break;
    case 8:
        if (--timer == 0)
            SetState(part, 9);
        break;
    case 9:
    case 10:
        if (!part->animDone)
            break;
        if (state == 9) {
        idle:
            SetState(part, 1);
            break;
        }
        SetState(part, 12);
        break;
    case 11:
        if (timer != 0) {
            timer--;
            break;
        }
        if (counter > 2)
            SetState(part, 16);
        if (part->animDone)
            SetState(part, 10);
        break;
    case 12:
        {
            s32 lim = 0x4000;
            if (counter > 0)
                lim = 0x2000;
            if (part->mirrorBits.flipX < 0) {
                if (part->x > lim)
                    break;
            } else {
                s32 x = part->x;

                if (x < LevelRight() - lim)
                    break;
            }
            SetState(part, 13);
            break;
        }
    case 16:
        {
            s32 y = part->y;

            if (y >= LevelBottom() + 0x2000) {
                StartMotion(part, 0);
                if ((u8)gLevelState->HasSuperBodySlam())
                    RequestRoomExit();
                SetState(part, 17);
            }
            break;
        }
    case 17:
        break;
    }
}

/* "Enter state `next`": sets the mode, then plays the matching
 * animation/spawns. */
void DingodileCtrl::SetState(MovingSprite *part, s32 next)
{
    SetMode(next);
    switch (next) {
    case 16:
        if (!(u8)gLevelState->HasSuperBodySlam())
            SpawnTurboRunPower(0xFFFF, 0xA0, 0xA9, 0);
        StartMotion(part, 3);
        break;
    case 12:
        SpawnShark(gLevelLayers->layer0->widthPx, 0x28, 1);
        SpawnShark(0, 0x46, 0);
        SpawnShark(LayerWidthPlus(0x46), 0x64, 1);
    case 1:
    case 14:
        SetTargetAnim(part, 0);
        StartMotion(part, 1);
        break;
    case 3:
    case 13:
        SetTargetAnim(part, 6);
        StartMotion(part, 0);
        break;
    case 4:
        SetTargetAnim(part, 1);
        StartMotion(part, 0);
        break;
    case 6:
        passes = 0;
    case 5:
        SetTargetAnim(part, 4);
        StartMotion(part, 0);
        break;
    case 2:
        StartMotion(part, 0);
        break;
    case 8:
        shield->mover->SetMode(2);
        timer = 0xD2;
        break;
    case 9:
    case 10:
        shield->mover->SetMode(1);
        SetTargetAnim(part, 2);
        break;
    case 11:
        timer = 0x64;
        if (counter > 2)
            timer = 1;
        gAudioContext->PlaySfx(SFX_BOSS_HIT, 0x100);
        part->f.b.visible = 0;
        StartMotion(part, 2);
        SetTargetAnim(part, 1);
        break;
    }
}

void DingodileCtrl::SpawnShieldOrRocket(s32 mode, u16 x, u16 y, MovingSprite *owner)
{
    MovingSprite *p = MovingSprite::Create(0xFFFF, x, y, 0);
    Ctrl *ctl;
    u8 *bits;

    p->f.b.visible = 0;
    p->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x288);
    switch (mode) {
    case 0:
        {
            s32 kind = 1;

            p->mirrorBits.gfxMode = kind;
            SetTag(p, 3);
            p->ResetFrameTimer();
            p->ResetFrameIndex();
            p->SetAnimDone(0);
            p->kind = kind;
        }
        {
            DingodileShieldCtrl *c;

            ctl = c = new DingodileShieldCtrl;
            c->owner = owner;
        }
        shield = p;
        break;
    case 1:
        gAudioContext->PlaySfx(SFX_DINGODILE_ROCKET, 0x100);
        SetTag(p, 7);
        p->ResetFrameTimer();
        p->ResetFrameIndex();
        p->SetAnimDone(0);
        p->kind = 4;
        {
            DingodileProjectileCtrl *c;

            ctl = c = new DingodileProjectileCtrl;
            c->owner = owner;
        }
        break;
    default:
        ctl = 0;
        break;
    }
    p->palette = p->GetAnimPaletteSlot();
    p->mover = ctl;
    ctl->Attach(p);
    bits = &((u8 *)gEntityFlags->list->params)[*gEntityFlags->list->paramOffsets];
    p->mirrorBits.flipX = ((*bits >> 1) ^ 1) & 1;
    p->mirrorBits.flipY = (*bits >> 2) & 1;
    p->f.b.active = 1;
    if (mode == 0)
        TouchableList()->Add(p);
    else
        CollidableList()->Add(p);
}

/* Spawns a shark (sprite bank 4's anim 1, `kind` 6) at (x, y), facing
 * `facing`, and registers it with gCollidableList. */
void DingodileCtrl::SpawnShark(u16 x, u16 y, u8 facing)
{
    MovingSprite *p = MovingSprite::Create(0xFFFF, x, y, 0);
    Ctrl *ctl;

    p->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x30);
    SetTag(p, 1);
    p->ResetFrameTimer();
    p->ResetFrameIndex();
    p->SetAnimDone(0);
    p->kind = 6;
    ctl = new DingodileSharkCtrl;
    p->palette = p->GetAnimPaletteSlot();
    p->mover = ctl;
    ctl->Attach(p);
    p->mirrorBits.flipX = facing;
    p->f.b.active = 1;
    ctl->Attach(p);
    CollidableList()->Add(p);
}

/* The shield's update (`part` is the shield): while the player isn't
 * dead and the shield is on screen, it hurts him on contact (with his
 * body box, or his attack box when he has none). Then: state 0 turns the
 * alpha blending on (1st target OBJ, 2nd target BG0-3 and OBJ, EVA = EVB
 * = 16) and moves to state 5; states 1/2 arm a two-blink countdown and
 * move to 3/4; states 3/4 toggle the part's blink bit every 20 frames
 * until the blinks run out (then state 5). */
void DingodileShieldCtrl::Update(MovingSprite *part)
{
    struct aabb a;
    struct aabb b;
    /* Kept from the C (they are register allocation, the same under
     * agbcp): the ROM leaves r4-r6 free and keeps the long-lived values
     * in r7-r10 (`this`, `&b`, `part`, &gPlayer). Holding r5 and r6
     * across the box builders makes the allocator skip them; without
     * the holds it starts at r5. #662 round 3: the ROM pushes r6 and
     * never uses it, and the greg dumps of the plain function show no
     * spill of r4-r6 (reload never takes them away from the long-lived
     * pseudos), so the ROM's allocation had pseudos in r5/r6 over the
     * box builders whose code is gone from the output; nothing tried
     * gives them. No -f flag or pair of flags helps. */
    MATCH_HOLD_REG(s32, hr5, r5);
    MATCH_HOLD_REG(s32, hr6, r6);

    if (part->IsOnScreen() && !gPlayer->dead) {
        MATCH_HOLD(hr5);
        MATCH_HOLD(hr6);
        GetSpriteAttackBox(&a, part);
        GetSpriteBodyBox(&b, gPlayer);
        if (!AABB_VALID(b))
            b = GetSpriteAttackBox_s(gPlayer);
        /* end of the hold */
        MATCH_USE(hr5);
        MATCH_USE(hr6);
        if (AabbOverlaps(&b, &a))
            HitPlayer(part);
    }

    switch (state) {
    case 0:
        {
            /* Kept from the C: the ROM builds the value with an `orrs`
             * chain in r5, which a constant expression folds into one
             * load (a constant-init, the same under agbcp). #662 round 2:
             * `acc` initialized at the top of the function keeps the
             * chain unfolded, but builds it in r0/r1 (and without the
             * holds above moves everything else). #662 round 3: with
             * `acc = BLDCNT_TGT1_OBJ` cse1 knows each `|=` operand and
             * folds the whole chain into one constant, so the chain needs
             * a starting value cse can't see. */
            MATCH_HOLD_REG(u32, acc, r5);
            u32 w;

            MATCH_CONST(acc, BLDCNT_TGT1_OBJ);
            acc |= BLDCNT_TGT2_BG0;
            acc |= BLDCNT_TGT2_BG1;
            acc |= BLDCNT_TGT2_BG2;
            acc |= BLDCNT_TGT2_BG3;
            w = acc | BLDCNT_TGT2_OBJ;
            w |= 0x100000;
            w |= 0x10000000;
            *(vu32 *)REG_ADDR_BLDCNT = w;
        }
        SetMode(5);
        break;
    case 1:
        blinksLeft = 2;
        blinkTimer = 0;
        SetMode(3);
        break;
    case 2:
        blinksLeft = 2;
        blinkTimer = 0;
        SetMode(4);
        break;
    case 3:
    case 4:
        if (blinkTimer == 0) {
            blinkTimer = 0x14;
            part->f.b.blink = !part->f.b.blink;
            if (blinksLeft == 0)
                SetMode(5);
            blinksLeft--;
        }
        blinkTimer--;
        break;
    case 5:
        break;
    }
}

/* The rocket's and the stalactite's update. The rocket (states 0-2)
 * rises, and at the top turns into the stalactite's spawner; the
 * stalactite (states 5, 3/4) falls. Either one breaks (state 6) when it
 * hits the player, the stalactite also when it hits Dingodile while he
 * is vulnerable, or the floor. */
void DingodileProjectileCtrl::Update(MovingSprite *part)
{
    struct aabb a;

    GetSpriteAttackBox(&a, part);
    if (a.w) {
        if (state != 4 && state != 6) {
            MovingSprite *t = owner;

            /* the vulnerable bit, extracted (as a 1-bit field, g++ tests
             * it with `movs #0x40; ands`) */
            if ((t->f.flags >> 6) & 1) {
                struct aabb hit = t->GetAnimHitbox();

                if (AabbOverlaps(&a, &hit)) {
                    owner->mover->SetMode(7);
                    SetMode(6);
                    SetTargetAnim(part, 8);
                    gAudioContext->PlaySfx(SFX_UNKNOWN_39, 0x100);
                }
            }
        }
        if (!gPlayer->dead) {
            struct aabb b;

            GetSpriteBodyBox(&b, gPlayer);
            if (!AABB_VALID(b))
                b = GetSpriteAttackBox_s(gPlayer);
            if (AabbOverlaps(&a, &b)) {
                HitPlayer(part);
                if (state == 3) {
                    SetMode(6);
                    SetTargetAnim(part, 8);
                    gAudioContext->PlaySfx(SFX_UNKNOWN_39, 0x100);
                }
            }
        }
    }

    switch (state) {
    case 0:
        StartTargetMotionY(part, (const struct speed_ramp *)gDingodileRocketRiseMotion);
        SetMode(1);
        break;
    case 1:
        if (part->y <= 0x800) {
            part->speedY = 0;
            part->rampY.start = 0;
            part->rampY.step = 0;
            part->rampY.target = 0;
            SetTargetAnim(part, 8);
            part->palette = part->GetAnimPaletteSlot();
            SpawnStalactite(Q8_TO_INT(part->x), Q8_TO_INT(part->y));
            SetMode(2);
        }
        break;
    case 5:
        SetTargetAnim(part, 9);
        StartTargetMotionY(part, (const struct speed_ramp *)gDingodileStalactiteFallMotion);
        SetMode(3);
        break;
    case 3:
    case 4:
        {
            s32 y = part->y;

            if (y >= LevelBottom() - 0x2000) {
                SetMode(6);
                SetTargetAnim(part, 8);
                gAudioContext->PlaySfx(SFX_UNKNOWN_39, 0x100);
            }
            break;
        }
    case 6:
        part->speedY = 0;
        part->rampY.start = 0;
        part->rampY.step = 0;
        part->rampY.target = 0;
    case 2:
        if (part->animDone)
            part->MarkGone();
        break;
    }
}

/* Spawns the stalactite (anim 8, then 9 as it falls) at (x, y). */
void DingodileProjectileCtrl::SpawnStalactite(u16 x, u16 y)
{
    MovingSprite *p = MovingSprite::Create(0xFFFF, x, y, 0);
    DingodileProjectileCtrl *c;

    p->f.b.visible = 0;
    p->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + 0x288);
    SetTag(p, 8);
    p->ResetFrameTimer();
    p->ResetFrameIndex();
    p->SetAnimDone(0);
    p->kind = 1;
    c = new DingodileProjectileCtrl(this);
    c->SetMode(5);
    p->palette = p->GetAnimPaletteSlot();
    p->mover = c;
    c->Attach(p);
    p->f.b.active = 1;
    CollidableList()->Add(p);
}

/* The shark's update: state 0 starts it across the level (motion record
 * 3, negated when it faces left), state 1 removes it once it is past the
 * level's edge. */
void DingodileSharkCtrl::Update(MovingSprite *part)
{
    switch (Ctrl::state) {
    case 0:
        if (part->mirrorBits.flipX < 0) {
            s32 a = -gDingodileMotionRecords[3].start;
            s32 c = -gDingodileMotionRecords[3].target;
            s32 b = gDingodileMotionRecords[3].step;
            part->speedX = a;
            part->rampX.start = a;
            part->rampX.step = b;
            part->rampX.target = c;
        } else {
            s32 a = gDingodileMotionRecords[3].start;
            s32 b = gDingodileMotionRecords[3].step;
            s32 c = gDingodileMotionRecords[3].target;
            part->speedX = a;
            part->rampX.start = a;
            part->rampX.step = b;
            part->rampX.target = c;
        }
        break;
    case 1:
        if (part->mirrorBits.flipX < 0) {
            if (part->x + 0x2800 <= 0)
                part->MarkGone();
        } else {
            s32 x = part->x;

            if (x >= LevelRight() + 0x2800)
                part->MarkGone();
        }
        break;
    }
}

DingodileSharkCtrl::DingodileSharkCtrl()
{
}

DingodileSharkCtrl::~DingodileSharkCtrl()
{
}

DingodileProjectileCtrl::~DingodileProjectileCtrl()
{
    owner = 0;
}

DingodileProjectileCtrl::DingodileProjectileCtrl()
{
}

DingodileShieldCtrl::~DingodileShieldCtrl()
{
}
