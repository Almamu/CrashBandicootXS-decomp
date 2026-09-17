#include "core.h"
#include "memory.h"

/* Branchless absolute value, matching this ROM's own codegen for `abs()`
 * (`asrs`/`eors`/`subs` on the value's own sign-extended shift, updating
 * the value in place) rather than a `?:`/`if`, which this compiler turns
 * into an actual branch. Written as sequential in-place updates (not a
 * single expression) so this compiler reuses the same register for the
 * whole sequence instead of materializing a fresh temporary. */
#define ABS32(x, sign) do { (sign) = (x) >> 0x1f; (x) ^= (sign); (x) -= (sign); } while (0)

/* The `InitActorPart`/`gUnknown_03000884`-rooted "self" object family
 * already documented in actor_part17.c/actor_part18.c/actor_part19.c/
 * actor_part28.c/actor_part32.c: a "part table" pointer at `self+0`
 * (copied from the constructor's `part` argument's own `+4` field), a
 * table-index/"kind" field at `self+0xc`, an anim-frame halfword/byte
 * pair at `self+0x10`/`self+0x12`, an accumulator at `self+8`, state at
 * `self+0x28`, a frame counter at `self+0x44`, a `+0x50`-rooted event/
 * trampoline table fed through `sub_803AD80`, and the `+0x48`(next)/
 * `+0x4c`(prev) circular doubly-linked list rooted at the player-pointer
 * global `gUnknown_03000884`. This file additionally pins down
 * `InitActorPart` itself (the constructor every other actor_part*.c file
 * already forward-declares and calls) plus a handful of new fields it
 * introduces: the constructor's raw `part`/`b`/`c`/`d` arguments cached
 * at `self+0x30`/`self+0x1c`/`self+0x20`/`self+0x24`, a "movement"
 * threshold pair at `self+0x14`/`self+0x34` (an absolute-value/packed-
 * bitfield distance metric compared against `gUnknown_030013C0`/
 * `gUnknown_030013C4`, gating whether the object fires its `+0x50`
 * table's slot-3 trampoline instead of animating), a one-shot byte flag
 * at `self+0x2c`, and a 12-byte little vector block at `self+0x38`
 * (copied from `part+0x14..0x20`) whose first three `s16` slots are a
 * position `UpdateAnimatedActorPart`'s sibling `sub_802AA0C` integrates
 * a per-axis velocity into. None of these objects' full shapes are
 * pinned down yet, so every access stays a raw offset with a doc comment
 * rather than a guessed struct, matching the established convention for
 * this object family. See docs/matching/issue-50-actor-2a69c.md. */

extern void *gUnknown_03000884;
extern void sub_802F338(void *arg0);

/* Trivial forwarder - ignores its own argument and calls
 * `sub_802F338(gUnknown_03000884)` (the player object), discarding its
 * return value. Same shape as `sub_802C0A8` in actor_part19.c. */
void sub_802A69C(void *arg0)
{
    sub_802F338(gUnknown_03000884);
}

extern void sub_802B864(void *arg0);

/* Same forwarder shape as `sub_802A69C`, calling `sub_802B864` instead. */
void sub_802A6B0(void *arg0)
{
    sub_802B864(gUnknown_03000884);
}

extern void sub_802F0DC(void *arg0);

/* Same forwarder shape as `sub_802A69C`, calling `sub_802F0DC` instead. */
void sub_802A6C4(void *arg0)
{
    sub_802F0DC(gUnknown_03000884);
}

extern void sub_802BFD4(void *arg0);

/* Same forwarder shape as `sub_802A69C`, calling `sub_802BFD4` (already
 * matched as a no-argument function in actor_part19.c) with the player
 * pointer anyway - the callee simply ignores it. */
void sub_802A6D8(void *arg0)
{
    sub_802BFD4(gUnknown_03000884);
}

extern void *gUnknown_03001418;
extern s32 sub_803AD7C(void *arg0, void *fn);

/* Passes its own `self` argument through to `sub_803AD7C`, alongside a
 * function pointer read from `gUnknown_03001418`'s own `+0x24` field
 * (`gUnknown_03001418` is itself a pointer to some shared record). */
s32 sub_802A6EC(void *self)
{
    void *tab = gUnknown_03001418;

    return sub_803AD7C(self, *(void **)((u8 *)tab + 0x24));
}

extern u8 gStaticData_087E4DF4[];
extern void sub_803B0A8(void *self, s32 arg1);
extern s32 sub_8029B2C(void);
extern s32 sub_8029E40(void);

/* The constructor every other `actor_part*.c` file already forward-
 * declares: seeds `self`'s part-table pointer (`+0`/`+4`, copied from
 * `part+4`/`part+8`) and header byte (`+0x18`, from `part+0xc`), resets
 * it via `sub_803B0A8`, marks it "dead" (`+0x50 = gStaticData_087E4DF4`)
 * until a real event table is assigned later, caches the constructor's
 * own `part`/`b`/`c`/`d` arguments (`+0x30`/`+0x1c`/`+0x20`/`+0x24`),
 * copies a 12-byte vector block from `part+0x14..0x20` to `self+0x38`,
 * resets state/counter (`+0x28`/`+0x44 = 0`) and the one-shot flag
 * (`+0x2c = 1`), computes the movement-threshold pair (`+0x34`/`+0x14`,
 * see this file's header comment), and links `self` into the circular
 * `+0x48`/`+0x4c` list rooted at the player pointer `gUnknown_03000884`
 * (or self-links it if that list is still empty). Returns `self`. */
