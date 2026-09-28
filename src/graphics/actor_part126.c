#include "core.h"
#include "actor_self.h"

/* Continues the `InitActorPart`/`gUnknown_03000884`-rooted "self" object
 * family (state at `self+0x28`, table-index/"kind" at `self+0xc`, an
 * anim-frame halfword/byte pair at `self+0x10`/`self+0x12`, an
 * accumulator at `self+8`, a `self+0x50`-rooted event/trampoline
 * table, and the position triple at `self+0x1c`/`self+0x20`/`self+0x24`)
 * already documented for `actor_part17.c`-`actor_part19i.c` and
 * `actor_part58.c`. Sits between `actor_part19i.c` (issue #53, ending
 * at `sub_802CC78`) and `actor_part62.c` (issue #54, starting at
 * `sub_802D3A8`) - the whole `0x0802CC9C`-`0x0802D3A8` gap
 * docs/matching/issue-53-actor-c7a8.md's "What's left" section
 * described as "a larger, sub_802DD9C/sub_802A6EC/sub_802B7E0-calling
 * state machine ... not attempted this pass". */

extern void *gUnknown_03000884;
extern void *gUnknown_030012BC;
extern void *gUnknown_030012C0;
extern s32 gUnknown_030014B8;
extern s32 gUnknown_0300088C[];

extern u8 sub_802A6EC(void *self);
extern u8 sub_802DD9C(void *self);
extern u8 sub_802B730(void *arg0);
extern u8 sub_802B7E0(void *arg0);
extern void sub_802A7B8(void *self);
extern void sub_802A980(void *self);
extern void PlaySfx(void *arg0, s32 sfxId, s32 volume);
extern void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d);
extern s32 GetAnimFrameBaseOffset(void *self);
extern s32 sub_803AD80(void *arg0, s32 arg1, void *arg2);
extern s32 sub_803ADB4(s32 a, s32 b);
extern s32 sub_8000E1C(s32 max);
extern s32 sub_80231EC(void *arg0, s32 arg1);
extern s32 QueueVramDmaTransfer(void *src, void *dest, u16 size, u16 unit);
extern s32 sub_802A570(s32 idx);
extern s32 sub_802A51C(s32 idx);
extern s32 sub_802A558(s32 idx);
extern s32 sub_802A540(s32 idx);
extern s32 sub_802A504(s32 idx);

extern u8 gStaticData_0817A78C[];
extern u8 gStaticData_0817A774[];
extern u8 gStaticData_0817A780[];
extern u8 gStaticData_0817A798[];
extern u8 gStaticData_0817A7D8[];
extern u8 gStaticData_0817A7B8[];
extern u8 gStaticData_087E4FB4[];
extern u8 gStaticData_087E4FD4[];
extern u8 gStaticData_087E4FF4[];
extern u8 gStaticData_087E5014[];
extern u8 gStaticData_087E5034[];

/* A 12-byte AABB record (the same shape actor_part19g.c copies as
 * `struct vec3_words`). */
struct box12 {
    s32 a, b, c;
};

struct hazard_part {
    u8 unk_00[0x14];
    struct box12 box;           // 0x14
};

/* actor_self with this class's own fields over its unk_ areas. */
struct hazard {
    struct anim_frame_record *anims; // 0x00
    u32 *frameOffsets;          // 0x04
    s32 animTime;               // 0x08
    s32 animIndex;              // 0x0C
    u16 animTimer;              // 0x10
    u8 animDone;                // 0x12
    u8 unk_13;
    s32 visible;                // 0x14
    s32 unk_18;
    s32 x;                      // 0x1C
    s32 y;                      // 0x20
    s32 z;                      // 0x24
    s32 state;                  // 0x28
    u8 deep;                    // 0x2C
    u8 unk_2D[3];
    struct hazard_part *part;   // 0x30
    s32 depth;                  // 0x34
    struct box12 box;           // 0x38
    s32 stateTime;              // 0x44
    u8 unk_48[8];
    struct actor_vtable *vtable; // 0x50
};

ACTOR_CALL_VIA_ALIASES

/* Plays the pickup sound and restarts animation sequence 1 ("used").
 * Wrapped in `if (1)` rather than `do { } while (0)` or an inline: both
 * of those change the block layout gcc emits. */
