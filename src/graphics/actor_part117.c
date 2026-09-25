#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `sub_800CBD4`,
 * `sub_800BD48` states 19-20's "spawn a child object" allocator
 * (called right after `sub_8026EDC(0x10)`, whose leftover return
 * value is the implicit `self` argument here per the Phase 1 doc's
 * own note about this call site setting no registers explicitly).
 *
 * This resolves two of the Phase 1 doc's open questions at once:
 * `sub_800CBD4` takes exactly **one** argument (`self`), not an
 * unconfirmed count as previously flagged; and `self+0xc` (the
 * "anchor" record read by `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC`
 * and by several of `sub_800B8DC`'s own dispatch states, per the
 * Phase 1/2 docs) is set here to the fixed global table
 * `gStaticData_087E3FA4` - i.e. every object constructed through this
 * path shares the same anchor record. Follows the exact same
 * "reset via `sub_800B8C8`, then re-point `self+0xc`'s table pointer,
 * return `self`" shape already matched for sibling constructors
 * `sub_801886C` (`src/graphics/actor_part16.c`) and `sub_8018858`
 * (`src/graphics/actor_part18.c`), plus a `nullsub_14(self)` no-op
 * tail call specific to this object type. */
extern u8 gStaticData_087E3FA4[];
extern void sub_800B8C8(void *self);
extern void nullsub_14(void *self);

void *sub_800CBD4(void *selfArg)
{
    u8 *self = selfArg;

    sub_800B8C8(self);
    *(void **)(self + 0xc) = gStaticData_087E3FA4;
    nullsub_14(self);
    return self;
}
