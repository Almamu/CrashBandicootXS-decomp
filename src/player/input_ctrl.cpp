#include "bg_layer.hpp"
#include "input_ctrl.hpp"
#include "sprite_obj.hpp"
#include "audio.hpp"
#include "level_state.hpp"

extern "C" {
#include "menus.h"
#include "gfx.h"
#include "memory.h"
#include "level.h"
#include "globals.h"
#include "math_util.h"
}

/* GitHub issue #21: 0x08017524-0x08017A44, the whole tail of the former
 * asm/code_3_2_17_16048.s.
 *
 * The first six functions, the swim controller's motion-queue accessors,
 * are at the end of swim_ctrl.cpp since #768.
 *
 * The other 19 are InputCtrl's (include/input_ctrl.hpp, gInputCtrlVtable;
 * #664, docs/cplusplus.md), the controller play_room.cpp attaches in room
 * kind 2, where the player uses sprite bank 2 (Crash riding a hover
 * vehicle; anim 1 is it blowing up). Each frame Update reads the held
 * D-pad bits from `gKeys` and queues motion entries for its target
 * through gInputCtrlMotionRecords: up/down select `motionY`, left/right
 * `motionX`, which also sets the camera lead's target offset
 * (`cameraLead`, spawned on demand by StateStart). Once the target passes
 * the level's right edge (`gLevelLayers`'s layer 0 width, less 0xA00) the
 * camera lead is marked gone and RequestRoomExit is signalled. It then
 * calls the current state's method through `stateFuncs`
 * (gInputCtrlStateFuncs) and applies the queued motion.
 *
 * UNUSED - no `bl`/`.4byte` reference in src/, and no Thumb pointer
 * anywhere in the ROM: SetMotionYPending, SetMotionXPending, CancelMotionY, CancelMotionX and
 * IsMotionYPending. Matched anyway.
 *
 * Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS), like the old_agbcc
 * C it replaces; agbcp's code differs (from KillPlayer's bitfield clears
 * on: old_agbcp loads the constant before the `ldrb`).
 * The C wrote out the virtual calls (a vtable-slot macro around
 * `_call_via_r2`/`_call_via_r3`) and the pointer-to-member dispatch (16
 * lines of slot arithmetic); here they are `SetMode(3)` and
 * `(this->*stateFuncs[state])()`. The "mark gone" bitmap sequence
 * (MarkEntityGone's, inlined twice) is Entity::MarkGone. */

/* The target is hit: plays the hurt sound, switches to state 3 (StateDead)
 * on animation `anim`, makes the target intangible and dead, takes a life
 * and reloads the target's palette. */
void InputCtrl::KillPlayer(s32 anim)
{
    gAudioContext->PlaySfx(SFX_PLAYER_HURT, 0x100);
    SetMode(3);
    SetTargetAnim(target, anim);
    target->f.b.collides = 0;
    target->f.b.vulnerable = 0;
    target->dead = 1;
    gLevelState->LoseLife();
    gPaletteCache->LoadSlot(target->palette, target->bank->anims[target->tag].paletteId);
}

/* gInputCtrlStateFuncs[0]: state 1 on animation 0, X motion entry 1 and
 * Y entry 0 queued, and the camera lead (created and listed the first
 * time) reset. */
void InputCtrl::StateStart()
{
    SetModeAnim(1, 0, 0, 0);
    motionXPending = 1;
    motionX = 1;
    motionYPending = 1;
    motionY = 0;
    dirState = 0;
    if (cameraLead == 0) {
        cameraLead = new CameraLead;
        CollidableList()->Add(cameraLead);
    }
    cameraLead->Reset();
}

/* Each frame, unless dead (state 3): ends the room once the target is past
 * the level's right edge, and steers from the held D-pad (up/down: Y
 * entries 3/5/0; right: X entry 8 and a near camera lead; left, for up to
 * 30 frames at a time: X entry 7 and a far lead; neither: X entry 1). Then
 * the state method, then the queued motion. */
void InputCtrl::Update(MovingSprite *)
{
    if (state != 3) {
        u32 keys;
        s32 x = target->x;

        if (x > INT_TO_Q8(gLevelLayers->layer0->widthPx) - 0xA00) {
            CameraLead *lead = cameraLead;

            lead->MarkGone();
            cameraLead = 0;
            RequestRoomExit();
        }

        keys = gKeys.all;
        if ((keys & DPAD_UP) && dirState != 1) {
            QueueNowY(3);
            dirState = 1;
        } else {
            if ((keys & DPAD_DOWN) && dirState != 2) {
                QueueNowY(5);
                dirState = 2;
            } else if (!(keys & (DPAD_UP | DPAD_DOWN))) {
                QueueNowY(0);
                dirState = 0;
            }
        }

        if ((keys & DPAD_LEFT) && flag20) {
            QueueNowX(7);
            SetLeadSpeed(0x3200);
            if (++timer > 30) {
                flag20 = 0;
                timer = 10;
            }
        } else if (keys & DPAD_RIGHT) {
            QueueNowX(8);
            SetLeadSpeed(0xA00);
        } else if (!(keys & DPAD_SIDEWAYS) || ((keys & DPAD_LEFT) && !flag20)) {
            SetLeadSpeed(0x1E00);
            QueueNowX(1);
        }

        if (!flag20 && --timer < 0) {
            timer = 0;
            if (!(keys & DPAD_LEFT))
                flag20 = 1;
        }
    }

    (this->*stateFuncs[state])();
    ApplyMotion();
}

