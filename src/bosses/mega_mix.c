#include "core.h"
#include "match.h"
#include "vtable.h"
#include "player.h"
#include "bosses.h"
#include "objects.h"
#include "gobj_1a794.h"

/* GitHub issue #22, ROM 0x08017ECC-0x08017FE8 - non-adjacent to
 * airship_fireball.c since `UpdateMegaMix` (NAKED-parked, see
 * mega_mix_update.c) sits between them. `self` (`struct mega_mix_ctrl`,
 * bosses.h) uses the same motion entry set lookup as
 * `StartCtrlTargetMotionYFromSet`/`StartCtrlTargetMotionXFromSet` (ctrl.c):
 * `animSet->entries[index]` is an `{a, b}` pair (`a` for X, `b` for Y),
 * each an index into the 12-byte `gMegaMixMotionRecords` table - the same
 * base+offset+fn-pointer-table family already named in
 * action_ctrl_states.c, just a per-vector-component variant instead of the
 * per-action variant. `part->mirror` bit 4/bit 5 negate the record's
 * `start`/`target` exactly like `SetCtrlTargetMotionX`/`StartCtrlTargetMotionX`/
 * `SetCtrlTargetMotionY`/`StartCtrlTargetMotionY`.
 *
 * `SetMegaMixMotionYFromSet`/`SetMegaMixMotionXFromSet` pin `part`/`tableEntry` to r3/r2 and read
 * the table entry's Z component before Y - without this, this compiler
 * spills `part` to a callee-saved register across the branch (an
 * unneeded push/pop the true-leaf ROM function doesn't have), the same
 * class of gap documented for `SetCtrlTargetMotionY`/`StartCtrlTargetMotionY`
 * (player_flags.c). */

/* `_call_via_r2` is declared by gobj_1a794.h. */

/* Reads the entry's `b` as the record index (bit 5 mirror test), writes
 * into `part->rampY`. */
void SetMegaMixMotionYFromSet(void *selfArg, void *partArg, s32 index)
{
    struct mega_mix_ctrl *self = selfArg;
    MATCH_HOLD_REG(struct gobj *, part, r3) = partArg;
    const struct entry_set *set = self->base.animSet;
    MATCH_HOLD_REG(const struct anim_pair *, arr, r0) = (const struct anim_pair *)set->entries;
    MATCH_HOLD_REG(s32, recOffset, r2) = index * 8;
    const struct anim_pair *rec;
    MATCH_HOLD_REG(s32, type, r1);
    MATCH_HOLD_REG(s32, typeOffset, r0);
    MATCH_HOLD_REG(const s32 *, base, r1);
    MATCH_HOLD_REG(const struct speed_ramp *, tableEntry, r2);

    /* &set->entries[index], then &gMegaMixMotionRecords[type], each one
     * add in the ROM's registers */
    asm("add %0, %0, %1" : "+r"(recOffset) : "r"(arr));
    rec = (const struct anim_pair *)recOffset;
    type = rec->b;
    typeOffset = type * 12;
    base = gMegaMixMotionRecords[0];
    asm("add %0, %1, %2" : "=r"(tableEntry) : "r"(typeOffset), "r"(base));

    if ((s32)(part->mirror << 26) < 0) {
        s32 x = -tableEntry->start;
        s32 z = -tableEntry->target;
        s32 y = tableEntry->step;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    } else {
        s32 x = tableEntry->start;
        s32 y = tableEntry->step;
        s32 z = tableEntry->target;

        part->rampY.start = x;
        part->rampY.step = y;
        part->rampY.target = z;
    }
}

/* Same shape as `SetMegaMixMotionYFromSet`, reading `a` as the record
 * index (bit 4 mirror test) and writing into `part->rampX` instead. */
void SetMegaMixMotionXFromSet(void *selfArg, void *partArg, s32 index)
{
    struct mega_mix_ctrl *self = selfArg;
    MATCH_HOLD_REG(struct gobj *, part, r3) = partArg;
    const struct entry_set *set = self->base.animSet;
    MATCH_HOLD_REG(const struct anim_pair *, arr, r0) = (const struct anim_pair *)set->entries;
    MATCH_HOLD_REG(s32, recOffset, r2) = index * 8;
    const struct anim_pair *rec;
    MATCH_HOLD_REG(s32, type, r1);
    MATCH_HOLD_REG(s32, typeOffset, r0);
    MATCH_HOLD_REG(const s32 *, base, r1);
    MATCH_HOLD_REG(const struct speed_ramp *, tableEntry, r2);

    /* &set->entries[index], then &gMegaMixMotionRecords[type], each one
     * add in the ROM's registers */
    asm("add %0, %0, %1" : "+r"(recOffset) : "r"(arr));
    rec = (const struct anim_pair *)recOffset;
    type = rec->a;
    typeOffset = type * 12;
    base = gMegaMixMotionRecords[0];
    asm("add %0, %1, %2" : "=r"(tableEntry) : "r"(typeOffset), "r"(base));

    if ((s32)(part->mirror << 27) < 0) {
        s32 x = -tableEntry->start;
        s32 z = -tableEntry->target;
        s32 y = tableEntry->step;

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    } else {
        s32 x = tableEntry->start;
        s32 y = tableEntry->step;
        s32 z = tableEntry->target;

        part->rampX.start = x;
        part->rampX.step = y;
        part->rampX.target = z;
    }
}

