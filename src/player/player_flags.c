#include "core.h"
#include "match.h"
#include "actor.h"
#include "gobj_1a794.h"
#include "player.h"
#include "globals.h"

/* The player object's accessors (`struct player`, player.h), then the
 * controllers' setters (SetCtrlMode/SetCtrlAnimSet) and the motion
 * setters that act on a controller's target, a moving sprite (`struct
 * gobj`, gobj_1a794.h: the player, a platform or a boss part). */

/* `collisionQueue` (+0x108) address getter. */
struct collision_queue *GetPlayerCollisionQueue(struct player *self)
{
    return &self->collisionQueue;
}

/* `dead` (+0x104) clear/set/get accessors. */
void ClearPlayerDead(struct player *self)
{
    self->dead = 0;
}

void SetPlayerDead(struct player *self)
{
    self->dead = 1;
}

u8 IsPlayerDead(struct player *self)
{
    return self->dead;
}

/* Sets `rampX` (+0x48) and starts speedX at its `start`, unless the
 * player is on slippery ground (`slippery`), where the speed is kept. */
void StartPlayerRampX(struct player *self, s32 a, s32 b, s32 c)
{
    if (self->slippery == 0) {
        self->speedX = a;
    }
    self->rampX.start = a;
    self->rampX.step = b;
    self->rampX.target = c;
}

/* Sets `rampX` only, keeping the current speedX. */
void SetPlayerRampX(struct player *self, s32 a, s32 b, s32 c)
{
    self->rampX.start = a;
    self->rampX.step = b;
    self->rampX.target = c;
}

/* `self+0x91` countdown byte decrement/clear/increment/get
 * accessors. */
void sub_800B4F8(struct player *self)
{
    if (self->countdown != 0) {
        self->countdown -= 1;
    }
}

void sub_800B508(struct player *self)
{
    self->countdown = 0;
}

void sub_800B510(struct player *self)
{
    self->countdown += 1;
}

u8 sub_800B51C(struct player *self)
{
    return self->countdown;
}

/* `self+0x8c` is a snapshot of the `gRoomFrameCount` frame counter
 * (the same counter documented in `docs/rom_map.md`); this tests
 * whether it's still ahead of the counter (unsigned comparison - a
 * signed one here would be a real, previously-caught bug). */
u8 IsPlayerInvulnerable(struct player *self)
{
    return self->deadline > gRoomFrameCount;
}

void ClearPlayerInvulnerability(struct player *self)
{
    self->deadline = 0;
}

/* Sets `self+0x8c` to `gRoomFrameCount + arg1` - arming the
 * "ahead of the counter" check `IsPlayerInvulnerable` performs. */
void SetPlayerInvulnerable(struct player *self, s32 arg1)
{
    self->deadline = gRoomFrameCount + arg1;
}

/* `self+0x88` byte set/get accessors. */
void SetPlayerControlMode(struct player *self, u8 arg1)
{
    self->ctrlMode = arg1;
}

u8 GetPlayerControlMode(struct player *self)
{
    return self->ctrlMode;
}

/* `self+0xac` pointer/word get/set accessors. */
s32 GetPlayerStandingOn(struct player *self)
{
    return (s32)self->carried;
}

void SetPlayerStandingOn(struct player *self, s32 arg1)
{
    self->carried = (struct gobj *)arg1;
}

/* `self+0x80` byte set/get accessors. */
void SetPlayerBusy(struct player *self, u8 arg1)
{
    self->busy = arg1;
}

u8 IsPlayerBusy(struct player *self)
{
    return self->busy;
}

/* `self+0x94` byte clear/increment(gated by `self+0x88`)/get
 * accessors, plus a plain clear/increment/get triple reusing the
 * same field (identical code emitted twice by the ROM - reproduced
 * as-is rather than deduplicated). */
void sub_800B584(struct player *self)
{
    self->listCount = 0;
}

void sub_800B58C(struct player *self)
{
    if (self->ctrlMode == 0) {
        self->listCount += 1;
    }
}

u8 sub_800B5A0(struct player *self)
{
    return self->listCount;
}

void sub_800B5A8(struct player *self)
{
    self->listCount = 0;
}

void sub_800B5B0(struct player *self)
{
    self->listCount += 1;
}

u8 sub_800B5BC(struct player *self)
{
    return self->listCount;
}

/* `self+0x92` byte clear/increment/get accessors. */
void sub_800B5C4(struct player *self)
{
    self->bounce = 0;
}

void sub_800B5CC(struct player *self)
{
    self->bounce += 1;
}

u8 sub_800B5D8(struct player *self)
{
    return self->bounce;
}

