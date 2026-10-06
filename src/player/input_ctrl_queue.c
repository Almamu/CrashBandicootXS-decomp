#include "core.h"
#include "player.h"
#include "objects.h"

/* GitHub issue #22, ROM 0x08017A44-0x08017AAC. Two groups of methods
 * share this file:
 *
 * - IsInputCtrlMotionXPending .. QueueInputCtrlMotionX (0x08017A44-
 *   0x08017A6C) finish the input_ctrl accessor run that ends
 *   input_ctrl.c (SetInputCtrlMotionYPending .. IsInputCtrlMotionYPending,
 *   0x08017A20-0x08017A40): they touch the input controller's motion
 *   queue (`struct input_ctrl`, player.h). Nothing calls them.
 * - BossCtrlHandleEvent/DestroyBossCtrl/CreateBossCtrl/GetCtrlTarget are
 *   the gBossCtrlVtable class (`struct boss_ctrl`, player.h), the
 *   controller base of the bosses: Mega-Mix (gMegaMixCtrlVtable), Tiny,
 *   the Neo Cortex fight's controller and Dingodile with his shield and
 *   rocket/stalactite. Its event slot keeps the event's msg and arg words
 *   at `+0x14`/`+0x18`; no subclass reads them (they leave `+0x14`-`+0x1B`
 *   alone). */

/* input_ctrl.motionXPending (`+0x17`). */
u8 IsInputCtrlMotionXPending(struct input_ctrl *self)
{
    return self->motionXPending;
}

/* Queues Y motion entry `val` (input_ctrl.motionY, `+0x15`), pending,
 * applied with the speed kept (motionYPending/motionYKeepSpeed). */
void QueueInputCtrlMotionYKeepSpeed(struct input_ctrl *self, u8 val)
{
    self->motionYKeepSpeed = 1;
    self->motionYPending = 1;
    self->motionY = val;
}

/* Queues X motion entry `val` (input_ctrl.motionX, `+0x14`), pending,
 * applied with the speed kept (motionXPending/motionXKeepSpeed). */
void QueueInputCtrlMotionXKeepSpeed(struct input_ctrl *self, u8 val)
{
    self->motionXKeepSpeed = 1;
    self->motionXPending = 1;
    self->motionX = val;
}

/* Queues Y motion entry `val` (motionY + motionYPending). */
void QueueInputCtrlMotionY(struct input_ctrl *self, u8 val)
{
    self->motionYPending = 1;
    self->motionY = val;
}

/* Queues X motion entry `val` (motionX + motionXPending). */
void QueueInputCtrlMotionX(struct input_ctrl *self, u8 val)
{
    self->motionXPending = 1;
    self->motionX = val;
}

/* gBossCtrlVtable's event slot (slot 2, CtrlHandleEvent in the base
 * class): stores the event's msg (`a`) and arg (`b`) at `+0x14`/`+0x18`.
 * The sender word `arg1` is unused. Nothing reads the stored words back. */
void BossCtrlHandleEvent(struct boss_ctrl *self, s32 arg1, s32 a, s32 b)
{
    (void)arg1;
    self->msg = a;
    self->arg = b;
}

/* Sets the table pointer (`+0xc`) to `gBossCtrlVtable`, then
 * tail-calls `DestroyCtrl(self, flags)` - which promptly resets it
 * back to `gCtrlVtable` (see ctrl.c) and, if
 * `flags` bit 0 is set, fires `OperatorDelete`. */
void DestroyBossCtrl(struct boss_ctrl *self, s32 flags)
{
    self->vtable = gBossCtrlVtable;
    DestroyCtrl(self, flags);
}

/* Resets via `InitCtrl` (table pointer to `gCtrlVtable`,
 * `state` cleared), then re-points the table at `gBossCtrlVtable`
 * and zeroes `target`/`msg`/`arg`. Returns `self`. */
struct boss_ctrl *CreateBossCtrl(struct boss_ctrl *self)
{
    InitCtrl(self);
    self->vtable = gBossCtrlVtable;
    self->target = 0;
    self->msg = 0;
    self->arg = 0;
    return self;
}

/* The `target` getter (`+0x10`), the controlled part. */
void *GetCtrlTarget(struct boss_ctrl *self)
{
    return self->target;
}
