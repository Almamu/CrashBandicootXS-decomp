#include "core.h"
#include "actor_self.h"
#include "audio.h"
#include "actor.h"
#include "bosses.h"
#include "globals.h"

/* Same boss-weapon subsystem as airship_fireball.c - see that file's header
 * comment and docs/matching/archive/issue-58-0x08030334-actor.md.
 * `gAirship` is the same small tracker object airship_fall.c
 * documents. */

/* Countdown timer (`gAirshipHp -= delta`) driving the boss-
 * weapon's "charge" bar: while it's still running, just plays a tick
 * sound and re-arms `gAirshipHitFlashTimer`'s DMA-refresh counter; once it
 * expires, resets the whole accumulator/velocity group
 * (`gAirshipVelX`/`gAirshipVelY`/`gAirshipVelZ`) and
 * fires the state-4/table-index-1 transition on the small tracker
 * object at `gAirship`. */
void DamageAirship(s32 delta)
{
    s32 remaining = gAirshipHp - delta;

    gAirshipHp = remaining;
    gAirshipHitFlashTimer = 0x12;

    if (remaining <= 0) {
        struct actor_self *self;

        gAirshipHp = 0;
        gAirshipVelX = 0;
        gAirshipVelY = 0;
        gAirshipVelZ = 0xaa;
        {
            register s32 four asm("r1") = 4;
            register s32 one asm("r2") = 1;

            gAirshipState = four;
            gAirshipStateTimer = 0;

            self = gAirship;
            self->animIndex = one;
        }
        {
            register u16 anim asm("r0") = self->anims[1].duration;
            register u8 zero1 asm("r1") = 0;

            *(u16 *)&self->animTimer = anim;
            *(u8 *)&self->animDone = zero1;
        }

        {
            s32 frame = GetAnimFrameBaseOffset(self);
            register s32 idx asm("r2") = self->animIndex;
            register u8 *table asm("r3") = (u8 *)self->anims;
            register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
            register s32 four asm("r2");
            register s32 val asm("r1");

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four);

            if (frame >= val) {
                self->animTime = 0;
            }
        }
    } else {
        PlaySfx(gAudioContext, 0x43, 0x100);
    }
}
