#include "ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "match.h"
#include "actor.h"
#include "globals.h"
}

/* The player object's accessors (`struct player`, player.h), then four
 * of the controller base class's methods (Ctrl, include/ctrl.hpp; the
 * rest are in src/objects/ctrl.cpp): the SetMode/SetAnimSet setters and
 * the Y motion setters, which act on a controller's target, a moving
 * sprite object (the player, a platform or a boss part).
 *
 * The file is C++ for the Ctrl methods (#664, docs/cplusplus.md); the
 * player's accessors are plain functions with C linkage (their
 * prototypes, in player.h, are C declarations). As C, the two motion
 * setters needed 8 register pins each to load the ramp in the ROM's
 * order; as C++, with the values in locals, they need none. */

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

/* `countdown` (+0x91) decrement/clear/increment/get accessors: the
 * crate-break limiter BreakCrateInStack arms (to 2) for the attack kinds
 * gAttackKindBreakLimited flags, and UpdatePlayer counts down.
 *
 * UNUSED - DecrementPlayerCountdown through StorePlayerListEntry below have
 * no caller anywhere in the ROM (no `bl` in src/, no Thumb pointer to them
 * in baserom.gba); the code that needs these fields reads them directly. */
void DecrementPlayerCountdown(struct player *self)
{
    if (self->countdown != 0) {
        self->countdown -= 1;
    }
}

void ClearPlayerCountdown(struct player *self)
{
    self->countdown = 0;
}

void IncrementPlayerCountdown(struct player *self)
{
    self->countdown += 1;
}

u8 GetPlayerCountdown(struct player *self)
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

/* `listCount` (+0x94) clear/increment (only in control mode 0, like the
 * crate list recording itself)/get accessors, then a plain
 * clear/increment/get triple on the same field (the `Alt` copies; the ROM
 * has both - reproduced as-is rather than deduplicated). UNUSED. */
void ClearPlayerListCount(struct player *self)
{
    self->listCount = 0;
}

void IncrementPlayerListCount(struct player *self)
{
    if (self->ctrlMode == 0) {
        self->listCount += 1;
    }
}

u8 GetPlayerListCount(struct player *self)
{
    return self->listCount;
}

void ClearPlayerListCountAlt(struct player *self)
{
    self->listCount = 0;
}

void IncrementPlayerListCountAlt(struct player *self)
{
    self->listCount += 1;
}

u8 GetPlayerListCountAlt(struct player *self)
{
    return self->listCount;
}

/* `bounce` (+0x92) clear/increment/get accessors. UNUSED. */
void ClearPlayerBounce(struct player *self)
{
    self->bounce = 0;
}

void IncrementPlayerBounce(struct player *self)
{
    self->bounce += 1;
}

u8 GetPlayerBounce(struct player *self)
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

/* Indexed getter into `list` (+0x98, the recently touched crates), gated by
 * `ctrlMode` and (for `idx > 4`) `listCount`; the same test
 * QueueCratePlayerCollision makes inline. UNUSED. Still pinned: unpinned
 * (`if (ctrlMode == 0 && (idx <= 4 || idx < listCount))`), agbcp swaps
 * the address computation's r0 and r1. */
s32 GetPlayerListEntry(struct player *self, s32 idx)
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

/* Stores `val` into `list` at index `listCount` (the next free slot,
 * without counting it), gated by `ctrlMode` and the index staying `<= 4`.
 * UNUSED. The C needed 6 register pins; built by agbcp, the plain code
 * matches. */
void StorePlayerListEntry(struct player *self, s32 val)
{
    if (self->ctrlMode == 0) {
        u32 idx = self->listCount;

        if (idx <= 4)
            self->list[idx] = (struct crate *)val;
    }
}

/* `state` setter (slot 4). */
void Ctrl::SetMode(s32 mode)
{
    state = mode;
}

/* `animSet` setter: the motion entry set the ...FromSet methods index
 * (play_room.c sets each room kind's). */
void Ctrl::SetAnimSet(const struct entry_set *set)
{
    animSet = set;
}

/* Copies `ramp` into `part`'s Y speed ramp (`rampY`; the speed is kept),
 * negating the start and the target when `part` is Y-mirrored (`mirror`
 * bit 5). The negated branch loads the target before the step. */
void Ctrl::SetTargetMotionY(SpriteObj *part, const speed_ramp *ramp)
{
    if ((s32)(part->mirror << 26) < 0) {
        s32 x = -ramp->start;
        s32 z = -ramp->target;
        s32 y = ramp->step;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    } else {
        s32 x = ramp->start;
        s32 y = ramp->step;
        s32 z = ramp->target;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
}

/* The same, also starting `speedY` at the (possibly negated) start: the
 * plain-copy counterpart of StartTargetMotionX (ctrl.cpp), which scales
 * the record by the entry set's `scale`. */
void Ctrl::StartTargetMotionY(SpriteObj *part, const speed_ramp *ramp)
{
    if ((s32)(part->mirror << 26) < 0) {
        s32 x = -ramp->start;
        s32 z = -ramp->target;
        s32 y = ramp->step;

        part->speedY = x;
        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    } else {
        s32 x = ramp->start;
        s32 y = ramp->step;
        s32 z = ramp->target;

        part->speedY = x;
        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
}
