#include "core.h"
#include "actor_self.h"

/* Same large per-instance "self" object as actor_part19.c/actor_part19g.c
 * (`struct actor_self`: state, anim index/timer/done flag, state timer,
 * anim accumulator, anim table pointer and method table), part of a
 * boss-weapon effect state machine - see
 * docs/matching/issue-58-0x08030334-actor.md and docs/status/actor.md. */

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *gAudioContext;

struct actor_timed {
    struct actor_self base;
    s32 timer;          // 0x54 - countdown until the transition fires
};

/* Countdown `timer`: once it reaches zero, plays a sound, sets the
 * "table-index 4" tag at `unk_18`, and fires the state-2/anim-1
 * transition (ACTOR_SET_STATE's stores, with the ROM's registers
 * pinned). The `animTimer`/`animDone` stores go through a cast of the
 * field's address: a plain member store is marked as a struct access,
 * which lets the scheduler move the zero into a different register. */
void sub_8030530(void *selfArg, s32 delta)
{
    struct actor_timed *self = selfArg;

    self->timer -= delta;
    if (self->timer <= 0) {
        self->base.unk_18 = 4;
        PlaySfx(gAudioContext, 4, 0x100);
        {
            register s32 stateVal asm("r0") = 2;
            register s32 idxVal asm("r1") = 1;

            self->base.state = stateVal;
            {
                register s32 zero asm("r2") = 0;

                self->base.stateTime = zero;
                self->base.animIndex = idxVal;
                {
                    register u16 anim asm("r0") = self->base.anims[1].duration;
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)&self->base.animTimer = anim;
                    *(u8 *)&self->base.animDone = zero2;
                }
                self->base.animTime = zero;
            }
        }
    }
}
