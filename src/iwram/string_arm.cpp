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
 * -mleaf-no-lr-save, -mno-cond-return and no instruction scheduling
 * (docs/cplusplus.md, "The IWRAM ARM code"). strlen_arm, strcpy_arm and
 * strcat_arm come out the same under stock agbcc_arm's flags.
 * strncpy_arm needs -mno-cond-return: the ROM's ARM gcc makes no
 * conditional returns. itoa_arm needs the rest: the ROM's ARM gcc saves
 * r4-r6 without lr, which stock agbcc_arm can't. See their comments and
 * docs/matching/iwram-image.md.
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
 * `c = 0; if (n != c) *dst = c;` makes the final test compare `n` with
 * the register just zeroed (`mov r3, #0; cmp r2, r3`) instead of the
 * immediate 0.
 *
 * The `n == 0` branch to the final `bx lr` stays a branch only under
 * -mno-cond-return (Makefile). Stock agbcc_arm's jump pass turns any
 * jump to a label followed by the return into a conditional return
 * (`bxeq lr`), gated only by arm.c's use_return_insn, which holds for
 * every frameless leaf with nothing saved, so no C shape avoids it; the
 * ROM's later ARM gcc never emits one. #662 rounds 3-4 needed an empty
 * asm barrier between that label and the return
 * (docs/matching/iwram-image.md, "Ninth step"). */
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
 * - The function reuses its variables once they are dead, as the ROM's
 *   registers show: `neg` is the sign flag and then the swap's left
 *   index (r4). On the '-' path it is reset to 0, so `b[neg]` stores the
 *   terminator and the swap starts at 0 on both paths. `digit` is the
 *   swap's right index and `num` holds its high byte (the ROM's r1 and
 *   r0, the SWI's registers). #662 round 8: the swap with its own `j`
 *   and `hi` locals needed a pin each (r1, r0); written with `digit` and
 *   `num` it is the ROM with neither.
 * - The sign is set by two ifs on the same test. As one if/else, jump.c
 *   hoists `neg = 0` above the branch (`if (...) { x = a; goto l; } x =
 *   b;` becomes `x = a; if (...) goto l; x = b;`), giving `mov r4, #0;
 *   addlt r4, r4, #1`. As two ifs, jump.c turns the first into a
 *   conditional move (`movge r4, #0`), cse drops the second compare,
 *   and the ccfsm prints the second if as the ROM's `movlt`/`rsblt`.
 *   gcc can't tell that the two ifs together always set `neg`, so it
 *   prints a false "might be used uninitialized"; the warning is left
 *   enabled on purpose and the Makefile builds this object with
 *   -Wno-error (UNINIT_WARNING_OBJS, #662). The MATCH_HOLD(neg) that
 *   used to silence it changed no code (#662 round 8).
 * - MATCH_KEEP(base) keeps the `!= 16` test on base (r2) after the copy
 *   to `divisor` (ip), as in the ROM.
 * - `(ten = 10)` keeps `cmp r1, #10` with `addge`/`addlt`: fold-const
 *   rewrites a plain `digit >= 10` to `digit > 9`.
 * - MATCH_CONST(len, 0) stops cse from reusing len's 0 for `neg = 0`.
 * - The '-' is a u8 pinned to r4. As a store to the s32 `neg`,
 *   reload_cse_move2add rewrites the later `neg = 0` as `neg - 45`
 *   (`subne r4, r4, #45`); it only tracks a register's constant into a
 *   set of the same or a narrower mode.
 * - #662 round 6, rewritten from the ROM as plain code (the SWI asm
 *   with its r0/r1 operands): 64 lines off, with lr and ip for the
 *   buffer and the length. Each site taken out alone, under the
 *   Makefile's flags, with -mstrict-cross-jump and
 *   -minterwork-return-lr added, or with -ffixed-lr (gcc's own way of
 *   keeping lr out of allocation): len's MATCH_CONST 2 lines (`movge
 *   r4, r5`: cse reuses len's 0), the base keep 15 (the compare and the
 *   loop share r2), the len/b/neg pins 22-28, j's 10, minus' and hi's 4
 *   each. The sign as one if/else needs no MATCH_HOLD but prints the
 *   `movge` after `movlt`/`rsblt` (2 lines); `neg = num < 0` is an
 *   `lsrs #31` (14). These are allocation and cse choices; no option
 *   the patched compiler has reaches them.
 * - #662 round 7, private agbcp_arm_patched builds, each checked on all
 *   ten functions of string_arm.o and sprite_arm.o (the ARM compiler
 *   builds nothing else; lib/ has no ARM C): without fold-const's `X >=
 *   C` to `X > C - 1` rewrite, a plain `digit >= 10` gives the ROM's
 *   `cmp r1, #10` and nothing else changes, but no other ARM function
 *   compares against a constant with `>=` or `<`, so that proves
 *   nothing. lr gets `len` because global-alloc counts it as already
 *   used (it is call-clobbered) and REG_ALLOC_ORDER has it before r4.
 *   Moving it after r11 changes nothing, matched or plain itoa_arm;
 *   not counting it as used changes HeapSortActorsByKey,
 *   UnpackRleSpriteFrame and LookupSpriteFrameCache, whose ROM code
 *   uses lr (even when limited to leaf functions), and both together
 *   change 5. len's 0 is reused for `neg = 0` (`movge r4, r5`) by
 *   reload_cse's operand substitution; without it strncpy_arm's `cmp
 *   r2, r3` and the other string functions change, and cheaper
 *   constants change 6-8 functions.
 * - #662 round 8, tools/natural_enum.py (every pair of 35 hand-written
 *   spellings and the automatic type/compound/order edits, scored on all
 *   five string functions): without the base keep the nearest is 1 line
 *   off, an SWI that also clobbers r2 (the BIOS leaves it alone), which
 *   gives the ROM's `mov ip, r2` but compares the copy; the '-' through
 *   `neg` is 1 line off (the `subne` above), through `digit`, `num` or a
 *   u8 local 2 (r3); len's 0 as a plain `len = 0` 1 line, in every
 *   order; `(ten = 10)` as `digit - 10 >= 0` 2 lines, the other
 *   spellings (`< 10` with the arms swapped, `?:`, `>= 0xA`, adding 7)
 *   3. The b/len/neg pins stay 6-14 lines off.
 * - #662 round 9 (natural_enum.py, every pair of 79 edits: each local's
 *   and parameter's type over s8-u32, a u32 return, `buf`/`b` as char *
 *   or s8 *, `minus`/`lo` as char or s32, the declaration and statement
 *   orders), with all the sites out (32 instructions off at best, as
 *   without the edits) and with each taken out alone: len's
 *   MATCH_CONST stays 1 off, the base keep 2 (1 with `divisor` a u8,
 *   which would truncate the base), minus' pin 2, the b and len pins
 *   13-14, neg's 11, digit's 4. num's r0 pin alone is byte-neutral, but
 *   it is what binds the SWI's r0 operand, so it stays with the SWI.
 *   The quotient and remainder as a two-word struct (the Div result) is
 *   44 off with every site out. */
s32 itoa_arm(s32 value, u8 *buf, s32 base)
{
    MATCH_HOLD_REG(s32, num, r0);
    MATCH_HOLD_REG(s32, digit, r1);
    MATCH_HOLD_REG(u8 *, b, r6);
    MATCH_HOLD_REG(s32, len, r5);
    MATCH_HOLD_REG(s32, neg, r4);
    s32 divisor;
    s32 ten;

    MATCH_CONST(len, 0);
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
    digit = len - 1;
    do {
        u8 lo = b[neg];
        num = b[digit];
        b[digit] = lo;
        b[neg] = num;
        neg++;
        digit--;
    } while (neg < digit);
    return len;
}
