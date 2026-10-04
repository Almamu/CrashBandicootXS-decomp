#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `sub_800C314`,
 * `UpdateEnemyCtrl`'s state-7 callee. Early-outs unless `owner+0x38`
 * (self+0x70, the "owner" object) is set, then dispatches on
 * `self+0x68` (values 0, 1, 6 handled; anything else no-ops):
 *
 * - Case 0: if `self+0x80` bit 0 is set, toggles `owner+0x28` bit 4
 *   (the established mirror-flag convention, see
 *   `src/graphics/actor_part17.c`'s `(s32)(part[0x28] << 27) < 0`
 *   idiom) and triggers `SetEnemyAnimMode(self, 1)`; otherwise just
 *   `SetEnemyAnimMode(self, 6)`. Either way, tails into
 *   `sub_800C8BC(self, 0)` + `sub_800C8AC(self, 0)`.
 * - Case 1: advances `self+0x80` as a wrapping 0-3 counter
 *   (`(self->0x80 + 1) % 4`, a classic gcc truncating-division-by-4
 *   expansion in the ROM), then triggers `sub_800C8BC(self, 2)` +
 *   `sub_800C8AC(self, 2)` + `SetEnemyAnimMode(self, 0)`.
 * - Case 6: same as case 0's toggle but on `owner+0x28` bit 5
 *   instead of bit 4, *plus* case 1's own `self+0x80` counter
 *   advance, then the same `sub_800C8BC`/`sub_800C8AC`/`SetEnemyAnimMode`
 *   trigger triple as case 1.
 *
 * Real C (issue #10 NAKED retry, docs/matching/issue-10-naked-retry.md).
 * The old blocker was the bit toggle: reading the bit into a local
 * first (`m = bit; bit = !m;`) gives the ROM's order - load, shift-test,
 * then the 0/1 materialized, shifted and merged with the `-0x11` mask.
 * Same bytes under both compilers. */
#include "part_ctrl.h"

void sub_800C314(struct part_ctrl *self)
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
        sub_800C8BC(self, 0);
        sub_800C8AC(self, 0);
        break;
    case 1:
        self->counter = (self->counter + 1) % 4;
        sub_800C8BC(self, 2);
        sub_800C8AC(self, 2);
        SetEnemyAnimMode(self, 0);
        break;
    case 6:
        {
            u32 m = target->mirror.u.y;
            target->mirror.u.y = !m;
        }
        self->counter = (self->counter + 1) % 4;
        sub_800C8BC(self, 2);
        sub_800C8AC(self, 2);
        SetEnemyAnimMode(self, 0);
        break;
    }
}
