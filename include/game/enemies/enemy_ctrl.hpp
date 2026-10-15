#ifndef GUARD_ENEMY_CTRL_HPP
#define GUARD_ENEMY_CTRL_HPP

/* The enemy controllers as C++ (#664, docs/cplusplus.md): the enemy
 * controller, the knocked controller HitEnemy hands a knocked-away enemy
 * to, and the periodic spawner (src/game/enemies/).
 *
 * No `#pragma interface`: g++ emits their vtables (EnemyCtrl's in
 * enemy_ctrl_update.cpp, the others in periodic_spawner.cpp and
 * knocked_enemy_ctrl.cpp; see ctrl.hpp);
 * cxx_symbols.txt maps their mangled names onto the C names.
 *
 * The three classes are in the namespace Game::Enemies (#1033,
 * docs/cplusplus.md "Namespaces"):
 * `Update__Q34Game7Enemies9EnemyCtrlPQ34Game8Entities12MovingSprite`,
 * `_vt.Q34Game7Enemies9EnemyCtrl` (in the section
 * `.gnu.linkonce.d._vt.Q34Game7Enemies9EnemyCtrl`, which ldscript.txt
 * names) & co. enemies.h's C declarations stay global. */

#include "game/entities/ctrl.hpp"
#include "game/entities/sprite_obj.hpp"

extern "C" {
#include "game/enemies/enemies.h"
}

/* The enemy controller (gEnemyCtrlVtable). It steers an enemy's sprite part
 * (`target`): `state` picks what Update does each frame (patrol, attack
 * cycle, oscillate, home in, ...; SetState), and `mode` is the part's
 * animation mode (SetAnimMode), which the per-state updaters step
 * through. The enemy spawners (src/game/enemies/spawn_enemies.cpp) make them
 * with `new EnemyCtrl`, a 0x8C-byte block. It doesn't use
 * Ctrl's `owner` and `state`: Attach sets `target`, and its own `state`
 * at 0x74 hides Ctrl's. */

/* `state` (#843, docs/flags/enemy_ctrl.md): what Update does each
 * frame, set once by the spawner (SetState). The names say which
 * updaters Update runs; the enemies that use each are in the doc. */
#define ENEMY_STATE_STILL 1             // nothing (the moray eel, the laser barrier)
#define ENEMY_STATE_PATROL 2            // UpdatePatrol
#define ENEMY_STATE_TRIGGER 3           // UpdateTriggerBox (the venus flytrap)
#define ENEMY_STATE_ATTACK_CYCLE 4      // UpdateAttackCycle
#define ENEMY_STATE_FALL 5              // falls, and is gone below the level (the seal)
#define ENEMY_STATE_BOB 6               // UpdateBob (the jellyfish)
#define ENEMY_STATE_FLIP_CYCLE 7        // UpdateFlipCycle (the squid)
#define ENEMY_STATE_HOP 8               // UpdateHop (the frog)
#define ENEMY_STATE_HOME_X_OSCILLATE 9  // UpdateHomingX, UpdateOscillateX
#define ENEMY_STATE_HOME_Y 10           // UpdateHomingY
#define ENEMY_STATE_HOME 11             // UpdateHomingX and Y; a sea mine blows up on contact
// Does nothing in Update or SetState, and no spawner sets it.
#define ENEMY_STATE_UNK12 12
#define ENEMY_STATE_PATROL_ATTACK 13    // UpdatePatrol, UpdateAttackCycle
#define ENEMY_STATE_ATTACK_OSCILLATE 14 // UpdateAttackCycle, UpdateOscillateY (the pufferfish)
#define ENEMY_STATE_PATROL_HOME_Y 15    // UpdatePatrol, UpdateHomingY (the shark)
#define ENEMY_STATE_ATTACK_SHOOT 16     // UpdateAttackCycle, UpdateShooter
#define ENEMY_STATE_SWOOP 17            // the vulture's swoop, UpdateTriggerBox
#define ENEMY_STATE_SAUCER 18           // patrol, attack cycle and popup (the saucer lab assistant)

/* `mode` (#843): the animation mode, an index into the `anims` table
 * (SetAnimMode). The updaters step through them: 0-2 in the normal pose,
 * 3-5 into, in and out of the attack, 6-7 the normal ones' attack-pose
 * versions. */
#define ENEMY_MODE_NORMAL 0        // idle or walking
#define ENEMY_MODE_TURN 1          // turning round (UpdatePatrol, UpdateHop, UpdateFlipCycle)
#define ENEMY_MODE_STRIKE 2        // UpdateTriggerBox's triggered move, UpdateShooter's shot
#define ENEMY_MODE_ATTACK_START 3  // UpdateAttackCycle: going into the attack
#define ENEMY_MODE_ATTACK 4        // UpdateAttackCycle: attacking (UpdatePatrol walks in it)
#define ENEMY_MODE_ATTACK_END 5    // UpdateAttackCycle: coming out of it
#define ENEMY_MODE_ATTACK_TURN 6   // turning in the attack (UpdateFlipCycle's vertical flip)
#define ENEMY_MODE_ATTACK_STRIKE 7 // UpdateShooter's shot from the attack pose

