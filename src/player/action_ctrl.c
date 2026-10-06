#include "core.h"
#include "action_obj.h"
#include "audio.h"
#include "player.h"
#include "objects.h"
#include "globals.h"

/* Continuation of action_ctrl_moves.c (issue #18's chunk, the last one) -
 * covers `nullsub_17` through `ActionCtrlSetTargetAnim` (all matched); non-adjacent
 * to action_ctrl_moves.c since the parked `ActionCtrlStateBodySlamStart` sits raw between
 * them (asm/code_3_2_17_156ec.s). Same "self" object family documented
 * at the top of action_ctrl_states.c/hovercraft_parts.c. */

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);

void nullsub_17(void)
{
}

/* While `self+0x29` is clear: tail-calls `StartActionCtrlRun` first. Always
 * tail-calls `ActionCtrlStateRun` afterward. */
void ActionCtrlStateTurboRun(struct act *selfArg)
{
    register struct act *self asm("r4") = selfArg;

    if (self->turboRun == 0) {
        StartActionCtrlRun(self);
    }
    ActionCtrlStateRun(self);
}

void nullsub_18(void)
{
}

/* Trivial tail-call. */
void sub_8015774(struct act *self)
{
    ActionCtrlStateIdle(self);
}

/* Fires the mgr trampoline pair with `a`/`b` as the two action
 * arguments, then conditionally latches `frame`/`frames` from
 * `c`/`d` unless either is the `0x7FFFFFFF` sentinel. */
void SetActionCtrlModeAnim(struct act *self, s32 a, s32 b, s32 c, s32 d)
{
    struct act_vtable *mgr = self->vt;
    struct act_method *off;

    _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)a, mgr->m20.fn);
    off = &self->vt->m50;
    _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)b, off->fn);

    if (c != 0x7FFFFFFF) {
        self->frame = c;
    }
    if (d != 0x7FFFFFFF) {
        self->frames = d;
    }
}


/* If the player's `+0x100` flag is set: picks a replacement `mode` for
 * a handful of special values (`0x12` when the player's `+0x60` is
 * nonzero -> `0x25`; `0xd`/`0x18` -> `0x26`, both playing a fixed cue
 * via `StopSfx`/`PlaySfx`) and otherwise just re-arms the cue via
 * `StopSfx` with the original `mode`. Always tail-calls
 * `SetCtrlTargetAnim(arg0, arg1, mode)`.
 *
 * The control flow below is written as explicit `goto`s matching the
 * ROM's own block layout exactly (one label per ROM branch target, no
 * `if`/`else` restructuring at all): an `if`/`else if` chain testing
 * `mode==0xd || mode==0x18` compiles to a different (non-matching)
 * decision tree than the ROM's own `cmp #0x12/beq`, `cmp #0x12/bgt`,
 * `cmp #0xd/beq`, (fallthrough) `cmp #0x18/beq` triangle - this
 * project's usual "translate the disassembly's control flow directly,
 * don't re-infer it as structured C" convention applies to branch
 * *shape* just as much as to instruction *choice*.
 *
 * The tail call is declared to return `s32` (reinterpreting
 * `SetCtrlTargetAnim`'s real `u8` return through a function-pointer cast)
 * purely so the value is considered live in `r0` at the return point:
 * a genuinely `void` tail call leaves `r0` free, and gcc then reuses
 * it as the epilogue's `pop`/`bx` scratch register, where the ROM uses
 * `r1`. Returning the call's result normally (matching its real `u8`
 * type) also frees `r0` for `r1`, but pulls in a spurious zero-
 * extension pair (`lsl`/`lsr #0x18`) the ROM doesn't have, since gcc
 * always widens a `char`-returning call's result before propagating
 * it further; the raw-`s32` reinterpretation sidesteps that widening
 * entirely since the value is never treated as narrower than a full
 * register. See docs/matching/naked-sub_80157c4-matched.md for the
 * full derivation (this was the sole remaining residual after a
 * 99.8%-matching pass). */
