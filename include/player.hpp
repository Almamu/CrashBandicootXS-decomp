#ifndef GUARD_PLAYER_HPP
#define GUARD_PLAYER_HPP

/* The player as C++ (#664, docs/cplusplus.md, part 8): the class behind
 * gPlayerVtable, whose code is src/player/player_*.cpp. It has no C view
 * (the C `struct player` went with its last C reader, #754).
 * cxx_symbols.txt maps the methods to the C names. The controllers' record
 * types and tables (src/data defines them) and the player's IWRAM data are
 * here too.
 *
 * No `#pragma interface`: g++ emits the vtable in player_collide.cpp (see
 * ctrl.hpp). */

#include "ctrl.hpp"
#include "sprite_obj.hpp"
#include "part_list.hpp"

extern "C" {
#include "core.h"
#include "actor.h"
#include "aabb.h"
#include "objects.h"
#include "constants/action_states.h"
#include "constants/attack_kinds.h"
}

/* The swim controller's turn: speedX at each frame of the turn animation
 * (state 4), copied to the stack in one go by StartSwimCtrlStroke
 * (gSwimCtrlTurnSpeeds). */
struct speed_table {
    s32 v[8];
};

/* The animation of a swim controller mode at one tilt level
 * (gSwimCtrlModeLevelAnims), and a second byte (0xFF in some rows). */
struct level_anim {
    u8 anim;
    u8 unk_1;
};

/* The attack kind of each action controller state (QueueCratePlayerCollision,
 * src/data/object_tables_16bb6c.cpp). */
extern const s32 gActionCtrlStateAttackKinds[ACTION_STATE_COUNT];

/* The swim controller's animations: one row of 13 tilt levels per mode
 * (src/data/speed_table_16c090.cpp, action_table_16bf20.cpp), and the
 * stroke speeds. */
extern const struct level_anim gSwimCtrlModeLevelAnims[8][13];
extern const struct level_anim *const gSwimCtrlModeAnimRows[8];
extern const struct speed_table gSwimCtrlTurnSpeeds;

/* The player's speedY after its last ApplyPlayerVelocity (sym_iwram.txt;
 * gLastSpriteVelY is the moving sprites'). Nothing reads it. */
extern s32 gLastPlayerVelY;

/* Aku Aku's orbit frame counters (DrawPlayer, src/iwram/iwram_data.cpp). */
extern s32 gAkuAkuInvincibleFrame;
extern s32 gAkuAkuFollowFrame;

/* The player's vertical motion in the air (gCtrlMotionRecords' jump and
 * fall records, Q8, docs/fixed_point.md): from the jump's (negative,
 * upward) start speed, speedY steps by PLAYER_GRAVITY a frame (51/256 px
 * a frame per frame) until the player falls at PLAYER_MAX_FALL_SPEED
 * (6 px a frame). */
#define PLAYER_GRAVITY        Q8_FRAC(0, 51)
#define PLAYER_MAX_FALL_SPEED INT_TO_Q8(6)

/* The jumps' start speeds (gCtrlMotionRecords' Y records, upward: the
 * table negates them). A jump (Y entry 7, StateIdle, StateRun; also the
 * hop StartMaskHitJump makes) takes off at PLAYER_JUMP_SPEED; once the
 * take-off animation is done, StateJump restarts the rise at
 * PLAYER_JUMP_HELD_SPEED with A held (entry 9, and entry 0xA for the
 * flip jump) or PLAYER_JUMP_RELEASED_SPEED without (entry 8). The double
 * jump (TryDoubleJump, entries 0xC-0xE) starts at
 * PLAYER_DOUBLE_JUMP_SPEED. Bouncing off a crate (HandleEvent's
 * EVENT_BOUNCE) starts at PLAYER_BOUNCE_SPEED, or 5 px a frame with A
 * held; EVENT_BOUNCE_HIGH's is 5 px, or PLAYER_BOUNCE_HIGH_HELD_SPEED
 * with A held; a launch pad (EVENT_LAUNCH_PAD) throws him up at
 * PLAYER_LAUNCH_PAD_SPEED either way. */
#define PLAYER_JUMP_SPEED             Q8_FRAC(4, 98)  // 1122, record 11
#define PLAYER_JUMP_HELD_SPEED        Q8_FRAC(2, 149) // 661, records 13 and 14
#define PLAYER_JUMP_RELEASED_SPEED    Q8_FRAC(1, 154) // 410, record 12
#define PLAYER_DOUBLE_JUMP_SPEED      Q8_FRAC(4, 26)  // 1050, records 16-18
#define PLAYER_BOUNCE_SPEED           Q8_FRAC(4, 85)  // 1109, record 19
#define PLAYER_BOUNCE_HIGH_HELD_SPEED Q8_FRAC(5, 231) // 1511, record 22
#define PLAYER_LAUNCH_PAD_SPEED       Q8_FRAC(7, 208) // 2000, records 23 and 24