/* Applies the queued motion entries: each names a record of
 * gInputCtrlMotionRecords through the entry set's {X, Y} pairs, set with
 * the speed kept (SetTargetMotion*) or started (StartTargetMotion*). */
void InputCtrl::ApplyMotion()
{
    if (motionXPending == 1) {
        const speed_ramp *rec = &gInputCtrlMotionRecords[animSet->entries[motionX][0]];

        if (motionXKeepSpeed)
            SetTargetMotionX(target, &rec->start);
        else
            StartTargetMotionX(target, &rec->start);
        motionXPending = 0;
        motionXKeepSpeed = 0;
    }
    if (motionYPending == 1) {
        const speed_ramp *rec = &gInputCtrlMotionRecords[animSet->entries[motionY][1]];

        if (motionYKeepSpeed)
            SetTargetMotionY(target, rec);
        else
            StartTargetMotionY(target, rec);
        motionYPending = 0;
        motionYKeepSpeed = 0;
    }
}

/* Sets the state and the target's animation; the last two arguments are
 * unused. */
void InputCtrl::SetModeAnim(s32 mode, s32 anim, s32, s32)
{
    SetMode(mode);
    SetTargetAnim(target, anim);
}

/* gInputCtrlStateFuncs[3]: once the death animation ends, the target is
 * gone. */
void InputCtrl::StateDead()
{
    Player *t = target;

    if (t->animDone)
        t->MarkGone();
}

/* gInputCtrlStateFuncs[2]: once the target's animation ends, goes back to
 * state 1 on animation 0 and queues X motion entry 2 (the same cruise
 * record as entry 1, restarted). Only StateRide sets state 2, and only
 * when animation 0 ends, which never happens (see there), so this state
 * is never reached. */
void InputCtrl::StateUnusedRide()
{
    if (target->animDone) {
        SetModeAnim(1, 0, 0, 0);
        QueueNowX(2);
    }
}

/* gInputCtrlStateFuncs[1], the state the controller stays in for the whole
 * hover ride (Reset and StateStart select it; the D-pad steering is in
 * Update). It would switch to state 2 once the target's animation ends,
 * but the animation is 0, sprite bank 2's riding loop (SPRITE_ANIM_LOOP),
 * which never sets animDone. */
void InputCtrl::StateRide()
{
    if (target->animDone)
        SetModeAnim(2, 0, 0, 0);
}

/* State 0 on animation 0, both motion entries 0 queued. Nothing calls
 * it. */
void InputCtrl::Restart()
{
    SetModeAnim(0, 0, 0, 0);
    motionXPending = 1;
    motionX = 0;
    motionYPending = 1;
    motionY = 0;
}

/* State 1, entries 0 queued, no target or camera lead yet. */
void InputCtrl::Reset()
{
    state = 1;
    motionX = 0;
    motionY = 0;
    motionXPending = 1;
    motionYPending = 1;
    target = 0;
    cameraLead = 0;
    flag20 = 0;
}

/* Events 1-4 (the hits) kill the player on animation 1. */
void InputCtrl::HandleEvent(MovingSprite *, s32 event, s32)
{
    /* a non-literal lower bound keeps gcc from folding `>= 1` into
     * `> 0` (the ROM compares against 1) and from merging the two tests
     * into one unsigned range check */
    s32 lo = EVENT_HIT;

    if (event >= lo && event <= EVENT_HIT_EXPLOSION)
        KillPlayer(1);
}

/* The controlled sprite object is the player. */
void InputCtrl::Attach(MovingSprite *owner)
{
    target = (Player *)owner;
}

/* g++ stores gInputCtrlVtable and calls ~Ctrl (DestroyCtrl). */
InputCtrl::~InputCtrl()
{
}

/* Ctrl(), the vtable pointer, then Reset. play_room.cpp builds it with
 * `new InputCtrl`. */
InputCtrl::InputCtrl()
{
    Reset();
}

/* The motion queue's flags (the queue setters are in
 * input_ctrl_queue.cpp). */
void InputCtrl::SetMotionYPending()
{
    motionYPending = 1;
}

void InputCtrl::SetMotionXPending()
{
    motionXPending = 1;
}

void InputCtrl::CancelMotionY()
{
    motionYPending = 0;
    motionYKeepSpeed = 0;
}

void InputCtrl::CancelMotionX()
{
    motionXPending = 0;
    motionXKeepSpeed = 0;
}

u8 InputCtrl::IsMotionYPending()
{
    return motionYPending;
}
