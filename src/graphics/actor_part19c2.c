#include "core.h"
#include "actor_self.h"

/* Thin `InitActorPart`-based constructor (constant last-arg `1`,
 * unlike `sub_802C4A4`'s forwarded one), then computes a velocity
 * vector aiming toward a fixed offset point via the screen-projection
 * helpers `sub_8029E98`/`sub_8029EB4` plus `__divsi3` division -
 * the "homing/seek-toward-point effect" `sub_8032890` byte-for-byte
 * twins, per docs/rom_map.md.
 *
 * The Manhattan-distance/abs-value computation uses the ROM's own
 * branchless abs idiom (`(x ^ (x >> 31)) - (x >> 31)`, compiling to
 * `asr`/`eor`/`sub`) rather than a `(x < 0) ? -x : x` ternary, which
 * this compiler instead turns into a `cmp`/`bge`/`neg` branch. The
 * `x` (+0x1c) reload also needs pinning to `r1` and reading *after*
 * the `sub_8029EB4()` call (not before) - pinning it before the call
 * let this compiler's optimizer silently skip the reload and reuse a
 * stale register value from the unrelated `y` (+0x20) computation two
 * statements earlier, a genuine correctness bug caught by a direct
 * byte compare against the ROM, not just a register-choice cosmetic
 * mismatch. */
extern u8 gStaticData_087E4E74[];
extern s32 __divsi3(s32 arg0, s32 arg1);
extern s32 sub_8029E98(void);
extern s32 sub_8029EB4(void);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);

/* The seek effect (method table gStaticData_087E4E74). */
struct actor_seek {
    struct actor_self base;
    s32 velX;           // 0x54
    s32 velY;           // 0x58
    s32 count;          // 0x5C - the spawn parameter; sub_802C394 (actor_part19c.c)
                        // repeats its teardown drain this many times
};

void *sub_802C3E8(void *selfArg, s32 a, s32 b, s32 c, s32 spawnParam)
{
    struct actor_seek *self = selfArg;
    register s32 dy asm("r3");
    s32 sum;
    s32 q;

    InitActorPart(self, a, b, c, 1);
    self->base.vtable = (struct actor_vtable *)gStaticData_087E4E74;
    self->count = spawnParam;

    self->base.y += sub_8029E98();

    {
        register s32 ebResult asm("r0") = sub_8029EB4();
        register s32 old asm("r1") = self->base.x;
        dy = old + ebResult;
    }
    self->base.x = dy;

    {
        s32 a1 = dy - 0x1000;
        s32 a2 = (a1 ^ (a1 >> 31)) - (a1 >> 31);
        s32 dx = self->base.y;
        s32 b1 = dx - 0x1000;
        s32 b2 = (b1 ^ (b1 >> 31)) - (b1 >> 31);

        sum = a2 + b2;
        if (sum < 0) {
            sum += 0x7ff;
        }
        q = sum >> 0xb;

        self->velX = __divsi3(0x1000 - dy, q);
        self->velY = __divsi3(0x1000 - dx, q);
    }

    return self;
}

asm(".align 2, 0");
