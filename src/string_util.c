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
