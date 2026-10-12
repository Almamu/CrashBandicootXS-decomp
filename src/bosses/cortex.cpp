#include "bg_layer.hpp"
#include "boss_ctrl.hpp"
#include "platform.hpp"
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

/* The Neo Cortex fight (GitHub issues #23 and #24), ROM
 * 0x08018A30-0x080197F4, formerly asm/code_3_2_17_188d0.s and the head of
 * asm/code_3_2_17_188d0_1967c.s (details in
 * docs/matching/archive/issue-23-graphics.md and issue-24-boss-actor.md).
 * Built with old_agbcp (Makefile OLD_AGBCC_OBJS). Until #769 its start
 * was the end of cortex.cpp and its end the head of dingodile.cpp; each
 * class's methods end with its destructor and constructor, so the
 * controllers' boundaries put these functions in one original unit.
 *
 * Small controllers (include/boss_ctrl.hpp, include/platform.hpp), in ROM
 * order, for the fight in sprite bank 53 (the Tesla cannon at three
 * angles in anims 3-5, the green/red crosshairs in 0xF/0x10/0x12, the
 * shots in 0xE/0x11 and the shrinking gems in 9-0xD): the boss controller
 * (CortexBossCtrl) spawns the cannon and the target (crosshair), the
 * target (CortexTargetCtrl) hops across the level and then chases the
 * player, firing a shot (CortexShotCtrl) at each stop. In this fight the
 * red/green/yellow gem spawners hand over to SpawnCortexBossGem (bank
 * 32's gems, CortexBossGemCtrl), which a fast shot shrinks away, and the
 * level's type-6 platforms get a CortexBossPlatformMover. Then the
 * target's SetPlatformsKind, SetDest, destructor and constructor, the
 * cannon (CortexCannonCtrl) and the boss controller's SetState,
 * destructor and constructor.
 *
 * UNUSED - no `bl`/`.4byte` reference in asm/, data/ or src/, and no
 * Thumb pointer anywhere in the ROM: CortexCannonCtrl::SetState. Matched
 * anyway. */

/* The player's HandleEvent (Player, include/player.hpp). */
static inline void HitPlayer(Player *pl, s32 event)
{
    pl->HandleEvent(0, event, 0);
}

/* Sets `frame` to `idx`, clamped to the animation's last step. */
static inline void ClampFrame(MovingSprite *p, s32 idx)
{
    CLAMP_INDEX(idx, p->bank->anims[p->tag].frameCount);
    p->frame = idx;
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
    case CORTEX_BOSS_STATE_START:
        SpawnCannon(part);
        SpawnTarget(part);
        part->f.b.visible = 0;
        goto mode1;
    case CORTEX_BOSS_STATE_AIM:
        {
            s32 y = target->y;
            s32 n;

            if (y <= 0x5000)
                cannon->StartAnim(5);
            else if (y <= 0x7800)
                cannon->StartAnim(4);
            else
                cannon->StartAnim(3);

            n = target->x - part->x;
            part->mirrorFlags.mirrorX = n >= 0;
            cannon->mirrorFlags.mirrorX = n >= 0;

            s32 w = INT_TO_Q8(gLevelLayers->layer0->widthPx);

            n = (u32)(ABS_BRANCHLESS(n) * 12) / w;
            LIMIT_MAX(n, 5);
            n = 5 - n;
            ClampFrame(part, n);
            ClampFrame(cannon, n);
            break;
        }
    case CORTEX_BOSS_STATE_HIT:
        if (++counter > 2) {
            SetState(part, CORTEX_BOSS_STATE_DEFEATED);
            break;
        }
    mode1:
        SetState(part, CORTEX_BOSS_STATE_AIM);
        break;
    case CORTEX_BOSS_STATE_GONE:
        break;
    case CORTEX_BOSS_STATE_DEFEATED:
        cannon->y += 0x80;
        part->y += 0x80;
        if (part->y >= INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x4000) {
            if (gLevelState->HasTurboRun())
                RequestRoomExit();
            SetState(part, CORTEX_BOSS_STATE_GONE);
        }
        break;
    }
}

