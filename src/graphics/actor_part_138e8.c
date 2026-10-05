#include "core.h"
#include "action_obj.h"

/* GitHub issue #17, ROM 0x080138E8-0x08013C60 (details in
 * docs/matching/issue-17-0x08012fbc-actor.md, "Third pass"). Two more
 * gActionCtrlStateTable action-table handlers for the player/action object
 * (include/action_obj.h). Built with old_agbcc. */

extern u32 gKeys;
extern void *gAudioContext;
extern void *gLevelState;
extern void *gInput;
extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern u8 GetDpadDirection(void *pad);
extern u8 PlayerHasRoomForAnim(struct act_part *part, s32 action);
extern u8 HasSuperBodySlam(void *self);
extern u8 HasTurboRun(void *self);
extern void StartActionCtrlHighJump(struct act *self);
extern void StartActionCtrlSpin(struct act *self);
extern void ActionCtrlStateCrawl(struct act *self);
extern void StartActionCtrlRun(struct act *self);

/* Queues action `next` on the +0x31/+0x2F/+0x27 trio. As inline
 * parameters, old_agbcc materializes the values before the three stores;
 * `ActQueue27` stores a literal 1 for +0x2F, `ActTrio27` a caller value. */
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

/* Picks the part animation from its state: with tag 6, animation 9 on
 * frame 3 or 8 past it (or once finished); otherwise, once finished, 0x19
 * plus part animation 7 if HasSuperBodySlam allows it, else 0x18. */
void ActionCtrlStateFlipBodySlamStart(struct act *self)
{
    struct act_part *part = self->part;

    if (part->tag == 6)
    {
        s32 frame = part->frame;

        if (frame == 3)
            ACT_VCALL2(self, m50, part, 9);
        else if (frame > 3 || part->animDone)
            ACT_VCALL2(self, m50, part, 8);
    }
    else if (part->animDone)
    {
        if (HasSuperBodySlam(gLevelState))
        {
            ACT_VCALL1(self, m20, 0x19);
            ACT_VCALL2(self, m50, self->part, 7);
        }
        else
        {
            ACT_VCALL1(self, m20, 0x18);
        }
    }
}

/* The "player input/action handling" reader of docs/rom_map.md. Out of
 * contact it queues idle (5); otherwise the confirm edge
 * (PlayerHasRoomForAnim(part, 0xB)) hands off to StartActionCtrlHighJump and the alt edge
 * (PlayerHasRoomForAnim(part, 0x10)) to StartActionCtrlSpin. Then it counts the animation
 * (holding the part on frame 3 until +0x18 reaches +0x1C) and, once the
 * part's animation is done, dispatches on contact, the 0x100/0x200 held
 * bits, the D-pad and PlayerHasRoomForAnim(part, 2).
 *
 * The +0x2F store of the alt path and the 0x1B path reuses the 1 already
 * in a register (the `held & 1` test's, then +0x29's); the two final
 * VCALL2+trio tails are written out twice, as the ROM cross-jumps them
 * from the method call on. */
void ActionCtrlStateSlide(struct act *self)
{
    u32 in = gKeys;

    {
        struct act_part *part = self->part;

        if (part->contact == 0)
        {
            ActSetNext(self, 5);
        }
        else if (INPUT_HELD(in) & 1)
        {
            if (PlayerHasRoomForAnim(part, 0xB) == 1)
            {
                PlaySfx(gAudioContext, 0xC, 0x100);
                ActAndFlags0D(self->part, -2);
                ActAndFlags0D(self->part, -3);
                StartActionCtrlHighJump(self);
                return;
            }
        }
        else if (INPUT_PRESSED(in) & 2)
        {
            if (PlayerHasRoomForAnim(part, 0x10) == 1)
            {
                StartActionCtrlSpin(self);
                ActTrio27(self, 0, 1, 1);
                return;
            }
        }
    }

    if (++self->frame < self->frames)
    {
        struct act_part *part = self->part;
        s32 frame;
        s32 count;

        part->stepTimer = 0;
        frame = 3;
        count = part->bank->records[part->tag].frameCount;
        if (frame >= count)
            frame = count - 1;
        part->frame = frame;
        return;
    }
    {
        struct act_part *part = self->part;
        u8 contact;

        if (!part->animDone)
            return;
        contact = part->contact;
        if (contact == 0)
        {
            ACT_VCALL1(self, m20, 0x1A);
            ACT_VCALL2(self, m50, self->part, 0x1B);
            ActSetNext(self, 4);
            return;
        }
    }
    {
        u16 held = INPUT_HELD(in) & 0x100;

        if (held)
        {
            s32 zero = 0;

            ACT_VCALL1(self, m20, 0x14);
            ACT_VCALL2(self, m50, self->part, 0);
            self->frames = zero;
            ActQueue27(self, zero, 3);
            ActionCtrlStateCrawl(self);
            return;
        }
        {
            u8 dir = GetDpadDirection(gInput);

            if (dir != 0 && PlayerHasRoomForAnim(self->part, 2))
            {
                switch (dir)
                {
                case 3 ... 4:
                    if ((INPUT_HELD(in) & 0x200) && HasTurboRun(gLevelState))
                    {
                        self->turboRun = 1;
                        ACT_VCALL1(self, m20, 4);
                        ACT_VCALL2(self, m50, self->part, 0x18);
                        ActTrio27(self, held, 1, 0x1B);
                        return;
                    }
                    StartActionCtrlRun(self);
                    return;
                }
                ACT_VCALL1(self, m20, 0x12);
                ACT_VCALL2(self, m50, self->part, 2);
                ActQueue27(self, 0, 0);
                return;
            }
            else
            {
                u8 hit = PlayerHasRoomForAnim(self->part, 2);

                if (hit == 1)
                {
                    ACT_VCALL1(self, m20, 0x12);
                    ACT_VCALL2(self, m50, self->part, 2);
                    ActTrio27(self, 0, hit, 0);
                    return;
                }
                ACT_VCALL1(self, m20, 0x11);
                ACT_VCALL2(self, m50, self->part, 4);
                ActQueue27(self, 0, 0);
            }
        }
    }
}