s32 ActionCtrlSetTargetAnim(struct act *self, struct player *part, s32 mode)
{
    struct player *player = gPlayer;

    if (player->slippery == 0) {
        goto tail;
    }
    if (mode == 0x12) {
        goto case12;
    }
    if (mode > 0x12) {
        goto checkC18;
    }
    if (mode == 0xd) {
        goto setC26;
    }
    goto rearm;

checkC18:
    if (mode == 0x18) {
        goto setC26;
    }
    goto rearm;

case12:
    if (player->speedX == 0) {
        goto tail;
    }
    mode = 0x25;
    goto playCue;

setC26:
    mode = 0x26;
playCue:
    StopSfx(gAudioContext, 0x36);
    PlaySfx(gAudioContext, 0x36, 0x100);
    goto tail;

rearm:
    StopSfx(gAudioContext, 0x36);

tail:
    return ((s32 (*)(void *, void *, s32))SetCtrlTargetAnim)(self, part, mode);
}

/* GitHub issue #19: 0x08015840-0x08016128, `graphics`-labeled chunk that
 * turned out to be the same "self" action-table object family documented
 * at length in ctrl.c/action_ctrl_states.c/action_ctrl_moves.c and
 * above - recategorized `graphics`->`actor` (see docs/matching/
 * issue-19-0x08015840-actor.md). Directly adjacent to the matched span
 * above (which ends with the shared `SetActionCtrlModeAnim` trampoline
 * helper this file's first function calls) and its parked `ActionCtrlSetTargetAnim`
 * right before this chunk starts. Non-adjacent to swim_ctrl_drift.c (this
 * chunk's other matched file) since the left-raw
 * `StartPlayerCtrlStroke`/`StartPlayerCtrlSpin`/`ApplyPlayerCtrlSwimDrift` sit between them (see
 * asm/code_3_2_17_159f8.s). */

/* Fires the mgr trampoline pair (actions `0`/`0x12`), then resets the
 * `0x27`/`0x2f`/`0x31` and `0x28`/`0x30`/`0x32` state/counter/table-index
 * pairs (same trio shape as `StartActionCtrlHighJump`/`sub_8015558` in
 * action_ctrl_moves.c). */
void RestartActionCtrl(struct act *self)
{
    SetActionCtrlModeAnim(self, 0, 0x12, 0, 0);

    self->motionXKeepSpeed = 0;
    self->motionXPending = 1;
    self->motionX = 0;
    self->motionYKeepSpeed = 0;
    self->motionYPending = 1;
    self->motionY = 0;
}

/* Sets `self+0xc`'s table pointer to `gActionCtrlVtable`, then
 * tail-calls `DestroyCtrl(self, flags)` - which promptly resets it back
 * to `gCtrlVtable` (see ctrl.c) - same double-set
 * pattern as `DestroyBossCtrl`. */
void DestroyActionCtrl(struct act *self, s32 flags)
{
    self->vt = (struct act_vtable *)gActionCtrlVtable;
    DestroyCtrl(self, flags);
}

/* Resets via `InitCtrl` (table pointer to `gCtrlVtable`,
 * `self+8` cleared), re-points the table at `gActionCtrlVtable`, then
 * calls `ResetActionCtrl` (the child-object field-reset constructor
 * documented in wumpa.c). Returns `self`. */
