#include "core.h"
#include "player_ctrl.h"
#include "system.h"
#include "audio.h"
#include "player.h"
#include "gfx.h"
#include "objects.h"
#include "level.h"

/* GitHub issues #19 (its last raw function, CheckPlayerCtrlTurn) and #20
 * (0x08016128-0x08017524): the player-input controller class of
 * include/player_ctrl.h, method table gPlayerCtrlVtable. It drives the
 * diving Crash of the room-kind-1 (underwater) rooms: sprite bank 1 is
 * Crash with an air tank and flippers, `tilt` is his swim direction
 * (0 up, 6 level, 12 down) and ApplyPlayerCtrlSwimDrift blows the
 * bank-40 bubbles. States: 0 idle (floating, a slow bob), 1 swim,
 * 2 stroke (A), 3 spin (B/R), 4 turn round, 5 stop (back to idle),
 * 6 swim start (D-pad from idle), 7 dead.
 *
 * Built with the older compiler, tools/agbcc/bin/old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS) - see docs/matching/issue-20-player-ctrl.md.
 *
 * - UpdatePlayerCtrl (table slot +0x0C) is the per-frame update: D-pad
 *   up/down with auto-repeat (`repeat`) steps `tilt` (0..12) and
 *   re-applies the target's animation (ApplyLevel, whose out-of-line copy
 *   is ApplyPlayerCtrlTilt), then runs the per-state handler through
 *   gPlayerCtrlStateFuncs, a table of gcc 2.x pointer-to-member-functions:
 *   states 0..7 are PlayerCtrlStateIdle, PlayerCtrlStateSwim, PlayerCtrlStateStroke, PlayerCtrlStateSpin,
 *   PlayerCtrlStateTurn, PlayerCtrlStateStop, PlayerCtrlStateSwimStart, PlayerCtrlStateDead.
 * - PlayerCtrlHandleEvent (+0x14) is the message handler, AttachPlayerCtrl (+0x1C) sets
 *   the target, DestroyPlayerCtrl (+0x4C) is the destructor and InitPlayerCtrl the
 *   constructor (called from play_room.c).
 * - SetPlayerCtrlState sets the mode through the method table (+0x20/+0x50,
 *   called through the _call_via_r2/_call_via_r3 `_call_via_rN` thunks) and
 *   picks the animation from gPlayerCtrlModeAnimRows[mode][tilt].
 * - SetPlayerSwimDriftX writes the player's +0x48/+0x4C/+0x50 record from its
 *   speed, like swim_ctrl_drift.c's SetPlayerSwimDriftY does for +0x54..+0x5C.
 *
 * Several functions are inline helpers in the original (C++ inline
 * methods): SetState/SetPlayerCtrlState, ResetMode/StartPlayerCtrlSwim,
 * SetPlayerRecord/SetPlayerSwimDriftX and ApplyLevel/ApplyPlayerCtrlTilt are each inlined
 * at some call sites and also emitted out of line.
 *
 * UNUSED - no reference in asm/, src/ or data/ and no Thumb pointer in the
 * ROM: ApplyPlayerCtrlMotion, StartPlayerCtrlMotionYFromSet, StartPlayerCtrlMotionXFromSet, sub_8017330, ApplyPlayerCtrlTilt,
 * StartPlayerCtrlSwim, sub_801750C, SetPlayerCtrlMotionYPending, SetPlayerCtrlMotionXPending. Matched anyway. */

/* The held/pressed key words of gKeys. The zero-length array
 * makes the struct BLKmode, so a local copy lives on the stack (the ROM
 * reads `pressed` back with `ldrh [sp, #2]`); without it gcc keeps the
 * copy in a register. */
struct keys
{
    u16 held;
    u16 pressed;
    u8 pad[0];
};

extern void *gInput;
extern struct keys gKeys;
extern u32 gRoomFrameCount;
extern void *gAudioContext;
extern void *gLevelState;
extern void *gPaletteCache;
extern u8 *gEntityFlags;
extern struct pctrl_target *gPlayer;

typedef void (*pctrl_fn1)(void *self, s32 a);
typedef void (*pctrl_fn2)(void *self, struct pctrl_target *t, s32 a);
typedef void (*pctrl_fn0)(void *self);

