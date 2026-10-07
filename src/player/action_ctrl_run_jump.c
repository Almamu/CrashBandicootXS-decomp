#include "core.h"
#include "match.h"
#include "action_obj.h"
#include "gfx_part.h"
#include "system.h"
#include "audio.h"
#include "player.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #17, ROM 0x08012FBC-0x080134B8 (details in
 * docs/matching/archive/issue-17-0x08012fbc-actor.md, "Third pass"). Two more
 * gActionCtrlStateTable action-table handlers for the player/action object
 * (include/action_obj.h). Built with old_agbcc. */

/* The object LaunchEffectPart spawns for the 0x100 path, as far as it is used. */
/* Stores to the two "next action" trios. As inline parameters, old_agbcc
 * materializes the values before the stores; the `Set` forms store a
 * literal 0/1 for the first two bytes, and the `P` forms store the action
 * through a pointer the caller already holds (the ROM keeps the one it
 * tested). */
static inline void ActQueue27(struct act *self, s32 cur, s32 next)
{
    self->motionXKeepSpeed = cur;
    self->motionXPending = 1;
    self->motionX = next;
}

static inline void ActTrio27(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->motionXKeepSpeed = cur;
    self->motionXPending = flag;
    self->motionX = next;
}

static inline void ActTrio28(struct act *self, s32 cur, s32 flag, s32 next)
{
    self->motionYKeepSpeed = cur;
    self->motionYPending = flag;
    self->motionY = next;
}

static inline void ActSetNextP(struct act *self, u8 *slot, s32 next)
{
    self->motionYKeepSpeed = 0;
    self->motionYPending = 1;
    *slot = next;
}

static inline void ActNext28P(struct act *self, u8 *slot, s32 flag, s32 next)
{
    self->motionYKeepSpeed = 0;
    self->motionYPending = flag;
    *slot = next;
}

static inline void ActSetNext27P(struct act *self, u8 *slot, s32 next)
{
    self->motionXKeepSpeed = 0;
    self->motionXPending = 1;
    *slot = next;
}

static inline void ActHold27P(struct act *self, u8 *slot, s32 next)
{
    self->motionXKeepSpeed = 1;
    self->motionXPending = 1;
    *slot = next;
}

/* Like the header's ActAndFlags0D: as an inline parameter the value is
 * materialized before the part pointer's +0x68 address. */
static inline void ActSetContact(struct player *p, s32 v)
{
    p->hitAxes = v;
}

/* Unless CheckActionCtrlLeftGround reports busy: fire (pressed bit 0) plays action 5's
 * animations and queues action 7; alt (bit 1) hands off to StartActionCtrlSpin;
 * bit 8 plays animations 0xC/0xF, queues 0x1E, clears the player's +0x94
 * and spawns a 0x29 object from gEntitySpawner. Then the D-pad: 0 goes
 * through SetActionCtrlModeAnim and queues 0x1D by hand, 2/7/8 play animations
 * 0x10/3 and queue 0x1D. Held bit 9 in state 3 (and HasTurboRun) plays
 * 4/0x18 and queues 0x1B; without it, state 4 hands off to StartActionCtrlRun.
 *
 * The method calls use ACT_CALL (include/action_obj.h): with the
 * do/while form CSE doesn't carry the fire test's 1 (r7) into the +0x2F
 * stores after the calls. gInput's address is taken up front,
 * which is what keeps it in r8 across the calls, and the spawned object's
 * bits are bitfields so their masks come out as the ROM's -5/-4. */
void ActionCtrlStateRun(struct act *self)
{
    void **pad = &gInput;
    u32 in = gKeys.all;
    u8 busy = CheckActionCtrlLeftGround(self);

    if (busy)
        return;
    if (INPUT_PRESSED(in) & 1) {
        PlaySfx(gAudioContext, SFX_JUMP, 0x100);
        ACT_CALL1(self, m20, ACTION_STATE_JUMP);
        ACT_CALL2(self, m50, self->part, 0x13);
        self->frame = busy;
        ActSetNext(self, 7);
        return;
    }
    {
        u16 alt = INPUT_PRESSED(in) & 2;

        if (alt) {
            StartActionCtrlSpin(self);
            return;
        }
        if (INPUT_PRESSED(in) & R_BUTTON) {
            s32 frames;
            struct gfx_part *obj; /* the effect part (gfx_part.h) */

            PlaySfx(gAudioContext, SFX_SLIDE, 0x100);
            frames = 0x10;
            ACT_CALL1(self, m20, ACTION_STATE_SLIDE);
            ACT_CALL2(self, m50, self->part, 0xF);
            self->frame = alt;
            self->frames = frames;
            ActQueue27(self, alt, 0x1E);
            gPlayer->listCount = alt;
            gPlayer->listCount = alt;
            obj = LaunchEffectPart(gEntitySpawner, 0x29, 1, 0, 0xA, alt, (struct fx_part *)gPlayer);
            obj->hidden = 0;
            obj->gfxMode = 1;
        }
    }
    {
        u8 dir = GetDpadDirection(*pad);

        switch (dir) {
        case 0:
            SetActionCtrlModeAnim(self, ACTION_STATE_IDLE, 0x12, 0, dir);
            ActTrio27(self, dir, 1, dir);
            ActTrio28(self, dir, 1, dir);
            ActTrio27(self, dir, 1, 0x1D);
            break;
        case 2:
        case 7:
        case 8:
            {
                s32 zero = 0;

                ACT_CALL1(self, m20, ACTION_STATE_CROUCH_DOWN);
                ACT_CALL2(self, m50, self->part, 3);
                self->frames = zero;
                ActQueue27(self, zero, 0x1D);
                break;
            }
        }
    }
    {
        s32 held = (u16)(INPUT_HELD(in) & L_BUTTON);

        if (held) {
            if (self->state == ACTION_STATE_RUN && (u8)HasTurboRun(gLevelState)) {
                s32 zero;

                self->turboRun = 1;
                zero = 0;
                ACT_CALL1(self, m20, ACTION_STATE_TURBO_RUN);
                ACT_CALL2(self, m50, self->part, 0x18);
                ActTrio27(self, zero, 1, 0x1B);
            }
        } else if (self->state == ACTION_STATE_TURBO_RUN) {
            self->turboRun = held;
            StartActionCtrlRun(self);
        } else if (self->frame != 0) {
            self->frame = held;
        }
    }
    UpdatePlayerFacing(self);
}

