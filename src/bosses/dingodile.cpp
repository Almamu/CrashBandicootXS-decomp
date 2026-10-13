#include "bg_layer.hpp"
#include "boss_ctrl.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "room_exit.hpp"

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
 * last state signals RoomExit::Request once the part falls off the bottom.
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

s32 DingodileCtrl::GetHits()
{
    return counter;
}

/* Close enough to the player (on the side he is facing) to react. */
static inline void Approach(DingodileCtrl *self, MovingSprite *part, s32 d)
{
    if (d <= INT_TO_Q8(32) - 1)
        self->SetState(part, DINGODILE_STATE_FIRE);
}

static inline s32 AtLevelEdge(Sprite::MirrorBits *f, s32 x)
{
    if (f->flipX < 0)
        return x <= INT_TO_Q8(32);
    else
        return x >= LevelRight() - INT_TO_Q8(32);
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

        p->x = x + INT_TO_Q8(6);
        p->y = y;
    } else {
        s32 x = part->x;
        s32 y = part->y;
        MovingSprite *p = shield;

        p->x = x - INT_TO_Q8(6);
        p->y = y;
    }
    GetSpriteBodyBox(&hurt, part);
    if (this->state == DINGODILE_STATE_DAZED && gPlayer->kind == EVENT_ATTACK_SPIN) {
        GetSpriteAttackBox(&box, gPlayer);
        if (AabbW(&box) && AabbOverlaps(&box, &hurt)) {
            counter++;
            SetState(part, DINGODILE_STATE_HIT);
        }
    }

    state = this->state;
    switch (state) {
    case DINGODILE_STATE_START:
        SetState(part, DINGODILE_STATE_WALK);
        step = 0;
        passes = 0;
        part->kind = EVENT_NONE;
        part->f.b.visible = 0;
        part->f.b.vulnerable = 0;
        break;
    case DINGODILE_STATE_WALK:
    case DINGODILE_STATE_WALK_BACK:
        {
            Sprite::MirrorBits *f = &part->mirrorBits;
            s32 x = part->x;

            if (state == DINGODILE_STATE_WALK) {
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
            if (state == DINGODILE_STATE_WALK) {
                s32 lim = 1;
                if (counter > 0)
                    lim = 2;
                if (++passes >= lim) {
                    SetState(part, DINGODILE_STATE_FIRE_TURN);
                    break;
                }
            }
            goto turn;
        }
    case DINGODILE_STATE_TURN:
    case DINGODILE_STATE_TURN_BACK:
        if (part->animDone) {
            /* `prev` is needed: inlined, the code is 4 instructions shorter */
            u32 prev = state;
            s32 n;

            SetState(part, DINGODILE_STATE_WALK);
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

            /* `v` is needed: inlined, the code is 2 instructions shorter */
            s32 v = part->mirrorBits.flipX;

            part->mirrorBits.flipX = v ? 0 : 1;
            if (prev == DINGODILE_STATE_TURN) {
                if (passes == 0) {
                    timer = 0x73;
                    if (counter > 1)
                        nextState = DINGODILE_STATE_SHARK;
                    else
                        nextState = DINGODILE_STATE_WALK;
                    SetState(part, DINGODILE_STATE_WAIT);
                } else {
                    timer = 0x64;
                    nextState = DINGODILE_STATE_WALK;
                    SetState(part, DINGODILE_STATE_WAIT);
                }
                step = 0;
            } else {
                SetState(part, DINGODILE_STATE_WALK_BACK);
            }
        }
        break;
    case DINGODILE_STATE_SHARK:
        if (part->mirrorBits.flipX < 0)
            SpawnShark(0, 0x2D, 0);
        else
            SpawnShark(LayerWidthPlus(0x28), 0x2D, 1);
        goto idle;
    case DINGODILE_STATE_UNK4:
        if (!part->animDone)
            break;
        goto idle;
    case DINGODILE_STATE_FIRE:
    case DINGODILE_STATE_FIRE_TURN:
        if (part->frame == 0x14 && part->stepTimer == 0) {
            if (part->mirrorBits.flipX < 0)
                SpawnShieldOrRocket(DINGODILE_SPAWN_MODE_ROCKET, Q8_TO_INT(part->x) + 6,
                                    Q8_TO_INT(part->y) - 0x32, part);
            else
                SpawnShieldOrRocket(DINGODILE_SPAWN_MODE_ROCKET, Q8_TO_INT(part->x) - 6,
                                    Q8_TO_INT(part->y) - 0x32, part);
        }
        if (!part->animDone)
            break;
        if (this->state == DINGODILE_STATE_FIRE_TURN) {
            part->f.b.vulnerable = 1;
        turn:
            SetState(part, DINGODILE_STATE_TURN);
            break;
        }
        SetState(part, DINGODILE_STATE_WALK);
        /* scoped: the case labels below would cross `n` */
        {
            s32 n = 8;
            CLAMP_INDEX(n, part->bank->anims[part->tag].frameCount);
            part->frame = n;
        }
        break;
    case DINGODILE_STATE_WAIT:
        if (--timer == 0)
            SetState(part, nextState);
        break;
    case DINGODILE_STATE_STUNNED:
        StartMotion(part, 0);
        SetTargetAnim(part, 5);
        SetState(part, DINGODILE_STATE_DAZED);
        break;
    case DINGODILE_STATE_DAZED:
        if (--timer == 0)
            SetState(part, DINGODILE_STATE_RECOVER);
        break;
    case DINGODILE_STATE_RECOVER:
    case DINGODILE_STATE_RECOVER_HIT:
        if (!part->animDone)
            break;
        if (state == DINGODILE_STATE_RECOVER) {
        idle:
            SetState(part, DINGODILE_STATE_WALK);
            break;
        }
        SetState(part, DINGODILE_STATE_RETREAT);
        break;
    case DINGODILE_STATE_HIT:
        if (timer != 0) {
            timer--;
            break;
        }
        if (counter > 2)
            SetState(part, DINGODILE_STATE_DEFEATED);
        if (part->animDone)
            SetState(part, DINGODILE_STATE_RECOVER_HIT);
        break;
    case DINGODILE_STATE_RETREAT:
        {
            s32 lim = INT_TO_Q8(64);
            if (counter > 0)
                lim = INT_TO_Q8(32);
            if (part->mirrorBits.flipX < 0) {
                if (part->x > lim)
                    break;
            } else {
                s32 x = part->x;

                if (x < LevelRight() - lim)
                    break;
            }
            SetState(part, DINGODILE_STATE_TURN_BACK);
            break;
        }
    case DINGODILE_STATE_DEFEATED:
        {
            s32 y = part->y;

            if (y >= LevelBottom() + INT_TO_Q8(32)) {
                StartMotion(part, 0);
                if (gLevelState->HasSuperBodySlam())
                    RoomExit::Request();
                SetState(part, DINGODILE_STATE_GONE);
            }
            break;
        }
    case DINGODILE_STATE_GONE:
        break;
    }
}

