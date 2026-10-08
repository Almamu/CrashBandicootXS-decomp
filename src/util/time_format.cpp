extern "C" {
#include "core.h"
#include <libgcc.h>
#include "util.h"
}

/* Sits between the still-parked DrawWrappedText (asm/code_3_1_3.s) and
 * WaitForKeyPress (asm/code_3_1_4.s). */

/* Formats `value` (in centiseconds) as "MM:SS.X0" into `buf` (9 bytes,
 * NUL-terminated) - only one fractional digit is actually computed
 * (`value % 10`); the other is always '0'. */
void FormatCentiseconds(s32 value, u8 *buf)
{
    s32 q1, q2, secPart;

    buf[8] = 0;
    buf[7] = '0';
    buf[6] = __umodsi3(value, 10) + '0';
    buf[5] = '.';
    q1 = __udivsi3(value, 10);
    q2 = __udivsi3(q1, 60);
    secPart = __umodsi3(q1, 60);
    buf[4] = __umodsi3(secPart, 10) + '0';
    buf[3] = __udivsi3(secPart, 10) + '0';
    buf[2] = ':';
    buf[1] = __umodsi3(q2, 10) + '0';
    buf[0] = __udivsi3(q2, 10) + '0';
}
