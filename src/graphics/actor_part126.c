#include "core.h"

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
extern s32 sub_802B7E0(void *arg0);
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

/* Once-per-frame hazard/proximity state machine on the same `self`
 * object as `actor_part19*.c`: latches `self+0x2c` once `self+0x34`
 * (a cached depth) exceeds `0x15FF`. If not already "used"
 * (`self+0xc == 0`), snapshots the owning part table's own `+0x14`
 * 12-byte record into `self+0x38` and runs `sub_802DD9C`'s player
 * overlap test against it, transitioning to the "used" state (index 1)
 * on a hit; then, regardless, re-snapshots one of three static 12-byte
 * `gStaticData_0817A7xx` records into `self+0x38` and probes
 * `sub_802A6EC` against each in turn (the player's own
 * `sub_802B7E0`-mediated hazard for the first, plain proximity for the
 * other two), each on a hit also transitioning to "used". Once already
 * "used" (`self+0xc != 0`), skips all of that and just fires the
 * `self+0x50` trampoline (index 3) while `self+0x12` is set, or falls
 * back to `sub_802A7B8`.
 *
 * NAKED: every one of the three 12-byte record snapshots reuses the
 * same `self+0x38` scratch pointer (kept live in `r6`) across an
 * intervening `sub_802A6EC`/`sub_802DD9C` call, with `r5`/`r7` each
 * also switching roles (old state, then a "confirmed zero" reused for
 * every reset block's `self+0x44`/`self+8` clear) mid-function - the
 * exact "heavy r5/r6/r7 register reuse" shape
 * docs/matching/issue-53-actor-c7a8.md flagged as unattempted, and the
 * same categorical stack/register-reuse family already NAKED-parked
 * for `sub_802D7B0`/`sub_802DD9C` (docs/matching/issue-54-actor-
 * d3a8.md). Semantics are fully understood (see above); transcribed
 * instruction-for-instruction from the ROM disassembly rather than
 * chased further at the C level, per this project's established
 * escape hatch for this exact register-pressure family. */
NAKED void sub_802CC9C(void *selfArg)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r4, r0, #0\n\t"
        "ldr r1, [r4, #0x34]\n\t"
        "ldr r0, 4f\n\t"
        "cmp r1, r0\n\t"
        "ble 1f\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x2c\n\t"
        "mov r0, #1\n\t"
        "strb r0, [r1]\n\t"
    "1:\n\t"
        "ldr r7, [r4, #0xc]\n\t"
        "cmp r7, #0\n\t"
        "beq 2f\n\t"
        "b 13f\n\t"
    "2:\n\t"
        "ldr r0, [r4, #0x30]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, #0x38\n\t"
        "add r0, #0x14\n\t"
        "ldm r0!, {r2, r3, r5}\n\t"
        "stm r1!, {r2, r3, r5}\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802DD9C\n\t"
        "lsl r0, r0, #0x18\n\t"
        "add r6, r4, #0\n\t"
        "add r6, #0x38\n\t"
        "cmp r0, #0\n\t"
        "beq 3f\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #4\n\t"
        "bl PlaySfx\n\t"
        "mov r0, #1\n\t"
        "str r0, [r4, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r0, [r0, #0xc]\n\t"
        "mov r1, #0\n\t"
        "strh r0, [r4, #0x10]\n\t"
        "strb r1, [r4, #0x12]\n\t"
        "str r7, [r4, #8]\n\t"
    "3:\n\t"
        "add r0, r6, #0\n\t"
        "ldr r1, 6f\n\t"
        "ldm r1!, {r2, r3, r5}\n\t"
        "stm r0!, {r2, r3, r5}\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802A6EC\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r5, r0, #0x18\n\t"
        "cmp r5, #0\n\t"
        "beq 8f\n\t"
        "ldr r0, 7f\n\t"
        "ldr r0, [r0]\n\t"
        "bl sub_802B7E0\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 14f\n\t"
        "ldr r0, 5f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #4\n\t"
        "bl PlaySfx\n\t"
        "mov r0, #1\n\t"
        "str r0, [r4, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r0, [r0, #0xc]\n\t"
        "mov r1, #0\n\t"
        "strh r0, [r4, #0x10]\n\t"
        "strb r1, [r4, #0x12]\n\t"
        "str r7, [r4, #8]\n\t"
        "b 14f\n\t"
        ".align 2, 0\n"
    "4: .4byte 0x000015FF\n"
    "5: .4byte gUnknown_030012BC\n"
    "6: .4byte gStaticData_0817A78C\n"
    "7: .4byte gUnknown_03000884\n"
    "8:\n\t"
        "add r0, r6, #0\n\t"
        "ldr r1, 10f\n\t"
        "ldm r1!, {r2, r3, r7}\n\t"
        "stm r0!, {r2, r3, r7}\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802A6EC\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #4\n\t"
        "bl PlaySfx\n\t"
        "mov r0, #1\n\t"
        "str r0, [r4, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r0, [r0, #0xc]\n\t"
        "mov r1, #0\n\t"
        "strh r0, [r4, #0x10]\n\t"
        "strb r1, [r4, #0x12]\n\t"
        "str r5, [r4, #8]\n\t"
    "9:\n\t"
        "add r0, r6, #0\n\t"
        "ldr r1, 12f\n\t"
        "ldm r1!, {r2, r6, r7}\n\t"
        "stm r0!, {r2, r6, r7}\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802A6EC\n\t"
        "lsl r0, r0, #0x18\n\t"
        "cmp r0, #0\n\t"
        "beq 14f\n\t"
        "ldr r0, 11f\n\t"
        "ldr r0, [r0]\n\t"
        "mov r2, #0x80\n\t"
        "lsl r2, r2, #1\n\t"
        "mov r1, #4\n\t"
        "bl PlaySfx\n\t"
        "mov r0, #1\n\t"
        "str r0, [r4, #0xc]\n\t"
        "ldr r0, [r4]\n\t"
        "ldrh r0, [r0, #0xc]\n\t"
        "mov r1, #0\n\t"
        "strh r0, [r4, #0x10]\n\t"
        "strb r1, [r4, #0x12]\n\t"
        "str r5, [r4, #8]\n\t"
        "b 14f\n\t"
        ".align 2, 0\n"
    "10: .4byte gStaticData_0817A774\n"
    "11: .4byte gUnknown_030012BC\n"
    "12: .4byte gStaticData_0817A780\n"
    "13:\n\t"
        "ldrb r0, [r4, #0x12]\n\t"
        "cmp r0, #0\n\t"
        "beq 14f\n\t"
        "cmp r4, #0\n\t"
        "beq 15f\n\t"
        "ldr r1, [r4, #0x50]\n\t"
        "mov r3, #8\n\t"
        "ldrsh r0, [r1, r3]\n\t"
        "add r0, r4, r0\n\t"
        "ldr r2, [r1, #0xc]\n\t"
        "mov r1, #3\n\t"
        "bl sub_803AD80\n\t"
        "b 15f\n\t"
    "14:\n\t"
        "add r0, r4, #0\n\t"
        "bl sub_802A7B8\n\t"
    "15:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
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
