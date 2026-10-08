#ifndef GUARD_PLAYER_H
#define GUARD_PLAYER_H

/* The player subsystem (src/player/): the player object (class Player,
 * include/player.hpp: InitPlayer, UpdatePlayer, DrawPlayer, its flag
 * accessors and collision) and its controllers, the C++ classes ActionCtrl
 * (action_ctrl.hpp, the on-foot state machine), InputCtrl
 * (input_ctrl.hpp), PlayerCtrl (player_ctrl.hpp, the swim controller) and
 * the boss controllers (boss_ctrl.hpp).
 *
 * All of the player's code is C++, and Player has no C view (#754). This
 * header keeps the data the C++ code and the data tables share. */

#include "core.h"
#include "actor.h"
#include "actor_self.h"
#include "aabb.h"
#include "vtable.h"
#include "objects.h"
#include "constants/action_states.h"
#include "constants/attack_kinds.h"

struct sprite_bank;

/* The swim controller's turn: speedX at each frame of the turn animation
 * (state 4), copied to the stack in one go by StartPlayerCtrlStroke
 * (gPlayerCtrlTurnSpeeds). */
struct speed_table {
    s32 v[8];
};

/* The animation of a swim controller mode at one tilt level
 * (gPlayerCtrlModeLevelAnims), and a second byte (0xFF in some rows). */
struct level_anim {
    u8 anim;
    u8 unk_1;
};

/* Player::dir's bits (the motion direction, ApplyPlayerVelocity); also
 * the camera's followed sprite's (PlayRoom points gCamera->target at
 * gPlayer). */
#define PLAYER_DIR_RIGHT 1
#define PLAYER_DIR_LEFT 2
#define PLAYER_DIR_UP 4
#define PLAYER_DIR_DOWN 8
#define PLAYER_DIR_X 3   // PLAYER_DIR_RIGHT | PLAYER_DIR_LEFT
#define PLAYER_DIR_Y 0xC // PLAYER_DIR_UP | PLAYER_DIR_DOWN

/* Player::hitMask's axes: the swim controller zeroes speedX on an X hit
 * and speedY on a Y hit (PlayerCtrl::Update, swim_ctrl.cpp). */
#define PLAYER_HIT_X 3
#define PLAYER_HIT_Y 0xC

/* The controllers share a base, ctrl.hpp's class Ctrl (InitCtrl/
 * DestroyCtrl, ctrl.cpp): +0x04 the motion entry set (SetCtrlAnimSet),
 * +0x08 the state, +0x0C the method table. Most subclasses keep their
 * controlled part at +0x10. They are all C++ classes with no C view: the
 * action controller ActionCtrl (action_ctrl.hpp), the swim controller
 * PlayerCtrl (player_ctrl.hpp), the input controller InputCtrl
 * (input_ctrl.hpp) and the boss controllers (boss_ctrl.hpp). gPlayerCtrl
 * is a `void *`. */

/* The attack kind of each action controller state (QueueCratePlayerCollision,
 * src/data/object_tables_16bb6c.c). */
extern const s32 gActionCtrlStateAttackKinds[ACTION_STATE_COUNT];

/* The swim controller's animations: one row of 13 tilt levels per mode
 * (src/data/speed_table_16c090.c, action_table_16bf20.cpp), and the
 * stroke speeds. */
extern const struct level_anim gPlayerCtrlModeLevelAnims[8][13];
extern const struct level_anim *const gPlayerCtrlModeAnimRows[8];
extern const struct speed_table gPlayerCtrlTurnSpeeds;

/* The player's speedY after its last ApplyPlayerVelocity (sym_iwram.txt;
 * gLastSpriteVelY is the moving sprites'). Nothing reads it. */
extern s32 gLastPlayerVelY;

/* Aku Aku's orbit frame counters (DrawPlayer, src/iwram/iwram_data.cpp). */
extern s32 gAkuAkuInvincibleFrame;
extern s32 gAkuAkuFollowFrame;

/* The player controller's and the input controller's motion records
 * (src/data/motion_records_16b304.c; `struct speed_ramp`, objects.h). */
extern const struct speed_ramp gPlayerCtrlMotionRecords[31];
extern const struct speed_ramp gInputCtrlMotionRecords[9];

/* The entry sets PlayRoom gives the action, player and input controllers
 * through SetCtrlAnimSet (src/data/entry_set_16b92c.c,
 * src/data/entry_set_16b93c.c). */
extern const struct entry_set gActionCtrlMotionSet;
extern const struct entry_set gPlayerCtrlMotionSet;
extern const struct entry_set gInputCtrlMotionSet;

/* The player's controller (sym_iwram.txt), built by PlayRoom. */
extern void *gPlayerCtrl;

/* SetPlayerCtrlState's `timer`/`timerMax` value that keeps the current
 * one. */
#define CTRL_KEEP 0x7FFFFFFF

#endif /* GUARD_PLAYER_H */