struct act *InitActionCtrl(struct act *self)
{
    InitCtrl(self);
    self->vt = (struct act_vtable *)gActionCtrlVtable;
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
void sub_80158AC(struct act *self)
{
    *(s32 *)self->unk_14 = 0;
}

/* `self+0x32` byte setter, always 1. */
void SetActionCtrlMotionYKeepSpeed(struct act *self)
{
    self->motionYKeepSpeed = 1;
}

/* `self+0x31` byte setter, always 1. */
void SetActionCtrlMotionXKeepSpeed(struct act *self)
{
    self->motionXKeepSpeed = 1;
}

/* `self+0x30` byte setter, always 1. */
void SetActionCtrlMotionYPending(struct act *self)
{
    self->motionYPending = 1;
}

/* `self+0x2f` byte setter, always 1. */
void SetActionCtrlMotionXPending(struct act *self)
{
    self->motionXPending = 1;
}

/* `self+0x30` byte setter, always 0. */
void ClearActionCtrlMotionYPending(struct act *self)
{
    self->motionYPending = 0;
}

/* `self+0x2f` byte setter, always 0. */
void ClearActionCtrlMotionXPending(struct act *self)
{
    self->motionXPending = 0;
}

/* `self+0x30` byte getter. */
u8 IsActionCtrlMotionYPending(struct act *self)
{
    return self->motionYPending;
}

/* `self+0x2f` byte getter. */
u8 IsActionCtrlMotionXPending(struct act *self)
{
    return self->motionXPending;
}

/* Sets `self+0x32`/`self+0x30` to 1, and `self+0x28` to `val`. */
void QueueActionCtrlMotionYKeepSpeed(struct act *self, s32 val)
{
    self->motionYKeepSpeed = 1;
    self->motionYPending = 1;
    self->motionY = (u8)val;
}

/* Sets `self+0x31`/`self+0x2f` to 1, and `self+0x27` to `val`. */
void QueueActionCtrlMotionXKeepSpeed(struct act *self, s32 val)
{
    self->motionXKeepSpeed = 1;
    self->motionXPending = 1;
    self->motionX = (u8)val;
}

/* Sets `self+0x32` to 0, `self+0x30` to 1, and `self+0x28` to `val`. */
void QueueActionCtrlMotionY(struct act *self, s32 val)
{
    self->motionYKeepSpeed = 0;
    self->motionYPending = 1;
    self->motionY = (u8)val;
}

/* Sets `self+0x31` to 0, `self+0x2f` to 1, and `self+0x27` to `val`. */
void QueueActionCtrlMotionX(struct act *self, s32 val)
{
    self->motionXKeepSpeed = 0;
    self->motionXPending = 1;
    self->motionX = (u8)val;
}

/* `self+0x2d` byte getter. */
u8 sub_8015950(struct act *self)
{
    return self->prevState;
}

/* Big field reset: clears `self+0x26`/`self+8`/`self+0x24`/`self+0x25`,
 * sets `self+0x2c`/`self+0x2d` to 1, clears `self+0x14`/`self+0x10`/
 * `self+0x22`/`self+0x23`/`self+0x27`/`self+0x20`, sets `self+0x21` to
 * 6, clears `self+0x18`/`self+0x1c`, and clears the player's `+0x92`
 * byte. */
void ResetPlayerCtrl(struct player_ctrl *selfArg)
{
    register u8 *self asm("r3") = (u8 *)selfArg;
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
    gPlayer->bounce = zero;
}

/* Fires the mgr trampoline pair via `SetPlayerCtrlState(self, 0, 0, 0, 0)`,
 * then resets `self+0x27`/`self+0x20`/`self+0x21`(=6)/`self+0x22`, the
 * player's `bounce`, and `self+0x2c`(=1)/`self+0x24`/`self+0x2d`(=1)/
 * `self+0x25`. */
void RestartPlayerCtrl(struct player_ctrl *selfArg)
{
    u8 *self = (u8 *)selfArg;

    SetPlayerCtrlState(selfArg, 0, 0, 0, 0);

    self[0x27] = 0;
    self[0x20] = 0;
    self[0x21] = 6;
    self[0x22] = 0;
    gPlayer->bounce = 0;
    self[0x2c] = 1;
    self[0x24] = 0;
    self[0x2d] = 1;
    self[0x25] = 0;
}
asm(".align 2, 0");
