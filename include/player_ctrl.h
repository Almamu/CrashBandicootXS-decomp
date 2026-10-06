#ifndef GUARD_PLAYER_CTRL_H
#define GUARD_PLAYER_CTRL_H

/* The player-input controller object of src/player/swim_ctrl.c
 * (GitHub issues #19/#20, ROM 0x08016048-0x08017524): the underwater
 * (scuba-diving) controller play_room.c attaches in room kind 1, with
 * sprite bank 1 (Crash in an air tank and flippers). It is a C++-style class
 * with gcc 2.x method table gPlayerCtrlVtable (+0x0C UpdatePlayerCtrl
 * per-frame update, +0x14 PlayerCtrlHandleEvent message handler, +0x1C AttachPlayerCtrl
 * set target, +0x4C DestroyPlayerCtrl destructor; the rest are base-class
 * sub_800B6xx/sub_800B8xx functions). Constructor InitPlayerCtrl (called from
 * play_room.c), whose field reset is action_ctrl.c's ResetPlayerCtrl.
 * The dispatchers StartPlayerCtrlStroke/StartPlayerCtrlSpin/ApplyPlayerCtrlSwimDrift (swim_ctrl_stroke.c) and SetPlayerSwimDriftY (swim_ctrl_drift.c) are methods of the
 * same class. */

struct pctrl_method {
    s16 thisOffset;
    u8 unk_2[2];
    void *fn;
};

struct pctrl_vtable {
    u8 unk_00[0x20];
    struct pctrl_method setMode; // 0x20
    u8 unk_28[0x28];
    struct pctrl_method setAnim; // 0x50
};

struct pctrl_anim_pair {
    u32 a;
    u32 b;
};

struct player_ctrl {
    u8 unk_00[4];
    struct {
        struct pctrl_anim_pair *entries;
    } *animSet;                  // 0x04
    s32 state;                   // 0x08 - index into gPlayerCtrlStateFuncs
    struct pctrl_vtable *vtable; // 0x0C
    struct player *target;       // 0x10 - the player (gPlayer)
    s32 unk_14;                  // 0x14 - only ever cleared (ResetPlayerCtrl, sub_801750C)
    s32 timer;                   // 0x18
    s32 timerMax;                // 0x1C
    u8 repeat;                   // 0x20 - D-pad auto-repeat countdown
    // 0x21 - swim direction, 0 (up) .. 6 (level) .. 12 (down); column of gPlayerCtrlModeAnimRows
    u8 tilt;
    // 0x22 - row of gPlayerCtrlModeAnimRows (0 idle, 1 swim, 2 stroke, 3 spin, 4-7 turn)
    u8 mode;
    u8 spinCooldown; // 0x23 - frames until StartPlayerCtrlSpin is allowed again (set to 12)
    u8 motionX;      // 0x24 - queued X motion entry (animSet->entries[].a)
    u8 motionY;      // 0x25 - queued Y motion entry (animSet->entries[].b)
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
