#include "player_ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "system.h"
#include "audio.h"
#include "gfx.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"
#include "math_util.h"
}

/* GitHub issues #19 (its last raw function, CheckPlayerCtrlTurn) and #20
 * (0x08016128-0x08017524): the swim controller, PlayerCtrl
 * (include/player_ctrl.hpp, gPlayerCtrlVtable; #664, docs/cplusplus.md).
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
 *   (gPlayerCtrlStateFuncs).
 * - HandleEvent (slot 2) is the message handler, Attach (slot 3) sets the
 *   target; the constructor is play_room.c's `new PlayerCtrl`.
 * - SetState sets the state (SetMode) and picks the animation from
 *   gPlayerCtrlModeAnimRows[mode][tilt].
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

static inline u8 LevelAnim(PlayerCtrl *self)
{
    return gPlayerCtrlModeAnimRows[self->mode][self->tilt].anim;
}

/* the out-of-line copy is SetState */
static inline void SetStateNow(PlayerCtrl *self, s32 newState, s32 newMode, s32 newTimer,
                               s32 newTimerMax)
{
    self->SetMode(newState);
    self->mode = newMode;
    self->SetTargetAnim((SpriteObj *)self->target, LevelAnim(self));
    if (newTimer != CTRL_KEEP)
        self->timer = newTimer;
    if (newTimerMax != CTRL_KEEP)
        self->timerMax = newTimerMax;
}

/* the out-of-line copy is StartSwim */
static inline void ResetMode(PlayerCtrl *self)
{
    self->SetState(1, 1, CTRL_KEEP, 0);
}

/* The value arrives as a (constant-propagated) inline parameter, so the
 * bitfield store is the generic clear-then-or, as in the ROM. */
static inline void SetFlipX(struct player *t, u32 value)
{
    t->mirror.bits.flipX = value;
}

/* The facing tests: as C++, a test of the unsigned `bits.flipX` compiles
 * to `ands #16`, where the ROM (and the C front end) has `lsls #27` and a
 * sign test, which `mirror.sbits.flipX < 0` (the signed view) gives. The
 * one "faces right" test (HandleEvent's side 1) extracts the bit
 * (`lsls #27; lsrs #31`): a u8 copy of it, through this helper. */
static inline u8 FlipX(struct player *t)
{
    return t->mirror.bits.flipX;
}

static inline void SetHitAxes(struct player *t, s32 value)
{
    t->hitAxes = value;
}

/* QueueMotionX/QueueMotionY (input_ctrl.cpp) as this file has them
 * inlined. */
static inline void QueueNowX(PlayerCtrl *self, s32 value)
{
    self->motionXPending = 1;
    self->motionX = value;
}

static inline void QueueNowY(PlayerCtrl *self, s32 value)
{
    self->motionYPending = 1;
    self->motionY = value;
}

void PlayerCtrl::CheckTurn()
{
    u8 dir = GetDpadDirection(gInput);

    switch (state) {
    case 0 ... 3:
    case 5 ... 6:
        target->mirror.bits.flipY = 0;
        if (target->mirror.sbits.flipX < 0 && (dir == 4 || dir == 6 || dir == 8)) {
            target->mirror.bits.flipX = 0;
            if (state != 3) {
                SetState(4, 4, CTRL_KEEP, CTRL_KEEP);
                timerMax = 0;
            } else {
                SetState(4, 5, CTRL_KEEP, CTRL_KEEP);
            }
            unk_26 = 0;
        } else if (!(target->mirror.bits.flipX & 1) && (dir == 3 || dir == 5 || dir == 7)) {
            if (state != 3) {
                SetState(4, 6, CTRL_KEEP, CTRL_KEEP);
                timerMax = 0;
            } else {
                SetState(4, 7, CTRL_KEEP, CTRL_KEEP);
            }
            unk_26 = 0;
        }
        break;
    }
}

