#include "input_ctrl.hpp"

/* GitHub issue #22, ROM 0x08017A44-0x08017A70 (include/input_ctrl.hpp):
 * InputCtrl's IsMotionXPending .. QueueMotionX finish the input
 * controller's accessor run that ends input_ctrl.cpp
 * (SetInputCtrlMotionYPending .. IsInputCtrlMotionYPending,
 * 0x08017A20-0x08017A40): its motion queue. Nothing calls them. BossCtrl
 * follows in src/bosses/boss_ctrl.cpp. */

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