#define HAZARD_HIT(self)                                                       \
    if (1)                                                                     \
    {                                                                          \
        PlaySfx(gUnknown_030012BC, 4, 0x100);                                  \
        (self)->animIndex = 1;                                                 \
        (self)->animTimer = (self)->anims[1].duration;                         \
        (self)->animDone = 0;                                                  \
        (self)->animTime = 0;                                                  \
    } else (void)0

/* Once-per-frame hazard/proximity update. Latches `deep` once `depth`
 * passes 0x15FF. While unused (sequence 0) it tests the part table's own
 * box with sub_802DD9C, then gStaticData_0817A78C's box with sub_802A6EC
 * (a hit there only counts if sub_802B7E0 agrees), or failing that the
 * 0817A774 and 0817A780 boxes; any hit switches to sequence 1. Once used,
 * fires method 0x08 with 3 when the sequence has played through. */
void sub_802CC9C(void *selfArg)
{
    struct hazard *self = selfArg;

    if (self->depth > 0x15ff)
        self->deep = 1;

    if (self->animIndex == 0) {
        self->box = self->part->box;
        if (sub_802DD9C(self)) {
            HAZARD_HIT(self);
        }
        self->box = *(struct box12 *)gStaticData_0817A78C;
        if (sub_802A6EC(self)) {
            if (sub_802B7E0(gUnknown_03000884)) {
                HAZARD_HIT(self);
            }
        } else {
            self->box = *(struct box12 *)gStaticData_0817A774;
            if (sub_802A6EC(self)) {
                HAZARD_HIT(self);
            }
            self->box = *(struct box12 *)gStaticData_0817A780;
            if (sub_802A6EC(self)) {
                HAZARD_HIT(self);
            }
        }
    } else if (self->animDone) {
        if (self) {
            ACTOR_VCALL(self, m08, 3);
        }
        return;
    }
    sub_802A7B8(self);
}


/* `InitActorPart`-based constructor: forwards `a`/`b`/`c`/`d` straight
 * through, installs `self+0x50 = gStaticData_087E4FB4`, and clears the
 * `self+0x2c` one-shot flag. */
void *sub_802CDE4(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;

    InitActorPart(self, a, b, c, d);
    *(u8 **)(self + 0x50) = gStaticData_087E4FB4;
    self[0x2c] = 0;
    return self;
}

/* On the trampoline-fire edge (`sub_802A6EC`), forwards to
 * `sub_802B730(gUnknown_03000884)` (the player object), discarding its
 * result; always tail-calls `sub_802A7B8`. */
void sub_802CE10(void *selfArg)
{
    u8 *self = selfArg;

    if (sub_802A6EC(self)) {
        sub_802B730(gUnknown_03000884);
    }
    sub_802A7B8(self);
}

/* Same `InitActorPart`-based constructor shape as `sub_802CDE4`, minus
 * the `self+0x2c` clear, `self+0x50 = gStaticData_087E4FD4`. */
void *sub_802CE38(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;

    InitActorPart(self, a, b, c, d);
    *(u8 **)(self + 0x50) = gStaticData_087E4FD4;
    return self;
}

/* 3-way `self+0x28` state dispatch. State 0: on `sub_802A6EC`'s
 * trampoline-fire edge, calls `sub_802C14C(gUnknown_03000884)` (the
 * player object), plays a cue, and transitions to state 1/table-index
 * 1; otherwise, on `sub_802DD9C`'s player-overlap test, transitions the
 * same way. State 1: once `self+0x12` fires, dispatches the
 * `self+0x50` trampoline (index 3) instead of the usual
 * `sub_802A7B8` fallback. Any other state (and state 0/1's own
 * non-transition paths) falls through to `sub_802A7B8`. */
extern void sub_802C14C(void *selfArg);

