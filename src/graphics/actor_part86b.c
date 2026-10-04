#include "core.h"
#include "player_ctrl.h"

/* GitHub issue #19: 0x08015DF8-0x08015FDC, the third jump-table dispatcher
 * of the player-input controller class (include/player_ctrl.h), after
 * actor_part86.c's sub_80159F8/sub_8015C6C - see
 * docs/matching/issue-19-0x08015840-actor.md. Built with old_agbcc (the
 * Makefile's OLD_AGBCC_OBJS): the "scratch-register copy" before each
 * `>> 2` (`adds r5,r0,r5; adds r1,r5,#0; asrs r6,r1,#2`) that kept this
 * NAKED under the current agbcc is what old_agbcc emits for plain C. */

/* the object sub_8025BAC returns */
struct spawned
{
    u8 unk_00[0xC];
    u8 unk_0C_0:2; // 0x0C
    u8 unk_0C_2:1;
    u8 unk_0C_3:5;
};

extern void *gUnknown_03001304;
extern u32 gUnknown_0300082C;
extern void *gEntitySpawner;
extern u8 sub_8000760(void *arg);
extern s32 sub_8000E1C(s32 max);
extern struct spawned *sub_8025BAC(void *pool, s32 arg1, s32 kind, s32 x, s32 y, s32 arg5);
extern void sub_80172D0(s32 a, s32 b, s32 c);
extern void sub_8015FDC(s32 a, s32 b, s32 c);

/* Picks three tuning values by `state` - `mag` (always 300), `valB` and
 * `valA` - reads the D-pad direction (sub_8000760), on a 1-in-128 frame
 * tick and a coin flip spawns a kind-4 object at the target's position via
 * sub_8025BAC (clearing its +0x0C bit 2), then feeds the direction's
 * (valB/valA, +-mag) pair, 3/4-scaled on the diagonals, to sub_80172D0 and
 * sub_8015FDC. `mag`/`valB`/`valA` are unsigned, so `x * 3 / 4` is a plain
 * shift and only `-mag * 3 / 4` rounds toward zero. */
void sub_8015DF8(struct player_ctrl *self)
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

    dir = sub_8000760(gUnknown_03001304);
    if ((gUnknown_0300082C & 0x7F) == 0 && (u16)sub_8000E1C(2) == 0)
    {
        struct pctrl_target *t = self->target;
        s32 x = t->x >> 8;
        s32 y = (t->y >> 8) - 20;
        s32 flip = t->f28.flipX;
        struct spawned *obj = sub_8025BAC(gEntitySpawner, 40, 4, x, y, flip);

        if (obj != NULL)
            obj->unk_0C_2 = 0;
    }

    switch (dir)
    {
    case 5:
        sub_80172D0(0, valB * 3 / 4, -mag * 3 / 4);
        sub_8015FDC(0, valB * 3 / 4, -mag * 3 / 4);
        break;
    case 6:
        sub_80172D0(0, valB * 3 / 4, mag * 3 / 4);
        sub_8015FDC(0, valB * 3 / 4, -mag * 3 / 4);
        break;
    case 8:
        sub_80172D0(0, valB * 3 / 4, mag * 3 / 4);
        sub_8015FDC(0, valB * 3 / 4, mag * 3 / 4);
        break;
    case 7:
        sub_80172D0(0, valB * 3 / 4, -mag * 3 / 4);
        sub_8015FDC(0, valB * 3 / 4, mag * 3 / 4);
        break;
    case 0:
        sub_8015FDC(0, valA, 0);
        sub_80172D0(0, valA, 0);
        break;
    case 3:
        sub_80172D0(0, valB, -mag);
        sub_8015FDC(0, valA, 0);
        break;
    case 4:
        sub_80172D0(0, valB, mag);
        sub_8015FDC(0, valA, 0);
        break;
    case 1:
        sub_80172D0(0, valA, 0);
        sub_8015FDC(0, valB, -mag);
        break;
    case 2:
        sub_80172D0(0, valA, 0);
        sub_8015FDC(0, valB, mag);
        break;
    }
}
