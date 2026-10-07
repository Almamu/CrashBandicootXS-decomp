#ifndef GUARD_CONSTANTS_ACTION_STATES_H
#define GUARD_CONSTANTS_ACTION_STATES_H

/*
 * The player's action-controller states: ActionCtrl's `state` (and
 * `prevState`), the index into gActionCtrlStateTable
 * (src/data/action_table_16bf20.c) and gActionCtrlStateAttackKinds. A
 * state is entered through the controller's set-mode method
 * (ActionCtrl::SetMode, SetActionCtrlMode: vtable slot 4) or
 * ActionCtrl::SetModeAnim (SetActionCtrlModeAnim).
 *
 * Each name is the handler in the state's slot (ActionCtrlStateRun gives
 * ACTION_STATE_RUN). ActionCtrlStateAirborne handles six states; their
 * names come from the code that enters each one (ActionCtrlStateJump,
 * TryActionCtrlDoubleJump, HandleActionCtrlAirInput,
 * ActionCtrlStateBodySlamStart). Nothing sets states 1, 2, 6, 0x22,
 * 0x23, 0x24 or 0x27.
 */

#define ACTION_STATE_IDLE 0
#define ACTION_STATE_UNUSED_IDLE 1 // ActionCtrlStateUnusedIdle
#define ACTION_STATE_NOP2 2        // ActionCtrlStateNop2, unused
#define ACTION_STATE_RUN 3
#define ACTION_STATE_TURBO_RUN 4 // L held with HasTurboRun
#define ACTION_STATE_JUMP 5      // the takeoff; then AIRBORNE_JUMP or AIRBORNE_FLIP_JUMP
#define ACTION_STATE_NOP6 6      // ActionCtrlStateNop6, unused
#define ACTION_STATE_AIRBORNE_JUMP 7      // ActionCtrlStateJump without a held direction
#define ACTION_STATE_BODY_SLAM_START 8    // R in the air after a plain jump
#define ACTION_STATE_AIRBORNE_FLIP_JUMP 9 // ActionCtrlStateJump with a held direction; double jump
#define ACTION_STATE_FLIP_BODY_SLAM_START 0xA // R in the air after a flip jump
#define ACTION_STATE_AIRBORNE_HIGH_JUMP 0xB   // StartActionCtrlHighJump, StartActionCtrlMaskHitJump
#define ACTION_STATE_SLIDE 0xC
#define ACTION_STATE_SPIN 0xD
#define ACTION_STATE_AIR_SPIN 0xE
#define ACTION_STATE_TORNADO_SPIN 0xF
#define ACTION_STATE_CROUCH_DOWN 0x10
#define ACTION_STATE_CROUCH 0x11
#define ACTION_STATE_STAND_UP 0x12
#define ACTION_STATE_CRAWL_START 0x13
#define ACTION_STATE_CRAWL 0x14
#define ACTION_STATE_CRAWL_STAND_UP 0x15
#define ACTION_STATE_BODY_SLAM_LAND 0x16
#define ACTION_STATE_LAND 0x17
#define ACTION_STATE_AIRBORNE_BODY_SLAM 0x18       // after BODY_SLAM_START
#define ACTION_STATE_AIRBORNE_SUPER_BODY_SLAM 0x19 // after BODY_SLAM_START with HasSuperBodySlam
#define ACTION_STATE_AIRBORNE_FALL 0x1A
#define ACTION_STATE_CRAWL_STOP 0x1B
#define ACTION_STATE_LEFT_GROUND 0x1C
#define ACTION_STATE_DYING 0x1D // KillPlayer
#define ACTION_STATE_WARP_OUT 0x1E
#define ACTION_STATE_HANG_GRAB 0x1F
#define ACTION_STATE_HANG 0x20
#define ACTION_STATE_HANG_SPIN 0x21
#define ACTION_STATE_UNUSED_HANG 0x22      // ActionCtrlStateUnusedHang
#define ACTION_STATE_UNUSED_HANG_GRAB 0x23 // ActionCtrlStateUnusedHangGrab
#define ACTION_STATE_RELEASE_HANG 0x24     // ActionCtrlReleaseHang, unused as a state
#define ACTION_STATE_HANG_MOVE_START 0x25
#define ACTION_STATE_HANG_MOVE 0x26
#define ACTION_STATE_UNUSED_HANG_RELEASE 0x27 // ActionCtrlStateUnusedHangRelease
#define ACTION_STATE_HANG_STOP 0x28
#define ACTION_STATE_WARP_IN 0x29
#define ACTION_STATE_COUNT 42

#endif /* GUARD_CONSTANTS_ACTION_STATES_H */
