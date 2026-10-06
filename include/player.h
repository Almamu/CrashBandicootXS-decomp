#ifndef GUARD_PLAYER_H
#define GUARD_PLAYER_H

/* The player subsystem (src/player/): the player object (InitPlayer,
 * UpdatePlayer, DrawPlayer, its flag accessors and collision), and its
 * controllers: the action controller (`struct act`, action_obj.h, the
 * on-foot state machine), the input controller, the boss controller and
 * the swim controller (`struct player_ctrl`, player_ctrl.h).
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The player object is `struct player` below (gPlayer, globals.h); the
 * player's own functions take it. The action controller's functions
 * still take `void *` or `struct act *` (action_obj.h).
 * ResetActionCtrl (src/pickups/wumpa.c) is here, with the rest of the
 * action controller. */

#include "core.h"
#include "actor_self.h"
#include "vtable.h"
#include "objects.h"

struct act;
struct box_part;
struct crate;
struct gobj;
struct input_ctrl;
struct pctrl_motion_queue;
struct player_ctrl;
struct vec3;

/* The swim stroke's speed per step, copied to the stack in one go by
 * StartPlayerCtrlStroke (gStaticData_0816C090). */
struct speed_table
{
    s32 v[8];
};

/* The animation of a swim controller mode at one tilt level
 * (gPlayerCtrlModeLevelAnims), and a second byte (0xFF in some rows). */
struct level_anim
{
    u8 anim;
    u8 unk_1;
    u8 pad[2];
};

/* A sprite bank as the player code reads it (struct sprite_bank,
 * sprite_bank.h): its animation records, 0x1C bytes each (gobj_1a794.h's
 * `struct anim_rec`). */
struct act_anim_record
{
    u8 unk_00[4];
    s16 offX;              // 0x04 - a struct hitbox_quad (gfx.h)
    s16 offY;              // 0x06
    u8 padX;               // 0x08
    u8 padY;               // 0x09
    u8 unk_0A[0xA];
    u8 paletteId;          // 0x14 - LoadPaletteSlot/GetPaletteSlot record id
    u8 unk_15;
    u8 frameCount;         // 0x16
    u8 unk_17[5];
};

struct act_anim_bank
{
    struct act_anim_record *records;
    u8 unk_04[6];
    u16 unk_0A;            // 0x0A
};

/* A Q8 position (struct player.maskTrail). */
struct player_pos
{
    s32 x;
    s32 y;
};

/* The flags byte at +0x0C (struct actor.flags), as a byte or as bits
 * (the bit names are part_ctrl.h's `struct ctrl_target`). The views give
 * different code: clearing a bit through the bitfield is an `and` with a
 * negative constant, through the byte with a positive one. Packed, so
 * that the union is one byte (agbcc pads an unpacked one to 4). */
