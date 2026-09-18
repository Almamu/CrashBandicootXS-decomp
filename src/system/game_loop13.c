#include "core.h"

/* Sets bit `n` in *both* the second (`self+0x208`) and third
 * (`self+0x308`) bitmap arrays at once - see game_loop12.c's header
 * comment on this bitmap-array family.
 *
 * NAKED, not plain C: this is a true leaf function in the ROM (no
 * `push`/`pop` at all - `self` lives in `ip`/`r12` for the whole
 * function, freed up specifically because nothing here calls out). A
 * real C reconstruction (see git history) gets every instruction's
 * operation, operand, and register matching one-for-one *except* the
 * first two: the ROM does `mov ip, r0` (stash `self`) before `adds r2,
 * r1, #0` (copy `n` into its working register), and this compiler
 * always emits the `n`-copy first regardless of C statement order,
 * declaration order, or an explicit `asm volatile` ordering barrier
 * between the two - a fixed early-reload ordering for hard-register
 * parameter moves this compiler doesn't expose a way to influence from
 * C (pinning both values to their exact ROM registers up front just
 * reintroduces an unwanted `push {r4, lr}`/`pop {r4}` pair instead, per
 * the earlier attempts in docs/matching/issue-41-game-loop-25894.md).
 * Transcribed straight from the confirmed-correct ROM disassembly. */
NAKED void sub_80259D4(void *self, s32 n)
{
    asm(
        "mov ip, r0\n\t"
        "add r2, r1, #0\n\t"
        "add r0, r2, #0\n\t"
        "cmp r2, #0\n\t"
        "bge 1f\n\t"
        "add r0, r0, #0x1f\n\t"
    "1:\n\t"
        "asr r0, r0, #5\n\t"
        "lsl r3, r0, #2\n\t"
        "mov r1, #0x82\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, ip\n\t"
        "add r1, r1, r3\n\t"
        "lsl r0, r0, #5\n\t"
        "sub r0, r2, r0\n\t"
        "mov r2, #1\n\t"
        "lsl r2, r0\n\t"
        "ldr r0, [r1]\n\t"
        "orr r0, r2\n\t"
        "str r0, [r1]\n\t"
        "mov r1, #0xc2\n\t"
        "lsl r1, r1, #2\n\t"
        "add r1, ip\n\t"
        "add r1, r1, r3\n\t"
        "ldr r0, [r1]\n\t"
        "orr r0, r2\n\t"
        "str r0, [r1]\n\t"
        "bx lr\n\t"
        ".align 2, 0\n"
    );
}

/* Sets bit `n` of the third bitmap array, at `self+0x308` - see
 * game_loop12.c's header comment on this bitmap-array family. */
void sub_8025A0C(void *self, s32 n)
{
    u8 *base = (u8 *)self;
    s32 t = n;
    s32 wordIndex, shifted, bitIndex, mask;
    s32 *word;

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;
    base += 0x308;
    word = (s32 *)(base + shifted);
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;

    *word |= mask;
}

/* Stores `val >> 8` (a Q8-to-int truncation) into `self+4`. */
void sub_8025A3C(void *self, s32 val)
{
    *(s32 *)((u8 *)self + 4) = val >> 8;
}

extern void sub_8026ED0(void *self);

/* If bit 0 of `flags` is set, forwards to `sub_8026ED0` - same
 * conditional-destroy shape as `sub_8025D54`/game_loop4.c's
 * near-identical function. */
void sub_8025A44(void *self, s32 flags)
{
    if (flags & 1) {
        sub_8026ED0(self);
    }
}

/* Zeroes the two Q8 position words at `self+0`/`self+4`. */
void sub_8025A5C(void *self)
{
    *(s32 *)self = 0;
    *(s32 *)((u8 *)self + 4) = 0;
}