/* An `anims` entry for a mode the enemy has no animation for:
 * UpdateAttackCycle skips the start and end modes it is in. */
#define ENEMY_ANIM_NONE 8

namespace Game
{
namespace Enemies
{

class EnemyCtrl : public Game::Entities::Ctrl
{
public:
    s32 rangeX[2]; // 0x10 - homing bounds
    s32 rangeY[2]; // 0x18
    s32 boxL;      // 0x20 - hit/trigger box, relative to the target
                   //        (UpdateTriggerBox, SetTriggerBox)
    s32 boxT;      // 0x24
    s32 boxR;      // 0x28
    s32 boxB;      // 0x2C
    s32 idleTime;  // 0x30 - attack cycle (UpdateAttackCycle): frames in mode 0
                   //        before the attack (mode 3/4) starts
    // 0x34 - frames in the attack before it ends (mode 5); the cycle
    //        repeats every idleTime + attackTime frames of gRoomFrameCount
    s32 attackTime;
    s32 cycleOffset; // 0x38 - where in the cycle the enemy starts (SetState starts it
                     //        attacking when cycleOffset >= idleTime)
    s32 period;      // 0x3C - oscillator (SetOscillator, UpdateOscillateX)
    s32 phase;       // 0x40
    s32 amplitude;   // 0x44
    s32 shotPeriod;  // 0x48 - UpdateShooter fires every shotPeriod
    s32 shotPhase;   // 0x4C   frames, offset by shotPhase
    u8 unk_50[8];
    s32 speed;                            // 0x58 - homing
    s32 accel;                            // 0x5C
    s32 baseX;                            // 0x60 - oscillator base / last target x
    s32 baseY;                            // 0x64 - oscillator base / last target y
    s32 mode;                             // 0x68 - see SetAnimMode
    s32 kind;                             // 0x6C - the enemy kind (its sprite bank)
    Game::Entities::MovingSprite *target; // 0x70 - the steered part (Attach)
    s32 state;                            // 0x74 - Update's state (SetState)
    s32 modeB;                            // 0x78 - see SetMotionX
    s32 modeA;                            // 0x7C - see SetMotionY
    s32 counter;                          // 0x80
    const s32 *anims;                     // 0x84 - per-mode argument of SetTargetAnim: anim mode ->
    //        bank anim (gEnemyDefaultAnimMap..., SetModeTable)
    Game::Entities::MovingSprite *popup; // 0x88 - floating popup spawned in state 18

    EnemyCtrl();                                             // CreateEnemyCtrl
    virtual void Update(Game::Entities::MovingSprite *part); // UpdateEnemyCtrl
    virtual void HandleEvent(s32 from, s32 event, s32 arg);  // HitEnemy
    virtual void Attach(Game::Entities::MovingSprite *part); // AttachEnemyCtrl
    virtual ~EnemyCtrl();                                    // DestroyEnemyCtrl

    /* src/game/enemies/enemy_ctrl.cpp */
    void SetMotionY(s32 mode);
    void SetMotionX(s32 mode);
    void SetAnimMode(s32 mode);
    void UpdateOscillateX();
    void UpdateBob();
    void UpdateOscillateY();
    void Reset();
    void SetOscillator(s32 period, s32 phase, s32 amplitude);
    void SetShotPeriod(s32 period, s32 phase);
    void SetAttackTiming(s32 idleTime, s32 attackTime, s32 cycleOffset);
    void SetTriggerBox(s32 l, s32 t, s32 r, s32 b);
    void SetModeTable(const s32 *anims);
    void SetKind(s32 kind);

    /* src/game/enemies/enemy_ctrl.cpp */
    void UpdateAttackCycle();
    void UpdateTriggerBox();
    void SetState(s32 state);
    void SetRangeXSpeed(s32 radius, s32 speed, s32 accel);
    void SetRangeYSpeed(s32 radius, s32 speed, s32 accel);
    void SetRangeX(s32 radius);

    /* src/game/enemies/enemy_motion.cpp */
    void UpdateHomingX();
    void UpdateHomingY();
    void UpdateHop();
    void UpdateFlipCycle();

    /* src/game/enemies/enemy_patrol.cpp, src/game/enemies/enemy_shooter.cpp */
    void UpdatePatrol();
    void UpdateShooter();

