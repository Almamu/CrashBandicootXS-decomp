#ifndef GUARD_INPUT_CTRL_HPP
#define GUARD_INPUT_CTRL_HPP

/* The input controller as C++ (#664, docs/cplusplus.md):
 * src/game/crash/input_ctrl.cpp and input_ctrl_queue.cpp.
 *
 * No `#pragma interface`: g++ emits its vtable, gInputCtrlVtable, in
 * input_ctrl.cpp (see ctrl.hpp).
 *
 * In the namespace Game::Crash (#1033, player.hpp):
 * `_vt.Q34Game5Crash9InputCtrl` & co. */

#include "game/entities/ctrl.hpp"
#include "game/crash/player.hpp"
#include "game/ui/level_select.hpp"

/* InputCtrl's `state` (Ctrl::state; #843, docs/flags/input_ctrl.md): the
 * `stateFuncs` index. */
#define INPUT_CTRL_STATE_START 0       // StateStart: the ride's set-up (Restart, from Player::ResetForRoom)
#define INPUT_CTRL_STATE_RIDE 1        // StateRide: the whole ride (StateStart; Reset)
#define INPUT_CTRL_STATE_UNUSED_RIDE 2 // StateUnusedRide: StateRide's at animation 0's end, never reached
#define INPUT_CTRL_STATE_DEAD 3        // StateDead: KillPlayer; Update skips the steering

/* `dirState`: the D-pad's last vertical direction, queued once. */
#define INPUT_CTRL_DIR_STATE_NONE 0 // neither up nor down: Y entry 0
#define INPUT_CTRL_DIR_STATE_UP 1   // up: Y entry 3
#define INPUT_CTRL_DIR_STATE_DOWN 2 // down: Y entry 5

namespace Game
{
namespace Crash
{

/* The input controller (gInputCtrlVtable; 0x28 bytes):
 * the player's controller in room kind 2 (play_room.cpp creates it), where
 * the player uses sprite bank 2 (Crash riding a hover vehicle) and the
 * D-pad alone moves him. Each frame Update reads the D-pad, queues the
 * target's motion entries (up/down pick `motionY`, left/right `motionX`
 * and the camera lead's speed), runs the state method (`stateFuncs`) and
 * applies the queued motion (ApplyMotion). */
class InputCtrl : public Game::Entities::Ctrl
{
public:
    Player *target;      // 0x10
    u8 motionX;          // 0x14 - queued X motion entry (animSet->entries[][0])
    u8 motionY;          // 0x15 - queued Y motion entry (animSet->entries[][1])
    u8 dirState;         // 0x16 - the D-pad's last vertical direction: INPUT_CTRL_DIR_STATE_*
    u8 motionXPending;   // 0x17 - ApplyMotion applies motionX
    u8 motionYPending;   // 0x18 - ApplyMotion applies motionY
    u8 motionXKeepSpeed; // 0x19 - apply with SetTargetMotionX (speed kept), not Start...
    u8 motionYKeepSpeed; // 0x1A - the same for Y
    Game::Objects::CameraLead *cameraLead; // 0x1C - spawned by StateStart (level_select.hpp)
    u8 flag20;                             // 0x20 - left can still slow the ride down
    s32 timer; // 0x24 - how long left has been held, and the cooldown after

    /* The state methods, indexed by `state` (gInputCtrlStateFuncs,
     * src/game/crash/data/swim_input_ctrl_state_tables.cpp): INPUT_CTRL_STATE_*. */
    typedef void (InputCtrl::*StateFunc)();
    static const StateFunc stateFuncs[4];

    InputCtrl();                                              // CreateInputCtrl
    virtual void Update(Game::Entities::MovingSprite *part);  // 1
    virtual void HandleEvent(s32 from, s32 event, s32 arg);   // 2
    virtual void Attach(Game::Entities::MovingSprite *owner); // 3
    virtual ~InputCtrl();                                     // 9 DestroyInputCtrl
    void KillPlayer(s32 anim);
    void StateStart();
    void ApplyMotion();
    void SetModeAnim(s32 mode, s32 anim, s32 unused3, s32 unused4);
    void StateDead();
    void StateUnusedRide();
    void StateRide();
    void Restart();
    void Reset();
    void SetMotionYPending();
    void SetMotionXPending();
    void CancelMotionY();
    void CancelMotionX();
    u8 IsMotionYPending();
    u8 IsMotionXPending();
    void QueueMotionYKeepSpeed(u8 entry);
    void QueueMotionXKeepSpeed(u8 entry);
    void QueueMotionY(u8 entry);
    void QueueMotionX(u8 entry);

    /* QueueMotionX/QueueMotionY and the camera lead's speed as Update and
     * the state methods have them inlined: the value is computed before
     * the stores. */
    void QueueNowX(u8 entry)
    {
        motionXPending = 1;
        motionX = entry;
    }
    void QueueNowY(u8 entry)
    {
        motionYPending = 1;
        motionY = entry;
    }
    void SetLeadSpeed(s32 speed)
    {
        cameraLead->targetOffset = speed;
    }
};

LAYOUT_ASSERT(input_ctrl_hpp, sizeof(InputCtrl) == 0x28);

} // namespace Crash
} // namespace Game

#endif /* !GUARD_INPUT_CTRL_HPP */
