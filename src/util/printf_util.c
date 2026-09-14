#include "core.h"
#include <stdarg.h>

extern u8 *sub_80009F4(u8 *dest, u8 *fmt, s32 *valuePtr, u8 padChar,
                        s32 *charsConsumedPtr);
extern s32 itoa(s32 value, u8 *buffer, s32 base);

/* Custom sprintf: writes the formatted result of `fmt`/`args` into
 * `dest` (NUL-terminated) and returns a pointer to the end of it.
 * `args` is a raw array of 4-byte argument slots (not real varargs) -
 * every conversion, including `%c`, advances it by one slot regardless
 * of the actual value's size. Supported conversions: `%s` (string),
 * `%c` (single byte), `%d`/`%x`/`%X` (via itoa), `%<width>d/x/X`
 * with a space pad (`%5d`), and `%0<width>d/x/X` with a zero pad
 * (`%05d`, via sub_80009F4) - anything else (including a literal `%%`)
 * is echoed as-is.
 *
 * The specifier `switch` must list `case '%': default:` *last* (not
 * first) even though they share one body - agbcc lays out switch cases
 * in source order, and the ROM's own case for the shared "echo the
 * char verbatim" path sits physically last, right before the loop's
 * read-next-char continuation, with every other case branching to it
 * explicitly. Listing it first here made every other path merge
 * into *it* instead (still correct C, wrong bytes). The explicit `case
 * '%':` (rather than leaving '%' to fall under `default`) is also
 * required: it's what widens the switch's generated range check from
 * [`'0'`, `'x'`] to [`'%'`, `'x'`], matching the ROM's single bounds
 * check against the full range - without it, gcc generates *two*
 * separate range checks (one hand-written, one for the narrower
 * case-derived range), which is a genuine ROM/C structural mismatch,
 * not just registers.
 *
 * `charsConsumed` is deliberately declared inside the `'0'` and
 * `'1'`-`'9'` case bodies as two *separate* block-scoped locals (not
 * one shared variable) - the ROM gives them distinct stack slots
 * (`sp+4` vs `sp+8`, for a 0xc-byte frame) rather than reusing one.
 *
 * Genuinely `void`, not `u8 *`, despite every other function in this
 * printf stack (`itoa`, `sub_80009F4`) returning the advanced
 * pointer: the ROM's epilogue here never sets up r0 before the
 * `pop {r0}; bx r0` return dance, so whatever's left in r0 (0, from the
 * NUL-terminator write) is discarded by the caller regardless. */
void sub_8000AA8(u8 *dest, u8 *fmt, u32 *args)
{
    u8 c;

    for (;;) {
        c = *fmt;
        fmt++;
        if (c == 0) {
            break;
        }
        if (c != '%') {
            *dest = c;
            dest++;
            continue;
        }
        c = *fmt;
        fmt++;
        switch (c) {
        case '0': {
            s32 charsConsumed;
            dest = sub_80009F4(dest, fmt, (s32 *)args, '0', &charsConsumed);
            fmt += charsConsumed;
            args++;
            break;
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            s32 charsConsumed;
            dest = sub_80009F4(dest, fmt - 1, (s32 *)args, ' ', &charsConsumed);
            fmt += charsConsumed;
            args++;
            break;
        }
        case 's': {
            u8 *s = (u8 *)*args;
            args++;
            while (*s != 0) {
                *dest = *s;
                s++;
                dest++;
            }
            break;
        }
        case 'c':
            *dest = *(u8 *)args;
            dest++;
            args++;
            break;
        case 'd':
            dest += itoa(*(s32 *)args, dest, 10);
            args++;
            break;
        case 'x':
        case 'X':
            dest += itoa(*(s32 *)args, dest, 0x10);
            args++;
            break;
        case '%':
        default:
            *dest = c;
            dest++;
            break;
        }
    }
    *dest = 0;
}

/* Thin variadic wrapper: forwards straight to sub_8000AA8 with a
 * pointer to the first vararg (each slot is a plain 4-byte word, not
 * type-aware - matches sub_8000AA8's raw `u32 *` argument array). */
void sub_8000CA8(u8 *dest, u8 *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    sub_8000AA8(dest, fmt, (u32 *)args);
    va_end(args);
}

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");

