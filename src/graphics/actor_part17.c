#include "core.h"
#include "actor.h"

extern u8 gStaticData_0816B304[];
extern s32 sub_803AD84(void *addr, void *arg1, void *tableEntry, void *fn);
extern s32 sub_80008FC(s32 a, s32 b);
extern void sub_80087C0(void *part);
extern void sub_80087B4(void *part);
extern void sub_800872C(void *part, u8 val);
extern void sub_8026ED0(void *self);
extern u8 gStaticData_087E3E7C[];

/* Looks up `selfArg`'s `index`-th 8-byte record (via a double
 * pointer chain at `self+4`), uses its second word as a type index
 * into the 12-byte-stride `gStaticData_0816B304` table, and fires
 * that table entry's trampoline at `self + (int16 offset from
 * self->0xc's part+0x30)` through the function pointer at
 * part+0x34 - the same base+offset+fn-pointer convention already
 * seen in `sub_800B3AC`/`sub_8009D5C`. */
void sub_800B704(void *selfArg, void *arg1, s32 index)
{
    u8 *self = selfArg;
    void **mgr = *(void ***)(self + 4);
    register u8 *arr asm("r3") = *(u8 **)mgr;
    register s32 recOffset asm("r2") = index * 8;
    u8 *rec;
    s32 type;
    u8 *tableEntry;
    u8 *part;
    s16 offset;
    void *addr;
    void *fn;

    asm("add %0, %0, %1" : "+r" (recOffset) : "r" (arr));
    rec = (u8 *)recOffset;
    type = *(s32 *)(rec + 4);
    tableEntry = gStaticData_0816B304 + type * 12;
    part = *(u8 **)(self + 0xc);
    offset = *(s16 *)(part + 0x30);
    addr = self + offset;
    fn = *(void **)(part + 0x34);

    sub_803AD84(addr, arg1, tableEntry, fn);
}

/* Scales `vec` by `sub_80008FC(component, self->field4->field4)` per
 * axis and writes the result into `part+0x48`/`+0x4c`/`+0x50`,
 * negating X and Z when `part+0x28` bit 4 (a mirror flag, distinct
 * from the bit 5 flag used elsewhere) is set. */
void sub_800B734(void *selfArg, void *partArg, s32 *vec)
{
    u8 *self = selfArg;
    u8 *part = partArg;

    if ((s32)(part[0x28] << 27) < 0) {
        s32 x = -sub_80008FC(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = sub_80008FC(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = -sub_80008FC(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    } else {
        s32 x = sub_80008FC(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = sub_80008FC(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = sub_80008FC(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    }
}

/* Same scaled-vector write as `sub_800B734`, also duplicating the
 * (possibly negated) X component into `part+0x60` - the scaled-copy
 * counterpart of `sub_800B6D0`'s plain-copy `+0x64` duplication. */
void sub_800B7B0(void *selfArg, void *partArg, s32 *vec)
{
    u8 *self = selfArg;
    u8 *part = partArg;

    if ((s32)(part[0x28] << 27) < 0) {
        s32 x = -sub_80008FC(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = sub_80008FC(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = -sub_80008FC(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x60) = x;
        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    } else {
        s32 x = sub_80008FC(vec[0], *(s32 *)(*(void **)(self + 4) + 4));
        s32 y = sub_80008FC(vec[1], *(s32 *)(*(void **)(self + 4) + 4));
        s32 z = sub_80008FC(vec[2], *(s32 *)(*(void **)(self + 4) + 4));

        *(s32 *)(part + 0x60) = x;
        *(s32 *)(part + 0x48) = x;
        *(s32 *)(part + 0x4c) = y;
        *(s32 *)(part + 0x50) = z;
    }
}

/* Same shape as `sub_800B704`, reading the record's FIRST word as
 * the type index instead of its second, and `part+0x28`/`part+0x2c`
 * instead of `part+0x30`/`part+0x34` - a sibling accessor for a
 * second axis. */
void sub_800B838(void *selfArg, void *arg1, s32 index)
{
    u8 *self = selfArg;
    void **mgr = *(void ***)(self + 4);
    register u8 *arr asm("r3") = *(u8 **)mgr;
    register s32 recOffset asm("r2") = index * 8;
    u8 *rec;
    s32 type;
    u8 *tableEntry;
    u8 *part;
    s16 offset;
    void *addr;
    void *fn;

    asm("add %0, %0, %1" : "+r" (recOffset) : "r" (arr));
    rec = (u8 *)recOffset;
    type = *(s32 *)(rec + 0);
    tableEntry = gStaticData_0816B304 + type * 12;
    part = *(u8 **)(self + 0xc);
    offset = *(s16 *)(part + 0x28);
    addr = self + offset;
    fn = *(void **)(part + 0x2c);

    sub_803AD84(addr, arg1, tableEntry, fn);
}

void nullsub_13(void)
{
}
asm(".align 2, 0");

/* Sets `part`'s frame index (`+0x2d`) to `newVal`, but only if it
 * actually changed - otherwise a no-op returning 0. On a real change,
 * resets the sub-counter/frame-counter/"done" flag exactly like
 * `sub_80087D0` (not called directly here - inlined instead), clears
 * `part+0xc` bit 3, and returns 1. */
u8 sub_800B86C(void *unused, void *partArg, s32 newVal)
{
    u8 *part = partArg;
    u8 result = 0;

    if (part[0x2d] != newVal) {
        part[0x2d] = newVal;
        sub_80087C0(part);
        sub_80087B4(part);
        sub_800872C(part, 0);
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

/* Resets `self+0xc`'s table pointer to `gStaticData_087E3E7C`, then
 * (if bit 0 of `flags` is set) fires `sub_8026ED0` on `self`. */
void sub_800B8A8(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E3E7C;
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Resets `self+0xc`'s table pointer to `gStaticData_087E3E7C` and
 * clears `self+8`. */
void sub_800B8C8(void *selfArg)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E3E7C;
    *(s32 *)(self + 8) = 0;
}

/* `self+8` word getter. */
s32 sub_800B8D8(void *selfArg)
{
    return *(s32 *)((u8 *)selfArg + 8);
}
