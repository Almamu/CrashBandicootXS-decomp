#ifndef GUARD_ACTION_CTRL_HPP
#define GUARD_ACTION_CTRL_HPP

/* The action controller as C++ (#664, docs/cplusplus.md): the player's
 * controller on foot (room kind 0; PlayRoom creates it). Its code is in
 * src/player/action_ctrl*.cpp (Reset at the start of
 * action_ctrl_event.cpp, where the ROM puts it). cxx_symbols.txt maps
 * every method declared here to its C name, for the vtable and state
 * table data.
 *
 * No `#pragma interface`: g++ emits its vtable, gActionCtrlVtable, in
 * action_ctrl_update.cpp (see ctrl.hpp). */

#include "ctrl.hpp"
#include "player.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "action_obj.h"
}

/* The action controller (gActionCtrlVtable; 0x38 bytes, as PlayRoom's
 * `InitActionCtrl(OperatorNew(0x38))` allocates). Each frame Update runs
 * the current state's method (`stateFuncs`, indexed by `state`, an
 * ACTION_STATE_* id), then applies the queued motion entries (ApplyMotion):
 * `motionX`/`motionY` name rows of the entry set (`animSet`), applied with
 * the speed kept (SetTargetMotion*) or restarted (StartTargetMotion*). */
class ActionCtrl : public Ctrl
{
public:
    Player *part; // 0x10 - the player (Attach)
    s32 unk_14;   // 0x14 - only ever cleared (Reset, ClearUnk14)
    s32 frame;    // 0x18
    s32 frames;   // 0x1C
    // 0x20 - extra tornado-spin turns queued by pressing B again during a spin
    //        (max 3; needs HasTornadoSpin)
    u8 charge;
    // 0x21 - the current tornado turn's variant, 0-2 (anims 0x17/0x28/0x27, sfx 0x57+n)
    u8 tornadoVariant;
    // 0x22 - tornado turns played: counts up to `charge`, then back down
    //        (StartTornadoSpin); picks StartTornadoFall's descent
    u8 tornadoTurn;
    // 0x23 - StartTornadoFall has queued its slow descent this spin
    u8 tornadoFallQueued;
    // 0x24 - set once `tornadoTurn` reached `charge`: the turns count back down
    u8 tornadoUnwinding;
    // 0x25 - while nonzero (counting down), the idle and airborne states ignore
    //        the D-pad; releasing it clears the timer
    u8 dpadLockTimer;
    u8 spinCooldown;  // 0x26 - frames until the next spin is allowed (set to 12, counts down)
    u8 motionX;       // 0x27 - queued X motion entry (animSet->entries[][0])
    u8 motionY;       // 0x28 - queued Y motion entry (animSet->entries[][1])
    u8 turboRun;      // 0x29 - set on entering the turbo run (L, state 4); landing resumes it
                      //        instead of the plain run; the idle state clears it
    u8 unk_2A;        // 0x2A - only ever cleared (Reset, the flip body slam start in
                      //        HandleAirInput); nothing reads it
    u8 bumpTimer;     // 0x2B - 3 after a crate's side stopped the X motion (event 12); counts down
                      //        while at most one crate is touched, then re-queues bumpedMotionX
    u8 bumpedMotionX; // 0x2C - the motionX that bump cancelled
    u8 prevState;     // 0x2D - `state` before the last SetMode (a flip jump, 9, turns the
                      //        mid-air body slam into the flip body slam)
    // 0x2E - part->slippery last frame; Update calls UpdateSkidAnim on a change
    u8 prevSlippery;
    u8 motionXPending;   // 0x2F - ApplyMotion applies motionX
    u8 motionYPending;   // 0x30 - ApplyMotion applies motionY
    u8 motionXKeepSpeed; // 0x31 - apply with SetTargetMotionX (speed kept), not Start...
    u8 motionYKeepSpeed; // 0x32 - the same for Y
    u8 idleFidget;       // 0x33 - an idle fidget anim (0xE/5/0x1A, after 8/20/30 s) is playing;
                         //        Update doesn't force the idle anim back meanwhile
    u8 slamBlocked;      // 0x34 - R was held through a bounce (events 13/14): the mid-air body
                         //        slam needs R released first (Update clears it then)

