#include "core.h"
#include "match.h"
#include "action_obj.h"
#include "vtable.h"
#include "actor.h"
#include "audio.h"
#include "player.h"
#include "level.h"
#include "globals.h"

/* Continuation of action_ctrl_hang.c (issue #18's chunk) - covers
 * `StartActionCtrlTornadoFall`, `EndActionCtrlSpin` and `SteerActionCtrlSpin`. Same "self" object
 * family documented at the top of action_ctrl_states.c/hovercraft_parts.c. */

/* The tornado spin's slow descent, once per spin (`tornadoFallQueued`,
 * `self+0x23`): called once the spinning player leaves the ground
 * (ActionCtrlStateTornadoSpin) or, in the air spin, starts falling
 * (UpdateActionCtrl). Queues Y motion entry
 * 0x18/0x19/0x1a by `tornadoTurn` (`self+0x22`; a small jump table for
 * `[0,1]`/`2`/`[3,4]`, no-op if it is above 4) into `motionY` (`self+0x28`,
 * latching `self+0x30`/clearing `self+0x32` alongside it). Those entries
 * are gCtrlMotionRecords 29-31, {8, 18/14/6, 1280}: the more turns, the
 * slower the fall speeds up. Then always sets bit 0 of `part+0xd` and
 * clears `slamBlocked` (`self+0x34`). */
void StartActionCtrlTornadoFall(struct act *selfArg)
{
    MATCH_HOLD_REG(struct act *, self, r3) = selfArg;
    u8 *p23 = &self->tornadoFallQueued;

    if (*p23 != 0) {
        return;
    }
    *p23 = 1;

    /* Hand-written jump table (rather than a plain `switch`, or a C
     * computed-goto table) to get both the exact table layout and the
     * physical order of the three target blocks (`0x18`, `0x19`,
     * `0x1a`) - this compiler's own jump-table lowering for an
     * equivalent `switch` always matched the case-to-value mapping but
     * picked a different, seemingly source-order-independent block
     * layout every time (tried several case-label scatterings); a
     * computed-goto table hit the same table-order problem and also
     * pulled its `static const` array into a discarded `.data`
     * section this ROM has no room for. Spelling the table out in raw
     * asm, using the same "hand-placed local labels shared across a
     * single literal pool" idea as `DoSuperBodySlamShockwave`'s anti-CSE note in
     * hovercraft_parts.c, sidesteps both problems - `self` is pinned to
     * `r3` for the whole function so this block's hardcoded `r3` use
     * matches whatever the compiler already has it in. */
    {
        MATCH_HOLD_REG(s32, val, r2);
        MATCH_HOLD_REG(struct act *, selfIn, r3) = self;

        // clang-format off
        asm volatile(
            "add r0, r3, #0\n\t"
            "add r0, r0, #0x22\n\t"
            "ldrb r0, [r0]\n\t"
            "cmp r0, #4\n\t"
            "bhi L_80151c8_skip\n\t"
            "lsl r0, r0, #2\n\t"
            "ldr r1, L_80151c8_tbl\n\t"
            "add r0, r0, r1\n\t"
            "ldr r0, [r0]\n\t"
            "mov pc, r0\n\t"
            ".align 2, 0\n\t"
            "L_80151c8_tbl: .word L_80151c8_tbl2\n\t"
            "L_80151c8_tbl2:\n\t"
            "  .word L_80151c8_18\n\t"
            "  .word L_80151c8_18\n\t"
            "  .word L_80151c8_19\n\t"
            "  .word L_80151c8_1a\n\t"
            "  .word L_80151c8_1a\n\t"
            "L_80151c8_18:\n\t"
            "  mov %0, #0x18\n\t"
            "  b L_80151c8_store\n\t"
            "L_80151c8_19:\n\t"
            "  mov %0, #0x19\n\t"
            "  b L_80151c8_store\n\t"
            "L_80151c8_1a:\n\t"
            "  mov %0, #0x1a\n\t"
            "L_80151c8_store:\n\t"
            "  add r1, r3, #0\n\t"
            "  add r1, r1, #0x32\n\t"
            "  mov r0, #0\n\t"
            "  strb r0, [r1]\n\t"
            "  sub r1, r1, #2\n\t"
            "  mov r0, #1\n\t"
            "  strb r0, [r1]\n\t"
            "  add r0, r3, #0\n\t"
            "  add r0, r0, #0x28\n\t"
            "  strb %0, [r0]\n\t"
            "L_80151c8_skip:\n\t"
            : "=r"(val)
            : "r"(selfIn)
            : "r0", "r1", "cc"
        );
        // clang-format on
    }

    {
        MATCH_HOLD_REG(struct player *, part, r1) = self->part;
        MATCH_HOLD_REG(s32, one, r0) = 1;
        MATCH_HOLD_REG(u8, old, r2) = part->flags2;

        one |= old;
        part->flags2 = one;
    }
    {
        MATCH_HOLD_REG(u8 *, p34, r1) = &self->slamBlocked;
        MATCH_HOLD_REG(s32, zero, r0) = 0;

        *p34 = zero;
    }
}
/* Zero-fill alignment before the next function (see docs/matching.md's
 * alignment-padding gotcha). */