/* Resolves the same `b`-indexed `gMegaMixMotionRecords` table entry
 * as `SetMegaMixMotionYFromSet`, then tail-calls `StartCtrlTargetMotionY` (player_flags.c,
 * still parked) to do the mirror-gated copy itself. */
void StartMegaMixMotionYFromSet(void *selfArg, void *partArg, s32 index)
{
    struct mega_mix_ctrl *self = selfArg;
    MATCH_HOLD_REG(const struct anim_pair *, arr, r3) =
        (const struct anim_pair *)self->base.animSet->entries;
    MATCH_HOLD_REG(s32, acc, r2) = index * 8;
    MATCH_HOLD_REG(s32, type, r3);
    MATCH_HOLD_REG(const s32 *, base, r3);

    asm("add %0, %0, %1" : "+r"(acc) : "r"(arr));
    type = ((const struct anim_pair *)acc)->b;
    acc = type * 12;
    base = gMegaMixMotionRecords[0];
    asm("add %0, %0, %1" : "+r"(acc) : "r"(base));

    StartCtrlTargetMotionY(selfArg, partArg, (const struct speed_ramp *)acc);
}

/* Resolves the `a`-indexed table entry like `SetMegaMixMotionXFromSet`, then
 * tail-calls `StartCtrlTargetMotionX` (ctrl.c). */
void StartMegaMixMotionXFromSet(void *selfArg, void *partArg, s32 index)
{
    struct mega_mix_ctrl *self = selfArg;
    MATCH_HOLD_REG(const struct anim_pair *, arr, r3) =
        (const struct anim_pair *)self->base.animSet->entries;
    MATCH_HOLD_REG(s32, acc, r2) = index * 8;
    MATCH_HOLD_REG(s32, type, r3);
    MATCH_HOLD_REG(const s32 *, base, r3);

    asm("add %0, %0, %1" : "+r"(acc) : "r"(arr));
    type = ((const struct anim_pair *)acc)->a;
    acc = type * 12;
    base = gMegaMixMotionRecords[0];
    asm("add %0, %0, %1" : "+r"(acc) : "r"(base));

    StartCtrlTargetMotionX(selfArg, partArg, (s32 *)acc);
}

/* Calls method table slot 4 (SetCtrlMode) with 1 via `_call_via_r2`,
 * clears `latch`, resets `stamp` to `-1`, and re-points `animSet` at
 * `gMegaMixMotionSet`. */
void ResetMegaMixCtrl(void *selfArg)
{
    struct mega_mix_ctrl *self = selfArg;
    const struct vtable_slot *table = self->base.vtable;

    MATCH_HOLD_REG(s32, zero, r0);
    u8 *p;

    _call_via_r2((u8 *)self + table[4].delta, (void *)1, table[4].fn);
    p = &self->latch;
    zero = 0;
    *p = zero;
    self->stamp = zero - 1;
    self->base.animSet = &gMegaMixMotionSet;
}

/* Re-points the method table at `gMegaMixCtrlVtable`, then
 * tail-calls `DestroyBossCtrl` (which promptly overwrites it again via
 * `DestroyCtrl` - same double-set pattern as `DestroyBossCtrl` itself). */
void DestroyMegaMixCtrl(void *selfArg, s32 flags)
{
    struct mega_mix_ctrl *self = selfArg;

    self->base.vtable = gMegaMixCtrlVtable;
    DestroyBossCtrl(&self->base, flags);
}

/* `CreateBossCtrl`-style init, but re-pointing the table at
 * `gMegaMixCtrlVtable` and finishing with `ResetMegaMixCtrl` instead of
 * zeroing `self+0x10`/`+0x14`/`+0x18` directly. Returns `self`. */
void *CreateMegaMixCtrl(void *selfArg)
{
    struct mega_mix_ctrl *self = selfArg;

    CreateBossCtrl(&self->base);
    self->base.vtable = gMegaMixCtrlVtable;
    ResetMegaMixCtrl(self);
    return self;
}
