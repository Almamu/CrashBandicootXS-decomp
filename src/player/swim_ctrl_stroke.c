#include "core.h"
#include "player_ctrl.h"
#include "util.h"
#include "system.h"
#include "audio.h"
#include "player.h"
#include "objects.h"
#include "level.h"

/* GitHub issue #19: 0x080159F8-0x08015DF8, the first two of the three
 * jump-table dispatchers of the player-input controller class
 * (include/player_ctrl.h) documented in
 * docs/matching/issue-19-0x08015840-actor.md. The third, ApplyPlayerCtrlSwimDrift, is
 * swim_ctrl_stroke.c. All three are called from swim_ctrl.c's
 * per-state handlers.
 *
 * Built with the older compiler, tools/agbcc/bin/old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS), like swim_ctrl.c right after it. Under old_agbcc
 * these are plain C; the "extra scratch-register copy" that kept them
 * NAKED under the current agbcc is simply old_agbcc's register allocation.
 *
 * Both dispatch on `tilt` (a 13-step direction index). The case bodies
 * appear in the ROM in the order below (the source order); the shared
 * tails are gcc's cross-jumping, not gotos. */

#define KEEP 0x7FFFFFFF

extern u32 gRoomFrameCount;
extern void *gAudioContext;
extern void *gInput;

/* `v`, mirrored when the target faces left */
#define SIGNED_X(t, v) ((t)->f28.flipX ? -(v) : (v))

/* Sets the target's speed for the current `tilt` (state 4 instead reads
 * the frame-indexed stack copy of gStaticData_0816C090, negated unless
 * `mode` is 6) and steps `tilt` towards 0/3/6/9/12. */
void StartPlayerCtrlStroke(struct player_ctrl *self)
{
    struct pctrl_target *t;

    self->deadline = gRoomFrameCount + 16;
    if (self->state == 4)
    {
        struct speed_table tbl = gStaticData_0816C090;

        if (self->mode == 6)
            self->target->speedX = tbl.v[self->target->frame];
        else
            self->target->speedX = -tbl.v[self->target->frame];
        return;
    }

    t = self->target;
    /* the ROM re-stores the byte it just read (ldrb/strb); a plain
     * self-assignment is deleted by the optimizer */
    *(volatile u8 *)&t->tag = t->tag;
    ResetSpriteFrameTimer(t);
    ResetSpriteFrameIndex(t);
    SetSpriteAnimDone(t, 0);
    SetPlayerCtrlState(self, 2, 2, KEEP, KEEP);

    switch (self->tilt)
    {
    case 1:
        self->target->speedX = SIGNED_X(self->target, 176);
        self->target->speedY = -704;
        self->tilt = 0;
        break;
    case 2:
        self->target->speedX = SIGNED_X(self->target, 352);
        self->target->speedY = -704;
        self->tilt = 3;
        break;
    case 4:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = -352;
        self->tilt = 3;
        break;
    case 5:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = -176;
        self->tilt = 6;
        break;
    case 7:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = 176;
        self->tilt = 6;
        break;
    case 8:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = 352;
        self->tilt = 9;
        break;
    case 10:
        self->target->speedX = SIGNED_X(self->target, 352);
        self->target->speedY = 704;
        self->tilt = 9;
        break;
    case 11:
        self->target->speedX = SIGNED_X(self->target, 176);
        self->target->speedY = 704;
        self->tilt = 12;
        break;
    case 6:
        {
            s32 v = SIGNED_X(self->target, 704);
            self->target->speedX = v;
        }
        break;
    case 3:
        self->target->speedX = SIGNED_X(self->target, 528);
        self->target->speedY = -528;
        break;
    case 0:
        self->target->speedY = -704;
        break;
    case 9:
        self->target->speedX = SIGNED_X(self->target, 528);
        self->target->speedY = 528;
        break;
    case 12:
        self->target->speedY = 704;
        break;
    }
}

/* The same dispatch with a fixed speed of 960 and a 3/4 factor on the
 * diagonals. `speed` is a u16: gcc then knows `speed * 3` is non-negative
 * and divides by 4 with a plain shift, while `-speed * 3 / 4` gets the
 * round-toward-zero adjustment (fold() distributes `* 3 / 4` over the
 * SIGNED_X ternary, so each arm is divided separately). `flag` zeroes the
 * horizontal speed when the target's +0x68 bits are set and the D-pad is
 * not held sideways. */