asm(".align 2, 0");

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);

/* Always sets `self+0x26 = 0xc`. For `mode` `3`/`4`: if `flags` bit
 * `0x200` is set and `HasTurboRun(gLevelState)` is true, latches
 * `self+0x29`, fires the mgr trampoline pair with actions `4`/`0x18`,
 * and sets the state/counter/table-index trio (`0x31`/`0x2f`/`0x27`) to
 * `0`/`1`/`0x1b` - otherwise falls back to `StartActionCtrlRun`. For every
 * other `mode`: resets via `SetActionCtrlModeAnim(self, ACTION_STATE_IDLE, 0x12, 0, 0)` and clears
 * both state/counter/table-index trios (`0x31`/`0x2f`/`0x27` and
 * `0x32`/`0x30`/`0x28`).
 *
 * Matched in a later pass (docs/matching/archive/issue-18-0x08014f8c-actor.md,
 * "Later pass: strag2 retry"): `self`/`mode` as real `u8 *`/`u8` parameters fixed the
 * entry home-copy order the old draft got backwards; the `flags` test
 * needs the constant-copy escape below. */
void EndActionCtrlSpin(struct act *self, u8 mode, s32 flags)
{
    self->spinCooldown = 0xc;
    switch (mode) {
    case 3:
    case 4:
        {
            s32 m = 0x200;
            s32 m2;

            /* The ROM builds 0x200 in r1 and ANDs through a copy in r0, into
             * flags' own r2: the MATCH_CONST escape keeps the copy (m2) apart
             * from m, and the volatile use of m and flags right after the
             * `and` stops combine from sinking it into the test and regmove
             * from retargeting it onto m2. */
            MATCH_CONST(m2, m);
            flags &= m2;
            asm volatile("" : "+r"(flags) : "r"(m));
            if (flags != 0 && (u8)HasTurboRun(gLevelState)) {
                struct act_vtable *mgr;
                struct act_method *off;
                u8 one;
                u8 *p = &self->turboRun;

                one = 1;
                *p = one;
                mgr = self->vt;
                _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)ACTION_STATE_TURBO_RUN,
                             mgr->m20.fn);
                off = &self->vt->m50;
                _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x18, off->fn);
                {
                    u8 idx = 0x1b;

                    self->motionXKeepSpeed = 0;
                    self->motionXPending = one;
                    self->motionX = idx;
                }
            } else {
                StartActionCtrlRun(self);
            }
            break;
        }
    default:
        {
            u8 zero = 0;

            SetActionCtrlModeAnim(self, ACTION_STATE_IDLE, 0x12, 0, zero);
            self->motionXKeepSpeed = zero;
            self->motionXPending = 1;
            self->motionX = zero;
            self->motionYKeepSpeed = zero;
            self->motionYPending = 1;
            self->motionY = zero;
            break;
        }
    }
}

/* While `self+0x27`/`self+0x2b` are both clear and `mode` is `3`/`4`:
 * sets the state/counter/table-index trio (`0x31`/`0x2f`/`0x27`) to
 * `0`/`1`/`0x17`. Independently, for `mode <= 2`: resets the same trio
 * to `0`/`1`/`0`. Always tail-calls `UpdatePlayerFacing`.
 *
 * Matched in a later pass (docs/matching/archive/issue-18-0x08014f8c-actor.md,
 * "Later pass: strag2 retry"): the `0x17`/`0` table indices go through `u8` locals
 * so they're materialized before the stores, which also moves `mode`
 * into `r4` as in the ROM. */
