#include "core.h"

/* Same boss-weapon "self"/tracker object family as actor_part20.c/
 * actor_part23.c/actor_part25.c - see actor_part20.c's header comment
 * and docs/matching/issue-58-0x08030334-actor.md. */

extern s32 GetAnimFrameBaseOffset(void *self);
extern void sub_802A4F8(void);
extern s32 gUnknown_03001548;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001554;
extern s32 gUnknown_03001558;
extern s32 gUnknown_0300155C;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern void *gUnknown_03001534;

/* Boss-weapon camera-relative position accumulator: advances
 * `gUnknown_03001548` by its per-frame delta (`gUnknown_03001560`),
 * and - while `gUnknown_03001554` hasn't crossed its ceiling
 * (`0x81FF`) - resets the velocity group (`gUnknown_0300155C`/
 * `gUnknown_03001558`) and fires the state-2/table-index-0 transition
 * on the small tracker object (`gUnknown_03001534`, anim frame from
 * its own part-table pointer at `+0`), then always calls
 * `sub_802A4F8`. Same "pin the zero constant so it's loaded before its
 * address" idiom as `sub_8030C98` (actor_part23.c) - the value is
 * reused across four stores that would otherwise get reordered ahead
 * of the address loads that consume them. */
void sub_80306AC(void)
{
    gUnknown_03001548 += gUnknown_03001560;

    if (gUnknown_03001554 <= 0x81FF) {
        u8 *self;
        s32 *p1558 = &gUnknown_03001558;
        s32 *p155C = &gUnknown_0300155C;
        register s32 zero asm("r5") = 0;

        *p155C = zero;
        *p1558 = zero;
        {
            register s32 two asm("r1") = 2;
            gUnknown_03001538 = two;
        }
        gUnknown_0300153C = zero;

        self = gUnknown_03001534;
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }

        {
            s32 frame = GetAnimFrameBaseOffset(self);
            register s32 idx asm("r2") = *(s32 *)(self + 0xc);
            register u8 *table asm("r3") = *(u8 **)self;
            register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
            register s32 four asm("r2");
            register s32 val asm("r1");

            asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
            four = 4;
            val = *(s16 *)(entryPtr + four);

            if (frame >= val) {
                *(s32 *)(self + 8) = zero;
            }
        }

        sub_802A4F8();
    }
}
