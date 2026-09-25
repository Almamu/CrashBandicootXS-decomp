#include "core.h"

/* GitHub issue #9/#10: the three small `(self, mode)`-shaped trigger
 * functions the Phase 1 investigation (docs/matching/issue-9-10-0x0800b8dc-graphics.md)
 * flagged as the highest-value next target in the 0x0800B8DC-0x0800D040
 * cluster - shared by nearly every one of sub_800B8DC's dispatch states
 * and by all four of its self+0x68 sub-dispatchers.
 *
 * All three cache `mode` into a `self`-local field and then delegate to
 * a shared trigger primitive that reads `self+0xc`'s "anchor" record
 * (an object with several `{s16 offset, void *fn}` pairs at different
 * byte offsets, each feeding a different `sub_803AD8x`-family call -
 * see the Phase 1 doc's own "self+0xC" section) and `self+0x70`
 * ("owner"), then fires `sub_803AD84(self + offset, owner, tableEntry,
 * fn)`.
 *
 * `sub_800C8AC`/`sub_800C8BC` are thin wrappers around the two
 * already-matched `sub_800B704`/`sub_800B838` accessors
 * (src/graphics/actor_part17.c) - same "look up an 8-byte record from
 * self+4's array, translate its type word through the shared
 * gStaticData_0816B304 table, then trigger via self+0xc's anchor pair"
 * shape, just reusing two *different* pairs of that anchor record
 * (part+0x28/+0x2c vs part+0x30/+0x34) and two different words of the
 * same 8-byte record (word 0 vs word 1) - this resolves two more of the
 * anchor record's pair offsets the Phase 1 doc left open.
 *
 * `sub_800C8CC` is the odd one out: instead of going through
 * `sub_800B704`/`sub_800B838`'s global-table-plus-type-index lookup, it
 * indexes `self->0x84` *directly* by `mode` (`((void **)self->0x84)[mode]`)
 * to get its table entry, and reads its own anchor pair at
 * part+0x50/+0x54. This resolves the Phase 1 doc's open question about
 * `self+0x84`: it's not a single small record (the doc's original
 * guess, based on a different, unrelated caller elsewhere), but a
 * per-instance array of pointers, direct-indexed by `mode`, that plays
 * the same "table entry" role `gStaticData_0816B304[type]` plays for
 * `sub_800C8AC`/`sub_800C8BC` - i.e. a per-object override table
 * parallel to the shared global one. */

extern void sub_800B704(void *selfArg, void *arg1, s32 index);
extern void sub_800B838(void *selfArg, void *arg1, s32 index);
extern s32 sub_803AD84(void *arg0, void *arg1, void *arg2, void *arg3);

/* Caches `mode` into `self->0x7c`, then delegates to `sub_800B704`
 * (the anchor's part+0x30/+0x34 pair, record word 1 as type). */
void sub_800C8AC(void *selfArg, s32 mode)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x7c) = mode;
    sub_800B704(self, *(void **)(self + 0x70), mode);
}

/* Same shape as `sub_800C8AC`, caching into `self->0x78` and
 * delegating to `sub_800B838` instead (the anchor's part+0x28/+0x2c
 * pair, record word 0 as type). */
void sub_800C8BC(void *selfArg, s32 mode)
{
    u8 *self = selfArg;

    *(s32 *)(self + 0x78) = mode;
    sub_800B838(self, *(void **)(self + 0x70), mode);
}

/* Caches `mode` into `self->0x68`, then triggers directly (no
 * gStaticData_0816B304 lookup): reads the anchor's part+0x50/+0x54
 * pair for the offset/fn, and indexes `self->0x84`'s own pointer array
 * by `mode` for the table-entry argument. */
void sub_800C8CC(void *selfArg, s32 mode)
{
    u8 *self = selfArg;
    u8 *rec;
    s16 offset;
    void *addr;
    void *owner;
    void **table;
    void *entry;
    void *fn;

    *(s32 *)(self + 0x68) = mode;
    rec = *(u8 **)(self + 0xc);
    rec += 0x50;
    offset = *(s16 *)rec;
    addr = self + offset;
    owner = *(void **)(self + 0x70);
    table = *(void ***)(self + 0x84);
    entry = table[mode];
    fn = *(void **)(rec + 4);

    sub_803AD84(addr, owner, entry, fn);
}
asm(".align 2, 0");
