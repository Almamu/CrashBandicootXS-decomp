#include "bg_layer.hpp"
#include "enemy_ctrl.hpp"
#include "spawners.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "util.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "math_util.h"
#include "player.h"
}

/* EnemyCtrl's Update (UpdateEnemyCtrl, gEnemyCtrlVtable slot 1) and
 * HandleEvent (HitEnemy, slot 2), include/enemy_ctrl.hpp. They sit next
 * to each other in the ROM; neither calls the other directly (state 11
 * calls HandleEvent through the vtable). The states' semantic map is in
 * docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md. */

/* Sets `t`'s `gone` bit and, if it has an id, its bit in the "gone"
 * bitmap at gEntityFlags+0x108. */
static inline void MarkGone(MovingSprite *t)
{
    ENTITY_MARK_GONE(t->f.b.gone, t->id);
}

static inline void SetVelX(MovingSprite *t, s32 a, s32 b, s32 c)
{
    t->speedX = a;
    t->rampX.start = a;
    t->rampX.step = b;
    t->rampX.target = c;
}

static inline void SetVelY(MovingSprite *t, s32 a, s32 b, s32 c)
{
    t->speedY = a;
    t->rampY.start = a;
    t->rampY.step = b;
    t->rampY.target = c;
}

/* Both values are evaluated before either store, as in the ROM. */
static inline void SetPos(MovingSprite *t, s32 x, s32 y)
{
    t->x = x;
    t->y = y;
}

/* The target's `hit` bit, read the way the ROM does (`lsrs #3; ands
 * #1` on the flags byte; the bitfield would be tested with `movs #8;
 * ands`). */
static inline u32 TargetHit(MovingSprite *t)
{
    return (t->f.flags >> 3) & 1;
}

/* An 18-state dispatcher keyed off `state` (1-18; 0 or > 18 is a no-op,
 * the ROM's `subs r0, #1` / `cmp r0, #0x11` / `bls`). The part argument
 * isn't used: the controller steers its own `target`.
 *
 * The case bodies are in the ROM's block order; 1 and 12 are explicit
 * empty cases so the table is indexed by `state - 1`. What the match
 * needed (see docs/matching/archive/big-naked-retry-2.md):
 * - state 18's second `animDone` test reads the byte through a
 *   `vu8`, so jump threading can't fold it into the first test (the
 *   ROM reloads the target and tests again);
 * - PlayAmbientSfx's `false` is a literal at the call (in a `bool`
 *   local, the 0 gets a register of its own the later stores reuse; the
 *   ROM rematerializes it after the argument slot's address), and the
 *   volume is a separate local;
 * - state 5's height tests read `t->y` into a local first, and the
 *   second test goes through its own `t2`;
 * - state 9 reads each position into a local before storing it. */
void EnemyCtrl::Update(MovingSprite *)
{
    switch (state) {
    case 1:
    case 12:
        break;
    case 17:
        if (target->y >= baseY) {
            if (target->speedX == 0 && target->speedY != 0) {
                SetMotionY(0);
            } else if (target->speedX < 0) {
                u8 onScreen;

                if (target->animDone)
                    SetAnimMode(0);
                onScreen = target->IsOnScreen();
                if (onScreen == 0) {
                    MovingSprite *t;

                    SetPos(target, baseX, baseY - 0x6400);
                    SetMotionX(0);
                    t = target;
                    SetVelY(t, 0x80, 0, 0x80);
                    t->f.b.active = 1;
                }
            } else {
                UpdateTriggerBox();
            }
        }
        {
            MovingSprite *t = target;

            if (t->frame == 0 && t->stepTimer == 0 && t->IsOnScreen())
                gAudioContext->PlaySfx(SFX_UNKNOWN_13, 0x100);
        }
        break;
    case 5:
        {
            MovingSprite *t = target;
            s32 y = t->y;

            if (y > INT_TO_Q8(gLevelLayers->layer0->heightPx) - 0x1E00) {
                t->f.b.collides = 0;
                {
                    MovingSprite *t2 = target;

                    y = t2->y;
                    if (y > INT_TO_Q8(gLevelLayers->layer0->heightPx) + 0x1E00)
                        MarkGone(t2);
                }
            } else if (t->hitAxes == 8) {
                SetVelY(t, 0, 0, 0);
            } else {
                SetVelY(t, 0x400, 0, 0x400);
            }
        }
        break;
    case 18:
        {
            MovingSprite *t = target;
            s32 x = Q8_TO_INT(t->x);
            s32 y = Q8_TO_INT(t->y);
            Player *p = gPlayer;
            s32 dx = ABS_BRANCHLESS(x - Q8_TO_INT(p->x));
            s32 d = ABS_BRANCHLESS(y - Q8_TO_INT(p->y));
            s32 vol;

            LIMIT_MIN(d, dx);
            d = CLAMP_MIN(d, 0x20);
            LIMIT_MAX(d, 0xa0);
            vol = 0x100 - (d - 0x20) * 2;
            gAudioContext->PlayAmbientSfx(SFX_SAUCER_HUM, 8, vol, false);
        }
        {
            /* The first test reads the target through a local and the
             * second reads it again: the ROM reloads the target and its
             * animDone for the mode-5 test (#662 round 2; the C read
             * animDone through a volatile cast). */
            MovingSprite *t = target;

            if (t->animDone && mode == 3) {
                MovingSprite *pop = LaunchHarmfulEffectPart(0x1d, 0, 0, 0x2b, 0, t);

                popup = pop;
                pop->kind = 3;
                gAudioContext->PlaySfx(SFX_SAUCER_ATTACK, 0x100);
            } else if (target->animDone && mode == 5) {
                MarkGone(popup);
                popup = 0;
            }
        }
        UpdatePatrol();
        UpdateAttackCycle();
        if (mode == 1) {
            MovingSprite *t = target;
            u32 m = t->mirrorFlags.mirrorX;

            t->mirrorFlags.mirrorX = !m;
            SetAnimMode(0);
            SetMotionX(1);
        } else if (mode == 6) {
            MovingSprite *t = target;
            u32 m = t->mirrorFlags.mirrorX;

            t->mirrorFlags.mirrorX = !m;
            SetAnimMode(4);
            {
                MovingSprite *t2 = target;

                t2->frame = t2->bank->anims[t2->tag].frameCount - 1;
            }
            SetMotionX(1);
        }
        if (popup)
            popup->x = target->x;
        break;
    case 3:
        UpdateTriggerBox();
        break;
    case 2:
        UpdatePatrol();
        break;
    case 13:
        UpdatePatrol();
        /* fallthrough */
    case 4:
        UpdateAttackCycle();
        break;
    case 14:
        UpdateAttackCycle();
        UpdateOscillateY();
        break;
    case 6:
        UpdateBob();
        break;
    case 7:
        UpdateFlipCycle();
        break;
    case 8:
        UpdateHop();
        break;
    case 9:
        if (!gHomingEnemyXSaved) {
            s32 x = target->x;

            gHomingEnemyX = x;
            gHomingEnemyXSaved = 1;
        }
        if (!gHomingEnemyYSaved) {
            s32 y = target->y;

            gHomingEnemyY = y;
            gHomingEnemyYSaved = 1;
        }
        UpdateHomingX();
        UpdateOscillateX();
        {
            MovingSprite *t = target;
            s32 x, y;

            x = t->x;
            gHomingEnemyX = x;
            y = t->y;
            gHomingEnemyY = y;
        }
        break;
    case 11:
        UpdateHomingX();
        UpdateHomingY();
        /* A sea mine that touched something explodes: it hits itself. */
        if (TargetHit(target) && kind == ENEMY_KIND_SEA_MINE) {
            HandleEvent(0, EVENT_HIT, 0);
            gAudioContext->PlaySfx(SFX_EXPLOSION, 0x100);
        }
        break;
    case 15:
        UpdatePatrol();
        /* fallthrough */
    case 10:
        UpdateHomingY();
        break;
    case 16:
        UpdateAttackCycle();
        UpdateShooter();
        break;
    }
}

