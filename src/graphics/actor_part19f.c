#include "core.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after the parked `sub_802C208` (see actor_part19e.c) - same `self`
 * object and conventions documented there. */

extern u8 gUnknown_030014A0;
extern void *gUnknown_030012BC;

extern s32 GetAnimFrameBaseOffset(void *self);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);

/* Constant getter - returns `gUnknown_030014A0`. */
u8 sub_802C264(void)
{
    return gUnknown_030014A0;
}

/* Sets `self+0x14`, advances `self+0x1c`/`self+0x20` by `self+0x54`/
 * `self+0x58` (a velocity pair), and once both exceed `0x1000`: reads
 * ahead by `self+0x10`'s current anim value into the `+8` accumulator
 * and, once the frame counter reaches the same `+4`-halfword-of-a-0xc
 * table-entry threshold `sub_802C0BC` uses, backs the accumulator off
 * by the entry's `+4`/`+6` halfword delta and marks `self+0x12`.
 * Otherwise (the common per-frame case) just plays a sound cue and
 * fires the `self+0x50` trampoline. */
void sub_802C270(void *selfArg)
{
    u8 *self = selfArg;
    s32 x, y;

    *(s32 *)(self + 0x14) = 1;

    x = *(s32 *)(self + 0x1c) + *(s32 *)(self + 0x54);
    *(s32 *)(self + 0x1c) = x;
    y = *(s32 *)(self + 0x20) + *(s32 *)(self + 0x58);
    *(s32 *)(self + 0x20) = y;

    if (x > 0x1000 && y > 0x1000) {
        goto frameBlock;
    }

    PlaySfx(gUnknown_030012BC, 0xe, 0x100);
    if (self != 0) {
        u8 *table = *(u8 **)(self + 0x50);
        sub_803AD80(self + *(s16 *)(table + 8), (void *)3, *(void **)(table + 0xc));
    }
    return;

frameBlock:
    {
        register s32 sixteenConst asm("r3") = 0x10;
        register s32 delta asm("r1") = *(s16 *)(self + sixteenConst);

        *(s32 *)(self + 8) += delta;
        self[0x12] = 0;
    }
    {
        s32 frame = GetAnimFrameBaseOffset(self);
        register s32 idx asm("r2") = *(s32 *)(self + 0xc);
        register u8 *table asm("r3") = *(u8 **)self;
        register u8 *entryPtr asm("r1") = (u8 *)(idx * 0xc);
        register s32 four asm("r3");
        register s32 e4 asm("r2");

        asm("add %0, %0, %1" : "+r" (entryPtr) : "r" (table));
        four = 4;
        e4 = *(s16 *)(entryPtr + four);

        if (frame >= e4) {
            register s32 six asm("r0") = 6;
            register s32 e6 asm("r1") = *(s16 *)(entryPtr + six);
            register s32 diff asm("r1") = e4 - e6;

            diff <<= 8;
            *(s32 *)(self + 8) -= diff;
            self[0x12] = 1;
        }
    }
}

asm(".align 2, 0");