    /* The enemy spawners (src/game/enemies/spawn_enemies.cpp, #915), under their
     * C names: each creates an enemy, a MovingSprite moved by a new
     * EnemyCtrl. gEntitySpawnFuncs' enemy slots, and SpawnSeal (the seal
     * spawner's callback, PeriodicSpawner::Spawn). */
    static void SpawnLizard(u32 id, u16 x, u16 y, u16 param);
    static void SpawnVulture(u32 id, u16 x, u16 y, u16 param);
    static void SpawnVenusFlytrap(u32 id, u16 x, u16 y, u16 param);
    static void SpawnPatrollingJungleEnemy(u32 id, u16 x, u16 y, u16 param);
    static void SpawnBlowgunTribesman(u32 id, u16 x, u16 y, u16 param);
    static void SpawnPenguin(u32 id, u16 x, u16 y, u16 param);
    static void SpawnSeal(u32 id, u16 x, u16 y, u16 param);
    static void SpawnPolarBear(u32 id, u16 x, u16 y, u16 param);
    static void SpawnPufferfish(u32 id, u16 x, u16 y, u16 param);
    static void SpawnShark(u32 id, u16 x, u16 y, u16 param);
    static void SpawnMorayEel(u32 id, u16 x, u16 y, u16 param);
    static void SpawnElectricEel(u32 id, u16 x, u16 y, u16 param);
    static void SpawnSquid(u32 id, u16 x, u16 y, u16 param);
    static void SpawnJellyfish(u32 id, u16 x, u16 y, u16 param);
    static void SpawnLaserBarrier(u32 id, u16 x, u16 y, u16 param);
    static void SpawnStationarySpaceEnemy(u32 id, u16 x, u16 y, u16 param);
    static void SpawnPatrollingSpaceEnemy(u32 id, u16 x, u16 y, u16 param);
    static void SpawnSaucerLabAssistant(u32 id, u16 x, u16 y, u16 param);
    static void SpawnPistonCrusher(u32 id, u16 x, u16 y, u16 param);
    static void SpawnFlamethrowerLabAssistant(u32 id, u16 x, u16 y, u16 param);
    static void SpawnHomingSewerEnemy(u32 id, u16 x, u16 y, u16 param);
    static void SpawnPatrollingSewerEnemy(u32 id, u16 x, u16 y, u16 param);
    static void SpawnRat(u32 id, u16 x, u16 y, u16 param);
    static void SpawnFrog(u32 id, u16 x, u16 y, u16 param);
    static void SpawnSeaMine(u32 id, u16 x, u16 y, u16 param);
    static void SpawnWoodenCrusher(u32 id, u16 x, u16 y, u16 param);
};

LAYOUT_ASSERT(enemy_ctrl_hpp, sizeof(EnemyCtrl) == 0x8C);

/* The knocked enemy's controller (gKnockedEnemyCtrlVtable,
 * src/game/enemies/knocked_enemy_ctrl.cpp): HitEnemy
 * hands a spun or slid enemy's part to one, and it marks the part gone
 * once it has left the screen. */
class KnockedEnemyCtrl : public Game::Entities::Ctrl
{
public:
    KnockedEnemyCtrl();                                      // CreateKnockedEnemyCtrl
    virtual void Update(Game::Entities::MovingSprite *part); // UpdateKnockedEnemyCtrl
    virtual ~KnockedEnemyCtrl();                             // DestroyKnockedEnemyCtrl
    void Reset();                                            // ResetKnockedEnemyCtrl
};

LAYOUT_ASSERT(enemy_ctrl_hpp, sizeof(KnockedEnemyCtrl) == 0x10);

/* A periodic trigger entity (gPeriodicSpawnerVtable, 0x28 bytes;
 * src/game/enemies/periodic_spawner.cpp): calls `callback` at its own position
 * once every `period` frames while near the camera. PeriodicSpawner::Spawn
 * (src/game/objects/spawn_objects.cpp) makes one with SpawnSeal. */
class PeriodicSpawner : public Game::Entities::Entity
{
public:
    void (*callback)(u32 id, u16 x, u16 y, u16 param); // 0x1C
    s32 period;                                        // 0x20
    s32 phase;                                         // 0x24

    PeriodicSpawner();          // CreatePeriodicSpawner
    virtual void Update();      // UpdatePeriodicSpawner
    virtual ~PeriodicSpawner(); // DestroyPeriodicSpawner
    void SetPeriod(s32 period, s32 phase);
    void SetCallback(void (*callback)(u32 id, u16 x, u16 y, u16 param));

    /* The seal spawner, entity type ENTITY_SEAL_SPAWNER's slot
     * (spawn_objects.cpp, #915): the only PeriodicSpawner the ROM builds. */
    static void Spawn(u32 id, u16 x, u16 y, u16 param); // SpawnSealSpawner
};

LAYOUT_ASSERT(enemy_ctrl_hpp, sizeof(PeriodicSpawner) == 0x28);

} // namespace Enemies
} // namespace Game

#endif /* !GUARD_ENEMY_CTRL_HPP */