#if NON_MATCHING
/* strstr, with optional case-insensitive matching (`caseInsensitive`
 * nonzero lowercases both sides before comparing). NOT YET BYTE-
 * MATCHING - see docs/matching.md, "Parked, not matched: sub_8000CBC"
 * for the full account; compiled only under `make NON_MATCHING=1`, the
 * checked-in assembly (asm/code_3_1_2.s) is used otherwise. Not
 * printf-related, but kept in this file rather than a new one purely
 * to preserve the ROM's address order (it sits immediately after
 * sub_8000CA8) without another ldscript.txt split.
 *
 * `haystack`/`hcOuter` are pinned to r5/r3 to match the ROM (both
 * plain scratch here - `hcOuter`'s own lifetime never crosses a call,
 * so no r4-r7 hazard). The rest of the "normalize a char to lowercase"
 * logic is written as `x = *p; p++; if (cond) x += 0x20;` using a
 * *plain*, unpinned local for the actual arithmetic (`hcOuterVal`,
 * `firstChar`, and the inner loop's `hc`/`nc`), with the pinned
 * `hcOuter` only ever assigned *from* the finished result and never
 * used mid-computation - pinning the working value directly (tried
 * first) makes gcc skip the (redundant, since the value's already in
 * 0-255 range) truncate-back-to-u8 step the ROM always has, and
 * collapses the compare's and the add's separate `r0`-routed copies of
 * the byte into one direct read/write of the pinned register instead.
 * This *plain*-local version reproduces the ROM's total instruction
 * count/registers everywhere except the shape of that "unchanged"
 * skip: the ROM still executes an (otherwise redundant) copy-into-r0
 * on the untaken branch before the shared truncate, where the plain
 * `if (cond) x += 0x20;` here just branches straight past the whole
 * conversion. An `s32 t = x; if (cond) t = x + 0x20; x = (u8)t;` form
 * *does* reproduce that shape exactly (verified in isolation), but
 * adding it back in this function specifically pushes register
 * pressure just far enough to spill `caseInsensitive` into r8 (needing
 * a save/restore dance the ROM doesn't have) - a worse mismatch than
 * the one it fixes. Whichever local register ends up hosting that
 * temp needs to be pinned without disturbing anything else already
 * correct here; not yet found.
 *
 * `needleRest` is pinned to `ip` (r12, also just scratch here - never
 * live across a call) to match the ROM, which computes it as a plain
 * copy of `needle` *before* reading `firstChar`, then increments it
 * *afterward* via a separate `movs r0, #1; add ip, r0` (the only way to
 * add an immediate to a high register in Thumb) - `needleRest = needle
 * + 1;` as one expression instead computes the sum in a low register
 * first and copies the result into `ip`, which is shorter but not what
 * the ROM does; no plain-C phrasing reproduced the ROM's instruction
 * order/split here, so the increment is spelled out as inline asm. */
u8 *sub_8000CBC(u8 *haystack0, u8 *needle, s32 caseInsensitive)
{
    register u8 *haystack asm("r5");
    u8 firstChar;
    register u8 *needleRest asm("ip");
    u8 *h, *n;
    u8 hcOuterVal;
    register s32 hcOuter asm("r3");
    u8 hc, nc;

    needleRest = needle;
    haystack = haystack0;
    firstChar = *needle;
    asm volatile("mov r0, #1\n\tadd %0, %0, r0" : "+r"(needleRest) : : "r0");
    if (firstChar == 0) {
        return 0;
    }
    if (caseInsensitive) {
        if ((u8)(firstChar - 'A') <= 0x19) {
            firstChar += 0x20;
        }
    }
outer:
    hcOuterVal = *haystack;
    haystack++;
    if (caseInsensitive) {
        if ((u8)(hcOuterVal - 'A') <= 0x19) {
            hcOuterVal += 0x20;
        }
    }
    hcOuter = hcOuterVal;
    if (hcOuter == firstChar) {
        goto matchStart;
    }
    if (hcOuter != 0) {
        goto outer;
    }
    return 0;

matchStart:
    h = haystack;
    n = needleRest;
inner:
    nc = *n;
    n++;
    if (nc == 0) {
        goto found;
    }
    hc = *h;
    h++;
    if (caseInsensitive) {
        if ((u8)(nc - 'A') <= 0x19) nc += 0x20;
        if ((u8)(hc - 'A') <= 0x19) hc += 0x20;
    }
    if (nc == hc) {
        goto inner;
    }
    goto outer;

found:
    return haystack - 1;
}
#endif /* NON_MATCHING */