/* "Enter state `next`": sets the mode, then plays the matching
 * animation/spawns. */
void DingodileCtrl::SetState(MovingSprite *part, s32 next)
{
    SetMode(next);
    switch (next) {
    case DINGODILE_STATE_DEFEATED:
        if (!gLevelState->HasSuperBodySlam())
            SpawnTurboRunPower(0xFFFF, 0xA0, 0xA9, 0);
        StartMotion(part, 3);
        break;
    case DINGODILE_STATE_RETREAT:
        SpawnShark(gLevelLayers->layer0->widthPx, 0x28, 1);
        SpawnShark(0, 0x46, 0);
        SpawnShark(LayerWidthPlus(0x46), 0x64, 1);
    case DINGODILE_STATE_WALK:
    case DINGODILE_STATE_WALK_BACK:
        SetTargetAnim(part, 0);
        StartMotion(part, 1);
        break;
    case DINGODILE_STATE_TURN:
    case DINGODILE_STATE_TURN_BACK:
        SetTargetAnim(part, 6);
        StartMotion(part, 0);
        break;
    case DINGODILE_STATE_UNK4:
        SetTargetAnim(part, 1);
        StartMotion(part, 0);
        break;
    case DINGODILE_STATE_FIRE_TURN:
        passes = 0;
    case DINGODILE_STATE_FIRE:
        SetTargetAnim(part, 4);
        StartMotion(part, 0);
        break;
    case DINGODILE_STATE_WAIT:
        StartMotion(part, 0);
        break;
    case DINGODILE_STATE_DAZED:
        shield->mover->SetMode(DINGODILE_SHIELD_STATE_BLINK_START_DAZED);
        timer = 0xD2;
        break;
    case DINGODILE_STATE_RECOVER:
    case DINGODILE_STATE_RECOVER_HIT:
        shield->mover->SetMode(DINGODILE_SHIELD_STATE_BLINK_START);
        SetTargetAnim(part, 2);
        break;
    case DINGODILE_STATE_HIT:
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
    const u8 *bits;

    p->f.b.visible = 0;
    p->bank = gSpriteBankSet->Bank(54);
    switch (mode) {
    case DINGODILE_SPAWN_MODE_SHIELD:
        {
            s32 kind = 1;

            p->mirrorBits.gfxMode = kind;
            p->StartAnim(3);
            p->kind = kind;
        }
        /* scoped, as the rocket's below, where `c` has the other type */
        {
            DingodileShieldCtrl *c;

            ctl = c = new DingodileShieldCtrl;
            c->owner = owner;
        }
        shield = p;
        break;
    case DINGODILE_SPAWN_MODE_ROCKET:
        gAudioContext->PlaySfx(SFX_DINGODILE_ROCKET, 0x100);
        p->StartAnim(7);
        p->kind = EVENT_HIT_EXPLOSION;
        /* scoped, as the shield's */
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
    bits = &gEntityFlags->list->params[*gEntityFlags->list->paramOffsets];
    p->mirrorBits.flipX = ((*bits >> ENTITY_PARAMS_BIT_FLIP_X) ^ 1) & 1;
    p->mirrorBits.flipY = TEST_BIT_SHR(*bits, ENTITY_PARAMS_BIT_FLIP_Y);
    p->f.b.active = 1;
    if (mode == DINGODILE_SPAWN_MODE_SHIELD)
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

    p->bank = gSpriteBankSet->Bank(4);
    p->StartAnim(1);
    p->kind = EVENT_HIT_BITE;
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

    if (part->IsOnScreen() && !gPlayer->dead) {
        GetSpriteAttackBox(&a, part);
        GetSpriteBodyBox(&b, gPlayer);
        if (!AabbW(&b))
            b = GetSpriteAttackBox_s(gPlayer);
        if (AabbOverlaps(&b, &a))
            HitPlayer(part);
    }

    switch (state) {
    case DINGODILE_SHIELD_STATE_START:
        {
            /* The blend value is built in a `struct blend_regs` (gfx.h,
             * gBlendRegs' type): BLDCNT/BLDALPHA's word and BLDY's byte.
             * The 8-byte struct is a DImode pseudo, and only its first
             * word is ever set, so the pair is live from the function's
             * entry (the bldy half is never written) and across every
             * call: global-alloc gives it r5:r6, which is why the ROM
             * saves r6 and never uses it, keeps `this` in r7, and builds
             * the chain in r5; the start value, a subreg set of the
             * pair, isn't folded into the ORs by cse or combine.
             * Until #662 round 9 the function held r5 and r6 with two
             * MATCH_HOLD_REGs over the box builders and started the
             * chain from a MATCH_CONST in a pinned r5 (rounds 2-8 had
             * tried the accumulator as every integer type, a u64, a
             * union blend and at function scope). */
            struct blend_regs regs;
            u32 w;

            regs.blend.raw = BLDCNT_TGT1_OBJ;
            regs.blend.raw |= BLDCNT_TGT2_BG0;
            regs.blend.raw |= BLDCNT_TGT2_BG1;
            regs.blend.raw |= BLDCNT_TGT2_BG2;
            regs.blend.raw |= BLDCNT_TGT2_BG3;
            w = regs.blend.raw | BLDCNT_TGT2_OBJ;
            w |= 0x100000;   /* EVA 16 */
            w |= 0x10000000; /* EVB 16 */
            *(vu32 *)REG_ADDR_BLDCNT = w;
        }
        SetMode(DINGODILE_SHIELD_STATE_STEADY);
        break;
    case DINGODILE_SHIELD_STATE_BLINK_START:
        blinksLeft = 2;
        blinkTimer = 0;
        SetMode(DINGODILE_SHIELD_STATE_BLINK);
        break;
    case DINGODILE_SHIELD_STATE_BLINK_START_DAZED:
        blinksLeft = 2;
        blinkTimer = 0;
        SetMode(DINGODILE_SHIELD_STATE_BLINK_DAZED);
        break;
    case DINGODILE_SHIELD_STATE_BLINK:
    case DINGODILE_SHIELD_STATE_BLINK_DAZED:
        if (blinkTimer == 0) {
            blinkTimer = 0x14;
            part->f.b.blink = !part->f.b.blink;
            if (blinksLeft == 0)
                SetMode(DINGODILE_SHIELD_STATE_STEADY);
            blinksLeft--;
        }
        blinkTimer--;
        break;
    case DINGODILE_SHIELD_STATE_STEADY:
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
        if (state != DINGODILE_PROJECTILE_STATE_UNK4 && state != DINGODILE_PROJECTILE_STATE_BREAK) {
            MovingSprite *t = owner;

            /* the vulnerable bit, extracted (as a 1-bit field, g++ tests
             * it with `movs #0x40; ands`) */
            if ((t->f.flags >> ENTITY_BIT_VULNERABLE) & 1) {
                struct aabb hit = t->GetAnimHitbox();

                if (AabbOverlaps(&a, &hit)) {
                    owner->mover->SetMode(DINGODILE_STATE_STUNNED);
                    SetMode(DINGODILE_PROJECTILE_STATE_BREAK);
                    SetTargetAnim(part, 8);
                    gAudioContext->PlaySfx(SFX_UNKNOWN_39, 0x100);
                }
            }
        }
        if (!gPlayer->dead) {
            struct aabb b;

            GetSpriteBodyBox(&b, gPlayer);
            if (!AabbW(&b))
                b = GetSpriteAttackBox_s(gPlayer);
            if (AabbOverlaps(&a, &b)) {
                HitPlayer(part);
                if (state == DINGODILE_PROJECTILE_STATE_FALL) {
                    SetMode(DINGODILE_PROJECTILE_STATE_BREAK);
                    SetTargetAnim(part, 8);
                    gAudioContext->PlaySfx(SFX_UNKNOWN_39, 0x100);
                }
            }
        }
    }

    switch (state) {
    case DINGODILE_PROJECTILE_STATE_LAUNCH:
        StartTargetMotionY(part, &gDingodileRocketRiseMotion);
        SetMode(DINGODILE_PROJECTILE_STATE_RISE);
        break;
    case DINGODILE_PROJECTILE_STATE_RISE:
        if (part->y <= INT_TO_Q8(8)) {
            part->speedY = 0;
            part->rampY.start = 0;
            part->rampY.step = 0;
            part->rampY.target = 0;
            SetTargetAnim(part, 8);
            part->palette = part->GetAnimPaletteSlot();
            SpawnStalactite(Q8_TO_INT(part->x), Q8_TO_INT(part->y));
            SetMode(DINGODILE_PROJECTILE_STATE_SPENT);
        }
        break;
    case DINGODILE_PROJECTILE_STATE_DROP:
        SetTargetAnim(part, 9);
        StartTargetMotionY(part, &gDingodileStalactiteFallMotion[0]);
        SetMode(DINGODILE_PROJECTILE_STATE_FALL);
        break;
    case DINGODILE_PROJECTILE_STATE_FALL:
    case DINGODILE_PROJECTILE_STATE_UNK4:
        {
            /* `y` is needed: inlined, the instructions are reordered */
            s32 y = part->y;

            if (y >= LevelBottom() - INT_TO_Q8(32)) {
                SetMode(DINGODILE_PROJECTILE_STATE_BREAK);
                SetTargetAnim(part, 8);
                gAudioContext->PlaySfx(SFX_UNKNOWN_39, 0x100);
            }
            break;
        }
    case DINGODILE_PROJECTILE_STATE_BREAK:
        part->speedY = 0;
        part->rampY.start = 0;
        part->rampY.step = 0;
        part->rampY.target = 0;
    case DINGODILE_PROJECTILE_STATE_SPENT:
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
    p->bank = gSpriteBankSet->Bank(54);
    p->StartAnim(8);
    p->kind = EVENT_HIT;
    c = new DingodileProjectileCtrl(this);
    c->SetMode(DINGODILE_PROJECTILE_STATE_DROP);
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
    case DINGODILE_SHARK_STATE_START:
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
    case DINGODILE_SHARK_STATE_UNK1:
        if (part->mirrorBits.flipX < 0) {
            if (part->x + INT_TO_Q8(40) <= 0)
                part->MarkGone();
        } else {
            /* `x` is needed: inlined, the code is 9 instructions shorter */
            s32 x = part->x;

            if (x >= LevelRight() + INT_TO_Q8(40))
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
