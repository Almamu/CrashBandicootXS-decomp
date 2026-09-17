#include "core.h"
#include "gba/defines.h"

/* Trivial ROM leaf shared by the three division/modulo routines below
 * as their divide-by-zero handler. Real ROM bytes are `mov pc, lr`
 * (not `bx lr`) - agbcc's plain-C codegen for an empty function picks
 * `bx lr` here instead (observed directly; the routines below, which
 * DO have a real body, naturally emit `mov pc, lr` for their own
 * returns), so this one needs to be hand-written via NAKED to get the
 * exact instruction. */
NAKED void nullsub_8(void)
{
    asm("mov pc, lr");
}
asm(".align 2, 0");

#if NON_MATCHING
/* Signed integer division (a / b), truncating toward zero - the
 * generic "atan2-style" math primitive referenced throughout
 * docs/rom_map.md/matching.md (see math_util.c's sub_800090C/
 * sub_8000924/sub_80008F0 wrappers, and sub_8027940's digit-splitting
 * use). Classic shift-and-subtract binary long division, 4 bits at a
 * time: normalizes `divisor`/`bit` up to the dividend's magnitude,
 * then repeatedly tests the top 4 candidate bit positions before
 * shrinking by another nibble.
 *
 * NOT YET BYTE-MATCHING, prologue/epilogue only: every single
 * instruction in the function BODY matches the ROM exactly once
 * `dividend`/`divisor`/`quotient`/`bit`/`mask` are pinned to
 * r0/r1/r2/r3/r4 respectively and `sign` to `ip` (avoids a 5th
 * push-requiring register - the ROM stores the a^b sign there too,
 * confirmed by its own `mov ip, r4`/`mov r4, ip` pair). What doesn't
 * match: the ROM's entry/exit sequence is TWO physically different
 * shapes depending on path - `push {r4}` (no `lr`) only on the
 * fallthrough (b != 0) path, ending in a direct `mov pc, lr` (this
 * path never calls anything, so lr is never touched); a completely
 * separate, minimal `push {lr} ... bl nullsub_8 ... pop {pc}` only on
 * the b == 0 path. This is real per-path register-save minimization
 * (effectively shrink-wrapping) that agbcc - based on gcc 2.9, long
 * before shrink-wrapping existed in mainline gcc - does not do from
 * plain C no matter how the `if` is phrased (early-return at the top,
 * inverted-condition with the trivial case falling through at the
 * bottom, both tried): it always emits one `push`/`pop` pair sized for
 * every register used anywhere in the function, at the single function
 * entry/exit. Reproducing the ROM's exact split would require
 * hand-writing the whole function as NAKED asm, throwing away a
 * genuine, fully body-matching C reconstruction for a structural gap
 * that isn't really about this function's logic - parked instead. */
s32 sub_803ADB4(s32 a, s32 b)
{
    register u32 dividend asm("r0");
    register u32 divisor asm("r1");
    register u32 quotient asm("r2");
    register u32 bit asm("r3");
    register u32 mask asm("r4");
    register s32 sign asm("ip");

    if (b == 0) {
        nullsub_8();
        return 0;
    }

    sign = a ^ b;
    bit = 1;
    quotient = 0;
    divisor = b;
    if ((s32)divisor >= 0) {
    } else {
        divisor = -divisor;
    }
    dividend = a;
    if ((s32)dividend >= 0) {
    } else {
        dividend = -dividend;
    }

    if (dividend >= divisor) {
        mask = 1u << 0x1c;

        while (divisor < mask && divisor < dividend) {
            divisor <<= 4;
            bit <<= 4;
        }
        mask <<= 3;
        while (divisor < mask && divisor < dividend) {
            divisor <<= 1;
            bit <<= 1;
        }

        do {
            if (dividend >= divisor) {
                dividend -= divisor;
                quotient |= bit;
            }
            mask = divisor >> 1;
            if (dividend >= mask) {
                dividend -= mask;
                mask = bit >> 1;
                quotient |= mask;
            }
            mask = divisor >> 2;
            if (dividend >= mask) {
                dividend -= mask;
                mask = bit >> 2;
                quotient |= mask;
            }
            mask = divisor >> 3;
            if (dividend >= mask) {
                dividend -= mask;
                mask = bit >> 3;
                quotient |= mask;
            }
            if (dividend == 0) {
                break;
            }
            bit >>= 4;
            if (bit == 0) {
                break;
            }
            divisor >>= 4;
        } while (1);
    }

    dividend = quotient;
    if (sign >= 0) {
    } else {
        dividend = -dividend;
    }
    return dividend;
}

static inline u32 Ror32(u32 v, u32 n)
{
    return (v >> n) | (v << (32 - n));
}

