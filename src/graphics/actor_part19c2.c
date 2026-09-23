#include "core.h"

/* Thin `InitActorPart`-based constructor (constant last-arg `1`,
 * unlike `sub_802C4A4`'s forwarded one), then computes a velocity
 * vector aiming toward a fixed offset point via the screen-projection
 * helpers `sub_8029E98`/`sub_8029EB4` plus `sub_803ADB4` division -
 * the "homing/seek-toward-point effect" `sub_8032890` byte-for-byte
 * twins, per docs/rom_map.md.
 *
 * The Manhattan-distance/abs-value computation uses the ROM's own
 * branchless abs idiom (`(x ^ (x >> 31)) - (x >> 31)`, compiling to
 * `asr`/`eor`/`sub`) rather than a `(x < 0) ? -x : x` ternary, which
 * this compiler instead turns into a `cmp`/`bge`/`neg` branch. The
 * `self+0x1c` reload also needs pinning to `r1` and reading *after*
 * the `sub_8029EB4()` call (not before) - pinning it before the call
 * let this compiler's optimizer silently skip the reload and reuse a
 * stale register value from the unrelated `self+0x20` computation two
 * statements earlier, a genuine correctness bug caught by a direct
 * byte compare against the ROM, not just a register-choice cosmetic
 * mismatch. */
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

    {
        register s32 ebResult asm("r0") = sub_8029EB4();
        register s32 old asm("r1") = *(s32 *)(self + 0x1c);
        dy = old + ebResult;
    }
    *(s32 *)(self + 0x1c) = dy;

    {
        s32 a1 = dy - 0x1000;
        s32 a2 = (a1 ^ (a1 >> 31)) - (a1 >> 31);
        s32 dx = *(s32 *)(self + 0x20);
        s32 b1 = dx - 0x1000;
        s32 b2 = (b1 ^ (b1 >> 31)) - (b1 >> 31);

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

asm(".align 2, 0");
