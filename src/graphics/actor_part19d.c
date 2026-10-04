#include "core.h"
#include "actor_self.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after the still-raw `sub_802C7A8` (see docs/matching.md) - same
 * `self` object and conventions documented there. */

extern void *gAudioContext;
extern void *gLevelState;
extern void *gUnknown_03000884;

extern u8 sub_802A6EC(void *self);
extern u8 sub_802DD9C(void *self);
extern void sub_8022FEC(void *self);
extern void sub_802C128(void *arg0);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void sub_802C4C8(void *selfArg);

/* On proximity (`sub_802A6EC`), ties the lap counter and the lock-timer
 * setter `sub_802C128`, then transitions to the shared "used"
 * animation sequence 0x12. Whether or
 * not that fired, on `sub_802DD9C`'s overlap test transitions a second
 * time with its own sound cue - both share the same state-0x12
 * transition block (plus `self->unk_18 = 1`) before tail-calling the
 * shared cleanup `sub_802C4C8`.
 *
 * The `*(T *)&self->...` stores are deliberate: through a pointer they aren't
 * marked as struct-member accesses, which keeps gcc's scheduler from
 * moving the `anims[0x12]` load below the zero constants (the ROM loads
 * it first); plain member stores reorder it. */
void sub_802C904(struct actor_self *self)
{
    if (self->animIndex != 0x12 && sub_802A6EC(self)) {
        sub_8022FEC(gLevelState);
        sub_802C128(gUnknown_03000884);
        self->animIndex = 0x12;
        {
            register u16 anim asm("r0") = self->anims[0x12].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            self->animTime = zero2;
        }
        self->unk_18 = 1;
    }

    if (self->animIndex != 0x12 && sub_802DD9C(self)) {
        PlaySfx(gAudioContext, 3, 0x100);
        sub_8022FEC(gLevelState);
        self->animIndex = 0x12;
        {
            register u16 anim asm("r0") = self->anims[0x12].duration;
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
            self->animTime = zero2;
        }
        self->unk_18 = 1;
    }

    sub_802C4C8(self);
}

asm(".align 2, 0");