/* Virtual calls. Plain-brace macros on purpose: a do/while(0) wrapper
 * emits loop notes that change what CSE and cross-jumping do, and a
 * ({ }) statement expression leaves a USE insn that blocks cross-jumping
 * (see docs/matching/issue-20-player-ctrl.md). */
#define SET_MODE(obj, a)                                                        \
    {                                                                          \
        struct pctrl_method *_m = &(obj)->vtable->setMode;                     \
        ((pctrl_fn1)_m->fn)((u8 *)(obj) + _m->thisOffset, (a));                \
    }
#define SET_ANIM(obj, t, a)                                                    \
    {                                                                          \
        struct pctrl_method *_m = &(obj)->vtable->setAnim;                     \
        ((pctrl_fn2)_m->fn)((u8 *)(obj) + _m->thisOffset, (t), (a));           \
    }

#define KEEP 0x7FFFFFFF

static inline u8 LevelAnim(struct player_ctrl *self)
{
    return gPlayerCtrlModeAnimRows[self->mode][self->tilt].anim;
}

/* the out-of-line copy is SetPlayerCtrlState */
static inline void SetState(struct player_ctrl *self, s32 a, s32 mode, s32 timer, s32 timerMax)
{
    SET_MODE(self, a);
    self->mode = mode;
    SET_ANIM(self, self->target, LevelAnim(self));
    if (timer != KEEP)
        self->timer = timer;
    if (timerMax != KEEP)
        self->timerMax = timerMax;
}

/* the out-of-line copy is StartPlayerCtrlSwim */
static inline void ResetMode(struct player_ctrl *self)
{
    SetPlayerCtrlState(self, 1, 1, KEEP, 0);
}

/* The value arrives as a (constant-propagated) inline parameter, so the
 * bitfield store is the generic clear-then-or, as in the ROM. */
static inline void SetFlipX(struct pctrl_target *t, u32 value)
{
    t->f28.flipX = value;
}

static inline void SetHitAxes(struct pctrl_target *t, s32 value)
{
    t->hitAxes = value;
}

static inline void QueueMotionX(struct player_ctrl *self, s32 value)
{
    self->motionXPending = 1;
    self->motionX = value;
}

static inline void QueueMotionY(struct player_ctrl *self, s32 value)
{
    self->motionYPending = 1;
    self->motionY = value;
}

void CheckPlayerCtrlTurn(struct player_ctrl *self)
{
    u8 dir = GetDpadDirection(gInput);

    switch (self->state)
    {
    case 0 ... 3:
    case 5 ... 6:
        self->target->f28.flipY = 0;
        if (self->target->f28.flipX && (dir == 4 || dir == 6 || dir == 8))
        {
            self->target->f28.flipX = 0;
            if (self->state != 3)
            {
                SetPlayerCtrlState(self, 4, 4, KEEP, KEEP);
                self->timerMax = 0;
            }
            else
            {
                SetPlayerCtrlState(self, 4, 5, KEEP, KEEP);
            }
            self->unk_26 = 0;
        }
        else if (!(self->target->f28.flipX & 1) && (dir == 3 || dir == 5 || dir == 7))
        {
            if (self->state != 3)
            {
                SetPlayerCtrlState(self, 4, 6, KEEP, KEEP);
                self->timerMax = 0;
            }
            else
            {
                SetPlayerCtrlState(self, 4, 7, KEEP, KEEP);
            }
            self->unk_26 = 0;
        }
        break;
    }
}

void PlayerCtrlHandleEvent(struct player_ctrl *self, s32 unused, s32 msg, s32 arg)
{
    switch (msg)
    {
    case 12:
    {
        s32 side = arg & 3;

        if (side == 2)
        {
            if (self->target->f28.flipX)
                QueueMotionX(self, 0);
        }
        else if (side == 1)
        {
            if (!self->target->f28.flipX)
                QueueMotionX(self, 0);
        }
        else
        {
            break;
        }
        self->target->speedX = 0;
        break;
    }
    case 5:
        PlayerCtrlKillPlayer(self, 0x2D);
        break;
    case 3:
        PlayerCtrlKillPlayer(self, 0x2B);
        break;
    case 4:
        PlayerCtrlKillPlayer(self, 0x2C);
        break;
    case 1:
    case 6:
    case 10:
        PlayerCtrlKillPlayer(self, 0x2E);
        break;
    case 13:
        break;
    }
}

