#ifndef GUARD_PLAYER_H
#define GUARD_PLAYER_H

/* The player subsystem (src/player/): the player object (InitPlayer,
 * UpdatePlayer, DrawPlayer, its flag accessors and collision), and its
 * controllers: the action controller (class ActionCtrl, action_ctrl.hpp,
 * the on-foot state machine), the input controller, the boss controller and
 * the swim controller (`struct player_ctrl`, player_ctrl.h).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The player object is class Player (include/player.hpp; all of its code
 * is C++), and `struct player` below is its C view (gPlayer, globals.h) for
 * the C files; the prototypes of its methods keep their C names
 * (cxx_symbols.txt) and take it. Each controller's functions take its
 * struct: `struct act` (the action controller; an incomplete type, as all
 * of its code is C++), `struct player_ctrl` (player_ctrl.h), and `struct
 * input_ctrl` below. ResetActionCtrl
 * (src/pickups/wumpa.cpp) is here, with the rest of the action
 * controller. */

#include "core.h"
#include "actor_self.h"
#include "vtable.h"
#include "objects.h"
#include "constants/action_states.h"
#include "constants/attack_kinds.h"

struct act;
struct box_part;
struct crate;
struct gobj;
struct follow_child;
struct input_ctrl;
struct player_ctrl;

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
 * (the bit names are part_ctrl.h's `struct ctrl_target`). The views give
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

/* The player's method table (gPlayerVtable), as the C callers read it
 * (room_frame.c): a `this` adjustment and the function of each slot. The
 * slot names are the functions gPlayerVtable holds. */
struct player_vtable {
    struct actor_method unk_00;             // 0x00 - empty (no RTTI)
    struct actor_method collide;            // 0x08 - CollidePlayer
    struct actor_method getHitbox;          // 0x10 - GetSpriteObjHitbox
    struct actor_method update;             // 0x18 - UpdatePlayer
    struct actor_method draw;               // 0x20 - DrawPlayer
    struct actor_method isOnScreen;         // 0x28 - IsSpriteObjOnScreen
    struct actor_method overlapsRect;       // 0x30 - SpriteObjOverlapsRect
    struct actor_method isNearCamera;       // 0x38 - IsSpriteObjNearCamera
    struct actor_method isInsideRect;       // 0x40 - IsSpriteObjInsideRect
    struct actor_method getClassId;         // 0x48 - GetGroundSpriteClassId
    struct actor_method destroy;            // 0x50 - DestroyPlayer
    struct actor_method getPriority;        // 0x58 - GetSpriteObjPriority
    struct actor_method applyVelocity;      // 0x60 - ApplyPlayerVelocity
    struct actor_method handleEvent;        // 0x68 - PlayerHandleEvent (the hit handler)
    struct actor_method collideWithObjects; // 0x70 - CollidePlayerWithObjects
};

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
    const struct player_vtable *vtable; // 0x18 - gPlayerVtable
    void *lastHitbox;                   // 0x1C - struct gobj.lastHitbox
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
 * controlled part at +0x10. The action controller is class ActionCtrl
 * (action_ctrl.hpp) and the swim controller `struct player_ctrl`
 * (player_ctrl.h); the input controller is below, the boss controllers
 * are boss_ctrl.hpp's classes. */

/* The input controller (gInputCtrlVtable, src/player/input_ctrl.cpp and
 * input_ctrl_queue.cpp): the player's controller in the rooms where the
 * player is moved by the input alone. The C view of
 * include/input_ctrl.hpp's class InputCtrl, which keeps the same layout
 * (checked there). Its code is all C++; the C files only pass it around
 * (play_room.c creates it). */
struct input_ctrl {
    u8 unk_00[4];
    const struct entry_set *animSet;  // 0x04
    s32 state;                        // 0x08
    const struct vtable_slot *vtable; // 0x0C - gInputCtrlVtable
    struct player *target;            // 0x10
    u8 motionX;                       // 0x14 - queued X motion entry (animSet->entries[][0])
    u8 motionY;                       // 0x15 - queued Y motion entry (animSet->entries[][1])
    u8 dirState;                      // 0x16
    u8 motionXPending;                // 0x17 - ApplyInputCtrlMotion applies motionX
    u8 motionYPending;                // 0x18 - ApplyInputCtrlMotion applies motionY
    u8 motionXKeepSpeed; // 0x19 - apply with SetCtrlTargetMotionX (speed kept), not Start...
    u8 motionYKeepSpeed; // 0x1A - the same for Y
    u8 unk_1B;
    struct follow_child *cameraLead; // 0x1C - the camera lead (class CameraLead, level_select.hpp)
    u8 flag20;                       // 0x20
    u8 unk_21[3];
    s32 timer; // 0x24
};

/* The controllers' state functions, indexed by state
 * (src/data/action_table_16bf20.c, player_pmf_16c250.c). */
extern const struct actor_pmf gActionCtrlStateTable[ACTION_STATE_COUNT];
extern const struct actor_pmf gPlayerCtrlStateFuncs[8];
extern const struct actor_pmf gInputCtrlStateFuncs[4];

/* The attack kind of each action controller state (QueueCratePlayerCollision,
 * src/data/object_tables_16bb6c.c). */
extern const s32 gActionCtrlStateAttackKinds[ACTION_STATE_COUNT];

/* The swim controller's animations: one row of 13 tilt levels per mode
 * (src/data/speed_table_16c090.c, action_table_16bf20.c), and the
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

/* src/player/action_ctrl.cpp */
extern void ActionCtrlStateNop6(void);
extern void ActionCtrlStateTurboRun(struct act *self);
extern void ActionCtrlStateNop2(void);
extern void ActionCtrlStateUnusedIdle(struct act *self);
extern struct act *InitActionCtrl(struct act *self);

/* src/player/action_ctrl_hang.cpp */
extern void ActionCtrlStateLeftGround(struct act *self);
extern void ActionCtrlStateDying(struct act *self);
extern void ActionCtrlStateWarpIn(struct act *self);
extern void ActionCtrlStateHang(struct act *self);
extern void ActionCtrlStateUnusedHang(struct act *self);
extern void ActionCtrlReleaseHang(struct act *self);
extern void ActionCtrlStateHangMoveStart(struct act *self);
extern void ActionCtrlStateHangMove(struct act *self);
extern void ActionCtrlStateHangStop(struct act *self);
extern void DoSuperBodySlamShockwave(struct act *self);

/* src/player/action_ctrl_idle.cpp */
extern void ActionCtrlStateIdle(struct act *self);

/* src/player/action_ctrl_land.cpp */
extern void ActionCtrlStateCrawlStandUp(struct act *self);
extern void ActionCtrlStateBodySlamLand(struct act *self);
extern void ActionCtrlStateLand(struct act *self);

/* src/player/action_ctrl_moves.cpp */
extern void ActionCtrlStateUnusedHangRelease(struct act *self);
extern void ActionCtrlStateUnusedHangGrab(struct act *self);
extern void ActionCtrlStateHangSpin(struct act *self);
extern void ActionCtrlStateHangGrab(struct act *self);
extern void ActionCtrlStateWarpOut(struct act *self);
extern void ActionCtrlStateCrawlStop(struct act *self);
extern void ActionCtrlStateBodySlamStart(struct act *self);

/* src/player/action_ctrl_run_jump.cpp */
extern void ActionCtrlStateRun(struct act *self);
extern void ActionCtrlStateJump(struct act *self);

/* src/player/action_ctrl_states.cpp */
extern void ActionCtrlStateAirborne(struct act *self);
extern void ActionCtrlStateFlipBodySlamStart(struct act *self);
extern void ActionCtrlStateSlide(struct act *self);
extern void ActionCtrlStateSpin(struct act *self);
extern void ActionCtrlStateAirSpin(struct act *self);
extern void ActionCtrlStateTornadoSpin(struct act *self);
extern void ActionCtrlStateCrouchDown(struct act *self);
extern void ActionCtrlStateCrouch(struct act *self);
extern void ActionCtrlStateStandUp(struct act *self);
extern void ActionCtrlStateCrawlStart(struct act *self);
extern void ActionCtrlStateCrawl(struct act *self);

/* src/player/input_ctrl.cpp: the swim controller's motion queue
 * accessors (PlayerCtrl, include/player_ctrl.hpp), then InputCtrl's methods
 * (include/input_ctrl.hpp), under their C names (cxx_symbols.txt), for the
 * vtables, the state tables and the C callers. */
extern void InputCtrlStateStart(struct input_ctrl *self);
extern void InputCtrlStateDead(struct input_ctrl *self);
extern void InputCtrlStateUnusedRide(struct input_ctrl *self);
extern void InputCtrlStateRide(struct input_ctrl *self);
extern struct input_ctrl *CreateInputCtrl(struct input_ctrl *self);

/* src/player/kill_player.cpp */
extern void KillPlayer(struct act *self, s32 id);

/* src/player/player_*.cpp: Player's methods (include/player.hpp) under
 * their C names (cxx_symbols.txt), for the C callers. */

/* src/player/player_collide.cpp */
extern u8 CollidePlayer(struct player *self);

/* src/player/player_flags.cpp: Player's accessors (include/player.hpp). */
extern void SetPlayerBusy(struct player *self, u8 arg1);
/* Ctrl's methods (include/ctrl.hpp) under their C names
 * (cxx_symbols.txt), for the C callers. */
extern void SetCtrlAnimSet(void *self, s32 val);

/* src/player/player_init.cpp */
extern struct player *InitPlayer(struct player *self, u16 arg1, u16 arg2, u16 arg3, u16 unused);

/* src/player/player_reset.cpp */
extern void ResetPlayerForRoom(struct player *self);

/* src/player/swim_ctrl.cpp, swim_ctrl_drift.cpp, swim_ctrl_stroke.cpp:
 * the swim controller's methods (PlayerCtrl, include/player_ctrl.hpp)
 * under their C names (cxx_symbols.txt), for the state table and the C
 * callers. */
extern void PlayerCtrlStateIdle(struct player_ctrl *self);
extern void PlayerCtrlStateSwim(struct player_ctrl *self);
extern void PlayerCtrlStateStroke(struct player_ctrl *self);
extern void PlayerCtrlStateSpin(struct player_ctrl *self);
extern void PlayerCtrlStateTurn(struct player_ctrl *self);
extern void PlayerCtrlStateSwimStart(struct player_ctrl *self);
extern void PlayerCtrlStateStop(struct player_ctrl *self);
extern void PlayerCtrlStateDead(struct player_ctrl *self);
/* SetPlayerCtrlState's `timer`/`timerMax` value that keeps the current
 * one. */
#define CTRL_KEEP 0x7FFFFFFF
extern struct player_ctrl *InitPlayerCtrl(struct player_ctrl *self);

#endif /* GUARD_PLAYER_H */
