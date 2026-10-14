#include "swim_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"
#include "level_state.hpp"
#include "key_input.hpp"

extern "C" {
#include "system.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "math_util.h"
}

using Engine::Sys::KeyInput;

/* GitHub issues #19 (its last raw function, CheckSwimCtrlTurn) and #20
 * (0x08016128-0x08017524): the swim controller, SwimCtrl
 * (include/swim_ctrl.hpp, gSwimCtrlVtable; #664, docs/cplusplus.md).
 * It drives the diving Crash of the room-kind-1 (underwater) rooms:
 * sprite bank 1 is Crash with an air tank and flippers, `tilt` is his swim
 * direction (0 up, 6 level, 12 down) and ApplySwimDrift blows the bank-40
 * bubbles. States: 0 idle (floating, a slow bob), 1 swim, 2 stroke (A),
 * 3 spin (B/R), 4 turn round, 5 stop (back to idle), 6 swim start (D-pad
 * from idle), 7 dead.
 *
 * Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS), like the old_agbcc
 * C it replaces - see docs/matching/archive/issue-20-player-ctrl.md.
 *
 * - Update (slot 1) is the per-frame update: D-pad up/down with
 *   auto-repeat (`repeat`) steps `tilt` (0..12) and re-applies the
 *   target's animation (ApplyLevel, whose out-of-line copy is ApplyTilt),
 *   then runs the state method through `stateFuncs`
 *   (gSwimCtrlStateFuncs).
 * - HandleEvent (slot 2) is the message handler, Attach (slot 3) sets the
 *   target; the constructor is play_room.cpp's `new SwimCtrl`.
 * - SetState sets the state (SetMode) and picks the animation from
 *   gSwimCtrlModeAnimRows[mode][tilt].
 * - SetDriftX writes the player's X drift ramp from its speed, like
 *   swim_ctrl_drift.cpp's SetDriftY does for Y.
 *
 * Several functions are inline helpers in the original (C++ inline
 * methods): SetStateNow/SetState, ResetMode/StartSwim,
 * SetDriftNowX/SetDriftX and ApplyLevel/ApplyTilt are each inlined at
 * some call sites and also emitted out of line.
 *
 * UNUSED - no reference in asm/, src/ or data/ and no Thumb pointer in the
 * ROM: ApplyMotion, StartMotionYFromSet, StartMotionXFromSet,
 * GetDriftStep, ApplyTilt, StartSwim, ClearUnk14, SetMotionYPending,
 * SetMotionXPending. Matched anyway. */

/* The held/pressed key words of gKeys. The zero-length array
 * makes the struct BLKmode, so a local copy lives on the stack (the ROM
 * reads `pressed` back with `ldrh [sp, #2]`); without it gcc keeps the
 * copy in a register. */
struct keys {
    u16 held;
    u16 pressed;
    u8 pad[0];
};

static inline u8 LevelAnim(SwimCtrl *self)
{
    return gSwimCtrlModeAnimRows[self->mode][self->tilt].anim;
}

/* the out-of-line copy is SetState */
static inline void SetStateNow(SwimCtrl *self, s32 newState, s32 newMode, s32 newTimer,
                               s32 newTimerMax)
{
    self->SetMode(newState);
    self->mode = newMode;
    self->SetTargetAnim(self->target, LevelAnim(self));
    if (newTimer != CTRL_KEEP)
        self->timer = newTimer;
    if (newTimerMax != CTRL_KEEP)
        self->timerMax = newTimerMax;
}

/* the out-of-line copy is StartSwim */
static inline void ResetMode(SwimCtrl *self)
{
    self->SetState(SWIM_STATE_SWIM, SWIM_MODE_SWIM, CTRL_KEEP, 0);
}

/* The facing tests: as C++, a test of the unsigned `mirrorFlags.mirrorX`
 * compiles to `ands #16`, where the ROM (and the C front end) has `lsls
 * #27` and a sign test, which `mirrorBits.flipX < 0` (the signed view)
 * gives. The
 * one "faces right" test (HandleEvent's side 1) extracts the bit
 * (`lsls #27; lsrs #31`): a u8 copy of it, through this helper. */
