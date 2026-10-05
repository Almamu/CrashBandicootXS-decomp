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
 * Most of the player's own functions still take `void *`: the player
 * object has no shared struct yet (gPlayer has 16 local views), so their
 * real type waits for `globals.h` (docs/headers_plan.md, batch 10).
 * ResetActionCtrl (src/pickups/wumpa.c) is here, with the rest of the
 * action controller. */

#include "core.h"
#include "actor_self.h"
#include "vtable.h"
#include "objects.h"

struct a884_part;
struct ab9c_obj;
struct ac2c_self;
struct act;
struct box_part;
struct ctrl_target;
struct input_ctrl;
struct orbit_self;
struct pctrl_motion_queue;
struct pctrl_target;
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
extern void AttachInputCtrl(struct input_ctrl *self, struct ctrl_target *target);
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
extern u8 CollidePlayer(struct a884_part *self);

/* src/player/player_event.c */
extern void CollidePlayerWithObjects(struct ab9c_obj *self);
extern void PlayerHandleEvent(struct ac2c_self *self, s32 a, s32 code, s32 c);
extern void DrawPlayer(struct orbit_self *self);

/* src/player/player_flags.c */
extern void *GetPlayerCollisionQueue(void *self);
extern void ClearPlayerDead(void *self);
extern void SetPlayerDead(void *self);
extern u8 IsPlayerDead(void *self);
extern void StartPlayerRampX(void *self, s32 a, s32 b, s32 c);
extern void SetPlayerRampX(void *self, s32 a, s32 b, s32 c);
extern void sub_800B4F8(void *self);
extern void sub_800B508(void *self);
extern void sub_800B510(void *self);
extern u8 sub_800B51C(void *self);
extern u8 IsPlayerInvulnerable(void *self);
extern void ClearPlayerInvulnerability(void *self);
extern void SetPlayerInvulnerable(void *self, s32 arg1);
extern void SetPlayerControlMode(void *self, u8 arg1);
extern u8 GetPlayerControlMode(void *self);
extern s32 GetPlayerStandingOn(void *self);
extern void SetPlayerStandingOn(void *self, s32 arg1);
extern void SetPlayerBusy(void *self, u8 arg1);
extern u8 IsPlayerBusy(void *self);
extern void sub_800B584(void *self);
extern void sub_800B58C(void *self);
extern u8 sub_800B5A0(void *self);
extern void sub_800B5A8(void *self);
extern void sub_800B5B0(void *self);
extern u8 sub_800B5BC(void *self);
extern void sub_800B5C4(void *self);
extern void sub_800B5CC(void *self);
extern u8 sub_800B5D8(void *self);
extern void SetPlayerBumped(void *self, u8 arg1);
extern u8 IsPlayerBumped(void *self);
extern u8 GetPlayerPushRight(void *self);
extern void SetPlayerPushRight(void *self, u8 arg1);
extern u8 GetPlayerPushLeft(void *self);
extern void SetPlayerPushLeft(void *self, u8 arg1);
extern u8 IsPlayerHanging(void *self);
extern void SetPlayerHanging(void *self, u8 arg1);
extern u8 IsPlayerSlippery(void *self);
extern void SetPlayerSlippery(void *self, u8 arg1);
extern s32 sub_800B650(void *self, s32 idx);
extern void sub_800B678(void *self, s32 val);
extern void SetCtrlMode(void *self, s32 val);
extern void SetCtrlAnimSet(void *self, s32 val);
extern void SetCtrlTargetMotionY(void *unused, void *self, struct vec3 *vec);
extern void StartCtrlTargetMotionY(void *unused, void *self, struct vec3 *vec);

/* src/player/player_init.c */
extern void *InitPlayer(void *self, u16 arg1, u16 arg2, u16 arg3, u16 unused);

/* src/player/player_reset.c */
extern void ResetPlayer(void *self);
extern void ResetPlayerForRoom(void *self);

/* src/player/player_update.c */
extern s32 ApplyPlayerVelocity(void *self);
extern u8 HasPlayerRampYTarget(void *self);
extern void ClearPlayerSpeedY(void *self);
extern void StopPlayerFalling(void *self);
extern void UpdatePlayer(void *self);
extern u8 PlayerTouchesBox(void *self, void *buf);
extern void DestroyPlayer(void *self, u32 arg1);

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
extern void AttachPlayerCtrl(struct player_ctrl *self, struct pctrl_target *target);
extern void StartPlayerCtrlMotionYFromSet(struct player_ctrl *self, struct pctrl_target *target, s32 idx);
extern void StartPlayerCtrlMotionXFromSet(struct player_ctrl *self, struct pctrl_target *target, s32 idx);
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
