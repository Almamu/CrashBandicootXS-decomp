#include "core.h"
#include "memory.h"

/* Continues the `InitActorPart`/`gUnknown_03000884`-rooted "self" object
 * family documented in actor_part50.c/actor_part19.c: a "part table"
 * pointer at `self+0`, a table-index/"kind" field at `self+0xc`, an
 * anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, state at `self+0x28`, a frame counter at
 * `self+0x44`, and the movement-threshold-cached position triple at
 * `self+0x1c`/`self+0x20`/`self+0x24`. This file's functions are mostly
 * `InitActorPart`-calling constructor variants (each installing a
 * different `self+0x50` event/trampoline table before doing a small
 * amount of table-specific setup) plus a couple of small self-standing
 * helpers operating on the unrelated `gUnknown_030012C0`-rooted "player"
 * object's `+0x78` counter field (an Aku-Aku-mask-style add/remove
 * pair, `sub_802D4B0`/`sub_802D4EC`) and a `gUnknown_03001494`-rooted
 * sibling object (`sub_802D490`). See docs/matching/issue-54-actor-d3a8.md. */

extern void *gUnknown_030012C0;
extern void *gUnknown_030012BC;
extern s32 sub_80231EC(void *arg0, s32 arg1);
extern void sub_802D204(void *self, s32 arg1);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void *InitActorPart(void *self, void *part, s32 b, s32 c, s32 d);
extern u8 sub_802A6EC(void *self);
extern void sub_802A7B8(void *self);
extern void *gUnknown_03000884;
extern void sub_802BFD4(void *arg0);
extern void sub_802C0BC(void *selfArg, s32 arg1);
extern u8 sub_802DD9C(void *self);
extern void sub_8022FEC(void *self);
extern s32 sub_8029748(s32 arg0);
extern void sub_802B12C(s32 arg0, s32 arg1, s32 arg2);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *fn);
extern s32 sub_802973C(void);

extern u8 gStaticData_087E5054[];
extern u8 gStaticData_087E5074[];
extern u8 gStaticData_087E5094[];
extern u8 gStaticData_087E50B4[];

/* Passes its argument through to `sub_80231EC(gUnknown_030012C0, 0)`,
 * then `sub_802D204(self, 0)` - a trivial reset pair on a different,
 * `gUnknown_03001494`-rooted object family, unrelated to this file's
 * `self` (see `actor_part19.c`'s `sub_802C018`, which calls this with
 * `gUnknown_03001494`). */
void sub_802D490(void *self)
{
    sub_80231EC(gUnknown_030012C0, 0);
    sub_802D204(self, 0);
}

/* "Remove a mask": plays a sound, decrements `gUnknown_030012C0`'s
 * `+0x78` counter (floored at 0), pushes the new count via
 * `sub_80231EC`, and always calls `sub_802D204(self, 1)`. Returns the
 * (possibly unchanged) counter. */
s32 sub_802D4B0(void *self)
{
    s32 count;

    PlaySfx(gUnknown_030012BC, 0, 0x100);
    count = *(s32 *)((u8 *)gUnknown_030012C0 + 0x78);
    if (count != 0) {
        count -= 1;
        sub_80231EC(gUnknown_030012C0, count);
    }
    sub_802D204(self, 1);
    return count;
}

/* "Add a mask" - the increment counterpart to `sub_802D4B0` above,
 * capped at 3, `sub_802D204(self, 0)` instead of `1`. */
s32 sub_802D4EC(void *self)
{
    s32 count;

    PlaySfx(gUnknown_030012BC, 1, 0x100);
    count = *(s32 *)((u8 *)gUnknown_030012C0 + 0x78);
    if (count != 3) {
        count += 1;
        sub_80231EC(gUnknown_030012C0, count);
    }
    sub_802D204(self, 0);
    return count;
}

/* Constructor variant: calls `InitActorPart` with `b`/`c`/`d` offset by
 * fixed deltas (`-0x1000`/`-0x1E00`/`-0x200`, the same constants
 * `sub_802D3A8` uses for its own state-0 scatter targets - plausibly a
 * "spawn at scatter offset" helper feeding that function), installs the
 * `gStaticData_087E5054` event table, then pushes the caller's own
 * 6th argument through `sub_80231EC` before resetting state via
 * `sub_802D204(self, 0)`. */
