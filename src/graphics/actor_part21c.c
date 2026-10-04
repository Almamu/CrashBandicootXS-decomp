#include "core.h"
#include "actor_self.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c/actor_part25.c - see actor_part20.c's header comment
 * and docs/matching/issue-58-0x08030334-actor.md. */

extern s32 GetAnimFrameBaseOffset(void *self);
extern void sub_802A4F8(void);
extern s32 gAirshipZ;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001554;
extern s32 gUnknown_03001558;
extern s32 gUnknown_0300155C;
extern s32 gAirshipState;
extern s32 gUnknown_0300153C;
extern struct actor_self *gAirship;

/* Boss-weapon camera-relative position accumulator: advances
 * `gAirshipZ` by its per-frame delta (`gUnknown_03001560`),
 * and - while `gUnknown_03001554` hasn't crossed its ceiling
 * (`0x81FF`) - resets the velocity group (`gUnknown_0300155C`/
 * `gUnknown_03001558`) and fires the state-2/table-index-0 transition
 * on the small tracker object (`gAirship`, anim frame from
 * its own part-table pointer at `+0`), then always calls
 * `sub_802A4F8`. Same "pin the zero constant so it's loaded before its
 * address" idiom as `AirshipStateFall` (actor_part23.c) - the value is
 * reused across four stores that would otherwise get reordered ahead
 * of the address loads that consume them. The `*(T *)&self->...` stores keep
 * gcc from treating them as struct-member accesses, which would let the
 * scheduler move the `anims[0]` load below the zero constant. */
void AirshipStateApproach(void)
{
    gAirshipZ += gUnknown_03001560;

    if (gUnknown_03001554 <= 0x81FF) {
        struct actor_self *self;
        s32 *p1558 = &gUnknown_03001558;
        s32 *p155C = &gUnknown_0300155C;
        register s32 zero asm("r5") = 0;

        *p155C = zero;
        *p1558 = zero;
        {
            register s32 two asm("r1") = 2;
            gAirshipState = two;
        }
        gUnknown_0300153C = zero;

        self = gAirship;
        self->animIndex = zero;
        {
            register u16 anim asm("r0") = self->anims[0].duration;
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
            val = *(s16 *)(entryPtr + four); /* anims[idx].loopThreshold */

            if (frame >= val) {
                self->animTime = zero;
            }
        }

        sub_802A4F8();
    }
}
