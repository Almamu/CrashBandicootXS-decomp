#include "core.h"

/* GitHub issue #9/#10: `sub_800BFA8`, the last raw function in the
 * `0x0800B8DC`-`0x0800D040` cluster's own `asm/code_3_2_17_bfa8.s`
 * chunk - called only from `sub_800B8DC` state 15 (case 42, right
 * after `sub_800C40C`), per docs/matching/issue-9-10-0x0800b8dc-
 * graphics.md. Small "close enough" gate + `self+0x68`-keyed 2-way
 * dispatch, the same `self+0x68` sub-state byte `sub_800C40C`/
 * `sub_800C5D4` already key off (docs/rom_map.md's "generic state-
 * machine selector" family, now 8+ confirmed sites).
 *
 * Prelude: `sub_803AE4C(gUnknown_0300082C + self->0x48 - self->0x4c,
 * self->0x48)` - the same "close enough" scalar-check primitive used
 * throughout this cluster, here against the still-unexplained global
 * `gUnknown_0300082C` read as a plain word (not the table-base-pointer
 * role `sub_800C40C` uses it in - `docs/rom_map.md` already flags this
 * global as multi-shaped across its 3 confirmed sites). When the check
 * passes (result `0`), `self->0x68 == 0`/`4` trigger
 * `sub_800C8CC(self, 2)`/`sub_800C8CC(self, 7)` respectively; anything
 * else is a no-op.
 *
 * When the check fails, dispatch moves to `owner` (`self->0x70`).  If
 * `owner->0x38` (the cluster's established "enabled" byte) is set,
 * `self->0x68 == 2`/`7` trigger `sub_800C8CC(self, 0)`/
 * `sub_800C8CC(self, 4)`. Otherwise (`owner->0x38 == 0`),
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
 * ROM never touches), and hoisting the `gUnknown_0300082C` read into
 * its own statement ahead of `self->0x48`'s (two independent loads
 * gcc's scheduler otherwise reorders relative to the ROM). Confirmed
 * byte-identical to `baserom.gba`'s own raw bytes at
 * `0x0800BFA8`-`0x0800C074` via the isolated cpp/agbcc/as +
 * objcopy/cmp pipeline (only `bl` relocation sites and the
 * `gUnknown_0300082C` literal-pool word differ, both of which resolve
 * correctly once linked) plus a full clean `rm -rf build && make
 * NON_MATCHING=1 report` (no warnings) and `rm -rf build
 * crashbandicootxs.elf crashbandicootxs.gba crashbandicootxs.map &&
 * make compare` (`La suma coincide`). This closes out
 * `asm/code_3_2_17_bfa8.s` entirely - retired from `ldscript.txt`. */

extern void sub_800C8CC(void *self, s32 mode);
extern s32 sub_803AE4C(s32 a, s32 b);
extern void *sub_800C9C8(s32 a, s32 b, s32 c, s32 d, s32 e, void *f);
extern u32 gUnknown_0300082C;

void sub_800BFA8(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;
    u8 *owner;
    u8 *record;
    s32 base = (s32)gUnknown_0300082C;
    s32 field48 = *(s32 *)(self + 0x48);
    s32 divCheck = sub_803AE4C(base + field48 - *(s32 *)(self + 0x4c), field48);

    if (divCheck == 0) {
        switch (*(s32 *)(self + 0x68)) {
        case 0:
            sub_800C8CC(self, 2);
            break;
        case 4:
            sub_800C8CC(self, 7);
            break;
        }
        return;
    }

    owner = *(u8 **)(self + 0x70);
    if (*(u8 *)(owner + 0x38) != 0) {
        switch (*(s32 *)(self + 0x68)) {
        case 2:
            sub_800C8CC(self, 0);
            break;
        case 7:
            sub_800C8CC(self, 4);
            break;
        }
        return;
    }

    record = NULL;
    switch (*(s32 *)(self + 0x68)) {
    case 2:
        if (*(s32 *)(owner + 0x30) == 0xa && *(s32 *)(owner + 0x34) == 0)
            record = sub_800C9C8(0xc, 6, 0, -0xa, 0x400, owner);
        break;
    case 7:
        if (*(s32 *)(owner + 0x30) == 8 && *(s32 *)(owner + 0x34) == 0)
            record = sub_800C9C8(0xc, 6, 0, 8, 0x400, owner);
        break;
    }
    if (record != NULL)
        record[0xa] = 8;
}
