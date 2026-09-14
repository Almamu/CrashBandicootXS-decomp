#include "core.h"

extern s32 sub_8000140(s32 value, s32 base, s32 *remainder);

/* Custom itoa: converts `value` to a NUL-terminated string in `buffer`
 * (base 2-36), returning the digit count (not including the NUL or the
 * '-' sign). Base 16 gets a fast path using bit-AND + arithmetic-shift
 * instead of a division call, producing uppercase hex digits; any other
 * base falls back to sub_8000140 (a divmod helper). Digits are produced
 * least-significant-first then reversed in place at the end.
 *
 * Every local here is register-pinned to r0-r7, chosen to match the
 * ROM's own allocation exactly - none of them touch the r4-r7 hazard in
 * matching_decomp_register_pinning memory, since each pinned r4-r7
 * variable's whole lifetime genuinely survives a call within this
 * function (v/buf/baseR/len/negative all live across the sub_8000140
 * call in the generic-base branch; j survives across the reversal
 * loop's body). `i`/`rem` reuse r1, and `j`/`baseR` reuse r4, matching
 * the ROM reusing a register once its previous occupant is dead - these
 * are two *separate* C locals with non-overlapping lifetimes, not one
 * value living in two places. Plain (unpinned) locals for `len`/
 * `negative` put them in the opposite registers from the ROM (r5/r7
 * instead of r7/r5) with no consistent way found to flip just one via
 * declaration order - pinning `negative` to r5 was enough to push gcc's
 * own allocator onto r7 for `len` unpinned, which is what actually
 * keeps the r7 push/pop safe here (a *pinned* r7 never gets saved, see
 * memory; letting gcc pick r7 on its own always does). */
s32 sub_800094C(s32 value, u8 *buffer, s32 base)
{
    register s32 v asm("r3");
    register u8 *buf asm("r6");
    register s32 baseR asm("r4");
    s32 len;
    register s32 negative asm("r5");
    register s32 rem asm("r1");
    s32 temp;
    register s32 i asm("r1");
    register s32 j asm("r4");
    register s32 k asm("r5");

    v = value;
    buf = buffer;
    baseR = base;
    len = 0;
    negative = 0;
    if (v < 0) {
        negative = 1;
        v = -v;
    }
    if (baseR == 16) {
        register s32 mask asm("r2") = 0xF;
        do {
            rem = v & mask;
            temp = v;
            if (v < 0) {
                temp += 0xF;
            }
            v = temp >> 4;
            if (rem > 9) {
                rem = rem + 0x37;
            } else {
                rem = rem + 0x30;
            }
            buf[len] = rem;
            len++;
        } while (v > 0);
    } else {
        s32 rem2;
        s32 quotient;
        do {
            quotient = sub_8000140(v, baseR, &rem2);
            v = quotient;
            rem2 += 0x30;
            buf[len] = rem2;
            len++;
        } while (v > 0);
    }
    if (negative) {
        buf[len] = '-';
        len++;
    }
    buf[len] = 0;

    i = 0;
    while (buf[i] != 0) i++;
    j = i - 1;
    k = 0;
    while (k < j) {
        u8 *p1 = &buf[k];
        u8 tmp = *p1;
        u8 *p2 = &buf[j];
        u8 val = *p2;
        *p1 = val;
        *p2 = tmp;
        k++;
        j--;
    }
    return len;
}

/* Trailing padding: the assembler's default NOP-encoding pad doesn't
 * match the ROM's zero-fill here (see matching_decomp_alignment_fix
 * memory). */
asm(".align 2, 0");

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
 * `bl sub_800094C` copies here rather than one shared one like the
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
 * `srcp`/`dstp` are pinned to r2/r3 to match the ROM once `charsConsumed`
 * living in r1 across the whole function pushes gcc's own choice
 * elsewhere. The final copy-from-`buf`-into-`dest` loop must be written
 * with an explicit `goto check` (matching the ROM's own
 * jump-to-condition-first shape) rather than a `for(;;) { ...; if
 * (c==0) break; ...}` - the more natural form makes gcc hoist a
 * `buf + 1` address computation out to before the switch and cache it
 * in a second high register (r9) the ROM never uses; the goto form
 * doesn't trigger that hoist. The two other inline-asm/pin spots
 * (`negOne`/r0 and the explicit `mov %0, #0`/r0 before the final NUL
 * write) exist because the ROM re-materializes a literal it already
 * has sitting in a register from an unrelated preceding comparison,
 * rather than reusing it - plain C naturally reuses the already-live
 * value instead. */
u8 *sub_80009F4(u8 *dest, u8 *fmt, s32 *valuePtr, u8 padChar,
                s32 *charsConsumedPtr)
{
    u8 buf[0x20];
    u8 *fmtStart;
    u8 c;
    s32 width;
    s32 digitCount;
    s32 i;
    register u8 *srcp asm("r2");
    register u8 *dstp asm("r3");
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
        digitCount = sub_800094C(v, bufp, base);
        width -= digitCount;
skipSub:
        ;
    }
    width -= 1;

    {
        register s32 negOne asm("r0") = -1;
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
        register u8 zero asm("r0");
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
