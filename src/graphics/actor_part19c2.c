#include "core.h"

#if NON_MATCHING
/* NOT YET BYTE-MATCHING - see docs/matching.md, "Parked, not matched:
 * sub_802C3E8" for the full account; compiled only under
 * `make NON_MATCHING=1`, the checked-in assembly
 * (asm/code_3_2_20_28568_c3e8.s) is used otherwise. Semantics are
 * fully understood and every load/store, branch and call is confirmed
 * correct; the residual gap is register-allocation choices in the
 * Manhattan-distance/abs-value computation (this compiler picks a
 * different, logically-equivalent register for a couple of
 * intermediate values than the ROM's own choice). Thin
 * `InitActorPart`-based constructor (constant last-arg `1`, unlike
 * `sub_802C4A4`'s forwarded one), then computes a velocity vector
 * aiming toward a fixed offset point via the screen-projection
 * helpers `sub_8029E98`/`sub_8029EB4` plus `sub_803ADB4` division -
 * the "homing/seek-toward-point effect" `sub_8032890` byte-for-byte
 * twins, per docs/rom_map.md. */
extern u8 gStaticData_087E4E74[];
extern s32 sub_803ADB4(s32 arg0, s32 arg1);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);

void *sub_802C3E8(void *selfArg, s32 a, s32 b, s32 c, s32 spawnParam)
{
    u8 *self = selfArg;
    register s32 dy asm("r3");
    s32 sum;
    s32 q;

    InitActorPart(self, a, b, c, 1);
    *(u8 **)(self + 0x50) = gStaticData_087E4E74;
    *(s32 *)(self + 0x5c) = spawnParam;

    *(s32 *)(self + 0x20) += sub_8029E98();

    dy = *(s32 *)(self + 0x1c) + sub_8029EB4();
    *(s32 *)(self + 0x1c) = dy;

    {
        s32 a1 = dy - 0x1000;
        s32 a2 = (a1 < 0) ? -a1 : a1;
        s32 dx = *(s32 *)(self + 0x20);
        s32 b1 = dx - 0x1000;
        s32 b2 = (b1 < 0) ? -b1 : b1;

        sum = a2 + b2;
        if (sum < 0) {
            sum += 0x7ff;
        }
        q = sum >> 0xb;

        *(s32 *)(self + 0x54) = sub_803ADB4(0x1000 - dy, q);
        *(s32 *)(self + 0x58) = sub_803ADB4(0x1000 - dx, q);
    }

    return self;
}
#endif /* NON_MATCHING */

asm(".align 2, 0");
