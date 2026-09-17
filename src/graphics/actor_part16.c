#include "core.h"
#include "actor.h"

/* Continuation of the big unnamed object introduced in
 * actor_part15.c - see that file's header comment. */

extern u32 gUnknown_0300082C;

/* `self+0x108` address getter. */
void *sub_800B4A4(void *selfArg)
{
    return (u8 *)selfArg + 0x108;
}

/* `self+0x104` byte clear/set/get accessors. */
void sub_800B4AC(void *selfArg)
{
    u8 *self = selfArg;
    self[0x104] = 0;
}

void sub_800B4B8(void *selfArg)
{
    u8 *self = selfArg;
    self[0x104] = 1;
}

u8 sub_800B4C4(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x104];
}

/* Bulk-sets `self+0x48`/`self+0x4c`/`self+0x50`; also sets
 * `self+0x60` to the first argument, but only if `self+0x100` is
 * clear. */
void sub_800B4D0(void *selfArg, s32 a, s32 b, s32 c)
{
    u8 *self = selfArg;

    if (self[0x100] == 0) {
        *(s32 *)(self + 0x60) = a;
    }
    *(s32 *)(self + 0x48) = a;
    *(s32 *)(self + 0x4c) = b;
    *(s32 *)(self + 0x50) = c;
}

/* Same bulk setter as `sub_800B4D0`, without the conditional
 * `self+0x60` write. */
void sub_800B4F0(void *selfArg, s32 a, s32 b, s32 c)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x48) = a;
    *(s32 *)(self + 0x4c) = b;
    *(s32 *)(self + 0x50) = c;
}

/* `self+0x91` countdown byte decrement/clear/increment/get
 * accessors. */
void sub_800B4F8(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x91] != 0) {
        self[0x91] -= 1;
    }
}

void sub_800B508(void *selfArg)
{
    u8 *self = selfArg;
    self[0x91] = 0;
}

void sub_800B510(void *selfArg)
{
    u8 *self = selfArg;
    self[0x91] += 1;
}

u8 sub_800B51C(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x91];
}

/* `self+0x8c` is a snapshot of the `gUnknown_0300082C` frame counter
 * (the same counter documented in `docs/rom_map.md`); this tests
 * whether it's still ahead of the counter (unsigned comparison - a
 * signed one here would be a real, previously-caught bug). */
u8 sub_800B524(void *selfArg)
{
    u8 *self = selfArg;
    return *(u32 *)(self + 0x8c) > gUnknown_0300082C;
}

void sub_800B53C(void *selfArg)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x8c) = 0;
}

/* Sets `self+0x8c` to `gUnknown_0300082C + arg1` - arming the
 * "ahead of the counter" check `sub_800B524` performs. */
void sub_800B544(void *selfArg, s32 arg1)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0x8c) = gUnknown_0300082C + arg1;
}

/* `self+0x88` byte set/get accessors. */
void sub_800B554(void *selfArg, u8 arg1)
{
    u8 *self = selfArg;
    self[0x88] = arg1;
}

u8 sub_800B55C(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x88];
}

/* `self+0xac` pointer/word get/set accessors. */
s32 sub_800B564(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 0xac);
}

void sub_800B56C(void *selfArg, s32 arg1)
{
    u8 *self = selfArg;
    *(s32 *)(self + 0xac) = arg1;
}

/* `self+0x80` byte set/get accessors. */
void sub_800B574(void *selfArg, u8 arg1)
{
    u8 *self = selfArg;
    self[0x80] = arg1;
}

u8 sub_800B57C(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x80];
}

/* `self+0x94` byte clear/increment(gated by `self+0x88`)/get
 * accessors, plus a plain clear/increment/get triple reusing the
 * same field (identical code emitted twice by the ROM - reproduced
 * as-is rather than deduplicated). */
void sub_800B584(void *selfArg)
{
    u8 *self = selfArg;
    self[0x94] = 0;
}

void sub_800B58C(void *selfArg)
{
    u8 *self = selfArg;

    if (self[0x88] == 0) {
        self[0x94] += 1;
    }
}

u8 sub_800B5A0(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x94];
}

void sub_800B5A8(void *selfArg)
{
    u8 *self = selfArg;
    self[0x94] = 0;
}

void sub_800B5B0(void *selfArg)
{
    u8 *self = selfArg;
    self[0x94] += 1;
}

u8 sub_800B5BC(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x94];
}

/* `self+0x92` byte clear/increment/get accessors. */
void sub_800B5C4(void *selfArg)
{
    u8 *self = selfArg;
    self[0x92] = 0;
}

void sub_800B5CC(void *selfArg)
{
    u8 *self = selfArg;
    self[0x92] += 1;
}

u8 sub_800B5D8(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x92];
}

/* `self+0x90` byte set/get accessors. */
void sub_800B5E0(void *selfArg, u8 arg1)
{
    u8 *self = selfArg;
    self[0x90] = arg1;
}

u8 sub_800B5E8(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x90];
}

