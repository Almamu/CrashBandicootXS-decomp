#include "core.h"
#include "audio.h"
#include "player.h"
#include "camera_lead.h"
#include "menus.h"
#include "gfx.h"
#include "objects.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "entity_bits.h"

/* GitHub issue #21: 0x08017524-0x08017A44, the whole tail of the former
 * asm/code_3_2_17_16048.s.
 *
 * The first six functions are byte accessors of the underwater player
 * controller (include/player_ctrl.h, swim_ctrl.c right before
 * them): its queued X/Y motion entries (`+0x24`/`+0x25`) and their
 * "pending" flags (`+0x2C`/`+0x2D`).
 *
 * The other 19 are one self-contained actor-part subclass,
 * `struct input_ctrl`, whose method table is `gInputCtrlVtable`. It is
 * the controller play_room.c attaches in room kind 2, where the player
 * uses sprite bank 2 (Crash riding a hover vehicle; anim 1 is it
 * blowing up)
 * (constructor `CreateInputCtrl` - called from play_room.c - destructor
 * `DestroyInputCtrl`; every other slot it calls through is a base-class
 * `sub_800B6xx`/`sub_800B8xx` function). Each frame (`UpdateInputCtrl`, table
 * slot +0x0C) it reads the held D-pad bits from `gKeys` and
 * picks animation pairs for its target (`+0x10`) through
 * `gInputCtrlMotionRecords`'s 12-byte records: up/down select one channel
 * (`motionY`), left/right the other (`motionX`), which also sets a speed-like
 * value at the camera lead's `+0x78` (`+0x1C`, spawned on demand by
 * `InputCtrlStateStart`). Once the target passes the level's right edge
 * (`gLevelLayers`'s layer 0 width, less 0xA00) the child is marked
 * gone and `RequestRoomExit` is signalled. It then dispatches the current
 * `state` through `gInputCtrlStateFuncs`, a table of gcc 2.x
 * pointer-to-member-functions: state 0 `InputCtrlStateStart`, 1 `InputCtrlStateRide`,
 * 2 `InputCtrlStateUnusedRide`, 3 `InputCtrlStateDead`. That call sequence - and the
 * `_call_via_r1`/`AD80`/`AD84` "call via r1/r2/r3" trampolines used for
 * every virtual call - is what gcc's C++ front end emits, so this object
 * was very likely written in C++.
 *
 * UNUSED - no `bl`/`.4byte` reference in the asm/ sources or any .c file under
 * src/, and no Thumb pointer anywhere in the ROM: `ClearPlayerCtrlMotionYPending`,
 * `ClearPlayerCtrlMotionXPending`, `IsPlayerCtrlMotionYPending`, `IsPlayerCtrlMotionXPending`, `QueuePlayerCtrlMotionY`,
 * `QueuePlayerCtrlMotionX`, `SetInputCtrlMotionYPending`, `SetInputCtrlMotionXPending`, `CancelInputCtrlMotionY`,
 * `CancelInputCtrlMotionX`, `IsInputCtrlMotionYPending`. Matched anyway.
 *
 * Built with the older compiler, tools/agbcc/bin/old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS), like swim_ctrl.c before it. Under old_agbcc the
 * whole file is plain C: no register pins, `volatile` re-reads or empty
 * `asm` barriers (those were needed to imitate old_agbcc's output with the
 * current agbcc).
 *
 * Matching notes (details in docs/matching/archive/issue-21-input-ctrl.md): the
 * virtual-call macros take the method-table entry's address once; the
 * `QueueMotionX`/`QueueMotionY`/`SetCameraLeadSpeed` inline helpers reproduce the ROM
 * evaluating the stored constant before the store's own loads; the
 * "mark actor gone" bitmap sequence (MarkEntityGone's, inlined twice) is
 * entity_bits.h's ENTITY_MARK_GONE - its word index comes from a signed
 * division of the zero-extended id. */

/* include/player_ctrl.h's `struct player_ctrl`, as far as these
 * accessors see it */
struct pctrl_motion_queue {
    u8 unk_00[0x24];
    u8 motionX; // 0x24
    u8 motionY; // 0x25
    u8 unk_26[6];
    u8 motionXPending; // 0x2C
    u8 motionYPending; // 0x2D
};

extern s32 _call_via_r2(void *self, s32 arg, void *fn);
extern s32 _call_via_r3(void *self, void *arg1, void *arg2, void *fn);

/* A virtual call as gcc 2.x lowers it: take the method-table entry's
 * address once, then read its `this` adjustment and function from it.
 * `if (1) { } else (void)0` rather than `do { } while (0)`, whose loop
 * notes are not neutral under this compiler (include/actor_self.h). */
#define CTRL_CALL2(obj, m, a)                                                  \
    if (1) {                                                                   \
        struct actor_method *_m = &(obj)->vtable->m;                            \
        _call_via_r2((u8 *)(obj) + _m->thisOffset, (a), _m->fn);                \
    } else (void)0
