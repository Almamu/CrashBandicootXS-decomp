#include "input_ctrl.hpp"
#include "boss_ctrl.hpp"

/* GitHub issue #22, ROM 0x08017A44-0x08017AAC. Two classes' methods
 * share this file (include/input_ctrl.hpp, include/boss_ctrl.hpp):
 *
 * - InputCtrl's IsMotionXPending .. QueueMotionX (0x08017A44-0x08017A6C)
 *   finish the input controller's accessor run that ends input_ctrl.cpp
 *   (SetInputCtrlMotionYPending .. IsInputCtrlMotionYPending,
 *   0x08017A20-0x08017A40): its motion queue. Nothing calls them.
 * - BossCtrl, the controller base of the bosses: Mega-Mix
 *   (gMegaMixCtrlVtable), Tiny, the Neo Cortex fight's controller and
 *   Dingodile with his shield and rocket/stalactite. */

u8 InputCtrl::IsMotionXPending()
{
    return motionXPending;
}

/* Queues Y motion entry `entry`, applied with the speed kept. */
void InputCtrl::QueueMotionYKeepSpeed(u8 entry)
{
    motionYKeepSpeed = 1;
    motionYPending = 1;
    motionY = entry;
}

/* Queues X motion entry `entry`, applied with the speed kept. */
void InputCtrl::QueueMotionXKeepSpeed(u8 entry)
{
    motionXKeepSpeed = 1;
    motionXPending = 1;
    motionX = entry;
}

/* Queues Y motion entry `entry`. */
void InputCtrl::QueueMotionY(u8 entry)
{
    motionYPending = 1;
    motionY = entry;
}

/* Queues X motion entry `entry`. */
void InputCtrl::QueueMotionX(u8 entry)
{
    motionXPending = 1;
    motionX = entry;
}

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