void PlayerCtrlKillPlayer(struct player_ctrl *self, s32 anim)
{
    PlaySfx(gAudioContext, 0x1B, 0x100);
    SET_MODE(self, 7);
    SET_ANIM(self, self->target, anim);
    self->target->flag7 = 0;
    self->target->flag6 = 0;
    self->target->dead = 1;
    LoseLife(gLevelState);
    LoadPaletteSlot(gPaletteCache, self->target->slot,
                self->target->anim->records[self->target->tag].paletteId);
}

/* Runs the pointer-to-member handler for the current state. */
#define PMF_DISPATCH(self)                                                      \
    {                                                                          \
        s32 idx = gPlayerCtrlStateFuncs[(self)->state].index;                   \
        struct vtable_slot e;                                                  \
        void *fn;                                                              \
        s32 d;                                                                 \
        s32 adj;                                                               \
                                                                               \
        if (idx > 0)                                                           \
        {                                                                      \
            e = (*(struct vtable_slot **)((u8 *)(self) + gPlayerCtrlStateFuncs[(self)->state].u.vtableOffset))[idx - 1]; \
            fn = e.fn;                                                         \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            fn = gPlayerCtrlStateFuncs[(self)->state].u.fn;                     \
        }                                                                      \
        d = gPlayerCtrlStateFuncs[(self)->state].thisOffset;                   \
        if (idx > 0)                                                           \
            adj = e.delta + d;                                                 \
        else                                                                   \
            adj = d;                                                           \
        ((pctrl_fn0)fn)((u8 *)(self) + adj);                                   \
    }

/* the out-of-line copy is SetPlayerSwimDriftX */
static inline void SetPlayerRecord(s32 a, s32 b, s32 c)
{
    struct pctrl_target *p = gPlayer;
    s32 v = p->speedX;
    s32 t = v * v / 0x4000 + 4;
    s32 signV;
    s32 signC;
    s32 absV;
    s32 absC;

    signV = v >> 31;
    absV = (v ^ signV) - signV;
    signC = c >> 31;
    absC = (c ^ signC) - signC;
    if (absV > absC)
    {
        p->rampXStart = a;
        p->rampXStep = t;
    }
    else if (v * c < 0)
    {
        s32 sum = t + b;

        p->rampXStart = a;
        p->rampXStep = sum;
    }
    else
    {
        p->rampXStart = a;
        p->rampXStep = b;
    }
    p->rampXTarget = c;
}

/* Sets bit `id` of the gEntityFlags+0x108 bitmap. Kept a
 * do/while(0) macro: its loop notes stop CSE from reusing the id already
 * loaded for the caller's 0xFFFF test, so the ROM's reload comes out
 * naturally, and the word index is gcc's signed division of it. */
#define SET_ID_BIT(idExpr)                                                     \
    do                                                                         \
    {                                                                          \
        s32 _id = (idExpr);                                                    \
        u8 *_base = gEntityFlags;                                         \
        s32 _word = _id / 32;                                                  \
        s32 _off = _word * 4;                                                  \
        u32 *_slot = (u32 *)(_base + 0x108);                                   \
                                                                               \
        _slot = (u32 *)((u8 *)_slot + _off);                                   \
        *_slot |= 1 << (_id - _word * 32);                                     \
    } while (0)

/* "Mark gone": MarkEntityGone's sequence (graphics.c), inlined - set flags
 * bit 0, then unless the id is 0xFFFF set its bit in the bitmap. */
static inline void MarkGone(struct pctrl_target *t)
{
    t->gone = 1;
    if (t->id != 0xFFFF)
        SET_ID_BIT(t->id);
}

static inline void ClampFrame(struct pctrl_target *t, s32 frame)
{
    s32 n = t->anim->records[t->tag].frames;

    if (frame >= n)
        frame = n - 1;
    t->frame = frame;
}

static inline void RestoreFrame(struct pctrl_target *t, s32 frame, s32 f34)
{
    ClampFrame(t, frame);
    t->stepTimer = f34;
}

/* Re-applies the animation for the current mode/level; the out-of-line
 * copy is ApplyPlayerCtrlTilt. */
