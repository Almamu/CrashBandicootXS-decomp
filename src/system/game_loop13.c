#include "core.h"

/* Sets bit `n` in *both* the second (`self+0x208`) and third
 * (`self+0x308`) bitmap arrays at once - see game_loop12.c's header
 * comment on this bitmap-array family.
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/naked-sub_80259d4-matched.md for the derivation of how
 * this was finally matched. This is a true leaf function in the ROM
 * (no `push`/`pop` at all - `self` lives in `ip`/`r12` for the whole
 * function). The gap: the ROM does `mov ip, r0` (stash `self`) before
 * `adds r2, r1, #0` (copy `n` into its own working register `t`), and
 * this compiler always emits the `n`-copy first regardless of C
 * source order - fixed by materializing both moves as one opaque
 * inline-asm block. The ROM also keeps `n`'s pristine copy (`t`, r2)
 * untouched by the "clamp negative indices" adjustment (which lands in
 * a *separate* register, r0), reusing the untouched `t` again later
 * for `bitIndex` - a second local (`adjusted`) instead of adjusting
 * `t` in place reproduces that split. */
void sub_80259D4(void *self, s32 n)
{
    register u8 *base asm("ip");
    register s32 t asm("r2");
    s32 adjusted, wordIndex;
    register s32 bitIndex asm("r0");
    register s32 mask asm("r2");
    register s32 shifted asm("r3");
    register s32 addr asm("r1");

    asm volatile("mov %0, %2\n\tadd %1, %3, #0" : "=r"(base), "=r"(t) : "r"(self), "r"(n));

    adjusted = t;
    if (t < 0) {
        adjusted += 0x1f;
    }
    wordIndex = adjusted >> 5;
    shifted = wordIndex << 2;

    addr = 0x208;
    addr += (s32)base;
    addr += shifted;
    bitIndex = t - (wordIndex << 5);
    mask = 1 << bitIndex;
    *(s32 *)addr |= mask;

    addr = 0x308;
    addr += (s32)base;
    addr += shifted;
    *(s32 *)addr |= mask;
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