void PlayerCtrl::HandleEvent(SpriteObj *, s32 event, s32 arg)
{
    s32 side;

    switch (event) {
    case EVENT_BUMP:
        side = arg & 3;
        if (side == 2) {
            if (target->mirror.sbits.flipX < 0)
                QueueNowX(this, 0);
        } else if (side == 1) {
            if (!FlipX(target))
                QueueNowX(this, 0);
        } else {
            break;
        }
        target->speedX = 0;
        break;
    case 5:
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

void PlayerCtrl::KillPlayer(s32 anim)
{
    PlaySfx(gAudioContext, SFX_PLAYER_HURT, 0x100);
    SetMode(7);
    SetTargetAnim((SpriteObj *)target, anim);
    target->flags.bits.flag7 = 0;
    target->flags.bits.flag6 = 0;
    target->dead = 1;
    LoseLife(gLevelState);
    LoadPaletteSlot(gPaletteCache, target->slot, target->anim->records[target->tag].paletteId);
}

/* the out-of-line copy is SetDriftX */
static inline void SetDriftNowX(s32 start, s32 step, s32 target)
{
    struct player *p = gPlayer;
    s32 v = p->speedX;
    s32 t = v * v / 0x4000 + 4;
    s32 signV;
    s32 signC;
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
        s32 sum = t + step;

        p->rampX.start = start;
        p->rampX.step = sum;
    } else {
        p->rampX.start = start;
        p->rampX.step = step;
    }
    p->rampX.target = target;
}

/* "Mark gone": MarkEntityGone's sequence (graphics.c), inlined - set flags
 * bit 0, then unless the id is 0xFFFF set its bit in the bitmap. */
static inline void MarkGone(struct player *t)
{
    ENTITY_MARK_GONE(t->flags.bits.gone, t->id);
}

static inline void ClampFrame(struct player *t, s32 frame)
{
    s32 n = t->anim->records[t->tag].frameCount;

    CLAMP_INDEX(frame, n);
    t->frame = frame;
}

static inline void RestoreFrame(struct player *t, s32 frame, s32 f34)
{
    ClampFrame(t, frame);
    t->stepTimer = f34;
}

/* Re-applies the animation for the current mode/level; the out-of-line
 * copy is ApplyTilt. */
static inline void ApplyLevel(PlayerCtrl *self)
{
    struct player *t = self->target;
    u8 *tag = &t->tag;

    if (*tag != 0x20 && *tag != 0x1D && *tag != 0x1F) {
        s32 frame = t->frame;
        s32 f34 = t->stepTimer;

        *tag = gPlayerCtrlModeAnimRows[self->mode][self->tilt].anim;
        ResetSpriteFrameTimer(t);
        ResetSpriteFrameIndex(t);
        SetSpriteAnimDone(t, 0);
        RestoreFrame(self->target, frame, f34);
    } else {
        s32 frame;
        s32 f34;

        switch (self->mode) {
        case 7:
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

void PlayerCtrl::Update(SpriteObj *)
{
    if (state == 7) {
        (this->*stateFuncs[state])();
    } else {
        u32 keys;
        u8 dir;
        void *inp;

        if (spinCooldown)
            spinCooldown--;
        inp = gInput;
        keys = gKeys.all; /* the whole word, held keys low */
        dir = GetDpadDirection(inp);

        if (!(keys & (DPAD_UP | DPAD_DOWN)) && state != 2) {
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
        } else if (state != 2 || dir != 0) {
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
        if (target->hitMask & PLAYER_HIT_X)
            target->speedX = 0;
        if (target->hitMask & PLAYER_HIT_Y)
            target->speedY = 0;
        SetHitAxes(target, 0);
        ApplySwimDrift();
    }

    if (state == 3 || mode == 5 || mode == 7)
        target->kind = 0x13;
    else
        target->kind = 1;
}

/* UNUSED */
void PlayerCtrl::ApplyMotion()
{
    const speed_ramp *rec;

    if (motionXPending == 1) {
        motionXPending = 0;
        rec = &gPlayerCtrlMotionRecords[animSet->entries[motionX][0]];
        Ctrl::StartTargetMotionX((SpriteObj *)target, &rec->start);
    }
    if (motionYPending == 1) {
        motionYPending = 0;
        rec = &gPlayerCtrlMotionRecords[animSet->entries[motionY][1]];
        Ctrl::StartTargetMotionY((SpriteObj *)target, rec);
    }
}

void PlayerCtrl::StateIdle()
{
    struct keys k;
    u8 dir;
    u8 count;
    void *inp = gInput;

    k = *(struct keys *)&gKeys;
    dir = GetDpadDirection(inp);
    count = ++idleTimer;
    if (count == 30) {
        QueueNowY(this, 1);
    } else if (count > 59) {
        QueueNowY(this, 2);
        idleTimer = 0;
    }
    if (target->animDone)
        SetTargetAnim((SpriteObj *)target, 0x1F);
    if (k.pressed & A_BUTTON) {
        StartStroke();
    } else if (k.pressed & (B_BUTTON | R_BUTTON)) {
        StartSpin();
    } else if (dir) {
        SetMode(6);
        SetTargetAnim((SpriteObj *)target, 0x1D);
        QueueNowX(this, 0xC);
    }
    unk_26 = 0;
    CheckTurn();
}

void PlayerCtrl::StateSwim()
{
    struct keys k;
    u8 dir = GetDpadDirection(gInput);

    k = *(struct keys *)&gKeys;
    if (k.pressed & A_BUTTON) {
        StartStroke();
        return;
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
        StartSpin();
    if (dir == 0 && tilt == 6) {
        SetMode(5);
        SetTargetAnim((SpriteObj *)target, 0x20);
        mode = dir;
    }
    CheckTurn();
}

void PlayerCtrl::StateStroke()
{
    void *inp = gInput;
    struct keys k = *(struct keys *)&gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON) {
        StartSpin();
        return;
    }
    if (target->animDone || gRoomFrameCount > deadline) {
        u8 dir = GetDpadDirection(inp);

        if (kp->pressed & A_BUTTON)
            StartStroke();
        else if (dir)
            ResetMode(this);
        else if (tilt == 6) {
            SetMode(5);
            SetTargetAnim((SpriteObj *)target, 0x20);
        } else
            ResetMode(this);
    }
    CheckTurn();
}

void PlayerCtrl::StateSpin()
{
    u8 dir = GetDpadDirection(gInput);

    if (++timer >= timerMax || target->animDone) {
        gPlayer->bounce = 0;
        spinCooldown = 0xC;
        if (dir == 0) {
            ResetMode(this);
            timerMax = 1;
        } else {
            ResetMode(this);
        }
    }
    CheckTurn();
}

void PlayerCtrl::StateTurn()
{
    struct keys k = *(struct keys *)&gKeys;
    s32 frame;

    if (timerMax != 0 && ++timer >= timerMax) {
        spinCooldown = 0xC;
        timerMax = 0;
        switch (mode) {
        case 7:
            frame = target->frame;
            mode = 6;
            SetTargetAnim((SpriteObj *)target, gPlayerCtrlModeAnimRows[6][tilt].anim);
            ClampFrame(target, frame);
            break;
        case 5:
            frame = target->frame;
            mode = 4;
            SetTargetAnim((SpriteObj *)target, gPlayerCtrlModeAnimRows[4][tilt].anim);
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
        if (mode == 4)
            mode = 5;
        else
            mode = 7;
        SetTargetAnim((SpriteObj *)target, gPlayerCtrlModeAnimRows[mode][tilt].anim);
        StartSpin();
        ClampFrame(target, frame);
        return;
    }
    if (k.pressed & A_BUTTON)
        StartStroke();
    if (target->animDone == 0)
        return;
    switch (mode) {
    case 4:
        if (target->mirror.sbits.flipX < 0)
            target->mirror.bits.flipX = 0;
        ResetMode(this);
        break;
    case 6:
        SetFlipX(target, 1);
        ResetMode(this);
        break;
    case 5:
        if (target->mirror.sbits.flipX < 0)
            target->mirror.bits.flipX = 0;
        SetStateNow(this, 3, 3, CTRL_KEEP, CTRL_KEEP);
        break;
    case 7:
        SetFlipX(target, 1);
        SetStateNow(this, 3, 3, CTRL_KEEP, CTRL_KEEP);
        break;
    }
    motionXPending = 1;
}

void PlayerCtrl::StateSwimStart()
{
    void *inp = gInput;
    struct keys k = *(struct keys *)&gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON) {
        StartSpin();
        return;
    }
    if (target->animDone) {
        u8 dir = GetDpadDirection(inp);

        if (kp->pressed & A_BUTTON)
            StartStroke();
        else if (dir)
            ResetMode(this);
        else if (tilt == 6) {
            SetMode(5);
            SetTargetAnim((SpriteObj *)target, 0x20);
        }
    }
    CheckTurn();
}

void PlayerCtrl::StateStop()
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
        mode = 0;
        SetStateNow(this, 0, 0, 0, 0);
    }
    CheckTurn();
}

void PlayerCtrl::StateDead()
{
    SetDriftY(0, 5, 0);
    SetDriftNowX(0, 5, 0);
    if (target->animDone)
        MarkGone(target);
}

void PlayerCtrl::Attach(SpriteObj *owner)
{
    target = (struct player *)owner;
}

/* UNUSED */
void PlayerCtrl::StartMotionYFromSet(struct player *part, s32 idx)
{
    Ctrl::StartTargetMotionY((SpriteObj *)part,
                             &gPlayerCtrlMotionRecords[animSet->entries[idx][1]]);
}

/* UNUSED */
void PlayerCtrl::StartMotionXFromSet(struct player *part, s32 idx)
{
    Ctrl::StartTargetMotionX((SpriteObj *)part,
                             &gPlayerCtrlMotionRecords[animSet->entries[idx][0]].start);
}

void PlayerCtrl::SetState(s32 newState, s32 newMode, s32 newTimer, s32 newTimerMax)
{
    SetStateNow(this, newState, newMode, newTimer, newTimerMax);
}

void PlayerCtrl::SetDriftX(s32 start, s32 step, s32 target)
{
    SetDriftNowX(start, step, target);
}

/* UNUSED. The ramp step SetDriftX computes from the player's speed:
 * v^2 / 0x4000 + 4. */
s32 PlayerCtrl::GetDriftStep(s32 v)
{
    return v * v / 0x4000 + 4;
}

/* UNUSED */
void PlayerCtrl::ApplyTilt()
{
    ApplyLevel(this);
}

/* UNUSED */
void PlayerCtrl::StartSwim()
{
    SetState(1, 1, CTRL_KEEP, 0);
}

/* g++ stores gPlayerCtrlVtable and calls ~Ctrl (DestroyCtrl). */
PlayerCtrl::~PlayerCtrl()
{
}

/* Ctrl(), the vtable pointer, then Reset (action_ctrl.c's
 * ResetPlayerCtrl). */
PlayerCtrl::PlayerCtrl()
{
    Reset();
}

/* UNUSED */
void PlayerCtrl::ClearUnk14()
{
    unk_14 = 0;
}

/* UNUSED */
void PlayerCtrl::SetMotionYPending()
{
    motionYPending = 1;
}

/* UNUSED */
void PlayerCtrl::SetMotionXPending()
{
    motionXPending = 1;
}