void *sub_802D528(void *self, void *part, s32 b, s32 c, s32 d, s32 sixth)
{
    InitActorPart(self, part, b - 0x1000, c - 0x1E00, d - 0x200);
    *(u8 **)((u8 *)self + 0x50) = gStaticData_087E5054;
    sub_80231EC(gUnknown_030012C0, sixth);
    sub_802D204(self, 0);
    return self;
}

/* Trivial forwarder: `sub_80231EC(gUnknown_030012C0, arg1)`, where
 * `arg1` is this function's own second parameter, passed straight
 * through in `r1` (the same "ignore my own first argument, forward my
 * second" shape as `sub_802BFD4` in actor_part50.c). */
void sub_802D57C(void *arg0, s32 arg1)
{
    sub_80231EC(gUnknown_030012C0, arg1);
}

/* Trivial getter: `gUnknown_030012C0`'s `+0x78` counter (the same field
 * `sub_802D4B0`/`sub_802D4EC` above adjust). */
s32 sub_802D590(void)
{
    return *(s32 *)((u8 *)gUnknown_030012C0 + 0x78);
}

/* Once `self`'s frame counter (`+0x44`) exceeds 5, latches the one-shot
 * flag at `+0x2c`. Then, on `sub_802A6EC`'s trampoline-fire edge, calls
 * `sub_802BFD4(gUnknown_03000884)` (the player object), and always
 * advances via `sub_802A7B8`. */
void sub_802D59C(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;

    if (*(s32 *)(self + 0x44) > 5) {
        self[0x2c] = 1;
    }

    if (sub_802A6EC(self)) {
        sub_802BFD4(gUnknown_03000884);
    }

    sub_802A7B8(self);
}

/* Plain `InitActorPart` passthrough constructor (no offset applied)
 * installing the `gStaticData_087E5074` event table and clearing the
 * one-shot flag at `+0x2c` (rather than setting it, unlike
 * `InitActorPart`'s own default of `1`). */
void *sub_802D5D4(void *self, void *part, s32 b, s32 c, s32 d)
{
    InitActorPart(self, part, b, c, d);
    *(u8 **)((u8 *)self + 0x50) = gStaticData_087E5074;
    ((u8 *)self)[0x2c] = 0;
    return self;
}

/* On `sub_802A6EC`'s trampoline-fire edge, forwards `self+0x1c` to
 * `sub_802C0BC(gUnknown_03000884, ...)` (the player object) and, the
 * first time through (guarded by a one-shot byte flag at `self+0x54`),
 * plays a sound. Always advances via `sub_802A7B8`. */
void sub_802D600(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;

    if (sub_802A6EC(self)) {
        sub_802C0BC(gUnknown_03000884, *(s32 *)(self + 0x1c));
        {
            u8 *flag = self + 0x54;

            if (*flag == 0) {
                PlaySfx(gUnknown_030012BC, 0x28, 0x100);
                *flag = 1;
            }
        }
    }

    sub_802A7B8(self);
}

/* Constructor: `InitActorPart` passthrough (`b`==own `posY` argument,
 * reused below), installs `gStaticData_087E5094`, then classifies
 * `posY>>8` into a 3-way "kind" (`self+0xc`: 0 if `< -0x14`, 1 if
 * `<= 0x14`, else 2) that selects which of the part table's three
 * 0xc-stride anim records seeds `self+0x10`/`0x12`, and resets the
 * accumulator (`+8`) and the `+0x54` one-shot flag both to 0. */
void *sub_802D648(void *self, void *part, s32 posY, s32 c, s32 d)
{
    s32 classify = posY;
    s32 idx;

    InitActorPart(self, part, posY, c, d);
    *(u8 **)((u8 *)self + 0x50) = gStaticData_087E5094;

    classify >>= 8;

    {
        s32 low = -0x14;

        idx = 0;
        if (classify >= low) {
            idx = 1;
            if (classify > 0x14) {
                idx = 2;
            }
        }
    }

    *(s32 *)((u8 *)self + 0xc) = idx;
    {
        u8 *table = *(u8 **)self;
        u16 anim = *(u16 *)(table + idx * 0xc);
        register u8 zeroShared asm("r2") = 0;
        register s32 zeroAccum asm("r1") = 0;

        *(u16 *)((u8 *)self + 0x10) = anim;
        ((u8 *)self)[0x12] = zeroShared;
        *(s32 *)((u8 *)self + 8) = zeroAccum;
        ((u8 *)self)[0x54] = zeroShared;
    }

    return self;
}

