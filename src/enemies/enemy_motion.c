#include "core.h"
#include "part_ctrl.h"
#include "enemies.h"
#include "audio.h"
#include "globals.h"
#include "player.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `UpdateEnemyHomingX`/
 * `UpdateEnemyHomingY`, the X-axis/Y-axis "homing velocity-target setter"
 * pair the Phase 1 doc's own priority list flagged as the cluster's
 * next likely-real-C win. Both take only `self` and write into
 * `self+0x70` ("owner"): given `owner`'s position on the relevant
 * axis relative to `gPlayer`'s own object (the player/
 * camera), and `self`'s own `0x10`/`0x14` (X) or `0x18`/`0x1c` (Y)
 * bounds, picks one of four `{vx, vy}` pairs and writes them into
 * `owner+0x48`/`0x4c`/`0x50` (X) or `owner+0x54`/`0x58`/`0x5c` (Y) -
 * the same "velocity-target triple" convention the Phase 1 doc's
 * field table already documents at those offsets. `self+0x58`/`0x5c`
 * hold the actual homing speed magnitude (X and Y share the same
 * pair - this is a diagonal-speed setting, not two independent
 * speeds), and the middle "near player" band always sets vy to
 * `self->0x5c` unconditionally regardless of axis, which is
 * consistent with velocity-target components for the *other* axis
 * carrying the diagonal-approach rate while this axis's own drift
 * stops.
 *
 * Real C (issue #10 NAKED retry, see docs/matching/issue-10-naked-retry.md).
 * The earlier "cross-jump divergence" note was a source-shape problem:
 * each of the five cases does its own `{a, b, a}` store triple through
 * a block-scoped `a`/`b` pair (SET_VEL below); the compiler's cross-jump
 * then merges four of them into the shared tail and leaves the `0, 0x10`
 * moves duplicated, exactly as in the ROM. The `-speed` case keeps its
 * own stores because its registers differ. Same bytes under both
 * compilers. */

#define SET_VEL(v, a_, b_) \
    {                      \
        s32 a = (a_);      \
        s32 b = (b_);      \
        (v)[0] = a;        \
        (v)[1] = b;        \
        (v)[2] = a;        \
    }

void UpdateEnemyHomingX(struct part_ctrl *self)
{
    struct ctrl_target *target = self->target;
    s32 x = target->x;
    s32 d = x - gPlayer->x;

    if (d > 20) {
        if (x < self->rangeX[0])
            SET_VEL(target->rampX, 0, 0x10)
        else
            SET_VEL(target->rampX, -self->speed, self->accel)
    } else if (d < -20) {
        if (x > self->rangeX[1])
            SET_VEL(target->rampX, 0, 0x10)
        else
            SET_VEL(target->rampX, self->speed, self->accel)
    } else {
        SET_VEL(target->rampX, 0, self->accel)
    }
}

/* Y-axis mirror of `UpdateEnemyHomingX` above: `target->y`, `rampY`, and the
 * `rangeY` bounds tested in the opposite order. */
void UpdateEnemyHomingY(struct part_ctrl *self)
{
    struct ctrl_target *target = self->target;
    s32 y = target->y;
    s32 d = y - gPlayer->y;

    if (d > 20) {
        if (y < self->rangeY[1])
            SET_VEL(target->rampY, 0, 0x10)
        else
            SET_VEL(target->rampY, -self->speed, self->accel)
    } else if (d < -20) {
        if (y > self->rangeY[0])
            SET_VEL(target->rampY, 0, 0x10)
        else
            SET_VEL(target->rampY, self->speed, self->accel)
    } else {
        SET_VEL(target->rampY, 0, self->accel)
    }
}
asm(".align 2, 0");

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

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `UpdateEnemyFlipCycle`,
 * `UpdateEnemyCtrl`'s state-7 callee. Early-outs unless `owner+0x38`
 * (self+0x70, the "owner" object) is set, then dispatches on
 * `self+0x68` (values 0, 1, 6 handled; anything else no-ops):
 *
 * - Case 0: if `self+0x80` bit 0 is set, toggles `owner+0x28` bit 4
 *   (the established mirror-flag convention, see
 *   `src/objects/ctrl.c`'s `(s32)(part[0x28] << 27) < 0`
 *   idiom) and triggers `SetEnemyAnimMode(self, 1)`; otherwise just
 *   `SetEnemyAnimMode(self, 6)`. Either way, tails into
 *   `SetEnemyMotionX(self, 0)` + `SetEnemyMotionY(self, 0)`.
 * - Case 1: advances `self+0x80` as a wrapping 0-3 counter
 *   (`(self->0x80 + 1) % 4`, a classic gcc truncating-division-by-4
 *   expansion in the ROM), then triggers `SetEnemyMotionX(self, 2)` +
 *   `SetEnemyMotionY(self, 2)` + `SetEnemyAnimMode(self, 0)`.
 * - Case 6: same as case 0's toggle but on `owner+0x28` bit 5
 *   instead of bit 4, *plus* case 1's own `self+0x80` counter
 *   advance, then the same `SetEnemyMotionX`/`SetEnemyMotionY`/`SetEnemyAnimMode`
 *   trigger triple as case 1.
 *
 * Real C (issue #10 NAKED retry, docs/matching/issue-10-naked-retry.md).
 * The old blocker was the bit toggle: reading the bit into a local
 * first (`m = bit; bit = !m;`) gives the ROM's order - load, shift-test,
 * then the 0/1 materialized, shifted and merged with the `-0x11` mask.
 * Same bytes under both compilers. */

void UpdateEnemyFlipCycle(struct part_ctrl *self)
{
    struct ctrl_target *target = self->target;

    if (!target->animDone)
        return;
    switch (self->mode) {
    case 0:
        if (self->counter & 1) {
            u32 m = target->mirror.u.x;
            target->mirror.u.x = !m;
            SetEnemyAnimMode(self, 1);
        } else {
            SetEnemyAnimMode(self, 6);
        }
        SetEnemyMotionX(self, 0);
        SetEnemyMotionY(self, 0);
        break;
    case 1:
        self->counter = (self->counter + 1) % 4;
        SetEnemyMotionX(self, 2);
        SetEnemyMotionY(self, 2);
        SetEnemyAnimMode(self, 0);
        break;
    case 6:
        {
            u32 m = target->mirror.u.y;
            target->mirror.u.y = !m;
        }
        self->counter = (self->counter + 1) % 4;
        SetEnemyMotionX(self, 2);
        SetEnemyMotionY(self, 2);
        SetEnemyAnimMode(self, 0);
        break;
    }
}
