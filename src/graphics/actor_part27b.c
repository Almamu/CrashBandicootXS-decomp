#include "core.h"
#include "vtable.h"

/* GitHub issue #22, ROM 0x08017ECC-0x08017FE8 - non-adjacent to
 * actor_part20.c since `UpdateMegaMix` (NAKED-parked, see
 * actor_part27a.c) sits between them. `self` uses the same `self+4` double-
 * pointer-chain record lookup as `StartCtrlTargetMotionYFromSet`/`StartCtrlTargetMotionXFromSet`
 * (actor_part17.c): `self+4` is a manager pointer whose own first
 * word is an array of 8-byte records, indexed here by `index`. Each
 * record's word (offset 0 or 4, depending on the function) is a type
 * id into the 12-byte-stride `gMegaMixMotionRecords` table - the same
 * base+offset+fn-pointer-table family already named in
 * actor_part18.c, just a per-vector-component variant instead of the
 * per-action variant. `part+0x28` bit 4/bit 5 mirror flags negate the
 * X/Z components exactly like `SetCtrlTargetMotionX`/`StartCtrlTargetMotionX`/
 * `SetCtrlTargetMotionY`/`StartCtrlTargetMotionY`. `self+0xc`'s table convention and
 * `self+0x1c`/`self+0x20` also match actor_part20.c's group.
 *
 * `SetMegaMixMotionYFromSet`/`SetMegaMixMotionXFromSet` pin `part`/`tableEntry` to r3/r2 and read
 * the table entry's Z component before Y - without this, this compiler
 * spills `part` to a callee-saved register across the branch (an
 * unneeded push/pop the true-leaf ROM function doesn't have), the same
 * class of gap documented for `SetCtrlTargetMotionY`/`StartCtrlTargetMotionY`
 * (actor_part16.c). */

extern u8 gMegaMixMotionRecords[];
extern u8 gMegaMixMotionSet[];
extern u8 gMegaMixCtrlVtable[];
extern s32 _call_via_r2(void *arg0, void *arg1, void *arg2);
extern void StartCtrlTargetMotionY(void *unused, void *selfArg, s32 *vec);
extern void StartCtrlTargetMotionX(void *selfArg, void *partArg, s32 *vec);
extern void DestroyBossCtrl(void *selfArg, s32 flags);
extern void *CreateBossCtrl(void *selfArg);

/* Reads `rec+4` as the type id (bit 5 mirror test), writes into
 * `part+0x54`/`+0x58`/`+0x5c`. */
void SetMegaMixMotionYFromSet(void *selfArg, void *partArg, s32 index)
{
    u8 *self = selfArg;
    register u8 *part asm("r3") = partArg;
    void **mgr = *(void ***)(self + 4);
    register u8 *arr asm("r0") = *(u8 **)mgr;
    register s32 recOffset asm("r2") = index * 8;
    u8 *rec;
    register s32 type asm("r1");
    register s32 typeOffset asm("r0");
    register u8 *base asm("r1");
    register u8 *tableEntry asm("r2");

    asm("add %0, %0, %1" : "+r" (recOffset) : "r" (arr));
    rec = (u8 *)recOffset;
    type = *(s32 *)(rec + 4);
    typeOffset = type * 12;
    base = gMegaMixMotionRecords;
    asm("add %0, %1, %2" : "=r" (tableEntry) : "r" (typeOffset), "r" (base));

    if ((s32)(part[0x28] << 26) < 0) {
        s32 x = -*(s32 *)(tableEntry + 0);
        s32 z = -*(s32 *)(tableEntry + 8);
        s32 y = *(s32 *)(tableEntry + 4);

        *(s32 *)(part + 0x54) = x;
        *(s32 *)(part + 0x58) = y;
        *(s32 *)(part + 0x5c) = z;
    } else {
        s32 x = *(s32 *)(tableEntry + 0);
        s32 y = *(s32 *)(tableEntry + 4);
        s32 z = *(s32 *)(tableEntry + 8);

        *(s32 *)(part + 0x54) = x;
        *(s32 *)(part + 0x58) = y;
        *(s32 *)(part + 0x5c) = z;
    }
}

/* Same shape as `SetMegaMixMotionYFromSet`, reading `rec+0` as the type id (bit 4
 * mirror test) and writing into `part+0x48`/`+0x4c`/`+0x50` instead. */