static inline void ApplyLevel(struct player_ctrl *self)
{
    struct pctrl_target *t = self->target;
    u8 *tag = &t->tag;

    if (*tag != 0x20 && *tag != 0x1D && *tag != 0x1F)
    {
        s32 frame = t->frame;
        s32 f34 = t->stepTimer;

        *tag = gPlayerCtrlModeAnimRows[self->mode][self->tilt].anim;
        ResetSpriteFrameTimer(t);
        ResetSpriteFrameIndex(t);
        SetSpriteAnimDone(t, 0);
        RestoreFrame(self->target, frame, f34);
    }
    else
    {
        switch (self->mode)
        {
        case 7:
        case 14 ... 18:
        case 23:
        case 33 ... 37:
        case 41:
        {
            s32 frame = self->target->frame;
            s32 f34 = self->target->stepTimer;

            ResetMode(self);
            RestoreFrame(self->target, frame, f34);
            break;
        }
        default:
            ResetMode(self);
            break;
        }
    }
}

void UpdatePlayerCtrl(struct player_ctrl *self)
{
    if (self->state == 7)
    {
        PMF_DISPATCH(self);
    }
    else
    {
        u32 keys;
        u8 dir;
        void *inp;

        if (self->spinCooldown)
            self->spinCooldown--;
        inp = gInput;
        keys = *(u32 *)&gKeys; /* the whole word, held keys low */
        dir = GetDpadDirection(inp);

        if (!(keys & (DPAD_UP | DPAD_DOWN)) && self->state != 2)
        {
            if (self->repeat && self->tilt != 6)
                self->repeat = self->repeat - 1;
            else
                self->repeat = 3;
            if (self->repeat == 0 && self->tilt != 6)
            {
                if (self->tilt <= 5)
                    self->tilt++;
                if (self->tilt > 6)
                    self->tilt--;
                if (self->tilt != 6)
                    self->repeat = 3;
                ApplyLevel(self);
            }
        }
        else if (self->state != 2 || dir != 0)
        {
            if (self->repeat == 0 || --self->repeat == 0)
            {
                self->repeat = 3;
                if (keys & DPAD_UP)
                {
                    if (keys & DPAD_SIDEWAYS)
                    {
                        if (self->tilt > 3)
                        {
                            self->tilt--;
                            ApplyLevel(self);
                        }
                        else if (self->tilt <= 2)
                        {
                            self->tilt++;
                            ApplyLevel(self);
                        }
                    }
                    else if (self->tilt != 0)
                    {
                        self->tilt--;
                        ApplyLevel(self);
                    }
                }
                else if (keys & DPAD_DOWN)
                {
                    if (keys & DPAD_SIDEWAYS)
                    {
                        if (self->tilt <= 8)
                        {
                            self->tilt++;
                            ApplyLevel(self);
                        }
                        else if (self->tilt > 9)
                        {
                            self->tilt--;
                            ApplyLevel(self);
                        }
                    }
                    else if (self->tilt <= 11)
                    {
                        self->tilt++;
                        ApplyLevel(self);
                    }
                }
            }
        }

        PMF_DISPATCH(self);
        if (self->target->hitMask & 3)
            self->target->speedX = 0;
        if (self->target->hitMask & 0xC)
            self->target->speedY = 0;
        SetHitAxes(self->target, 0);
        ApplyPlayerCtrlSwimDrift(self);
    }

    if (self->state == 3 || self->mode == 5 || self->mode == 7)
        self->target->kind = 0x13;
    else
        self->target->kind = 1;
}

/* The register pins are load-bearing (docs/workflow.md step 7): the ROM
 * loads the record index into r1 and scales it into r0 before adding the
 * table base into r2; every unpinned form tried scales straight into r2. */
/* UNUSED */
void ApplyPlayerCtrlMotion(struct player_ctrl *self)
{
    if (self->motionXPending == 1)
    {
        struct motion_rec *rec;

        self->motionXPending = 0;
        {
            register u32 i asm("r1") = self->animSet->entries[self->motionX].a;
            register u32 off asm("r0") = i * sizeof(struct motion_rec);

            rec = (struct motion_rec *)(off + (u32)gPlayerCtrlMotionRecords);
        }
        StartCtrlTargetMotionX(self, self->target, (s32 *)rec);
    }
    if (self->motionYPending == 1)
    {
        struct motion_rec *rec;

        self->motionYPending = 0;
        {
            register u32 i asm("r1") = self->animSet->entries[self->motionY].b;
            register u32 off asm("r0") = i * sizeof(struct motion_rec);

            rec = (struct motion_rec *)(off + (u32)gPlayerCtrlMotionRecords);
        }
        StartCtrlTargetMotionY(self, self->target, (struct vec3 *)rec);
    }
}

