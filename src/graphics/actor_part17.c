#include "core.h"
#include "actor.h"
#include "vtable.h"

extern u8 gStaticData_0816B304[];
extern s32 _call_via_r3(void *addr, void *arg1, void *tableEntry, void *fn);
extern s32 FixedMul(s32 a, s32 b);
extern void ResetSpriteFrameTimer(void *part);
extern void ResetSpriteFrameIndex(void *part);
extern void SetSpriteAnimDone(void *part, u8 val);
extern void OperatorDelete(void *self);
extern u8 gCtrlVtable[];

/* Looks up `selfArg`'s `index`-th 8-byte record (via a double
 * pointer chain at `self+4`), uses its second word as a type index
 * into the 12-byte-stride `gStaticData_0816B304` table, and calls
 * slot 6 of the vtable at `self+0xc` with that table entry - the same
 * base+offset+fn-pointer convention already seen in
 * `DestroyPlayer`/`ResolvePlayerContact`. */
void StartCtrlTargetMotionYFromSet(void *selfArg, void *arg1, s32 index)
{
    u8 *self = selfArg;
    void **mgr = *(void ***)(self + 4);
    register u8 *arr asm("r3") = *(u8 **)mgr;
    register s32 recOffset asm("r2") = index * 8;
    u8 *rec;
    s32 type;
    u8 *tableEntry;
    struct vtable_slot *vtbl;
    s16 offset;
    void *addr;
    void *fn;

    asm("add %0, %0, %1" : "+r" (recOffset) : "r" (arr));
    rec = (u8 *)recOffset;
    type = *(s32 *)(rec + 4);
    tableEntry = gStaticData_0816B304 + type * 12;
    vtbl = *(struct vtable_slot **)(self + 0xc);
    offset = vtbl[6].delta;
    addr = self + offset;
    fn = vtbl[6].fn;

    _call_via_r3(addr, arg1, tableEntry, fn);
}

/* Scales `vec` by `FixedMul(component, self->field4->field4)` per
 * axis and writes the result into `part+0x48`/`+0x4c`/`+0x50`,
 * negating X and Z when `part+0x28` bit 4 (a mirror flag, distinct
 * from the bit 5 flag used elsewhere) is set. */
void SetCtrlTargetMotionX(void *selfArg, void *partArg, s32 *vec)
{
    u8 *self = selfArg;
    u8 *part = partArg;

    if ((s32)(part[0x28] << 27) < 0) {
        s32 x = -FixedMul(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = FixedMul(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = -FixedMul(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    } else {
        s32 x = FixedMul(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = FixedMul(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = FixedMul(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    }
}

/* Same scaled-vector write as `SetCtrlTargetMotionX`, also duplicating the
 * (possibly negated) X component into `part+0x60` - the scaled-copy
 * counterpart of `StartCtrlTargetMotionY`'s plain-copy `+0x64` duplication. */
void StartCtrlTargetMotionX(void *selfArg, void *partArg, s32 *vec)
{
    u8 *self = selfArg;
    u8 *part = partArg;

    if ((s32)(part[0x28] << 27) < 0) {
        s32 x = -FixedMul(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = FixedMul(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = -FixedMul(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x60) = x;
        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    } else {
        s32 x = FixedMul(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = FixedMul(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = FixedMul(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x60) = x;
        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    }
}

/* Same shape as `StartCtrlTargetMotionYFromSet`, reading the record's FIRST word as
 * the type index instead of its second, and vtable slot 5 instead of
 * slot 6 - a sibling accessor for a second axis. */
void StartCtrlTargetMotionXFromSet(void *selfArg, void *arg1, s32 index)
{
    u8 *self = selfArg;
    void **mgr = *(void ***)(self + 4);
    register u8 *arr asm("r3") = *(u8 **)mgr;
    register s32 recOffset asm("r2") = index * 8;
    u8 *rec;
    s32 type;
    u8 *tableEntry;
    struct vtable_slot *vtbl;
    s16 offset;
    void *addr;
    void *fn;

    asm("add %0, %0, %1" : "+r" (recOffset) : "r" (arr));
    rec = (u8 *)recOffset;
    type = *(s32 *)(rec + 0);
    tableEntry = gStaticData_0816B304 + type * 12;
    vtbl = *(struct vtable_slot **)(self + 0xc);
    offset = vtbl[5].delta;
    addr = self + offset;
    fn = vtbl[5].fn;

    _call_via_r3(addr, arg1, tableEntry, fn);
}

/* gCtrlVtable slot 2, the base controller's event handler: empty
 * (ActionCtrlHandleEvent, PlayerCtrlHandleEvent and HitEnemy override it). */
void CtrlHandleEvent(void)
{
}
asm(".align 2, 0");

/* Sets `part`'s frame index (`+0x2d`) to `newVal`, but only if it
 * actually changed - otherwise a no-op returning 0. On a real change,
 * resets the sub-counter/frame-counter/"done" flag exactly like
 * `SetSpriteAnim` (not called directly here - inlined instead), clears
 * `part+0xc` bit 3, and returns 1. */
u8 SetCtrlTargetAnim(void *unused, void *partArg, s32 newVal)
{
    u8 *part = partArg;
    u8 result = 0;

    if (part[0x2d] != newVal) {
        part[0x2d] = newVal;
        ResetSpriteFrameTimer(part);
        ResetSpriteFrameIndex(part);
        SetSpriteAnimDone(part, 0);
        {
            register s32 mask asm("r0") = -9;
            register s32 byte asm("r1") = part[0xc];
            register s32 masked asm("r0");

            masked = mask & byte;
            part[0xc] = masked;
        }
        result = 1;
    }
    return result;
}

/* `self+0` word setter. */
void sub_800B8A4(void *selfArg, s32 val)
{
    *(s32 *)selfArg = val;
}

/* Resets `self+0xc`'s table pointer to `gCtrlVtable`, then
 * (if bit 0 of `flags` is set) fires `OperatorDelete` on `self`. */
void DestroyCtrl(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gCtrlVtable;
    if (flags & 1) {
        OperatorDelete(self);
    }
}

/* Resets `self+0xc`'s table pointer to `gCtrlVtable` and
 * clears `self+8`. */
void InitCtrl(void *selfArg)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gCtrlVtable;
    *(s32 *)(self + 8) = 0;
}

/* `self+8` word getter. */
s32 GetCtrlMode(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 8);
}
