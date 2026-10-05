#include "core.h"

/* GitHub issue #19: 0x08015840-0x08016128, `graphics`-labeled chunk that
 * turned out to be the same "self" action-table object family documented
 * at length in actor_part17.c/actor_part18.c/actor_part38c.c/
 * actor_part38d.c - recategorized `graphics`->`actor` (see docs/matching/
 * issue-19-0x08015840-actor.md). Directly adjacent to actor_part38d.c's
 * matched span (which ends with the shared `SetActionCtrlModeAnim` trampoline
 * helper this file's first function calls) and its parked `ActionCtrlSetTargetAnim`
 * right before this chunk starts. Non-adjacent to actor_part57b.c (this
 * chunk's other matched file) since the left-raw
 * `StartPlayerCtrlStroke`/`StartPlayerCtrlSpin`/`ApplyPlayerCtrlSwimDrift` sit between them (see
 * asm/code_3_2_17_159f8.s). */

extern void SetActionCtrlModeAnim(void *selfArg, s32 a, s32 b, s32 c, s32 d);
extern void DestroyCtrl(void *selfArg, s32 flags);
extern void InitCtrl(void *selfArg);
extern void ResetActionCtrl(void *selfArg);
extern u8 gActionCtrlVtable[];
extern void *gPlayer;
extern void SetPlayerCtrlState(void *selfArg, s32 a, s32 b, s32 c, s32 d);

/* Fires the mgr trampoline pair (actions `0`/`0x12`), then resets the
 * `0x27`/`0x2f`/`0x31` and `0x28`/`0x30`/`0x32` state/counter/table-index
 * pairs (same trio shape as `StartActionCtrlHighJump`/`sub_8015558` in
 * actor_part38c.c). */
void RestartActionCtrl(void *selfArg)
{
    u8 *self = selfArg;

    SetActionCtrlModeAnim(self, 0, 0x12, 0, 0);

    self[0x31] = 0;
    self[0x2f] = 1;
    self[0x27] = 0;
    self[0x32] = 0;
    self[0x30] = 1;
    self[0x28] = 0;
}

/* Sets `self+0xc`'s table pointer to `gActionCtrlVtable`, then
 * tail-calls `DestroyCtrl(self, flags)` - which promptly resets it back
 * to `gCtrlVtable` (see actor_part17.c) - same double-set
 * pattern as `DestroyBossCtrl`. */
void DestroyActionCtrl(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gActionCtrlVtable;
    DestroyCtrl(self, flags);
}

/* Resets via `InitCtrl` (table pointer to `gCtrlVtable`,
 * `self+8` cleared), re-points the table at `gActionCtrlVtable`, then
 * calls `ResetActionCtrl` (the child-object field-reset constructor
 * documented in actor_part39.c). Returns `self`. */
void *InitActionCtrl(void *selfArg)
{
    u8 *self = selfArg;

    InitCtrl(self);
    *(void **)(self + 0xc) = gActionCtrlVtable;
    ResetActionCtrl(self);
    return self;
}

/* The accessors below work on the action controller's (include/
 * action_obj.h `struct act`) motion queue, the fields ApplyActionCtrlMotion
 * consumes: `+0x27`/`+0x28` motionX/motionY (the queued entries),
 * `+0x2f`/`+0x30` their "pending" flags and `+0x31`/`+0x32` the
 * "keep speed" flags (apply with SetCtrlTargetMotionX/Y instead of
 * StartCtrlTargetMotionX/Y). */

/* `self+0x14` word setter, always zero. */
void sub_80158AC(void *selfArg)
{
    *(s32 *)((u8 *)selfArg + 0x14) = 0;
}

/* `self+0x32` byte setter, always 1. */
void SetActionCtrlMotionYKeepSpeed(void *selfArg)
{
    ((u8 *)selfArg)[0x32] = 1;
}

/* `self+0x31` byte setter, always 1. */
void SetActionCtrlMotionXKeepSpeed(void *selfArg)
{
    ((u8 *)selfArg)[0x31] = 1;
}

/* `self+0x30` byte setter, always 1. */
void SetActionCtrlMotionYPending(void *selfArg)
{
    ((u8 *)selfArg)[0x30] = 1;
}

/* `self+0x2f` byte setter, always 1. */
void SetActionCtrlMotionXPending(void *selfArg)
{
    ((u8 *)selfArg)[0x2f] = 1;
}