void sub_802CE5C(void *selfArg)
{
    u8 *self = selfArg;
    register s32 state asm("r5") = *(s32 *)(self + 0x28);

    if (state == 0) {
        goto case0;
    }
    if (state == 1) {
        goto case1;
    }
    goto done;

case0:
    {
        register s32 fired asm("r6") = sub_802A6EC(self);

        if (fired) {
            sub_802C14C(gUnknown_03000884);
            PlaySfx(gUnknown_030012BC, 4, 0x100);
            *(s32 *)(self + 0x28) = 1;
            *(s32 *)(self + 0x44) = state;
            *(s32 *)(self + 0xc) = 1;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = state;
            goto done;
        }
        if (sub_802DD9C(self)) {
            PlaySfx(gUnknown_030012BC, 4, 0x100);
            *(s32 *)(self + 0x28) = 1;
            *(s32 *)(self + 0x44) = fired;
            *(s32 *)(self + 0xc) = 1;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = fired;
        }
    }
    goto done;

case1:
    if (self[0x12] != 0) {
        if (self != 0) {
            u8 *table = *(u8 **)(self + 0x50);
            sub_803AD80(self + *(s16 *)(table + 8), 3, *(void **)(table + 0xc));
        }
        return;
    }

done:
    sub_802A7B8(self);
}

/* Same `InitActorPart`-based constructor shape as `sub_802CE38`,
 * `self+0x50 = gStaticData_087E4FF4`. */
void *sub_802CF0C(void *selfArg, s32 a, s32 b, s32 c, s32 d)
{
    u8 *self = selfArg;

    InitActorPart(self, a, b, c, d);
    *(u8 **)(self + 0x50) = gStaticData_087E4FF4;
    return self;
}

/* Applies `self`'s own velocity (`self+0x54`/`0x58`/`0x5c`) to its
 * position, and while idle (`self+0x28 == 0`) counts down
 * `self+0x60`, re-deriving a fresh velocity/homing target via
 * `sub_802D044` once it expires. While idle, also probes
 * `sub_802A6EC`'s trampoline-fire edge against the player
 * (`sub_802B730`) or, failing that, `sub_802DD9C`'s player-overlap
 * test - either hit re-arms a fixed outward velocity (`self+0x54`
 * biased by `self+0x1c`'s sign), a random negative Y kick
 * (`self+0x58`), bumps `self+0x5c`, plays a cue, and transitions to
 * state 1/table-index 0. Always tail-calls `sub_802A7B8`. */
extern void sub_802D044(void *selfArg, s32 arg1);

void sub_802CF30(void *selfArg)
{
    u8 *self = selfArg;
    register s32 state asm("r6");

    *(s32 *)(self + 0x1c) += *(s32 *)(self + 0x54);
    *(s32 *)(self + 0x20) += *(s32 *)(self + 0x58);
    *(s32 *)(self + 0x24) += *(s32 *)(self + 0x5c);

    state = *(s32 *)(self + 0x28);
    if (state == 0) {
        s32 remain = *(s32 *)(self + 0x60) - 1;
        *(s32 *)(self + 0x60) = remain;
        if (remain <= 0) {
            sub_802D044(self, *(s32 *)(self + 0x64));
        }

        {
            register s32 fired asm("r5") = sub_802A6EC(self);

            if (fired) {
                if (sub_802B730(gUnknown_03000884)) {
                    s32 velX = (*(s32 *)(self + 0x1c) > 0) ? 0x600 : 0xFFFFFA00;

                    *(s32 *)(self + 0x54) = velX;
                    *(s32 *)(self + 0x58) = -(s32)(u16)sub_8000E1C(0x300);
                    *(s32 *)(self + 0x5c) += 0x200;
                    PlaySfx(gUnknown_030012BC, 5, 0x100);
                    *(s32 *)(self + 0x28) = 1;
                    *(s32 *)(self + 0x44) = state;
                    *(s32 *)(self + 0xc) = state;
                    {
                        register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
                        register u8 zero1 asm("r1") = 0;

                        *(u16 *)(self + 0x10) = anim;
                        self[0x12] = zero1;
                    }
                    *(s32 *)(self + 8) = state;
                }
            } else if (sub_802DD9C(self)) {
                s32 velX = (*(s32 *)(self + 0x1c) > 0) ? 0x600 : 0xFFFFFA00;

                *(s32 *)(self + 0x54) = velX;
                *(s32 *)(self + 0x58) = -(s32)(u16)sub_8000E1C(0x300);
                *(s32 *)(self + 0x5c) += 0x200;
                PlaySfx(gUnknown_030012BC, 5, 0x100);
                *(s32 *)(self + 0x28) = 1;
                *(s32 *)(self + 0x44) = fired;
                *(s32 *)(self + 0xc) = fired;
                {
                    register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
                    register u8 zero1 asm("r1") = 0;

                    *(u16 *)(self + 0x10) = anim;
                    self[0x12] = zero1;
                }
                *(s32 *)(self + 8) = fired;
            }
        }
    }

    sub_802A7B8(self);
}

