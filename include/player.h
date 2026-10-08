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
 * Player (gPlayer, globals.h) for the C files left (bonus_round.c); this
 * header keeps the data the C++ code and the data tables share. */

#include "core.h"
#include "actor_self.h"
#include "vtable.h"
#include "objects.h"
#include "constants/action_states.h"
#include "constants/attack_kinds.h"

struct box_part;
struct crate;
struct gobj;
struct follow_child;

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
    u8 pad[2];
};

/* A sprite bank as the player code reads it (struct sprite_bank,
 * sprite_bank.h): its animation records, 0x1C bytes each (gobj_1a794.h's
 * `struct anim_rec`). */
struct act_anim_record {
    u8 unk_00[4];
    s16 offX; // 0x04 - a struct hitbox_quad (gfx.h)
    s16 offY; // 0x06
    u8 padX;  // 0x08
    u8 padY;  // 0x09
    u8 unk_0A[0xA];
    u8 paletteId; // 0x14 - LoadPaletteSlot/GetPaletteSlot record id
    u8 unk_15;
    u8 frameCount; // 0x16
    u8 unk_17[5];
};

struct act_anim_bank {
    struct act_anim_record *records;
    u8 unk_04[6];
    u16 animCount; // 0x0A - struct sprite_bank.animCount
};

/* A Q8 position (struct player.maskTrail). */
struct player_pos {
    s32 x;
    s32 y;
};

/* The flags byte at +0x0C (struct actor.flags), as a byte or as bits
 * (the bit names are entity.hpp's `union EntityFlags`). The views give
 * different code: clearing a bit through the bitfield is an `and` with a
 * negative constant, through the byte with a positive one. Packed, so
 * that the union is one byte (agbcc pads an unpacked one to 4). */
union player_flags {
    u8 all;
    struct {
        u8 gone:1;
        u8 unk_1:1;
        u8 visible:1;
        u8 hit:1;
        u8 flag4:1;
        u8 unk_5:1;
        u8 flag6:1;
        u8 flag7:1;
    } __attribute__((packed)) bits;
} __attribute__((packed));

/* The mirror byte at +0x28 (bit 4: X mirrored, bit 5: Y mirrored), as a
 * byte (the action controller), as `u32` bits (the swim controller,
 * crate_hit.cpp, crate_touch.cpp) or as `s32` bits (crate_break.cpp, the
 * layout of `struct crate`). The bit views read the same, but the signed
 * one expands to more insns before optimization, which shifts the
 * `.LCB` label numbers in the `.s`. Packed, so that the union is one byte. */
union player_mirror {
    u8 all;
    struct {
        u8 unk_0:4;
        u32 flipX:1;
        u32 flipY:1;
        u8 unk_6:2;
    } __attribute__((packed)) bits;
    struct {
        u32 unk_0:4;
        s32 flipX:1;
        s32 flipY:1;
        u32 unk_6:2;
    } __attribute__((packed)) sbits;
} __attribute__((packed));

/* The player object (gPlayer) as the C files see it: the C view of class
 * Player (include/player.hpp, which checks the size), a ground sprite
 * (InitGroundSprite, the same 0x80-byte base as gobj_1a794.h's `struct
 * gobj`, whose names it keeps) with the player's own fields after it.
 * PlayRoom builds it in a 0x350-byte block (InitPlayer); its method table
 * is gPlayerVtable. Only the fields the code reads are named. */
