#include "core.h"

/* GitHub issue #22, ROM 0x08017ECC-0x08017FE8 - non-adjacent to
 * actor_part20.c since `sub_8017AB0` (NAKED-parked, see
 * actor_part27a.c) sits between them. `self` uses the same `self+4` double-
 * pointer-chain record lookup as `sub_800B704`/`sub_800B838`
 * (actor_part17.c): `self+4` is a manager pointer whose own first
 * word is an array of 8-byte records, indexed here by `index`. Each
 * record's word (offset 0 or 4, depending on the function) is a type
 * id into the 12-byte-stride `gStaticData_0816C2D8` table - the same
 * base+offset+fn-pointer-table family already named in
 * actor_part18.c, just a per-vector-component variant instead of the
 * per-action variant. `part+0x28` bit 4/bit 5 mirror flags negate the
 * X/Z components exactly like `sub_800B734`/`sub_800B7B0`/
 * `sub_800B6A0`/`sub_800B6D0`. `self+0xc`'s table convention and
 * `self+0x1c`/`self+0x20` also match actor_part20.c's group.
 *
 * `sub_8017ECC`/`sub_8017F14` pin `part`/`tableEntry` to r3/r2 and read
 * the table entry's Z component before Y - without this, this compiler
 * spills `part` to a callee-saved register across the branch (an
 * unneeded push/pop the true-leaf ROM function doesn't have), the same
 * class of gap documented for `sub_800B6A0`/`sub_800B6D0`
 * (actor_part16.c). */

extern u8 gStaticData_0816C2D8[];
extern u8 gStaticData_0816C2D0[];
extern u8 gStaticData_087E43C4[];
extern s32 sub_803AD80(void *arg0, void *arg1, void *arg2);
extern void sub_800B6D0(void *unused, void *selfArg, s32 *vec);
extern void sub_800B7B0(void *selfArg, void *partArg, s32 *vec);
extern void sub_8017A78(void *selfArg, s32 flags);
extern void *sub_8017A8C(void *selfArg);

/* Reads `rec+4` as the type id (bit 5 mirror test), writes into
 * `part+0x54`/`+0x58`/`+0x5c`. */
void sub_8017ECC(void *selfArg, void *partArg, s32 index)
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
    base = gStaticData_0816C2D8;
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

/* Same shape as `sub_8017ECC`, reading `rec+0` as the type id (bit 4
 * mirror test) and writing into `part+0x48`/`+0x4c`/`+0x50` instead. */
void sub_8017F14(void *selfArg, void *partArg, s32 index)
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
    base = gStaticData_0816C2D8;
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

/* Resolves the same `rec+4`-typed `gStaticData_0816C2D8` table entry
 * as `sub_8017ECC`, then tail-calls `sub_800B6D0` (actor_part16.c,
 * still parked) to do the mirror-gated copy itself. */
void sub_8017F5C(void *selfArg, void *partArg, s32 index)
{
    register u8 *arr asm("r3") = *(u8 **)(*(void ***)((u8 *)selfArg + 4));
    register s32 acc asm("r2") = index * 8;
    register s32 type asm("r3");
    register u8 *base asm("r3");

    asm("add %0, %0, %1" : "+r" (acc) : "r" (arr));
    type = *(s32 *)((u8 *)acc + 4);
    acc = type * 12;
    base = gStaticData_0816C2D8;
    asm("add %0, %0, %1" : "+r" (acc) : "r" (base));

    sub_800B6D0(selfArg, partArg, (s32 *)acc);
}

/* Resolves the `rec+0`-typed table entry like `sub_8017F14`, then
 * tail-calls `sub_800B7B0` (actor_part17.c). */
void sub_8017F80(void *selfArg, void *partArg, s32 index)
{
    register u8 *arr asm("r3") = *(u8 **)(*(void ***)((u8 *)selfArg + 4));
    register s32 acc asm("r2") = index * 8;
    register s32 type asm("r3");
    register u8 *base asm("r3");

    asm("add %0, %0, %1" : "+r" (acc) : "r" (arr));
    type = *(s32 *)((u8 *)acc + 0);
    acc = type * 12;
    base = gStaticData_0816C2D8;
    asm("add %0, %0, %1" : "+r" (acc) : "r" (base));

    sub_800B7B0(selfArg, partArg, (s32 *)acc);
}

/* Fires the usual `self+0xc`-table base+offset+fn-pointer trampoline
 * (action `1`) via `sub_803AD80`, clears `self+0x20`'s byte, resets
 * `self+0x1c` to `-1`, and re-points `self+4` at
 * `gStaticData_0816C2D0`. */
void sub_8017FA4(void *selfArg)
{
    u8 *self = selfArg;
    u8 *table = *(u8 **)(self + 0xc);

    register s32 zero asm("r0");
    u8 *p;

    sub_803AD80(self + *(s16 *)(table + 0x20), (void *)1,
                *(void **)(table + 0x24));
    p = self + 0x20;
    zero = 0;
    *p = zero;
    *(s32 *)(self + 0x1c) = zero - 1;
    *(void **)(self + 4) = gStaticData_0816C2D0;
}

/* Re-points `self+0xc`'s table pointer at `gStaticData_087E43C4`, then
 * tail-calls `sub_8017A78` (which promptly overwrites it again via
 * `sub_800B8A8` - same double-set pattern as `sub_8017A78` itself). */
void sub_8017FD4(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E43C4;
    sub_8017A78(self, flags);
}

/* `sub_8017A8C`-style init, but re-pointing the table at
 * `gStaticData_087E43C4` and finishing with `sub_8017FA4` instead of
 * zeroing `self+0x10`/`+0x14`/`+0x18` directly. Returns `self`. */
void *sub_8017FE8(void *selfArg)
{
    u8 *self = selfArg;

    sub_8017A8C(self);
    *(void **)(self + 0xc) = gStaticData_087E43C4;
    sub_8017FA4(self);
    return self;
}
