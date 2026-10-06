#include "core.h"
#include "match.h"
#include "action_obj.h"
#include "system.h"
#include "audio.h"
#include "player.h"
#include "level.h"
#include "globals.h"

/* GitHub issue #17, ROM 0x080134B8-0x080138E8 (details in
 * docs/matching/archive/issue-17-0x08012fbc-actor.md, "Third pass"). Built with
 * old_agbcc. */

/* The spark object SpawnEffectPart spawns, as far as it is used. */
struct spark
{
    u8 unk_00[0xC];
    u8 unk_0C_0:2;
    u8 unk_0C_2:1;
    u8 unk_0C_3:5;
    u8 unk_0D[0x13];
    struct act_anim_bank *bank; // 0x20
    u8 unk_24[4];
    u8 unk_28_0:2;         // 0x28
    u8 unk_28_2:6;
    u8 unk_29[4];
    u8 tag;                // 0x2D
    u8 unk_2E[2];
    s32 frame;             // 0x30
};

/* Byte masks with the mask as an `s32` parameter: the AND stays in SImode
 * (a plain `*p & -0x11` is narrowed to 0xEF), so the -0x11 the ROM derives
 * from the 1 already in r7 (`subs r7, #0x12`) can be shared by both
 * sparks. */
static inline void AndByte(u8 *p, s32 clear)
{
    *p &= clear;
}

static inline void OrMaskByte(u8 *p, s32 clear, s32 set)
{
    *p = (*p & clear) | set;
}

/* The coordinates are inline parameters, so they are computed before the
 * pool pointer is loaded, as in the ROM. */
static inline struct spark *SpawnSpark(s32 x, s32 y, s32 mirror)
{
    return SpawnEffectPart(gEntitySpawner, 0x29, 1, x, y, mirror);
}

/* The "next action" trio stores (include/action_obj.h's ActSetNext for
 * the other trio): inline parameters are materialized before the stores. */
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

static inline void ActSetNextP(struct act *self, u8 *slot, s32 next)
{
    self->motionYKeepSpeed = 0;
    self->motionYPending = 1;
    *slot = next;
}

static inline void ActSetContact(struct player *p, s32 v)
{
    p->hitAxes = v;
}

/* The table's shared airborne handler (docs/rom_map.md, reused by 6 of
 * the 42 slots: states 7, 9, 0xB, 0x18, 0x19 and 0x1A). The alt edge restarts the charge animation (as in
 * ActionCtrlStateJump); out of contact it ticks the +0x25 countdown and otherwise
 * queues action 4 from state 0x1A. On contact bit 2 it lands the part (frame
 * 2, ClearPlayerSpeedY); on contact bit 3 in states 0x18/0x19 it spawns two
 * sparks at the player (+-0x14 px, the first mirrored) and plays 0x16/0x11;
 * state 0xE queues from the shoulder bits; the rest plays 0x17/0x16.
 *
 * The first spark's bit-2 clear is written twice: the second store folds
 * away, but its extra use of the -5 mask is what gives that mask sb (and
 * the -0x11 r8) as in the ROM. */
