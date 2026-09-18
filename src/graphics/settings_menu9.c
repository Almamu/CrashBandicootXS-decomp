#include "core.h"

extern s32 sub_803AE4C(s32 dividend, s32 divisor);
extern s32 sub_803ADB4(s32 dividend, s32 divisor);

/* Decimal `itoa`: writes `value`'s decimal digits (unsigned, most
 * significant first) to `dest`, NUL-terminated, and returns the digit
 * count. Shared by every settings-row/results-widget number label in
 * this ROM region (`src/graphics/settings_menu6.c`/`settings_menu7.c`
 * already call it as an `extern`). Builds the digits least-significant
 * first into a small stack buffer via the div/mod library primitives
 * (`src/util/math_div_util.c`), then reverses them into `dest`.
 *
 * `val`'s explicit `r5` pin (initialized from the `value` parameter,
 * rather than just using `value` directly) is required to reproduce
 * the ROM's parameter-home order: with a plain unpinned `value`, this
 * compiler always copies argument registers to their home pseudo-regs
 * in ascending source-register order (r0 before r1), but the ROM
 * copies r1 (`dest` -> r7) first, r0 (`value` -> r5) second - pinning
 * `val`'s initializer as a separate reg-var assignment defers the r0
 * copy until just before the loop that needs it, matching the ROM's
 * order, while leaving `dest` to the natural allocator (an *explicit*
 * pin on `dest` would also have to include r7 in the push/pop list by
 * hand - a genuine ABI hazard in this toolchain, see
 * matching_decomp_register_pinning memory - whereas the natural
 * allocator gets the push/pop list right on its own once it reaches
 * for r7 by itself). */
s32 sub_80060AC(s32 value, u8 *dest)
{
    register s32 val asm("r5") = value;
    u8 buf[0xc];
    s32 count;
    s32 i;

    count = 0;
    do {
        u8 *p = &buf[count];
        *p = (u8)sub_803AE4C(val, 10) + '0';
        val = sub_803ADB4(val, 10);
        count++;
    } while (val != 0);

    i = 0;
    do {
        count--;
        dest[count] = buf[i];
        i++;
    } while (count != 0);
    dest[i] = 0;

    return i;
}

/* Formats a `" <NN%>"`-shaped scratch string (space, `<`, decimal
 * digits of `arg1 * 5`, `%`, `>`, NUL) into `out` via sub_80060AC
 * above. `arg0` is read by nothing in this function - a genuinely
 * unused parameter (the ROM's own r0 -> r0 first instruction discards
 * it before ever reading it). */
void sub_80060F8(s32 arg0, s32 arg1, u8 *out)
{
    s32 value = arg1 * 5;
    s32 count;

    out[0] = ' ';
    out[1] = '<';
    count = sub_80060AC(value, out + 2);
    out[count + 2] = '%';
    out[count + 3] = '>';
    out[count + 4] = 0;
}
/* Trailing byte-padding gotcha (see docs/matching.md/
 * matching_decomp_alignment_fix memory): the ROM pads the gap before
 * the next function (sub_8006124, still-raw at this point) with zero
 * bytes (an explicit `.align 2, 0` in the original assembly), but this
 * compiler's own default inter-function padding is a `mov r8, r8`
 * NOP-equivalent instead. */
asm(".align 2, 0");
