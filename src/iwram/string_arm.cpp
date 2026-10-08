extern "C" {
#include "core.h"
#include "match.h"
#include "iwram.h"
}

/*
 * IWRAM 0x030000D4-0x0300024C (stored in ROM at 0x087E56B8): ARM copies
 * of the string helpers the Thumb code also has (src/util/number_format.cpp,
 * src/util/string.cpp), part of the IWRAM image crt0 copies to
 * 0x03000000 at boot (see src/iwram/iwram_data.cpp and docs/data.md).
 *
 * Built as ARM code (Makefile ARM_OBJS) from C++ with agbcp_arm_patched,
 * -mleaf-no-lr-save and no instruction scheduling (docs/cplusplus.md,
 * "The IWRAM ARM code"). strlen_arm, strcpy_arm, strncpy_arm and
 * strcat_arm come out the same under stock agbcc_arm's flags. itoa_arm
 * needs the rest: the ROM's ARM gcc saves r4-r6 without lr, which stock
 * agbcc_arm can't - see its comment and docs/matching/iwram-image.md.
 *
 * UNUSED - no caller anywhere in the ROM (checked: none of these five
 * addresses appears as a word in baserom.gba, and ARM code can only be
 * reached from Thumb through a pointer).
 */

/* strlen. UNUSED - see the file comment. */
s32 strlen_arm(u8 *s)
{
    s32 i = 0;

    while (s[i] != 0)
        i++;
    return i;
}

/* strcpy; the terminator is stored from the byte that ended the loop.
 * UNUSED - see the file comment. */
void strcpy_arm(u8 *dst, u8 *src)
{
    u8 c;

    while ((c = *src) != 0) {
        *dst++ = c;
        src++;
    }
    *dst = c;
}

/* strncpy without the zero padding: copies at most n bytes, and
 * NUL-terminates only if fewer than n were copied. UNUSED - see the
 * file comment.
 *
 * Two details reproduce the ROM's code:
 * - `c = 0; if (n != c) *dst = c;` makes the final test compare `n`
 *   with the register just zeroed (`mov r3, #0; cmp r2, r3`) instead of
 *   the immediate 0.
 * - The empty asm at the end emits no code. Without it agbcc_arm turns
 *   the `n == 0` branch to the final `bx lr` into a conditional return
 *   (`bxeq lr`): its jump pass rewrites any jump whose target is directly
 *   followed by a return (jump.c, "turn it into a RETURN insn").
 *   The asm sits between that label and the return, so the branch
 *   stays. The ROM's ARM compiler never emits a conditional return (see
 *   docs/matching/iwram-image.md). */
void strncpy_arm(u8 *dst, u8 *src, s32 n)
{
    u8 c;

    if (n != 0) {
        c = *src;
        if (c != 0) {
            do {
                *dst++ = c;
                if (--n == 0)
                    break;
                c = *++src;
            } while (c != 0);
        }
        c = 0;
        if (n != c)
            *dst = c;
    }
    MATCH_BARRIER();
}

/* strcat. UNUSED - see the file comment. */
void strcat_arm(u8 *dst, u8 *src)
{
    u8 c;

    while (*dst != 0)
        dst++;
    while ((c = *src) != 0) {
        *dst++ = c;
        src++;
    }
    *dst = c;
}

