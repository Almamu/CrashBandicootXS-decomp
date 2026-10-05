#include "core.h"

/* GitHub issue #9/#10 (0x0800B8DC-0x0800D040 cluster, see
 * docs/matching/issue-9-10-0x0800b8dc-graphics.md): `CreateKnockedEnemyCtrl`,
 * `HitEnemy` states 19-20's "spawn a child object" allocator
 * (called right after `OperatorNew(0x10)`, whose leftover return
 * value is the implicit `self` argument here per the Phase 1 doc's
 * own note about this call site setting no registers explicitly).
 *
 * This resolves two of the Phase 1 doc's open questions at once:
 * `CreateKnockedEnemyCtrl` takes exactly **one** argument (`self`), not an
 * unconfirmed count as previously flagged; and `self+0xc` (the
 * "anchor" record read by `SetEnemyMotionY`/`SetEnemyMotionX`/`SetEnemyAnimMode`
 * and by several of `UpdateEnemyCtrl`'s own dispatch states, per the
 * Phase 1/2 docs) is set here to the fixed global table
 * `gKnockedEnemyCtrlVtable` - i.e. every object constructed through this
 * path shares the same anchor record. Follows the exact same
 * "reset via `InitCtrl`, then re-point `self+0xc`'s table pointer,
 * return `self`" shape already matched for sibling constructors
 * `sub_801886C` (`src/graphics/actor_part16.c`) and `sub_8018858`
 * (`src/graphics/actor_part18.c`), plus a `nullsub_14(self)` no-op
 * tail call specific to this object type. */
extern u8 gKnockedEnemyCtrlVtable[];
extern void InitCtrl(void *self);
extern void nullsub_14(void *self);

void *CreateKnockedEnemyCtrl(void *selfArg)
{
    u8 *self = selfArg;

    InitCtrl(self);
    *(void **)(self + 0xc) = gKnockedEnemyCtrlVtable;
    nullsub_14(self);
    return self;
}