void *InitActorPart(void *selfArg, void *partArg, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;
    register u8 *part asm("r4") = partArg;
    register s32 bReg asm("r5") = b;
    register s32 cReg asm("r6") = c;

    {
        u8 vC = part[0xc];
        u32 v4 = *(u32 *)(part + 4);
        u32 v8 = *(u32 *)(part + 8);

        *(u32 *)self = v4;
        *(u32 *)(self + 4) = v8;
        *(u32 *)(self + 0x18) = vC;
    }

    sub_803B0A8(self, 0);

    *(u8 **)(self + 0x50) = gStaticData_087E4DF4;
    *(s32 *)(self + 0x1c) = bReg;
    *(s32 *)(self + 0x20) = cReg;
    *(s32 *)(self + 0x24) = d;
    *(u8 **)(self + 0x30) = part;

    {
        struct blob0xc { u32 w0, w1, w2; };

        *(struct blob0xc *)(self + 0x38) = *(struct blob0xc *)(part + 0x14);
    }

    *(s32 *)(self + 0x28) = 0;
    *(s32 *)(self + 0x44) = 0;
    self[0x2c] = 1;

    {
        register s32 value asm("r2") = *(s32 *)(self + 0x24) - (sub_8029B2C() << 8);
        s32 sign;

        ABS32(value, sign);
        *(s32 *)(self + 0x34) = value;
        value = (value >> 1) & 0x7f80;

        {
            s32 c = *(s32 *)(self + 0x20);
            s32 cSign;
            s32 b, bSign;

            ABS32(c, cSign);
            b = *(s32 *)(self + 0x1c);
            ABS32(b, bSign);
            c = c + b;
            c >>= 0xb;
            c &= 0x7f;
            value |= c;
        }

        *(s32 *)(self + 0x14) = value;

        if (*(s32 *)(self + 0x34) > sub_8029E40()) {
            *(s32 *)(self + 0x14) |= 0x8000;
        }
    }

    {
        u8 *head = gUnknown_03000884;

        if (head != NULL) {
            *(u8 **)(self + 0x4c) = head;
            *(u8 **)(self + 0x48) = *(u8 **)(head + 0x48);
            *(u8 **)(head + 0x48) = self;
            *(u8 **)(*(u8 **)(self + 0x48) + 0x4c) = self;
        } else {
            *(u8 **)(self + 0x4c) = self;
            *(u8 **)(self + 0x48) = self;
        }
    }

    return self;
}

extern s32 gUnknown_030013C4;
extern s32 gUnknown_030013C0;
extern s32 sub_803AD80(void *arg0, s32 arg1, void *fn);
extern s32 GetAnimFrameBaseOffset(void *self);

/* Recomputes `self`'s movement-threshold pair (`+0x34`/`+0x14`, same
 * formula as `InitActorPart`, using `self`'s own already-stored `+0x24`
 * in place of the constructor's `d` argument). If the result falls
 * outside `[gUnknown_030013C0-0x200, gUnknown_030013C4+0x200]`, fires
 * the `+0x50` event table's slot-3 trampoline instead of animating;
 * otherwise advances the frame counter (`+0x44`), applies the current
 * anim-frame delta (`+0x10`) to the position accumulator (`+8`), and -
 * once the animation's base offset reaches the current keyframe's `+4`
 * threshold - rewinds `+8` by the keyframe's `+4`/`+6` delta and marks
 * `+0x12` "done". */
void sub_802A7B8(void *selfArg)
{
    u8 *self = selfArg;

    {
        register s32 value asm("r2") = *(s32 *)(self + 0x24) - (sub_8029B2C() << 8);
        s32 sign;

        ABS32(value, sign);
        *(s32 *)(self + 0x34) = value;
        value = (value >> 1) & 0x7f80;

        {
            s32 c = *(s32 *)(self + 0x20);
            s32 cSign;
            s32 b, bSign;

            ABS32(c, cSign);
            b = *(s32 *)(self + 0x1c);
            ABS32(b, bSign);
            c = c + b;
            c >>= 0xb;
            c &= 0x7f;
            value |= c;
        }

        *(s32 *)(self + 0x14) = value;

        if (*(s32 *)(self + 0x34) > sub_8029E40()) {
            *(s32 *)(self + 0x14) |= 0x8000;
        }
    }

    if (*(s32 *)(self + 0x34) > gUnknown_030013C4 + 0x200 ||
        *(s32 *)(self + 0x34) < gUnknown_030013C0 - 0x200) {
        if (self != NULL) {
            u8 *table = *(u8 **)(self + 0x50);
            s32 offset = *(s16 *)(table + 8);
            u8 *addr = self + offset;
            void *fn = *(void **)(table + 0xc);

            sub_803AD80(addr, 3, fn);
        }
        return;
    }

    *(s32 *)(self + 0x44) += 1;
    *(s32 *)(self + 8) += *(s16 *)(self + 0x10);
    self[0x12] = 0;

    {
        s32 frame = GetAnimFrameBaseOffset(self);
        s32 idx = *(s32 *)(self + 0xc);
        u8 *table = *(u8 **)self;
        s32 recordAddr = idx * 0xc;
        register u8 *record asm("r1");

        recordAddr += (s32)table;
        record = (u8 *)recordAddr;

        {
            s32 v4 = *(s16 *)(record + 4);

            if (frame >= v4) {
                s32 v6 = *(s16 *)(record + 6);

                *(s32 *)(self + 8) -= (v4 - v6) << 8;
                self[0x12] = 1;
            }
        }
    }
}

asm(".align 2, 0");