void SteerActionCtrlSpin(struct act *self, u8 mode)
{
    if (self->motionX == 0 && self->bumpTimer == 0) {
        switch (mode) {
        case 3:
        case 4:
            {
                u8 idx = 0x17;

                self->motionXKeepSpeed = 0;
                self->motionXPending = 1;
                self->motionX = idx;
                break;
            }
        }
    }
    if (mode <= 2) {
        u8 idx = 0;

        self->motionXKeepSpeed = idx;
        self->motionXPending = 1;
        self->motionX = idx;
    }
    UpdatePlayerFacing(self);
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");

/* Continuation of the code above (issue #18's chunk) - covers
 * `SetActionCtrlMode` through `ActionCtrlStateBodySlamStart`. Same "self" object family
 * documented at the top of action_ctrl_states.c/hovercraft_parts.c. */

/* Clears `self+0x33`, saves `self+8`'s previous value (truncated) into
 * `self+0x2d`, overwrites `self+8` with `arg1`, and clears
 * `self+0x2c`/`self+0x2b`. If `arg1` isn't `0xd`/`0xe`, also clears
 * `part+0x90` and the player's `+0x92`/`+0x94` (written twice - the ROM
 * really does re-derive the player pointer and store the same byte
 * there a second time). */
void SetActionCtrlMode(struct act *self, s32 arg1)
{
    s32 old;

    self->idleFidget = 0;
    old = self->state;
    self->prevState = (u8)old;
    self->state = arg1;
    self->bumpedMotionX = 0;
    self->bumpTimer = 0;

    if ((u32)(arg1 - ACTION_STATE_SPIN) > 1) {
        struct player *part = self->part;
        u8 *player;

        part->bumped = 0;

        /* bounce (0x92) and listCount (0x94), as byte stores: as struct
         * member stores the 0 is not kept in r4 */
        player = *(u8 *volatile *)&gPlayer;
        player[0x92] = 0;
        player = *(u8 *volatile *)&gPlayer;
        player[0x94] = 0;
        player = *(u8 *volatile *)&gPlayer;
        player[0x94] = 0;
    }
}

/* While `self+0x26` is clear: plays a fixed cue, resets `self+0x18`/
 * `0x1c` to `0`/`0x18`, fires the mgr trampoline pair (actions `0x10`
 * then `0xd`), and clears `self+0x20`-`self+0x24`. */
void StartActionCtrlSpin(struct act *self)
{

    if (self->spinCooldown == 0) {
        struct vtable_slot *mgr;
        struct act_method *off;

        PlaySfx(gAudioContext, SFX_SPIN, 0x100);
        self->frame = 0;
        self->frames = 0x18;

        off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x10, off->fn);
        mgr = (struct vtable_slot *)self->vt;
        _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_SPIN, mgr[4].fn);

        self->tornadoVariant = 0;
        self->charge = 0;
        self->tornadoTurn = 0;
        self->tornadoFallQueued = 0;
        self->tornadoUnwinding = 0;
    }
}

/* Same shape as `StartActionCtrlSpin`, different action codes (`0x1e`/`0x21`)
 * and the field-clear runs before the trampoline pair instead of
 * after. */
void StartActionCtrlHangSpin(struct act *self)
{

    if (self->spinCooldown == 0) {
        struct vtable_slot *mgr;
        struct act_method *off;

        PlaySfx(gAudioContext, SFX_SPIN, 0x100);
        self->frame = 0;
        self->frames = 0x18;

        self->tornadoVariant = 0;
        self->charge = 0;
        self->tornadoTurn = 0;
        self->tornadoFallQueued = 0;
        self->tornadoUnwinding = 0;

        off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x1e, off->fn);
        mgr = (struct vtable_slot *)self->vt;
        _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_HANG_SPIN, mgr[4].fn);
    }
}

/* Two near-identical arms keyed on `self+0x29`: both fire the mgr
 * trampoline pair and set the state/counter trio (`0x31`/`0x2f`), then
 * latch `self+0x31` again if `part+0x100` is set - only the
 * trampoline actions and the table-index (`self+0x27`, `0x1b` vs `1`)
 * differ between the two arms. */
