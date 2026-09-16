#include "core.h"
#include "actor.h"

struct aabb {
    s32 field_0;
    s32 field_4;
    s32 field_8;
    s32 field_c;
};

extern void *sub_8007C30(void *dest, void *pt);
extern void *sub_8007CF8(void *dest, void *pt);
extern u8 sub_8001688(void *buf1, void *buf2);

/* Builds `part`'s primary AABB (`sub_8007C30`) and tests it against
 * `region` (`sub_8001688`, the same collision-test function used by
 * `sub_8007DBC`/`sub_8008304`'s sibling); if that already overlaps,
 * returns 2. Otherwise builds the secondary AABB (`sub_8007CF8`) and
 * re-tests; if that misses, returns 0. If it hits, returns 2 unless
 * `part->flags` bit 6 is set, in which case it returns the (nonzero)
 * hit-test result itself. Needed the flags byte loaded into `part`'s
 * own dying register (`r5`, matching the ROM's `ldrb r5, [r5, #0xc]`
 * self-overwrite - `part` is never used again afterward) and read
 * through a `u32` (not `s32`) intermediate so the `>> 6` compiles to
 * a logical `lsr` instead of an arithmetic `asr`. */
s32 sub_8009FF4(void *part, void *region)
{
    struct aabb box;
    s32 result;

    sub_8007C30(&box, part);
    if ((u8)sub_8001688(&box, region) != 0) {
        goto returnTwo;
    }

    {
        struct aabb box2;

        sub_8007CF8(&box2, part);
        box = box2;

        result = (u8)sub_8001688(&box, region);
        if (result == 0) {
            goto end;
        }
        {
            register u32 flags asm("r5") = *((u8 *)part + 0xc);
            register u32 shifted asm("r0") = flags >> 6;
            register u32 test asm("r0");
            register u32 mask asm("r1") = 1;

            test = shifted & mask;
            if (test != 0) {
                goto end;
            }
        }
    }
returnTwo:
    result = 2;
end:
    return result;
}

extern void *sub_803AD7C(void *arg0, void *fn);

/* Fires a `self->table+0x70/0x74`-driven trampoline via `sub_803AD7C`
 * (same `table+N`/`table+N+4` convention used throughout this ROM
 * region) and always returns 0. Needed the trampoline's `addr = self
 * + offset` computed before the `fn` load (reusing the adjusted table
 * pointer's own dying register for `fn`), the same accumulator-style
 * fix established for `sub_8009F1C`/`sub_8009FB0` above. */
s32 sub_800A050(void *self)
{
    register u8 *tblAdj asm("r1") = *(u8 **)((u8 *)self + 0x18) + 0x70;
    register s32 offset asm("r2") = *(s16 *)tblAdj;
    register void *addr asm("r0");
    register void *fn asm("r1");

    addr = (u8 *)self + offset;
    fn = *(void **)(tblAdj + 4);
    sub_803AD7C(addr, fn);
    return 0;
}

/* `self+0x74` get/clear/OR-set accessors. */
s32 sub_800A068(void *self)
{
    return *(s32 *)((u8 *)self + 0x74);
}

/* `self+0x74 != 0`, via the branchless `(-x | x) >> 31` idiom rather
 * than a plain comparison. */
s32 sub_800A06C(void *self)
{
    s32 val = *(s32 *)((u8 *)self + 0x74);
    return (u32)(-val | val) >> 31;
}

void sub_800A078(void *self)
{
    *(s32 *)((u8 *)self + 0x74) = 0;
}

void sub_800A080(void *self, s32 val)
{
    *(s32 *)((u8 *)self + 0x74) |= val;
}

/* `self+0x68` byte get/set pair. */
void sub_800A088(void *self, u8 val)
{
    *((u8 *)self + 0x68) = val;
}

u8 sub_800A090(void *self)
{
    return *((u8 *)self + 0x68);
}

/* `self+0x64`/`self+0x60` setters. */
void sub_800A098(void *self, s32 val)
{
    *(s32 *)((u8 *)self + 0x64) = val;
}

void sub_800A09C(void *self, s32 val)
{
    *(s32 *)((u8 *)self + 0x60) = val;
}

/* `self+0x60`/`self+0x64` getters - the setters' siblings above. */
s32 sub_800A0A0(void *self)
{
    return *(s32 *)((u8 *)self + 0x60);
}

s32 sub_800A0A4(void *self)
{
    return *(s32 *)((u8 *)self + 0x64);
}

/* `self+0x44` (the keyframe/table record pointer used by
 * `sub_8009F1C`/`sub_8009FB0`/`sub_800A0AC`) getter. */
void *sub_800A0A8(void *self)
{
    return *(void **)((u8 *)self + 0x44);
}

extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);

/* Sets `self+0x44` to `rec`, then fires `rec->table+0x18/0x1c`'s
 * trampoline via `sub_803AD80` with `self` as the second argument.
 * Same `addr`-before-`fn` fix as `sub_8009F1C`/`sub_8009FB0`. */
void sub_800A0AC(void *self, void *rec)
{
    *(void **)((u8 *)self + 0x44) = rec;

    {
        register u8 *tbl asm("r2") = *(u8 **)((u8 *)rec + 0xc);
        register s32 offset asm("r1") = *(s16 *)(tbl + 0x18);
        register void *addr asm("r0");
        register void *fn asm("r2");

        addr = (u8 *)rec + offset;
        fn = *(void **)(tbl + 0x1c);
        sub_803AD80(addr, self, fn);
    }
}

/* `self+0x64`/`self+0x54`/`self+0x58`/`self+0x5c` bulk setter -
 * `self+0x64` and `self+0x54` both get the same first argument. */
void sub_800A0CC(void *self, s32 a, s32 b, s32 c)
{
    *(s32 *)((u8 *)self + 0x64) = a;
    *(s32 *)((u8 *)self + 0x54) = a;
    *(s32 *)((u8 *)self + 0x58) = b;
    *(s32 *)((u8 *)self + 0x5c) = c;
}

/* Same shape as `sub_800A0CC` above, without the `self+0x64` write. */
void sub_800A0D8(void *self, s32 a, s32 b, s32 c)
{
    *(s32 *)((u8 *)self + 0x54) = a;
    *(s32 *)((u8 *)self + 0x58) = b;
    *(s32 *)((u8 *)self + 0x5c) = c;
}

/* `self+0x60`/`self+0x48`/`self+0x4c`/`self+0x50` bulk setter - the
 * velocity/accel/max-velocity pair `sub_8009DF4` clamps, same
 * "shared first write" shape as `sub_800A0CC`. */
void sub_800A0E0(void *self, s32 a, s32 b, s32 c)
{
    *(s32 *)((u8 *)self + 0x60) = a;
    *(s32 *)((u8 *)self + 0x48) = a;
    *(s32 *)((u8 *)self + 0x4c) = b;
    *(s32 *)((u8 *)self + 0x50) = c;
}

/* Same shape as `sub_800A0E0` above, without the `self+0x60` write. */
void sub_800A0EC(void *self, s32 a, s32 b, s32 c)
{
    *(s32 *)((u8 *)self + 0x48) = a;
    *(s32 *)((u8 *)self + 0x4c) = b;
    *(s32 *)((u8 *)self + 0x50) = c;
}

/* `self+0x69` (cleared by `sub_8009F50`, set 0 by that same
 * initializer) getter. */
u8 sub_800A0F4(void *self)
{
    return *((u8 *)self + 0x69);
}
asm(".align 2, 0");
