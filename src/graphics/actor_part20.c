#include "core.h"

/* Same large per-instance "self" object as actor_part19.c/actor_part19g.c
 * (state at `+0x28`, table-index at `+0xc`, an anim-frame halfword/byte
 * pair at `+0x10`/`+0x12`, a counter at `+0x44`, an accumulator at `+8`,
 * a "part table" pointer at `+0`, and an event/trampoline table pointer
 * at `+0x50` this time), part of a boss-weapon effect state machine -
 * see docs/matching/issue-58-0x08030334-actor.md and
 * docs/status/actor.md. */

extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *gUnknown_030012BC;

/* Countdown timer at `self+0x54`: once it reaches zero, plays a sound,
 * sets the "table-index 4" tag at `self+0x18`, and fires the state-2/
 * table-index-1 transition (anim frame from `self`'s part-table pointer
 * at `+0xc`). */
void sub_8030530(void *selfArg, s32 delta)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x54) -= delta;
    if (*(s32 *)(self + 0x54) <= 0) {
        *(s32 *)(self + 0x18) = 4;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
        {
            register s32 stateVal asm("r0") = 2;
            register s32 idxVal asm("r1") = 1;

            *(s32 *)(self + 0x28) = stateVal;
            {
                register s32 zero asm("r2") = 0;

                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = idxVal;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                    register u8 zero2 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero2;
                }
                *(s32 *)(self + 8) = zero;
            }
        }
    }
}
