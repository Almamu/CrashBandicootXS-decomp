#include "action_ctrl.hpp"
#include "sprite_obj.hpp"
#include "player.hpp"
#include "audio.hpp"

extern "C" {
#include "globals.h"
}

/* ActionCtrl's small methods (include/action_ctrl.hpp, gActionCtrlVtable;
 * #664, docs/cplusplus.md): the empty and forwarding state methods, the
 * mode+animation setter, the target animation override (the skid swap),
 * the constructor and destructor, and the motion queue accessors. The C
 * wrote out the virtual calls (`_call_via_r2`/`_call_via_r3` through the
 * method table); here they are `SetMode(...)` and `SetTargetAnim(...)`.
 * The swim controller's Reset and Restart, which the ROM puts right after
 * them, start swim_ctrl_stroke.cpp. */

/* gActionCtrlStateTable's slot 6 (and StateNop2 slot 2): empty handlers
 * of states nothing sets. */
void ActionCtrl::StateNop6()
{
}

/* The turbo run: the plain run's state method, after restarting the run
 * (StartRun) if `turboRun` was cleared. */
void ActionCtrl::StateTurboRun()
{
    if (turboRun == 0)
        StartRun();
    StateRun();
}

void ActionCtrl::StateNop2()
{
}

/* gActionCtrlStateTable's slot 1, a state nothing sets: runs the idle
 * state's method. */
void ActionCtrl::StateUnusedIdle()
{
    StateIdle();
}

/* Enters `mode` on target animation `anim`, then sets `frame`/`frames`
 * unless they are the 0x7FFFFFFF "keep" sentinel. */
void ActionCtrl::SetModeAnim(s32 mode, s32 anim, s32 frame, s32 frames)
{
    SetMode(mode);
    SetTargetAnim(part, anim);
    if (frame != 0x7FFFFFFF)
        this->frame = frame;
    if (frames != 0x7FFFFFFF)
        this->frames = frames;
}

/* Ctrl::SetTargetAnim, with the skid animations on slippery ground: the
 * idle animation (0x12) becomes 0x25 while the player still moves, the
 * run and turbo run animations (0xD, 0x18) become 0x26, each starting the
 * skid sound; any other animation stops it. The `s32` return (ctrl.hpp)
 * passes Ctrl::SetTargetAnim's result through as is; with `u8`, g++
 * zero-extends it after the call, which the ROM doesn't (the C needed a
 * cast of SetCtrlTargetAnim to an `s32` function, and gotos for the
 * switch's layout). */
s32 ActionCtrl::SetTargetAnim(MovingSprite *part, s32 anim)
{
    Player *player = gPlayer;

    if (player->slippery) {
        switch (anim) {
        case 0x12:
            if (player->speedX == 0)
                break;
            anim = 0x25;
            goto skid;
        case 0xD:
        case 0x18:
            anim = 0x26;
        skid:
            gAudioContext->StopSfx(SFX_SKID);
            gAudioContext->PlaySfx(SFX_SKID, 0x100);
            break;
        default:
            gAudioContext->StopSfx(SFX_SKID);
            break;
        }
    }
    return Ctrl::SetTargetAnim(part, anim);
}

/* Back to idle (animation 0x12) with both motion entries 0. */
void ActionCtrl::Restart()
{
    SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, 0);
    motionXKeepSpeed = 0;
    motionXPending = 1;
    motionX = 0;
    motionYKeepSpeed = 0;
    motionYPending = 1;
    motionY = 0;
}

ActionCtrl::~ActionCtrl()
{
}

ActionCtrl::ActionCtrl()
{
    Reset();
}

/* The motion queue accessors: `motionX`/`motionY` are the queued entries,
 * `motionXPending`/`motionYPending` whether ApplyMotion applies them and
 * `motionXKeepSpeed`/`motionYKeepSpeed` whether it keeps the speed
 * (SetTargetMotionX/Y instead of StartTargetMotionX/Y). */

/* UNUSED, and nothing reads the word (Reset also clears it), so it stays
 * unnamed. */
void ActionCtrl::ClearUnk14()
{
    unk_14 = 0;
}

void ActionCtrl::SetMotionYKeepSpeed()
{
    motionYKeepSpeed = 1;
}

void ActionCtrl::SetMotionXKeepSpeed()
{
    motionXKeepSpeed = 1;
}

void ActionCtrl::SetMotionYPending()
{
    motionYPending = 1;
}

void ActionCtrl::SetMotionXPending()
{
    motionXPending = 1;
}

void ActionCtrl::ClearMotionYPending()
{
    motionYPending = 0;
}

void ActionCtrl::ClearMotionXPending()
{
    motionXPending = 0;
}

u8 ActionCtrl::IsMotionYPending()
{
    return motionYPending;
}

u8 ActionCtrl::IsMotionXPending()
{
    return motionXPending;
}

void ActionCtrl::QueueMotionYKeepSpeed(s32 entry)
{
    motionYKeepSpeed = 1;
    motionYPending = 1;
    motionY = entry;
}

void ActionCtrl::QueueMotionXKeepSpeed(s32 entry)
{
    motionXKeepSpeed = 1;
    motionXPending = 1;
    motionX = entry;
}

void ActionCtrl::QueueMotionY(s32 entry)
{
    motionYKeepSpeed = 0;
    motionYPending = 1;
    motionY = entry;
}

void ActionCtrl::QueueMotionX(s32 entry)
{
    motionXKeepSpeed = 0;
    motionXPending = 1;
    motionX = entry;
}

/* UNUSED - no caller anywhere in the ROM (no `bl` in src/, no Thumb
 * pointer to it in baserom.gba). */
u8 ActionCtrl::GetPrevState()
{
    return prevState;
}
