#include "enemy_ctrl.hpp"
#include "spawners.hpp"
#include "player.hpp"

extern "C" {
#include "match.h"
#include <libgcc.h>
#include "level.h"
#include "globals.h"
#include "math_util.h"
#include "player.h"
}

/* KnockedEnemyCtrl, the knocked enemy's controller (include/enemy_ctrl.hpp),
 * ROM 0x0800CB64-0x0800CBF4. g++ emits gKnockedEnemyCtrlVtable here. An
 * old_agbcp object (OLD_AGBCC_OBJS). */

/* Marks the knocked enemy's part gone once it has left the screen (the
 * same test as EffectCtrl::Update's first). */
void KnockedEnemyCtrl::Update(MovingSprite *part)
{
    if (!part->IsOnScreen())
        part->MarkGone();
}

/* Empty: the constructor calls it where EnemyCtrl's calls Reset. */
void KnockedEnemyCtrl::Reset()
{
}

/* g++ sets the vtable pointer back to gKnockedEnemyCtrlVtable, then
 * calls ~Ctrl (DestroyCtrl). */
KnockedEnemyCtrl::~KnockedEnemyCtrl()
{
}

/* Ctrl() (InitCtrl), the vtable pointer, then Reset. HitEnemy `new`s
 * one. */
KnockedEnemyCtrl::KnockedEnemyCtrl()
{
    Reset();
}
