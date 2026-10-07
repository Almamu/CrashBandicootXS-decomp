#include "enemy_ctrl.hpp"

extern "C" {
#include "math_util.h"
}

/* GitHub issue #9/#10: EnemyCtrl's patrol (include/enemy_ctrl.hpp),
 * called from Update's states 2, 13, 15 and 18
 * (docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md).
 *
 * A small dispatcher keyed off `mode`: modes 0 and 4 (walking) turn
 * around at the patrol bounds (rangeX; which bound depends on the
 * target's X mirror bit), playing turn animation mode 1 or 6 and
 * stopping the X motion. Modes 1 and 6 (turning) flip the mirror bit
 * once the turn animation is done, then walk on (animation mode 0 or 4,
 * X motion 1); a penguin also clamps its keyframe tick to 8. Any other
 * mode is a no-op.
 *
 * old_agbcc's code (issue #10 NAKED retry,
 * docs/matching/archive/issue-10-naked-retry.md). Two details carry it:
 *  - The position gate's second clause re-tests the mirror bit (`cmp r3,
 *    #0; blt`) instead of being jump-threaded away. That needs the two
 *    tests to differ in RTL until after jump threading: both test bit 4
 *    as a sign, the first as the word's (`<< 27`), the second as the
 *    byte's (`<< 3`); combine turns both into the same sign test of one
 *    shared `lsl #27` afterwards. (The C did it with the signed and
 *    unsigned bitfield views of `mirror`; g++ tests a 1-bit field with
 *    an `and`, whichever its signedness.)
 *  - The toggle reads the bit into a local first (`m = bit; bit = !m;`).
 */
void EnemyCtrl::UpdatePatrol()
{
    struct ctrl_target *part;
    s32 t;
    s32 steps;

    switch (mode) {
    case 0:
        if (((s32)(sprite->mirror << 27) < 0 && target->x < rangeX[0]) ||
            ((s8)(sprite->mirror << 3) >= 0 && target->x > rangeX[1])) {
            SetAnimMode(1);
            SetMotionX(0);
        }
        break;
    case 1:
        if (target->animDone) {
            u32 m = target->mirror.x;
            target->mirror.x = !m;
            SetAnimMode(0);
            SetMotionX(1);
            if (kind == ENEMY_KIND_PENGUIN) {
                part = target;
                t = 8;
                steps = (*part->keyframes)[part->frame].steps;
                CLAMP_INDEX(t, steps);
                part->tick = t;
            }
        }
        break;
    case 4:
        if (((s32)(sprite->mirror << 27) < 0 && target->x < rangeX[0]) ||
            ((s8)(sprite->mirror << 3) >= 0 && target->x > rangeX[1])) {
            SetAnimMode(6);
            SetMotionX(0);
        }
        break;
    case 6:
        if (target->animDone) {
            u32 m = target->mirror.x;
            target->mirror.x = !m;
            SetAnimMode(4);
            SetMotionX(1);
        }
        break;
    }
}
