#include "core.h"

/* Same "self" object family as actor_part28.c - see that file's header
 * comment and docs/matching/issue-62-0x08033804-actor.md. */

extern void *gUnknown_030012BC;

extern void sub_8033804(void);
extern void sub_803388C(void);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);

/* Applies `dmg` damage to `self+0x54`, and once it drops to zero (or
 * below), marks `self` dead (`self+0x6c=1`), fires the singleton's own
 * death transition (`sub_803388C`), switches `self` to its death
 * state/anim (state 2, table-index 2, anim frame from `self`'s part
 * table at `+0x18`), and plays the death sound; otherwise just plays a
 * hit sound. */
void sub_8033AE0(void *selfArg, s32 dmg)
{
    u8 *self = selfArg;

    sub_8033804();
    *(s32 *)(self + 0x54) -= dmg;

    if (*(s32 *)(self + 0x54) <= 0) {
        u8 *deadFlag = self + 0x6c;
        register s32 zero asm("r4") = 0;

        *deadFlag = 1;
        sub_803388C();
        {
            register s32 stateVal asm("r0") = 2;

            *(s32 *)(self + 0x28) = stateVal;
            *(s32 *)(self + 0x44) = zero;
            *(s32 *)(self + 0xc) = stateVal;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0x18);
                register u8 zero2 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero2;
            }
            *(s32 *)(self + 8) = zero;
        }
        PlaySfx(gUnknown_030012BC, 4, 0x100);
    } else {
        PlaySfx(gUnknown_030012BC, 0x45, 0x100);
    }
}
