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

/* strstr, with optional case-insensitive matching (`caseInsensitive`
 * nonzero lowercases both sides before comparing): scans `haystack0`
 * for the first occurrence of `needle`, lowercasing both sides
 * byte-by-byte before comparing whenever `caseInsensitive` is nonzero
 * (`(u8)(c - 'A') <= 0x19` is the ROM's own range check for
 * `'A'`-`'Z'`), returning a pointer into `haystack0` at the match or
 * `0` if not found or if `needle` is empty. Not printf-related, but
 * kept in this file rather than a new one purely to preserve the ROM's
 * address order (it sits immediately after sub_8000CA8) without
 * another ldscript.txt split.
 *
 * Two compiler gaps, both closed with the techniques already used
 * elsewhere in this file's neighbors (see naked-sub_8001524-matched.md
 * and naked-sub_80010e0-matched.md):
 *
 * 1. The "normalize a char to lowercase, on the unchanged path" branch
 *    shape: the ROM routes the untaken branch of the range check
 *    through a redundant copy-into-r0 before a shared truncate, where
 *    a plain `if (cond) x += 0x20;` just branches straight past it,
 *    and gcc folds an equivalent ternary back into the same shape once
 *    it proves the truncate redundant. Fixed the same way as
 *    `sub_8001524`/`sub_8001624`: each fold is materialized as an
 *    opaque inline-asm block the optimizer can't see into.
 * 2. The inner verify loop's "needle exhausted, match found" check
 *    compiled with the opposite branch sense from the ROM (`bne` to a
 *    same-iteration fallthrough instead of the ROM's `beq` clear across
 *    to a tail shared with the epilogue) whenever the match-found value
 *    was computed inline at the check site - gcc's block linearizer
 *    always inlines a short taken-branch target right at the branch.
 *    Deferring the computation to a label placed after the whole
 *    scan/verify loop (so it's the function's last basic block, exactly
 *    where the ROM put it, immediately before the shared epilogue) gets
 *    gcc to lay out the branch the same way the ROM's compiler did. */
u8 *sub_8000CBC(u8 *haystack0, u8 *needle, s32 caseInsensitive)
{
    register u8 *needleRest asm("ip") = needle;
    register u8 *haystack asm("r5") = haystack0;
    register u32 c0 asm("r6") = *needle;
    register u32 hc asm("r3");
    register u8 *matchHaystack asm("r4");
    register u8 *matchNeedle asm("r2");
    register u32 nc asm("r3");
    register u32 hc2 asm("r1");

    asm volatile("mov r0, #1\n\tadd %0, r0" : "+r"(needleRest) :: "r0");

    if (c0 == 0) {
        return 0;
    }
    if (caseInsensitive != 0) {
        asm volatile(
            "add r0, %0, #0\n\t"
            "sub r0, #0x41\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr r0, r0, #0x18\n\t"
            "cmp r0, #0x19\n\t"
            "bhi 1f\n\t"
            "add r0, %0, #0\n\t"
            "add r0, #0x20\n\t"
            "b 2f\n\t"
            "1:\n\t"
            "add r0, %0, #0\n\t"
            "2:\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr %0, r0, #0x18\n\t"
            : "+r"(c0) :: "r0");
    }

scan:
    hc = *haystack;
    haystack++;
    if (caseInsensitive != 0) {
        asm volatile(
            "add r0, %0, #0\n\t"
            "sub r0, #0x41\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr r0, r0, #0x18\n\t"
            "cmp r0, #0x19\n\t"
            "bhi 1f\n\t"
            "add r0, %0, #0\n\t"
            "add r0, #0x20\n\t"
            "b 2f\n\t"
            "1:\n\t"
            "add r0, %0, #0\n\t"
            "2:\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr %0, r0, #0x18\n\t"
            : "+r"(hc) :: "r0");
    }
    if (hc == c0) {
        goto verify;
    }
    if (hc != 0) {
        goto scan;
    }
    return 0;

verify:
    matchHaystack = haystack;
    matchNeedle = needleRest;
inner:
    nc = *matchNeedle;
    matchNeedle++;
    if (nc == 0) {
        goto matchFound;
    }
    hc2 = *matchHaystack;
    matchHaystack++;
    if (caseInsensitive != 0) {
        asm volatile(
            "add r0, %0, #0\n\t"
            "sub r0, #0x41\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr r0, r0, #0x18\n\t"
            "cmp r0, #0x19\n\t"
            "bhi 1f\n\t"
            "add r0, %0, #0\n\t"
            "add r0, #0x20\n\t"
            "b 2f\n\t"
            "1:\n\t"
            "add r0, %0, #0\n\t"
            "2:\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr %0, r0, #0x18\n\t"
            : "+r"(nc) :: "r0");
        asm volatile(
            "add r0, %0, #0\n\t"
            "sub r0, #0x41\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr r0, r0, #0x18\n\t"
            "cmp r0, #0x19\n\t"
            "bhi 1f\n\t"
            "add r0, %0, #0\n\t"
            "add r0, #0x20\n\t"
            "b 2f\n\t"
            "1:\n\t"
            "add r0, %0, #0\n\t"
            "2:\n\t"
            "lsl r0, r0, #0x18\n\t"
            "lsr %0, r0, #0x18\n\t"
            : "+r"(hc2) :: "r0");
    }
    if (nc == hc2) {
        goto inner;
    }
    goto scan;

matchFound:
    return haystack - 1;
}
asm(".align 2, 0");
