#include "core.h"
#include "actor.h"
#include "gobj_1a794.h"

/* Continuation of the big unnamed object introduced in
 * actor_part15.c - see that file's header comment. */

/* Base-class accessors of the level object/player (`struct gobj`,
 * gobj_1a794.h). */

/* `unk_108` address getter. */
void *sub_800B4A4(void *selfArg)
{
    return ((struct gobj *)selfArg)->unk_108;
}

/* `dead` (+0x104) clear/set/get accessors. */
void ClearPlayerDead(void *selfArg)
{
    struct gobj *self = selfArg;
    self->dead = 0;
}

void SetPlayerDead(void *selfArg)
{
    struct gobj *self = selfArg;
    self->dead = 1;
}

u8 IsPlayerDead(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->dead;
}

/* Bulk-sets `self+0x48`/`self+0x4c`/`self+0x50`; also sets
 * `self+0x60` to the first argument, but only if `self+0x100` is
 * clear. */
void sub_800B4D0(void *selfArg, s32 a, s32 b, s32 c)
{
    struct gobj *self = selfArg;

    if (self->unk_100 == 0) {
        self->speedX = a;
    }
    self->velA.x = a;
    self->velA.y = b;
    self->velA.z = c;
}

/* Same bulk setter as `sub_800B4D0`, without the conditional
 * `self+0x60` write. */
void sub_800B4F0(void *selfArg, s32 a, s32 b, s32 c)
{
    struct gobj *self = selfArg;

    self->velA.x = a;
    self->velA.y = b;
    self->velA.z = c;
}

/* `self+0x91` countdown byte decrement/clear/increment/get
 * accessors. */
void sub_800B4F8(void *selfArg)
{
    struct gobj *self = selfArg;

    if (self->countdown != 0) {
        self->countdown -= 1;
    }
}

void sub_800B508(void *selfArg)
{
    struct gobj *self = selfArg;
    self->countdown = 0;
}

void sub_800B510(void *selfArg)
{
    struct gobj *self = selfArg;
    self->countdown += 1;
}

u8 sub_800B51C(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->countdown;
}

/* `self+0x8c` is a snapshot of the `gRoomFrameCount` frame counter
 * (the same counter documented in `docs/rom_map.md`); this tests
 * whether it's still ahead of the counter (unsigned comparison - a
 * signed one here would be a real, previously-caught bug). */
u8 IsPlayerInvulnerable(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->deadline > gRoomFrameCount;
}

void ClearPlayerInvulnerability(void *selfArg)
{
    struct gobj *self = selfArg;
    self->deadline = 0;
}

/* Sets `self+0x8c` to `gRoomFrameCount + arg1` - arming the
 * "ahead of the counter" check `IsPlayerInvulnerable` performs. */
void SetPlayerInvulnerable(void *selfArg, s32 arg1)
{
    struct gobj *self = selfArg;
    self->deadline = gRoomFrameCount + arg1;
}

/* `self+0x88` byte set/get accessors. */
void SetPlayerControlMode(void *selfArg, u8 arg1)
{
    struct gobj *self = selfArg;
    self->ctrlMode = arg1;
}

u8 GetPlayerControlMode(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->ctrlMode;
}

/* `self+0xac` pointer/word get/set accessors. */
s32 GetPlayerStandingOn(void *selfArg)
{
    return (s32)((struct gobj *)selfArg)->carried;
}

void SetPlayerStandingOn(void *selfArg, s32 arg1)
{
    struct gobj *self = selfArg;
    self->carried = (struct gobj *)arg1;
}

/* `self+0x80` byte set/get accessors. */
void SetPlayerBusy(void *selfArg, u8 arg1)
{
    struct gobj *self = selfArg;
    self->busy = arg1;
}

u8 IsPlayerBusy(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->busy;
}

/* `self+0x94` byte clear/increment(gated by `self+0x88`)/get
 * accessors, plus a plain clear/increment/get triple reusing the
 * same field (identical code emitted twice by the ROM - reproduced
 * as-is rather than deduplicated). */
void sub_800B584(void *selfArg)
{
    struct gobj *self = selfArg;
    self->listCount = 0;
}

void sub_800B58C(void *selfArg)
{
    struct gobj *self = selfArg;

    if (self->ctrlMode == 0) {
        self->listCount += 1;
    }
}

u8 sub_800B5A0(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->listCount;
}

void sub_800B5A8(void *selfArg)
{
    struct gobj *self = selfArg;
    self->listCount = 0;
}

void sub_800B5B0(void *selfArg)
{
    struct gobj *self = selfArg;
    self->listCount += 1;
}

u8 sub_800B5BC(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->listCount;
}

/* `self+0x92` byte clear/increment/get accessors. */
void sub_800B5C4(void *selfArg)
{
    struct gobj *self = selfArg;
    self->unk_92 = 0;
}

void sub_800B5CC(void *selfArg)
{
    struct gobj *self = selfArg;
    self->unk_92 += 1;
}

u8 sub_800B5D8(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->unk_92;
}

/* `self+0x90` byte set/get accessors. */
void sub_800B5E0(void *selfArg, u8 arg1)
{
    struct gobj *self = selfArg;
    self->unk_90 = arg1;
}

