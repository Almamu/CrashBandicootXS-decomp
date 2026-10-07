#ifndef GUARD_MATH_UTIL_H
#define GUARD_MATH_UTIL_H

/* Shared helpers for the common arithmetic (#667): fixed-point
 * conversions, min/max/abs/clamp and the sine table lookups.
 *
 * Every helper is a macro that expands to exactly the expression it
 * replaces, token for token apart from parentheses, so a converted site
 * compiles to the same bytes. agbcc (gcc 2.9) is sensitive to things
 * that look equivalent, so:
 *
 * - Nothing here casts. The signedness of a shift comes from its operand:
 *   Q8_TO_INT of an s32 is an arithmetic shift (`asr`), of a u32 a
 *   logical one (`lsr`), exactly as the spelled-out `>> 8` was. Keep the
 *   operand's own cast inside the argument: `Q8_TO_INT((s16)x)`.
 * - The operands keep their order: Q8_MUL(a, b) is `(a * b) >> 8`, not
 *   `(b * a) >> 8`, and MIN(a, b) compares `a < b`. Convert a site only
 *   when its operands are in the macro's order.
 * - Expression and statement forms are different code. MIN/MAX/ABS/CLAMP
 *   are ternaries and replace ternaries; the ROM's `if (x > hi) x = hi;`
 *   statements are LIMIT_MAX/LIMIT_MIN/CLAMP_INDEX/MAKE_ABS, which
 *   expand to that `if`. A statement macro is a bare `if` (a do/while(0)
 *   wrapper would add loop notes and can change the object), so don't
 *   use one as the body of an `if` that has an `else`.
 * - Arguments may be evaluated twice (MIN, MAX, ABS, CLAMP and the
 *   statement forms); the sites they replace evaluated them twice too.
 *
 * Not every `>> 8` is a fixed-point conversion (a byte extraction or a
 * divide by 256 isn't): convert the ones that are. tools/common_ops.py
 * lists the remaining spelled-out sites. */

/* Fixed point. Q8 (24.8: positions, velocities, `animTime`) is the
 * game's main format; some code also uses Q12 and Q16. Check what a
 * `>> 16` is before converting it: `(x << 16) >> 16` is a sign extension
 * to 16 bits, not a conversion. */
#define Q8_TO_INT(x) ((x) >> 8)
#define INT_TO_Q8(x) ((x) << 8)
#define Q12_TO_INT(x) ((x) >> 12)
#define INT_TO_Q12(x) ((x) << 12)
#define Q16_TO_INT(x) ((x) >> 16)
#define INT_TO_Q16(x) ((x) << 16)

/* The Q8 product of two Q8 values (or a Q8 value and a Q8 scale):
 * multiplies, then drops the extra 8 fraction bits. Overflows for large
 * operands, unlike util/fixed_math.c's FixedMul, which shifts first. */
#define Q8_MUL(a, b) (((a) * (b)) >> 8)

/* Generic expression helpers (ternaries). */
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

/* Statement helpers: the ROM's clamp `if`s, in place on an lvalue. */
#define LIMIT_MAX(x, hi) if ((x) > (hi)) (x) = (hi)
#define LIMIT_MIN(x, lo) if ((x) < (lo)) (x) = (lo)
#define MAKE_ABS(x) if ((x) < 0) (x) = -(x)
/* Clamps an index to the last valid one of `n` (a frame index to the
 * animation's frame count, ...): `if (i >= n) i = n - 1`. */
#define CLAMP_INDEX(i, n) if ((i) >= (n)) (i) = (n) - 1

/* The sine table (gSineTable, globals.h): 256 steps per turn, scaled by
 * 0x100, so the result is Q8. `angle` is masked to the table; the
 * cosine is the sine a quarter turn (0x40) on. These are the plain
 * lookups; a site that reads the table through a local pointer (often a
 * register pin) or with another mask keeps its own spelling. */
#define SIN_Q8(angle) (gSineTable[(angle) & 0xFF])
#define COS_Q8(angle) (gSineTable[((angle) + 0x40) & 0xFF])

#endif // GUARD_MATH_UTIL_H
