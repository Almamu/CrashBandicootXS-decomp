#include "core.h"

/* GitHub issue #9/#10: `UpdateEnemyHop`, another of the four
 * `self+0x68`-dispatching siblings (docs/matching/issue-9-10-0x0800b8dc-graphics.md)
 * - called from UpdateEnemyCtrl's own state 8.
 *
 * Unconditional prelude: if `owner->4` (Y position) is still less than
 * `self->0x64`, the function is a no-op (early return). Otherwise it
 * always fires `SetEnemyMotionX(self, 0)` + `SetEnemyMotionY(self, 0)` and
 * re-syncs `owner->4` from `self->0x64` before dispatching further -
 * reads as "once the tracked Y target is reached, latch it and re-fire
 * the anchor triggers once". Two further `self+0x68`-keyed sub-cases
 * follow, gated by whether `owner->0x38` is set:
 *  - `owner->0x38 == 0`: modes 0/1 trigger `SetEnemyAnimMode` with a
 *    constant (1) or toggle `owner->0x28` bit 4 (the same mask-and-or
 *    idiom `UpdateEnemyPatrol` uses) then trigger mode 0; anything else is a
 *    no-op.
 *  - `owner->0x38 != 0`, gated further by `owner->0x30 == 8` and
 *    `owner->0x34 == 0` (the same "blocking condition" pair the Phase 1
 *    doc's field table documents): modes 0/1 both trigger
 *    `SetEnemyMotionX`/`SetEnemyMotionY` with mode 3 then play SFX `0x14` -
 *    mode 0 keeps `owner->0x28`'s existing mirror bit (`SetEnemyMotionX(self,3)`),
 *    mode 1 forces it clear (`SetEnemyMotionX(self,0)`) before the same
 *    `SetEnemyMotionY(self,3)` + SFX tail.
 *
 * Real C since the issue #9-#11 NAKED retry (see below). */

#include "part_ctrl.h"

extern void PlaySfx(void *ctx, s32 sfxId, s32 volume);
extern void *gAudioContext;

/* Real C (issue #9-#11 NAKED retry): the only gap in the old draft was
 * the post-call `target->y = baseY` store - `baseY` pinned to r1 gives the
 * ROM's r2/r1 split, and every later access reuses the same `t`. */
void UpdateEnemyHop(struct part_ctrl *self)
{
    struct ctrl_target *t;

    if (self->target->y < self->baseY)
        return;
    SetEnemyMotionX(self, 0);
    SetEnemyMotionY(self, 0);
    t = self->target;
    {
        register s32 by asm("r1") = self->baseY;
        t->y = by;
    }
    if (t->animDone) {
        switch (self->mode) {
        case 0:
            SetEnemyAnimMode(self, 1);
            break;
        case 1:
            {
                u32 m = t->mirror.u.x;
                t->mirror.u.x = !m;
            }
            SetEnemyAnimMode(self, 0);
            break;
        }
    } else if (t->tick == 8 && t->timer == 0) {
        switch (self->mode) {
        case 0:
            SetEnemyMotionX(self, 3);
            SetEnemyMotionY(self, 3);
            PlaySfx(gAudioContext, 0x14, 0x100);
            break;
        case 1:
            SetEnemyMotionX(self, 0);
            SetEnemyMotionY(self, 3);
            PlaySfx(gAudioContext, 0x14, 0x100);
            break;
        }
    }
}