static inline u8 FlipX(Player *t)
{
    return t->mirrorFlags.mirrorX;
}

/* QueueMotionX/QueueMotionY (at the end of this file) as this file has them
 * inlined. */
static inline void QueueNowX(SwimCtrl *self, s32 value)
{
    self->motionXPending = 1;
    self->motionX = value;
}

static inline void QueueNowY(SwimCtrl *self, s32 value)
{
    self->motionYPending = 1;
    self->motionY = value;
}

void SwimCtrl::CheckTurn()
{
    u8 dir = gInput->GetDpadDirection();

    switch (state) {
    case SWIM_STATE_IDLE ... SWIM_STATE_SPIN:
    case SWIM_STATE_STOP ... SWIM_STATE_SWIM_START:
        target->mirrorFlags.mirrorY = 0;
        if (target->mirrorBits.flipX < 0 &&
            (dir == KEY_DIR_RIGHT || dir == KEY_DIR_UP_RIGHT || dir == KEY_DIR_DOWN_RIGHT)) {
            target->mirrorFlags.mirrorX = 0;
            if (state != SWIM_STATE_SPIN) {
                SetState(SWIM_STATE_TURN, SWIM_MODE_TURN_RIGHT, CTRL_KEEP, CTRL_KEEP);
                timerMax = 0;
            } else {
                SetState(SWIM_STATE_TURN, SWIM_MODE_SPIN_TURN_RIGHT, CTRL_KEEP, CTRL_KEEP);
            }
            unk_26 = 0;
        } else if (!(target->mirrorFlags.mirrorX & 1) &&
                   (dir == KEY_DIR_LEFT || dir == KEY_DIR_UP_LEFT || dir == KEY_DIR_DOWN_LEFT)) {
            if (state != SWIM_STATE_SPIN) {
                SetState(SWIM_STATE_TURN, SWIM_MODE_TURN_LEFT, CTRL_KEEP, CTRL_KEEP);
                timerMax = 0;
            } else {
                SetState(SWIM_STATE_TURN, SWIM_MODE_SPIN_TURN_LEFT, CTRL_KEEP, CTRL_KEEP);
            }
            unk_26 = 0;
        }
        break;
    }
}

void SwimCtrl::HandleEvent(s32, s32 event, s32 arg)
{
    s32 side;

    switch (event) {
    case EVENT_BUMP:
        side = arg & 3;
        if (side == 2) {
            if (target->mirrorBits.flipX < 0)
                QueueNowX(this, 0);
        } else if (side == 1) {
            if (!FlipX(target))
                QueueNowX(this, 0);
        } else {
            break;
        }
        target->speedX = 0;
        break;
    case EVENT_HIT_PUFFERFISH:
        KillPlayer(0x2D);
        break;
    case EVENT_HIT_ELECTRIC:
        KillPlayer(0x2B);
        break;
    case EVENT_HIT_EXPLOSION:
        KillPlayer(0x2C);
        break;
    case EVENT_HIT:
    case EVENT_HIT_BITE:
    case EVENT_HIT_CRUSH:
        KillPlayer(0x2E);
        break;
    case EVENT_BOUNCE:
        break;
    }
}

void SwimCtrl::KillPlayer(s32 anim)
{
    gAudioContext->PlaySfx(SFX_PLAYER_HURT, 0x100);
    SetMode(SWIM_STATE_DEAD);
    SetTargetAnim(target, anim);
    target->f.b.collides = 0;
    target->f.b.vulnerable = 0;
    target->dead = 1;
    gLevelState->LoseLife();
    gPaletteCache->LoadSlot(target->palette, target->bank->anims[target->tag].paletteId);
}