/* `bumped` (+0x90) set/get accessors. */
void SetPlayerBumped(struct player *self, u8 arg1)
{
    self->bumped = arg1;
}

u8 IsPlayerBumped(struct player *self)
{
    return self->bumped;
}

/* `self+0x103`/`self+0x102`/`self+0x101`/`self+0x100` byte get/set
 * accessor pairs (`pushRight`/`pushLeft`: the standing player is moved
 * 1px per frame that way), `hanging` (+0x101) and `slippery` (+0x100). */
u8 GetPlayerPushRight(struct player *self)
{
    return self->pushRight;
}

void SetPlayerPushRight(struct player *self, u8 arg1)
{
    self->pushRight = arg1;
}

u8 GetPlayerPushLeft(struct player *self)
{
    return self->pushLeft;
}

void SetPlayerPushLeft(struct player *self, u8 arg1)
{
    self->pushLeft = arg1;
}

u8 IsPlayerHanging(struct player *self)
{
    return self->hanging;
}

void SetPlayerHanging(struct player *self, u8 arg1)
{
    self->hanging = arg1;
}

u8 IsPlayerSlippery(struct player *self)
{
    return self->slippery;
}

void SetPlayerSlippery(struct player *self, u8 arg1)
{
    self->slippery = arg1;
}

/* Indexed getter into the `self+0x98` 5-entry `s32` array, gated by
 * `self+0x88` and (for `idx > 4`) `self+0x94`'s own count. */
s32 sub_800B650(struct player *self, s32 idx)
{
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
        MATCH_HOLD_REG(s32, offset, r0) = idx << 2;
        MATCH_HOLD_REG(u8 *, base, r1) = (u8 *)self->list;
        MATCH_HOLD_REG(u8 *, addr, r1);

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
void sub_800B678(struct player *selfArg, s32 val)
{
    MATCH_HOLD_REG(struct player *, self, r2) = selfArg;
    MATCH_HOLD_REG(s32, val3, r3) = val;

    if (self->ctrlMode == 0) {
        MATCH_HOLD_REG(u8 *, p94, r0) = &self->listCount;
        MATCH_HOLD_REG(u32, idx, r1) = *p94;

        if (idx <= 4) {
            MATCH_HOLD_REG(u8 *, arr, r0);
            MATCH_HOLD_REG(s32, offset, r1);

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

/* Copies `ramp` into `self+0x54`/`self+0x58`/`self+0x5c`, negating the
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
void SetCtrlTargetMotionY(void *unused, void *selfArg, const struct speed_ramp *ramp)
{
    MATCH_HOLD_REG(struct gobj *, self, r3) = selfArg;
    MATCH_HOLD_REG(const s32 *, v, r2) = (const s32 *)ramp;

    if ((s8)(self->mirror << 2) < 0) {
        MATCH_HOLD_REG(s32, x, r0) = -v[0];
        MATCH_HOLD_REG(s32, z, r1) = -v[2];
        MATCH_HOLD_REG(s32, y, r2) = v[1];

        self->rampY.start = x;
        self->rampY.step = y;
        self->rampY.target = z;
    } else {
        MATCH_HOLD_REG(s32, x, r0) = v[0];
        MATCH_HOLD_REG(s32, y, r1) = v[1];
        MATCH_HOLD_REG(s32, z, r2) = v[2];

        self->rampY.start = x;
        self->rampY.step = y;
        self->rampY.target = z;
    }
}

/* Same mirror-flag-gated copy as `SetCtrlTargetMotionY`, also duplicating the
 * (possibly negated) X component into `self+0x64`. Matched the same
 * way. */
void StartCtrlTargetMotionY(void *unused, void *selfArg, const struct speed_ramp *ramp)
{
    MATCH_HOLD_REG(struct gobj *, self, r3) = selfArg;
    MATCH_HOLD_REG(const s32 *, v, r2) = (const s32 *)ramp;

    if ((s8)(self->mirror << 2) < 0) {
        MATCH_HOLD_REG(s32, x, r0) = -v[0];
        MATCH_HOLD_REG(s32, z, r1) = -v[2];
        MATCH_HOLD_REG(s32, y, r2) = v[1];

        self->speedY = x;
        self->rampY.start = x;
        self->rampY.step = y;
        self->rampY.target = z;
    } else {
        MATCH_HOLD_REG(s32, x, r0) = v[0];
        MATCH_HOLD_REG(s32, y, r1) = v[1];
        MATCH_HOLD_REG(s32, z, r2) = v[2];

        self->speedY = x;
        self->rampY.start = x;
        self->rampY.step = y;
        self->rampY.target = z;
    }
}
asm(".align 2, 0");
