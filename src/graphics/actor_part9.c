#include "core.h"
#include "gobj_1a794.h"

/* Same `record->table+0x10/0x14`-driven trampoline shape as
 * `sub_8009F1C`/`sub_8009FB0`, but forwarding `arg1`/`arg2`/`arg3`
 * straight through as `sub_803AD88`'s own arg1-arg3 instead of
 * building them locally. The `table+0x14` function pointer read is a
 * genuine "dead read" - loaded into `r4` but never actually passed to
 * `sub_803AD88` (a plain 4-argument function, not itself a trampoline)
 * - the same idiom already confirmed and documented for
 * `sub_8007DBC`'s own `sub_803AD88` call in `actor_part2.c`. */
void sub_8009FD4(struct gobj *self, s32 arg1, s32 arg2, s32 arg3)
{
    struct mover *rec = self->mover;

    if (rec != 0) {
        struct mover_vtable *tbl = rec->vtable;
        void *addr = (u8 *)rec + tbl->m10.thisOffset;
        register void *deadRead asm("r4") = *(void *volatile *)&tbl->m10.fn;
        (void)deadRead;

        sub_803AD88(addr, arg1, arg2, arg3);
    }
}

extern void *sub_8007C30(void *dest, void *pt);
extern void *sub_8007CF8(void *dest, void *pt);

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


/* Fires a `self->table+0x70/0x74`-driven trampoline via `sub_803AD7C`
 * (same `table+N`/`table+N+4` convention used throughout this ROM
 * region) and always returns 0. Needed the trampoline's `addr = self
 * + offset` computed before the `fn` load (reusing the adjusted table
 * pointer's own dying register for `fn`), the same accumulator-style
 * fix established for `sub_8009F1C`/`sub_8009FB0` above. */
s32 sub_800A050(struct gobj *self)
{
    register u8 *tblAdj asm("r1") = (u8 *)self->vtable + 0x70;
    register s32 offset asm("r2") = *(s16 *)tblAdj;
    register void *addr asm("r0");
    register void *fn asm("r1");

    addr = (u8 *)self + offset;
    fn = *(void **)(tblAdj + 4);
    sub_803AD7C(addr, fn);
    return 0;
}

/* `self+0x74` get/clear/OR-set accessors. */
s32 sub_800A068(struct gobj *self)
{
    return self->unk_74;
}

/* `self+0x74 != 0`, via the branchless `(-x | x) >> 31` idiom rather
 * than a plain comparison. */
s32 sub_800A06C(struct gobj *self)
{
    s32 val = self->unk_74;
    return (u32)(-val | val) >> 31;
}

void sub_800A078(struct gobj *self)
{
    self->unk_74 = 0;
}

void sub_800A080(struct gobj *self, s32 val)
{
    self->unk_74 |= val;
}

/* `self+0x68` byte get/set pair. */
void sub_800A088(struct gobj *self, u8 val)
{
    self->unk_68 = val;
}

u8 sub_800A090(struct gobj *self)
{
    return self->unk_68;
}

/* `self+0x64`/`self+0x60` setters. */
void sub_800A098(struct gobj *self, s32 val)
{
    self->speedY = val;
}

void sub_800A09C(struct gobj *self, s32 val)
{
    self->speedX = val;
}

/* `self+0x60`/`self+0x64` getters - the setters' siblings above. */
s32 sub_800A0A0(struct gobj *self)
{
    return self->speedX;
}

s32 sub_800A0A4(struct gobj *self)
{
    return self->speedY;
}

/* `self+0x44` (the keyframe/table record pointer used by
 * `sub_8009F1C`/`sub_8009FB0`/`sub_800A0AC`) getter. */
struct mover *sub_800A0A8(struct gobj *self)
{
    return self->mover;
}

extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);

/* Sets `self+0x44` to `rec`, then fires `rec->table+0x18/0x1c`'s
 * trampoline via `sub_803AD80` with `self` as the second argument.
 * Same `addr`-before-`fn` fix as `sub_8009F1C`/`sub_8009FB0`. */
void sub_800A0AC(struct gobj *self, struct mover *rec)
{
    self->mover = rec;

    {
        register struct mover_vtable *tbl asm("r2") = rec->vtable;
        register s32 offset asm("r1") = tbl->m18.thisOffset;
        register void *addr asm("r0");
        register void *fn asm("r2");

        addr = (u8 *)rec + offset;
        fn = tbl->m18.fn;
        sub_803AD80(addr, self, fn);
    }
}

/* `self+0x64`/`self+0x54`/`self+0x58`/`self+0x5c` bulk setter -
 * `self+0x64` and `self+0x54` both get the same first argument. */
void sub_800A0CC(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->speedY = a;
    self->velB.x = a;
    self->velB.y = b;
    self->velB.z = c;
}

/* Same shape as `sub_800A0CC` above, without the `self+0x64` write. */
void sub_800A0D8(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->velB.x = a;
    self->velB.y = b;
    self->velB.z = c;
}

/* `self+0x60`/`self+0x48`/`self+0x4c`/`self+0x50` bulk setter - the
 * velocity/accel/max-velocity pair `sub_8009DF4` clamps, same
 * "shared first write" shape as `sub_800A0CC`. */
void sub_800A0E0(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->speedX = a;
    self->velA.x = a;
    self->velA.y = b;
    self->velA.z = c;
}

/* Same shape as `sub_800A0E0` above, without the `self+0x60` write. */
void sub_800A0EC(struct gobj *self, s32 a, s32 b, s32 c)
{
    self->velA.x = a;
    self->velA.y = b;
    self->velA.z = c;
}

/* `self+0x69` (cleared by `sub_8009F50`, set 0 by that same
 * initializer) getter. */
u8 sub_800A0F4(struct gobj *self)
{
    return self->unk_69[0];
}
asm(".align 2, 0");