void StartActionCtrlRun(struct act *self)
{

    if (self->turboRun != 0) {
        struct act_method *off;
        struct vtable_slot *mgr;
        u8 *p31;

        self->frame = 0;

        off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x18, off->fn);
        mgr = (struct vtable_slot *)self->vt;
        _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_TURBO_RUN, mgr[4].fn);

        {
            u8 val = 0x1b;

            p31 = &self->motionXKeepSpeed;
            *p31 = 0;
            {
                u8 *p2f = &self->motionXPending;
                u8 one = 1;

                *p2f = one;
                p2f -= 8;
                *p2f = val;

                if (self->part->slippery != 0) {
                    *p31 = one;
                }
            }
        }
    } else {
        struct act_method *off;
        struct vtable_slot *mgr;
        u8 *p31;

        off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0xd, off->fn);
        self->frame = 0;
        mgr = (struct vtable_slot *)self->vt;
        _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_RUN, mgr[4].fn);

        {
            u8 one = 1;
            u8 *p2f;

            p31 = &self->motionXKeepSpeed;
            *p31 = 0;
            p2f = &self->motionXPending;
            *p2f = one;
            p2f -= 8;
            *p2f = one;

            if (self->part->slippery != 0) {
                *p31 = one;
            }
        }
    }
}

/* Fires the mgr trampoline pair with actions `0xb`/`0xb`, clears
 * `self+0x18`, sets the state/counter/table-index trio to `0`/`1`/`0xb`,
 * and clears `part+0x68`. The motionYKeepSpeed/motionYPending stores stay
 * byte stores: as field stores the pinned `zero` changes the code
 * (docs/workflow.md step 7). */
void StartActionCtrlHighJump(struct act *self)
{
    MATCH_HOLD_REG(s32, zero, r5) = 0;
    MATCH_HOLD_REG(u8, idx, r2);
    struct vtable_slot *mgr = (struct vtable_slot *)self->vt;
    struct act_method *off;
    u8 *p28;

    _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_AIRBORNE_HIGH_JUMP, mgr[4].fn);
    off = &self->vt->m50;
    _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0xb, off->fn);

    self->frame = zero;
    idx = 0xb;
    *(u8 *)&self->motionYKeepSpeed = zero;
    *(u8 *)&self->motionYPending = 1;
    p28 = &self->motionY;
    MATCH_KEEP_VOLATILE(p28);
    *p28 = idx;
    self->part->hitAxes = zero;
}

/* Same shape as `StartActionCtrlHighJump`, but with Y motion entry 7 (the
 * plain jump the A button queues in ActionCtrlStateRun) instead of 0xb. The
 * hop Crash makes when the Aku Aku mask absorbs a hit:
 * ActionCtrlHandleEvent's event 11 calls it, and the only sender of
 * event 11 is PlayerHandleEvent's hit cases (1-10), right after they drop
 * the mask level by one (mask level 1 or 2). */
void StartActionCtrlMaskHitJump(struct act *self)
{
    MATCH_HOLD_REG(s32, zero, r5) = 0;
    MATCH_HOLD_REG(u8, idx, r2);
    struct vtable_slot *mgr = (struct vtable_slot *)self->vt;
    struct act_method *off;
    u8 *p28;

    _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_AIRBORNE_HIGH_JUMP, mgr[4].fn);
    off = &self->vt->m50;
    _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0xb, off->fn);

    self->frame = zero;
    idx = 7;
    *(u8 *)&self->motionYKeepSpeed = zero;
    *(u8 *)&self->motionYPending = 1;
    p28 = &self->motionY;
    MATCH_KEEP_VOLATILE(p28);
    *p28 = idx;
    self->part->hitAxes = zero;
}

/* Single-instruction store: `part = player` (the ctrl's AttachCtrl slot). */
void AttachActionCtrl(struct act *self, struct player *player)
{
    self->part = player;
}

/* gActionCtrlStateTable's slot 0x27, a state nothing sets: runs
 * ActionCtrlReleaseHang. */
void ActionCtrlStateUnusedHangRelease(struct act *self)
{
    ActionCtrlReleaseHang(self);
}

/* gActionCtrlStateTable's slot 0x23, a state nothing sets; the same code as
 * ActionCtrlStateHangGrab (slot 0x1F). While `part+0x38` is set: fires the
 * mgr trampoline pair (actions `0x20`/`0x1f`) and clears `self+0x18`/`0x1c`. */
