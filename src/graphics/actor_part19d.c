#include "core.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after the still-raw `sub_802C7A8` (see docs/matching.md) - same
 * `self` object and conventions documented there. */

extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern void *gUnknown_03000884;

extern u8 sub_802A6EC(void *self);
extern u8 sub_802DD9C(void *self);
extern void sub_8022FEC(void *self);
extern void sub_802C128(void *arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_802C4C8(void *selfArg);

/* On proximity (`sub_802A6EC`), ties the lap counter and the lock-timer
 * setter `sub_802C128`, then transitions to the shared "used" state
 * (anim frame from `self`'s part-table pointer at `+0xd8`). Whether or
 * not that fired, on `sub_802DD9C`'s overlap test transitions a second
 * time with its own sound cue - both share the same state-0x12
 * transition block (plus `self+0x18 = 1`) before tail-calling the
 * shared cleanup `sub_802C4C8`. */
void sub_802C904(void *selfArg)
{
    u8 *self = selfArg;

    if (*(s32 *)(self + 0xc) != 0x12 && sub_802A6EC(self)) {
        sub_8022FEC(gUnknown_030012C0);
        sub_802C128(gUnknown_03000884);
        *(s32 *)(self + 0xc) = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xd8);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
            *(s32 *)(self + 8) = zero2;
        }
        *(s32 *)(self + 0x18) = 1;
    }

    if (*(s32 *)(self + 0xc) != 0x12 && sub_802DD9C(self)) {
        PlaySfx(gUnknown_030012BC, 3, 0x100);
        sub_8022FEC(gUnknown_030012C0);
        *(s32 *)(self + 0xc) = 0x12;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xd8);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
            *(s32 *)(self + 8) = zero2;
        }
        *(s32 *)(self + 0x18) = 1;
    }

    sub_802C4C8(self);
}

asm(".align 2, 0");
