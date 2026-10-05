#include "core.h"
#include "memory.h"

/* More of the `gYeti`-rooted object's lifecycle (see
 * actor_part58.c's header comment): a state-flag setter, its
 * destructor, and its constructor. */

extern s32 gYetiState;

/* Arms `gYetiState = 3` - a state value none of this chunk's
 * other functions read back, plausibly consumed by the vtable-dispatch
 * caller itself. */
void StopYeti(void)
{
    gYetiState = 3;
}

extern void *gYeti;

/* Destructor: frees the object. */
void DestroyYeti(void)
{
    mem_free(gYeti);
}

extern s32 gYetiParamsIndex;
extern s32 gYetiX;
extern s32 gYetiDistance;
extern s32 gYetiPosition;
extern u8 gYetiKeyframes[];
extern u8 gYetiFrames[];
extern void SetActorAnim(void *self, s32 idx);
extern s32 GetCellAnimDistance(void);
extern void sub_8029E34(s32 arg0);
extern void LoadYetiGraphics(void);

/* Constructor: stashes the caller's argument in `gYetiParamsIndex`,
 * allocates and wires up a fresh instance (part table
 * `gYetiKeyframes`/`0817A880`, header byte `0xf`, reset via
 * `SetActorAnim`) into `gYeti`, resets the position-tracking
 * pair (`gYetiX` to 0, `030014CC` to `0xA000`,
 * `030014C8` derived the same way `YetiStateChase`/`YetiStateCharge` do),
 * primes `sub_8029E34`, clears `gYetiState`, and finally calls
 * `LoadYetiGraphics` (the object's own initial VRAM-pattern/DMA setup,
 * parked separately - see docs/matching/issue-54-actor-d3a8.md). */
void CreateYeti(void *arg0)
{
    u8 *obj;
    register u32 size asm("r0");
    register s32 flags asm("r1");
    register void **bcAddr asm("r5");

    gYetiParamsIndex = (s32)arg0;
    bcAddr = &gYeti;
    asm volatile("mov %0, #0x1c" : "=r"(size));
    asm volatile("mov %0, #0x80\n\tlsl %0, %0, #0x18" : "=r"(flags));
    obj = mem_alloc(size, flags);
    {
        u8 *v0 = gYetiKeyframes;
        u8 *v1 = gYetiFrames;
        s32 v2 = 0xf;

        *(u8 **)obj = v0;
        *(u8 **)(obj + 4) = v1;
        *(s32 *)(obj + 0x18) = v2;
    }
    SetActorAnim(obj, 0);
    *bcAddr = obj;

    gYetiX = 0;
    gYetiDistance = 0xa000;
    gYetiPosition = (GetCellAnimDistance() << 8) - gYetiDistance;
    sub_8029E34(gYetiDistance);

    gYetiState = 0;
    LoadYetiGraphics();
}

asm(".align 2, 0");

/* Sits right after actor_part60.c's `CreateYeti` and before
 * actor_part61.c's `YetiStateCaught` - the whole contiguous range that used
 * to be `asm/code_3_2_20_28568_c99c_e058.s`. */

/* A parameterized twin of `LoadYetiGraphics`'s (actor_part75.c) 16x16
 * triangular-fill dot-pattern loop, taking the destination buffer
 * (`dst`) and seed byte (`seed`) as real parameters instead of the
 * fixed stack buffer/`0`-or-`0x80` seed constants `LoadYetiGraphics` uses for
 * its own two inline copies of this same loop. No known caller anywhere
 * in the matched portion of this ROM region (`LoadYetiGraphics` always
 * inlines the loop itself rather than calling this) - kept byte-exact
 * regardless, per this project's standing convention for functions
 * without a confirmed call site. UNUSED.
 *
 * The condition is written as the "fill with 0xff" test so the 0xff
 * store comes first, as in the ROM. */
void sub_802E058(u8 *dst, u8 seed)
{
    s32 y, x;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if ((u32)(x - 3) > 9 || y <= 2 || y > 12)
                dst[y * 16 + x] = 0xff;
            else
                dst[y * 16 + x] = seed++;
        }
    }
}

asm(".align 2, 0");

/* Genuine no-op stub sitting between the still-raw `sub_802E058` (VRAM
 * pattern generator) and `YetiStateStop` - see
 * docs/matching/issue-54-actor-d3a8.md. */
void YetiStateCaught(void)
{
}

asm(".align 2, 0");