/* itoa: writes `value` in `base` to `buf` (upper-case hex digits, a
 * leading '-' for negative values), NUL-terminates it and returns its
 * length. Base 16 uses shifts; any other base divides with the BIOS Div
 * SWI (`swi 0x60000` in ARM state: r0 = quotient, r1 = remainder, r3
 * clobbered). UNUSED - see the file comment.
 *
 * The ROM saves r4-r6 without lr (`push {r4, r5, r6}` ... `pop {r4, r5,
 * r6}; bx lr`), which stock agbcc_arm can't do: it adds lr to every
 * register push. string_arm.o is built with agbcp_arm_patched's
 * -mleaf-no-lr-save, which leaves lr out when the function never uses
 * it, and with both scheduling passes off (the ROM keeps each loop's
 * `add`/`cmp`, the terminator store and the swap's `add`/`sub` in
 * source order). Makefile PATCHED_ARM_OBJS; docs/matching/iwram-image.md,
 * seventh pass.
 *
 * - The pins put the values in the ROM's registers (agbcc_arm's
 *   allocator would use lr and r2/r3). `num`/`digit` are also the SWI's
 *   r0/r1.
 * - `neg` is the sign flag and then the swap's left index, as r4 is in
 *   the ROM. On the '-' path it is reset to 0, so `b[neg]` stores the
 *   terminator and the swap starts at 0 on both paths.
 * - The sign is set by two ifs on the same test. As one if/else, jump.c
 *   hoists `neg = 0` above the branch (`if (...) { x = a; goto l; } x =
 *   b;` becomes `x = a; if (...) goto l; x = b;`), giving `mov r4, #0;
 *   addlt r4, r4, #1`. As two ifs, jump.c turns the first into a
 *   conditional move (`movge r4, #0`), cse drops the second compare,
 *   and the ccfsm prints the second if as the ROM's `movlt`/`rsblt`.
 *   gcc can't tell that the two ifs together always set `neg`;
 *   MATCH_HOLD(neg) defines it first (no code), for -Werror's "might be
 *   used uninitialized".
 * - MATCH_KEEP(base) keeps the `!= 16` test on base (r2) after the copy
 *   to `divisor` (ip), as in the ROM.
 * - `(ten = 10)` keeps `cmp r1, #10` with `addge`/`addlt`: fold-const
 *   rewrites a plain `digit >= 10` to `digit > 9`.
 * - MATCH_CONST(len, 0) stops cse from reusing len's 0 for `neg = 0`.
 * - The '-' is a u8 pinned to r4. As a store to the s32 `neg`,
 *   reload_cse_move2add rewrites the later `neg = 0` as `neg - 45`
 *   (`subne r4, r4, #45`); it only tracks a register's constant into a
 *   set of the same or a narrower mode. */
s32 itoa_arm(s32 value, u8 *buf, s32 base)
{
    MATCH_HOLD_REG(s32, num, r0);
    MATCH_HOLD_REG(s32, digit, r1);
    MATCH_HOLD_REG(u8 *, b, r6);
    MATCH_HOLD_REG(s32, len, r5);
    MATCH_HOLD_REG(s32, neg, r4);
    MATCH_HOLD_REG(s32, j, r1);
    s32 divisor;
    s32 ten;

    MATCH_CONST(len, 0);
    MATCH_HOLD(neg);
    num = value;
    if (num >= 0)
        neg = 0;
    if (num < 0) {
        neg = 1;
        num = -num;
    }
    b = buf;
    divisor = base;
    MATCH_KEEP(base);
    if (base != 16) {
        do {
            digit = divisor;
            asm("swi 0x60000" : "=r"(num), "=r"(digit) : "0"(num), "1"(digit) : "r3");
            b[len] = digit + '0';
            len++;
        } while (num != 0);
    } else {
        do {
            digit = num & 15;
            num >>= 4;
            if (digit >= (ten = 10))
                digit += 'A' - 10;
            else
                digit += '0';
            b[len] = digit;
            len++;
        } while (num != 0);
    }
    if (neg) {
        MATCH_HOLD_REG(u8, minus, r4) = '-';
        b[len] = minus;
        len++;
        neg = 0;
    }
    b[len] = neg;
    j = len - 1;
    do {
        u8 lo = b[neg];
        MATCH_HOLD_REG(u8, hi, r0) = b[j];
        b[j] = lo;
        b[neg] = hi;
        neg++;
        j--;
    } while (neg < j);
    return len;
}
