#include "core.h"
#include "memory.h"
#include "actor_self.h"
#include "actor_anim.h"

/* Branchless absolute value, matching this ROM's own codegen for `abs()`
 * (`asrs`/`eors`/`subs` on the value's own sign-extended shift, updating
 * the value in place) rather than a `?:`/`if`, which this compiler turns
 * into an actual branch. Written as sequential in-place updates (not a
 * single expression) so this compiler reuses the same register for the
 * whole sequence instead of materializing a fresh temporary. */
#define ABS32(x, sign) do { (sign) = (x) >> 0x1f; (x) ^= (sign); (x) -= (sign); } while (0)

/* The `InitActorPart`/`gActorList`-rooted "self" object family
 * already documented in actor_part17.c/actor_part18.c/actor_part19.c/
 * actor_part28.c/actor_part32.c: a "part table" pointer at `self+0`
 * (copied from the constructor's `part` argument's own `+4` field), a
 * table-index/"kind" field at `self+0xc`, an anim-frame halfword/byte
 * pair at `self+0x10`/`self+0x12`, an accumulator at `self+8`, state at
 * `self+0x28`, a frame counter at `self+0x44`, a `+0x50`-rooted event/
 * trampoline table fed through `_call_via_r2`, and the `+0x48`(next)/
 * `+0x4c`(prev) circular doubly-linked list rooted at the player-pointer
 * global `gActorList`. This file additionally pins down
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
 * position `DrawActor`'s sibling `sub_802AA0C` integrates
 * a per-axis velocity into. The object is `struct actor_self`
 * (actor_self.h: `x`/`y`/`z` are the cached `b`/`c`/`d`, `depth` and
 * `sortKey` the threshold pair) and the constructor's `part` a `struct
 * anim_table_record` (actor_anim.h). See
 * docs/matching/issue-50-actor-2a69c.md. */

extern struct actor_self *gActorList;
extern void AllocJetpackPlayerTiles(void *arg0);

/* Trivial forwarder - ignores its own argument and calls
 * `AllocJetpackPlayerTiles(gActorList)` (the player object), discarding its
 * return value. Same shape as `sub_802C0A8` in actor_part19.c. */
void JetpackReloadPlayerTiles(void *arg0)
{
    AllocJetpackPlayerTiles(gActorList);
}

extern void AllocPolarPlayerTiles(void *arg0);

/* Same forwarder shape as `JetpackReloadPlayerTiles`, calling `AllocPolarPlayerTiles` instead. */
void PolarReloadPlayerTiles(void *arg0)
{
    AllocPolarPlayerTiles(gActorList);
}

extern void sub_802F0DC(void *arg0);

/* Same forwarder shape as `JetpackReloadPlayerTiles`, calling `sub_802F0DC` instead. */
void sub_802A6C4(void *arg0)
{
    sub_802F0DC(gActorList);
}

extern void sub_802BFD4(void *arg0);

/* Same forwarder shape as `JetpackReloadPlayerTiles`, calling `sub_802BFD4` (already
 * matched as a no-argument function in actor_part19.c) with the player
 * pointer anyway - the callee simply ignores it. */
void sub_802A6D8(void *arg0)
{
    sub_802BFD4(gActorList);
}

extern void *gActorCategoryVtable;
extern s32 _call_via_r1(void *arg0, void *fn);

/* Passes its own `self` argument through to `_call_via_r1`, alongside a
 * function pointer read from `gActorCategoryVtable`'s own `+0x24` field
 * (`gActorCategoryVtable` is itself a pointer to some shared record). */
s32 IsTouchingPlayer(void *self)
{
    void *tab = gActorCategoryVtable;

    return _call_via_r1(self, *(void **)((u8 *)tab + 0x24));
}

extern u8 gActorVtable[];
extern void SetActorAnim(void *self, s32 arg1);
extern s32 sub_8029B2C(void);
extern s32 sub_8029E40(void);

/* The constructor every other `actor_part*.c` file already forward-
 * declares: seeds `self`'s part-table pointer (`+0`/`+4`, copied from
 * `part+4`/`part+8`) and header byte (`+0x18`, from `part+0xc`), resets
 * it via `SetActorAnim`, marks it "dead" (`+0x50 = gActorVtable`)
 * until a real event table is assigned later, caches the constructor's
 * own `part`/`b`/`c`/`d` arguments (`+0x30`/`+0x1c`/`+0x20`/`+0x24`),
 * copies a 12-byte vector block from `part+0x14..0x20` to `self+0x38`,
 * resets state/counter (`+0x28`/`+0x44 = 0`) and the one-shot flag
 * (`+0x2c = 1`), computes the movement-threshold pair (`+0x34`/`+0x14`,
 * see this file's header comment), and links `self` into the circular
 * `+0x48`/`+0x4c` list rooted at the player pointer `gActorList`
 * (or self-links it if that list is still empty). Returns `self`. */
