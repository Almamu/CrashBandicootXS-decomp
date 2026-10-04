#include "core.h"
#include "actor_self.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after `sub_802C208` (matched C, see actor_part19e.c) - same `self`
 * object and conventions documented there. */

/* `self` with the velocity pair this class adds after the common
 * prefix. */
struct moving_actor {
    struct actor_self base;
    s32 velX;                   // 0x54
    s32 velY;                   // 0x58
};

extern u8 gUnknown_030014A0;
extern void *gAudioContext;

extern s32 GetAnimFrameBaseOffset(void *self);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);

/* Constant getter - returns `gUnknown_030014A0`. */
u8 sub_802C264(void)
{
    return gUnknown_030014A0;
}

/* Sets `sortKey`, advances `x`/`y` by the velocity pair, and once both
 * exceed `0x1000`: adds the (signed) `animTimer` into the `animTime`
 * accumulator and, once the frame counter reaches the current anim
 * record's `loopThreshold` (the same test `sub_802C0BC` uses), backs
 * the accumulator off by `loopThreshold - loopBase` and marks
 * `animDone`. Otherwise (the common per-frame case) just plays a sound
 * cue and fires the vtable's `destroy` method with 3. */
void sub_802C270(void *selfArg)
{
    struct moving_actor *self = selfArg;
    s32 x, y;

    self->base.sortKey = 1;

    x = self->base.x + self->velX;
    self->base.x = x;
    y = self->base.y + self->velY;
    self->base.y = y;

    if (x > 0x1000 && y > 0x1000) {
        goto frameBlock;
    }

    PlaySfx(gAudioContext, 0xe, 0x100);
    if (self != 0) {
        struct actor_vtable *table = self->base.vtable;
        _call_via_r2((u8 *)self + table->destroy.thisOffset, (void *)3, table->destroy.fn);
    }
    return;

frameBlock:
    {
        /* `animTimer` read as an s16 through a register offset, as the
         * ROM does. */
        register s32 sixteenConst asm("r3") = 0x10;
        register s32 delta asm("r1") = *(s16 *)((u8 *)self + sixteenConst);

        self->base.animTime += delta;
        self->base.animDone = 0;
    }
    {
        s32 frame = GetAnimFrameBaseOffset(self);
        register s32 idx asm("r2") = self->base.animIndex;
        register u8 *table asm("r3") = (u8 *)self->base.anims;
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
            self->base.animTime -= diff;
            self->base.animDone = 1;
        }
    }
}

asm(".align 2, 0");