/* The cannon: animation 3 of bank 53, still, facing as `part` does. */
void CortexBossCtrl::SpawnCannon(MovingSprite *part)
{
    MovingSprite *c = MovingSprite::Create(0xFFFF, 0, 0, 0);
    Ctrl *ctrl;

    c->bank = gSpriteBankSet->Bank(53);
    c->StartAnim(3);
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

    c->bank = gSpriteBankSet->Bank(53);
    c->StartAnim(0xF);
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
void SpawnCortexBossGem(u32 id, u16 x, u16 y, u16 param, s32 kind)
{
    MovingSprite *c = MovingSprite::Create(id, x, y, param);
    Ctrl *ctrl;

    c->bank = gSpriteBankSet->Bank(32);
    switch (kind) {
    case CORTEX_BOSS_GEM_KIND_RED:
        c->StartAnim(3);
        break;
    case CORTEX_BOSS_GEM_KIND_GREEN:
        c->StartAnim(2);
        break;
    case CORTEX_BOSS_GEM_KIND_YELLOW:
        c->StartAnim(0);
        break;
    }
    c->palette = c->GetAnimPaletteSlot();
    ctrl = new CortexBossGemCtrl(kind);
    c->mover = ctrl;
    ctrl->Attach(c);
    c->kind = EVENT_NONE;
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
    /* `n` is the step block's own local: the ROM keeps it in r5, `part`
     * in r6 and `this` in r7. Declared for the whole function (as the C
     * had it), global allocation ranks `this` first (45 refs) and gives
     * it r6, `n` r7 and `part` r5, which took an r5 pin on `n` (#662
     * round 2). The C also pinned `part` and needed 11 more pins and an
     * `asm` for case 5's blink flag and frame clamps, which the C++
     * doesn't. */
    if (stepsLeft != 0) {
        /* the ROM loads `dx` before `steps`, and stores both coordinates
         * after the second division */
        s32 n;
        s32 total;
        s32 t;
        s32 nx, ny;

        n = --stepsLeft;
        t = dx * n;
        total = steps;
        nx = x - t / total;
        ny = y - dy * n / total;
        part->x = nx;
        part->y = ny;
    }

    switch (state) {
    case CORTEX_TARGET_STATE_START:
        part->f.b.visible = 0;
        SetState(part, CORTEX_TARGET_STATE_ENTER);
        break;
    case CORTEX_TARGET_STATE_ENTER:
    case CORTEX_TARGET_STATE_HOP:
    case CORTEX_TARGET_STATE_HOP_OFF:
        if (stepsLeft != 0)
            break;
        goto next;
    case CORTEX_TARGET_STATE_AIM:
        nextState = CORTEX_TARGET_STATE_FIRE;
        SetTargetAnim(part, 0x12);
        SetState(part, CORTEX_TARGET_STATE_ANIM);
        break;
    case CORTEX_TARGET_STATE_FIRE:
        FireShot(part, CORTEX_SHOT_KIND_SLOW);
        SetState(part, CORTEX_TARGET_STATE_HOP);
        SetTargetAnim(part, 0xF);
        break;
    case CORTEX_TARGET_STATE_ANIM:
        if (part->animDone) {
        next:
            SetState(part, nextState);
        }
        break;
    case CORTEX_TARGET_STATE_CHASE:
        {
            s32 left;

            if (blinking && ++blink > 9) {
                blink = 0;
                ClampFrame(part, part->frame ^ 1);
            }
            if ((left = stepsLeft) != 0)
                break;
            if (--timer == 0) {
                SetState(part, CORTEX_TARGET_STATE_ENTER);
                FireShot(part, CORTEX_SHOT_KIND_FAST);
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

            /* the round first, then the table's address */
            s32 round = boss->counter;

            stepsLeft = steps = gCortexTargetChaseSteps[round];
            break;
        }
    case CORTEX_TARGET_STATE_LEAVE:
        nextState = CORTEX_TARGET_STATE_GONE;
        SetState(part, CORTEX_TARGET_STATE_HOP_OFF);
        break;
    case CORTEX_TARGET_STATE_STOP:
        SetState(part, CORTEX_TARGET_STATE_LEAVE);
        break;
    case CORTEX_TARGET_STATE_GONE:
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
    case CORTEX_TARGET_STATE_LEAVE:
        SetDest(part, (u32)INT_TO_Q8(gLevelLayers->layer0->widthPx) >> 1,
                INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x2000);
        break;
    case CORTEX_TARGET_STATE_ENTER:
        SetPlatformsKind(0);
        part->animating = next;
        SetTargetAnim(part, 0xF);
        dirLeft = next;
        high = next;
        top = 0;
        SetDest(part, INT_TO_Q8(gLevelLayers->layer0->widthPx) - 0x400, 0x9800);
        nextState = CORTEX_TARGET_STATE_HOP;
        break;
    case CORTEX_TARGET_STATE_HOP:
        {
            s32 x, y;

            nextState = CORTEX_TARGET_STATE_AIM;
            x = this->x;
            if (dirLeft) {
                if (x - 0x1800 <= 0x400)
                    dirLeft = 0;
            } else if (x + 0x1C00 >= INT_TO_Q8(gLevelLayers->layer0->widthPx)) {
                nextState = CORTEX_TARGET_STATE_CHASE;
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
    case CORTEX_TARGET_STATE_CHASE:
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

    c->bank = gSpriteBankSet->Bank(53);
    switch (kind) {
    case CORTEX_SHOT_KIND_SLOW:
        c->StartAnim(0xE);
        break;
    case CORTEX_SHOT_KIND_FAST:
        c->StartAnim(0x11);
        break;
    }
    c->palette = c->GetAnimPaletteSlot();
    ctrl = new CortexShotCtrl(boss);
    ctrl->fast = kind == CORTEX_SHOT_KIND_FAST;
    c->mover = ctrl;
    ctrl->Attach(c);
    /* the inline's parameters (two loads of `part`, where the block copy
     * of SpawnCannon makes one) rank `part` above `this`, which the ROM
     * gives r6 and r7 */
    c->SetPos(part->x, part->y);
    c->f.b.visible = 0;
    c->kind = EVENT_HIT;
    c->f.b.active = 1;
    ForegroundList()->Add(c);
    if (kind == CORTEX_SHOT_KIND_FAST)
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

    if (part->kind == EVENT_HIT) {
        GetSpriteBodyBox(&a, gPlayer);
        if (a.w == 0) {
            GetSpriteAttackBox(&b, gPlayer);
            a = b;
        }
        GetSpriteAttackBox(&b, part);
        if (AabbOverlaps(&a, &b)) {
            if (gPlayer->dead == 0)
                HitPlayer(gPlayer, EVENT_HIT_CORTEX_SHOT);
            part->kind = EVENT_NONE;
        } else if (fast) {
            s32 n = gCollidableList->count;
            s32 i;

            for (i = 0; i < n; i++) {
                MovingSprite *e = gCollidableList->items[i]->AsMovingSprite();
                struct aabb c = e->GetAnimHitbox();

                if (AabbOverlaps(&c, &b)) {
                    boss->SetMode(CORTEX_BOSS_STATE_HIT);
                    /* through the inline: with a plain `e->kind = EVENT_HIT`,
                     * reload picks other registers for `part` (r8) in
                     * the next store and in MarkGone */
                    e->SetKind(EVENT_HIT);
                    part->kind = EVENT_NONE;
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

    if (state == CORTEX_BOSS_PLATFORM_STATE_START) {
        ClampFrame(part, 0x1A);
        state = CORTEX_BOSS_PLATFORM_STATE_RUN;
    }
    target = 0x1A;
    if (part->kind == EVENT_HIT)
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
    case CORTEX_BOSS_GEM_STATE_WAIT:
        if (part->kind == EVENT_HIT) {
            part->bank = gSpriteBankSet->Bank(53);
            switch (kind) {
            case CORTEX_BOSS_GEM_KIND_RED:
                SetTargetAnim(part, 0xC);
                break;
            case CORTEX_BOSS_GEM_KIND_GREEN:
                SetTargetAnim(part, 0xB);
                break;
            case CORTEX_BOSS_GEM_KIND_YELLOW:
                SetTargetAnim(part, 0xD);
                break;
            }
            part->palette = part->GetAnimPaletteSlot();
            SetMode(CORTEX_BOSS_GEM_STATE_SHRINK);
        }
        break;
    case CORTEX_BOSS_GEM_STATE_SHRINK:
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
CortexBossPlatformMover::CortexBossPlatformMover()
    : PlatformMover(0, 0, false, false, PLATFORM_TYPE_FLIP)
{
}

CortexShotCtrl::~CortexShotCtrl()
{
}

CortexShotCtrl::CortexShotCtrl(CortexBossCtrl *boss)
{
    this->boss = boss;
}

/* Sets `kind` to `flag` (0 or 1) on every part in the gTouchableList
 * list, the list the level's platforms join (platform_create.cpp). In the
 * Cortex fight those include the Cortex platform movers, which
 * UpdateCortexBossPlatformMover animates to frame 10 for kind 1 and
 * 0x1A otherwise; SetCortexTargetState clears it (state 1) and sets it
 * (state 5). */
void CortexTargetCtrl::SetPlatformsKind(u8 flag)
{
    s32 i;
    s32 n = gTouchableList->count;

    for (i = 0; i < n; i++) {
        MovingSprite *p = gTouchableList->items[i]->AsMovingSprite();

        if (flag)
            p->kind = EVENT_HIT;
        else
            p->kind = flag;
    }
}

/* Hops from `part`'s position to (x, y), in the boss round's number of
 * steps. */
void CortexTargetCtrl::SetDest(MovingSprite *part, s32 x, s32 y)
{
    u8 v;

    this->x = x;
    this->y = y;
    dx = x - part->x;
    dy = y - part->y;
    v = *(boss->counter + gCortexTargetHopSteps);
    steps = v;
    stepsLeft = v;
}

CortexTargetCtrl::~CortexTargetCtrl()
{
}

CortexTargetCtrl::CortexTargetCtrl(CortexBossCtrl *boss)
{
    stepsLeft = 0;
    this->boss = boss;
}

/* The same shape as CortexBossCtrl::SetState below (and
 * SetCortexTargetState above) without any state of its own. */
void CortexCannonCtrl::SetState(MovingSprite *, s32 next)
{
    SetMode(next);
}

void CortexCannonCtrl::Update(MovingSprite *part)
{
    if (state == CORTEX_CANNON_STATE_IDLE)
        part->f.b.visible = 0;
}

CortexCannonCtrl::~CortexCannonCtrl()
{
}

CortexCannonCtrl::CortexCannonCtrl()
{
}

/* State 3 (Neo Cortex beaten) also stops the target (mode 9) and, unless
 * the player has it, spawns the body slam power. */
void CortexBossCtrl::SetState(MovingSprite *, s32 next)
{
    if (next == CORTEX_BOSS_STATE_DEFEATED) {
        target->mover->SetMode(CORTEX_TARGET_STATE_STOP);
        if (!gLevelState->HasTurboRun())
            SpawnBodySlamPower(0xFFFF, 0x8C, 0x98, 0);
    }
    SetMode(next);
}

CortexBossCtrl::~CortexBossCtrl()
{
}

CortexBossCtrl::CortexBossCtrl()
{
}
