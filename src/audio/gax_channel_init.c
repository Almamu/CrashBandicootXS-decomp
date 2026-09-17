#include "core.h"
#include "audio.h"

extern struct GaxPlayerState *gUnknown_03001630;

/* Resets a per-channel voice object to its default state (clears the
 * accumulator/instrument-pointer fields, arms the `0x8AD0` "no note"
 * sentinel envelope, sets a default "unmuted" priority (`0xff`) and
 * portamento speed (`-1`, i.e. "off")), then picks a starting value for
 * `+0x52` from a single flag byte at `gUnknown_03001630`'s own `+0x42`
 * (two settings, 1 or 2) whose meaning isn't confirmed yet. Voice object
 * shape not modeled - same situation as the neighboring channel
 * functions in this file's ROM region. */
void sub_803A104(void *self)
{
    u8 *p = self;
    u32 zeroA = 0;
    register u32 zeroB asm("r1");
    u16 val;
    u8 b;

    *(u32 *)(p + 0x44) = zeroA;
    *(u8 *)(p + 0x10) = (u8)zeroA;
    *(u32 *)(p + 0x3c) = zeroA;
    zeroB = 0;
    val = 0x8AD0;
    *(u16 *)(p + 0x2a) = val;
    *(u8 *)(p + 0x11) = 1;
    *(u8 *)(p + 0x15) = 0xff;
    {
        s32 negOne = 1;
        negOne = -negOne;
        *(u8 *)(p + 0x18) = negOne;
    }
    *(u8 *)(p + 0xc) = zeroB;
    *(u8 *)(p + 0x12) = zeroB;
    *(u8 *)(p + 0xd) = zeroB;
    *(u8 *)(p + 0x24) = zeroB;
    *(u8 *)(p + 0x25) = zeroB;
    *(u32 *)(p + 0x4c) = 0x80000000;
    b = *((u8 *)gUnknown_03001630 + 0x42);
    {
        u32 v = 1;
        if (b != 0) {
            v = 2;
        }
        *(u8 *)(p + 0x52) = v;
    }
}