void *InitActorPart(void *selfArg, void *partArg, s32 b, s32 c, s32 d)
{
    struct actor_self *self = selfArg;
    register struct anim_table_record *part asm("r4") = partArg;
    register s32 bReg asm("r5") = b;
    register s32 cReg asm("r6") = c;

    {
        u8 vC = part->palette;
        struct anim_frame_record *v4 = part->table_A;
        u32 *v8 = part->table_B;

        self->anims = v4;
        self->frameOffsets = v8;
        self->palette = vC;
    }

    SetActorAnim(self, 0);

    self->vtable = (struct actor_vtable *)gActorVtable;
    self->x = bReg;
    self->y = cReg;
    self->z = d;
    ACTOR_RECORD(self) = part;
    *(struct anim_box *)self->unk_38 = part->box_14;

    self->state = 0;
    self->stateTime = 0;
    self->unk_2C[0] = 1;

    {
        register s32 value asm("r2") = self->z - (sub_8029B2C() << 8);
        s32 sign;

        ABS32(value, sign);
        self->depth = value;
        value = (value >> 1) & 0x7f80;

        {
            s32 c = self->y;
            s32 cSign;
            s32 b, bSign;

            ABS32(c, cSign);
            b = self->x;
            ABS32(b, bSign);
            c = c + b;
            c >>= 0xb;
            c &= 0x7f;
            value |= c;
        }

        self->sortKey = value;

        if (self->depth > sub_8029E40()) {
            self->sortKey |= 0x8000;
        }
    }

    {
        struct actor_self *head = gActorList;

        if (head != NULL) {
            ACTOR_LINK_PREV(self) = head;
            ACTOR_LINK_NEXT(self) = ACTOR_LINK_NEXT(head);
            ACTOR_LINK_NEXT(head) = self;
            ACTOR_LINK_PREV(ACTOR_LINK_NEXT(self)) = self;
        } else {
            ACTOR_LINK_PREV(self) = self;
            ACTOR_LINK_NEXT(self) = self;
        }
    }

    return self;
}

extern s32 gUnknown_030013C4;
extern s32 gUnknown_030013C0;
extern s32 _call_via_r2(void *arg0, s32 arg1, void *fn);
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
void UpdateActor(void *selfArg)
{
    struct actor_self *self = selfArg;

    {
        register s32 value asm("r2") = self->z - (sub_8029B2C() << 8);
        s32 sign;

        ABS32(value, sign);
        self->depth = value;
        value = (value >> 1) & 0x7f80;

        {
            s32 c = self->y;
            s32 cSign;
            s32 b, bSign;

            ABS32(c, cSign);
            b = self->x;
            ABS32(b, bSign);
            c = c + b;
            c >>= 0xb;
            c &= 0x7f;
            value |= c;
        }

        self->sortKey = value;

        if (self->depth > sub_8029E40()) {
            self->sortKey |= 0x8000;
        }
    }

    if (self->depth > gUnknown_030013C4 + 0x200 ||
        self->depth < gUnknown_030013C0 - 0x200) {
        if (self != NULL) {
            struct actor_vtable *table = self->vtable;
            s32 offset = table->destroy.thisOffset;
            u8 *addr = (u8 *)self + offset;
            void *fn = table->destroy.fn;

            _call_via_r2(addr, 3, fn);
        }
        return;
    }

    self->stateTime += 1;
    self->animTime += *(s16 *)&self->animTimer;
    self->animDone = 0;

    {
        s32 frame = GetAnimFrameBaseOffset(self);
        s32 idx = self->animIndex;
        u8 *table = (u8 *)self->anims;
        s32 recordAddr = idx * 0xc;
        register u8 *record asm("r1");

        recordAddr += (s32)table;
        record = (u8 *)recordAddr;

        {
            s32 v4 = *(s16 *)(record + 4);

            if (frame >= v4) {
                s32 v6 = *(s16 *)(record + 6);

                self->animTime -= (v4 - v6) << 8;
                self->animDone = 1;
            }
        }
    }
}

asm(".align 2, 0");