#define CTRL_CALL3(obj, m, a, b)                                               \
    if (1) {                                                                   \
        struct actor_method *_m = &(obj)->vtable->m;                            \
        _call_via_r3((u8 *)(obj) + _m->thisOffset, (a), (b), _m->fn);           \
    } else (void)0

static inline void SetCameraLeadSpeed(struct input_ctrl *self, s32 speed)
{
    self->cameraLead->targetOffset = speed;
}

static inline void QueueMotionX(struct input_ctrl *self, u8 anim)
{
    self->motionXPending = 1;
    self->motionX = anim;
}

static inline void QueueMotionY(struct input_ctrl *self, u8 anim)
{
    self->motionYPending = 1;
    self->motionY = anim;
}

void ClearPlayerCtrlMotionYPending(struct pctrl_motion_queue *self)
{
    self->motionYPending = 0;
}

void ClearPlayerCtrlMotionXPending(struct pctrl_motion_queue *self)
{
    self->motionXPending = 0;
}

u8 IsPlayerCtrlMotionYPending(struct pctrl_motion_queue *self)
{
    return self->motionYPending;
}

u8 IsPlayerCtrlMotionXPending(struct pctrl_motion_queue *self)
{
    return self->motionXPending;
}

void QueuePlayerCtrlMotionY(struct pctrl_motion_queue *self, u8 value)
{
    self->motionYPending = 1;
    self->motionY = value;
}

void QueuePlayerCtrlMotionX(struct pctrl_motion_queue *self, u8 value)
{
    self->motionXPending = 1;
    self->motionX = value;
}


void InputCtrlKillPlayer(struct input_ctrl *self, void *arg)
{
    PlaySfx(gAudioContext, SFX_PLAYER_HURT, 0x100);
    CTRL_CALL2(self, setMode, 3);
    CTRL_CALL3(self, setAnim, self->target, arg);
    self->target->flags.bits.flag7 = 0;
    self->target->flags.bits.flag6 = 0;
    self->target->dead = 1;
    LoseLife(gLevelState);
    {
        void *cache = gPaletteCache;
        struct player *t = self->target;

        LoadPaletteSlot(cache, t->slot, t->anim->records[t->tag].paletteId);
    }
}

void InputCtrlStateStart(struct input_ctrl *self)
{
    SetInputCtrlModeAnim(self, 1, NULL, 0, 0);
    self->motionXPending = 1;
    self->motionX = 1;
    self->motionYPending = 1;
    self->motionY = 0;
    self->dirState = 0;
    if (self->cameraLead == NULL) {
        self->cameraLead = CreateCameraLead(OperatorNew(0x80));
        AddToPartList(gCollidableList, self->cameraLead);
    }
    ResetCameraLead(self->cameraLead);
}


void UpdateInputCtrl(struct input_ctrl *self)
{
    if (self->state != 3) {
        u32 keys;
        s32 x = self->target->x;

        if (x > (gLevelLayers->layer0->widthPx << 8) - 0xA00) {
            {
                struct follow_child *c = self->cameraLead;

                ENTITY_MARK_GONE(c->flags.bits.gone, c->field_08);
            }
            self->cameraLead = NULL;
            RequestRoomExit();
        }

        keys = gKeys.all;
        if ((keys & DPAD_UP) && self->dirState != 1) {
            QueueMotionY(self, 3);
            self->dirState = 1;
        } else {
            if ((keys & DPAD_DOWN) && self->dirState != 2) {
                QueueMotionY(self, 5);
                self->dirState = 2;
            } else if (!(keys & (DPAD_UP | DPAD_DOWN))) {
                QueueMotionY(self, 0);
                self->dirState = 0;
            }
        }

        if ((keys & DPAD_LEFT) && self->flag20) {
            QueueMotionX(self, 7);
            SetCameraLeadSpeed(self, 0x3200);
            if (++self->timer > 30) {
                self->flag20 = 0;
                self->timer = 10;
            }
        } else if (keys & DPAD_RIGHT) {
            QueueMotionX(self, 8);
            SetCameraLeadSpeed(self, 0xA00);
        } else if (!(keys & DPAD_SIDEWAYS) || ((keys & DPAD_LEFT) && !self->flag20)) {
            SetCameraLeadSpeed(self, 0x1E00);
            QueueMotionX(self, 1);
        }

        if (!self->flag20 && --self->timer < 0) {
            self->timer = 0;
            if (!(keys & DPAD_LEFT))
                self->flag20 = 1;
        }
    }

    {
        s32 idx = gInputCtrlStateFuncs[self->state].index;
        struct vtable_slot e;
        void *fn;

        if (idx > 0) {
            // clang-format off
            e = (*(struct vtable_slot **)((u8 *)self +
                    gInputCtrlStateFuncs[self->state].u.vtableOffset))[idx - 1];
            // clang-format on
            fn = e.fn;
        } else {
            fn = gInputCtrlStateFuncs[self->state].u.fn;
        }
        {
            s32 d = gInputCtrlStateFuncs[self->state].thisOffset;
            s32 adj;

            if (idx > 0)
                adj = e.delta + d;
            else
                adj = d;
            _call_via_r2((u8 *)self + adj, d, fn);
        }
    }
    ApplyInputCtrlMotion(self);
}

