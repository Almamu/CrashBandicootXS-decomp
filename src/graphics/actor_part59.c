#include "core.h"
#include "memory.h"

/* More of the `gUnknown_030014BC`-rooted object's lifecycle (see
 * actor_part58.c's header comment): a state-flag setter, its
 * destructor, and its constructor. */

extern s32 gUnknown_030014D0;

/* Arms `gUnknown_030014D0 = 3` - a state value none of this chunk's
 * other functions read back, plausibly consumed by the vtable-dispatch
 * caller itself. */
void sub_802DFBC(void)
{
    gUnknown_030014D0 = 3;
}

extern void *gUnknown_030014BC;

/* Destructor: frees the object. */
void sub_802DFC8(void)
{
    mem_free(gUnknown_030014BC);
}

extern s32 gUnknown_030014D4;
extern s32 gUnknown_030014C4;
extern s32 gUnknown_030014CC;
extern s32 gUnknown_030014C8;
extern u8 gStaticData_0817A850[];
extern u8 gStaticData_0817A880[];
extern void sub_803B0A8(void *self, s32 idx);
extern s32 sub_8029B2C(void);
extern void sub_8029E34(s32 arg0);
extern void sub_802DE70(void);

/* Constructor: stashes the caller's argument in `gUnknown_030014D4`,
 * allocates and wires up a fresh instance (part table
 * `gStaticData_0817A850`/`0817A880`, header byte `0xf`, reset via
 * `sub_803B0A8`) into `gUnknown_030014BC`, resets the position-tracking
 * pair (`gUnknown_030014C4` to 0, `030014CC` to `0xA000`,
 * `030014C8` derived the same way `sub_802DB2C`/`sub_802DCC0` do),
 * primes `sub_8029E34`, clears `gUnknown_030014D0`, and finally calls
 * `sub_802DE70` (the object's own initial VRAM-pattern/DMA setup,
 * parked separately - see docs/matching/issue-54-actor-d3a8.md). */
void sub_802DFDC(void *arg0)
{
    u8 *obj;
    register u32 size asm("r0");
    register s32 flags asm("r1");
    register void **bcAddr asm("r5");

    gUnknown_030014D4 = (s32)arg0;
    bcAddr = &gUnknown_030014BC;
    asm volatile("mov %0, #0x1c" : "=r"(size));
    asm volatile("mov %0, #0x80\n\tlsl %0, %0, #0x18" : "=r"(flags));
    obj = mem_alloc(size, flags);
    {
        u8 *v0 = gStaticData_0817A850;
        u8 *v1 = gStaticData_0817A880;
        s32 v2 = 0xf;

        *(u8 **)obj = v0;
        *(u8 **)(obj + 4) = v1;
        *(s32 *)(obj + 0x18) = v2;
    }
    sub_803B0A8(obj, 0);
    *bcAddr = obj;

    gUnknown_030014C4 = 0;
    gUnknown_030014CC = 0xa000;
    gUnknown_030014C8 = (sub_8029B2C() << 8) - gUnknown_030014CC;
    sub_8029E34(gUnknown_030014CC);

    gUnknown_030014D0 = 0;
    sub_802DE70();
}

asm(".align 2, 0");
