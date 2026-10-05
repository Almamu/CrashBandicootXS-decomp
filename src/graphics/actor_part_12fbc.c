#include "core.h"
#include "action_obj.h"

/* GitHub issue #17, ROM 0x08012FBC-0x080134B8 (details in
 * docs/matching/issue-17-0x08012fbc-actor.md, "Third pass"). Two more
 * gActionCtrlStateTable action-table handlers for the player/action object
 * (include/action_obj.h). Built with old_agbcc. */

/* The object LaunchEffectPart spawns for the 0x100 path, as far as it is used. */
struct spawned
{
    u8 unk_00[0xC];
    u8 unk_0C_0:2;
    u8 unk_0C_2:1;
    u8 unk_0C_3:5;
    u8 unk_0D[0x1B];
    u8 unk_28_0:2;
    u8 unk_28_2:6;
};

extern u32 gKeys;
extern void *gAudioContext;
extern void *gLevelState;
extern u8 *gPlayer;
extern void *gEntitySpawner;
extern void *gInput;
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 GetDpadDirection(void *pad);
extern u8 CheckActionCtrlLeftGround(struct act *self);
extern void UpdatePlayerFacing(struct act *self);
extern void ClearPlayerSpeedY(struct act_part *part);
extern void StartActionCtrlSpin(struct act *self);
extern void StartActionCtrlRun(struct act *self);
extern void SetActionCtrlModeAnim(struct act *self, s32 a, s32 b, s32 c, s32 d);
extern u8 HasTurboRun(void *self);
extern struct spawned *LaunchEffectPart(void *pool, s32 a, s32 b, s32 c, s32 d, s32 e, void *f);

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
static inline void ActSetContact(struct act_part *p, s32 v)
{
    p->contact = v;
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
    u32 in = gKeys;
    u8 busy = CheckActionCtrlLeftGround(self);

    if (busy)
        return;
    if (INPUT_PRESSED(in) & 1)
    {
        PlaySfx(gAudioContext, 0xD, 0x100);
        ACT_CALL1(self, m20, 5);
        ACT_CALL2(self, m50, self->part, 0x13);
        self->frame = busy;
        ActSetNext(self, 7);
        return;
    }
    {
        u16 alt = INPUT_PRESSED(in) & 2;

        if (alt)
        {
            StartActionCtrlSpin(self);
            return;
        }
        if (INPUT_PRESSED(in) & 0x100)
        {
            s32 frames;
            struct spawned *obj;

            PlaySfx(gAudioContext, 0x1A, 0x100);
            frames = 0x10;
            ACT_CALL1(self, m20, 0xC);
            ACT_CALL2(self, m50, self->part, 0xF);
            self->frame = alt;
            self->frames = frames;
            ActQueue27(self, alt, 0x1E);
            gPlayer[0x94] = alt;
            gPlayer[0x94] = alt;
            obj = LaunchEffectPart(gEntitySpawner, 0x29, 1, 0, 0xA, alt, gPlayer);
            obj->unk_0C_2 = 0;
            obj->unk_28_0 = 1;
        }
    }
    {
        u8 dir = GetDpadDirection(*pad);

        switch (dir)
        {
        case 0:
            SetActionCtrlModeAnim(self, 0, 0x12, 0, dir);
            ActTrio27(self, dir, 1, dir);
            ActTrio28(self, dir, 1, dir);
            ActTrio27(self, dir, 1, 0x1D);
            break;
        case 2:
        case 7:
        case 8:
        {
            s32 zero = 0;

            ACT_CALL1(self, m20, 0x10);
            ACT_CALL2(self, m50, self->part, 3);
            self->frames = zero;
            ActQueue27(self, zero, 0x1D);
            break;
        }
        }
    }
    {
        s32 held = (u16)(INPUT_HELD(in) & 0x200);

        if (held)
        {
            if (self->state == 3 && HasTurboRun(gLevelState))
            {
                s32 zero;

                self->turboRun = 1;
                zero = 0;
                ACT_CALL1(self, m20, 4);
                ACT_CALL2(self, m50, self->part, 0x18);
                ActTrio27(self, zero, 1, 0x1B);
            }
        }
        else if (self->state == 4)
        {
            self->turboRun = held;
            StartActionCtrlRun(self);
        }
        else if (self->frame != 0)
        {
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
    if (self->part->contact & 4)
    {
        struct act_part *part;
        s32 frame;
        s32 count;

        ACT_VCALL1(self, m20, 0x1A);
        ACT_VCALL2(self, m50, self->part, 0x15);
        part = self->part;
        frame = 2;
        count = part->bank->records[part->tag].frameCount;
        if (frame >= count)
            frame = count - 1;
        part->frame = frame;
        ClearPlayerSpeedY(part);
        ActSetContact(self->part, 0);
        return;
    }
    {
        u32 in = gKeys;
        u8 busy = self->spinCooldown;

        if (busy == 0 && (INPUT_PRESSED(in) & 2))
        {
            s32 frames;

            PlaySfx(gAudioContext, 0xA, 0x100);
            frames = 0x18;
            ACT_VCALL1(self, m20, 0xE);
            ACT_VCALL2(self, m50, self->part, 0x10);
            self->frame = busy;
            self->frames = frames;
            self->unk_21 = busy;
            self->charge = busy;
            self->unk_22 = busy;
            self->unk_23 = busy;
            self->unk_24[0] = busy;
            gPlayer[0x92] = busy;
            return;
        }
    }
    {
        struct act_part *part = self->part;

        if (part->animDone)
        {
            u32 cur = gKeys;

            if ((cur & 1) && (cur & 0x30))
            {
                if (part->tag == 6)
                {
                    ACT_VCALL1(self, m20, 9);
                }
                else
                {
                    ACT_VCALL1(self, m20, 9);
                    ACT_VCALL2(self, m50, self->part, 6);
                }
                {
                    u8 *slot = &self->motionY;

                    if (*slot == 7)
                        ActSetNextP(self, slot, 0xA);
                }
            }
            else
            {
                ACT_VCALL1(self, m20, 7);
                ACT_VCALL2(self, m50, self->part, 0xC);
                {
                    u8 *slot = &self->motionY;

                    if (*slot == 7)
                    {
                        s32 one = 1;

                        asm("" : "+r"(one));

                        if (cur & 1)
                            ActNext28P(self, slot, one, 9);
                        else
                            ActNext28P(self, slot, one, 8);
                    }
                }
            }
        }
    }
    if (GetDpadDirection(gInput) <= 2)
    {
        if (gPlayer[0x100] == 0)
        {
            self->motionXKeepSpeed = 0;
            self->motionXPending = 1;
            self->motionX = 0;
        }
    }
    else
    {
        u8 *slot = &self->motionX;

        if (*slot == 0x1B || *slot == 0x1C)
        {
            ActHold27P(self, slot, 0x1C);
        }
        else if (self->frame != 0)
        {
            if (*slot != 0xD)
                ActSetNext27P(self, slot, 0xD);
        }
        else if (gPlayer[0x100] == 0)
        {
            ActSetNext27P(self, slot, 7);
        }
    }
    UpdatePlayerFacing(self);
}
asm(".align 2, 0");