u8 sub_800B5E8(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->unk_90;
}

/* `self+0x103`/`self+0x102`/`self+0x101`/`self+0x100` byte get/set
 * accessor pairs (`pushRight`/`pushLeft`: the standing player is moved
 * 1px per frame that way; +0x101/+0x100 not understood yet). */
u8 GetPlayerPushRight(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->pushRight;
}

void SetPlayerPushRight(void *selfArg, u8 arg1)
{
    struct gobj *self = selfArg;
    self->pushRight = arg1;
}

u8 GetPlayerPushLeft(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->pushLeft;
}

void SetPlayerPushLeft(void *selfArg, u8 arg1)
{
    struct gobj *self = selfArg;
    self->pushLeft = arg1;
}

u8 sub_800B620(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->unk_101;
}

void sub_800B62C(void *selfArg, u8 arg1)
{
    struct gobj *self = selfArg;
    self->unk_101 = arg1;
}

u8 sub_800B638(void *selfArg)
{
    struct gobj *self = selfArg;
    return self->unk_100;
}

void sub_800B644(void *selfArg, u8 arg1)
{
    struct gobj *self = selfArg;
    self->unk_100 = arg1;
}

/* Indexed getter into the `self+0x98` 5-entry `s32` array, gated by
 * `self+0x88` and (for `idx > 4`) `self+0x94`'s own count. */
s32 sub_800B650(void *selfArg, s32 idx)
{
    struct gobj *self = selfArg;
    s32 result;

    if (self->ctrlMode != 0) {
        goto ret0;
    }
    if (idx > 4) {
        if (idx >= self->listCount) {
            goto ret0;
        }
    }
    {
        register s32 offset asm("r0") = idx << 2;
        register u8 *base asm("r1") = (u8 *)self->list;
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
    register struct gobj *self asm("r2") = selfArg;
    register s32 val3 asm("r3") = val;

    if (self->ctrlMode == 0) {
        register u8 *p94 asm("r0") = &self->listCount;
        register u32 idx asm("r1") = *p94;

        if (idx <= 4) {
            register u8 *arr asm("r0");
            register s32 offset asm("r1");

            offset = idx << 2;
            arr = p94 + 4; /* &self->list[0] */
            arr = arr + offset;
            *(s32 *)arr = val3;
        }
    }
}

/* `self+8`/`self+4` word set accessors. */
void SetCtrlMode(void *selfArg, s32 val)
{
    u8 *self = selfArg;
    *(s32 *)(self + 8) = val;
}

void SetCtrlAnimSet(void *selfArg, s32 val)
{
    u8 *self = selfArg;
    *(s32 *)(self + 4) = val;
}

/* Copies `vec` into `self+0x54`/`self+0x58`/`self+0x5c`, negating the
 * X and Z components when `self+0x28` bit 5 is set (a mirror-flag
 * bit, matching the same encoding convention used throughout this
 * ROM for X/Z axis flips). Matched with `self`/`vec` pinned to
 * `r3`/`r2` (avoiding the callee-saved spill three earlier attempts
 * hit - see docs/matching.md's now-stale note and
 * asm/code_3_2_18.s's former guard) plus each branch's X/Y/Z locals
 * pinned to their own ABI registers in the ROM's actual load order:
 * X, then Z, then Y last in the negated branch (`v[1]`'s load is what
 * finally overwrites `v`'s own register, so it has to come after `Z`'s
 * load, not before it, even though the source lists them X/Y/Z). */
void SetCtrlTargetMotionY(void *unused, void *selfArg, struct vec3 *vec)
{
    register struct gobj *self asm("r3") = selfArg;
    register s32 *v asm("r2") = (s32 *)vec;

    if ((s8)(self->mirror << 2) < 0) {
        register s32 x asm("r0") = -v[0];
        register s32 z asm("r1") = -v[2];
        register s32 y asm("r2") = v[1];

        self->velB.x = x;
        self->velB.y = y;
        self->velB.z = z;
    } else {
        register s32 x asm("r0") = v[0];
        register s32 y asm("r1") = v[1];
        register s32 z asm("r2") = v[2];

        self->velB.x = x;
        self->velB.y = y;
        self->velB.z = z;
    }
}

/* Same mirror-flag-gated copy as `SetCtrlTargetMotionY`, also duplicating the
 * (possibly negated) X component into `self+0x64`. Matched the same
 * way. */
void StartCtrlTargetMotionY(void *unused, void *selfArg, struct vec3 *vec)
{
    register struct gobj *self asm("r3") = selfArg;
    register s32 *v asm("r2") = (s32 *)vec;

    if ((s8)(self->mirror << 2) < 0) {
        register s32 x asm("r0") = -v[0];
        register s32 z asm("r1") = -v[2];
        register s32 y asm("r2") = v[1];

        self->speedY = x;
        self->velB.x = x;
        self->velB.y = y;
        self->velB.z = z;
    } else {
        register s32 x asm("r0") = v[0];
        register s32 y asm("r1") = v[1];
        register s32 z asm("r2") = v[2];

        self->speedY = x;
        self->velB.x = x;
        self->velB.y = y;
        self->velB.z = z;
    }
}
asm(".align 2, 0");
