#include "gax_internal.h"

/* Looks up the first of 12 (8-byte-stride, first field a u32 threshold)
 * table entries whose threshold is >= `value`, returning its index, or
 * 0xb (the last entry) if none qualify. Table contents/meaning not
 * understood yet - kept as a raw byte pointer with a manual stride
 * rather than a guessed struct. */
s32 GaxFindMixRate(u32 value)
{
    u32 i = 0;
    const u8 *p = (const u8 *)gGaxMixRates;

    for (; i <= 0xb; i++) {
        if (*(const u32 *)p >= value) {
            return i;
        }
        p += 8;
    }
    return 0xb;
}
