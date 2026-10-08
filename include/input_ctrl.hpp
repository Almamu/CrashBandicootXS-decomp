#ifndef GUARD_INPUT_CTRL_HPP
#define GUARD_INPUT_CTRL_HPP

/* The input controller as C++ (#664, docs/cplusplus.md):
 * src/player/input_ctrl.cpp and input_ctrl_queue.cpp.
 *
 * No `#pragma interface`: g++ emits its vtable, gInputCtrlVtable, in
 * input_ctrl.cpp (see ctrl.hpp). */

#include "ctrl.hpp"
#include "player.hpp"
#include "level_select.hpp"

/* The input controller (gInputCtrlVtable, struct input_ctrl in player.h):
 * the player's controller in room kind 2 (play_room.cpp creates it), where
 * the player uses sprite bank 2 (Crash riding a hover vehicle) and the
 * D-pad alone moves him. Each frame Update reads the D-pad, queues the
 * target's motion entries (up/down pick `motionY`, left/right `motionX`
 * and the camera lead's speed), runs the state method (`stateFuncs`) and
 * applies the queued motion (ApplyMotion). */
class InputCtrl : public Ctrl
{
public:
    Player *target;      // 0x10
    u8 motionX;          // 0x14 - queued X motion entry (animSet->entries[][0])
    u8 motionY;          // 0x15 - queued Y motion entry (animSet->entries[][1])
    u8 dirState;         // 0x16 - the D-pad's last vertical direction: 0 none, 1 up, 2 down
    u8 motionXPending;   // 0x17 - ApplyMotion applies motionX
    u8 motionYPending;   // 0x18 - ApplyMotion applies motionY
    u8 motionXKeepSpeed; // 0x19 - apply with SetTargetMotionX (speed kept), not Start...
    u8 motionYKeepSpeed; // 0x1A - the same for Y
    u8 unk_1B;
    CameraLead *cameraLead; // 0x1C - spawned by StateStart (level_select.hpp)
    u8 flag20;              // 0x20 - left can still slow the ride down
    u8 unk_21[3];
    s32 timer; // 0x24 - how long left has been held, and the cooldown after

    /* The state methods, indexed by `state` (gInputCtrlStateFuncs,
     * src/data/player_pmf_16c250.c): 0 StateStart, 1 StateRide,
     * 2 StateUnusedRide, 3 StateDead. */
    typedef void (InputCtrl::*StateFunc)();
    static const StateFunc stateFuncs[4];

    InputCtrl();                                                        // CreateInputCtrl
    virtual void Update(MovingSprite *part);                            // 1
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg); // 2
    virtual void Attach(MovingSprite *owner);                           // 3
    virtual ~InputCtrl();                                               // 9 DestroyInputCtrl
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

COMPILE_TIME_ASSERT(input_ctrl_hpp, sizeof(InputCtrl) == sizeof(struct input_ctrl));

#endif /* !GUARD_INPUT_CTRL_HPP */
