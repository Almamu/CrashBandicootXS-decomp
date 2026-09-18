#include "core.h"

/* Same `InitActorPart`-rooted per-instance "self" object family already
 * documented in actor_part28.c/actor_part32.c/actor_part50.c: a "part
 * table" pointer at `self+0`, a table-index/"kind" field at `self+0xc`,
 * an anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, state at `self+0x28`, a frame counter at
 * `self+0x44`, and a `+0x50`-rooted event/trampoline table fed through
 * `sub_803AD80`. This particular object kind (constructed here by
 * `sub_8033EF4`, vtable `gStaticData_087E551C`) additionally caches its
 * own constructor `b`/`c` arguments at `self+0x58`/`self+0x5c` and has a
 * death/"dead" byte flag at `self+0x6c` - see
 * docs/matching/issue-63-0x08033ef4-actor.md. */

extern void *InitActorPart(void *selfArg, void *part, s32 b, s32 c, s32 d);
extern u8 gStaticData_087E551C[];

/* Constructor: forwards straight through to `InitActorPart`, then sets
 * `self`'s health (`+0x54=0x19`), event/trampoline table
 * (`+0x50=&gStaticData_087E551C`), caches its own `b`/`c` constructor
 * args at `+0x58`/`+0x5c`, and resets state/frame-counter/table-index
 * (`+0x28`/`+0x44`/`+0xc=0`), the anim-frame pair from the part table's
 * first entry, the accumulator (`+8=0`) and the death flag
 * (`+0x6c=0`). Returns `self`. */
void *sub_8033EF4(void *selfArg, void *part, s32 b, s32 cParam, s32 d)
{
    u8 *self = selfArg;
    register s32 bReg asm("r6") = b;
    register s32 c asm("r8") = cParam;
    register s32 dReg asm("r0") = d;
    register s32 health asm("r5") = 0x19;

    InitActorPart(self, part, b, cParam, dReg);
    *(s32 *)(self + 0x54) = health;
    *(void **)(self + 0x50) = gStaticData_087E551C;
    *(s32 *)(self + 0x58) = bReg;
    *(s32 *)(self + 0x5c) = c;
    {
        register s32 zero asm("r1") = 0;

        *(s32 *)(self + 0x28) = zero;
        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)*(void **)self;
            register u8 zero2 asm("r2") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero2;
            *(s32 *)(self + 8) = zero;
            self[0x6c] = zero2;
        }
    }

    return self;
}

extern void sub_8029E28(s32 arg0);
extern s32 sub_803AD80(void *arg0, void *arg1, void *fn);

/* Plays a fixed sound cue (`sub_8029E28(0x400)`), then - if `self` is
 * non-NULL and its `+0x12` flag is set - fires the `self+0x50` event
 * table's slot-3 trampoline at `self` offset by the table's `+8`
 * halfword. Same shape as `sub_8033BFC` (actor_part32.c). */
void sub_8033F48(void *selfArg)
{
    u8 *self = selfArg;

    sub_8029E28(0x400);

    if (self[0x12] != 0 && self != NULL) {
        register u8 *table asm("r1") = *(u8 **)(self + 0x50);
        register u8 *addr asm("r0");
        void *fn;

        {
            register s32 eight asm("r2") = 8;

            addr = self + *(s16 *)((u8 *)table + eight);
        }
        fn = *(void **)(table + 0xc);

        sub_803AD80(addr, (void *)3, fn);
    }
}

extern s32 sub_8033900(void);
extern s32 sub_80338F4(void);
extern s32 sub_80338E8(void);
extern s32 sub_8033880(void);
extern s32 sub_80338D0(void);

/* Syncs `self`'s position fields (`+0x1c`/`+0x20`/`+0x24`) from the
 * `gUnknown_030015AC` singleton's own position plus a fixed offset, and
 * - while the singleton's lifetime counter (`sub_8033880`) is still
 * under 3, and the singleton's own animation "kind" (`sub_80338D0`) is
 * either 2, or 3 with `self+0x34` still under its `0x4AFF` threshold -
 * switches `self` to state 1/table-index 1, resetting the anim-frame
 * pair from the part table's `+0xc` entry and clearing the
 * accumulator/frame counter. */
void sub_8033F74(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;

    *(s32 *)(self + 0x1c) = sub_8033900() + 0x1E00;
    *(s32 *)(self + 0x20) = sub_80338F4() - 0x3000;
    *(s32 *)(self + 0x24) = sub_80338E8() - 0x100;

    if (sub_8033880() <= 2
     && (sub_80338D0() == 2
      || (sub_80338D0() == 3 && *(s32 *)(self + 0x34) <= 0x4AFF))) {
        register s32 zero asm("r2") = 0;
        register s32 one asm("r0");

        *(s32 *)(self + 0x64) = zero;
        *(s32 *)(self + 0x68) = zero;
        one = 1;
        *(s32 *)(self + 0x28) = one;
        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0xc) = one;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
            register u8 zero2 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero2;
        }
        *(s32 *)(self + 8) = zero;
    }
}
