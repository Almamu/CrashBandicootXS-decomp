#include "core.h"
#include "match.h"
#include "util.h"
#include "system.h"

/* Custom itoa: converts `value` to a NUL-terminated string in `buffer`
 * (base 2-36), returning the digit count (not including the NUL or the
 * '-' sign). Base 16 gets a fast path using bit-AND + arithmetic-shift
 * instead of a division call, producing uppercase hex digits; any other
 * base falls back to DivMod (a divmod helper). Digits are produced
 * least-significant-first then reversed in place at the end.
 *
 * No pins: the plain locals land in the ROM's registers, `i`/`rem`
 * sharing r1 and `j`/`base` sharing r4 (separate C locals with
 * non-overlapping lifetimes, as in the ROM). */
s32 itoa(s32 value, u8 *buffer, s32 base)
{
    s32 len;
    s32 negative;
    s32 rem;
    s32 temp;
    s32 i;
    s32 j;
    s32 k;

    len = 0;
    negative = 0;
    if (value < 0) {
        negative = 1;
        value = -value;
    }
    if (base == 16) {
        do {
            rem = value & 0xF;
            temp = value;
            if (value < 0) {
                temp += 0xF;
            }
            value = temp >> 4;
            if (rem > 9) {
                rem = rem + 0x37;
            } else {
                rem = rem + 0x30;
            }
            buffer[len] = rem;
            len++;
        } while (value > 0);
    } else {
        s32 rem2;
        s32 quotient;
        do {
            quotient = DivMod(value, base, &rem2);
            value = quotient;
            rem2 += 0x30;
            buffer[len] = rem2;
            len++;
        } while (value > 0);
    }
    if (negative) {
        buffer[len] = '-';
        len++;
    }
    buffer[len] = 0;

    i = 0;
    while (buffer[i] != 0)
        i++;
    j = i - 1;
    k = 0;
    while (k < j) {
        u8 *p1 = &buffer[k];
        u8 tmp = *p1;
        u8 *p2 = &buffer[j];
        u8 val = *p2;
        *p1 = val;
        *p2 = tmp;
        k++;
        j--;
    }
    return len;
}

/* Formats a single printf-style `%<width><specifier>` conversion
 * (specifier is 'd', 'x' or 'X' - anything else pads but writes no
 * digits) at `*fmt` into `dest`, left-padding with `padChar` to reach
 * `width` (parsed from the digits right before the specifier - note
 * every digit after the first is added to the accumulator as its raw
 * character value rather than `digit - '0'`, a bug in the original that
 * only stays invisible for single-digit widths and had to be
 * reproduced literally to match). Writes the number of format-string
 * characters consumed (digits + specifier) through `charsConsumedPtr`
 * (a 5th, stack-passed argument, but not written until right before the
 * final return - the ROM keeps the computed count live in a register
 * the whole time rather than storing it early), and returns `dest`
 * advanced past everything just written.
 *
 * The specifier check must stay a `switch` using `goto` to a single
 * shared call site (not `break`, which produces two separate physical
 * `bl itoa` copies here rather than one shared one like the
 * ROM has) - and the value/buffer-pointer/base arguments must each be
 * assigned to their own local *inside* every case (not referenced
 * directly at the shared call site) so gcc's cross-jump merging only
 * shares the trailing `bl`+adjustment it finds byte-identical in both
 * paths, leaving the (also identical, but positioned earlier) argument
 * setup duplicated per branch like the ROM does. The `default` case
 * must `goto` past the `width -= digitCount` subtraction entirely
 * (skipping straight to the unconditional `width -= 1` after it)
 * rather than computing `digitCount = 0` and subtracting a no-op - the
 * ROM has no instruction for the default case's "subtraction" at all.
 * `dstp` is pinned to r3 to match the ROM once `charsConsumed` living
 * in r1 across the whole function pushes gcc's own choice elsewhere. The final copy-from-`buf`-into-`dest` loop must be written
 * with an explicit `goto check` (matching the ROM's own
 * jump-to-condition-first shape) rather than a `for(;;) { ...; if
 * (c==0) break; ...}` - the more natural form makes gcc hoist a
 * `buf + 1` address computation out to before the switch and cache it
 * in a second high register (r9) the ROM never uses; the goto form
 * doesn't trigger that hoist. The `negOne` local (a literal `-1` in
 * the loop tests compiles differently) and the explicit `mov %0, #0`
 * before the final NUL write exist because the ROM re-materializes a
 * literal it already has sitting in a register from an unrelated
 * preceding comparison, rather than reusing it - plain C naturally
 * reuses the already-live value instead. */
u8 *FormatPaddedNumber(u8 *dest, u8 *fmt, s32 *valuePtr, u8 padChar, s32 *charsConsumedPtr)
{
    u8 buf[0x20];
    u8 *fmtStart;
    u8 c;
    s32 width;
    s32 digitCount;
    s32 i;
    u8 *srcp;
    MATCH_HOLD_REG(u8 *, dstp, r3);
    u8 ch;
    s32 charsConsumed;

    fmtStart = fmt;
    c = *fmt;
    fmt++;
    width = c - 0x30;
    for (;;) {
        c = *fmt;
        fmt++;
        if ((u8)(c - 0x30) > 9) {
            break;
        }
        width = width * 10 + c;
    }

    {
        s32 v;
        u8 *bufp;
        s32 base;
        switch (c) {
        case 'd':
            v = *valuePtr;
            bufp = buf;
            base = 10;
            goto doCall;
        case 'x':
        case 'X':
            v = *valuePtr;
            bufp = buf;
            base = 0x10;
            goto doCall;
        default:
            goto skipSub;
        }
    doCall:
        digitCount = itoa(v, bufp, base);
        width -= digitCount;
    skipSub:;
    }
    width -= 1;

    {
        s32 negOne = -1;
        charsConsumed = fmt - fmtStart;
        if (width != negOne) {
            do {
                *dest = padChar;
                dest++;
                width--;
            } while (width != negOne);
        }
    }
    *dest = 0;

    dstp = dest;
    srcp = buf;
    goto check;
    do {
        *dstp = ch;
        srcp++;
        dstp++;
    check:
        ch = *srcp;
    } while (ch != 0);
    {
        u8 zero;
        asm volatile("mov %0, #0" : "=r"(zero));
        *dstp = zero;
    }

    i = 0;
    while (dest[i] != 0) {
        i++;
    }
    dest += i;
    *charsConsumedPtr = charsConsumed;
    return dest;
}
