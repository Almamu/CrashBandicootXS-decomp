#ifndef GUARD_ACTION_OBJ_H
#define GUARD_ACTION_OBJ_H

/* The C view of include/action_ctrl.hpp's class ActionCtrl (#664, the
 * same layout): the player's action controller. Its code is all C++
 * (src/player/action_ctrl*.cpp, kill_player.cpp); wumpa.c's
 * ResetActionCtrl, still C, is the last C user of the struct, and the C
 * prototypes in player.h (for the vtable and state table data) take it.
 * The input and flags2 helpers below are the C++ files'. */

#include "player.h"

struct act {
    u8 unk_00[4];
    const struct entry_set *animSet; // 0x04
    s32 state;                       // 0x08
    const void *vtable;              // 0x0C - gActionCtrlVtable
    struct player *part;             // 0x10
    s32 unk_14;                      // 0x14 - only ever cleared (ResetActionCtrl, sub_80158AC)
    s32 frame;                       // 0x18
    s32 frames;                      // 0x1C
    // 0x20 - extra tornado-spin turns queued by pressing B again during a spin
    //        (max 3; needs HasTornadoSpin)
    u8 charge;
    // 0x21 - the current tornado turn's variant, 0-2 (anims 0x17/0x28/0x27, sfx 0x57+n)
    u8 tornadoVariant;
    // 0x22 - tornado turns played: counts up to `charge`, then back down
    //        (StartActionCtrlTornadoSpin); picks StartActionCtrlTornadoFall's descent
    u8 tornadoTurn;
    // 0x23 - StartActionCtrlTornadoFall has queued its slow descent this spin
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
    u8 unk_2A;        // 0x2A - only ever cleared (ResetActionCtrl, the flip body slam start
                      //        in HandleActionCtrlAirInput); nothing reads it
    u8 bumpTimer;     // 0x2B - 3 after a crate's side stopped the X motion (event 12); counts down
                      //        while at most one crate is touched, then re-queues bumpedMotionX
    u8 bumpedMotionX; // 0x2C - the motionX that bump cancelled
    u8 prevState;     // 0x2D - `state` before the last SetActionCtrlMode (a flip jump, 9,
                      //        turns the mid-air body slam into the flip body slam)
    // 0x2E - part->slippery last frame; UpdateActionCtrl calls UpdateActionCtrlSkidAnim on a change
    u8 prevSlippery;
    u8 motionXPending;   // 0x2F - ApplyActionCtrlMotion applies motionX
    u8 motionYPending;   // 0x30 - ApplyActionCtrlMotion applies motionY
    u8 motionXKeepSpeed; // 0x31 - apply with SetCtrlTargetMotionX (speed kept), not Start...
    u8 motionYKeepSpeed; // 0x32 - the same for Y
    u8 idleFidget;       // 0x33 - an idle fidget anim (0xE/5/0x1A, after 8/20/30 s) is playing;
                         //        UpdateActionCtrl doesn't force the idle anim back meanwhile
    u8 slamBlocked;      // 0x34 - R was held through a bounce (events 13/14): the mid-air body
                         //        slam needs R released first (UpdateActionCtrl clears it then)
};

/* gKeys is the input word: low half held, high half newly
 * pressed. Handlers copy it to a stack slot and read the halves back from
 * there; the halves go through the local's address (a union or struct
 * member read is folded into a halfword load of the global itself). */
#define INPUT_HELD(in) (*(u16 *)&(in))
#define INPUT_PRESSED(in) (*(u16 *)((u8 *)&(in) + 2))

/* Byte read-modify-writes of part+0x0D, through a plain byte pointer: as
 * a struct member store, gcc's expansion leaves a dead `& 0` whose 0 CSE
 * then reuses for later zero stores, moving them (see
 * tiny_update.cpp). The mask arrives as an `s32` parameter so
 * old_agbcc materializes it before the load. */
#define ACT_PART_FLAGS0D(p) (*((u8 *)(p) + 0xD))

static inline void ActAndFlags0D(struct player *part, s32 mask)
{
    ACT_PART_FLAGS0D(part) &= mask;
}

static inline void ActOrFlags0D(struct player *part, s32 bits)
{
    ACT_PART_FLAGS0D(part) |= bits;
}

#endif // GUARD_ACTION_OBJ_H
