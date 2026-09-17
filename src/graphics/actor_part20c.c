#include "core.h"

/* GitHub issue #22, ROM 0x080187FC-0x08018884 - non-adjacent to
 * actor_part20b.c since the raw `sub_8018008`/`sub_8018400`/
 * `sub_801865C`/`sub_80186F0` block sits between them (see
 * asm/code_3_2_17_18008.s). `obj`'s `+8` word is a small 0/1/2 state
 * counter and `+0xc` is the usual per-category table pointer (same
 * `self+0xc` convention as actor_part20.c/20b.c). `other`/`part`'s
 * `+0x38`/`+0xc`/`+8` fields match the same `struct actor`-shaped
 * header used by every other "part" object in this ROM region
 * (compare `sub_8007DBC`'s `part->flags`/`part->field_08` bitmap-set
 * in actor_part2.c) - kept as raw offsets rather than `struct actor`
 * itself since this object is bigger than the 0x1c-byte `struct actor`
 * (its own `+0x38` byte is read directly here), matching the same
 * "three objects, none fully pinned down" caution documented in
 * actor_part18.c. */

extern u8 gStaticData_087E442C[];
extern void *gUnknown_03001308;
extern void *gUnknown_030012B4;
extern s32 sub_803AD84(void *addr, void *arg1, void *arg2, void *fn);
extern void sub_800B8A8(void *self, s32 flags);
extern void sub_800B8C8(void *self);

/* A two-state (`obj+8`: 0 then 1 then 2) "charge" handler. State 0
 * fires the usual table-trampoline pair (action 8) and advances to
 * state 1. State 1 accumulates `+0x400` per call into `other+4` until
 * it reaches `(gUnknown_03001308's sub-object's +0x14 word << 8) +
 * 0x2000`, then advances to state 2 (a "fully charged" terminal
 * state this function no longer touches).
 *
 * Written with explicit `goto`s (see docs/workflow.md's "force block
 * ordering" note) since a plain `if (state == 1) {...} else if (state
 * > 1) {} else if (state == 0) {...}` compiles correctly but with the
 * state-1/state-0 bodies physically swapped from the ROM's actual
 * layout - this compiler inverts the branch and reorders the blocks
 * even though nothing about that changes the emitted-instruction count
 * or the C's meaning. */
void sub_80187FC(void *objArg, void *otherArg)
{
    register u8 *obj asm("r3") = objArg;
    register u8 *other asm("r2") = otherArg;
    s32 state = *(s32 *)(obj + 8);

    if (state == 1)
        goto case1;
    if (state > 1)
        goto end;
    if (state != 0)
        goto end;

    {
        u8 *table;
        s16 offset;
        void *addr;
        void *fn;

        *(s32 *)(obj + 8) = 1;
        table = *(u8 **)(obj + 0xc);
        table += 0x50;
        offset = *(s16 *)table;
        addr = obj + offset;
        fn = *(void **)(table + 4);
        sub_803AD84(addr, other, (void *)8, fn);
    }
    goto end;

case1:
    {
        s32 timer = *(s32 *)(other + 4) + 0x400;
        u8 *subObj;
        s32 threshold;

        *(s32 *)(other + 4) = timer;
        subObj = *(u8 **)((u8 *)gUnknown_03001308 + 0x10);
        threshold = (*(s32 *)(subObj + 0x14) << 8) + 0x2000;
        if (timer >= threshold) {
            *(s32 *)(obj + 8) = 2;
        }
    }
end:
    ;
}

/* Sets `self+0xc`'s table pointer to `gStaticData_087E442C`, then
 * tail-calls `sub_800B8A8` - same double-set pattern as
 * `sub_8017A78`/`sub_8017FD4`. */
void sub_8018858(void *selfArg, s32 flags)
{
    u8 *self = selfArg;

    *(void **)(self + 0xc) = gStaticData_087E442C;
    sub_800B8A8(self, flags);
}

/* Resets via `sub_800B8C8`, then re-points the table at
 * `gStaticData_087E442C`. Returns `self`. */
void *sub_801886C(void *selfArg)
{
    u8 *self = selfArg;

    sub_800B8C8(self);
    *(void **)(self + 0xc) = gStaticData_087E442C;
    return self;
}

/* While `other+0x38` is set: ORs bit 0 into `other+0xc`'s flags, then
 * (unless `other+8`'s id is the sentinel `0xFFFF`) sets bit
 * `other+8 & 0x1f` in the `gUnknown_030012B4+0x108` word-indexed
 * bitmap - the same bitmap-set idiom `sub_8007DBC` uses via
 * `part->field_08`. The first argument is taken but never read
 * anywhere in this function's ROM body.
 *
 * Needs several `register ... asm("rN")` pins (matching the ROM's own
 * register choices) plus a `volatile` reload of `other+8` and one raw
 * `asm` for the index shift - without them this compiler happily
 * proves `other+8`'s zero-extended value never has its top bit set and
 * folds the ROM's `asrs`/`adds` shift-setup pair into a single `lsr`,
 * and CSEs away the ROM's second, seemingly redundant `ldrh` reload of
 * the same address (needed there only because the ROM's register
 * allocator picks a different register for the value on each side of
 * the branch). */
void sub_8018884(void *unusedArg, void *otherArg)
{
    register u8 *other asm("r1") = otherArg;

    (void)unusedArg;

    if (other[0x38] != 0) {
        register s32 one asm("r0") = 1;
        register u8 flags asm("r2") = other[0xc];

        one |= flags;
        other[0xc] = one;

        {
            register s32 sentinel asm("r0") = 0xFFFF;
            register u16 val asm("r4") = *(u16 *)(other + 8);

            if (val != sentinel) {
                register u16 val2 asm("r3") = *(u16 volatile *)(other + 8);
                register u8 *base asm("r2") = gUnknown_030012B4;
                register s32 idx asm("r0");
                s32 idxOffset;
                s32 *bitmap;
                register s32 bit asm("r0");

                asm("add %0, %1, #0\n\tasr %0, %0, #5" : "=r" (idx) : "r" (val2));
                idxOffset = idx * 4;
                bitmap = (s32 *)(base + 0x108);
                bitmap = (s32 *)((u8 *)bitmap + idxOffset);
                bit = val2 - (idx << 5);
                *bitmap |= 1 << bit;
            }
        }
    }
}
