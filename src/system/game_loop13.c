#include "core.h"

#if NON_MATCHING
/* Sets bit `n` in *both* the second (`self+0x208`) and third
 * (`self+0x308`) bitmap arrays at once - see game_loop12.c's header
 * comment on this bitmap-array family.
 *
 * PARKED, NOT BYTE-MATCHING: this is a true leaf function in the ROM
 * (no `push`/`pop` at all - `self` lives in `ip`/`r12` for the whole
 * function, freed up specifically because nothing here calls out).
 * Pinning `self` to `ip`, and - unlike the earlier attempt recorded in
 * docs/matching/issue-41-game-loop-25894.md - also pinning the
 * `shifted`/`addr` locals to `r3`/`r1` (matching the ROM's own choice
 * for those two) gets this down to a true leaf function with every
 * instruction's operation, operand, and register matching one-for-one
 * *except* the first two: the ROM does `mov ip, r0` (stash `self`)
 * before `adds r2, r1, #0` (copy `n` into its working register `t`),
 * this compiler always emits the `n`-copy first regardless of C
 * statement order, declaration order, or an explicit `asm volatile`
 * ordering barrier between them - seemingly a fixed early-reload
 * ordering for hard-register parameter moves this compiler doesn't
 * expose a way to influence from C. Pinning `t` itself to `r2` (the
 * ROM's register for it) reintroduces a `push {r4, lr}`/`pop {r4}`
 * pair instead (conflicts with `mask` needing that same register
 * later), so parked with `t` left unpinned - two swapped instructions,
 * same total size, everything else byte-for-byte. */
void sub_80259D4(void *self, s32 n)
{
    register u8 *base asm("ip") = (u8 *)self;
    s32 t = n;
    s32 wordIndex, bitIndex, mask;
    register s32 shifted asm("r3");
    register s32 addr asm("r1");

    if (t < 0) {
        t += 0x1f;
    }
    wordIndex = t >> 5;
    shifted = wordIndex << 2;

    addr = 0x208;
    addr += (s32)base;
    addr += shifted;
    bitIndex = n - (wordIndex << 5);
    mask = 1 << bitIndex;
    *(s32 *)addr |= mask;

    addr = 0x308;
    addr += (s32)base;
    addr += shifted;
    *(s32 *)addr |= mask;
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

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