/* Homing-velocity (re)initializer: with a negative `target` index,
 * arms a fixed slow downward drift (`self+0x54/0x58/0x5c/0x60` set to
 * constants). Otherwise derives a per-frame speed factor
 * (`sub_803ADB4` of `target`'s own "speed" record,
 * `gUnknown_0300088C[sub_802A570(target)]`, against the remaining
 * distance in Z) and scales the X/Y deltas toward `target`'s own
 * tracked position (`sub_802A558`/`sub_802A540`) by that factor,
 * caching the new countdown in `self+0x60` (floored at 1) and
 * `target`'s own Z record in `self+0x64`. */
void sub_802D044(void *selfArg, s32 target)
{
    u8 *self = selfArg;

    if (target < 0) {
        *(s32 *)(self + 0x58) = 0;
        *(s32 *)(self + 0x54) = 0;
        *(s32 *)(self + 0x5c) = 0x62;
        *(s32 *)(self + 0x60) = 0x40000000;
    } else {
        s32 idx = sub_802A570(target);
        s32 speed = gUnknown_0300088C[idx];
        s32 factor;
        s32 countdown;

        *(s32 *)(self + 0x5c) = speed;
        countdown = sub_803ADB4(sub_802A51C(target) - *(s32 *)(self + 0x24), *(s32 *)(self + 0x5c));
        *(s32 *)(self + 0x60) = countdown;
        if (countdown == 0) {
            *(s32 *)(self + 0x60) = 1;
        }

        {
            register s32 countdown2 asm("r1") = *(s32 *)(self + 0x60);
            register s32 lit asm("r0") = 0x1000;

            factor = sub_803ADB4(lit, countdown2);
        }
        *(s32 *)(self + 0x54) = factor * (sub_802A558(target) - *(s32 *)(self + 0x1c)) >> 0xc;
        *(s32 *)(self + 0x58) = factor * (sub_802A540(target) - *(s32 *)(self + 0x20)) >> 0xc;
        *(s32 *)(self + 0x64) = sub_802A504(target);
    }
}

/* `InitActorPart`-based constructor, forwarding `a`/`b`/`c`/`d`
 * straight through plus a 6th argument `e` (a pointer whose `+0x10`
 * field feeds `sub_802D044`'s homing target): installs
 * `self+0x50 = gStaticData_087E5014`, then calls
 * `sub_802D044(self, e->0x10)`. */
void *sub_802D0C8(void *selfArg, s32 a, s32 b, s32 c, s32 d, void *e)
{
    u8 *self = selfArg;

    InitActorPart(self, a, b, c, d);
    *(u8 **)(self + 0x50) = gStaticData_087E5014;
    sub_802D044(self, *(s32 *)((u8 *)e + 0x10));
    return self;
}

/* On the trampoline-fire edge, forwards to `sub_802B730` on the player
 * object (discarding its result), then, gated on `self+0x34`'s cached
 * depth crossing one of two thresholds paired with `self+0x28`'s
 * current tier, advances `self+0xc`'s table index and, once
 * `GetAnimFrameBaseOffset` reaches the new record's own threshold,
 * clears `self+8` (deep-tier variant clears it unconditionally with
 * the pre-increment tier value instead) and bumps `self+0x28`. */