/* Clears part+0x0D bits 0/1, then: on contact bit 2 plays animations
 * 0x1A/0x15, holds the part on frame 2 and hands it to ClearPlayerSpeedY; on the
 * alt edge (with +0x26 clear) plays 0xE/0x10 and clears the charge state
 * and the player's +0x92; once the part's animation is done, picks the
 * next attack animation from the held fire/shoulder bits (9/6 or 7/0xC)
 * and queues 0xA/9/8 while action 7 is pending. Finally the D-pad queues
 * idle, 0x1C (from 0x1B/0x1C), 0xD or 7.
 *
 * The 1 for the 9/8 +0x30 stores is set before the `cur & 1` test and
 * kept apart from the test's own constant (which the ROM rematerializes);
 * the barrier keeps gcc from folding the two into one register. */
void ActionCtrlStateJump(struct act *self)
{
    ActAndFlags0D(self->part, -2);
    ActAndFlags0D(self->part, -3);
    if (self->part->hitAxes & 4) {
        struct player *part;
        s32 frame;
        s32 count;

        ACT_VCALL1(self, m20, ACTION_STATE_AIRBORNE_FALL);
        ACT_VCALL2(self, m50, self->part, 0x15);
        part = self->part;
        frame = 2;
        count = part->anim->records[part->tag].frameCount;
        if (frame >= count)
            frame = count - 1;
        part->frame = frame;
        ClearPlayerSpeedY(part);
        ActSetContact(self->part, 0);
        return;
    }
    {
        u32 in = gKeys.all;
        u8 busy = self->spinCooldown;

        if (busy == 0 && (INPUT_PRESSED(in) & 2)) {
            s32 frames;

            PlaySfx(gAudioContext, SFX_SPIN, 0x100);
            frames = 0x18;
            ACT_VCALL1(self, m20, ACTION_STATE_AIR_SPIN);
            ACT_VCALL2(self, m50, self->part, 0x10);
            self->frame = busy;
            self->frames = frames;
            self->tornadoVariant = busy;
            self->charge = busy;
            self->tornadoTurn = busy;
            self->tornadoFallQueued = busy;
            self->tornadoUnwinding = busy;
            gPlayer->bounce = busy;
            return;
        }
    }
    {
        struct player *part = self->part;

        if (part->animDone) {
            u32 cur = gKeys.all;

            if ((cur & A_BUTTON) && (cur & DPAD_SIDEWAYS)) {
                if (part->tag == 6) {
                    ACT_VCALL1(self, m20, ACTION_STATE_AIRBORNE_FLIP_JUMP);
                } else {
                    ACT_VCALL1(self, m20, ACTION_STATE_AIRBORNE_FLIP_JUMP);
                    ACT_VCALL2(self, m50, self->part, 6);
                }
                {
                    u8 *slot = &self->motionY;

                    if (*slot == 7)
                        ActSetNextP(self, slot, 0xA);
                }
            } else {
                ACT_VCALL1(self, m20, ACTION_STATE_AIRBORNE_JUMP);
                ACT_VCALL2(self, m50, self->part, 0xC);
                {
                    u8 *slot = &self->motionY;

                    if (*slot == 7) {
                        s32 one = 1;

                        MATCH_KEEP(one);

                        if (cur & 1)
                            ActNext28P(self, slot, one, 9);
                        else
                            ActNext28P(self, slot, one, 8);
                    }
                }
            }
        }
    }
    if (GetDpadDirection(gInput) <= 2) {
        if (gPlayer->slippery == 0) {
            self->motionXKeepSpeed = 0;
            self->motionXPending = 1;
            self->motionX = 0;
        }
    } else {
        u8 *slot = &self->motionX;

        if (*slot == 0x1B || *slot == 0x1C) {
            ActHold27P(self, slot, 0x1C);
        } else if (self->frame != 0) {
            if (*slot != 0xD)
                ActSetNext27P(self, slot, 0xD);
        } else if (gPlayer->slippery == 0) {
            ActSetNext27P(self, slot, 7);
        }
    }
    UpdatePlayerFacing(self);
}