struct player {
    s32 x;   // 0x00 - Q8
    s32 y;   // 0x04 - Q8
    u16 id;  // 0x08 - bit index in the "gone" bitmap (InitPlayer: 0xFFFF)
    u8 kind; // 0x0A - object kind passed to the hit handlers: 0x13 while
             //        attacking, 0x14-0x16 during some attack actions, else 1
    u8 unk_0B;
    union player_flags flags; // 0x0C - struct actor.flags: bit 0 gone, 4 always active
                              //        (set by PlayRoom), 6 vulnerable, 7 collision enabled
    u8 flags2;                // 0x0D
    u8 unk_0E[0xA];
    const void *vtable; // 0x18 - gPlayerVtable (class Player's)
    void *lastHitbox;   // 0x1C - struct gobj.lastHitbox
    // 0x20 - the sprite bank (struct sprite_bank, sprite_bank.h)
    struct act_anim_bank *anim;
    // 0x24 - motion direction bits (ApplyPlayerVelocity): 1 right,
    //        2 left, 4 up, 8 down
    u8 dir;
/* `dir`'s bits; also struct camera_target.dirFlags (level.h), the same
 * byte (PlayRoom points gCamera->target at gPlayer). */
#define PLAYER_DIR_RIGHT 1
#define PLAYER_DIR_LEFT 2
#define PLAYER_DIR_UP 4
#define PLAYER_DIR_DOWN 8
#define PLAYER_DIR_X 3   // PLAYER_DIR_RIGHT | PLAYER_DIR_LEFT
#define PLAYER_DIR_Y 0xC // PLAYER_DIR_UP | PLAYER_DIR_DOWN
    u8 screenSpace; // 0x25
    u8 unk_26[2];
    union player_mirror mirror; // 0x28 - bit 4: X mirrored, bit 5: Y mirrored
    u8 slot:4;                  // 0x29 - palette slot (GetSpriteAnimPaletteSlot)
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating; // 0x2C - nonzero while the keyframe timer runs
    u8 tag;       // 0x2D - animation index into `anim`
    u8 unk_2E[2];
    s32 frame;     // 0x30 - step within the animation
    s32 stepTimer; // 0x34 - ticks spent on the current step
    u8 animDone;   // 0x38 - set once a non-looping animation ends
    u8 unk_39[0xB];
    void *ctrl;              // 0x44 - the room kind's controller (the action, swim, input or
                             //        boss controller; struct gobj.mover)
    struct speed_ramp rampX; // 0x48 - speedX's ramp (StartPlayerRampX)
    struct speed_ramp rampY; // 0x54 - speedY's ramp
    s32 speedX;              // 0x60
    s32 speedY;              // 0x64 - > 0: falling
    // 0x68 - collision axes the terrain probe resolved (8: Y, standing; 4: X)
    u8 hitAxes;
    u8 probeTries; // 0x69
    u8 unk_6A[2];
    s32 prevX;   // 0x6C - previous position (Q8), cached by ApplyPlayerVelocity
    s32 prevY;   // 0x70
    u32 hitMask; // 0x74 - probe axes hit this frame (bits 0-1: X, 2-3: Y)
/* hitMask's axes: the swim controller zeroes speedX on an X hit and
 * speedY on a Y hit (UpdatePlayerCtrl, swim_ctrl.cpp). */
#define PLAYER_HIT_X 3
#define PLAYER_HIT_Y 0xC
    s32 type; // 0x78 - struct gobj.type; ResetPlayer clears it
    u8 unk_7C[4];
    u8 busy; // 0x80 - set while a triggered crate animation runs (the crate's state
             //        bit 7), cleared when it ends; enemies skip the player meanwhile
    u8 unk_81[7];
    u8 ctrlMode; // 0x88 - control mode 0-3, picks the controller (player_reset.c);
                 //        1: crates fall at quarter speed and touched enemies just
                 //        vanish; nonzero stops `list` recording
    u8 unk_89[3];
    u32 deadline; // 0x8C - gRoomFrameCount frame IsPlayerInvulnerable tests against
    u8 bumped;    // 0x90 - set when a crate's side stopped the X motion
                  //        (ActionCtrlHandleEvent event 12); cleared when the
                  //        controller's bumpTimer runs out or its mode changes.
                  //        While set, crate_hit.cpp widens the player's box by 2 px
                  //        on each side
    u8 countdown; // 0x91 - crate-break limiter: BreakCrateInStack arms it (2) and skips the
                  //        break while it runs; UpdatePlayer counts it down
    u8 bounce;    // 0x92 - a counter (crate_break.cpp's name): crate_break.cpp tests and
                  //        steps it on a bounce, ResolvePlayerCollisions steps it, the action
                  //        controller clears it
    u8 unk_93;
    u8 listCount; // 0x94 - entries in `list`
    u8 unk_95[3];
    struct crate *list[5];          // 0x98 - the recently touched crates
    struct gobj *carried;           // 0xAC - the platform or crate the player stands on
    struct box_part *child;         // 0xB0 - a sprite object InitPlayer creates (sprite bank 0xCC),
                                    //        drawn with the player (DrawPlayer)
    s32 maskTrailIdx;               // 0xB4 - the newest entry of `maskTrail`
    struct player_pos maskTrail[8]; // 0xB8 - the player's recent positions, which Aku Aku follows
    u8 unk_F8[8];
    u8 slippery; // 0x100 - standing on terrain kind 5 (CollidePlayer): the player keeps
                 //         sliding (speedX isn't zeroed, motion keeps its speed, steps halve)
                 //         and skids (anims 0x25/0x26, sfx 0x36; ActionCtrlSetTargetAnim)
    // 0x101 - hanging from hang terrain (code 6): CollidePlayer sends event
    //         0x17 to grab and 0x18 when it's gone; ActionCtrlHandleEvent sets/clears it
    u8 hanging;
    u8 pushLeft;  // 0x102 - nonzero: moves the standing player 1px left per frame
    u8 pushRight; // 0x103 - nonzero: moves the standing player 1px right per frame
    u8 dead;      // 0x104 - the player died (KillPlayer and the other controllers'
                  //         kill handlers); blocks pause and further hits
    u8 cleared;   // 0x105 - CollidePlayerWithObjects
    u8 unk_106[2];
    // 0x108 - the embedded collision queue (objects.h;
    //         ResetCollisionQueue/DestroyCollisionQueue; GetPlayerCollisionQueue
    //         returns its address). Its `posCommitted` (0x10C) is the "position
    //         committed" byte
    struct collision_queue collisionQueue;
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
