extern "C" {
#include "core.h"
#include "util.h"
#include "system.h"
}

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

/* strcpy and strlen (src/util/string.cpp), as the inline copies
 * FormatPaddedNumber's code has: the ROM inlines both, re-storing the
 * terminator from a fresh `movs r0, #0`. */
static inline void InlineCopyString(u8 *dst, u8 *src)
{
    u8 c;
    while ((c = *src) != 0) {
        *dst = c;
        src++;
        dst++;
    }
    *dst = 0;
}

static inline s32 InlineStringLength(u8 *s)
{
    s32 i = 0;
    while (s[i] != 0)
        i++;
    return i;
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
 * `negOne` holds the padding loop's -1 so it's loaded before
 * `consumed` is computed, as in the ROM. */
u8 *FormatPaddedNumber(u8 *dest, u8 *fmt, s32 *valuePtr, u8 padChar, s32 *charsConsumedPtr)
{
    u8 buf[0x20];
    u8 *start = fmt;
    u8 c = *fmt++;
    s32 width = c - '0';
    s32 consumed;

    for (;;) {
        c = *fmt++;
        if ((u8)(c - '0') > 9)
            break;
        width = width * 10 + c;
    }

    switch (c) {
    case 'd':
        width -= itoa(*valuePtr, buf, 10);
        break;
    case 'x':
    case 'X':
        width -= itoa(*valuePtr, buf, 16);
        break;
    }
    width--;
    {
        s32 negOne = -1;

        consumed = fmt - start;
        while (width != negOne) {
            *dest++ = padChar;
            width--;
        }
    }
    *dest = 0;
    InlineCopyString(dest, buf);
    dest += InlineStringLength(dest);
    *charsConsumedPtr = consumed;
    return dest;
}
