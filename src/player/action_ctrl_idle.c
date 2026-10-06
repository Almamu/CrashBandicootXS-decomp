#include "core.h"
#include "action_obj.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "player.h"
#include "objects.h"
#include "level.h"
#include "globals.h"

/* Part of GitHub issue #16's remainder (0x08011BD4-0x08012D24): two
 * gActionCtrlStateTable action-table helpers for the player/action object
 * (include/action_obj.h, the same object issue #17's action_ctrl_run_jump.c
 * handlers use). Not ROM-adjacent to action_ctrl_left_ground.c's matched
 * `CheckActionCtrlLeftGround` (this file starts right where that one ends, at
 * 0x08012AF4).
 *
 * Built with old_agbcc (Makefile OLD_AGBCC_OBJS) since the issue #15/#16
 * NAKED retry (docs/matching/issue-15-16-naked-retry.md): `ActionCtrlStateIdle`
 * matches as plain C under it, and `ApplyActionCtrlMotion` followed in the
 * issue #15/#16 NAKED retry 2 (docs/matching/issue-15-16-naked-retry.md). */

/* Trio stores as in action_ctrl_run_jump.c: as inline parameters, old_agbcc
 * materializes the values before the stores. */
static inline void ActTrio27(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->motionXKeepSpeed = cur;
    self->motionXPending = flag;
    self->motionX = next;
}

static inline void ActHold27P(struct act *self, u8 *slot, s32 next)
{
    self->motionXKeepSpeed = 1;
    self->motionXPending = 1;
    *slot = next;
}

/* `part->slippery` (+0x100) read through an offset parameter: that keeps old_agbcc
 * from reusing an earlier 0x100 constant for the field address, which
 * the ROM rematerializes. */
static inline u8 PartByte(struct player *part, s32 offset)
{
    return *((u8 *)part + offset);
}

/* Runs a "part" OAM-visibility/priority housekeeping pass. If the
 * player's `+0x103` flag or `+0x102` byte just changed and `part+0x68`
 * is busy (`==8`), nudges the player's saved-position word by a fixed
 * delta and calls `SetSpritePrevPos`. If `self+8==0`, and the player isn't
 * already in a matching state, plays a fixed sound and fires the
 * `+0x50`/`+0x54` trampoline pair with id `0x12`. Then, keyed on
 * `self+0x2f`'s value (whether `1` or something else), either sets it
 * from the player's `+0x60`/`+0x100` state or reads it as-is; when it's
 * `1`, looks up a per-tag record in `gCtrlMotionRecords` (indexed
 * `(*(self+4))[tag]`, `tag = self+0x27`), copies a 12-byte stretch of
 * it onto the stack, optionally rescales two of its three fields
 * (halving one, doubling the other) via `FixedMul` when the part is
 * busy and the player is active, special-cases tag `0x1e` to zero the
 * record and rescale differently, then fires a `_call_via_r3` trampoline
 * (one of two field-pairs depending on `self+0x30`) with the stack
 * record as its payload and clears the flag. Finally, on `self+0x30==1`,
 * repeats a near-identical stack-record/rescale/trampoline sequence
 * keyed on `self+0x28`'s tag against the same table, then clears
 * `self+0x30`. */
/*
 * The +0x2f flag and the +0x27 tag are read straight through `self`
 * each time: old_agbcc's GCSE then makes the address copies the ROM
 * keeps (the flag address in r7, the tag address copied into r8), and
 * the record index is added to the table base after it is computed
 * (`*(table + i)`), which loads the table address late like the ROM.
 */
void ApplyActionCtrlMotion(struct act *self)
{
    struct speed_ramp rec;
    struct player *p;

    p = gPlayer;
    if (p->pushLeft == 0) {
        if (p->pushRight == 0)
            goto skip;
    }
    {
        if (self->part->hitAxes == 8) {
            if (p->pushLeft)
                p->x -= 0x100;
            else if (p->pushRight)
                p->x += 0x100;
            SetSpritePrevPos((struct gfx_part *)gPlayer, gPlayer->x, gPlayer->y);
        }
    }
skip:
    if (self->state == 0) {
        struct player *p = gPlayer;
        if (p->speedX == 0 && p->anim->unk_0A != 0x12 && self->idleFidget == 0) {
            StopSfx(gAudioContext, 0x36);
            ACT_CALL2(self, m50, gPlayer, 0x12);
        }
    }
    if (self->state == 0 || self->state == 0x11) {
        struct player *p = gPlayer;
        if (p->speedX != 0 && p->slippery == 0) {
            self->motionXKeepSpeed = 0;
            self->motionXPending = 1;
            self->motionX = 0;
        }
    }
    {
        u8 f = self->motionXPending;

        if (f == 1) {
            struct player *q;

            rec = *(gCtrlMotionRecords + (*self->anims)[self->motionX].first);
            q = gPlayer;
            if (q->slippery && self->part->hitAxes == 8 && q->speedX != 0) {
                self->motionXKeepSpeed = f;
                /* Three extra references to `self` (no code): they raise its
                 * allocation priority so the ROM's register choice for the
                 * tag-address copy and the rescale temporaries comes out. */
                asm("" : : "r"(self));
                asm("" : : "r"(self));
                asm("" : : "r"(self));
                rec.target = FixedMul(rec.target, 0x180);
                rec.step /= 2;
            }
            if (self->motionX == 0x1E) {
                self->motionXKeepSpeed = 0;
                if (gPlayer->slippery) {
                    rec.step = FixedMul(rec.step, 0x200);
                    rec.start = FixedMul(rec.start, 0x180);
                }
            }
            if (self->motionXKeepSpeed)
                ACT_CALL2(self, m38, self->part, &rec);
            else
                ACT_CALL2(self, m28, self->part, &rec);
            self->motionXPending = 0;
        }
    }
    {
        if (self->motionYPending == 1) {
            rec = *(gCtrlMotionRecords + (*self->anims)[self->motionY].second);
            if (self->motionYKeepSpeed)
                ACT_CALL2(self, m40, self->part, &rec);
            else
                ACT_CALL2(self, m30, self->part, &rec);
            self->motionYPending = 0;
        }
    }
}

