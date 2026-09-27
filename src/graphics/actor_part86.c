#include "core.h"
#include "player_ctrl.h"

/* GitHub issue #19: 0x080159F8-0x08015DF8, the first two of the three
 * jump-table dispatchers of the player-input controller class
 * (include/player_ctrl.h) documented in
 * docs/matching/issue-19-0x08015840-actor.md. The third, sub_8015DF8, is
 * actor_part86b.c. All three are called from actor_part_16048.c's
 * per-state handlers.
 *
 * Built with the older compiler, tools/agbcc/bin/old_agbcc (the Makefile's
 * OLD_AGBCC_OBJS), like actor_part_16048.c right after it. Under old_agbcc
 * these are plain C; the "extra scratch-register copy" that kept them
 * NAKED under the current agbcc is simply old_agbcc's register allocation.
 *
 * Both dispatch on `level` (a 13-step direction index). The case bodies
 * appear in the ROM in the order below (the source order); the shared
 * tails are gcc's cross-jumping, not gotos. */

#define KEEP 0x7FFFFFFF

/* the 8-word per-frame speed table copied to the stack by sub_80159F8 */
struct speed_table
{
    s32 v[8];
};

extern u32 gUnknown_0300082C;
extern void *gUnknown_030012BC;
extern void *gUnknown_03001304;
extern const struct speed_table gStaticData_0816C090;

extern u8 sub_8000760(void *arg);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_80087C0(struct pctrl_target *t);
extern void sub_80087B4(struct pctrl_target *t);
extern void sub_800872C(struct pctrl_target *t, s32 a);
extern void sub_8017264(struct player_ctrl *self, s32 a, s32 mode, s32 timer, s32 timerMax);

/* `v`, mirrored when the target faces left */
#define SIGNED_X(t, v) ((t)->f28.flipX ? -(v) : (v))

/* Sets the target's speed for the current `level` (state 4 instead reads
 * the frame-indexed stack copy of gStaticData_0816C090, negated unless
 * `mode` is 6) and steps `level` towards 0/3/6/9/12. */
void sub_80159F8(struct player_ctrl *self)
{
    struct pctrl_target *t;

    self->unk_28 = gUnknown_0300082C + 16;
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
    sub_80087C0(t);
    sub_80087B4(t);
    sub_800872C(t, 0);
    sub_8017264(self, 2, 2, KEEP, KEEP);

    switch (self->level)
    {
    case 1:
        self->target->speedX = SIGNED_X(self->target, 176);
        self->target->speedY = -704;
        self->level = 0;
        break;
    case 2:
        self->target->speedX = SIGNED_X(self->target, 352);
        self->target->speedY = -704;
        self->level = 3;
        break;
    case 4:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = -352;
        self->level = 3;
        break;
    case 5:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = -176;
        self->level = 6;
        break;
    case 7:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = 176;
        self->level = 6;
        break;
    case 8:
        self->target->speedX = SIGNED_X(self->target, 704);
        self->target->speedY = 352;
        self->level = 9;
        break;
    case 10:
        self->target->speedX = SIGNED_X(self->target, 352);
        self->target->speedY = 704;
        self->level = 9;
        break;
    case 11:
        self->target->speedX = SIGNED_X(self->target, 176);
        self->target->speedY = 704;
        self->level = 12;
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
void sub_8015C6C(struct player_ctrl *self)
{
    u16 speed;
    u8 flag;
    struct pctrl_target *t;

    if (self->cooldown != 0)
        return;

    speed = 960;
    flag = 0;
    PlaySfx(gUnknown_030012BC, 9, 256);
    if ((self->target->unk_68 & 3) && sub_8000760(gUnknown_03001304) <= 2)
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

    sub_8017264(self, 3, 3, 0, 24);
    switch (self->level)
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
