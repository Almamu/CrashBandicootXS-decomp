#ifndef GUARD_ENEMIES_H
#define GUARD_ENEMIES_H

/* The enemies subsystem (src/enemies/): the enemy controller (`struct
 * part_ctrl`, include/part_ctrl.h) that steers an enemy's sprite part, its
 * motion/attack updaters, the "knocked" controller HitEnemy launches, and
 * the periodic spawner.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md).
 *
 * The knocked controller is a plain 0x10-byte controller (InitCtrl,
 * src/objects/ctrl.c), so its functions take `void *` until the objects
 * subsystem's header gives that class a type. */

#include "core.h"
#include "actor.h"
#include "part_ctrl.h"
#include "vtable.h"

/* A periodic trigger actor (CreatePeriodicSpawner builds one on top of
 * `struct actor`, in a 0x28-byte block): calls `callback` at its own
 * position once every `period` frames while near the camera
 * (UpdatePeriodicSpawner). SpawnSealSpawner (src/level/spawn_objects.c)
 * makes one with SpawnSeal. */
struct periodic_spawner {
    struct actor base;      // 0x00 - `base.table` is the method table
    void (*callback)(void); // 0x1C - called through _call_via_r4
    s32 period;             // 0x20
    s32 phase;              // 0x24
};

struct entry_set;

/* The method tables (src/data/entity_vtables_7e3bec.c). */
extern const struct vtable_slot gEnemyCtrlVtable[13];
extern const struct vtable_slot gPeriodicSpawnerVtable[11];
extern const struct vtable_slot gKnockedEnemyCtrlVtable[13];

/* The motion entry set ResetEnemyCtrl gives every controller
 * (`manager`, src/data/object_tables_16bb6c.c). */
extern const struct entry_set gEnemyCtrlMotionSet;

/* The part position UpdateEnemyCtrl's state 9 records the first time it
 * runs, and the flags that say each axis was recorded (IWRAM,
 * sym_iwram.txt). */
extern s32 gHomingEnemyX;
extern s32 gHomingEnemyXSaved;
extern s32 gHomingEnemyY;
extern s32 gHomingEnemyYSaved;

/* src/enemies/enemy_ctrl.c */
extern void SetEnemyMotionY(struct part_ctrl *self, s32 mode);
extern void SetEnemyMotionX(struct part_ctrl *self, s32 mode);
extern void SetEnemyAnimMode(struct part_ctrl *self, s32 mode);
extern void UpdateEnemyOscillateX(struct part_ctrl *self);
extern void UpdateEnemyBob(struct part_ctrl *self);
extern void UpdateEnemyOscillateY(struct part_ctrl *self);
extern void *LaunchHarmfulEffectPart(s32 a, s32 b, s32 c, s32 d, s32 e, void *f);
extern void AttachEnemyCtrl(struct part_ctrl *self, struct ctrl_target *target);
extern s32 GetSfxVolumeAt(s32 x, s32 y);
extern void ResetEnemyCtrl(struct part_ctrl *self);
extern void DestroyEnemyCtrl(struct part_ctrl *self, s32 flags);
extern struct part_ctrl *CreateEnemyCtrl(struct part_ctrl *self);
extern void SetEnemyOscillator(struct part_ctrl *self, s32 period, s32 phase, s32 amplitude);
extern void SetEnemyShotPeriod(struct part_ctrl *self, s32 period, s32 phase);
extern void SetEnemyAttackTiming(struct part_ctrl *self, s32 idleTime, s32 attackTime,
                                 s32 cycleOffset);
extern void SetEnemyTriggerBox(struct part_ctrl *self, s32 l, s32 t, s32 r, s32 b);
extern void SetEnemyModeTable(struct part_ctrl *self, const s32 *anims);
extern void SetEnemyKind(struct part_ctrl *self, s32 kind);
extern void UpdatePeriodicSpawner(struct periodic_spawner *self);
extern void DestroyPeriodicSpawner(struct periodic_spawner *self, s32 flags);
extern struct periodic_spawner *CreatePeriodicSpawner(struct periodic_spawner *self);
extern void SetPeriodicSpawnerPeriod(struct periodic_spawner *self, s32 period, s32 phase);
extern void SetPeriodicSpawnerCallback(struct periodic_spawner *self, void (*callback)(void));
extern void UpdateKnockedEnemyCtrl(void *self, struct actor *other);
extern void ResetKnockedEnemyCtrl(void *self);
extern void DestroyKnockedEnemyCtrl(void *self, s32 flags);
extern void *CreateKnockedEnemyCtrl(void *self);

/* src/enemies/enemy_attack.c */
extern void UpdateEnemyAttackCycle(struct part_ctrl *self);
extern void UpdateEnemyTriggerBox(struct part_ctrl *self);
extern void SetEnemyState(struct part_ctrl *self, s32 state);
extern void SetEnemyRangeXSpeed(struct part_ctrl *self, s32 radius, s32 p2, s32 p3);
extern void SetEnemyRangeYSpeed(struct part_ctrl *self, s32 radius, s32 p2, s32 p3);
extern void SetEnemyRangeX(struct part_ctrl *self, s32 radius);

/* src/enemies/enemy_ctrl_update.c */
extern void UpdateEnemyCtrl(struct part_ctrl *self);
extern void HitEnemy(struct part_ctrl *self, s32 unused, s32 state);

/* src/enemies/enemy_motion.c */
extern void UpdateEnemyHomingX(struct part_ctrl *self);
extern void UpdateEnemyHomingY(struct part_ctrl *self);
extern void UpdateEnemyHop(struct part_ctrl *self);
extern void UpdateEnemyFlipCycle(struct part_ctrl *self);

/* src/enemies/enemy_patrol.c */
extern void UpdateEnemyPatrol(struct part_ctrl *self);

/* src/enemies/enemy_shooter.c */
extern void UpdateEnemyShooter(struct part_ctrl *self);

#endif /* GUARD_ENEMIES_H */