void PlayerCtrlStateIdle(struct player_ctrl *self)
{
    struct keys k;
    u8 dir;
    u8 count;
    void *inp = gInput;

    k = gKeys;
    dir = GetDpadDirection(inp);
    count = ++self->idleTimer;
    if (count == 30)
    {
        QueueMotionY(self, 1);
    }
    else if (count > 59)
    {
        QueueMotionY(self, 2);
        self->idleTimer = 0;
    }
    if (self->target->animDone)
        SET_ANIM(self, self->target, 0x1F);
    if (k.pressed & A_BUTTON)
    {
        StartPlayerCtrlStroke(self);
    }
    else if (k.pressed & (B_BUTTON | R_BUTTON))
    {
        StartPlayerCtrlSpin(self);
    }
    else if (dir)
    {
        SET_MODE(self, 6);
        SET_ANIM(self, self->target, 0x1D);
        QueueMotionX(self, 0xC);
    }
    self->unk_26 = 0;
    CheckPlayerCtrlTurn(self);
}

void PlayerCtrlStateSwim(struct player_ctrl *self)
{
    struct keys k;
    u8 dir = GetDpadDirection(gInput);

    k = gKeys;
    if (k.pressed & A_BUTTON)
    {
        StartPlayerCtrlStroke(self);
        return;
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
        StartPlayerCtrlSpin(self);
    if (dir == 0 && self->tilt == 6)
    {
        SET_MODE(self, 5);
        SET_ANIM(self, self->target, 0x20);
        self->mode = dir;
    }
    CheckPlayerCtrlTurn(self);
}

void PlayerCtrlStateStroke(struct player_ctrl *self)
{
    void *inp = gInput;
    struct keys k = gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON)
    {
        StartPlayerCtrlSpin(self);
        return;
    }
    if (self->target->animDone || gRoomFrameCount > self->deadline)
    {
        u8 dir = GetDpadDirection(inp);

        if (kp->pressed & A_BUTTON)
            StartPlayerCtrlStroke(self);
        else if (dir)
            ResetMode(self);
        else if (self->tilt == 6)
        {
            SET_MODE(self, 5);
            SET_ANIM(self, self->target, 0x20);
        }
        else
            ResetMode(self);
    }
    CheckPlayerCtrlTurn(self);
}

void PlayerCtrlStateSpin(struct player_ctrl *self)
{
    u8 dir = GetDpadDirection(gInput);

    if (++self->timer >= self->timerMax || self->target->animDone)
    {
        gPlayer->unk_92 = 0;
        self->spinCooldown = 0xC;
        if (dir == 0)
        {
            ResetMode(self);
            self->timerMax = 1;
        }
        else
        {
            ResetMode(self);
        }
    }
    CheckPlayerCtrlTurn(self);
}