/* the out-of-line copy is SetDriftX */
static inline void SetDriftNowX(s32 start, s32 step, s32 target)
{
    Player *p = gPlayer;
    s32 v = p->speedX;
    s32 t = v * v / 0x4000 + 4;
    s32 signV;
    s32 signC;
    /* `absV` and `absC`: |speed| and |target|, side by side */
    s32 absV;
    s32 absC;

    signV = v >> 31;
    absV = (v ^ signV) - signV;
    signC = target >> 31;
    absC = (target ^ signC) - signC;
    if (absV > absC) {
        p->rampX.start = start;
        p->rampX.step = t;
    } else if (v * target < 0) {
        /* `sum` is needed: inlined, the instructions are reordered */
        s32 sum = t + step;

        p->rampX.start = start;
        p->rampX.step = sum;
    } else {
        p->rampX.start = start;
        p->rampX.step = step;
    }
    p->rampX.target = target;
}

/* "Mark gone": MarkEntityGone's sequence (entity.cpp), inlined - set flags
 * bit 0, then unless the id is 0xFFFF set its bit in the bitmap. */
static inline void MarkGone(Player *t)
{
    ENTITY_MARK_GONE(t->f.b.gone, t->id);
}

static inline void ClampFrame(Player *t, s32 frame)
{
    s32 n = t->bank->anims[t->tag].frameCount;

    CLAMP_INDEX(frame, n);
    t->frame = frame;
}

static inline void RestoreFrame(Player *t, s32 frame, s32 f34)
{
    ClampFrame(t, frame);
    t->stepTimer = f34;
}

/* Re-applies the animation for the current mode/level; the out-of-line
 * copy is ApplyTilt. */
static inline void ApplyLevel(SwimCtrl *self)
{
    Player *t = self->target;
    u8 *tag = &t->tag;

    if (*tag != 0x20 && *tag != 0x1D && *tag != 0x1F) {
        s32 frame = t->frame;
        s32 f34 = t->stepTimer;

        *tag = gSwimCtrlModeAnimRows[self->mode][self->tilt].anim;
        t->ResetFrameTimer();
        t->ResetFrameIndex();
        t->SetAnimDone(0);
        RestoreFrame(self->target, frame, f34);
    } else {
        s32 frame;
        s32 f34;

        /* `mode` is a SWIM_MODE_* (0-7), so only SPIN_TURN_LEFT can
         * match (#843). The other labels stay numbers: what 14-18, 23,
         * 33-37 and 41 were meant to be isn't known (they aren't swim
         * modes, and as ACTION_STATE_* they form no set), and the ROM's
         * jump table has them all. */
        switch (self->mode) {
        case SWIM_MODE_SPIN_TURN_LEFT:
        case 14 ... 18:
        case 23:
        case 33 ... 37:
        case 41:
            frame = self->target->frame;
            f34 = self->target->stepTimer;
            ResetMode(self);
            RestoreFrame(self->target, frame, f34);
            break;
        default:
            ResetMode(self);
            break;
        }
    }
}

