extern "C" {
#include "core.h"
#include <stdarg.h>
#include "util.h"
}

/* Custom sprintf: writes the formatted result of `fmt`/`args` into
 * `dest` (NUL-terminated) and returns a pointer to the end of it.
 * `args` is a raw array of 4-byte argument slots (not real varargs) -
 * every conversion, including `%c`, advances it by one slot regardless
 * of the actual value's size. Supported conversions: `%s` (string),
 * `%c` (single byte), `%d`/`%x`/`%X` (via itoa), `%<width>d/x/X`
 * with a space pad (`%5d`), and `%0<width>d/x/X` with a zero pad
 * (`%05d`, via FormatPaddedNumber) - anything else (including a literal `%%`)
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
 * printf stack (`itoa`, `FormatPaddedNumber`) returning the advanced
 * pointer: the ROM's epilogue here never sets up r0 before the
 * `pop {r0}; bx r0` return dance, so whatever's left in r0 (0, from the
 * NUL-terminator write) is discarded by the caller regardless. */
void vsprintf(u8 *dest, u8 *fmt, u32 *args)
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
        case '0':
            {
                s32 charsConsumed;
                dest = FormatPaddedNumber(dest, fmt, (s32 *)args, '0', &charsConsumed);
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
        case '9':
            {
                s32 charsConsumed;
                dest = FormatPaddedNumber(dest, fmt - 1, (s32 *)args, ' ', &charsConsumed);
                fmt += charsConsumed;
                args++;
                break;
            }
        case 's':
            {
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

/* Thin variadic wrapper: forwards straight to vsprintf with a
 * pointer to the first vararg (each slot is a plain 4-byte word, not
 * type-aware - matches vsprintf's raw `u32 *` argument array). */
void sprintf(u8 *dest, u8 *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(dest, fmt, (u32 *)args);
    va_end(args);
}
