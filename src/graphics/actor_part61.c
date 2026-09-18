#include "core.h"

/* Same `InitActorPart`-rooted per-instance "self" object family
 * documented in actor_part57.c/actor_part28.c/actor_part32.c: a "part
 * table" pointer at `self+0`, a table-index/"kind" field at `self+0xc`,
 * an anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, state at `self+0x28`, a frame counter at
 * `self+0x44`, and a `+0x50`-rooted event/trampoline table fed through
 * `sub_803AD80`. This is the second object kind (constructed by the
 * parked `sub_8034058`, vtable `gStaticData_087E5554`), with a health-
 * like countdown at `self+0x54`, a "dead" byte flag at `self+0x58`, a
 * second one-shot byte flag at `self+0x2c`, the constructor's cached
 * gate byte at `self+0x59`, and a little "spawn/orbit" record at
 * `self+0x5c`/`self+0x60`/`self+0x64`/`self+0x68`/`self+0x6c` driving
 * `sub_8034188`'s position-plus-effect-spawn step. See
 * docs/matching/issue-63-0x08033ef4-actor.md. */

extern void sub_8033804(void);
extern void sub_803388C(void);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *gUnknown_030012BC;

/* Applies `dmg` damage to `self+0x54` and once it drops to zero (or
 * below): marks `self` dead (`+0x58=1`), sets the one-shot flag
 * (`+0x2c=1`), fires the singleton's own death transition
 * (`sub_803388C`), and switches `self` to state 1, table-index 0 or 1
 * depending on the constructor's cached gate byte (`+0x59`), resetting
 * the anim-frame pair and playing the death sound; otherwise just plays
 * a hit sound. Same shape as `sub_8033AE0` (actor_part30.c). */
void sub_8034110(void *selfArg, s32 dmg)
{
    u8 *self = selfArg;

    sub_8033804();
    *(s32 *)(self + 0x54) -= dmg;

    if (*(s32 *)(self + 0x54) <= 0) {
        u8 *flag;
        register s32 zero asm("r3");
        register s32 one asm("r1");
        s32 idx;

        sub_803388C();
        flag = self + 0x58;
        zero = 0;
        one = 1;
        *flag = one;
        flag -= 0x2c;
        *flag = one;
        {
            u8 gate = flag[0x2d];

            idx = 1;
            if (gate != 0) {
                idx = 0;
            }
        }
        *(s32 *)(self + 0x28) = one;
        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0xc) = idx;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + idx * 12);
            register u8 zero2 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero2;
        }
        *(s32 *)(self + 8) = zero;
        PlaySfx(gUnknown_030012BC, 4, 0x100);
    } else {
        PlaySfx(gUnknown_030012BC, 0x45, 0x100);
    }
}

extern void sub_802A7B8(void *self);
extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern void sub_802E5E4(s32 x, s32 y);
extern void *sub_80338C4(void);

/* Per-frame position sync (`+0x1c`/`+0x20`/`+0x24` from the singleton's
 * position plus `self`'s own `+0x5c`/`+0x60`/`+0x64` offsets), calling
 * `sub_802A7B8(self)` first for the frame's regular update. While `self`
 * is still in state 0 and `+0x34` is over its `0x2800` threshold, drives
 * an "orbit" counter at `+0x68`: at zero, spawns an effect at the synced
 * position (`sub_802E5E4`) and advances a lap counter (`+0x6c`),
 * reseeding `+0x68` from the singleton table's `+4`/`+8`/`+0xc` fields
 * depending on whether the lap counter just reached the table's `+8`
 * entry; otherwise just decrements the orbit counter. */
void sub_8034188(void *selfArg)
{
    register u8 *self asm("r5") = selfArg;

    sub_802A7B8(self);
    *(s32 *)(self + 0x1c) = sub_8033900() + *(s32 *)(self + 0x5c);
    *(s32 *)(self + 0x20) = sub_80338F4() + *(s32 *)(self + 0x60);
    {
        s32 base = sub_80338E8();
        register s32 field asm("r1") = *(s32 *)(self + 0x64);
        register s32 z asm("r2") = base + field;
        *(s32 *)(self + 0x24) = z;
    }

    if (*(s32 *)(self + 0x28) == 0 && *(s32 *)(self + 0x34) > 0x2800) {
        register s32 origCounter asm("r6") = *(s32 *)(self + 0x68);
        register s32 result asm("r0");

        if (origCounter == 0) {
            register s32 lap asm("r4");

            sub_802E5E4(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20));
            lap = *(s32 *)(self + 0x6c) + 1;
            *(s32 *)(self + 0x6c) = lap;

            if (lap == *(s32 *)((u8 *)sub_80338C4() + 8)) {
                *(s32 *)(self + 0x6c) = origCounter;
                result = *(s32 *)((u8 *)sub_80338C4() + 0xc);
            } else {
                result = *(s32 *)((u8 *)sub_80338C4() + 4);
            }
        } else {
            result = origCounter - 1;
        }

        *(s32 *)(self + 0x68) = result;
    }
}

/* Near-twin of `sub_8034188` (same position-sync/orbit-effect shape),
 * but does not call `sub_802A7B8(self)` first - this object's regular
 * per-frame update is driven elsewhere. */
void sub_80341F8(void *selfArg)
{
    register u8 *self asm("r5") = selfArg;

    *(s32 *)(self + 0x1c) = sub_8033900() + *(s32 *)(self + 0x5c);
    *(s32 *)(self + 0x20) = sub_80338F4() + *(s32 *)(self + 0x60);
    {
        s32 base = sub_80338E8();
        register s32 field asm("r1") = *(s32 *)(self + 0x64);
        register s32 z asm("r2") = base + field;
        *(s32 *)(self + 0x24) = z;
    }

    if (*(s32 *)(self + 0x28) == 0 && *(s32 *)(self + 0x34) > 0x2800) {
        register s32 origCounter asm("r6") = *(s32 *)(self + 0x68);
        register s32 result asm("r0");

        if (origCounter == 0) {
            register s32 lap asm("r4");

            sub_802E5E4(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20));
            lap = *(s32 *)(self + 0x6c) + 1;
            *(s32 *)(self + 0x6c) = lap;

            if (lap == *(s32 *)((u8 *)sub_80338C4() + 8)) {
                *(s32 *)(self + 0x6c) = origCounter;
                result = *(s32 *)((u8 *)sub_80338C4() + 0xc);
            } else {
                result = *(s32 *)((u8 *)sub_80338C4() + 4);
            }
        } else {
            result = origCounter - 1;
        }

        *(s32 *)(self + 0x68) = result;
    }
}

/* Constant getter - returns `self`'s death flag (`self+0x58`) for this
 * object kind. */
u8 sub_8034264(void *selfArg)
{
    u8 *self = selfArg;

    return self[0x58];
}

/* No-op stub. */
void nullsub_38(void)
{
}

asm(".align 2, 0");