union player_flags
{
    u8 all;
    struct
    {
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
 * crate_hit.c, crate_touch.c) or as `s32` bits (crate_break.c, the
 * layout of `struct crate`). The bit views read the same, but the signed
 * one expands to more insns before optimization, which shifts the
 * `.LCB` label numbers in the `.s`. Packed, so that the union is one byte. */
union player_mirror
{
    u8 all;
    struct
    {
        u8 unk_0:4;
        u32 flipX:1;
        u32 flipY:1;
        u8 unk_6:2;
    } __attribute__((packed)) bits;
    struct
    {
        u32 unk_0:4;
        s32 flipX:1;
        s32 flipY:1;
        u32 unk_6:2;
    } __attribute__((packed)) sbits;
} __attribute__((packed));

/* The player's method table (gPlayerVtable), as the callers read it: a
 * `this` adjustment and the function of each slot. The slot names are
 * the functions gPlayerVtable holds. */
struct player_vtable
{
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

/* The player object (gPlayer): a ground sprite (InitGroundSprite, the
 * same 0x80-byte base as gobj_1a794.h's `struct gobj`, whose names it
 * keeps) with the player's own fields after it. PlayRoom builds it in a
 * 0x350-byte block (InitPlayer); its method table is gPlayerVtable. The
 * names past 0x80 come from the accessors in player_flags.c. Only the
 * fields the code reads are named. */
struct player
{
    s32 x;              // 0x00 - Q8
    s32 y;              // 0x04 - Q8
    u16 id;             // 0x08 - bit index in the "gone" bitmap (InitPlayer: 0xFFFF)
    u8 kind;            // 0x0A - object kind passed to the hit handlers: 0x13 while
                        //        attacking, 0x14-0x16 during some attack actions, else 1
    u8 unk_0B;
    union player_flags flags; // 0x0C - struct actor.flags: bit 0 gone, 4 always active
                        //        (set by PlayRoom), 6 vulnerable, 7 collision enabled
    u8 flags2;          // 0x0D
    u8 unk_0E[0xA];
    const struct player_vtable *vtable; // 0x18 - gPlayerVtable
    void *platform;     // 0x1C - struct gobj.platform
    struct act_anim_bank *anim; // 0x20 - the sprite bank (struct sprite_bank, sprite_bank.h)
    u8 dir;             // 0x24 - motion direction bits (ApplyPlayerVelocity): 1 right,
                        //        2 left, 4 up, 8 down
    u8 screenSpace;     // 0x25
    u8 unk_26[2];
    union player_mirror mirror; // 0x28 - bit 4: X mirrored, bit 5: Y mirrored
    u8 slot:4;          // 0x29 - palette slot (GetSpriteAnimPaletteSlot)
    u8 unk_29_4:4;
    u8 unk_2A[2];
    u8 animating;       // 0x2C - nonzero while the keyframe timer runs
    u8 tag;             // 0x2D - animation index into `anim`
    u8 unk_2E[2];
    s32 frame;          // 0x30 - step within the animation
    s32 stepTimer;      // 0x34 - ticks spent on the current step
    u8 animDone;        // 0x38 - set once a non-looping animation ends
    u8 unk_39[0xB];
    void *ctrl;         // 0x44 - the room kind's controller (the action, swim, input or
                        //        boss controller; struct gobj.mover)
    struct speed_ramp rampX; // 0x48 - speedX's ramp (StartPlayerRampX)
    struct speed_ramp rampY; // 0x54 - speedY's ramp
    s32 speedX;         // 0x60
    s32 speedY;         // 0x64 - > 0: falling
    u8 hitAxes;         // 0x68 - collision axes the terrain probe resolved (8: Y, standing; 4: X)
    u8 probeTries;      // 0x69
    u8 unk_6A[2];
    s32 prevX;          // 0x6C - previous position (Q8), cached by ApplyPlayerVelocity
    s32 prevY;          // 0x70
    u32 hitMask;        // 0x74 - probe axes hit this frame (bits 0-1: X, 2-3: Y)
    u8 unk_78[8];
    u8 busy;            // 0x80 - set while a triggered crate animation runs (the crate's state
                        //        bit 7), cleared when it ends; enemies skip the player meanwhile
    u8 unk_81[7];
    u8 ctrlMode;        // 0x88 - control mode 0-3, picks the controller (player_reset.c);
                        //        1: crates fall at quarter speed and touched enemies just
                        //        vanish; nonzero stops `list` recording
    u8 unk_89[3];
    u32 deadline;       // 0x8C - gRoomFrameCount frame IsPlayerInvulnerable tests against
    u8 bumped;          // 0x90 - set when a crate's side stopped the X motion
                        //        (ActionCtrlHandleEvent event 12); cleared when the
                        //        controller's bumpTimer runs out or its mode changes.
                        //        While set, crate_hit.c widens the player's box by 2 px
                        //        on each side
    u8 countdown;       // 0x91 - crate_break.c sets it while a crate handles the player
    u8 bounce;          // 0x92 - a counter (sub_800B5C4/sub_800B5CC/sub_800B5D8; crate_break.c's
                        //        name): crate_break.c tests and steps it on a bounce,
                        //        ResolvePlayerCollisions steps it, the action controller clears it
    u8 unk_93;
    u8 listCount;       // 0x94 - entries in `list`
    u8 unk_95[3];
    struct crate *list[5]; // 0x98 - the recently touched crates (sub_800B678)
    struct gobj *carried; // 0xAC - the platform or crate the player stands on
    struct box_part *child; // 0xB0 - a sprite object InitPlayer creates (sprite bank 0xCC),
                        //        drawn with the player (DrawPlayer)
    s32 maskTrailIdx;   // 0xB4 - the newest entry of `maskTrail`
    struct player_pos maskTrail[8]; // 0xB8 - the player's recent positions, which Aku Aku follows
    u8 unk_F8[8];
    u8 slippery;        // 0x100 - standing on terrain kind 5 (CollidePlayer): the player keeps
                        //         sliding (speedX isn't zeroed, motion keeps its speed, steps halve)
                        //         and skids (anims 0x25/0x26, sfx 0x36; ActionCtrlSetTargetAnim)
    u8 hanging;         // 0x101 - hanging from hang terrain (code 6): CollidePlayer sends event
                        //         0x17 to grab and 0x18 when it's gone; ActionCtrlHandleEvent sets/clears it
    u8 pushLeft;        // 0x102 - nonzero: moves the standing player 1px left per frame
    u8 pushRight;       // 0x103 - nonzero: moves the standing player 1px right per frame
    u8 dead;            // 0x104 - the player died (KillPlayer and the other controllers'
                        //         kill handlers); blocks pause and further hits
    u8 cleared;         // 0x105 - CollidePlayerWithObjects
    u8 unk_106[2];
    u8 collisionQueue[4]; // 0x108 - the embedded collision queue (ResetCollisionQueue/
                          //         DestroyCollisionQueue; GetPlayerCollisionQueue returns its address)
    u8 unk_10C;         // 0x10C - the queue's "position committed" byte (ResetCollisionQueue
                        //         clears it, crate_break.c's D18C_COMMIT sets it): nonzero,
                        //         ApplyCrateCollision leaves the position alone
};

/* The method tables (src/data/entity_vtables_7e3bec.c). */
extern const struct vtable_slot gPlayerVtable[15];
extern const struct vtable_slot gActionCtrlVtable[13];
extern const struct vtable_slot gPlayerCtrlVtable[13];
extern const struct vtable_slot gInputCtrlVtable[13];
extern const struct vtable_slot gBossCtrlVtable[13];

/* The controllers' state functions, indexed by state
 * (src/data/action_table_16bf20.c, player_pmf_16c250.c). */
extern const struct actor_pmf gActionCtrlStateTable[42];
extern const struct actor_pmf gPlayerCtrlStateFuncs[8];
extern const struct actor_pmf gInputCtrlStateFuncs[4];

/* The attack kind of each action controller state (QueueCratePlayerCollision,
 * src/data/object_tables_16bb6c.c). */
extern const s32 gActionCtrlStateAttackKinds[42];

/* The swim controller's animations: one row of 13 tilt levels per mode
 * (src/data/speed_table_16c090.c, action_table_16bf20.c), and the
 * stroke speeds. */
extern const struct level_anim gPlayerCtrlModeLevelAnims[8][13];
extern const struct level_anim *const gPlayerCtrlModeAnimRows[8];
extern const struct speed_table gStaticData_0816C090;

/* Aku Aku's orbit frame counters (DrawPlayer, src/iwram/iwram_data.c). */
extern s32 gAkuAkuInvincibleFrame;
extern s32 gAkuAkuFollowFrame;

/* The player controller's and the input controller's motion records
 * (src/data/motion_records_16b304.c; `struct motion_rec`, objects.h). */
extern const struct motion_rec gPlayerCtrlMotionRecords[31];
extern const struct motion_rec gInputCtrlMotionRecords[9];

/* The entry sets PlayRoom gives the action, player and input controllers
 * through SetCtrlAnimSet (src/data/entry_set_16b92c.c,
 * src/data/entry_set_16b93c.c). */
extern const struct entry_set gActionCtrlMotionSet;
extern const struct entry_set gPlayerCtrlMotionSet;
extern const struct entry_set gInputCtrlMotionSet;

/* The player's controller (sym_iwram.txt), built by PlayRoom. */
extern void *gPlayerCtrl;

/* src/pickups/wumpa.c */
extern void ResetActionCtrl(void *self);

/* src/player/action_ctrl.c */
extern void nullsub_17(void);
extern void ActionCtrlStateTurboRun(void *self);
extern void nullsub_18(void);
extern void sub_8015774(void *self);
extern void SetActionCtrlModeAnim(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 ActionCtrlSetTargetAnim(void *arg0, void *other, s32 mode);
extern void RestartActionCtrl(void *self);
extern void DestroyActionCtrl(void *self, s32 flags);
extern void *InitActionCtrl(void *self);
extern void sub_80158AC(void *self);
extern void SetActionCtrlMotionYKeepSpeed(void *self);
extern void SetActionCtrlMotionXKeepSpeed(void *self);
extern void SetActionCtrlMotionYPending(void *self);
extern void SetActionCtrlMotionXPending(void *self);
extern void ClearActionCtrlMotionYPending(void *self);
extern void ClearActionCtrlMotionXPending(void *self);
extern u8 IsActionCtrlMotionYPending(void *self);
extern u8 IsActionCtrlMotionXPending(void *self);
extern void QueueActionCtrlMotionYKeepSpeed(void *self, s32 val);
extern void QueueActionCtrlMotionXKeepSpeed(void *self, s32 val);
extern void QueueActionCtrlMotionY(void *self, s32 val);
extern void QueueActionCtrlMotionX(void *self, s32 val);
extern u8 sub_8015950(void *self);
extern void ResetPlayerCtrl(void *self);
extern void RestartPlayerCtrl(void *self);

/* src/player/action_ctrl_event.c */
extern void ActionCtrlHandleEvent(struct act *self, s32 arg1, s32 arg2, s32 arg3);

/* src/player/action_ctrl_hang.c */
extern void ActionCtrlStateLeftGround(struct act *self);
extern void ActionCtrlStateDying(struct act *self);
extern void ActionCtrlStateWarpIn(struct act *self);
extern void ActionCtrlStateHang(struct act *self);
extern void sub_8014AEC(struct act *self);
extern void ActionCtrlReleaseHang(struct act *self);
extern void ActionCtrlStateHangMoveStart(struct act *self);
extern void ActionCtrlStateHangMove(struct act *self);
extern void ActionCtrlStateHangStop(struct act *self);
extern void DoSuperBodySlamShockwave(void *self);
extern void StartActionCtrlTornadoSpin(u8 *self, s32 id, s32 param2);

/* src/player/action_ctrl_idle.c */
extern void ApplyActionCtrlMotion(struct act *self);
extern void ActionCtrlStateIdle(struct act *self);

/* src/player/action_ctrl_land.c */
extern void ActionCtrlStateCrawlStandUp(struct act *self);
extern void ActionCtrlStateBodySlamLand(struct act *self);
extern void ActionCtrlStateLand(struct act *self);

/* src/player/action_ctrl_left_ground.c */
extern u8 CheckActionCtrlLeftGround(void *self);

/* src/player/action_ctrl_moves.c */
extern void sub_80151C8(void *self);
extern void EndActionCtrlSpin(struct act *self, u8 mode, s32 flags);
extern void SteerActionCtrlSpin(u8 *self, u8 mode);
extern void SetActionCtrlMode(void *self, s32 arg1);
extern void StartActionCtrlSpin(void *self);
extern void StartActionCtrlHangSpin(void *self);
extern void StartActionCtrlRun(void *self);
extern void StartActionCtrlHighJump(void *self);
extern void sub_8015558(void *self);
extern void AttachActionCtrl(void *self, void *val);
extern void sub_80155AC(void *self);
extern void sub_80155B8(void *self);
extern void ActionCtrlStateHangSpin(void *self);
extern void ActionCtrlStateHangGrab(void *self);
extern void ActionCtrlStateWarpOut(void *self);
extern void ActionCtrlStateCrawlStop(void *self);
extern void ActionCtrlStateBodySlamStart(u8 *self);

/* src/player/action_ctrl_run_jump.c */
extern void ActionCtrlStateRun(struct act *self);
extern void ActionCtrlStateJump(struct act *self);

/* src/player/action_ctrl_states.c */
extern void ActionCtrlStateAirborne(struct act *self);
extern void ActionCtrlStateFlipBodySlamStart(struct act *self);
extern void ActionCtrlStateSlide(struct act *self);
extern void ActionCtrlStateSpin(struct act *self);
extern void ActionCtrlStateAirSpin(struct act *self);
extern void ActionCtrlStateTornadoSpin(struct act *self);
extern void ActionCtrlStateCrouchDown(struct act *self);
extern void ActionCtrlStateCrouch(struct act *self);
extern void ActionCtrlStateStandUp(void *self);
extern void ActionCtrlStateCrawlStart(void *self);
extern void ActionCtrlStateCrawl(struct act *self);

/* src/player/action_ctrl_update.c */
extern void UpdateActionCtrl(struct act *self);
extern u8 TryActionCtrlDoubleJump(struct act *self);
extern void HandleActionCtrlAirInput(struct act *self);

/* src/player/input_ctrl.c */
extern void ClearPlayerCtrlMotionYPending(struct pctrl_motion_queue *self);
extern void ClearPlayerCtrlMotionXPending(struct pctrl_motion_queue *self);
extern u8 IsPlayerCtrlMotionYPending(struct pctrl_motion_queue *self);
extern u8 IsPlayerCtrlMotionXPending(struct pctrl_motion_queue *self);
extern void QueuePlayerCtrlMotionY(struct pctrl_motion_queue *self, u8 value);
extern void QueuePlayerCtrlMotionX(struct pctrl_motion_queue *self, u8 value);
extern void InputCtrlKillPlayer(struct input_ctrl *self, void *arg);
extern void InputCtrlStateStart(struct input_ctrl *self);
extern void UpdateInputCtrl(struct input_ctrl *self);
extern void ApplyInputCtrlMotion(struct input_ctrl *self);
extern void SetInputCtrlModeAnim(struct input_ctrl *self, s32 mode, void *arg, s32 unused3, s32 unused4);
extern void InputCtrlStateDead(struct input_ctrl *self);
extern void sub_801793C(struct input_ctrl *self);
extern void sub_801796C(struct input_ctrl *self);
extern void RestartInputCtrl(struct input_ctrl *self);
extern void ResetInputCtrl(struct input_ctrl *self);
extern void InputCtrlHandleEvent(struct input_ctrl *self, s32 arg1, s32 arg2);
extern void AttachInputCtrl(struct input_ctrl *self, struct player *target);
extern void DestroyInputCtrl(struct input_ctrl *self, s32 flags);
extern struct input_ctrl *CreateInputCtrl(struct input_ctrl *self);
extern void SetInputCtrlMotionYPending(struct input_ctrl *self);
extern void SetInputCtrlMotionXPending(struct input_ctrl *self);
extern void CancelInputCtrlMotionY(struct input_ctrl *self);
extern void CancelInputCtrlMotionX(struct input_ctrl *self);
extern u8 IsInputCtrlMotionYPending(struct input_ctrl *self);

/* src/player/input_ctrl_queue.c */
extern u8 IsInputCtrlMotionXPending(void *self);
extern void QueueInputCtrlMotionYKeepSpeed(void *self, u8 val);
extern void QueueInputCtrlMotionXKeepSpeed(void *self, u8 val);
extern void QueueInputCtrlMotionY(void *self, u8 val);
extern void QueueInputCtrlMotionX(void *self, u8 val);
extern void BossCtrlHandleEvent(void *self, s32 arg1, s32 a, s32 b);
extern void DestroyBossCtrl(void *self, s32 flags);
extern void *CreateBossCtrl(void *self);
extern void *GetCtrlTarget(void *self);

/* src/player/kill_player.c */
extern void KillPlayer(void *self, s32 id);
extern void sub_8012238(void *self);
extern s32 UpdatePlayerFacing(void *self);

/* src/player/player_anim_room.c */
extern u8 PlayerHasRoomForAnim(struct box_part *self, s32 x);

/* src/player/player_collide.c */
extern u8 CollidePlayer(struct player *self);

/* src/player/player_event.c */
extern void CollidePlayerWithObjects(struct player *self);
extern void PlayerHandleEvent(struct player *self, s32 a, s32 code, s32 c);
extern void DrawPlayer(struct player *self);

/* src/player/player_flags.c */
extern void *GetPlayerCollisionQueue(struct player *self);
extern void ClearPlayerDead(struct player *self);
extern void SetPlayerDead(struct player *self);
extern u8 IsPlayerDead(struct player *self);
extern void StartPlayerRampX(struct player *self, s32 a, s32 b, s32 c);
extern void SetPlayerRampX(struct player *self, s32 a, s32 b, s32 c);
extern void sub_800B4F8(struct player *self);
extern void sub_800B508(struct player *self);
extern void sub_800B510(struct player *self);
extern u8 sub_800B51C(struct player *self);
extern u8 IsPlayerInvulnerable(struct player *self);
extern void ClearPlayerInvulnerability(struct player *self);
extern void SetPlayerInvulnerable(struct player *self, s32 arg1);
extern void SetPlayerControlMode(struct player *self, u8 arg1);
extern u8 GetPlayerControlMode(struct player *self);
extern s32 GetPlayerStandingOn(struct player *self);
extern void SetPlayerStandingOn(struct player *self, s32 arg1);
extern void SetPlayerBusy(struct player *self, u8 arg1);
extern u8 IsPlayerBusy(struct player *self);
extern void sub_800B584(struct player *self);
extern void sub_800B58C(struct player *self);
extern u8 sub_800B5A0(struct player *self);
extern void sub_800B5A8(struct player *self);
extern void sub_800B5B0(struct player *self);
extern u8 sub_800B5BC(struct player *self);
extern void sub_800B5C4(struct player *self);
extern void sub_800B5CC(struct player *self);
extern u8 sub_800B5D8(struct player *self);
extern void SetPlayerBumped(struct player *self, u8 arg1);
extern u8 IsPlayerBumped(struct player *self);
extern u8 GetPlayerPushRight(struct player *self);
extern void SetPlayerPushRight(struct player *self, u8 arg1);
extern u8 GetPlayerPushLeft(struct player *self);
extern void SetPlayerPushLeft(struct player *self, u8 arg1);
extern u8 IsPlayerHanging(struct player *self);
extern void SetPlayerHanging(struct player *self, u8 arg1);
extern u8 IsPlayerSlippery(struct player *self);
extern void SetPlayerSlippery(struct player *self, u8 arg1);
extern s32 sub_800B650(struct player *self, s32 idx);
extern void sub_800B678(struct player *self, s32 val);
extern void SetCtrlMode(void *self, s32 val);
extern void SetCtrlAnimSet(void *self, s32 val);
extern void SetCtrlTargetMotionY(void *unused, void *self, struct vec3 *vec);
extern void StartCtrlTargetMotionY(void *unused, void *self, struct vec3 *vec);

/* src/player/player_init.c */
extern struct player *InitPlayer(struct player *self, u16 arg1, u16 arg2, u16 arg3, u16 unused);

/* src/player/player_reset.c */
extern void ResetPlayer(struct player *self);
extern void ResetPlayerForRoom(struct player *self);

/* src/player/player_update.c */
extern s32 ApplyPlayerVelocity(struct player *self);
extern u8 HasPlayerRampYTarget(struct player *self);
extern void ClearPlayerSpeedY(struct player *self);
extern void StopPlayerFalling(struct player *self);
extern void UpdatePlayer(struct player *self);
extern u8 PlayerTouchesBox(struct player *self, struct aabb *box);
extern void DestroyPlayer(struct player *self, u32 arg1);

/* src/player/swim_ctrl.c */
extern void CheckPlayerCtrlTurn(struct player_ctrl *self);
extern void PlayerCtrlHandleEvent(struct player_ctrl *self, s32 unused, s32 msg, s32 arg);
extern void PlayerCtrlKillPlayer(struct player_ctrl *self, s32 anim);
extern void UpdatePlayerCtrl(struct player_ctrl *self);
extern void ApplyPlayerCtrlMotion(struct player_ctrl *self);
extern void PlayerCtrlStateIdle(struct player_ctrl *self);
extern void PlayerCtrlStateSwim(struct player_ctrl *self);
extern void PlayerCtrlStateStroke(struct player_ctrl *self);
extern void PlayerCtrlStateSpin(struct player_ctrl *self);
extern void PlayerCtrlStateTurn(struct player_ctrl *self);
extern void PlayerCtrlStateSwimStart(struct player_ctrl *self);
extern void PlayerCtrlStateStop(struct player_ctrl *self);
extern void PlayerCtrlStateDead(struct player_ctrl *self);
extern void AttachPlayerCtrl(struct player_ctrl *self, struct player *target);
extern void StartPlayerCtrlMotionYFromSet(struct player_ctrl *self, struct player *target, s32 idx);
extern void StartPlayerCtrlMotionXFromSet(struct player_ctrl *self, struct player *target, s32 idx);
extern void SetPlayerCtrlState(struct player_ctrl *self, s32 a, s32 mode, s32 timer, s32 timerMax);
extern void SetPlayerSwimDriftX(s32 a, s32 b, s32 c);
extern s32 sub_8017330(s32 v);
extern void ApplyPlayerCtrlTilt(struct player_ctrl *self);
extern void StartPlayerCtrlSwim(struct player_ctrl *self);
extern void DestroyPlayerCtrl(struct player_ctrl *self, s32 flags);
extern struct player_ctrl *InitPlayerCtrl(struct player_ctrl *self);
extern void sub_801750C(struct player_ctrl *self);
extern void SetPlayerCtrlMotionYPending(struct player_ctrl *self);
extern void SetPlayerCtrlMotionXPending(struct player_ctrl *self);

/* src/player/swim_ctrl_drift.c */
extern void SetPlayerSwimDriftY(s32 arg0, s32 arg1, s32 arg2);

/* src/player/swim_ctrl_stroke.c */
extern void StartPlayerCtrlStroke(struct player_ctrl *self);
extern void StartPlayerCtrlSpin(struct player_ctrl *self);
extern void ApplyPlayerCtrlSwimDrift(struct player_ctrl *self);

#endif /* GUARD_PLAYER_H */