    /* The state methods, indexed by `state` (gActionCtrlStateTable,
     * src/data/action_table_16bf20.cpp, which names each state). */
    typedef void (ActionCtrl::*StateFunc)();
    static const StateFunc stateFuncs[ACTION_STATE_COUNT];

    /* gActionCtrlVtable's slots; slots 5-8, 11 and 12 are Ctrl's.
     * HandleEvent is in src/player/action_ctrl_event.cpp. */
    ActionCtrl();                                                       // InitActionCtrl
    virtual void Update(MovingSprite *unused);                          // 1
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg); // 2
    virtual void Attach(MovingSprite *owner);                           // 3
    virtual void SetMode(s32 mode);                                     // 4
    virtual ~ActionCtrl();                                              // 9 DestroyActionCtrl
    virtual s32 SetTargetAnim(MovingSprite *part, s32 anim);            // 10

    /* The motion queue stores as the methods have them inlined: the
     * values are computed before the three stores. QueueX/QueueY set all
     * three bytes; QueueNowX/QueueNowY are QueueMotionX/QueueMotionY. */
    void QueueX(s32 keepSpeed, s32 pending, s32 entry)
    {
        motionXKeepSpeed = keepSpeed;
        motionXPending = pending;
        motionX = entry;
    }
    void QueueY(s32 keepSpeed, s32 pending, s32 entry)
    {
        motionYKeepSpeed = keepSpeed;
        motionYPending = pending;
        motionY = entry;
    }
    void QueueNowX(s32 entry)
    {
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = entry;
    }
    void QueueNowXKeepSpeed(s32 entry)
    {
        motionXKeepSpeed = 1;
        motionXPending = 1;
        motionX = entry;
    }
    void QueueNowY(s32 entry)
    {
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = entry;
    }
    /* QueueNowX with the `motionXKeepSpeed` value a parameter too (a
     * register the method already holds), the 1 still a literal. */
    void QueuePendingX(s32 keepSpeed, s32 entry)
    {
        motionXKeepSpeed = keepSpeed;
        motionXPending = 1;
        motionX = entry;
    }

    /* The same through a pointer to `motionX`/`motionY` that the caller
     * already holds (the ROM keeps the one it tested). */
    void QueueNowXAt(u8 *slot, s32 entry)
    {
        motionXKeepSpeed = 0;
        motionXPending = 1;
        *slot = entry;
    }
    void QueueNowXKeepSpeedAt(u8 *slot, s32 entry)
    {
        motionXKeepSpeed = 1;
        motionXPending = 1;
        *slot = entry;
    }
    void QueueNowYAt(u8 *slot, s32 entry)
    {
        motionYKeepSpeed = 0;
        motionYPending = 1;
        *slot = entry;
    }
    void QueueYAt(u8 *slot, s32 pending, s32 entry)
    {
        motionYKeepSpeed = 0;
        motionYPending = pending;
        *slot = entry;
    }

    /* SetModeAnim inlined, for a `frame` that is set: the frame value is
     * computed before the two calls. */
    void SetModeAnimNow(s32 mode, s32 anim, s32 frame)
    {
        SetMode(mode);
        SetTargetAnim(part, anim);
        this->frame = frame;
    }
    void SetModeAnimNow(s32 mode, s32 anim, s32 frame, s32 frames)
    {
        SetMode(mode);
        SetTargetAnim(part, anim);
        this->frame = frame;
        this->frames = frames;
    }

    /* src/player/action_ctrl_event.cpp */
    void Reset();

    /* src/player/action_ctrl.cpp */
    void StateNop6();
    void StateTurboRun();
    void StateNop2();
    void StateUnusedIdle();
    void SetModeAnim(s32 mode, s32 anim, s32 frame, s32 frames);
    void Restart();
    void ClearUnk14();
    void SetMotionYKeepSpeed();
    void SetMotionXKeepSpeed();
    void SetMotionYPending();
    void SetMotionXPending();
    void ClearMotionYPending();
    void ClearMotionXPending();
    u8 IsMotionYPending();
    u8 IsMotionXPending();
    void QueueMotionYKeepSpeed(s32 entry);
    void QueueMotionXKeepSpeed(s32 entry);
    void QueueMotionY(s32 entry);
    void QueueMotionX(s32 entry);
    u8 GetPrevState();

    /* src/player/action_ctrl_hang.cpp */
    void StateLeftGround();
    void StateDying();
    void StateWarpIn();
    void StateHang();
    void StateUnusedHang();
    void ReleaseHang();
    void StateHangMoveStart();
    void StateHangMove();
    void StateHangStop();
    void DoSuperBodySlamShockwave();
    void StartTornadoSpin(s32 id, s32 param2);

    /* src/player/action_ctrl_idle.cpp */
    void ApplyMotion();
    void StateIdle();

    /* src/player/action_ctrl_land.cpp */
    void StateCrawlStandUp();
    void StateBodySlamLand();
    void StateLand();

    /* src/player/action_ctrl_left_ground.cpp */
    u8 CheckLeftGround();

    /* src/player/action_ctrl_moves.cpp */
    void StartTornadoFall();
    void EndSpin(u8 mode, s32 flags);
    void SteerSpin(u8 mode);
    void StartSpin();
    void StartHangSpin();
    void StartRun();
    void StartHighJump();
    void StartMaskHitJump();
    void StateUnusedHangRelease();
    void StateUnusedHangGrab();
    void StateHangSpin();
    void StateHangGrab();
    void StateWarpOut();
    void StateCrawlStop();
    void StateBodySlamStart();

    /* src/player/action_ctrl_run_jump.cpp */
    void StateRun();
    void StateJump();

    /* src/player/action_ctrl_states.cpp */
    void StateAirborne();
    void StateFlipBodySlamStart();
    void StateSlide();
    void StateSpin();
    void StateAirSpin();
    void StateTornadoSpin();
    void StateCrouchDown();
    void StateCrouch();
    void StateStandUp();
    void StateCrawlStart();
    void StateCrawl();

    /* src/player/action_ctrl_update.cpp */
    u8 TryDoubleJump();
    void HandleAirInput();

    /* src/player/action_ctrl_kill.cpp */
    void KillPlayer(s32 anim);
    void UpdateSkidAnim();
    s32 UpdateFacing();
};

COMPILE_TIME_ASSERT(action_ctrl_hpp, sizeof(ActionCtrl) == 0x38);

/* The player's `slippery` (+0x100), read through an inline function (and
 * written through Player::StoreSlippery): the 0x100 offset is then
 * materialized at each access, as in the ROM, instead of shared with an
 * earlier 0x100 (an R_BUTTON test, a PlaySfx volume) through a register. */
static inline u8 IsSlippery(Player *p)
{
    return p->slippery;
}

/* Byte read-modify-writes of the player's flags2 (+0x0D), through a
 * pointer to it: as a member store, gcc's expansion leaves a dead `& 0`
 * whose 0 CSE then reuses for later zero stores, moving them (see
 * tiny.cpp). The mask arrives as an `s32` parameter so old_agbcp
 * materializes it before the load. */
static inline void ActAndFlags0D(Player *part, s32 mask)
{
    u8 *flags2 = &part->f.bytes.flags2;

    *flags2 &= mask;
}

static inline void ActOrFlags0D(Player *part, s32 bits)
{
    u8 *flags2 = &part->f.bytes.flags2;

    *flags2 |= bits;
}

#endif /* !GUARD_ACTION_CTRL_HPP */
