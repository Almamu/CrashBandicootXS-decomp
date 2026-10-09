#include "player.hpp"

extern "C" {
#include "globals.h"
#include "math_util.h"
#include "actor.h"
}

/* The player's constructor (Player, include/player.hpp; InitPlayer).
 * g++ constructs the ground sprite (InitGroundSprite), stores the vtable
 * pointer and constructs the collision queue (ResetCollisionQueue, which
 * empties it); the body creates Aku Aku (`child`, sprite bank 0xCC), resets
 * the player and sets the spawn's id and position. */
Player::Player(u16 id, u16 px, u16 py, u16 unused)
{
    Sprite *c = Sprite::Create(0, 0, 0, 0);

    child = c;
    c->bank = (const struct sprite_bank *)(SPRITE_BANK_BASE + (0xcc << 1));
    {
        /* One 0 for both stores, kept in a register across the calls, as
         * in the ROM (two literals give `id` and the 0 each other's
         * registers). */
        s32 zero = 0;

        c->tag = zero;
        c->ResetFrameTimer();
        c->ResetFrameIndex();
        c->SetAnimDone(0);
        maskTrailIdx = zero;
    }
    Reset();

    this->id = id;
    x = INT_TO_Q8((s32)px);
    y = INT_TO_Q8((s32)py);
}

/* The player's accessors (Player, include/player.hpp; part 8). They were
 * plain functions with C linkage on `struct player` (part 3) until the
 * player became a class; cxx_symbols.txt maps them to their C names. The
 * four Ctrl methods the ROM puts after them start src/objects/ctrl.cpp. */

/* `collisionQueue` (+0x108) address getter. */
CollisionQueue *Player::GetCollisionQueue()
{
    return &collisionQueue;
}

/* `dead` (+0x104) clear/set/get accessors. */
void Player::ClearDead()
{
    dead = 0;
}

void Player::SetDead()
{
    dead = 1;
}

u8 Player::IsDead()
{
    return dead;
}

/* Sets `rampX` (+0x48) and starts speedX at its `start`, unless the
 * player is on slippery ground (`slippery`), where the speed is kept. */
void Player::StartRampX(s32 a, s32 b, s32 c)
{
    if (slippery == 0) {
        speedX = a;
    }
    rampX.start = a;
    rampX.step = b;
    rampX.target = c;
}

/* Sets `rampX` only, keeping the current speedX. */
void Player::SetRampX(s32 a, s32 b, s32 c)
{
    rampX.start = a;
    rampX.step = b;
    rampX.target = c;
}

/* `countdown` (+0x91) decrement/clear/increment/get accessors: the
 * crate-break limiter BreakCrateInStack arms (to 2) for the attack kinds
 * gAttackKindBreakLimited flags, and UpdatePlayer counts down.
 *
 * UNUSED - DecrementPlayerCountdown through StorePlayerListEntry below have
 * no caller anywhere in the ROM (no `bl` in src/, no Thumb pointer to them
 * in baserom.gba); the code that needs these fields reads them directly. */
void Player::DecrementCountdown()
{
    if (countdown != 0) {
        countdown -= 1;
    }
}

void Player::ClearCountdown()
{
    countdown = 0;
}

void Player::IncrementCountdown()
{
    countdown += 1;
}

u8 Player::GetCountdown()
{
    return countdown;
}

/* `deadline` (+0x8C) is a snapshot of the `gRoomFrameCount` frame counter
 * (the same counter documented in `docs/rom_map.md`); this tests
 * whether it's still ahead of the counter (unsigned comparison - a
 * signed one here would be a real, previously-caught bug). */
u8 Player::IsInvulnerable()
{
    return deadline > gRoomFrameCount;
}

void Player::ClearInvulnerability()
{
    deadline = 0;
}

/* Sets `deadline` to `gRoomFrameCount + value` - arming the
 * "ahead of the counter" check `IsPlayerInvulnerable` performs. */
void Player::SetInvulnerable(s32 value)
{
    deadline = gRoomFrameCount + value;
}

/* `ctrlMode` (+0x88) set/get accessors. */
void Player::SetControlMode(u8 value)
{
    ctrlMode = value;
}

u8 Player::GetControlMode()
{
    return ctrlMode;
}

/* `carried` (+0xAC) get/set accessors. */
Sprite *Player::GetStandingOn()
{
    return carried;
}

void Player::SetStandingOn(Sprite *part)
{
    carried = part;
}

/* `busy` (+0x80) set/get accessors. */
void Player::SetBusy(u8 value)
{
    busy = value;
}

u8 Player::IsBusy()
{
    return busy;
}

/* `listCount` (+0x94) clear/increment (only in control mode 0, like the
 * crate list recording itself)/get accessors, then a plain
 * clear/increment/get triple on the same field (the `Alt` copies; the ROM
 * has both - reproduced as-is rather than deduplicated). UNUSED. */
void Player::ClearListCount()
{
    listCount = 0;
}

void Player::IncrementListCount()
{
    if (ctrlMode == 0) {
        listCount += 1;
    }
}

u8 Player::GetListCount()
{
    return listCount;
}

void Player::ClearListCountAlt()
{
    listCount = 0;
}

void Player::IncrementListCountAlt()
{
    listCount += 1;
}

u8 Player::GetListCountAlt()
{
    return listCount;
}

/* `bounce` (+0x92) clear/increment/get accessors. UNUSED. */
void Player::ClearBounce()
{
    bounce = 0;
}

void Player::IncrementBounce()
{
    bounce += 1;
}

u8 Player::GetBounce()
{
    return bounce;
}

/* `bumped` (+0x90) set/get accessors. */
void Player::SetBumped(u8 value)
{
    bumped = value;
}

u8 Player::IsBumped()
{
    return bumped;
}

/* `pushRight` (+0x103), `pushLeft` (+0x102), `hanging` (+0x101) and
 * `slippery` (+0x100) get/set pairs. */
u8 Player::GetPushRight()
{
    return pushRight;
}

void Player::SetPushRight(u8 value)
{
    pushRight = value;
}

u8 Player::GetPushLeft()
{
    return pushLeft;
}

void Player::SetPushLeft(u8 value)
{
    pushLeft = value;
}

u8 Player::IsHanging()
{
    return hanging;
}

void Player::SetHanging(u8 value)
{
    hanging = value;
}

u8 Player::IsSlippery()
{
    return slippery;
}

void Player::SetSlippery(u8 value)
{
    slippery = value;
}

/* Indexed getter into `list` (+0x98, the recently touched crates), gated by
 * `ctrlMode` and (for `idx > 4`) `listCount`; the same test
 * QueueCratePlayerCollision makes inline. UNUSED. The C needed 3 pins
 * and gotos (agbcp swapped the address computation's r0 and r1); as a
 * method, the plain test matches. */
Crate *Player::GetListEntry(s32 idx)
{
    if (ctrlMode == 0 && (idx <= 4 || idx < listCount))
        return list[idx];
    return 0;
}

/* Stores `val` into `list` at index `listCount` (the next free slot,
 * without counting it), gated by `ctrlMode` and the index staying `<= 4`.
 * UNUSED. The C needed 6 register pins; built by agbcp, the plain code
 * matches. */
void Player::StoreListEntry(Crate *crate)
{
    if (ctrlMode == 0) {
        u32 idx = listCount;

        if (idx <= 4)
            list[idx] = crate;
    }
}
