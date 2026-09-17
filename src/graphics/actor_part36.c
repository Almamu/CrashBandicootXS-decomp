#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern void *gUnknown_030012BC;

extern void sub_8033804(void);
extern void sub_803388C(void);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);

/* `sub_8033AE0`'s gated twin: only applies damage while `self` is in
 * state 1. On death, uses table-index 3 and the anim frame from
 * `self`'s part table `+0x24` field (instead of `sub_8033AE0`'s
 * table-index 2/`+0x18`), and reuses the just-checked `state` value
 * (always 1 here) for the death-flag store, matching the ROM's literal
 * register reuse. */
void sub_8033E18(void *selfArg, s32 dmg)
{
    u8 *self = selfArg;
    s32 state = *(s32 *)(self + 0x28);

    if (state == 1) {
        sub_8033804();
        *(s32 *)(self + 0x54) -= dmg;

        if (*(s32 *)(self + 0x54) <= 0) {
            u8 *deadFlag = self + 0x6c;
            register s32 zero asm("r4") = 0;

            *deadFlag = state;
            sub_803388C();
            {
                register s32 stateVal asm("r0") = 2;
                register s32 three asm("r1") = 3;

                *(s32 *)(self + 0x28) = stateVal;
                *(s32 *)(self + 0x44) = zero;
                *(s32 *)(self + 0xc) = three;
            }
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x24);
                register u8 zero2 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero2;
            }
            *(s32 *)(self + 8) = zero;
            PlaySfx(gUnknown_030012BC, 4, 0x100);
        } else {
            PlaySfx(gUnknown_030012BC, 0x45, 0x100);
        }
    }
}
