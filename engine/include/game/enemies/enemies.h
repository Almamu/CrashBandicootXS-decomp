#ifndef GUARD_ENEMIES_H
#define GUARD_ENEMIES_H

/* The enemies subsystem (src/game/enemies/): the enemy controller that steers
 * an enemy's sprite part (a MovingSprite, include/game/entities/sprite_obj.hpp), its
 * motion/attack updaters, the "knocked" controller HitEnemy launches, and
 * the periodic spawner.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers.md).
 *
 * The subsystem is C++ (#664): the classes are EnemyCtrl, KnockedEnemyCtrl
 * and PeriodicSpawner in include/game/enemies/enemy_ctrl.hpp, and the enemy spawners
 * (spawn_enemies.cpp) are C++ too. */

#include "core.h"
#include "engine/actor/actor.h"
#include "constants/entities.h"

struct entry_set;

/* The motion entry set ResetEnemyCtrl gives every controller
 * (`manager`, src/game/crates/data/crate_tables.cpp). */
extern const struct entry_set gEnemyCtrlMotionSet;

/* The part position UpdateEnemyCtrl's state 9 records the first time it
 * runs, and the flags that say each axis was recorded (IWRAM,
 * games/xs/sym_iwram.txt). */
extern s32 gHomingEnemyX;
extern s32 gHomingEnemyXSaved;
extern s32 gHomingEnemyY;
extern s32 gHomingEnemyYSaved;

/* src/game/enemies/enemy_ctrl.cpp: GetSfxVolumeAt (C linkage). The methods
 * have no C caller left and no C name here (cxx_symbols.txt has them). */
extern s32 GetSfxVolumeAt(s32 x, s32 y);

#endif /* GUARD_ENEMIES_H */
