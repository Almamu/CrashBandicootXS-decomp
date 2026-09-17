#include "core.h"

extern u8 gStaticData_085A6150[];

/* Looks up the first of 12 (8-byte-stride, first field a u32 threshold)
 * table entries whose threshold is >= `value`, returning its index, or
 * 0xb (the last entry) if none qualify. Table contents/meaning not
 * understood yet - kept as a raw byte pointer with a manual stride
 * rather than a guessed struct. */
s32 sub_8037FA0(u32 value)
{
    u32 i = 0;
    u8 *p = gStaticData_085A6150;

    for (; i <= 0xb; i++) {
        if (*(u32 *)p >= value) {
            return i;
        }
        p += 8;
    }
    return 0xb;
}
