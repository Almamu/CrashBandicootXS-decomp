#ifndef GUARD_ENEMY_CTRL_HPP
#define GUARD_ENEMY_CTRL_HPP

/* The enemy controllers as C++ (#664, docs/cplusplus.md): the enemy
 * controller, the knocked controller HitEnemy hands a knocked-away enemy
 * to, and the periodic spawner (src/enemies/).
 *
 * `#pragma interface`: no vtable is emitted for these (see ctrl.hpp);
 * cxx_symbols.txt maps their mangled names onto the C names. */
#pragma interface

#include "ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "part_ctrl.h"
#include "enemies.h"
}

/* The enemy controller (gEnemyCtrlVtable; struct part_ctrl in
 * part_ctrl.h is its C view). It steers an enemy's sprite part
 * (`target`): `state` picks what Update does each frame (patrol, attack
 * cycle, oscillate, home in, ...; SetState), and `mode` is the part's
 * animation mode (SetAnimMode), which the per-state updaters step
 * through. The enemy spawners (src/level/spawn_enemies.cpp) make them
 * with `new EnemyCtrl`, a 0x8C-byte block. It doesn't use
 * Ctrl's `owner` and `state`: Attach sets `target`, and its own `state`
 * at 0x74 hides Ctrl's. */
class EnemyCtrl : public Ctrl
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
    s32 speed; // 0x58 - homing
    s32 accel; // 0x5C
    s32 baseX; // 0x60 - oscillator base / last target x
    s32 baseY; // 0x64 - oscillator base / last target y
    s32 mode;  // 0x68 - see SetAnimMode
    s32 kind;  // 0x6C - the enemy kind (its sprite bank)
    /* 0x70: the steered part, through part_ctrl.h's field view
     * (`target`) or as the sprite object the Ctrl methods and its own
     * virtual methods take (`sprite`). */
    union {
        struct ctrl_target *target;
        MovingSprite *sprite;
    };
    s32 state;                 // 0x74 - Update's state (SetState)
    s32 modeB;                 // 0x78 - see SetMotionX
    s32 modeA;                 // 0x7C - see SetMotionY
    s32 counter;               // 0x80
    const s32 *anims;          // 0x84 - per-mode argument of SetTargetAnim: anim mode ->
                               //        bank anim (gEnemyDefaultAnimMap..., SetModeTable)
    struct ctrl_target *popup; // 0x88 - floating popup spawned in state 18

    EnemyCtrl();                                                        // CreateEnemyCtrl
    virtual void Update(MovingSprite *part);                            // UpdateEnemyCtrl
    virtual void HandleEvent(MovingSprite *sender, s32 event, s32 arg); // HitEnemy
    virtual void Attach(MovingSprite *part);                            // AttachEnemyCtrl
    virtual ~EnemyCtrl();                                               // DestroyEnemyCtrl

    /* src/enemies/enemy_ctrl.cpp */
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

    /* src/enemies/enemy_attack.cpp */
    void UpdateAttackCycle();
    void UpdateTriggerBox();
    void SetState(s32 state);
    void SetRangeXSpeed(s32 radius, s32 speed, s32 accel);
    void SetRangeYSpeed(s32 radius, s32 speed, s32 accel);
    void SetRangeX(s32 radius);

    /* src/enemies/enemy_motion.cpp */
    void UpdateHomingX();
    void UpdateHomingY();
    void UpdateHop();
    void UpdateFlipCycle();

    /* src/enemies/enemy_patrol.cpp, src/enemies/enemy_shooter.cpp */
    void UpdatePatrol();
    void UpdateShooter();
};

COMPILE_TIME_ASSERT(enemy_ctrl_hpp, sizeof(EnemyCtrl) == sizeof(struct part_ctrl));

/* The knocked enemy's controller (gKnockedEnemyCtrlVtable): HitEnemy
 * hands a spun or slid enemy's part to one, and it marks the part gone
 * once it has left the screen. */
class KnockedEnemyCtrl : public Ctrl
{
public:
    KnockedEnemyCtrl();                      // CreateKnockedEnemyCtrl
    virtual void Update(MovingSprite *part); // UpdateKnockedEnemyCtrl
    virtual ~KnockedEnemyCtrl();             // DestroyKnockedEnemyCtrl
    void Reset();                            // ResetKnockedEnemyCtrl
};

COMPILE_TIME_ASSERT(enemy_ctrl_hpp, sizeof(KnockedEnemyCtrl) == sizeof(struct ctrl));

/* A periodic trigger entity (gPeriodicSpawnerVtable; struct
 * periodic_spawner in enemies.h): calls `callback` at its own position
 * once every `period` frames while near the camera. SpawnSealSpawner
 * (src/level/spawn_objects.cpp) makes one with SpawnSeal. */
class PeriodicSpawner : public Entity
{
public:
    void (*callback)(u32 arg, u16 x, u16 y, u16 arg3); // 0x1C
    s32 period;                                        // 0x20
    s32 phase;                                         // 0x24

    PeriodicSpawner();          // CreatePeriodicSpawner
    virtual void Update();      // UpdatePeriodicSpawner
    virtual ~PeriodicSpawner(); // DestroyPeriodicSpawner
    void SetPeriod(s32 period, s32 phase);
    void SetCallback(void (*callback)(u32 arg, u16 x, u16 y, u16 arg3));
};

COMPILE_TIME_ASSERT(enemy_ctrl_hpp, sizeof(PeriodicSpawner) == sizeof(struct periodic_spawner));

/* src/enemies/enemy_ctrl.cpp: EntitySpawner::LaunchEffectPart from `src`
 * (spawners.hpp), the new part harmful. C linkage, C++ callers only. */
extern "C" MovingSprite *LaunchHarmfulEffectPart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                                 MovingSprite *src);

#endif /* !GUARD_ENEMY_CTRL_HPP */