/* A further sibling/callee of the same action-table family. Reads the
 * D-pad (`GetDpadDirection`) and ticks `self+0x25` down on release. If
 * `part+0x38` is set, fires the `+0x50`/`+0x54` trampoline (id `0x12`)
 * and clears `part+0x33`. Bumps `self+0x1c`'s frame counter; while the
 * player's type is `0x12` and `+0x30 == 0`, once the counter passes one
 * of two thresholds (`0x708`, then a further gated pair keyed on a
 * `self+4`-relative negative-distance test), fires further `+0x50`/
 * `+0x54` trampoline calls with escalating ids and resets the counter,
 * setting `self+0x33`. Bails early if `CheckActionCtrlLeftGround(self)` reports busy.
 * Otherwise dispatches the input snapshot's low bits: bit 0 plays a
 * fixed sound and fires two trampoline pairs, bit 1 tail-calls
 * `StartActionCtrlSpin`, bit `0x80` (high byte) fires a different trampoline
 * pair - all converging on `UpdatePlayerFacing`. A further branch (input byte
 * unset, `self+0x25==0`) reads `self+8`'s snapshot value against `2`/
 * `8`-range checks to gate a `HasTurboRun`-confirmed trampoline call
 * (id `4`/`0x18`) or fall through to `StartActionCtrlRun`/a final `+0x20`/
 * `+0x24` trampoline pair, each path ending in `UpdatePlayerFacing`.
 *
 * Matched under old_agbcc. The pad object is loaded before the input word
 * is spilled (its argument is read first); the held-0x100 test's result
 * is what +0x29 is cleared with; the D-pad `else` part sits after the
 * first UpdatePlayerFacing tail, reached by a goto, as in the ROM's layout; and
 * the 3..8 range case comes before case 2. */
void ActionCtrlStateIdle(struct act *self)
{
    void *pad = gInput;
    u32 in = gKeys.all;
    u8 dir = GetDpadDirection(pad);
    s32 frames;
    struct player *part;

    if (self->unk_24[1] != 0) {
        self->unk_24[1]--;
        if (dir == 0)
            self->unk_24[1] = dir;
    }
    part = self->part;
    if (part->animDone) {
        ACT_CALL2(self, m50, part, 0x12);
        self->idleFidget = 0;
    }
    frames = ++self->frames;
    part = self->part;
    if (part->tag == 0x12 && part->frame == 0) {
        if (frames > 0x708) {
            ACT_CALL2(self, m50, part, 0x1A);
            self->frames = 0;
        } else if (frames >= 0x49D && frames <= 0x4C3) {
            ACT_CALL2(self, m50, part, 5);
            self->frames = 0x4C4;
        } else if (frames >= 0x1E1 && frames <= 0x207) {
            ACT_CALL2(self, m50, part, 0xE);
            self->frames = 0x208;
        } else {
            goto skip;
        }
        self->idleFidget = 1;
    }
skip:
    {
        u8 busy = CheckActionCtrlLeftGround(self);
        u16 held;

        if (busy)
            return;
        if (INPUT_PRESSED(in) & 1) {
            PlaySfx(gAudioContext, 0xD, 0x100);
            ACT_CALL1(self, m20, 5);
            ACT_CALL2(self, m50, self->part, 0x13);
            self->frame = busy;
            ActSetNext(self, 7);
        } else {
            u16 alt = INPUT_PRESSED(in) & 2;

            if (alt) {
                StartActionCtrlSpin(self);
            } else {
                if ((held = INPUT_HELD(in) & 0x100) == 0)
                    goto other;
                ACT_CALL1(self, m20, 0x10);
                ACT_CALL2(self, m50, self->part, 3);
                self->frames = alt;
            }
        }
        UpdatePlayerFacing(self);
        return;
    other:
        self->turboRun = held;
        if (dir == 0) {
            struct player *p = self->part;
            u8 *slot;

            if (PartByte(p, 0x100) && *(slot = &self->motionX) != 0x1F && p->speedX != 0)
                ActHold27P(self, slot, 0x1F);
        } else {
            u8 wait = self->unk_24[1];

            if (wait == 0) {
                switch (dir) {
                case 3 ... 8:
                    if ((INPUT_HELD(in) & 0x200) && (u8)HasTurboRun(gLevelState)) {
                        self->turboRun = 1;
                        ACT_CALL1(self, m20, 4);
                        ACT_CALL2(self, m50, self->part, 0x18);
                        ActTrio27(self, wait, 1, 0x1B);
                    } else {
                        StartActionCtrlRun(self);
                    }
                    break;
                case 2:
                    ACT_CALL1(self, m20, 0x10);
                    ACT_CALL2(self, m50, self->part, 3);
                    self->frames = wait;
                    break;
                }
            }
        }
        UpdatePlayerFacing(self);
    }
}
/* Trailing byte count isn't a multiple of 4 - pad with zeros, not a nop. */
asm(".align 2, 0");
