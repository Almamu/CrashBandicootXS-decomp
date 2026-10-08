#ifndef GUARD_PLAYER_HPP
#define GUARD_PLAYER_HPP

/* The player as C++ (#664, docs/cplusplus.md, part 8): the class behind
 * gPlayerVtable, whose code is src/player/player_*.cpp. It has no C view
 * (player.h's `struct player` went with its last C reader, #754).
 * cxx_symbols.txt maps the methods to the C names.
 *
 * No `#pragma interface`: g++ emits the vtable in player_collide.cpp (see
 * ctrl.hpp). */

#include "ctrl.hpp"
#include "sprite_obj.hpp"
#include "part_list.hpp"

extern "C" {
#include "player.h"
}

class Crate;

/* The player (gPlayer): a ground sprite with the player's own fields after
 * it. PlayRoom builds it (`new Player(0xFFFF, 0, 0, 0)`, InitPlayer on a
 * 0x350-byte block in its C). Its controller, `mover`, is the room kind's:
 * the action, swim, input or boss controller (`ctrlMode`). Slots 2, 5-9
 * and 11 are the base classes'. */
class Player : public GroundSprite
{
public:
    // 0x80 - set while a triggered crate animation runs (the crate's state bit 7),
    //        cleared when it ends; enemies skip the player meanwhile
    u8 busy;
    u8 unk_81[7];
    // 0x88 - the control mode 0-3, which picks the controller (ResetForRoom); 1: crates
    //        fall at quarter speed and touched enemies just vanish; nonzero stops `list`
    //        recording
    u8 ctrlMode;
    u32 deadline; // 0x8C - the gRoomFrameCount frame the invulnerability lasts until
    // 0x90 - a crate's side stopped the X motion (ActionCtrl::HandleEvent, event 12);
    //        while set, crate_hit.cpp widens the player's box by 2 px on each side
    u8 bumped;
    // 0x91 - the crate-break limiter: Crate::BreakInStack arms it (2) and skips the
    //        break while it runs; Update counts it down
    u8 countdown;
    u8 bounce; // 0x92 - stepped on a crate bounce (crate_break.cpp), cleared by the controller
    u8 unk_93;
    u8 listCount;             // 0x94 - entries in `list`
    Crate *list[5];           // 0x98 - the recently touched crates
    Sprite *carried;          // 0xAC - the platform or crate the player stands on
    Sprite *child;            // 0xB0 - Aku Aku, drawn with the player (Draw)
    s32 maskTrailIdx;         // 0xB4 - the newest entry of `maskTrail`
    struct vec2 maskTrail[8]; // 0xB8 - the recent positions, which Aku Aku follows
    u8 unk_F8[8];
    // 0x100 - standing on terrain kind 5 (CheckPlayerContact): the player keeps
    //         sliding and skids (ActionCtrl::SetTargetAnim)
    u8 slippery;
    // 0x101 - hanging from hang terrain (code 6): CheckPlayerContact sends
    //         EVENT_HANG_GRAB and EVENT_HANG_RELEASE; ActionCtrl::HandleEvent sets it
    u8 hanging;
    u8 pushLeft;  // 0x102 - nonzero: moves the standing player 1 px left per frame
    u8 pushRight; // 0x103 - nonzero: moves the standing player 1 px right per frame
    u8 dead;      // 0x104 - the player died; blocks pause and further hits
    u8 cleared;   // 0x105 - TouchPlayer's latch
    CollisionQueue collisionQueue; // 0x108 - the crate collisions of the frame

    Player(u16 id, u16 px, u16 py, u16 unused);             // InitPlayer
    virtual s32 CheckPlayerContact();                       // 1 CollidePlayer
    virtual void Update();                                  // 3 UpdatePlayer
    virtual void Draw();                                    // 4 DrawPlayer
    virtual ~Player();                                      // 10 DestroyPlayer
    virtual s32 ApplyVelocity();                            // 12 ApplyPlayerVelocity
    virtual void HandleEvent(s32 from, s32 event, s32 arg); // 13 PlayerHandleEvent
    virtual void TouchPlayer();                             // 14 CollidePlayerWithObjects