void PlayerCtrlStateTurn(struct player_ctrl *self)
{
    struct keys k = gKeys;
    s32 frame;

    if (self->timerMax != 0 && ++self->timer >= self->timerMax)
    {
        self->spinCooldown = 0xC;
        self->timerMax = 0;
        switch (self->mode)
        {
        case 7:
            frame = self->target->frame;
            self->mode = 6;
            SET_ANIM(self, self->target, gPlayerCtrlModeAnimRows[6][self->tilt].anim);
            ClampFrame(self->target, frame);
            break;
        case 5:
            frame = self->target->frame;
            self->mode = 4;
            SET_ANIM(self, self->target, gPlayerCtrlModeAnimRows[4][self->tilt].anim);
            ClampFrame(self->target, frame);
            break;
        }
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
    {
        if (self->spinCooldown != 0 || self->timerMax != 0)
            return;
        self->timer = 0;
        self->timerMax = 0x18;
        frame = self->target->frame;
        if (self->mode == 4)
            self->mode = 5;
        else
            self->mode = 7;
        SET_ANIM(self, self->target, gPlayerCtrlModeAnimRows[self->mode][self->tilt].anim);
        StartPlayerCtrlSpin(self);
        ClampFrame(self->target, frame);
        return;
    }
    if (k.pressed & A_BUTTON)
        StartPlayerCtrlStroke(self);
    if (self->target->animDone == 0)
        return;
    switch (self->mode)
    {
    case 4:
        if (self->target->f28.flipX)
            self->target->f28.flipX = 0;
        ResetMode(self);
        break;
    case 6:
        SetFlipX(self->target, 1);
        ResetMode(self);
        break;
    case 5:
        if (self->target->f28.flipX)
            self->target->f28.flipX = 0;
        SetState(self, 3, 3, KEEP, KEEP);
        break;
    case 7:
        SetFlipX(self->target, 1);
        SetState(self, 3, 3, KEEP, KEEP);
        break;
    }
    self->motionXPending = 1;
}

void PlayerCtrlStateSwimStart(struct player_ctrl *self)
{
    void *inp = gInput;
    struct keys k = gKeys;
    struct keys *kp = &k;

    if (kp->pressed & B_BUTTON)
    {
        StartPlayerCtrlSpin(self);
        return;
    }
    if (self->target->animDone)
    {
        u8 dir = GetDpadDirection(inp);

        if (kp->pressed & A_BUTTON)
            StartPlayerCtrlStroke(self);
        else if (dir)
            ResetMode(self);
        else if (self->tilt == 6)
        {
            SET_MODE(self, 5);
            SET_ANIM(self, self->target, 0x20);
        }
    }
    CheckPlayerCtrlTurn(self);
}

void PlayerCtrlStateStop(struct player_ctrl *self)
{
    struct keys k = gKeys;

    if (k.pressed & A_BUTTON)
    {
        StartPlayerCtrlStroke(self);
        return;
    }
    if (k.pressed & (B_BUTTON | R_BUTTON))
    {
        StartPlayerCtrlSpin(self);
        return;
    }
    if (self->target->animDone)
    {
        self->mode = 0;
        SetState(self, 0, 0, 0, 0);
    }
    CheckPlayerCtrlTurn(self);
}

void PlayerCtrlStateDead(struct player_ctrl *self)
{
    SetPlayerSwimDriftY(0, 5, 0);
    SetPlayerRecord(0, 5, 0);
    if (self->target->animDone)
        MarkGone(self->target);
}

void AttachPlayerCtrl(struct player_ctrl *self, struct pctrl_target *target)
{
    self->target = target;
}

/* UNUSED */
void StartPlayerCtrlMotionYFromSet(struct player_ctrl *self, struct pctrl_target *target, s32 idx)
{
    StartCtrlTargetMotionY(self, target, (struct vec3 *)&gPlayerCtrlMotionRecords[self->animSet->entries[idx].b]);
}

/* UNUSED */
void StartPlayerCtrlMotionXFromSet(struct player_ctrl *self, struct pctrl_target *target, s32 idx)
{
    StartCtrlTargetMotionX(self, target, (s32 *)&gPlayerCtrlMotionRecords[self->animSet->entries[idx].a]);
}

void SetPlayerCtrlState(struct player_ctrl *self, s32 a, s32 mode, s32 timer, s32 timerMax)
{
    SetState(self, a, mode, timer, timerMax);
}

void SetPlayerSwimDriftX(s32 a, s32 b, s32 c)
{
    SetPlayerRecord(a, b, c);
}

/* UNUSED */
s32 sub_8017330(s32 v)
{
    return v * v / 0x4000 + 4;
}

/* UNUSED */
void ApplyPlayerCtrlTilt(struct player_ctrl *self)
{
    ApplyLevel(self);
}

/* UNUSED */
void StartPlayerCtrlSwim(struct player_ctrl *self)
{
    SetPlayerCtrlState(self, 1, 1, KEEP, 0);
}

void DestroyPlayerCtrl(struct player_ctrl *self, s32 flags)
{
    self->vtable = (struct pctrl_vtable *)gPlayerCtrlVtable;
    DestroyCtrl(self, flags);
}

struct player_ctrl *InitPlayerCtrl(struct player_ctrl *self)
{
    InitCtrl(self);
    self->vtable = (struct pctrl_vtable *)gPlayerCtrlVtable;
    ResetPlayerCtrl(self);
    return self;
}

/* UNUSED */
void sub_801750C(struct player_ctrl *self)
{
    self->unk_14 = 0;
}

/* UNUSED */
void SetPlayerCtrlMotionYPending(struct player_ctrl *self)
{
    self->motionYPending = 1;
}

/* UNUSED */
void SetPlayerCtrlMotionXPending(struct player_ctrl *self)
{
    self->motionXPending = 1;
}
