#include "core.h"
#include "memory.h"

/* Continuation of actor_part19.c's player/action-object family, right
 * after the parked `sub_802C2FC` (see actor_part19b.c) - same `self`
 * object and conventions documented there. */

extern void *gUnknown_030012C0;

extern u8 gStaticData_087E4DF4[];
extern u8 gStaticData_087E4E74[];

extern void sub_8023430(void *self);

/* Same "iterate `self+0x5c` times draining `gUnknown_030012C0`,
 * retarget the vtable to the 'dead' state, unlink from the circular
 * `+0x48`/`+0x4c` list, free on `arg1 & 1`" teardown shape as
 * `sub_802C19C` above, but with a plain iteration count instead of a
 * `gUnknown_03001488` global drain. */
void sub_802C394(void *selfArg, u32 arg1)
{
    register u8 *self asm("r4") = selfArg;
    u32 arg1r = arg1;
    s32 i;

    *(u8 **)(self + 0x50) = gStaticData_087E4E74;

    for (i = 0; i < *(s32 *)(self + 0x5c); i++) {
        sub_8023430(gUnknown_030012C0);
    }

    *(u8 **)(self + 0x50) = gStaticData_087E4DF4;

    {
        u8 *prev = *(u8 **)(self + 0x4c);
        u8 *next = *(u8 **)(self + 0x48);
        *(u8 **)(prev + 0x48) = next;
    }
    {
        u8 *next = *(u8 **)(self + 0x48);
        u8 *prev = *(u8 **)(self + 0x4c);
        *(u8 **)(next + 0x4c) = prev;
    }

    if (arg1r & 1) {
        mem_free(self);
    }
}

asm(".align 2, 0");