void ActionCtrlStateAirborne(struct act *self)
{
    void *pad = gInput;
    u32 in = gKeys.all;
    u8 contact = self->part->hitAxes;
    u8 dir = GetDpadDirection(pad);
    s32 state = self->state;

    if (state != 0xE)
    {
        u8 busy = self->spinCooldown;

        if (busy == 0 && (INPUT_PRESSED(in) & 2) && (u32)(state - 0x18) > 1)
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
            gPlayer->bounce = busy;
        }
    }
    if (contact == 0)
    {
        u8 *timer;

        UpdatePlayerFacing(self);
        timer = &self->unk_24[1];
        if (*timer != 0)
        {
            (*timer)--;
            if (dir == 0)
                *timer = contact;
            return;
        }
        HandleActionCtrlAirInput(self);
        if (self->state == 0x1A)
        {
            u8 *slot = &self->motionY;

            if (*slot == 0)
                ActSetNextP(self, slot, 4);
        }
        return;
    }
    {
        u8 bit4 = contact & 4;

        if (bit4)
        {
            if (self->state != 0x1A)
            {
                ActOrFlags0D(self->part, 1);
                self->slamBlocked = 0;
                if (self->state != 0xE)
                {
                    struct player *part;
                    s32 frame;
                    s32 count;

                    ACT_VCALL1(self, m20, 0x1A);
                    ACT_VCALL2(self, m50, self->part, 0x15);
                    part = self->part;
                    frame = 2;
                    count = part->anim->records[part->tag].frameCount;
                    if (frame >= count)
                        frame = count - 1;
                    part->frame = frame;
                }
                ClearPlayerSpeedY(self->part);
            }
            UpdatePlayerFacing(self);
            HandleActionCtrlAirInput(self);
            ActSetContact(self->part, 0);
            return;
        }
        if (contact == 1 || contact == 2)
        {
            UpdatePlayerFacing(self);
            HandleActionCtrlAirInput(self);
            self->part->hitAxes = bit4;
            return;
        }
        if ((contact & 8) && self->part->speedY >= 0)
        {
            s32 st;

            ACT_PART_FLAGS0D(self->part) |= 1;
            self->slamBlocked = bit4;
            st = self->state;
            if ((u32)(st - 0x18) <= 1)
            {
                struct spark *obj;
                s32 x;
                s32 y;
                s32 frame;
                s32 count;

                x = gPlayer->x;
                x >>= 8;
                x += 0x14;
                y = gPlayer->y;
                y >>= 8;
                y += 0xC;
                obj = SpawnSpark(x, y, 1);
                obj->unk_28_0 = 1;
                obj->unk_0C_2 = 0;
                obj->unk_0C_2 = 0;
                OrMaskByte((u8 *)obj + 0x28, -0x11, 0x10);
                frame = 3;
                count = obj->bank->records[obj->tag].frameCount;
                if (frame >= count)
                    frame = count - 1;
                obj->frame = frame;

                x = gPlayer->x;
                x >>= 8;
                x -= 0x14;
                y = gPlayer->y;
                y >>= 8;
                y += 0xC;
                obj = SpawnSpark(x, y, bit4);
                obj->unk_28_0 = 1;
                obj->unk_0C_2 = 0;
                AndByte((u8 *)obj + 0x28, -0x11);
                frame = 3;
                count = obj->bank->records[obj->tag].frameCount;
                if (frame >= count)
                    frame = count - 1;
                obj->frame = frame;

                if (self->state == 0x19)
                    DoSuperBodySlamShockwave(self);
                if (self->state != 0x1D)
                {
                    PlaySfx(gAudioContext, 0x19, 0x100);
                    ACT_VCALL1(self, m20, 0x16);
                    ACT_VCALL2(self, m50, self->part, 0x11);
                    self->motionYKeepSpeed = bit4;
                    self->motionYPending = 1;
                    self->motionY = bit4;
                }
                return;
            }
            if (st == 0xE)
            {
                u16 held = INPUT_HELD(in) & 0x30;

                if (held)
                    ActTrio27(self, bit4, 1, 1);
                else
                {
                    self->motionXKeepSpeed = held;
                    self->motionXPending = 1;
                    self->motionX = held;
                }
                ActSetNext(self, 0);
                if (self->unk_22)
                    ACT_VCALL1(self, m20, 0xF);
                else
                    ACT_VCALL1(self, m20, 0xD);
                return;
            }
            if (gPlayer->slippery)
            {
                ACT_VCALL1(self, m20, 0x17);
                ACT_VCALL2(self, m50, self->part, 0x16);
            }
            else
            {
                ACT_VCALL1(self, m20, 0x17);
                ACT_VCALL2(self, m50, self->part, 0x16);
            }
            ActQueue27(self, 0, 0);
            ActSetNext(self, 0);
            return;
        }
    }
    ActQueue27(self, 0, 0);
}
asm(".align 2, 0");

/* GitHub issue #17, ROM 0x080138E8-0x08013C60 (details in
 * docs/matching/archive/issue-17-0x08012fbc-actor.md, "Third pass"). Two more
 * gActionCtrlStateTable action-table handlers for the player/action object
 * (include/action_obj.h). Built with old_agbcc. */

/* Picks the part animation from its state: with tag 6, animation 9 on
 * frame 3 or 8 past it (or once finished); otherwise, once finished, 0x19
 * plus part animation 7 if HasSuperBodySlam allows it, else 0x18. */
