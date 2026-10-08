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
 * The subsystem is C++ (#664): the classes are EnemyCtrl, KnockedEnemyCtrl
 * and PeriodicSpawner in include/enemy_ctrl.hpp. The prototypes below are
 * their methods' C names (cxx_symbols.txt), for the vtables; the enemy
 * spawners (spawn_enemies.cpp) are C++ too. The knocked controller is a plain
 * 0x10-byte controller (objects.h's `struct ctrl`), so its functions take
 * `void *`. */

#include "core.h"
#include "actor.h"
#include "part_ctrl.h"
#include "vtable.h"

/* A periodic trigger actor (CreatePeriodicSpawner builds one on top of
 * `struct actor`, in a 0x28-byte block): calls `callback` at its own
 * position once every `period` frames while near the camera
 * (UpdatePeriodicSpawner). SpawnSealSpawner (src/level/spawn_objects.cpp)
 * makes one with SpawnSeal. */
struct periodic_spawner {
    struct actor base;      // 0x00 - `base.table` is the method table
    void (*callback)(void); // 0x1C - called through _call_via_r4
    s32 period;             // 0x20
    s32 phase;              // 0x24
};

struct entry_set;

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

/* src/enemies/enemy_ctrl.cpp: GetSfxVolumeAt (C linkage). The methods
 * have no C caller left and no C name here (cxx_symbols.txt has them). */
extern s32 GetSfxVolumeAt(s32 x, s32 y);

#endif /* GUARD_ENEMIES_H */
