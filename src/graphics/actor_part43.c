#include "core.h"
#include "actor_self.h"

/* Same large per-instance "self" object family as actor_part17.c/
 * actor_part18.c/actor_part19.c/actor_part20.c (state at `self+0x28`,
 * table-index at `self+0xc`, an anim-frame halfword/byte pair at
 * `self+0x10`/`self+0x12`, an accumulator at `self+8`, a "part table"
 * pointer at `self+0`), part of a second boss-weapon "spawn/pre-
 * attack" singleton whose own flags/counters live at
 * `gUnknown_030014E0`-`gUnknown_03001518` - a different singleton
 * cluster than issue #58's `gUnknown_03001534` one and issue #62's
 * `gUnknown_030015AC` one. See docs/matching/issue-56-0x0802f0dc-actor.md
 * and docs/status/actor.md. */

extern void sub_8029BAC(s32 arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void FreezeLevelClock(void *arg0, s32 arg1);
extern s32 sub_802A4D4(void);
extern s32 __divsi3(s32 arg0, s32 arg1);
extern s32 AddLife(void *self);
extern void *gUnknown_030012BC;
extern void *gLevelState;
extern u8 gUnknown_03001504;
extern u8 gUnknown_03001506;
extern u8 gUnknown_03001507;
extern s32 gUnknown_03001508;
extern s32 gUnknown_0300150C;
extern s32 gUnknown_030014E4;
extern s32 gUnknown_030014EC;
extern s32 gUnknown_030014F0;
extern s32 gUnknown_030014F8;
extern s32 gUnknown_030014FC;

struct actor_hp {
    struct actor_self base;
    s32 hp;             // 0x54 - refilled by sub_802F164, capped at gUnknown_030014E4
};

/* Constructor/reset: while the singleton flag (`gUnknown_03001506`) is
 * off, resets `self` to state 5/table-index 4 (idle-ish), plays a cue,
 * and - only if the current game-mode flag at `gLevelState+0x8c`
 * is set - fires an extra one-shot effect via `FreezeLevelClock`. */
void sub_802F0DC(void *selfArg)
{
    register struct actor_self *self asm("r4") = selfArg;
    register s32 zero asm("r5") = gUnknown_03001506;

    if (zero == 0) {
        gUnknown_03001507 = zero;
        gUnknown_03001504 = 1;
        gUnknown_03001506 = 1;
        sub_8029BAC(0x3c);
        gUnknown_03001508 = zero;
        gUnknown_0300150C = zero;
        {
            register s32 five asm("r0") = 5;
            register s32 four asm("r1") = 4;

            self->state = five;
            self->stateTime = zero;
            self->animIndex = four;
        }
        {
            register u16 anim asm("r0") = self->anims[4].duration;
            register u8 zero2 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero2;
        }
        self->animTime = zero;
        PlaySfx(gUnknown_030012BC, 0x3b, 0x100);
        if (*((u8 *)gLevelState + 0x8c) != 0) {
            FreezeLevelClock(gLevelState, 0x2710);
        }
    }
}

/* State-machine update for the same singleton (issue #56): while `self`
 * is in one of the "active" states (5-state-history via
 * `self+0x28` == 1/6/2/3), resets `self`'s table index/anim to the idle
 * frame if it wasn't already, latches the target position at `self+0x1c`/
 * `self+0x20` from the two arguments, and re-arms state 6 (playing a cue
 * only on the *first* transition into it). While the current game-mode
 * flag at `gLevelState+0x8c` is clear and the `gUnknown_030014EC`
 * frame-timer has advanced far enough (>0x14 frames since the last pass),
 * drives a 5-case round-robin (`gUnknown_030014F0`, wrapping 0-4) once
 * every >0xbe-frame window: cases 0-2 feed the `gUnknown_030014FC` reward
 * accumulator (by 1/5/0x14) while not paused, case 3 advances `self+0x54`'s
 * own accumulator (clamped to `gUnknown_030014E4`) while the other
 * singleton flag is clear, and case 4 fires a one-shot effect plus a cue.
 * Every path through the round-robin (taken or not) re-samples the
 * frame timer and advances the round-robin index. */
void sub_802F164(void *selfArg, s32 xArg, s32 yArg)
{
    register struct actor_hp *self asm("r5") = selfArg;
    register s32 x asm("r3") = xArg;
    register s32 y asm("r4") = yArg;
    s32 state = self->base.state;
    u8 paused;

    if (state != 1 && state != 6 && state != 2 && state != 3) {
        return;
    }

    if (self->base.animIndex != 5) {
        self->base.animIndex = 5;
        {
            register u16 anim asm("r0") = self->base.anims[5].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->base.animTimer = anim;
            *(u8 *)&self->base.animDone = zero1;
            self->base.animTime = zero2;
        }
    }

    self->base.x = x;
    self->base.y = y;

    if (self->base.state != 6) {
        sub_8029BAC(0x50);
    }
    self->base.state = 6;

    {
        register s32 *p1508 asm("r2") = &gUnknown_03001508;
        register s32 *p150c asm("r1") = &gUnknown_0300150C;
        register s32 zero asm("r0") = 0;

        *p150c = zero;
        *p1508 = zero;
        self->base.stateTime = zero;
    }

    paused = *((u8 *)gLevelState + 0x8c);
    if (paused != 0) {
        return;
    }

    if (sub_802A4D4() - gUnknown_030014EC <= 0x14) {
        return;
    }

    if (sub_802A4D4() - gUnknown_030014EC > 0xbe) {
        gUnknown_030014F0 = paused;
    }

    switch (gUnknown_030014F0) {
    case 0:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            if (gUnknown_030014FC == 0) {
                gUnknown_030014F8 = 0xf;
            }
            gUnknown_030014FC += 1;
        }
        break;
    case 1:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            if (gUnknown_030014FC == 0) {
                gUnknown_030014F8 = 0xf;
            }
            gUnknown_030014FC += 5;
        }
        break;
    case 2:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            if (gUnknown_030014FC == 0) {
                gUnknown_030014F8 = 0xf;
            }
            gUnknown_030014FC += 0x14;
        }
        break;
    case 3:
        if (gUnknown_03001506 == 0) {
            register s32 *maxPtr asm("r4") = &gUnknown_030014E4;
            register s32 max asm("r1") = *maxPtr;
            register s32 mul asm("r0") = 0x14;
            s32 v = self->hp + __divsi3(max * mul, 0x64);

            self->hp = v;
            {
                register s32 cap asm("r4") = *maxPtr;

                if (v > cap) {
                    self->hp = cap;
                }
            }
        }
        break;
    case 4:
        if (*((u8 *)gLevelState + 0x8c) == 0) {
            AddLife(gLevelState);
            PlaySfx(gUnknown_030012BC, 7, 0x100);
        }
        break;
    }

    gUnknown_030014EC = sub_802A4D4();
    gUnknown_030014F0++;
    if (gUnknown_030014F0 == 5) {
        gUnknown_030014F0 = 0;
    }
}