/* The other designer-tuned steps of gCtrlMotionRecords (Q8 px a frame
 * per frame, and px a frame):
 * - PLAYER_DOUBLE_JUMP_ACCEL_X: the double jumps' X record (8), from 0
 *   up to 2 px a frame;
 * - PLAYER_SLIDE_DECEL: the slide's X record (10, X entry 0x1E,
 *   StateIdle's R), from 5.5 px a frame down to 0;
 * - PLAYER_FALL_START_SPEED, PLAYER_FALL_ACCEL: walking off the ground
 *   (Y entry 4, CheckLeftGround and others; record 27) falls from
 *   PLAYER_FALL_START_SPEED, speeding up by PLAYER_FALL_ACCEL to 5 px a
 *   frame; the tornado spin's fall (StartTornadoFall, Y entries
 *   0x18-0x1A, records 29-31) starts from the same speed and speeds up
 *   the slower the more turns it made (`tornadoTurn` 0-1, 2, 3-4);
 * - PLAYER_ICE_STOP_DECEL: on ice, with the D-pad released, the slide
 *   to a stop (record 34, X entry 0x1F, StateIdle). */
#define PLAYER_DOUBLE_JUMP_ACCEL_X          Q8_FRAC(0, 24)
#define PLAYER_SLIDE_DECEL                  Q8_FRAC(0, 56)
#define PLAYER_FALL_START_SPEED             Q8_FRAC(0, 8)
#define PLAYER_FALL_ACCEL                   Q8_FRAC(0, 56)
#define PLAYER_TORNADO_FALL_ACCEL_TURNS_0_1 Q8_FRAC(0, 18)
#define PLAYER_TORNADO_FALL_ACCEL_TURNS_2   Q8_FRAC(0, 14)
#define PLAYER_TORNADO_FALL_ACCEL_TURNS_3_4 Q8_FRAC(0, 6)
#define PLAYER_ICE_STOP_DECEL               Q8_FRAC(0, 8)

/* The player controller's and the input controller's motion records
 * (src/data/motion_records_16b304.cpp; `struct speed_ramp`, objects.h). */
extern const struct speed_ramp gSwimCtrlMotionRecords[31];
extern const struct speed_ramp gInputCtrlMotionRecords[9];

/* The entry sets PlayRoom gives the action, player and input controllers
 * through SetCtrlAnimSet (src/data/entry_set_16b92c.cpp,
 * src/data/entry_set_16b93c.cpp). */
extern const struct entry_set gActionCtrlMotionSet;
extern const struct entry_set gSwimCtrlMotionSet;
extern const struct entry_set gInputCtrlMotionSet;

/* The player's controller (sym_iwram.txt), built by PlayRoom
 * (swim_ctrl.hpp's class). */
extern class SwimCtrl *gSwimCtrl;

/* Player::dir, hitAxes and hitMask hold box_part.h's direction bits
 * (PART_DIR_*): the motion direction, ApplyPlayerVelocity; the camera
 * follows them too (PlayRoom points gCamera->target at gPlayer). */

/* The controllers share a base, ctrl.hpp's class Ctrl (InitCtrl/
 * DestroyCtrl, ctrl.cpp): +0x04 the motion entry set (SetCtrlAnimSet),
 * +0x08 the state, +0x0C the method table. Most subclasses keep their
 * controlled part at +0x10. They are all C++ classes with no C view: the
 * action controller ActionCtrl (action_ctrl.hpp), the swim controller
 * SwimCtrl (swim_ctrl.hpp), the input controller InputCtrl
 * (input_ctrl.hpp) and the boss controllers (boss_ctrl.hpp). */

class Crate;

/* `ctrlMode` (#843, docs/flags/player_ctrl_mode.md): which controller
 * the player has, from the room kind. PlayRoom builds the controller and
 * sets the mode, and ResetForRoom restarts the controller it names. */
#define PLAYER_CTRL_MODE_ACTION 0 // ActionCtrl, on foot (ROOM_KIND_ON_FOOT)
#define PLAYER_CTRL_MODE_SWIM 1   // SwimCtrl, underwater (ROOM_KIND_UNDERWATER)
// An empty case in ResetForRoom; nothing sets it.
#define PLAYER_CTRL_MODE_UNK2 2
#define PLAYER_CTRL_MODE_INPUT 3 // InputCtrl, the hover levels (ROOM_KIND_HOVER)

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
    // 0x88 - the controller, PLAYER_CTRL_MODE_* (ResetForRoom); underwater, crates
    //        fall at quarter speed and touched enemies just vanish; only on foot is
    //        `list` recorded
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

    /* The accessors (src/player/player.cpp; their C names, cxx_symbols.txt,
     * are GetPlayerCollisionQueue, ClearPlayerDead, ...). Most are
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

inline Player *MovingSprite::AsPlayer()
{
    return (Player *)this;
}

#endif /* !GUARD_PLAYER_HPP */
