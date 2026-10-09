#include "enemy_ctrl.hpp"
#include "spawners.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "util.h"
#include <libgcc.h>
#include "player.h"
#include "objects.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #9/#10: EnemyCtrl's attack updaters and state setter
 * (include/enemy_ctrl.hpp), from the 0x0800B8DC-0x0800D040 cluster
 * (docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md). The whole
 * file is old_agbcc's code (issue #10 NAKED retry,
 * docs/matching/archive/issue-10-naked-retry.md).
 *
 * UpdateAttackCycle (Update's states 4, 13, 14, 16 and 18) runs the
 * enemy's attack cycle off gRoomFrameCount: every idleTime + attackTime
 * frames, offset by cycleOffset, an idle enemy (mode 0) starts its
 * attack (animation mode 3, or 4 when the table maps mode 3 to
 * animation 8), and attackTime frames later an attacking one (mode 4)
 * ends it (mode 5, or straight back to 0 likewise). Penguins also stop
 * and restart their walk; crushers are solid only while slammed down,
 * and play the slam sound; the flamethrower lab assistant spawns its
 * flame at keyframe 9.
 * `__modsi3` is a remainder (`a % b`).
 *
 * UpdateTriggerBox (states 3 and 17) waits in mode 0 for the player to
 * touch its trigger box (boxL..boxB around the target, mirrored with
 * it), then plays animation mode 2; a vulture also swoops (a fixed
 * speed ramp), and is never let above baseY. Mode 2 goes back to 0 once
 * the animation is done.
 *
 * The spawn is an inline copy of LaunchHarmfulEffectPart (below):
 * passing the arguments through inline parameters is what materializes
 * them in the ROM's order, and the `+0xC` flag writes are bitfield
 * stores (QImode `-0x41`/`-9` masks). */

/* LaunchHarmfulEffectPart (below), inlined. */
static inline MovingSprite *SpawnPart(s32 a, s32 b, s32 c, s32 d, s32 e, MovingSprite *f)
{
    MovingSprite *obj = gEntitySpawner->LaunchEffectPart(a, b, c, d, e, f);

    obj->f.b.visible = 1;
    obj->f.b.vulnerable = 0;
    return obj;
}

void EnemyCtrl::UpdateAttackCycle()
{
    switch (mode) {
    case 0:
        if (attackTime > 0 &&
            __modsi3(gRoomFrameCount + (idleTime + attackTime) * 2 - cycleOffset - idleTime,
                     idleTime + attackTime) == 0) {
            if (anims[3] != 8)
                SetAnimMode(3);
            else
                SetAnimMode(4);
            if (kind == ENEMY_KIND_PENGUIN) {
                SetMotionX(0);
            } else if (kind == ENEMY_KIND_WOODEN_CRUSHER || kind == ENEMY_KIND_PISTON_CRUSHER) {
                target->f.b.solid = 0;
            }
        }
        break;
    case 4:
        if (idleTime > 0 && __modsi3(gRoomFrameCount + idleTime + attackTime - cycleOffset,
                                     idleTime + attackTime) == 0) {
            if (anims[5] != 8)
                SetAnimMode(5);
            else
                SetAnimMode(0);
        }
        break;
    case 3:
        if (target->animDone) {
            SetAnimMode(4);
            if (kind == ENEMY_KIND_WOODEN_CRUSHER || kind == ENEMY_KIND_PISTON_CRUSHER) {
                target->f.b.solid = 1;
                gAudioContext->PlaySfx(SFX_CRUSHER_SLAM, 0x100);
            } else if (kind == ENEMY_KIND_PENGUIN) {
                gAudioContext->PlaySfx(SFX_UNKNOWN_09, 0x100);
            }
        }
        if (kind == ENEMY_KIND_FLAMETHROWER_LAB_ASSISTANT && target->frame == 9 &&
            target->stepTimer == 0) {
            SpawnPart(0x17, 4, -0x2d, 2, 0, target)->kind = 2;
            gAudioContext->PlaySfx(SFX_FLAMETHROWER, 0x100);
        }
        break;
    case 5:
        if (target->animDone) {
            SetAnimMode(0);
            if (kind != ENEMY_KIND_PENGUIN)
                break;
            SetMotionX(1);
        }
        if (kind == ENEMY_KIND_PENGUIN && target->frame == 8 && target->stepTimer == 0)
            gAudioContext->PlaySfx(SFX_UNKNOWN_23, 0x100);
        break;
    }
}

void EnemyCtrl::UpdateTriggerBox()
{
    s32 m;
    struct aabb box;
    s32 x, y, w, h;

    if (kind == ENEMY_KIND_VULTURE) {
        /* The clamp sets the position through Entity::SetPos with the x
         * unchanged, as the oscillators below do (#662 round 5). The x
         * store is a no-op reload_cse_regs deletes, and flow2 then
         * deletes its load, but the target's fourth reference ranks it
         * above baseY in global-alloc (round 3 found it 3 references
         * over 6 insns against baseY's 3 over 4), so the target takes r1
         * as in the ROM. That is the `target->x = target->x` dummy
         * store the round-2 permuter found; written as a plain
         * `t->y = baseY` the two swap (an r1 pin until round 4). */
        MovingSprite *part = target;

        if (part->y < baseY) {
            part->SetPos(part->x, baseY);
            SetMotionY(0);
        }
    }
    switch (m = mode) {
    case 0:
        x = Q8_TO_INT(target->x);
        y = Q8_TO_INT(target->y);
        w = boxR - boxL;
        h = boxB - boxT;
        SetAabbPos(&box, x + boxL, y + boxT);
        SetAabbSize(&box, w, h);
        /* X-mirrored: bit 4 as a sign test (`lsl #27`); g++ tests the
         * bitfield with an `and`. */
        if ((s32)(target->mirror << 27) < 0)
            box.x = Q8_TO_INT(target->x) * 2 - (box.x + box.w);
        if (gPlayer->TouchesBox(&box)) {
            SetAnimMode(2);
            if (kind == ENEMY_KIND_VULTURE) {
                MovingSprite *part = target;
                s32 a = 0x300, b = 0x20, c;

                part->speedY = a;
                part->rampY.start = a;
                part->rampY.step = b;
                part->rampY.target = m;
                c = -0x200;
                part->speedX = m;
                part->rampX.start = m;
                part->rampX.step = b;
                part->rampX.target = c;
            }
        }
        break;
    case 2:
        if (target->animDone)
            SetAnimMode(0);
        break;
    }
}

/* SetState's inline copies of SetMotionY, SetMotionX and SetAnimMode
 * (below): its `bl`s go to Ctrl::StartTargetMotion*FromSet
 * directly, never to those three, and SetTargetAnim is called through
 * the vtable in place. In SetState (old_agbcc, issue #10 NAKED retry):
 *  - The groups the ROM keeps apart ({1,3,17} vs {6,9,10,11}) are
 *    separate cases; reload picks a different scratch register for the
 *    trigger's `ldrsh` in each, so cross-jumping can't merge them.
 *  - That scratch register is picked round-robin in insn order, so the
 *    {4,14,16} keyframe tail has to be written out in both branches
 *    (cross-jumping then merges the copies): with a single shared tail
 *    the else branch's `ldrsh` gets r4 instead of the ROM's r1.
 *  - State 5's velocity stores go through inline setters, which puts
 *    the shared 0 in r2 before the first store. */

static inline void SetMotionYInline(EnemyCtrl *self, s32 mode)
{
    self->modeA = mode;
    self->Ctrl::StartTargetMotionYFromSet(self->target, mode);
}

static inline void SetMotionXInline(EnemyCtrl *self, s32 mode)
{
    self->modeB = mode;
    self->Ctrl::StartTargetMotionXFromSet(self->target, mode);
}

static inline void SetAnimModeInline(EnemyCtrl *self, s32 mode)
{
    self->mode = mode;
    self->SetTargetAnim(self->target, self->anims[mode]);
}

static inline void SetVelX(MovingSprite *t, s32 v, s32 w)
{
    t->speedX = v;
    t->rampX.start = v;
    t->rampX.step = w;
    t->rampX.target = v;
}

static inline void SetVelY(MovingSprite *t, s32 v, s32 w)
{
    t->speedY = v;
    t->rampY.start = v;
    t->rampY.step = w;
    t->rampY.target = v;
}

/* Sets Update's `state` (1-18) and starts it: the motion and animation
 * modes it begins with (state 5 falls: a fixed speed ramp), then latches
 * the target's position into baseX/baseY. The level spawners call it
 * once the enemy is set up. */
void EnemyCtrl::SetState(s32 newState)
{
    state = newState;
    switch (newState) {
    case 5:
        {
            MovingSprite *part = target;

            SetVelX(part, -0x180, 0);
            SetVelY(part, 0x400, 0);
            part->f.b.collides = 1;
        }
        break;
    case 6:
    case 9:
    case 10:
    case 11:
        SetAnimModeInline(this, 0);
        break;
    case 2:
    case 15:
        SetMotionXInline(this, 1);
        SetAnimModeInline(this, 0);
        break;
    case 13:
    case 18:
        SetMotionXInline(this, 1);
    case 4:
    case 14:
    case 16:
        if (cycleOffset >= idleTime) {
            SetAnimModeInline(this, 4);
            {
                MovingSprite *part = target;
                part->frame = part->bank->anims[part->tag].frameCount - 1;
            }
        } else {
            SetAnimModeInline(this, 0);
            if (kind == ENEMY_KIND_STATIONARY_SPACE_ENEMY) {
                MovingSprite *part = target;
                part->frame = part->bank->anims[part->tag].frameCount - 1;
            }
        }
        break;
    case 1:
    case 3:
    case 17:
        SetAnimModeInline(this, 0);
        break;
    case 8:
        SetMotionXInline(this, 3);
        SetMotionYInline(this, 3);
        SetAnimModeInline(this, 0);
        break;
    case 7:
        SetMotionXInline(this, 2);
        SetAnimModeInline(this, 0);
        counter = 0;
        break;
    case 12:
        break;
    }
    baseX = target->x;
    baseY = target->y;
}

/* Sets the X homing bounds to the target's x +/- `radius` (Q8) and
 * caches the homing speed pair. */
void EnemyCtrl::SetRangeXSpeed(s32 radius, s32 newSpeed, s32 newAccel)
{
    s32 x = target->x;
    rangeX[0] = x - INT_TO_Q8(radius);
    rangeX[1] = target->x + INT_TO_Q8(radius);
    accel = newAccel;
    speed = newSpeed;
}

/* The Y-axis version of SetRangeXSpeed. */
void EnemyCtrl::SetRangeYSpeed(s32 radius, s32 newSpeed, s32 newAccel)
{
    s32 y = target->y;
    rangeY[1] = y - INT_TO_Q8(radius);
    rangeY[0] = target->y + INT_TO_Q8(radius);
    accel = newAccel;
    speed = newSpeed;
}

/* SetRangeXSpeed's bounds without the speed pair. */
void EnemyCtrl::SetRangeX(s32 radius)
{
    s32 x = target->x;
    rangeX[0] = x - INT_TO_Q8(radius);
    rangeX[1] = target->x + INT_TO_Q8(radius);
}

/* The enemy controller's small methods (include/enemy_ctrl.hpp), ROM
 * 0x0800C8AC-0x0800CACC: EnemyCtrl's mode triggers, oscillators,
 * constructor, destructor and setters. The periodic spawner and the
 * knocked controller follow in periodic_spawner.cpp and
 * knocked_enemy_ctrl.cpp. EnemyCtrl's Update and HandleEvent are in
 * enemy_ctrl_update.cpp, its per-state updaters above and in
 * enemy_motion.cpp, enemy_patrol.cpp and enemy_shooter.cpp. */

/* Sets the motion mode: starts the motion set's Y record `mode` on the
 * target, through Ctrl's method called directly (not through the
 * vtable). */
void EnemyCtrl::SetMotionY(s32 mode)
{
    modeA = mode;
    Ctrl::StartTargetMotionYFromSet(target, mode);
}

/* The same with the X record. */
void EnemyCtrl::SetMotionX(s32 mode)
{
    modeB = mode;
    Ctrl::StartTargetMotionXFromSet(target, mode);
}

/* Sets the animation mode: plays `anims[mode]` on the target, through
 * the SetTargetAnim slot. */
void EnemyCtrl::SetAnimMode(s32 m)
{
    mode = m;
    SetTargetAnim(target, anims[m]);
}

/* The three oscillators move the target along a sine wave
 * (gSineTable) around baseX/baseY, `amplitude` high. The phase is
 * gRoomFrameCount scaled by `period` (UpdateOscillateX/Y) or at half
 * rate (UpdateBob), offset by `phase`; the `+ 0x100` keeps the index
 * positive before the mask, and g++ folds it into the ROM's
 * `phase + 0xFFFFFF00`.
 *
 * Each one sets the whole position through Entity::SetPos, passing the
 * other coordinate back unchanged (#662 round 5). That accounts for the
 * registers the C++ needed pins for until round 4, including the saved
 * registers no instruction uses (r5 in UpdateBob, r8 in
 * UpdateOscillateY): the unchanged coordinate is a pseudo that holds a
 * register through allocation, and after reload, reload_cse_regs
 * deletes its store as a no-op (the register still holds that memory
 * word) and flow2 deletes the load, but the prologue still saves the
 * register. In UpdateOscillateX the y read is just before SetPos, as
 * the target's is: that extra pseudo moves the product into r2.
 *
 * The divisions are `/`, not explicit __udivsi3 calls: g++ expands `/`
 * as a const libcall, which doesn't clobber memory, so reload_cse still
 * knows UpdateOscillateY's x word after the call. Through a call to the
 * declared function the x store stays. */
void EnemyCtrl::UpdateOscillateX()
{
    const s16 *table = gSineTable;
    u32 t = INT_TO_Q8(gRoomFrameCount) / period;

    target->SetPos(baseX + table[(t + 0x100 - phase) & 0xff] * amplitude, target->y);
}

void EnemyCtrl::UpdateBob()
{
    MovingSprite *part = target;
    const s16 *table = gSineTable;
    s32 x = part->x;
    u32 t = gRoomFrameCount / 2;

    part->SetPos(x, baseY + table[(t + 0x100 - phase) & 0xff] * amplitude);
}

void EnemyCtrl::UpdateOscillateY()
{
    MovingSprite *part = target;
    const s16 *table = gSineTable;
    s32 x = part->x;
    u32 t = INT_TO_Q8(gRoomFrameCount) / period;

    part->SetPos(x, baseY + table[(t + 0x100 - phase) & 0xff] * amplitude);
}

/* Launches a harmful effect part (state 18's floating popup, the
 * shooters' shots): LaunchEffectPart from gEntitySpawner, then flags
 * bit 2 set and bit 6 cleared on the new part.
 *
 * The OR and the mask are separate locals, assigned in the ROM's order
 * (`flags = 4; flags |= obj[0xc]; mask = -0x41`): a single expression
 * swaps the loads and loads the mask positive. LaunchEffectPart reads
 * five arguments after the pool; the sixth, `f`, is only stored to the
 * stack, as in the ROM. */
MovingSprite *LaunchHarmfulEffectPart(s32 a, s32 b, s32 c, s32 d, s32 e, MovingSprite *src)
{
    MovingSprite *obj;
    s32 flags;
    s32 mask;

    obj = gEntitySpawner->LaunchEffectPart(a, b, c, d, e, src);
    flags = 4;
    flags |= obj->f.flags;
    mask = -0x41;
    obj->f.flags = flags & mask;
    return obj;
}

/* Takes the part to steer. The level spawners attach it through this
 * slot. */
void EnemyCtrl::Attach(MovingSprite *part)
{
    target = part;
}

/* A sound's volume at (x, y), in pixels: full (0x100) within 0x20
 * pixels of the player on both axes, falling to 0 at 0xA0 pixels on the
 * farther axis (EnemyCtrl::Update's state 18 computes the same inline).
 * The C needed six register pins for this under agbcc; it is
 * old_agbcc's code, and old_agbcp gives it as written. */
s32 GetSfxVolumeAt(s32 x, s32 y)
{
    Player *p = gPlayer;
    s32 dx = ABS_BRANCHLESS(x - Q8_TO_INT(p->x));
    s32 d = ABS_BRANCHLESS(y - Q8_TO_INT(p->y));

    LIMIT_MIN(d, dx);
    d = CLAMP_MIN(d, 0x20);
    LIMIT_MAX(d, 0xa0);
    return 0x100 - (d - 0x20) * 2;
}

/* No target, no mode table, no popup, and the enemy motion set. */
void EnemyCtrl::Reset()
{
    target = 0;
    anims = 0;
    animSet = &gEnemyCtrlMotionSet;
    popup = 0;
}

/* g++ sets the vtable pointer back to gEnemyCtrlVtable, then calls ~Ctrl
 * (DestroyCtrl) with the same flags. */
EnemyCtrl::~EnemyCtrl()
{
}

/* Ctrl() (InitCtrl), the vtable pointer, then Reset. */
EnemyCtrl::EnemyCtrl()
{
    Reset();
}

/* The oscillator: the phase's period and offset, and the amplitude. */
void EnemyCtrl::SetOscillator(s32 newPeriod, s32 newPhase, s32 newAmplitude)
{
    period = newPeriod;
    phase = newPhase;
    amplitude = newAmplitude;
}

/* The shooter's period and phase (UpdateShooter). */
void EnemyCtrl::SetShotPeriod(s32 newPeriod, s32 newPhase)
{
    shotPeriod = newPeriod;
    shotPhase = newPhase;
}

/* The attack cycle (UpdateAttackCycle, SetState) - the same stores as
 * spawn_enemies.cpp's inline SetAttackCycle, which the enemy spawners
 * use instead.
 * UNUSED - no caller anywhere in the ROM (checked every src/ and lib/ .c
 * file and every word-aligned Thumb pointer in baserom.gba). */
void EnemyCtrl::SetAttackTiming(s32 newIdleTime, s32 newAttackTime, s32 newCycleOffset)
{
    idleTime = newIdleTime;
    attackTime = newAttackTime;
    cycleOffset = newCycleOffset;
}

/* The trigger box (UpdateTriggerBox), relative to the target. The fourth
 * corner arrives on the stack. */
void EnemyCtrl::SetTriggerBox(s32 l, s32 t, s32 r, s32 b)
{
    boxL = l;
    boxR = r;
    boxT = t;
    boxB = b;
}

/* The per-mode animation table (SetAnimMode). */
void EnemyCtrl::SetModeTable(const s32 *newAnims)
{
    anims = newAnims;
}

/* The enemy kind; the level spawners store `kind` directly.
 * UNUSED - no caller anywhere in the ROM (checked every src/ and lib/ .c
 * file and every word-aligned Thumb pointer in baserom.gba). */
void EnemyCtrl::SetKind(s32 newKind)
{
    kind = newKind;
}
