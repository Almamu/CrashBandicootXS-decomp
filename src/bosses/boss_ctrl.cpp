#include "boss_ctrl.hpp"

/* GitHub issue #22, ROM 0x08017A70-0x08017AB0 (include/boss_ctrl.hpp):
 * BossCtrl, the controller base of the bosses: Mega-Mix
 * (gMegaMixCtrlVtable), Tiny, the Neo Cortex fight's controller and
 * Dingodile with his shield and rocket/stalactite. g++ emits
 * gBossCtrlVtable here. */

/* Keeps the event's msg and arg; the sender is unused. Nothing reads
 * them back. */
void BossCtrl::HandleEvent(MovingSprite *, s32 event, s32 arg)
{
    msg = event;
    this->arg = arg;
}

BossCtrl::~BossCtrl()
{
}

BossCtrl::BossCtrl()
{
    target = 0;
    msg = 0;
    arg = 0;
}

/* The controlled part. */
void *BossCtrl::GetTarget()
{
    return target;
}