/* The enemy is hit (`event`, the EVENT_* code). If the player is in
 * ctrlMode 1, the enemy just vanishes in a puff (effect 0x28). Otherwise
 * its popup goes, and:
 * - spun or slid into (EVENT_ATTACK_SPIN/SLIDE), it is knocked away: its
 *   part gets a KnockedEnemyCtrl, flies off away from the player at a
 *   random height, and this controller deletes itself;
 * - hit or body-slammed, it is squashed: effect 0x29 at its position,
 *   and its part is marked gone.
 * Any other event does nothing more.
 *
 * What the match needs (late NAKED retry 3,
 * docs/matching/archive/late-naked-retry-3.md): the squash stores the
 * layer from an `s32 one` local, so the `1` is loaded before the `-4`
 * mask and shared with the `gone` OR. The squash's MarkGone is
 * Entity::MarkGone (the bitmap indexed, the method's own `1`), which
 * gives the ROM's fresh `movs #1` after the shift count; the C built that
 * `1` with a constant-init asm (#662 round 2). The C also held r2 live
 * across the first MarkGone's id compare with an asm-only register
 * variable, to get the ROM's reload registers; g++ gives them without
 * it. */
static inline MovingSprite *SpawnAt(s32 kind, s32 x, s32 y)
{
    return gEntitySpawner->SpawnEffectPart(kind, 2, x, y, 0);
}

void EnemyCtrl::HandleEvent(MovingSprite *, s32 event, s32)
{
    if (gPlayer->ctrlMode == 1) {
        MarkGone(target);
        SpawnAt(0x28, Q8_TO_INT(target->x), Q8_TO_INT(target->y));
        gAudioContext->PlaySfx(SFX_UNKNOWN_5A, 0x80);
        return;
    }
    if (popup)
        MarkGone(popup);
    switch (event) {
    case EVENT_ATTACK_SPIN:
    case EVENT_ATTACK_SLIDE:
        {
            KnockedEnemyCtrl *knocked = new KnockedEnemyCtrl;
            MovingSprite *part = target;
            MovingSprite *t;
            s32 a, v;

            part->mover = knocked;
            knocked->Attach(part);
            target->f.b.collides = 0;
            t = target;
            if ((a = t->x) > gPlayer->x)
                SetVelX(t, 0x1000, 0, 0x1800);
            else
                SetVelX(t, -0x1000, 0, -0x1800);
            v = ((u16)RandRange(3) << 9) - 0x200;
            {
                MovingSprite *t2 = target;

                SetVelY(t2, v, 0, v);
                t2->f.b.visible = 0;
            }
            gAudioContext->PlaySfx(SFX_ENEMY_KNOCKED_AWAY, 0x80);
            delete this;
        }
        break;
    case EVENT_HIT:
    case EVENT_ATTACK_BODY_SLAM:
    case EVENT_ATTACK_SUPER_BODY_SLAM:
        {
            MovingSprite *obj = SpawnAt(0x29, Q8_TO_INT(target->x), Q8_TO_INT(target->y));
            s32 one = 1;

            obj->f.b.visible = 0;
            obj->mirrorFlags.gfxMode = one;
            target->MarkGone();
        }
        break;
    }
}