void SetMegaMixMotionXFromSet(void *selfArg, void *partArg, s32 index)
{
    u8 *self = selfArg;
    register u8 *part asm("r3") = partArg;
    void **mgr = *(void ***)(self + 4);
    register u8 *arr asm("r0") = *(u8 **)mgr;
    register s32 recOffset asm("r2") = index * 8;
    u8 *rec;
    register s32 type asm("r1");
    register s32 typeOffset asm("r0");
    register u8 *base asm("r1");
    register u8 *tableEntry asm("r2");

    asm("add %0, %0, %1" : "+r" (recOffset) : "r" (arr));
    rec = (u8 *)recOffset;
    type = *(s32 *)(rec + 0);
    typeOffset = type * 12;
    base = gMegaMixMotionRecords;
    asm("add %0, %1, %2" : "=r" (tableEntry) : "r" (typeOffset), "r" (base));

    if ((s32)(part[0x28] << 27) < 0) {
        s32 x = -*(s32 *)(tableEntry + 0);
        s32 z = -*(s32 *)(tableEntry + 8);
        s32 y = *(s32 *)(tableEntry + 4);

        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    } else {
        s32 x = *(s32 *)(tableEntry + 0);
        s32 y = *(s32 *)(tableEntry + 4);
        s32 z = *(s32 *)(tableEntry + 8);

        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    }
}

/* Resolves the same `rec+4`-typed `gMegaMixMotionRecords` table entry
 * as `SetMegaMixMotionYFromSet`, then tail-calls `StartCtrlTargetMotionY` (actor_part16.c,
 * still parked) to do the mirror-gated copy itself. */
void StartMegaMixMotionYFromSet(void *selfArg, void *partArg, s32 index)
{
    register u8 *arr asm("r3") = *(u8 **)(*(void ***)((u8 *)selfArg + 4));
    register s32 acc asm("r2") = index * 8;
    register s32 type asm("r3");
    register u8 *base asm("r3");

    asm("add %0, %0, %1" : "+r" (acc) : "r" (arr));
    type = *(s32 *)((u8 *)acc + 4);
    acc = type * 12;
    base = gMegaMixMotionRecords;
    asm("add %0, %0, %1" : "+r" (acc) : "r" (base));

    StartCtrlTargetMotionY(selfArg, partArg, (s32 *)acc);
}

/* Resolves the `rec+0`-typed table entry like `SetMegaMixMotionXFromSet`, then
 * tail-calls `StartCtrlTargetMotionX` (actor_part17.c). */
void StartMegaMixMotionXFromSet(void *selfArg, void *partArg, s32 index)
{
    register u8 *arr asm("r3") = *(u8 **)(*(void ***)((u8 *)selfArg + 4));
    register s32 acc asm("r2") = index * 8;
    register s32 type asm("r3");
    register u8 *base asm("r3");

    asm("add %0, %0, %1" : "+r" (acc) : "r" (arr));
    type = *(s32 *)((u8 *)acc + 0);
    acc = type * 12;
    base = gMegaMixMotionRecords;
    asm("add %0, %0, %1" : "+r" (acc) : "r" (base));

    StartCtrlTargetMotionX(selfArg, partArg, (s32 *)acc);
}

/* Fires the usual `self+0xc`-table base+offset+fn-pointer trampoline
 * (action `1`) via `_call_via_r2`, clears `self+0x20`'s byte, resets
 * `self+0x1c` to `-1`, and re-points `self+4` at
 * `gMegaMixMotionSet`. */
void ResetMegaMixCtrl(void *selfArg)
{
    u8 *self = selfArg;
    struct vtable_slot *table = *(struct vtable_slot **)(self + 0xc);

    register s32 zero asm("r0");
    u8 *p;

    _call_via_r2(self + table[4].delta, (void *)1, table[4].fn);
    p = self + 0x20;
    zero = 0;
    *p = zero;
    *(s32 *)(self + 0x1c) = zero - 1;
    *(void **)(self + 4) = gMegaMixMotionSet;
}

/* Re-points `self+0xc`'s table pointer at `gMegaMixCtrlVtable`, then
 * tail-calls `DestroyBossCtrl` (which promptly overwrites it again via
 * `DestroyCtrl` - same double-set pattern as `DestroyBossCtrl` itself). */
void DestroyMegaMixCtrl(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gMegaMixCtrlVtable;
    DestroyBossCtrl(self, flags);
}

/* `CreateBossCtrl`-style init, but re-pointing the table at
 * `gMegaMixCtrlVtable` and finishing with `ResetMegaMixCtrl` instead of
 * zeroing `self+0x10`/`+0x14`/`+0x18` directly. Returns `self`. */
void *CreateMegaMixCtrl(void *selfArg)
{
    u8 *self = selfArg;

    CreateBossCtrl(self);
    *(void **)(self + 0xc) = gMegaMixCtrlVtable;
    ResetMegaMixCtrl(self);
    return self;
}