    void Reset();                    // ResetPlayer
    void ResetForRoom();             // ResetPlayerForRoom
    u8 HasRoomForAnim(s32 anim);     // PlayerHasRoomForAnim
    u8 HasRampYTarget();             // HasPlayerRampYTarget
    void ClearSpeedY();              // ClearPlayerSpeedY
    void StopFalling();              // StopPlayerFalling (UNUSED)
    u8 TouchesBox(struct aabb *box); // PlayerTouchesBox

    /* The accessors (src/player/player_flags.cpp; their C names are
     * player.h's GetPlayerCollisionQueue, ClearPlayerDead, ...). Most are
     * UNUSED (see there). */
    CollisionQueue *GetCollisionQueue();
    void ClearDead();
    void SetDead();
    u8 IsDead();
    void StartRampX(s32 a, s32 b, s32 c);
    void SetRampX(s32 a, s32 b, s32 c);
    void DecrementCountdown();
    void ClearCountdown();
    void IncrementCountdown();
    u8 GetCountdown();
    u8 IsInvulnerable();
    void ClearInvulnerability();
    void SetInvulnerable(s32 value);
    void SetControlMode(u8 value);
    u8 GetControlMode();
    Sprite *GetStandingOn();
    void SetStandingOn(Sprite *part);
    void SetBusy(u8 value);
    u8 IsBusy();
    void ClearListCount();
    void IncrementListCount();
    u8 GetListCount();
    void ClearListCountAlt();
    void IncrementListCountAlt();
    u8 GetListCountAlt();
    void ClearBounce();
    void IncrementBounce();
    u8 GetBounce();
    void SetBumped(u8 value);
    u8 IsBumped();
    u8 GetPushRight();
    void SetPushRight(u8 value);
    u8 GetPushLeft();
    void SetPushLeft(u8 value);
    u8 IsHanging();
    void SetHanging(u8 value);
    u8 IsSlippery();
    void SetSlippery(u8 value);
    Crate *GetListEntry(s32 idx);
    void StoreListEntry(Crate *crate);

    /* Stores to the player's bytes through inline methods: as an inline
     * parameter, the value is materialized before the field's address,
     * where a plain store computes the address first. The ROM has both. */
    void StoreHitAxes(s32 axes)
    {
        hitAxes = axes;
    }
    void StoreSlippery(s32 value)
    {
        slippery = value;
    }
    void StorePushLeft(s32 value)
    {
        pushLeft = value;
    }
    void StorePushRight(s32 value)
    {
        pushRight = value;
    }

    /* GetSpriteFrameAnchor (Sprite::GetFrameAnchor) inlined, as
     * CheckPlayerContact and ActionCtrl::HandleEvent have it: the current
     * frame's anchor point (the 3-box and 1-box frame layouts have one; the
     * others use gEmptySpritePoint). With a return per case, the result
     * gets the register the ROM has (r3 in CheckPlayerContact, which keeps
     * r3 out of reload's registers there). */
    const struct sprite_point *FrameAnchor()
    {
        const struct sprite_frame *info = GetFrame();

        switch (info->pieces[0] >> 4) {
        case 0:
            return &((const struct sprite_frame_3box_anchor *)info)->anchor;
        case 1:
            return &gEmptySpritePoint;
        case 2:
            return &gEmptySpritePoint;
        case 3:
            return &gEmptySpritePoint;
        case 4:
            return &gEmptySpritePoint;
        case 5:
            return &gEmptySpritePoint;
        case 6:
            return &((const struct sprite_frame_1box_anchor *)info)->anchor;
        default:
            return &gEmptySpritePoint;
        }
    }
};

COMPILE_TIME_ASSERT(player_hpp, sizeof(Player) == 0x350);

#endif /* !GUARD_PLAYER_HPP */