/* `self+0x30` byte setter, always 0. */
void ClearActionCtrlMotionYPending(void *selfArg)
{
    ((u8 *)selfArg)[0x30] = 0;
}

/* `self+0x2f` byte setter, always 0. */
void ClearActionCtrlMotionXPending(void *selfArg)
{
    ((u8 *)selfArg)[0x2f] = 0;
}

/* `self+0x30` byte getter. */
u8 IsActionCtrlMotionYPending(void *selfArg)
{
    return ((u8 *)selfArg)[0x30];
}

/* `self+0x2f` byte getter. */
u8 IsActionCtrlMotionXPending(void *selfArg)
{
    return ((u8 *)selfArg)[0x2f];
}

/* Sets `self+0x32`/`self+0x30` to 1, and `self+0x28` to `val`. */
void QueueActionCtrlMotionYKeepSpeed(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x32] = 1;
    self[0x30] = 1;
    self[0x28] = (u8)val;
}

/* Sets `self+0x31`/`self+0x2f` to 1, and `self+0x27` to `val`. */
void QueueActionCtrlMotionXKeepSpeed(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x31] = 1;
    self[0x2f] = 1;
    self[0x27] = (u8)val;
}

/* Sets `self+0x32` to 0, `self+0x30` to 1, and `self+0x28` to `val`. */
void QueueActionCtrlMotionY(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x32] = 0;
    self[0x30] = 1;
    self[0x28] = (u8)val;
}

/* Sets `self+0x31` to 0, `self+0x2f` to 1, and `self+0x27` to `val`. */
void QueueActionCtrlMotionX(void *selfArg, s32 val)
{
    u8 *self = selfArg;

    self[0x31] = 0;
    self[0x2f] = 1;
    self[0x27] = (u8)val;
}

/* `self+0x2d` byte getter. */
u8 sub_8015950(void *selfArg)
{
    return ((u8 *)selfArg)[0x2d];
}

/* Big field reset: clears `self+0x26`/`self+8`/`self+0x24`/`self+0x25`,
 * sets `self+0x2c`/`self+0x2d` to 1, clears `self+0x14`/`self+0x10`/
 * `self+0x22`/`self+0x23`/`self+0x27`/`self+0x20`, sets `self+0x21` to
 * 6, clears `self+0x18`/`self+0x1c`, and clears the player's `+0x92`
 * byte. */
void ResetPlayerCtrl(void *selfArg)
{
    register u8 *self asm("r3") = selfArg;
    register u8 *p asm("r0") = self + 0x26;
    register s32 zero asm("r1") = 0;
    register s32 one asm("r2");

    *p = zero;
    *(s32 *)(self + 8) = zero;
    p -= 2;
    *p = zero;
    p += 1;
    *p = zero;
    p += 7;
    one = 1;
    *p = one;
    p += 1;
    *p = one;
    *(s32 *)(self + 0x14) = zero;
    *(s32 *)(self + 0x10) = zero;
    p -= 0xb;
    *p = zero;
    p += 1;
    *p = zero;
    p += 4;
    *p = zero;
    p -= 7;
    *p = zero;
    {
        register u8 *p21 asm("r2") = self + 0x21;
        *p21 = 6;
    }
    *(s32 *)(self + 0x18) = zero;
    *(s32 *)(self + 0x1c) = zero;
    ((u8 *)gPlayer)[0x92] = zero;
}

/* Fires the mgr trampoline pair via `SetPlayerCtrlState(self, 0, 0, 0, 0)`,
 * then resets `self+0x27`/`self+0x20`/`self+0x21`(=6)/`self+0x22`, the
 * player's `+0x92`, and `self+0x2c`(=1)/`self+0x24`/`self+0x2d`(=1)/
 * `self+0x25`. */
void RestartPlayerCtrl(void *selfArg)
{
    u8 *self = selfArg;

    SetPlayerCtrlState(self, 0, 0, 0, 0);

    self[0x27] = 0;
    self[0x20] = 0;
    self[0x21] = 6;
    self[0x22] = 0;
    ((u8 *)gPlayer)[0x92] = 0;
    self[0x2c] = 1;
    self[0x24] = 0;
    self[0x2d] = 1;
    self[0x25] = 0;
}
asm(".align 2, 0");
