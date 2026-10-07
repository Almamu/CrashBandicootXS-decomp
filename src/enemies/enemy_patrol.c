#include "core.h"

/* GitHub issue #9/#10: one of the four `self+0x68`-dispatching siblings
 * the Phase 1/2 investigation (docs/matching/archive/issue-9-10-0x0800b8dc-graphics.md)
 * flagged as the next highest-value target in the 0x0800B8DC-0x0800D040
 * cluster - independently called from several of UpdateEnemyCtrl's own
 * `self+0x74` dispatch states (2, 13, 15, 18).
 *
 * A small 4-case dispatcher keyed off `self+0x68` (the same sub-state
 * byte `UpdateEnemyCtrl`'s own state machine reads/writes, and the field
 * `UpdateEnemyAttackCycle` also dispatches on): modes 0 and 4 share an identical
 * "mirror-aware position gate" (compares `owner`'s X position against
 * `self+0x10`/`self+0x14` bounds, direction picked by `owner+0x28` bit
 * 4, the established mirror-flag convention) before triggering
 * `SetEnemyAnimMode` with a different constant (1 vs 6) and always calling
 * `SetEnemyMotionX(self, 0)`. Modes 1 and 6 both toggle `owner+0x28` bit 4
 * (the "flag active + bitmap-set" idiom's own bit, an unconditional
 * flip via the mask-and-or idiom, not the position gate) when
 * `owner+0x38` is set, then trigger `SetEnemyAnimMode`/`SetEnemyMotionX` with
 * different constants; mode 1 additionally clamps `owner+0x30` against
 * a keyframe-record byte (`owner+0x20`-table[`owner+0x2d`]+0x16, the
 * `BreakCrateTouchedByPlayer`-style 28-byte-stride record convention) when
 * `self+0x6c == 0xf`. Any other mode is a silent no-op.
 *
 * Real C under old_agbcc (issue #10 NAKED retry,
 * docs/matching/archive/issue-10-naked-retry.md). Two details carry it:
 *  - The position gate's second clause re-tests the mirror bit (`cmp r3,
 *    #0; blt`) instead of being jump-threaded away. That needs the two
 *    tests to differ in RTL until after jump threading: the first reads
 *    the bit through the unsigned view, the second through the signed
 *    one (`struct ctrl_target`'s `mirror` union); combine turns both into
 *    the same sign test of one shared `lsl #27` afterwards.
 *  - The toggle reads the bit into a local first (`m = bit; bit = !m;`).
 */
#include "part_ctrl.h"
#include "enemies.h"

void UpdateEnemyPatrol(struct part_ctrl *self)
{
    struct ctrl_target *target;
    s32 t;
    s32 steps;

    switch (self->mode) {
    case 0:
        if ((self->target->mirror.u.x && self->target->x < self->rangeX[0]) ||
            (!self->target->mirror.s.x && self->target->x > self->rangeX[1])) {
            SetEnemyAnimMode(self, 1);
            SetEnemyMotionX(self, 0);
        }
        break;
    case 1:
        if (self->target->animDone) {
            u32 m = self->target->mirror.u.x;
            self->target->mirror.u.x = !m;
            SetEnemyAnimMode(self, 0);
            SetEnemyMotionX(self, 1);
            if (self->kind == ENEMY_KIND_PENGUIN) {
                target = self->target;
                t = 8;
                steps = (*target->keyframes)[target->frame].steps;
                if (t >= steps)
                    t = steps - 1;
                target->tick = t;
            }
        }
        break;
    case 4:
        if ((self->target->mirror.u.x && self->target->x < self->rangeX[0]) ||
            (!self->target->mirror.s.x && self->target->x > self->rangeX[1])) {
            SetEnemyAnimMode(self, 6);
            SetEnemyMotionX(self, 0);
        }
        break;
    case 6:
        if (self->target->animDone) {
            u32 m = self->target->mirror.u.x;
            self->target->mirror.u.x = !m;
            SetEnemyAnimMode(self, 4);
            SetEnemyMotionX(self, 1);
        }
        break;
    }
}
