#include "core.h"
#include "player.h"
#include "objects.h"

/* GitHub issue #22, ROM 0x08017A44-0x08017AAC. Two groups of methods
 * share this file:
 *
 * - IsInputCtrlMotionXPending .. QueueInputCtrlMotionX (0x08017A44-
 *   0x08017A6C) finish the input_ctrl accessor run that ends
 *   input_ctrl.c (SetInputCtrlMotionYPending .. IsInputCtrlMotionYPending,
 *   0x08017A20-0x08017A40): the bytes they touch are input_ctrl's motion
 *   queue (`+0x14` motionX, `+0x15` motionY, `+0x17`/`+0x18`
 *   motionX/YPending, `+0x19`/`+0x1A` motionX/YKeepSpeed). Nothing calls
 *   them.
 * - BossCtrlHandleEvent/DestroyBossCtrl/CreateBossCtrl are the
 *   gBossCtrlVtable class, the controller base of the bosses: Mega-Mix
 *   (gMegaMixCtrlVtable), Tiny, the Neo Cortex fight's controller and
 *   Dingodile with his shield and rocket/stalactite. Its event slot keeps
 *   the event's msg and arg words at `+0x14`/`+0x18`; no subclass reads
 *   them (they leave `+0x14`-`+0x1B` alone).
 *
 * `self` is the per-level "player/action" ctrl object documented in
 * action_ctrl_states.c's top-of-file comment; `self+0xc` is its method table
 * and `self+0x10` the "part" it drives. The accesses stay raw offsets. */

/* input_ctrl.motionXPending (`+0x17`). */
u8 IsInputCtrlMotionXPending(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x17];
}

/* Queues Y motion entry `val` (input_ctrl.motionY, `+0x15`), pending,
 * applied with the speed kept (motionYPending/motionYKeepSpeed). */
void QueueInputCtrlMotionYKeepSpeed(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x1a] = 1;
    self[0x18] = 1;
    self[0x15] = val;
}

/* Queues X motion entry `val` (input_ctrl.motionX, `+0x14`), pending,
 * applied with the speed kept (motionXPending/motionXKeepSpeed). */
void QueueInputCtrlMotionXKeepSpeed(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x19] = 1;
    self[0x17] = 1;
    self[0x14] = val;
}

/* Queues Y motion entry `val` (motionY + motionYPending). */
void QueueInputCtrlMotionY(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x18] = 1;
    self[0x15] = val;
}

/* Queues X motion entry `val` (motionX + motionXPending). */
void QueueInputCtrlMotionX(void *selfArg, u8 val)
{
    u8 *self = selfArg;

    self[0x17] = 1;
    self[0x14] = val;
}

/* gBossCtrlVtable's event slot (slot 2, CtrlHandleEvent in the base
 * class): stores the event's msg (`a`) and arg (`b`) at `+0x14`/`+0x18`.
 * The sender word `arg1` is unused. Nothing reads the stored words back. */
void BossCtrlHandleEvent(void *selfArg, s32 arg1, s32 a, s32 b)
{
    u8 *self = selfArg;

    (void)arg1;
    *(s32 *)(self + 0x14) = a;
    *(s32 *)(self + 0x18) = b;
}

/* Sets `self+0xc`'s table pointer to `gBossCtrlVtable`, then
 * tail-calls `DestroyCtrl(self, flags)` - which promptly resets it
 * back to `gCtrlVtable` (see ctrl.c) and, if
 * `flags` bit 0 is set, fires `OperatorDelete`. */
void DestroyBossCtrl(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = (void *)gBossCtrlVtable;
    DestroyCtrl(self, flags);
}

/* Resets via `InitCtrl` (table pointer to `gCtrlVtable`,
 * `self+8` cleared), then re-points the table at `gBossCtrlVtable`
 * and zeroes `self+0x10`/`self+0x14`/`self+0x18`. Returns `self`. */
void *CreateBossCtrl(void *selfArg)
{
    u8 *self = selfArg;

    InitCtrl(self);
    *(void **)(self + 0xc) = (void *)gBossCtrlVtable;
    *(s32 *)(self + 0x10) = 0;
    *(s32 *)(self + 0x14) = 0;
    *(s32 *)(self + 0x18) = 0;
    return self;
}

/* `self+0x10` pointer getter - the "part" sub-object. */
void *GetCtrlTarget(void *selfArg)
{
    return *(void **)((u8 *)selfArg + 0x10);
}
