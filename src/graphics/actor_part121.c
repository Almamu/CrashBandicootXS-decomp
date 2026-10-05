#include "core.h"
#include "gobj_1a794.h"

/* GitHub issue #9/#10: `sub_800BFA8`, the last raw function in the
 * `0x0800B8DC`-`0x0800D040` cluster's own `asm/code_3_2_17_bfa8.s`
 * chunk - called only from `UpdateEnemyCtrl` state 15 (case 42, right
 * after `UpdateEnemyAttackCycle`), per docs/matching/issue-9-10-0x0800b8dc-
 * graphics.md. Small "close enough" gate + `self+0x68`-keyed 2-way
 * dispatch, the same `self+0x68` sub-state byte `UpdateEnemyAttackCycle`/
 * `sub_800C5D4` already key off (docs/rom_map.md's "generic state-
 * machine selector" family, now 8+ confirmed sites).
 *
 * Prelude: `__modsi3(gRoomFrameCount + self->0x48 - self->0x4c,
 * self->0x48)` - the same "close enough" scalar-check primitive used
 * throughout this cluster, here against the still-unexplained global
 * `gRoomFrameCount` read as a plain word (not the table-base-pointer
 * role `UpdateEnemyAttackCycle` uses it in - `docs/rom_map.md` already flags this
 * global as multi-shaped across its 3 confirmed sites). When the check
 * passes (result `0`), `self->0x68 == 0`/`4` trigger
 * `SetEnemyAnimMode(self, 2)`/`SetEnemyAnimMode(self, 7)` respectively; anything
 * else is a no-op.
 *
 * When the check fails, dispatch moves to `owner` (`self->0x70`).  If
 * `owner->0x38` (the cluster's established "enabled" byte) is set,
 * `self->0x68 == 2`/`7` trigger `SetEnemyAnimMode(self, 0)`/
 * `SetEnemyAnimMode(self, 4)`. Otherwise (`owner->0x38 == 0`),
 * `self->0x68 == 2`/`7` each gate a `sub_800C9C8(0xc, 6, 0, d, 0x400,
 * owner)` call (`d = -0xa` for mode 2, `d = 8` for mode 7) behind an
 * `owner->0x30`/`owner->0x34` magic-constant check (`0xa`/`0` and
 * `8`/`0` respectively - the same blocking-condition pair
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md's field table
 * already documents); on success the returned record's `+0xa` byte is
 * set to `8`.
 *
 * Matched as real C - small enough to avoid this cluster's usual
 * `self`/`owner` multi-field-liveness register-pressure trap. Needed
 * two of this project's established gcc-2.9 register-allocation
 * techniques (see matching_decomp_register_pinning memory /
 * docs/matching.md): pinning `self` to `asm("r4")` (gcc's unforced
 * allocator otherwise duplicates `self` into a spare `r5` purely to
 * re-read `self->0x68` a second time, pushing/popping a register the
 * ROM never touches), and hoisting the `gRoomFrameCount` read into
 * its own statement ahead of `self->0x48`'s (two independent loads
 * gcc's scheduler otherwise reorders relative to the ROM). Confirmed
 * byte-identical to `baserom.gba`'s own raw bytes at
 * `0x0800BFA8`-`0x0800C074` via the isolated cpp/agbcc/as +
 * objcopy/cmp pipeline (only `bl` relocation sites and the
 * `gRoomFrameCount` literal-pool word differ, both of which resolve
 * correctly once linked) plus a full clean `rm -rf build && make
 * NON_MATCHING=1 report` (no warnings) and `rm -rf build
 * crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
 * make compare` (`La suma coincide`). This closes out
 * `asm/code_3_2_17_bfa8.s` entirely - retired from `ldscript.txt`. */

extern void SetEnemyAnimMode(void *self, s32 mode);
extern s32 __modsi3(s32 a, s32 b);
extern void *sub_800C9C8(s32 a, s32 b, s32 c, s32 d, s32 e, void *f);

/* The fields of this cluster's controller object (the class of
 * UpdateEnemyCtrl, see actor_part124.c's `struct trigger_ctrl`) read here:
 * `period`/`phase` make the gate below pass once every `period` frames,
 * `mode` is the `self+0x68` sub-state and `owner` the controlled
 * object. */
struct trigger_ctrl {
    u8 unk_00[0x48];
    s32 period;         // 0x48
    s32 phase;          // 0x4C
    u8 unk_50[0x18];
    s32 mode;           // 0x68
    u8 unk_6c[4];
    struct gobj *owner; // 0x70
};

void sub_800BFA8(void *selfArg)
{
    register struct trigger_ctrl *self asm("r4") = selfArg;
    struct gobj *owner;
    u8 *record;
    s32 base = (s32)gRoomFrameCount;
    s32 period = self->period;
    s32 divCheck = __modsi3(base + period - self->phase, period);

    if (divCheck == 0) {
        switch (self->mode) {
        case 0:
            SetEnemyAnimMode(self, 2);
            break;
        case 4:
            SetEnemyAnimMode(self, 7);
            break;
        }
        return;
    }

    owner = self->owner;
    if (owner->animDone != 0) {
        switch (self->mode) {
        case 2:
            SetEnemyAnimMode(self, 0);
            break;
        case 7:
            SetEnemyAnimMode(self, 4);
            break;
        }
        return;
    }

    record = NULL;
    switch (self->mode) {
    case 2:
        if (owner->frame == 0xa && owner->stepTimer == 0)
            record = sub_800C9C8(0xc, 6, 0, -0xa, 0x400, owner);
        break;
    case 7:
        if (owner->frame == 8 && owner->stepTimer == 0)
            record = sub_800C9C8(0xc, 6, 0, 8, 0x400, owner);
        break;
    }
    if (record != NULL)
        record[0xa] = 8;
}
