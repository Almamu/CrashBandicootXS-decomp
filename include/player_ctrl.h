#ifndef GUARD_PLAYER_CTRL_H
#define GUARD_PLAYER_CTRL_H

/* The swim controller: the C view of include/player_ctrl.hpp's C++ class
 * PlayerCtrl (src/player/swim_ctrl.cpp, swim_ctrl_drift.cpp and
 * swim_ctrl_stroke.cpp, method table gPlayerCtrlVtable; #664,
 * docs/cplusplus.md), same layout. It drives the diving Crash of the
 * room-kind-1 (underwater) rooms, with sprite bank 1 (Crash in an air
 * tank and flippers). Its C users are play_room.c (InitPlayerCtrl) and
 * action_ctrl.c (ResetPlayerCtrl, RestartPlayerCtrl: PlayerCtrl's Reset
 * and Restart, still C). */

struct entry_set;
struct vtable_slot;
struct player;

struct player_ctrl {
    u8 unk_00[4];
    const struct entry_set *animSet;  // 0x04
    s32 state;                        // 0x08 - index into gPlayerCtrlStateFuncs
    const struct vtable_slot *vtable; // 0x0C - gPlayerCtrlVtable
    struct player *target;            // 0x10 - the player (gPlayer)
    s32 unk_14;                       // 0x14 - only ever cleared (ResetPlayerCtrl, sub_801750C)
    s32 timer;                        // 0x18
    s32 timerMax;                     // 0x1C
    u8 repeat;                        // 0x20 - D-pad auto-repeat countdown
    // 0x21 - swim direction, 0 (up) .. 6 (level) .. 12 (down); column of gPlayerCtrlModeAnimRows
    u8 tilt;
    // 0x22 - row of gPlayerCtrlModeAnimRows (0 idle, 1 swim, 2 stroke, 3 spin, 4-7 turn)
    u8 mode;
    u8 spinCooldown; // 0x23 - frames until StartPlayerCtrlSpin is allowed again (set to 12)
    u8 motionX;      // 0x24 - queued X motion entry (animSet->entries[][0])
    u8 motionY;      // 0x25 - queued Y motion entry (animSet->entries[][1])
    // 0x26 - only ever cleared (ResetPlayerCtrl, PlayerCtrlStateIdle, CheckPlayerCtrlTurn);
    // nothing reads it
    u8 unk_26;
    u8 idleTimer; // 0x27 - PlayerCtrlStateIdle's bob timer (Y motion 1 at 30, 2 at 60)
    // 0x28 - gRoomFrameCount + 16 (StartPlayerCtrlStroke);
    // the state waits until it passes or the anim ends
    u32 deadline;
    u8 motionXPending; // 0x2C - motionX is queued
    u8 motionYPending; // 0x2D - motionY is queued
};

#endif /* GUARD_PLAYER_CTRL_H */
