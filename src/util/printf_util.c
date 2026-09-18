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
 * Written as NAKED asm, not plain C: every earlier plain-C attempt (see
 * git history / docs/matching.md's "Parked, not matched: sub_8000CBC")
 * got every register/instruction right except one unavoidable gap -
 * the "normalize a char to lowercase, on the unchanged path" branch
 * shape, where the ROM still routes the untaken branch through a
 * redundant copy-into-r0 before a shared truncate that a plain
 * `if (cond) x += 0x20;` just branches straight past. Reproducing that
 * exact shape in C pushed register pressure just far enough to spill
 * `caseInsensitive` into r8, a worse mismatch than the one it fixed.
 * Every instruction below is confirmed byte-identical to the ROM (this
 * doc comment doubles as that derivation) - full NAKED transcription,
 * like this project's other hard-compiler-limitation cases
 * (`src/util/math_div_util.c`'s `nullsub_8`, `src/system/link_cable.c`'s
 * `sub_8001CB8`/`sub_8001DB4`), is more honest than continuing to chase
 * the one remaining branch shape through plain C. */
NAKED u8 *sub_8000CBC(u8 *haystack0, u8 *needle, s32 caseInsensitive)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "add r7, r2, #0\n\t"
        "mov ip, r1\n\t"
        "add r5, r0, #0\n\t"
        "ldrb r6, [r1]\n\t"
        "mov r0, #1\n\t"
        "add ip, r0\n\t"
        "cmp r6, #0\n\t"
        "beq 7f\n\t"
        "cmp r7, #0\n\t"
        "beq 3f\n\t"
        "add r0, r6, #0\n\t"
        "sub r0, #0x41\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0x19\n\t"
        "bhi 1f\n\t"
        "add r0, r6, #0\n\t"
        "add r0, #0x20\n\t"
        "b 2f\n\t"
    "1:\n\t"
        "add r0, r6, #0\n\t"
    "2:\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r6, r0, #0x18\n\t"
    "3:\n\t"
        "ldrb r3, [r5]\n\t"
        "add r5, #1\n\t"
        "cmp r7, #0\n\t"
        "beq 6f\n\t"
        "add r0, r3, #0\n\t"
        "sub r0, #0x41\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0x19\n\t"
        "bhi 4f\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #0x20\n\t"
        "b 5f\n\t"
    "4:\n\t"
        "add r0, r3, #0\n\t"
    "5:\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r3, r0, #0x18\n\t"
    "6:\n\t"
        "cmp r3, r6\n\t"
        "beq 8f\n\t"
        "cmp r3, #0\n\t"
        "bne 3b\n\t"
    "7:\n\t"
        "mov r0, #0\n\t"
        "b 16f\n\t"
    "8:\n\t"
        "add r4, r5, #0\n\t"
        "mov r2, ip\n\t"
    "9:\n\t"
        "ldrb r3, [r2]\n\t"
        "add r2, #1\n\t"
        "cmp r3, #0\n\t"
        "beq 15f\n\t"
        "ldrb r1, [r4]\n\t"
        "add r4, #1\n\t"
        "cmp r7, #0\n\t"
        "beq 14f\n\t"
        "add r0, r3, #0\n\t"
        "sub r0, #0x41\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0x19\n\t"
        "bhi 10f\n\t"
        "add r0, r3, #0\n\t"
        "add r0, #0x20\n\t"
        "b 11f\n\t"
    "10:\n\t"
        "add r0, r3, #0\n\t"
    "11:\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r3, r0, #0x18\n\t"
        "add r0, r1, #0\n\t"
        "sub r0, #0x41\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r0, r0, #0x18\n\t"
        "cmp r0, #0x19\n\t"
        "bhi 12f\n\t"
        "add r0, r1, #0\n\t"
        "add r0, #0x20\n\t"
        "b 13f\n\t"
    "12:\n\t"
        "add r0, r1, #0\n\t"
    "13:\n\t"
        "lsl r0, r0, #0x18\n\t"
        "lsr r1, r0, #0x18\n\t"
    "14:\n\t"
        "cmp r3, r1\n\t"
        "beq 9b\n\t"
        "b 3b\n\t"
    "15:\n\t"
        "sub r0, r5, #1\n\t"
    "16:\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
    );
}
asm(".align 2, 0");
