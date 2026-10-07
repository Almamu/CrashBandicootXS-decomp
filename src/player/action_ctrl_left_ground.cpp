#include "action_ctrl.hpp"
#include "sprite_obj.hpp"

/* ActionCtrl's CheckLeftGround (include/action_ctrl.hpp; #664,
 * docs/cplusplus.md). Built by old_agbcp (the Makefile's OLD_AGBCC_OBJS):
 * the C, built by agbcc, pinned 3 registers to old_agbcc's code. */

/* Whether the player has left the ground (`hitAxes` bit 3, the floor,
 * clear): then the fall (state 0x1A, animation 0x1B) after more than two
 * probe tries, or state 0x1C (just left the ground) before, with the
 * falling Y motion (entry 4) queued. */
u8 ActionCtrl::CheckLeftGround()
{
    if ((part->hitAxes & 8) == 0) {
        if (part->probeTries > 2) {
            SetMode(ACTION_STATE_AIRBORNE_FALL);
            SetTargetAnim(part, 0x1B);
        } else {
            SetMode(ACTION_STATE_LEFT_GROUND);
        }
        QueueNowY(4);
        return 1;
    }
    return 0;
}
