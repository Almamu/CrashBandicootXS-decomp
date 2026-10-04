#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `sub_800C18C`/
 * `sub_800C1E8`, the X-axis/Y-axis "homing velocity-target setter"
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
#include "part_ctrl.h"

extern struct ctrl_target *gPlayer;

#define SET_VEL(v, a_, b_) \
    {                      \
        s32 a = (a_);      \
        s32 b = (b_);      \
        (v)[0] = a;        \
        (v)[1] = b;        \
        (v)[2] = a;        \
    }

void sub_800C18C(struct part_ctrl *self)
{
    struct ctrl_target *target = self->target;
    s32 x = target->x;
    s32 d = x - gPlayer->x;

    if (d > 20) {
        if (x < self->rangeX[0])
            SET_VEL(target->velA, 0, 0x10)
        else
            SET_VEL(target->velA, -self->speed, self->accel)
    } else if (d < -20) {
        if (x > self->rangeX[1])
            SET_VEL(target->velA, 0, 0x10)
        else
            SET_VEL(target->velA, self->speed, self->accel)
    } else {
        SET_VEL(target->velA, 0, self->accel)
    }
}

/* Y-axis mirror of `sub_800C18C` above: `target->y`, `velB`, and the
 * `rangeY` bounds tested in the opposite order. */
void sub_800C1E8(struct part_ctrl *self)
{
    struct ctrl_target *target = self->target;
    s32 y = target->y;
    s32 d = y - gPlayer->y;

    if (d > 20) {
        if (y < self->rangeY[1])
            SET_VEL(target->velB, 0, 0x10)
        else
            SET_VEL(target->velB, -self->speed, self->accel)
    } else if (d < -20) {
        if (y > self->rangeY[0])
            SET_VEL(target->velB, 0, 0x10)
        else
            SET_VEL(target->velB, self->speed, self->accel)
    } else {
        SET_VEL(target->velB, 0, self->accel)
    }
}
asm(".align 2, 0");
