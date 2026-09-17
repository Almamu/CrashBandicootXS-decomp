#include "core.h"

extern s8 gStaticData_085A9EAC[];

/* Per-tick update of a voice's `+0x2e` field from a signed lookup table
 * (`gStaticData_085A9EAC`), gated on the currently bound instrument's
 * (`+0x3c`) 10th byte (`+9`) being non-zero - reads like a vibrato/
 * tremolo-style periodic effect: `+0x23` is a delay counter that ticks
 * down once per call, and once it expires a new phase index is derived
 * from the instrument's own `+0xa` step value and wrapped to 0-0x3f
 * before indexing the table again. The result is scaled by the
 * instrument's `+9` byte (>> 8, a standard Q8 multiply-down). Channel/
 * voice object shape not modeled yet - kept as raw offsets, same as
 * gax_channel_note_cut.c right next to this function in ROM. */
void sub_8039FFC(void *self)
{
    register u8 *p asm("r2") = self;
    u8 *inst = *(u8 **)(p + 0x3c);
    u32 result = inst[9];

    if (result != 0) {
        u8 *flagPtr = p + 0x23;

        if (*flagPtr == 0) {
            *(u16 *)(p + 0x3a) = (*(u16 *)(p + 0x3a) + inst[0xa]) & 0x3f;
        } else {
            *flagPtr -= 1;
        }
        {
            u8 *table = (u8 *)gStaticData_085A9EAC;
            u16 phase = *(u16 *)(p + 0x3a);
            u32 zero = 0;
            u8 *addr = table + phase;
            s8 tableVal = *(s8 *)(addr + zero);
            result = (tableVal * (*(u8 **)(p + 0x3c))[9]) >> 8;
        }
    }
    *(u16 *)(p + 0x2e) = result;
}