/* `self+0x103`/`self+0x102`/`self+0x101`/`self+0x100` byte get/set
 * accessor pairs - likely a small array of per-difficulty or
 * per-phase flag bytes given the identical shape and adjacency. */
u8 sub_800B5F0(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x103];
}

void sub_800B5FC(void *selfArg, u8 arg1)
{
    u8 *self = selfArg;
    self[0x103] = arg1;
}

u8 sub_800B608(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x102];
}

void sub_800B614(void *selfArg, u8 arg1)
{
    u8 *self = selfArg;
    self[0x102] = arg1;
}

u8 sub_800B620(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x101];
}

void sub_800B62C(void *selfArg, u8 arg1)
{
    u8 *self = selfArg;
    self[0x101] = arg1;
}

u8 sub_800B638(void *selfArg)
{
    u8 *self = selfArg;
    return self[0x100];
}

void sub_800B644(void *selfArg, u8 arg1)
{
    u8 *self = selfArg;
    self[0x100] = arg1;
}

/* Indexed getter into the `self+0x98` 5-entry `s32` array, gated by
 * `self+0x88` and (for `idx > 4`) `self+0x94`'s own count. */
s32 sub_800B650(void *selfArg, s32 idx)
{
    u8 *self = selfArg;
    s32 result;

    if (self[0x88] != 0) {
        goto ret0;
    }
    if (idx > 4) {
        if (idx >= self[0x94]) {
            goto ret0;
        }
    }
    {
        register s32 offset asm("r0") = idx << 2;
        register u8 *base asm("r1") = self + 0x98;
        register u8 *addr asm("r1");

        addr = base + offset;
        result = *(s32 *)addr;
    }
    goto end;
ret0:
    result = 0;
end:
    return result;
}

/* Appends `val` into the same `self+0x98` array at the index held in
 * `self+0x94`, gated by `self+0x88` and the index staying `<= 4`. */
void sub_800B678(void *selfArg, s32 val)
{
    register u8 *self asm("r2") = selfArg;
    register s32 val3 asm("r3") = val;

    if (self[0x88] == 0) {
        register u8 *p94 asm("r0") = self + 0x94;
        register u32 idx asm("r1") = *p94;

        if (idx <= 4) {
            register u8 *arr asm("r0");
            register s32 offset asm("r1");

            offset = idx << 2;
            arr = p94 + 4;
            arr = arr + offset;
            *(s32 *)arr = val3;
        }
    }
}

/* `self+8`/`self+4` word set accessors. */
void sub_800B698(void *selfArg, s32 val)
{
    u8 *self = selfArg;
    *(s32 *)(self + 8) = val;
}

void sub_800B69C(void *selfArg, s32 val)
{
    u8 *self = selfArg;
    *(s32 *)(self + 4) = val;
}

#if NON_MATCHING
/* Copies `vec` into `self+0x54`/`self+0x58`/`self+0x5c`, negating the
 * X and Z components when `self+0x28` bit 5 is set (a mirror-flag
 * bit, matching the same encoding convention used throughout this
 * ROM for X/Z axis flips). Parked: see docs/matching.md and
 * asm/code_3_2_18.s for the "unavoidable callee-saved register spill"
 * gap - three independent register-pin/restructure attempts all
 * produced an identical 8-byte-larger leaf-with-frame version. */
void sub_800B6A0(void *unused, void *selfArg, s32 *vec)
{
    u8 *self = selfArg;

    if ((s8)(self[0x28] << 2) < 0) {
        s32 x = -vec[0];
        s32 y = vec[1];
        s32 z = -vec[2];

        *(s32 *)(self + 0x54) = x;
        *(s32 *)(self + 0x58) = y;
        *(s32 *)(self + 0x5c) = z;
    } else {
        s32 x = vec[0];
        s32 y = vec[1];
        s32 z = vec[2];

        *(s32 *)(self + 0x54) = x;
        *(s32 *)(self + 0x58) = y;
        *(s32 *)(self + 0x5c) = z;
    }
}

/* Same mirror-flag-gated copy as `sub_800B6A0`, also duplicating the
 * (possibly negated) X component into `self+0x64`. Parked for the
 * same reason. */
void sub_800B6D0(void *unused, void *selfArg, s32 *vec)
{
    u8 *self = selfArg;

    if ((s8)(self[0x28] << 2) < 0) {
        s32 x = -vec[0];
        s32 y = vec[1];
        s32 z = -vec[2];

        *(s32 *)(self + 0x64) = x;
        *(s32 *)(self + 0x54) = x;
        *(s32 *)(self + 0x58) = y;
        *(s32 *)(self + 0x5c) = z;
    } else {
        s32 x = vec[0];
        s32 y = vec[1];
        s32 z = vec[2];

        *(s32 *)(self + 0x64) = x;
        *(s32 *)(self + 0x54) = x;
        *(s32 *)(self + 0x58) = y;
        *(s32 *)(self + 0x5c) = z;
    }
}
#endif /* NON_MATCHING */
