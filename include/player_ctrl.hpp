#ifndef GUARD_PLAYER_CTRL_HPP
#define GUARD_PLAYER_CTRL_HPP

/* The swim controller as C++ (#664, docs/cplusplus.md):
 * src/player/swim_ctrl.cpp, swim_ctrl_drift.cpp and swim_ctrl_stroke.cpp,
 * and the six motion-queue accessors at the top of input_ctrl.cpp.
 *
 * No `#pragma interface`: g++ emits its vtable, gPlayerCtrlVtable, in
 * swim_ctrl.cpp (see ctrl.hpp). */

#include "ctrl.hpp"
#include "player.hpp"

extern "C" {
#include "player_ctrl.h"
}

/* The swim controller (gPlayerCtrlVtable, struct player_ctrl in
 * player_ctrl.h): the diving Crash's controller in the room-kind-1
 * (underwater) rooms, where play_room.c creates it (`new PlayerCtrl`,
 * InitPlayerCtrl(OperatorNew(0x30)) in its C). See swim_ctrl.cpp for the
 * states. Reset and Restart are in src/player/action_ctrl.cpp
 * (ResetPlayerCtrl, RestartPlayerCtrl), which the ROM puts with the action
 * controller's code. */
class PlayerCtrl : public Ctrl
{
public:
    Player *target; // 0x10 - the player (gPlayer)
    s32 unk_14;     // 0x14 - only ever cleared (Reset, ClearUnk14)
    s32 timer;      // 0x18
    s32 timerMax;   // 0x1C
    u8 repeat;      // 0x20 - D-pad auto-repeat countdown
    // 0x21 - swim direction, 0 (up) .. 6 (level) .. 12 (down); column of gPlayerCtrlModeAnimRows
    u8 tilt;
    // 0x22 - row of gPlayerCtrlModeAnimRows (0 idle, 1 swim, 2 stroke, 3 spin, 4-7 turn)
    u8 mode;
    u8 spinCooldown; // 0x23 - frames until StartSpin is allowed again (set to 12)
    u8 motionX;      // 0x24 - queued X motion entry (animSet->entries[][0])
    u8 motionY;      // 0x25 - queued Y motion entry (animSet->entries[][1])
    // 0x26 - only ever cleared (Reset, StateIdle, CheckTurn); nothing reads it
    u8 unk_26;
    u8 idleTimer; // 0x27 - StateIdle's bob timer (Y motion 1 at 30, 2 at 60)
    // 0x28 - gRoomFrameCount + 16 (StartStroke); the state waits until it
    // passes or the anim ends
    u32 deadline;
    u8 motionXPending; // 0x2C - motionX is queued
    u8 motionYPending; // 0x2D - motionY is queued

    /* The state methods, indexed by `state` (gPlayerCtrlStateFuncs,
     * src/data/player_pmf_16c250.c): 0 StateIdle, 1 StateSwim,
     * 2 StateStroke, 3 StateSpin, 4 StateTurn, 5 StateStop,
     * 6 StateSwimStart, 7 StateDead. */
    typedef void (PlayerCtrl::*StateFunc)();
    static const StateFunc stateFuncs[8];

    PlayerCtrl();                                                       // InitPlayerCtrl
    virtual void Update(MovingSprite *part);                            // 1
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg); // 2
    virtual void Attach(MovingSprite *owner);                           // 3
    virtual ~PlayerCtrl();                                              // 9 DestroyPlayerCtrl
    void CheckTurn();
    void KillPlayer(s32 anim);
    void ApplyMotion();
    void StateIdle();
    void StateSwim();
    void StateStroke();
    void StateSpin();
    void StateTurn();
    void StateSwimStart();
    void StateStop();
    void StateDead();
    void StartMotionYFromSet(Player *target, s32 idx);
    void StartMotionXFromSet(Player *target, s32 idx);
    void SetState(s32 newState, s32 newMode, s32 newTimer, s32 newTimerMax);
    void ApplyTilt();
    void StartSwim();
    void ClearUnk14();
    void SetMotionYPending();
    void SetMotionXPending();
    void StartStroke();
    void StartSpin();
    void ApplySwimDrift();
    void Reset();   // ResetPlayerCtrl, action_ctrl.cpp
    void Restart(); // RestartPlayerCtrl, action_ctrl.cpp
    void ClearMotionYPending();
    void ClearMotionXPending();
    u8 IsMotionYPending();
    u8 IsMotionXPending();
    void QueueMotionY(u8 entry);
    void QueueMotionX(u8 entry);

    /* The player's X and Y drift ramps (`rampX`/`rampY`): they use only
     * gPlayer. */
    static void SetDriftX(s32 start, s32 step, s32 target);
    static void SetDriftY(s32 start, s32 step, s32 target);
    static s32 GetDriftStep(s32 v);
};

COMPILE_TIME_ASSERT(player_ctrl_hpp, sizeof(PlayerCtrl) == sizeof(struct player_ctrl));

#endif /* !GUARD_PLAYER_CTRL_HPP */
