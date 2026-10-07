#include "core.h"
#include "match.h"
#include "action_obj.h"
#include "audio.h"
#include "player.h"
#include "player_ctrl.h"
#include "objects.h"
#include "globals.h"

/* Continuation of action_ctrl_moves.c (issue #18's chunk, the last one) -
 * covers `ActionCtrlStateNop6` through `ActionCtrlSetTargetAnim` (all matched); non-adjacent
 * to action_ctrl_moves.c since the parked `ActionCtrlStateBodySlamStart` sits raw between
 * them (asm/code_3_2_17_156ec.s). Same "self" object family documented
 * at the top of action_ctrl_states.c/hovercraft_parts.c. */

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);

/* gActionCtrlStateTable's slot 6 (and ActionCtrlStateNop2 slot 2): empty
 * handlers of states nothing sets. */
void ActionCtrlStateNop6(void)
{
}

/* While `self+0x29` is clear: tail-calls `StartActionCtrlRun` first. Always
 * tail-calls `ActionCtrlStateRun` afterward. */
void ActionCtrlStateTurboRun(struct act *selfArg)
{
    MATCH_HOLD_REG(struct act *, self, r4) = selfArg;

    if (self->turboRun == 0) {
        StartActionCtrlRun(self);
    }
    ActionCtrlStateRun(self);
}

void ActionCtrlStateNop2(void)
{
}

/* gActionCtrlStateTable's slot 1, a state nothing sets: runs the idle
 * state's handler. */
void ActionCtrlStateUnusedIdle(struct act *self)
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
 * register. See docs/matching/archive/naked-sub_80157c4-matched.md for the
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
    StopSfx(gAudioContext, SFX_SKID);
    PlaySfx(gAudioContext, SFX_SKID, 0x100);
    goto tail;

rearm:
    StopSfx(gAudioContext, SFX_SKID);

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
 * right before this chunk starts. Non-adjacent to swim_ctrl_drift.cpp (this
 * chunk's other matched file) since the left-raw
 * `StartPlayerCtrlStroke`/`StartPlayerCtrlSpin`/`ApplyPlayerCtrlSwimDrift` sit between them (see
 * asm/code_3_2_17_159f8.s). */

/* Fires the mgr trampoline pair (actions `0`/`0x12`), then resets the
 * `0x27`/`0x2f`/`0x31` and `0x28`/`0x30`/`0x32` state/counter/table-index
 * pairs (same trio shape as `StartActionCtrlHighJump`/`StartActionCtrlMaskHitJump` in
 * action_ctrl_moves.c). */
void RestartActionCtrl(struct act *self)
{
    SetActionCtrlModeAnim(self, ACTION_STATE_IDLE, 0x12, 0, 0);

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

/* `self+0x14` word setter, always zero. UNUSED, and nothing reads the
 * word (ResetActionCtrl also clears it), so it stays unnamed. */
void sub_80158AC(struct act *self)
{
    self->unk_14 = 0;
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

/* `prevState` (+0x2D) getter. UNUSED - no caller anywhere in the ROM (no
 * `bl` in src/, no Thumb pointer to it in baserom.gba). */
u8 GetActionCtrlPrevState(struct act *self)
{
    return self->prevState;
}

/* Big field reset: clears `unk_26`/`state`/`motionX`/`motionY`, sets
 * `motionXPending`/`motionYPending` to 1, clears `unk_14`/`target`/
 * `mode`/`spinCooldown`/`idleTimer`/`repeat`, sets `tilt` to 6, clears
 * `timer`/`timerMax`, and clears the player's `bounce`. The byte stores
 * walk one pointer from `unk_26`, as the ROM does. */
void ResetPlayerCtrl(struct player_ctrl *selfArg)
{
    MATCH_HOLD_REG(struct player_ctrl *, self, r3) = selfArg;
    MATCH_HOLD_REG(u8 *, p, r0) = &self->unk_26;
    MATCH_HOLD_REG(s32, zero, r1) = 0;
    MATCH_HOLD_REG(s32, one, r2);

    *p = zero;
    self->state = zero;
    p -= 2;
    *p = zero;
    p += 1;
    *p = zero;
    p += 7;
    one = 1;
    *p = one;
    p += 1;
    *p = one;
    self->unk_14 = zero;
    self->target = (struct player *)zero;
    p -= 0xb;
    *p = zero;
    p += 1;
    *p = zero;
    p += 4;
    *p = zero;
    p -= 7;
    *p = zero;
    {
        MATCH_HOLD_REG(u8 *, p21, r2) = &self->tilt;
        *p21 = 6;
    }
    self->timer = zero;
    self->timerMax = zero;
    gPlayer->bounce = zero;
}

/* Fires the mgr trampoline pair via `SetPlayerCtrlState(self, 0, 0, 0, 0)`,
 * then resets `idleTimer`/`repeat`/`tilt`(=6)/`mode`, the player's
 * `bounce`, and `motionXPending`(=1)/`motionX`/`motionYPending`(=1)/
 * `motionY`. */
void RestartPlayerCtrl(struct player_ctrl *selfArg)
{
    struct player_ctrl *self = selfArg;

    SetPlayerCtrlState(selfArg, 0, 0, 0, 0);

    self->idleTimer = 0;
    self->repeat = 0;
    self->tilt = 6;
    self->mode = 0;
    gPlayer->bounce = 0;
    self->motionXPending = 1;
    self->motionX = 0;
    self->motionYPending = 1;
    self->motionY = 0;
}