/* State machine: while `self+0xc` ("kind") is still 0, first checks
 * `sub_802A6EC`'s trampoline-fire edge (transitions to kind 1, seeds
 * anim from the part table's `+0xc` record, plays a sound, refreshes
 * the player via `sub_8022FEC`, and fires `sub_8029748`/`sub_802B12C`
 * position-tied calls), then - only if still kind 0 - checks
 * `sub_802DD9C`'s AABB-overlap test (transitions to kind 3, seeds anim
 * from the `+0x24` record, arms `self+0x18`, plays a different sound,
 * and refreshes the player again). Finally, once kind==3 and the
 * anim-done flag (`+0x12`) is set, fires the `+0x50` table's slot-3
 * trampoline (guarded by a redundant `self != NULL` check matching the
 * ROM); otherwise advances via `sub_802A7B8`. */
void sub_802D6A0(void *selfArg)
{
    register u8 *self asm("r4") = selfArg;
    s32 kind = *(s32 *)(self + 0xc);

    if (kind == 0) {
        if (sub_802A6EC(self)) {
            *(s32 *)(self + 0xc) = 1;
            {
                u8 *table = *(u8 **)self;
                u16 anim = *(u16 *)(table + 0xc);
                register u8 zero asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero;
            }
            *(s32 *)(self + 8) = kind;

            PlaySfx(gUnknown_030012BC, 0x17, 0x100);
            sub_8022FEC(gUnknown_030012C0);
            sub_8029748(*(s32 *)(self + 0x24));
            sub_802B12C(*(s32 *)(self + 0x1c), *(s32 *)(self + 0x20) - 0xF00, *(s32 *)(self + 0x24));
        }

        kind = *(s32 *)(self + 0xc);
        if (kind == 0 && sub_802DD9C(self)) {
            *(s32 *)(self + 0xc) = 3;
            {
                u8 *table = *(u8 **)self;
                u16 anim = *(u16 *)(table + 0x24);
                register u8 zero asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero;
            }
            *(s32 *)(self + 8) = kind;
            *(s32 *)(self + 0x18) = 1;

            PlaySfx(gUnknown_030012BC, 3, 0x100);
            sub_8022FEC(gUnknown_030012C0);
        }
    }

    if (*(s32 *)(self + 0xc) == 3 && self[0x12] != 0) {
        if (self != NULL) {
            u8 *table = *(u8 **)(self + 0x50);
            s32 offset = *(s16 *)(table + 8);
            u8 *addr = self + offset;
            void *fn = *(void **)(table + 0xc);

            sub_803AD80(addr, 3, fn);
        }
    } else {
        sub_802A7B8(self);
    }
}

/* Constructor: `InitActorPart` passthrough installing
 * `gStaticData_087E50B4`, then - only if `sub_802973C()` equals the
 * caller's own `d` argument - transitions to kind 2 (anim from the part
 * table's `+0x18` record, accumulator/flag/counter all reset) and plays
 * a sound. */
void *sub_802D764(void *self, void *part, s32 b, s32 c, s32 d)
{
    InitActorPart(self, part, b, c, d);
    *(u8 **)((u8 *)self + 0x50) = gStaticData_087E50B4;

    if (sub_802973C() == d) {
        *(s32 *)((u8 *)self + 0xc) = 2;
        {
            u8 *table = *(u8 **)self;
            u16 anim = *(u16 *)(table + 0x18);
            register u8 zero1 asm("r1") = 0;
            register s32 zero2 asm("r2") = 0;

            *(u16 *)((u8 *)self + 0x10) = anim;
            ((u8 *)self)[0x12] = zero1;
            *(s32 *)((u8 *)self + 8) = zero2;
        }

        PlaySfx(gUnknown_030012BC, 0x17, 0x100);
    }

    return self;
}

asm(".align 2, 0");
