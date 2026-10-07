#include "action_ctrl.hpp"
#include "sprite_obj.hpp"

extern "C" {
#include "system.h"
#include "globals.h"
}

/* Three of ActionCtrl's state methods, the landings (include/
 * action_ctrl.hpp; #664, docs/cplusplus.md). */

/* Standing up from a crawl: once the animation is done, idle. */
void ActionCtrl::StateCrawlStandUp()
{
    Player *p = part;

    if (p->animDone != 0) {
        SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, 0);
        motionXKeepSpeed = 0;
        motionXPending = 1;
        motionX = 0;
        motionYKeepSpeed = 0;
        motionYPending = 1;
        motionY = 0;
    }
}

/* Landing from a body slam: once the animation is done, idle, or
 * crouching down (animation 3) with R held or the D-pad down. */
void ActionCtrl::StateBodySlamLand()
{
    Player *p = part;

    if (p->animDone != 0) {
        void *pad = gInput;
        u16 held = gKeys.all & R_BUTTON;
        u8 crouch = held != 0;

        switch (GetDpadDirection(pad)) {
        case 2:
        case 7:
        case 8:
            crouch = 1;
            break;
        }

        if (crouch == 0) {
            SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, crouch);
            motionXKeepSpeed = crouch;
            motionXPending = 1;
            motionX = crouch;
            motionYKeepSpeed = crouch;
            motionYPending = 1;
            motionY = crouch;
        } else {
            SetMode(ACTION_STATE_CROUCH_DOWN);
            SetTargetAnim(part, 3);
            QueueNowX(0);
        }
    }
}

/* Landing: crouching down (animation 3) with R held; otherwise, once the
 * animation is done, idle. Then the idle state's method. */
void ActionCtrl::StateLand()
{
    u16 held;

    frame = 0;
    if ((held = gKeys.all & R_BUTTON) != 0) {
        SetMode(ACTION_STATE_CROUCH_DOWN);
        SetTargetAnim(part, 3);
        frames = 0;
        return;
    }
    if (part->animDone) {
        SetModeAnim(ACTION_STATE_IDLE, 0x12, 0, held);
        motionXKeepSpeed = held;
        motionXPending = 1;
        motionX = held;
        motionYKeepSpeed = held;
        motionYPending = 1;
        motionY = held;
    }
    StateIdle();
}