void ActionCtrlStateFlipBodySlamStart(struct act *self)
{
    struct player *part = self->part;

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
        if ((u8)HasSuperBodySlam(gLevelState))
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
    u32 in = gKeys.all;

    {
        struct player *part = self->part;

        if (part->hitAxes == 0)
        {
            ActSetNext(self, 5);
        }
        else if (INPUT_HELD(in) & 1)
        {
            if (PlayerHasRoomForAnim((struct box_part *)part, 0xB) == 1)
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
            if (PlayerHasRoomForAnim((struct box_part *)part, 0x10) == 1)
            {
                StartActionCtrlSpin(self);
                ActTrio27(self, 0, 1, 1);
                return;
            }
        }
    }

    if (++self->frame < self->frames)
    {
        struct player *part = self->part;
        s32 frame;
        s32 count;

        part->stepTimer = 0;
        frame = 3;
        count = part->anim->records[part->tag].frameCount;
        if (frame >= count)
            frame = count - 1;
        part->frame = frame;
        return;
    }
    {
        struct player *part = self->part;
        u8 contact;

        if (!part->animDone)
            return;
        contact = part->hitAxes;
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

            if (dir != 0 && PlayerHasRoomForAnim((struct box_part *)self->part, 2))
            {
                switch (dir)
                {
                case 3 ... 4:
                    if ((INPUT_HELD(in) & 0x200) && (u8)HasTurboRun(gLevelState))
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
                u8 hit = PlayerHasRoomForAnim((struct box_part *)self->part, 2);

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

/* GitHub issue #17, ROM 0x08013C60-0x0801426C, formerly
 * asm/code_3_2_17_12af4.s (details in
 * docs/matching/archive/issue-17-0x08012fbc-actor.md, "Second pass").
 *
 * Five more entries of the gActionCtrlStateTable 42-slot action table
 * (docs/rom_map.md), the same player/action object as
 * action_ctrl_states.c: `self+0xc` is its method table,
 * `self+0x10` its on-screen part, `self+0x18`/`+0x1c` an animation
 * counter/limit, and the +0x27..+0x32 bytes the shared "next action"
 * trio (action_ctrl_states.c). Each handler snapshots the input word
 * gKeys (the high half is the newly-pressed buttons) and
 * most also the D-pad direction GetDpadDirection remaps. */

/* ActSetNext for the "fire" paths below. There the ROM loads a fresh 1 for
 * +0x30; plain C reuses the 1 of the preceding `pressed & 1` test, which
 * gcc then keeps in a callee-saved register across the method call (an
 * extra push). The barrier hides the constant, and the pins give its
 * store the ROM's registers. */
static inline void ActSetNextB(struct act *self, s32 next)
{
    register s32 one asm("r0");
    register u8 *flag asm("r1");

    self->motionYKeepSpeed = 0;
    flag = &self->motionYPending;
    one = 1;
    MATCH_KEEP(one);
    *flag = one;
    self->motionY = next;
}

/* Charge-attack step: queues idle (5) once the part leaves contact, jumps
 * to action 7 on fire during contact bit 3, counts alt presses into
 * +0x20 (max 3), and at the animation's end either releases the charge
 * (StartActionCtrlTornadoSpin) or hands off to EndActionCtrlSpin. The handlers below load
 * GetDpadDirection's argument before taking the input snapshot, hence the
 * `pad` local. */
void ActionCtrlStateSpin(struct act *self)
{
    u32 in;
    u8 dir;

    {
        void *pad = gInput;

        in = gKeys.all;
        dir = GetDpadDirection(pad);
    }
    if (self->part->hitAxes == 0)
    {
        ActSetNext(self, 5);
    }
    if ((INPUT_PRESSED(in) & 1) && (self->part->hitAxes & 8))
    {
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        ACT_VCALL1(self, m20, 0xE);
        ActSetNextB(self, 7);
        self->unk_22 = 0;
        self->unk_23 = 0;
        self->part->hitAxes = 0;
        return;
    }
    if ((u8)HasTornadoSpin(gLevelState) && (INPUT_PRESSED(in) & 2) && self->spinCooldown == 0)
    {
        if (++self->charge > 3)
            self->charge = 3;
    }
    SteerActionCtrlSpin(self, dir);
    if (++self->frame >= self->frames || self->part->animDone)
    {
        if (self->charge)
            StartActionCtrlTornadoSpin(self, 0xF, 0xD);
        else
            EndActionCtrlSpin(self, dir, in);
    }
}

void ActionCtrlStateAirSpin(struct act *self)
{
    u32 in;
    struct player *part;

    in = gKeys.all;
    part = self->part;

    if ((part->hitAxes & 8) && part->speedY > 0)
    {
        ActOrFlags0D(part, 1);
        self->slamBlocked = 0;
        ACT_VCALL1(self, m20, 0xD);
        self->motionYKeepSpeed = 0;
        self->motionYPending = 1;
        self->motionY = 0;
        ActionCtrlStateSpin(self);
        return;
    }
    if ((u8)HasTornadoSpin(gLevelState) && (INPUT_PRESSED(in) & 2) && self->spinCooldown == 0)
    {
        if (++self->charge > 3)
            self->charge = 3;
    }
    {
        s32 frame = ++self->frame;
        s32 frames = self->frames;

        part = self->part;
        if (frame < frames && !part->animDone)
            goto done;
    }
    {
        u8 charge;

        ActOrFlags0D(part, 1);
        charge = self->charge;
        if (charge)
        {
            StartActionCtrlTornadoSpin(self, 0xE, 0xE);
        }
        else
        {
            self->spinCooldown = 0xC;
            ACT_VCALL1(self, m20, 0x1A);
            ACT_VCALL2(self, m50, self->part, 0x15);
            self->frame = charge;
            self->frames = charge;
        }
    }
done:
    ActionCtrlStateAirborne(self);
}

void ActionCtrlStateTornadoSpin(struct act *self)
{
    u32 in;
    u8 dir;

    {
        void *pad = gInput;

        in = gKeys.all;
        dir = GetDpadDirection(pad);
    }
    if (self->part->hitAxes == 0)
    {
        if (self->unk_22)
        {
            sub_80151C8(self);
        }
        else
        {
            ActSetNext(self, 5);
        }
    }
    if ((INPUT_PRESSED(in) & 1) && (self->part->hitAxes & 8))
    {
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        ACT_VCALL1(self, m20, 0xE);
        ActSetNextB(self, 7);
        self->unk_23 = 0;
        self->part->hitAxes = 0;
        return;
    }
    if ((u8)HasTornadoSpin(gLevelState) && (INPUT_PRESSED(in) & 2) && self->spinCooldown == 0)
    {
        if (++self->charge > 3)
            self->charge = 3;
    }
    SteerActionCtrlSpin(self, dir);
    if (++self->frame >= self->frames || self->part->animDone)
        StartActionCtrlTornadoSpin(self, 0xF, 0xD);
}

void ActionCtrlStateCrouchDown(struct act *self)
{
    u32 in;
    s32 fire;

    in = gKeys.all;
    fire = INPUT_PRESSED(in) & 1;

    if (fire)
    {
        PlaySfx(gAudioContext, 0xC, 0x100);
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        StartActionCtrlHighJump(self);
        return;
    }
    if (self->part->hitAxes == 8 && (u8)(self->motionY - 4) <= 1)
    {
        self->motionYKeepSpeed = fire;
        self->motionYPending = 1;
        self->motionY = fire;
    }
    if (self->part->animDone)
    {
        ACT_VCALL1(self, m20, 0x11);
        ACT_VCALL2(self, m50, self->part, 4);
    }
}

/* On the "confirm" edge (PlayerHasRoomForAnim(part, 0xB)) hands off to
 * StartActionCtrlHighJump like ActionCtrlStateCrawlStart (action_ctrl_states.c); otherwise, unless
 * CheckActionCtrlLeftGround reports busy, turns the part to face the D-pad direction
 * (setting +0x2F), starts a walk (action 3) on a horizontal direction,
 * and - with neither shoulder button held - either starts action 2 on a
 * PlayerHasRoomForAnim(part, 2) hit or falls back to idle.
 *
 * old_agbcc. The facing block reads `self->part` each time (GCSE turns
 * the reloads into the ROM's r2 copy) and spells its two bit tests
 * differently so gcc doesn't thread the second into the first. The
 * second branch writes through a scoped `volatile u8 *` so its `adds r2,
 * #40` stays in place ahead of the -0x11 mask - see
 * docs/matching/archive/issue-15-16-17-naked-retry-2.md. */
void ActionCtrlStateCrouch(struct act *self)
{
    u32 in;
    u8 dir;
    s32 turned;
    s32 moved;

    {
        void *pad = gInput;

        in = gKeys.all;
        dir = GetDpadDirection(pad);
    }
    if ((INPUT_PRESSED(in) & 1) && PlayerHasRoomForAnim((struct box_part *)self->part, 0xB) == 1)
    {
        PlaySfx(gAudioContext, 0xC, 0x100);
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        StartActionCtrlHighJump(self);
        return;
    }
    if (CheckActionCtrlLeftGround(self))
        return;

    turned = 0;
    if ((s32)(self->part->mirror.all << 27) < 0 && (dir == 4 || dir == 6 || dir == 8))
    {
        u8 *p = &self->part->mirror.all;
        s32 m = -0x11;

        m &= *p;
        *p = m;
        self->motionXPending = 1;
        turned = 1;
        goto turn_done;
    }
    if ((s8)(self->part->mirror.all << 3) >= 0 && (dir == 3 || dir == 5 || dir == 7))
    {
        s32 m;

        turned = 1;
        {
            /* volatile: keeps the `+0x28` address in the part copy's
             * register and computed ahead of the -0x11 mask, as in the
             * ROM (a plain pointer lands in a fresh register) */
            volatile u8 *p = &self->part->mirror.all;

            m = -0x11;
            m &= *p;
            m |= 0x10;
            *p = m;
        }
        self->motionXPending = turned;
    }
turn_done:

    moved = 0;
    if (!turned)
    {
        switch (GetDpadDirection(gInput))
        {
        case 3:
        case 4:
        case 7:
        case 8:
            ACT_VCALL1(self, m20, 0x13);
            ACT_VCALL2(self, m50, self->part, 0x14);
            ActQueue27(self, moved, 3);
            moved = 1;
            break;
        }
    }

    {
        s32 held = INPUT_HELD(in) & 0x180;

        if (held == 0)
        {
            u8 hit = PlayerHasRoomForAnim((struct box_part *)self->part, 2);

            if (hit == 1)
            {
                ACT_VCALL1(self, m20, 0x12);
                ACT_VCALL2(self, m50, self->part, 2);
                self->motionXKeepSpeed = held;
                self->motionXPending = hit;
                self->motionX = held;
            }
            else if (!moved)
            {
                ACT_VCALL1(self, m20, 0x11);
                ACT_VCALL2(self, m50, self->part, 4);
                self->motionXKeepSpeed = moved;
                self->motionXPending = 1;
                self->motionX = moved;
                self->motionYKeepSpeed = moved;
                self->motionYPending = 1;
                self->motionY = moved;
            }
        }
    }
}
asm(".align 2, 0");

/* This file (and action_ctrl_land.c, its non-adjacent continuation) covers
 * part of `gActionCtrlStateTable`, the 42-slot per-level action dispatch
 * table documented in docs/rom_map.md ("`gActionCtrlStateTable` is a
 * 42-slot, fully-populated action dispatch table") - `self` is the
 * player/action object those table entries are invoked on, not the
 * small (0x1c-byte) `struct actor` from include/actor.h. `self+0xc` is
 * a per-category table of `{s16 offset; void *fn}` pairs (at least two
 * entries known so far, `+0x20`/`+0x24` and `+0x50`/`+0x54`) fed
 * through the `_call_via_r2`/`_call_via_r3` trampolines together with
 * `self+offset` and `self+0x10` (a "part" sub-object) - the same
 * base+offset+fn-pointer convention already named in ctrl.c's
 * doc comments. The `+0x27`/`+0x28`/`+0x29`/`+0x2f`/`+0x30`/`+0x31`/
 * `+0x32` bytes are a state/flag/table-index trio pair this whole
 * action-table family shares (`struct act`'s motionX/motionY queue,
 * include/action_obj.h). */

extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern s32 _call_via_r3(void *arg0, void *arg1, void *arg2, void *arg3);

/* Clears `part+0x38`'s "busy" flag by resetting the shared
 * flag/counter/table-index trio (`+0x31`/`+0x2f`/`+0x27` and
 * `+0x32`/`+0x30`/`+0x28`) via `SetActionCtrlModeAnim`, but only while that flag
 * is actually set. */
void ActionCtrlStateStandUp(struct act *self)
{
    struct player *part = self->part;

    if (part->animDone != 0) {
        SetActionCtrlModeAnim(self, 0, 0x12, 0, 0);
        self->motionXKeepSpeed = 0;
        self->motionXPending = 1;
        self->motionX = 0;
        self->motionYKeepSpeed = 0;
        self->motionYPending = 1;
        self->motionY = 0;
    }
}

/* On the "confirm" input edge (checked via `PlayerHasRoomForAnim(part, 0xb)`),
 * plays a sound, clears two `part+0xd` bits (the runtime `& -2`/`& -3`
 * negation rather than a folded mask - see docs/matching.md), and hands
 * off to `StartActionCtrlHighJump`. Otherwise, while `part+0x38` is set, fires the
 * usual base+offset+fn-pointer trampoline pair and tail-calls
 * `ActionCtrlStateCrawl` (below). */
void ActionCtrlStateCrawlStart(struct act *self)
{
    u32 snap = *(u32 *)&gKeys;

    if ((*(u16 *)((u8 *)&snap + 2) & 1) != 0
        && PlayerHasRoomForAnim((struct box_part *)self->part, 0xb) == 1) {
        PlaySfx(gAudioContext, 0xc, 0x100);

        {
            register u8 *part asm("r1") = (u8 *)self->part;
            register s32 mask asm("r0") = 2;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }
        {
            register u8 *part asm("r1") = (u8 *)self->part;
            register s32 mask asm("r0") = 3;
            mask = -mask;
            mask &= part[0xd];
            part[0xd] = mask;
        }

        StartActionCtrlHighJump(self);
        return;
    }

    if (self->part->animDone != 0) {
        struct act_vtable *mgr = self->vt;
        _call_via_r2((u8 *)self + mgr->m20.thisOffset, (void *)0x14,
                    mgr->m20.fn);
        {
            struct act_method *off = &self->vt->m50;
            _call_via_r3((u8 *)self + off->thisOffset, self->part,
                        (void *)0, off->fn);
        }
        ActionCtrlStateCrawl(self);
    }
}

/* The shared handler `ActionCtrlStateCrawlStart` tail-calls: same "confirm" edge check
 * (short-circuits before reaching `CheckActionCtrlLeftGround` when it fires), then
 * (once `CheckActionCtrlLeftGround(self)` is clear) dispatches on `GetDpadDirection`'s
 * D-pad-remap result - `1` fires one trampoline pair, `0`/`2` fires
 * another - before falling into a shared tail that, when the input
 * snapshot's `0x180` bits are clear and `PlayerHasRoomForAnim(part, 2)` just
 * fired, runs a third trampoline pair and finishes with
 * `UpdatePlayerFacing`.
 *
 * Formerly NAKED; matches under old_agbcc (this whole file is built with
 * it, see docs/matching/archive/issue-15-16-17-naked-retry-2.md): the case 0/2
 * trio goes through ActQueue27 so its 0 is materialized before the
 * stores, and the case 1 trio is written out in both branches, with the
 * calls in the do/while ACT_VCALL form, so gcc cross-jumps the shared
 * `bl` of the second method call as the ROM does (the if/else ACT_CALL
 * form there changes the whole block). */
void ActionCtrlStateCrawl(struct act *selfArg)
{
    struct act *self = selfArg;
    u32 in = gKeys.all;
    u8 busy;
    u8 dir;
    u8 hit;
    u32 held;

    if ((INPUT_PRESSED(in) & 1) && PlayerHasRoomForAnim((struct box_part *)self->part, 0xB) == 1)
    {
        PlaySfx(gAudioContext, 0xC, 0x100);
        ActAndFlags0D(self->part, -2);
        ActAndFlags0D(self->part, -3);
        StartActionCtrlHighJump(self);
        return;
    }
    busy = CheckActionCtrlLeftGround(self);
    if (busy != 0)
        return;
    dir = GetDpadDirection(gInput);
    switch (dir)
    {
    case 0:
    case 2:
        ACT_CALL1(self, m20, 0x1B);
        ACT_CALL2(self, m50, self->part, 1);
        ActQueue27(self, 0, 0);
        break;
    case 1:
        if (PlayerHasRoomForAnim((struct box_part *)self->part, 2) == 1)
        {
            ACT_VCALL1(self, m20, 0x15);
            ACT_VCALL2(self, m50, self->part, 2);
            self->motionXKeepSpeed = busy;
            self->motionXPending = dir;
            self->motionX = busy;
            break;
        }
        ACT_VCALL1(self, m20, 0x11);
        ACT_VCALL2(self, m50, self->part, 4);
        self->motionXKeepSpeed = busy;
        self->motionXPending = dir;
        self->motionX = busy;
        break;
    }
    held = INPUT_HELD(in) & 0x180;
    if (held == 0 && (hit = PlayerHasRoomForAnim((struct box_part *)self->part, 2)) == 1)
    {
        ACT_CALL1(self, m20, 0x12);
        ACT_CALL2(self, m50, self->part, 2);
        self->motionXKeepSpeed = held;
        self->motionXPending = hit;
        self->motionX = held;
    }
    UpdatePlayerFacing(self);
}
/* Trailing byte count isn't a multiple of 4 - without this, `as` pads
 * with its default NOP fill instead of the ROM's zero fill (see
 * docs/matching.md's alignment-padding gotcha). */
asm(".align 2, 0");
