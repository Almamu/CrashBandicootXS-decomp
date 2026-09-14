#include "core.h"

/* Sits between the still-parked sub_8000EE4 (asm/code_3_1_3.s) and
 * sub_80010E0 (asm/code_3_1_4.s). */

extern s32 sub_803AF1C(s32 value, s32 divisor);
extern s32 sub_8037E54(s32 value, s32 divisor);

/* Formats `value` (in centiseconds) as "MM:SS.X0" into `buf` (9 bytes,
 * NUL-terminated) - only one fractional digit is actually computed
 * (`value % 10`); the other is always '0'. */
void FormatCentiseconds(s32 value, u8 *buf)
{
    s32 q1, q2, secPart;

    buf[8] = 0;
    buf[7] = '0';
    buf[6] = sub_803AF1C(value, 10) + '0';
    buf[5] = '.';
    q1 = sub_8037E54(value, 10);
    q2 = sub_8037E54(q1, 60);
    secPart = sub_803AF1C(q1, 60);
    buf[4] = sub_803AF1C(secPart, 10) + '0';
    buf[3] = sub_8037E54(secPart, 10) + '0';
    buf[2] = ':';
    buf[1] = sub_803AF1C(q2, 10) + '0';
    buf[0] = sub_8037E54(q2, 10) + '0';
}

/* Trailing padding (see matching_decomp_alignment_fix memory). */
asm(".align 2, 0");