void ActionCtrlStateUnusedHangGrab(struct act *self)
{

    if (self->part->animDone != 0) {
        MATCH_HOLD_REG(s32, zero, r4) = 0;
        struct vtable_slot *mgr = (struct vtable_slot *)self->vt;
        struct act_method *off;

        _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_HANG, mgr[4].fn);
        off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x1f, off->fn);

        self->frame = zero;
        self->frames = zero;
    }
}

/* Bumps `frame`; once it reaches `frames` (or the part's `animDone` is
 * already set), stamps `unk_26 = 0xc`, fires the `m20`/`m50` methods
 * (actions `0x20`/`0x1f`), and resets `frame`/`frames` to `0`.
 * Always tail-calls `UpdatePlayerFacing`. */
void ActionCtrlStateHangSpin(struct act *self)
{
    self->frame += 1;
    if (self->frame >= self->frames || self->part->animDone != 0) {
        MATCH_HOLD_REG(s32, zero, r4);
        u8 *p26 = &self->spinCooldown;
        struct act_method *m;

        zero = 0;
        *p26 = 0xc;

        m = &self->vt->m20;
        _call_via_r2((u8 *)self + m->thisOffset, (void *)ACTION_STATE_HANG, m->fn);
        m = &self->vt->m50;
        _call_via_r3((u8 *)self + m->thisOffset, self->part, (void *)0x1f, m->fn);

        self->frame = zero;
        self->frames = zero;
    }

    UpdatePlayerFacing(self);
}

/* Same shape as `ActionCtrlStateUnusedHangGrab` - byte-identical ROM encoding at a
 * different address (no shared caller; kept as a separate copy rather
 * than a wrapper to match). */
void ActionCtrlStateHangGrab(struct act *self)
{

    if (self->part->animDone != 0) {
        MATCH_HOLD_REG(s32, zero, r4) = 0;
        struct vtable_slot *mgr = (struct vtable_slot *)self->vt;
        struct act_method *off;

        _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_HANG, mgr[4].fn);
        off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)0x1f, off->fn);

        self->frame = zero;
        self->frames = zero;
    }
}

/* While `part+0x38` is set: sets the player's `+0xc` bit `0x80` and
 * tail-calls `RequestRoomExit`. */
void ActionCtrlStateWarpOut(struct act *self)
{

    if (self->part->animDone != 0) {
        MATCH_HOLD_REG(struct player *, player, r1) = gPlayer;
        MATCH_HOLD_REG(s32, bit, r0) = 0x80;
        MATCH_HOLD_REG(u8, old, r2) = player->flags.all;

        bit |= old;
        player->flags.all = bit;
        RequestRoomExit();
    }
}

/* While `part+0x38` is set: fires the mgr trampoline pair with actions
 * `0x11`/`4`. */
void ActionCtrlStateCrawlStop(struct act *self)
{

    if (self->part->animDone != 0) {
        struct vtable_slot *mgr = (struct vtable_slot *)self->vt;
        struct act_method *off;

        _call_via_r2((u8 *)self + mgr[4].delta, (void *)ACTION_STATE_CROUCH, mgr[4].fn);
        off = &self->vt->m50;
        _call_via_r3((u8 *)self + off->thisOffset, self->part, (void *)4, off->fn);
    }
}

/* While `part+0x38` is set: when `HasSuperBodySlam(gLevelState)` is
 * true, fires the mgr trampoline pair with actions `0x19`/`7`;
 * otherwise fires only the first trampoline with action `0x18`.
 *
 * Reads `self` through the parameter itself, with no local copy: an old
 * `u8 *self = selfArg;` copy survived GCSE's copy propagation into the
 * `else` arm, which is what forced the extra `push {r5}`/`adds r5, r4, #0`
 * - see docs/matching/archive/issue-18-0x08014f8c-actor.md, "Later pass: strag2
 * retry". */
void ActionCtrlStateBodySlamStart(struct act *self)
{
    if (self->part->animDone != 0) {
        if ((u8)HasSuperBodySlam(gLevelState)) {
            struct act_method *m = &self->vt->m20;

            _call_via_r2((u8 *)self + m->thisOffset, (void *)ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM,
                         m->fn);
            m = &self->vt->m50;
            _call_via_r3((u8 *)self + m->thisOffset, self->part, (void *)7, m->fn);
        } else {
            struct act_method *m = &self->vt->m20;

            _call_via_r2((u8 *)self + m->thisOffset, (void *)ACTION_STATE_AIRBORNE_BODY_SLAM,
                         m->fn);
        }
    }
}

/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