void ApplyInputCtrlMotion(struct input_ctrl *self)
{
    if (self->motionXPending == 1) {
        u8 *rec = (u8 *)gInputCtrlMotionRecords + self->animSet->entries[self->motionX].a * 12;

        if (self->motionXKeepSpeed)
            CTRL_CALL3(self, setMotionX, self->target, rec);
        else
            CTRL_CALL3(self, startMotionX, self->target, rec);
        self->motionXPending = 0;
        self->motionXKeepSpeed = 0;
    }
    if (self->motionYPending == 1) {
        u8 *rec = (u8 *)gInputCtrlMotionRecords + self->animSet->entries[self->motionY].b * 12;

        if (self->motionYKeepSpeed)
            CTRL_CALL3(self, setMotionY, self->target, rec);
        else
            CTRL_CALL3(self, startMotionY, self->target, rec);
        self->motionYPending = 0;
        self->motionYKeepSpeed = 0;
    }
}

void SetInputCtrlModeAnim(struct input_ctrl *self, s32 mode, void *arg, s32 unused3, s32 unused4)
{
    CTRL_CALL2(self, setMode, mode);
    CTRL_CALL3(self, setAnim, self->target, arg);
}

void InputCtrlStateDead(struct input_ctrl *self)
{
    struct player *t = self->target;

    if (t->animDone)
        ENTITY_MARK_GONE(t->flags.bits.gone, t->id);
}

/* gInputCtrlStateFuncs[2]: once the target's animation ends, goes back to
 * state 1 on animation 0 and queues X motion entry 2 (the same cruise
 * record as entry 1, restarted). Only InputCtrlStateRide sets state 2, and
 * only when animation 0 ends, which never happens (see there), so this
 * state is never reached. */
void InputCtrlStateUnusedRide(struct input_ctrl *self)
{
    if (self->target->animDone) {
        SetInputCtrlModeAnim(self, 1, NULL, 0, 0);
        QueueMotionX(self, 2);
    }
}

/* gInputCtrlStateFuncs[1], the state the controller stays in for the whole
 * hover ride (ResetInputCtrl and InputCtrlStateStart select it; the D-pad
 * steering is in UpdateInputCtrl). It would switch to state 2 once the
 * target's animation ends, but the animation is 0, sprite bank 2's riding
 * loop (SPRITE_ANIM_LOOP), which never sets animDone. */
void InputCtrlStateRide(struct input_ctrl *self)
{
    if (self->target->animDone)
        SetInputCtrlModeAnim(self, 2, NULL, 0, 0);
}

void RestartInputCtrl(struct input_ctrl *self)
{
    SetInputCtrlModeAnim(self, 0, NULL, 0, 0);
    self->motionXPending = 1;
    self->motionX = 0;
    self->motionYPending = 1;
    self->motionY = 0;
}

void ResetInputCtrl(struct input_ctrl *self)
{
    self->state = 1;
    self->motionX = 0;
    self->motionY = 0;
    self->motionXPending = 1;
    self->motionYPending = 1;
    self->target = NULL;
    self->cameraLead = NULL;
    self->flag20 = 0;
}

void InputCtrlHandleEvent(struct input_ctrl *self, s32 arg1, s32 arg2)
{
    /* a non-literal lower bound keeps gcc from folding `>= 1` into
     * `> 0` (the ROM compares against 1) and from merging the two tests
     * into one unsigned range check */
    s32 lo = 1;

    if (arg2 >= lo && arg2 <= 4)
        InputCtrlKillPlayer(self, (void *)1);
}

void AttachInputCtrl(struct input_ctrl *self, struct player *target)
{
    self->target = target;
}

void DestroyInputCtrl(struct input_ctrl *self, s32 flags)
{
    self->vtable = (struct ctrl_vtable *)gInputCtrlVtable;
    DestroyCtrl(self, flags);
}

struct input_ctrl *CreateInputCtrl(struct input_ctrl *self)
{
    InitCtrl(self);
    self->vtable = (struct ctrl_vtable *)gInputCtrlVtable;
    ResetInputCtrl(self);
    return self;
}

void SetInputCtrlMotionYPending(struct input_ctrl *self)
{
    self->motionYPending = 1;
}

void SetInputCtrlMotionXPending(struct input_ctrl *self)
{
    self->motionXPending = 1;
}

void CancelInputCtrlMotionY(struct input_ctrl *self)
{
    self->motionYPending = 0;
    self->motionYKeepSpeed = 0;
}

void CancelInputCtrlMotionX(struct input_ctrl *self)
{
    self->motionXPending = 0;
    self->motionXKeepSpeed = 0;
}

u8 IsInputCtrlMotionYPending(struct input_ctrl *self)
{
    return self->motionYPending;
}
