#ifndef GUARD_PLAYER_H
#define GUARD_PLAYER_H

/* The player subsystem (src/player/): the player object (class Player,
 * include/player.hpp: InitPlayer, UpdatePlayer, DrawPlayer, its flag
 * accessors and collision) and its controllers, the C++ classes ActionCtrl
 * (action_ctrl.hpp, the on-foot state machine), InputCtrl
 * (input_ctrl.hpp), PlayerCtrl (player_ctrl.hpp, the swim controller) and
 * the boss controllers (boss_ctrl.hpp).
 *
 * All of the player's code is C++. `struct player` below is the C view of
 * Player (gPlayer, globals.h); no C file reads it any more (bonus_round.c
 * was the last). This header keeps the data the C++ code and the data
 * tables share. */

#include "core.h"
#include "actor.h"
#include "actor_self.h"
#include "aabb.h"
#include "vtable.h"
#include "objects.h"
#include "constants/action_states.h"
#include "constants/attack_kinds.h"

struct follow_child;
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

/* The player object (gPlayer) as the C files see it: the C view of class
 * Player (include/player.hpp, which checks every named field's offset
 * against the class's), a ground sprite (GroundSprite's 0x80-byte base)
 * with the player's own fields after it. The fields have the class's
 * names and types; the class's notes describe them. Where the class has
 * an anonymous union (Sprite's `bank` and `mirror` bits), which agbcc
 * can't express, the view has its first member, and a class pointer is a
 * `void *`. No C file reads it any more. */
struct player {
    s32 x;   // 0x00 - Q8
    s32 y;   // 0x04 - Q8
    u16 id;  // 0x08
    u8 kind; // 0x0A - the object kind sent to the hit handlers: 0x13 while
             //        attacking, 0x14-0x16 during some attack actions, else 1
    u8 unused_0B;
    union EntityFlags f; // 0x0C (actor.h)
    s16 halfW;           // 0x10
    s16 halfH;           // 0x12
    u8 rawW;             // 0x14
    u8 rawH;             // 0x15
    u8 unused_16[2];
    const void *vtable;             // 0x18 - gPlayerVtable (the class's vtable pointer)
    void *lastHitbox;               // 0x1C
    const struct sprite_bank *bank; // 0x20 - the sprite bank (sprite_bank.h)
    u8 dir;                         // 0x24 - PLAYER_DIR_*
/* `dir`'s bits (motion direction, ApplyPlayerVelocity); also struct
 * camera_target.dir (level.h), the same byte (PlayRoom points
 * gCamera->target at gPlayer). */
#define PLAYER_DIR_RIGHT 1
#define PLAYER_DIR_LEFT 2
#define PLAYER_DIR_UP 4
#define PLAYER_DIR_DOWN 8
#define PLAYER_DIR_X 3   // PLAYER_DIR_RIGHT | PLAYER_DIR_LEFT
#define PLAYER_DIR_Y 0xC // PLAYER_DIR_UP | PLAYER_DIR_DOWN
    u8 screenSpace; // 0x25
    u8 unk_26[2];
    u8 mirror;     // 0x28 - bit 4: X mirrored, bit 5: Y mirrored
    u8 palette:4;  // 0x29 - the OBJ palette slot
    u32:20;        // 0x29 bit 4-0x2B: the unused rest of the 0x28 word
    u8 animating;  // 0x2C
    u8 tag;        // 0x2D - the animation in the bank
    s32 frame;     // 0x30
    s32 stepTimer; // 0x34
    u8 animDone;   // 0x38
    u8 unk_39[3];
    u16 affine;              // 0x3C
    s32 unk_40;              // 0x40
    void *mover;             // 0x44 - the room kind's controller (Ctrl *)
    struct speed_ramp rampX; // 0x48
    struct speed_ramp rampY; // 0x54
    s32 speedX;              // 0x60
    s32 speedY;              // 0x64 - > 0: falling
    u8 hitAxes;              // 0x68 - the axes the terrain probe resolved (8: Y, standing; 4: X)
    u8 probeTries;           // 0x69
    s32 prevX;               // 0x6C
    s32 prevY;               // 0x70
    s32 hitMask;             // 0x74 - the probe axes hit this frame (bits 0-1: X, 2-3: Y)
/* hitMask's axes: the swim controller zeroes speedX on an X hit and
 * speedY on a Y hit (PlayerCtrl::Update, swim_ctrl.cpp). */
#define PLAYER_HIT_X 3
#define PLAYER_HIT_Y 0xC
    s32 type; // 0x78 - ResetPlayer clears it
    u8 unk_7C[4];
    u8 busy; // 0x80
    u8 unk_81[7];
    u8 ctrlMode;  // 0x88
    u32 deadline; // 0x8C - the gRoomFrameCount frame IsPlayerInvulnerable tests against
    u8 bumped;    // 0x90
    u8 countdown; // 0x91
    u8 bounce;    // 0x92
    u8 unk_93;
    u8 listCount;             // 0x94
    void *list[5];            // 0x98 - Crate *s
    void *carried;            // 0xAC - a Sprite *
    void *child;              // 0xB0 - a Sprite *
    s32 maskTrailIdx;         // 0xB4
    struct vec2 maskTrail[8]; // 0xB8 (aabb.h)
    u8 unk_F8[8];
    u8 slippery;                           // 0x100
    u8 hanging;                            // 0x101
    u8 pushLeft;                           // 0x102
    u8 pushRight;                          // 0x103
    u8 dead;                               // 0x104
    u8 cleared;                            // 0x105
    struct collision_queue collisionQueue; // 0x108 (objects.h)
};

COMPILE_TIME_ASSERT(player_h, sizeof(struct player) == 0x350);

/* The controllers share a base, ctrl.hpp's class Ctrl (InitCtrl/
 * DestroyCtrl, ctrl.cpp): +0x04 the motion entry set (SetCtrlAnimSet),
 * +0x08 the state, +0x0C the method table. Most subclasses keep their
 * controlled part at +0x10. They are all C++ classes with no C view: the
 * action controller ActionCtrl (action_ctrl.hpp), the swim controller
 * PlayerCtrl (player_ctrl.hpp), the input controller InputCtrl
 * (input_ctrl.hpp) and the boss controllers (boss_ctrl.hpp). The C files
 * see them only as gPlayerCtrl's `void *`. */

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

/* Aku Aku's orbit frame counters (DrawPlayer, src/iwram/iwram_data.c). */
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
