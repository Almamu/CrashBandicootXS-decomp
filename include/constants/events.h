#ifndef GUARD_CONSTANTS_EVENTS_H
#define GUARD_CONSTANTS_EVENTS_H

/*
 * Event IDs: the third argument of an object's event method (vtable
 * +0x68, `handleEvent`: this, sender, event, arg). PlayerHandleEvent is the
 * player's; it handles some itself and forwards the rest to its
 * controller's event slot (NOTIFY: ActionCtrlHandleEvent on foot,
 * SwimCtrlHandleEvent swimming, InputCtrlHandleEvent). An enemy's is
 * HitEnemy.
 *
 * A touched object sends its own `kind`: CollidePartWithPlayer,
 * CheckSpritePickup and the bosses call the player's slot with the hazard's
 * or pickup's kind, and the player's kind (1, or one of the attacks while
 * it attacks: UpdateActionCtrl, swim_ctrl.cpp) goes to the enemy it hits. So
 * these values are also the kinds of those objects.
 *
 * Hits 1-10: PlayerHandleEvent takes a mask away (then sends
 * EVENT_MASK_HIT) or, without one, kills the player; the controller picks
 * the death animation (KillPlayer: ActionCtrlHandleEvent, SwimCtrlKillPlayer).
 * Hits 5 and 7 keep their numbers: 5 comes only from the pufferfish
 * (SpawnPufferfish) and 7 only from the frog (SpawnFrog), and neither
 * says what kind of hit it is (the controllers only pick a death
 * animation for them).
 */

// No event: a harmless object's kind (the seaweed and flames of
// spawn_objects.cpp, Cortex's gems until a shot hits them and his spent
// shots, Dingodile in his first state). No event method has a case for it
#define EVENT_NONE 0

// The plain hit: the player's default kind (UpdateActionCtrl), sent by
// deadly terrain (CollidePlayer, terrain kind 1), crates
// (QueueCratePlayerCollision) and the bosses (Tiny, Mega Mix, Dingodile);
// to an enemy, being jumped on (CollidePartWithPlayer)
#define EVENT_HIT 1
// The flamethrower lab assistant's flame (UpdateEnemyAttackCycle, enemy kind 0x17)
#define EVENT_HIT_FIRE 2
// The electric eel's part (SpawnElectricEel) and the popup an enemy
// controller launches (UpdateEnemyCtrl, state 18)
#define EVENT_HIT_ELECTRIC 3
// An exploding crate (ExplodeCrate, SFX_EXPLOSION), the sea mine
// (SpawnSeaMine) and Dingodile's projectile
#define EVENT_HIT_EXPLOSION 4
// The shark, moray eel, polar bear and venus flytrap (spawn_enemies.cpp) and
// Dingodile's shark (SpawnDingodileShark)
#define EVENT_HIT_BITE 6
// The blowgun tribesman's dart (EnemyCtrl::UpdateShooter, its state 16)
#define EVENT_HIT_DART 8
// Neo Cortex's shot (UpdateCortexShot)
#define EVENT_HIT_CORTEX_SHOT 9
// A crusher: the wooden crusher (SpawnWoodenCrusher), and a crate landing
// on the player (QueueCratePlayerCollision drops the masks first)
#define EVENT_HIT_CRUSH 10

// A hit took a mask away (PlayerHandleEvent's hits): the controller starts
// the mask-hit jump (StartActionCtrlMaskHitJump)
#define EVENT_MASK_HIT 11
// The player ran into something solid; `arg` is the side's hit-axis bits
// (1/2 X, 4/8 Y), which the controller applies
#define EVENT_BUMP 12
// Jumped on an enemy or a crate: the controller starts a bounce
#define EVENT_BOUNCE 13
// The higher bounce, off an arrow crate (SFX_ARROW_CRATE_BOUNCE)
#define EVENT_BOUNCE_HIGH 14
// Standing on the bonus-round platform (CreatePlatform kind 5):
// RequestBonusRound, the warp-out
#define EVENT_WARP_BONUS_ROUND 15
// Standing on a gem platform (CreatePlatform kinds 3, 9-12): RequestGemPath, the warp-out
#define EVENT_WARP_GEM_PATH 16
// Standing on the exit pad (CreatePlatform kind 4, SpawnRoomExit): freezes a time trial's
// clock, the warp-out
#define EVENT_WARP_EXIT 17
// The room exit's zone (SpawnRoomExit, kind 0x12): RequestRoomExit
#define EVENT_ROOM_EXIT 18
// The player's attacks, its kind while it does them (UpdateActionCtrl):
// spin (ground, air, tornado, hanging), slide, body slam and super body
// slam (also DoSuperBodySlamShockwave's); HitEnemy knocks the enemy away
// (spin, slide) or squashes it
#define EVENT_ATTACK_SPIN 19
#define EVENT_ATTACK_SLIDE 20
#define EVENT_ATTACK_BODY_SLAM 21
#define EVENT_ATTACK_SUPER_BODY_SLAM 22
// Hang terrain (code 6) above the player / gone (CollidePlayer)
#define EVENT_HANG_GRAB 23
#define EVENT_HANG_RELEASE 24
// A launch pad (CheckLaunchPadContact): the tornado-spin launch
#define EVENT_LAUNCH_PAD 25
// An Aku Aku mask (OpenAkuAkuCrate, OpenMysteryCrate, the start marker): RaiseMaskLevel
#define EVENT_MASK_GAIN 26
// The pickups (their kinds, spawn_gems.cpp, spawn_pickups.cpp), each setting its
// flag in PlayerHandleEvent
#define EVENT_CRYSTAL 27
#define EVENT_STOPWATCH 28 // StartTimeTrial
#define EVENT_CRATE_GEM 29
#define EVENT_GEM_PATH_GEM 30
#define EVENT_RED_GEM 31
#define EVENT_GREEN_GEM 32
#define EVENT_BLUE_GEM 33
#define EVENT_YELLOW_GEM 34
// The four powers (spawn_pickups.cpp): each ends the room (RequestRoomExit)
#define EVENT_POWER_DOUBLE_JUMP 35
#define EVENT_POWER_TORNADO_SPIN 36
#define EVENT_POWER_BODY_SLAM 37
#define EVENT_POWER_TURBO_RUN 38

#endif /* GUARD_CONSTANTS_EVENTS_H */