void sub_802D0F4(void *selfArg)
{
    u8 *self = selfArg;
    register s32 threshold1 asm("r0");
    register s32 depth asm("r1");

    if (sub_802A6EC(self)) {
        sub_802B730(gUnknown_03000884);
    }

    threshold1 = 0x6400;
    depth = *(s32 *)(self + 0x34);

    if ((depth > threshold1 && *(s32 *)(self + 0x28) == 2)
        || (depth > 0x5A00 && *(s32 *)(self + 0x28) == 1)) {
        s32 frame;
        register s32 idx asm("r1") = *(s32 *)(self + 0xc) + 1;

        *(s32 *)(self + 0xc) = idx;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + idx * 0xc);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        frame = GetAnimFrameBaseOffset(self);
        {
            register s32 idx2 asm("r2") = *(s32 *)(self + 0xc);
            register u8 *table2 asm("r3") = *(u8 **)self;
            register s32 threshold asm("r1") = *(s16 *)(table2 + idx2 * 0xc + 4);

            if (frame >= threshold) {
                *(s32 *)(self + 8) = 0;
            }
        }
        goto increment;
    } else if (depth > 0x5000) {
        register s32 zero asm("r5") = *(s32 *)(self + 0x28);

        if (zero == 0) {
            s32 frame;
            register s32 idx asm("r1") = *(s32 *)(self + 0xc) + 1;

            *(s32 *)(self + 0xc) = idx;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + idx * 0xc);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            frame = GetAnimFrameBaseOffset(self);
            {
                register s32 idx2 asm("r2") = *(s32 *)(self + 0xc);
                register u8 *table2 asm("r3") = *(u8 **)self;
                register s32 threshold asm("r1") = *(s16 *)(table2 + idx2 * 0xc + 4);

                if (frame >= threshold) {
                    *(s32 *)(self + 8) = zero;
                }
            }
            goto increment;
        }
    }

    goto tail;

increment:
    *(s32 *)(self + 0x28) = *(s32 *)(self + 0x28) + 1;

tail:
    sub_802A7B8(self);
}

/* `InitActorPart`-based constructor: forwards `self`/`d` straight
 * through, passing `b` (a `u8 *`, cast to `s32` for `InitActorPart`'s
 * own generic third argument) and `c` unchanged; installs
 * `self+0x50 = gStaticData_087E5034`, then classifies a "kind"
 * (`self+0xc`) from `b`'s own first byte (bumped by 1 if `c > 0`),
 * scaled `*4 - 0x40`, to seed `self+0x10`/`self+0x12`/`self+8` from the
 * part table. */
void *sub_802D1B8(void *selfArg, u8 *b, s32 c, s32 d, s32 e)
{
    u8 *self = selfArg;
    register s32 kind asm("r1");

    InitActorPart(self, (s32)b, c, d, e);
    *(u8 **)(self + 0x50) = gStaticData_087E5034;

    kind = *b;
    if (c > 0) {
        kind += 1;
    }
    kind = kind * 4 - 0x40;
    *(s32 *)(self + 0xc) = kind;
    {
        register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + kind * 3 * 4);
        register u8 zero1 asm("r1") = 0;
        register s32 zero2 asm("r2") = 0;

        *(u16 *)(self + 0x10) = anim;
        self[0x12] = zero1;
        *(s32 *)(self + 8) = zero2;
    }
    return self;
}

/* VRAM-gauge/state-transition driver for a `gUnknown_030014B8`-counted
 * effect: while the current hazard tier (`gUnknown_030012C0->0x78`)
 * and the `retrigger` flag are both zero, just clears `self+0x2c`;
 * otherwise DMAs one of four `gStaticData_0817A798`-indexed gauge
 * strips and resets `self`'s table index/anim, arming `self+0x2c`.
 * Then: tier 3 arms a long `gUnknown_030014B8` countdown and
 * transitions to state 1; tier 0 with `retrigger` set transitions to
 * state 2/table-index 1 instead; any other combination just clears
 * `gUnknown_030014B8` and, if `self+0x28` was already non-zero, resets
 * `self` back to state 0/table-index 0. */