void SwimCtrl::Update(MovingSprite *)
{
    if (state == SWIM_STATE_DEAD) {
        (this->*stateFuncs[state])();
    } else {
        u32 keys;
        u8 dir;
        /* `inp` is needed: inlined, the registers differ */
        KeyInput *inp;

        if (spinCooldown)
            spinCooldown--;
        inp = gInput;
        keys = gKeys.all; /* the whole word, held keys low */
        dir = inp->GetDpadDirection();

        if (!(keys & (DPAD_UP | DPAD_DOWN)) && state != SWIM_STATE_STROKE) {
            if (repeat && tilt != 6)
                repeat = repeat - 1;
            else
                repeat = 3;
            if (repeat == 0 && tilt != 6) {
                if (tilt <= 5)
                    tilt++;
                if (tilt > 6)
                    tilt--;
                if (tilt != 6)
                    repeat = 3;
                ApplyLevel(this);
            }
        } else if (state != SWIM_STATE_STROKE || dir != KEY_DIR_NONE) {
            if (repeat == 0 || --repeat == 0) {
                repeat = 3;
                if (keys & DPAD_UP) {
                    if (keys & DPAD_SIDEWAYS) {
                        if (tilt > 3) {
                            tilt--;
                            ApplyLevel(this);
                        } else if (tilt <= 2) {
                            tilt++;
                            ApplyLevel(this);
                        }
                    } else if (tilt != 0) {
                        tilt--;
                        ApplyLevel(this);
                    }
                } else if (keys & DPAD_DOWN) {
                    if (keys & DPAD_SIDEWAYS) {
                        if (tilt <= 8) {
                            tilt++;
                            ApplyLevel(this);
                        } else if (tilt > 9) {
                            tilt--;
                            ApplyLevel(this);
                        }
                    } else if (tilt <= 11) {
                        tilt++;
                        ApplyLevel(this);
                    }
                }
            }
        }

        (this->*stateFuncs[state])();
        if (target->hitMask & PART_DIR_X_MASK)
            target->speedX = 0;
        if (target->hitMask & PART_DIR_Y_MASK)
            target->speedY = 0;
        target->StoreHitAxes(0);
        ApplySwimDrift();
    }

    if (state == SWIM_STATE_SPIN || mode == SWIM_MODE_SPIN_TURN_RIGHT ||
        mode == SWIM_MODE_SPIN_TURN_LEFT)
        target->kind = EVENT_ATTACK_SPIN;
    else
        target->kind = EVENT_HIT;
}

/* UNUSED */
void SwimCtrl::ApplyMotion()
{
    const speed_ramp *rec;

    if (motionXPending == 1) {
        motionXPending = 0;
        rec = &gSwimCtrlMotionRecords[animSet->entries[motionX][0]];
        Ctrl::StartTargetMotionX(target, &rec->start);
    }
    if (motionYPending == 1) {
        motionYPending = 0;
        rec = &gSwimCtrlMotionRecords[animSet->entries[motionY][1]];
        Ctrl::StartTargetMotionY(target, rec);
    }
}

void SwimCtrl::StateIdle()
{
    struct keys k;
    /* `dir` is needed: inlined, the instructions differ */
    u8 dir;
    u8 count;
    /* `inp` is needed: inlined, the instructions are reordered */
    KeyInput *inp = gInput;

    k = *(struct keys *)&gKeys;
    dir = inp->GetDpadDirection();
    count = ++idleTimer;
    if (count == 30) {
        QueueNowY(this, 1);
    } else if (count > 59) {
        QueueNowY(this, 2);
        idleTimer = 0;
    }
    if (target->animDone)
        SetTargetAnim(target, 0x1F);
    if (k.pressed & A_BUTTON) {
        StartStroke();
    } else if (k.pressed & (B_BUTTON | R_BUTTON)) {
        StartSpin();
    } else if (dir) {
        SetMode(SWIM_STATE_SWIM_START);
        SetTargetAnim(target, 0x1D);
        QueueNowX(this, 0xC);
    }
    unk_26 = 0;
    CheckTurn();
}

void SwimCtrl::StateSwim()
{
    struct keys k;
    u8 dir = gInput->GetDpadDirection();

    k = *(struct keys *)&gKeys;
    if (k.pressed & A_BUTTON) {
        StartStroke();
        return;
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
        StartSpin();
    if (dir == KEY_DIR_NONE && tilt == 6) {
        SetMode(SWIM_STATE_STOP);
        SetTargetAnim(target, 0x20);
        mode = dir; // KEY_DIR_NONE: SWIM_MODE_IDLE
    }
    CheckTurn();
}

void SwimCtrl::StateStroke()
{
    /* `inp` is needed: inlined, the instructions differ */
    KeyInput *inp = gInput;
    struct keys k = *(struct keys *)&gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON) {
        StartSpin();
        return;
    }
    if (target->animDone || gRoomFrameCount > deadline) {
        /* `dir` is needed: inlined, the instructions differ */
        u8 dir = inp->GetDpadDirection();

        if (kp->pressed & A_BUTTON)
            StartStroke();
        else if (dir)
            ResetMode(this);
        else if (tilt == 6) {
            SetMode(SWIM_STATE_STOP);
            SetTargetAnim(target, 0x20);
        } else
            ResetMode(this);
    }
    CheckTurn();
}

