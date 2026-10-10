#ifndef GUARD_PLAYER_H
#define GUARD_PLAYER_H

/* The player subsystem (src/player/): the player object (class Player,
 * include/player.hpp: InitPlayer, UpdatePlayer, DrawPlayer, its flag
 * accessors and collision) and its controllers, the C++ classes ActionCtrl
 * (action_ctrl.hpp, the on-foot state machine), InputCtrl
 * (input_ctrl.hpp), SwimCtrl (swim_ctrl.hpp, the swim controller) and
 * the boss controllers (boss_ctrl.hpp).
 *
 * All of the player's code is C++, and Player has no C view (#754). This
 * header keeps what C needs: the record types and tables src/data/*.c
 * defines, and the C-linkage data the C++ shares with them. */

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
 * (state 4), copied to the stack in one go by StartSwimCtrlStroke
 * (gSwimCtrlTurnSpeeds). */
struct speed_table {
    s32 v[8];
};

/* The animation of a swim controller mode at one tilt level
 * (gSwimCtrlModeLevelAnims), and a second byte (0xFF in some rows). */
struct level_anim {
    u8 anim;
    u8 unk_1;
};

/* The attack kind of each action controller state (QueueCratePlayerCollision,
 * src/data/object_tables_16bb6c.cpp). */
extern const s32 gActionCtrlStateAttackKinds[ACTION_STATE_COUNT];

/* The swim controller's animations: one row of 13 tilt levels per mode
 * (src/data/speed_table_16c090.cpp, action_table_16bf20.cpp), and the
 * stroke speeds. */
extern const struct level_anim gSwimCtrlModeLevelAnims[8][13];
extern const struct level_anim *const gSwimCtrlModeAnimRows[8];
extern const struct speed_table gSwimCtrlTurnSpeeds;

/* The player's speedY after its last ApplyPlayerVelocity (sym_iwram.txt;
 * gLastSpriteVelY is the moving sprites'). Nothing reads it. */
extern s32 gLastPlayerVelY;

/* Aku Aku's orbit frame counters (DrawPlayer, src/iwram/iwram_data.cpp). */
extern s32 gAkuAkuInvincibleFrame;
extern s32 gAkuAkuFollowFrame;

/* The player controller's and the input controller's motion records
 * (src/data/motion_records_16b304.cpp; `struct speed_ramp`, objects.h). */
extern const struct speed_ramp gSwimCtrlMotionRecords[31];
extern const struct speed_ramp gInputCtrlMotionRecords[9];

/* The entry sets PlayRoom gives the action, player and input controllers
 * through SetCtrlAnimSet (src/data/entry_set_16b92c.cpp,
 * src/data/entry_set_16b93c.cpp). */
extern const struct entry_set gActionCtrlMotionSet;
extern const struct entry_set gSwimCtrlMotionSet;
extern const struct entry_set gInputCtrlMotionSet;

/* The player's controller (sym_iwram.txt), built by PlayRoom. It names
 * SwimCtrl's own tag: an incomplete struct to C, the class to C++. */
extern struct SwimCtrl *gSwimCtrl;

#endif /* GUARD_PLAYER_H */
