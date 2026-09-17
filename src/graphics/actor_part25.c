#include "core.h"

/* Same boss-weapon subsystem as actor_part20.c - see that file's header
 * comment and docs/matching/issue-58-0x08030334-actor.md.
 * `gUnknown_03001534` is the same small tracker object actor_part23.c
 * documents. */

extern s32 GetAnimFrameBaseOffset(void *self);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *gUnknown_030012BC;
extern s32 gUnknown_0300156C;
extern s32 gUnknown_03001578;
extern s32 gUnknown_03001558;
extern s32 gUnknown_0300155C;
extern s32 gUnknown_03001560;
extern s32 gUnknown_03001538;
extern s32 gUnknown_0300153C;
extern void *gUnknown_03001534;

/* Countdown timer (`gUnknown_0300156C -= delta`) driving the boss-
 * weapon's "charge" bar: while it's still running, just plays a tick
 * sound and re-arms `gUnknown_03001578`'s DMA-refresh counter; once it
 * expires, resets the whole accumulator/velocity group
 * (`gUnknown_03001558`/`gUnknown_0300155C`/`gUnknown_03001560`) and
 * fires the state-4/table-index-1 transition on the small tracker
 * object at `gUnknown_03001534`. */
void sub_803146C(s32 delta)
{
    s32 remaining = gUnknown_0300156C - delta;

    gUnknown_0300156C = remaining;
    gUnknown_03001578 = 0x12;

    if (remaining <= 0) {
        u8 *self;

        gUnknown_0300156C = 0;
        gUnknown_03001558 = 0;
        gUnknown_0300155C = 0;
        gUnknown_03001560 = 0xaa;
        {
            register s32 four asm("r1") = 4;
            register s32 one asm("r2") = 1;

            gUnknown_03001538 = four;
            gUnknown_0300153C = 0;

            self = gUnknown_03001534;
            *(s32 *)(self + 0xc) = one;
        }
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
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
                *(s32 *)(self + 8) = 0;
            }
        }
    } else {
        PlaySfx(gUnknown_030012BC, 0x43, 0x100);
    }
}