void SwimCtrl::StateSpin()
{
    /* `dir` is needed: inlined, the code is 2 instructions shorter */
    u8 dir = gInput->GetDpadDirection();

    if (++timer >= timerMax || target->animDone) {
        gPlayer->bounce = 0;
        spinCooldown = 0xC;
        if (dir == KEY_DIR_NONE) {
            ResetMode(this);
            timerMax = 1;
        } else {
            ResetMode(this);
        }
    }
    CheckTurn();
}

void SwimCtrl::StateTurn()
{
    struct keys k = *(struct keys *)&gKeys;
    s32 frame;

    if (timerMax != 0 && ++timer >= timerMax) {
        spinCooldown = 0xC;
        timerMax = 0;
        switch (mode) {
        case SWIM_MODE_SPIN_TURN_LEFT:
            frame = target->frame;
            mode = SWIM_MODE_TURN_LEFT;
            SetTargetAnim(target, gSwimCtrlModeAnimRows[SWIM_MODE_TURN_LEFT][tilt].anim);
            ClampFrame(target, frame);
            break;
        case SWIM_MODE_SPIN_TURN_RIGHT:
            frame = target->frame;
            mode = SWIM_MODE_TURN_RIGHT;
            SetTargetAnim(target, gSwimCtrlModeAnimRows[SWIM_MODE_TURN_RIGHT][tilt].anim);
            ClampFrame(target, frame);
            break;
        }
    }
    if (k.pressed & (B_BUTTON | R_BUTTON)) {
        if (spinCooldown != 0 || timerMax != 0)
            return;
        timer = 0;
        timerMax = 0x18;
        frame = target->frame;
        if (mode == SWIM_MODE_TURN_RIGHT)
            mode = SWIM_MODE_SPIN_TURN_RIGHT;
        else
            mode = SWIM_MODE_SPIN_TURN_LEFT;
        SetTargetAnim(target, gSwimCtrlModeAnimRows[mode][tilt].anim);
        StartSpin();
        ClampFrame(target, frame);
        return;
    }
    if (k.pressed & A_BUTTON)
        StartStroke();
    if (target->animDone == 0)
        return;
    switch (mode) {
    case SWIM_MODE_TURN_RIGHT:
        if (target->mirrorBits.flipX < 0)
            target->mirrorFlags.mirrorX = 0;
        ResetMode(this);
        break;
    case SWIM_MODE_TURN_LEFT:
        target->SetFlipX(1);
        ResetMode(this);
        break;
    case SWIM_MODE_SPIN_TURN_RIGHT:
        if (target->mirrorBits.flipX < 0)
            target->mirrorFlags.mirrorX = 0;
        SetStateNow(this, SWIM_STATE_SPIN, SWIM_MODE_SPIN, CTRL_KEEP, CTRL_KEEP);
        break;
    case SWIM_MODE_SPIN_TURN_LEFT:
        target->SetFlipX(1);
        SetStateNow(this, SWIM_STATE_SPIN, SWIM_MODE_SPIN, CTRL_KEEP, CTRL_KEEP);
        break;
    }
    motionXPending = 1;
}

void SwimCtrl::StateSwimStart()
{
    /* `inp` is needed: inlined, the instructions differ */
    KeyInput *inp = gInput;
    struct keys k = *(struct keys *)&gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON) {
        StartSpin();
        return;
    }
    if (target->animDone) {
        /* `dir` is needed: inlined, the code is 2 instructions shorter */
        u8 dir = inp->GetDpadDirection();

        if (kp->pressed & A_BUTTON)
            StartStroke();
        else if (dir)
            ResetMode(this);
        else if (tilt == 6) {
            SetMode(SWIM_STATE_STOP);
            SetTargetAnim(target, 0x20);
        }
    }
    CheckTurn();
}