/* Signed modulo (a % b), result takes the sign of the dividend (C's
 * '%'). Same nibble-at-a-time shift-and-subtract shape as sub_803ADB4,
 * but tracks a rotated bitmask of "which of the last 3 sub-levels
 * (divisor>>1/2/3) contributed" (via `ror` instead of a plain shift, so
 * a quotient-bit weight too small to represent by a normal shift still
 * leaves a nonzero marker bit up near bit 31) instead of a quotient -
 * used afterwards to add back exactly the fractional divisor amounts
 * that were subtracted from the running remainder but shouldn't have
 * been kept, since only the top-level (whole `divisor`) subtraction is
 * a genuine remainder step.
 *
 * NOT YET BYTE-MATCHING: verified correct (e.g. 7 % 3 == 1 traced by
 * hand through this exact reconstruction), and structurally mirrors
 * the ROM's register roles (r0 dividend, r1 divisor, r2 correction
 * mask, r3 bit, r4 scratch), but `Ror32` compiles to a shift/shift/or
 * triple instead of the ROM's single `ror` instruction, pulling in
 * extra scratch registers the ROM doesn't use. Same
 * shrink-wrapping-shaped prologue/epilogue gap as sub_803ADB4/
 * sub_803AF1C on top of that. Parked without spending further effort
 * chasing the `ror` codegen, since the entry/exit shape alone already
 * rules out a byte-exact match. */
s32 sub_803AE4C(s32 a, s32 b)
{
    register u32 dividend asm("r0");
    register u32 divisor asm("r1");
    register u32 corrMask asm("r2");
    register u32 bit asm("r3");
    register u32 mask asm("r4");
    s32 dividendSign;

    bit = 1;
    if (b == 0) {
        nullsub_8();
        return 0;
    }
    divisor = b;
    if ((s32)divisor >= 0) {
    } else {
        divisor = -divisor;
    }

    dividendSign = a;
    dividend = a;
    if ((s32)dividend >= 0) {
    } else {
        dividend = -dividend;
    }

    if (dividend >= divisor) {
        mask = 1u << 0x1c;
        while (divisor < mask && divisor < dividend) {
            divisor <<= 4;
            bit <<= 4;
        }
        mask <<= 3;
        while (divisor < mask && divisor < dividend) {
            divisor <<= 1;
            bit <<= 1;
        }

        corrMask = 0;
        do {
            if (dividend >= divisor) {
                dividend -= divisor;
            }
            mask = divisor >> 1;
            if (dividend >= mask) {
                dividend -= mask;
                corrMask |= Ror32(bit, 1);
            }
            mask = divisor >> 2;
            if (dividend >= mask) {
                dividend -= mask;
                corrMask |= Ror32(bit, 2);
            }
            mask = divisor >> 3;
            if (dividend >= mask) {
                dividend -= mask;
                corrMask |= Ror32(bit, 3);
            }
            if (dividend == 0) {
                break;
            }
            bit >>= 4;
            if (bit == 0) {
                break;
            }
            divisor >>= 4;
        } while (1);

        if (corrMask & 0xE0000000) {
            if (corrMask & Ror32(bit, 3)) {
                dividend += divisor >> 3;
            }
            if (corrMask & Ror32(bit, 2)) {
                dividend += divisor >> 2;
            }
            if (corrMask & Ror32(bit, 1)) {
                dividend += divisor >> 1;
            }
        }
    }

    if (dividendSign >= 0) {
        return dividend;
    }
    return -(s32)dividend;
}

/* Unsigned modulo (a % b) - same shape as sub_803AE4C but with no sign
 * handling at all (used where the caller already knows both operands
 * are non-negative), plus a `dividend < divisor` fast-return path the
 * signed version doesn't have. See sub_803AE4C for the correction-mask
 * technique.
 *
 * NOT YET BYTE-MATCHING, same reasons as sub_803AE4C above (the `ror`
 * codegen gap and the shared shrink-wrapping-shaped prologue/epilogue
 * gap - this one additionally has ROM-side per-path register-save
 * differences of its own: the `dividend < divisor` fast path returns
 * via a bare `mov pc, lr` with no `push` at all). Parked. */
u32 sub_803AF1C(u32 a, u32 b)
{
    register u32 dividend asm("r0");
    register u32 divisor asm("r1");
    register u32 corrMask asm("r2");
    register u32 bit asm("r3");
    register u32 mask asm("r4");

    dividend = a;
    divisor = b;

    if (b == 0) {
        nullsub_8();
        return 0;
    }
    bit = 1;
    if (dividend < divisor) {
        return dividend;
    }

    mask = 1u << 0x1c;
    while (divisor < mask && divisor < dividend) {
        divisor <<= 4;
        bit <<= 4;
    }
    mask <<= 3;
    while (divisor < mask && divisor < dividend) {
        divisor <<= 1;
        bit <<= 1;
    }

    corrMask = 0;
    do {
        if (dividend >= divisor) {
            dividend -= divisor;
        }
        mask = divisor >> 1;
        if (dividend >= mask) {
            dividend -= mask;
            corrMask |= Ror32(bit, 1);
        }
        mask = divisor >> 2;
        if (dividend >= mask) {
            dividend -= mask;
            corrMask |= Ror32(bit, 2);
        }
        mask = divisor >> 3;
        if (dividend >= mask) {
            dividend -= mask;
            corrMask |= Ror32(bit, 3);
        }
        if (dividend == 0) {
            break;
        }
        bit >>= 4;
        if (bit == 0) {
            break;
        }
        divisor >>= 4;
    } while (1);

    if (corrMask & 0xE0000000) {
        if (corrMask & Ror32(bit, 3)) {
            dividend += divisor >> 3;
        }
        if (corrMask & Ror32(bit, 2)) {
            dividend += divisor >> 2;
        }
        if (corrMask & Ror32(bit, 1)) {
            dividend += divisor >> 1;
        }
    }
    return dividend;
}
#endif