void StartPlayerCtrlSpin(struct player_ctrl *self)
{
    u16 speed;
    u8 flag;
    struct pctrl_target *t;

    if (self->spinCooldown != 0)
        return;

    speed = 960;
    flag = 0;
    PlaySfx(gAudioContext, 9, 256);
    if ((self->target->hitAxes & 3) && GetDpadDirection(gInput) <= 2)
        flag = 1;

    if (self->state == 4)
    {
        if (flag)
        {
            self->target->speedX = 0;
        }
        else
        {
            s32 v = speed;
            if (self->mode == 7)
                v = -speed;
            self->target->speedX = v;
        }
        return;
    }

    SetPlayerCtrlState(self, 3, 3, 0, 24);
    switch (self->tilt)
    {
    case 6:
        if (!flag)
        {
            s32 v;
            if (self->target->f28.flipX) v = -speed; else v = speed;
            self->target->speedX = v;
        }
        else
            self->target->speedX = 0;
        break;
    case 3:
        if (!flag)
        {
            s32 v = SIGNED_X(self->target, speed) * 3 / 4;
            self->target->speedX = v;
        }
        else
            self->target->speedX = 0;
        self->target->speedY = -speed * 3 / 4;
        break;
    case 0:
        {
            s32 v = -speed;
            self->target->speedY = v;
        }
        break;
    case 9:
        if (!flag)
        {
            s32 v = SIGNED_X(self->target, speed) * 3 / 4;
            self->target->speedX = v;
        }
        else
            self->target->speedX = 0;
        self->target->speedY = speed * 3 / 4;
        break;
    case 12:
        self->target->speedY = speed;
        break;
    }
}

/* GitHub issue #19: 0x08015DF8-0x08015FDC, the third jump-table dispatcher
 * of the player-input controller class (include/player_ctrl.h), after
 * StartPlayerCtrlStroke/StartPlayerCtrlSpin above - see
 * docs/matching/issue-19-0x08015840-actor.md. Built with old_agbcc (the
 * Makefile's OLD_AGBCC_OBJS): the "scratch-register copy" before each
 * `>> 2` (`adds r5,r0,r5; adds r1,r5,#0; asrs r6,r1,#2`) that kept this
 * NAKED under the current agbcc is what old_agbcc emits for plain C. */

/* the object SpawnEffectPart returns */
struct spawned
{
    u8 unk_00[0xC];
    u8 unk_0C_0:2; // 0x0C
    u8 unk_0C_2:1;
    u8 unk_0C_3:5;
};

extern void *gEntitySpawner;

/* Picks three tuning values by `state` - `mag` (always 300), `valB` and
 * `valA` - reads the D-pad direction (GetDpadDirection), on a 1-in-128 frame
 * tick and a coin flip spawns a kind-4 object at the target's position via
 * SpawnEffectPart (clearing its +0x0C bit 2), then feeds the direction's
 * (valB/valA, +-mag) pair, 3/4-scaled on the diagonals, to SetPlayerSwimDriftX and
 * SetPlayerSwimDriftY. `mag`/`valB`/`valA` are unsigned, so `x * 3 / 4` is a plain
 * shift and only `-mag * 3 / 4` rounds toward zero. */
void ApplyPlayerCtrlSwimDrift(struct player_ctrl *self)
{
    u16 mag;
    u8 valB;
    u8 valA;
    u8 dir;

    if (self->state == 2)
    {
        mag = 300;
        valB = 20;
        valA = 30;
    }
    else if (self->state == 3)
    {
        mag = 300;
        valB = 20;
        valA = 32;
    }
    else
    {
        mag = 300;
        valB = 15;
        valA = 5;
    }

    dir = GetDpadDirection(gInput);
    if ((gRoomFrameCount & 0x7F) == 0 && (u16)RandRange(2) == 0)
    {
        struct pctrl_target *t = self->target;
        s32 x = t->x >> 8;
        s32 y = (t->y >> 8) - 20;
        s32 flip = t->f28.flipX;
        struct spawned *obj = SpawnEffectPart(gEntitySpawner, 40, 4, x, y, flip);

        if (obj != NULL)
            obj->unk_0C_2 = 0;
    }

    switch (dir)
    {
    case 5:
        SetPlayerSwimDriftX(0, valB * 3 / 4, -mag * 3 / 4);
        SetPlayerSwimDriftY(0, valB * 3 / 4, -mag * 3 / 4);
        break;
    case 6:
        SetPlayerSwimDriftX(0, valB * 3 / 4, mag * 3 / 4);
        SetPlayerSwimDriftY(0, valB * 3 / 4, -mag * 3 / 4);
        break;
    case 8:
        SetPlayerSwimDriftX(0, valB * 3 / 4, mag * 3 / 4);
        SetPlayerSwimDriftY(0, valB * 3 / 4, mag * 3 / 4);
        break;
    case 7:
        SetPlayerSwimDriftX(0, valB * 3 / 4, -mag * 3 / 4);
        SetPlayerSwimDriftY(0, valB * 3 / 4, mag * 3 / 4);
        break;
    case 0:
        SetPlayerSwimDriftY(0, valA, 0);
        SetPlayerSwimDriftX(0, valA, 0);
        break;
    case 3:
        SetPlayerSwimDriftX(0, valB, -mag);
        SetPlayerSwimDriftY(0, valA, 0);
        break;
    case 4:
        SetPlayerSwimDriftX(0, valB, mag);
        SetPlayerSwimDriftY(0, valA, 0);
        break;
    case 1:
        SetPlayerSwimDriftX(0, valA, 0);
        SetPlayerSwimDriftY(0, valB, -mag);
        break;
    case 2:
        SetPlayerSwimDriftX(0, valA, 0);
        SetPlayerSwimDriftY(0, valB, mag);
        break;
    }
}