void SwimCtrl::StateStop()
{
    struct keys k = *(struct keys *)&gKeys;

    if (k.pressed & A_BUTTON) {
        StartStroke();
        return;
    }
    if (k.pressed & (B_BUTTON | R_BUTTON)) {
        StartSpin();
        return;
    }
    if (target->animDone) {
        mode = SWIM_MODE_IDLE;
        SetStateNow(this, SWIM_STATE_IDLE, SWIM_MODE_IDLE, 0, 0);
    }
    CheckTurn();
}

void SwimCtrl::StateDead()
{
    SetDriftY(0, 5, 0);
    SetDriftNowX(0, 5, 0);
    if (target->animDone)
        MarkGone(target);
}

void SwimCtrl::Attach(MovingSprite *owner)
{
    target = owner->AsPlayer();
}

/* UNUSED */
void SwimCtrl::StartMotionYFromSet(Player *part, s32 idx)
{
    Ctrl::StartTargetMotionY(part, &gSwimCtrlMotionRecords[animSet->entries[idx][1]]);
}

/* UNUSED */
void SwimCtrl::StartMotionXFromSet(Player *part, s32 idx)
{
    Ctrl::StartTargetMotionX(part, &gSwimCtrlMotionRecords[animSet->entries[idx][0]].start);
}

void SwimCtrl::SetState(s32 newState, s32 newMode, s32 newTimer, s32 newTimerMax)
{
    SetStateNow(this, newState, newMode, newTimer, newTimerMax);
}

void SwimCtrl::SetDriftX(s32 start, s32 step, s32 target)
{
    SetDriftNowX(start, step, target);
}

/* UNUSED. The ramp step SetDriftX computes from the player's speed:
 * v^2 / 0x4000 + 4. */
s32 SwimCtrl::GetDriftStep(s32 v)
{
    return v * v / 0x4000 + 4;
}

/* UNUSED */
void SwimCtrl::ApplyTilt()
{
    ApplyLevel(this);
}

/* UNUSED */
void SwimCtrl::StartSwim()
{
    SetState(SWIM_STATE_SWIM, SWIM_MODE_SWIM, CTRL_KEEP, 0);
}

/* g++ stores gSwimCtrlVtable and calls ~Ctrl (DestroyCtrl). */
SwimCtrl::~SwimCtrl()
{
}

/* Ctrl(), the vtable pointer, then Reset (ResetSwimCtrl, in
 * swim_ctrl_stroke.cpp). */
SwimCtrl::SwimCtrl()
{
    Reset();
}

/* UNUSED */
void SwimCtrl::ClearUnk14()
{
    unk_14 = 0;
}

/* UNUSED */
void SwimCtrl::SetMotionYPending()
{
    motionYPending = 1;
}

/* UNUSED */
void SwimCtrl::SetMotionXPending()
{
    motionXPending = 1;
}

/* The accessors of the queued X/Y motion entries (`motionX`/`motionY`)
 * and their "pending" flags (the start of input_ctrl.cpp until #768).
 * UNUSED - no `bl`/`.4byte` reference in src/, and no Thumb pointer
 * anywhere in the ROM. */
void SwimCtrl::ClearMotionYPending()
{
    motionYPending = 0;
}

void SwimCtrl::ClearMotionXPending()
{
    motionXPending = 0;
}

u8 SwimCtrl::IsMotionYPending()
{
    return motionYPending;
}

u8 SwimCtrl::IsMotionXPending()
{
    return motionXPending;
}

void SwimCtrl::QueueMotionY(u8 entry)
{
    motionYPending = 1;
    motionY = entry;
}

void SwimCtrl::QueueMotionX(u8 entry)
{
    motionXPending = 1;
    motionX = entry;
}