void sub_802D204(void *selfArg, s32 retriggerParam)
{
    u8 *self = selfArg;
    u8 retrigger = (u8)retriggerParam;
    register s32 tier asm("r5") = *(s32 *)((u8 *)gUnknown_030012C0 + 0x78);

    if (tier == 0 && retrigger == 0) {
        self[0x2c] = tier;
    } else {
        register s32 zero asm("r6");

        QueueVramDmaTransfer(gStaticData_0817A798 + (tier - 1) * 0x20, (void *)0x050003C0, 0x20, 0x10);
        {
            u8 *addr = self + 0x2c;

            zero = 0;
            *addr = 1;
        }
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        {
            s32 frame = GetAnimFrameBaseOffset(self);
            s32 idx = *(s32 *)(self + 0xc);
            u8 *table = *(u8 **)self;

            if (frame >= *(s16 *)(table + idx * 0xc + 4)) {
                *(s32 *)(self + 8) = zero;
            }
        }
    }

    if (tier == 3) {
        {
            register s32 *addr asm("r1") = &gUnknown_030014B8;
            register s32 val asm("r0") = 0x1F4;

            *addr = val;
        }
        {
            register s32 state asm("r0") = 1;
            register s32 zero asm("r2") = 0;

            *(s32 *)(self + 0x28) = state;
            *(s32 *)(self + 0x44) = zero;
            *(s32 *)(self + 0xc) = zero;
            {
                register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
                register u8 zero1 asm("r1") = 0;

                *(u16 *)(self + 0x10) = anim;
                self[0x12] = zero1;
            }
            *(s32 *)(self + 8) = zero;
        }
        return;
    } else if (tier == 0 && retrigger != 0) {
        register s32 two asm("r0");
        register s32 one asm("r1");

        gUnknown_030014B8 = tier;
        two = 2;
        one = 1;
        *(s32 *)(self + 0x28) = two;
        *(s32 *)(self + 0x44) = tier;
        *(s32 *)(self + 0xc) = one;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self + 0xc);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = tier;
        return;
    } else {
        register s32 *addr asm("r0") = &gUnknown_030014B8;
        register s32 zero asm("r2") = 0;

        *addr = zero;
        if (*(s32 *)(self + 0x28) == 0) {
            return;
        }
        *(s32 *)(self + 0x28) = zero;

        *(s32 *)(self + 0x44) = zero;
        *(s32 *)(self + 0xc) = zero;
        {
            register u16 anim asm("r0") = *(u16 *)(*(u8 **)self);
            register u8 zero1 asm("r1") = 0;

            *(u16 *)(self + 0x10) = anim;
            self[0x12] = zero1;
        }
        *(s32 *)(self + 8) = zero;
    }
}

/* Drives `gUnknown_030014B8`'s countdown, DMAing one of two gauge
 * strips per frame (`gStaticData_0817A7D8` on the low bit set,
 * `gStaticData_0817A7B8` otherwise) and, once it expires, resetting
 * the hazard tier via `sub_80231EC(gUnknown_030012C0, 2)` then
 * `sub_802D204(self, 0)`. Independently re-fires `sub_802D204` once
 * state 2's own `self+0x12` edge trips. Always advances `self`'s own
 * anim frame (`sub_802A980`, frame-counter bump, and the usual
 * wrap-around `GetAnimFrameBaseOffset` check). */
void sub_802D2DC(void *selfArg)
{
    u8 *self = selfArg;

    if (gUnknown_030014B8 != 0) {
        if (gUnknown_030014B8 & 4) {
            QueueVramDmaTransfer(gStaticData_0817A7D8, (void *)0x050003C0, 0x20, 0x10);
        } else {
            QueueVramDmaTransfer(gStaticData_0817A7B8, (void *)0x050003C0, 0x20, 0x10);
        }

        gUnknown_030014B8 -= 1;
        if (gUnknown_030014B8 == 0) {
            sub_80231EC(gUnknown_030012C0, 2);
            sub_802D204(self, 0);
        }
    }

    if (*(s32 *)(self + 0x28) == 2 && self[0x12] != 0) {
        sub_802D204(self, 0);
    }

    sub_802A980(self);
    *(s32 *)(self + 0x44) += 1;
    *(s32 *)(self + 8) += *(s16 *)(self + 0x10);
    self[0x12] = 0;

    {
        s32 frame = GetAnimFrameBaseOffset(self);
        register s32 idx2 asm("r2") = *(s32 *)(self + 0xc);
        register u8 *table2 asm("r3") = *(u8 **)self;
        register u8 *record asm("r1") = (u8 *)(idx2 * 0xc);

        asm("add %0, %0, %1" : "+r" (record) : "r" (table2));
        {
            register s32 threshold asm("r2") = *(s16 *)(record + 4);

            if (frame >= threshold) {
                register s32 diff asm("r0") = (threshold - *(s16 *)(record + 6)) << 8;

                *(s32 *)(self + 8) -= diff;
                self[0x12] = 1;
            }
        }
    }
}

asm(".align 2, 0");
