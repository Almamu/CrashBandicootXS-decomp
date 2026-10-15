#ifndef GUARD_UTIL_H
#define GUARD_UTIL_H

/* The util subsystem (engine/src/engine/util/): box tests, fixed-point math, the RNG,
 * the Bresenham line stepper and the string/number formatting helpers.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers.md, "Codegen exceptions").
 *
 * DestroyLargeFont/DestroySmallFont (engine/src/engine/util/aabb_setters.cpp) are
 * LargeFont's and SmallFont's destructors (engine/include/engine/text/font.hpp).
 * strcpy/strlen (engine/src/engine/util/string.cpp, defined under the C names
 * CopyString/StringLength to avoid gcc's built-ins) are not declared
 * here: nothing outside string.cpp calls them. */

#include "core.h"
#include "engine/util/aabb.h"
#include "engine/util/line_util.h"

/* The RNG's state (engine/src/engine/sys/iwram_data.cpp). */
extern u32 gRandSeed;

/* engine/src/engine/util/aabb.cpp */
extern u8 AabbOverlapsInclusiveX(struct aabb *a, struct aabb *b);
extern u8 AabbOverlaps(struct aabb *a, struct aabb *b);

/* engine/src/engine/sys/iwram_alloc.cpp */
extern void IwramFree(void *address);
extern void *IwramAlloc(u32 size);

/* engine/src/engine/util/aabb_setters.cpp */
extern void SetAabbSize(struct aabb *dest, s32 w, s32 h);
extern void SetAabbPos(struct aabb *dest, s32 x, s32 y);

/* Both setters, position first. Through this inline, a stack box's
 * address is the constant `frame + offset` in each call's argument
 * (aabb.h's accessors), so each call computes `add r0, sp, #N` for
 * itself, as the ROM does; called directly with `&box`, each argument
 * is a pseudo that cse1 ties to the first one, held across the calls
 * (#662 round 4). */
static inline void SetAabb(struct aabb *dest, s32 x, s32 y, s32 w, s32 h)
{
    SetAabbPos(dest, x, y);
    SetAabbSize(dest, w, h);
}

/* engine/src/engine/util/fixed_math.cpp */
extern s32 FixedDistSq(s32 x1, s32 x2, s32 y1, s32 y2);
extern u32 FixedDist(s32 x1, s32 x2, s32 y1, s32 y2);
extern s32 FixedDiv(s32 numerator, s32 denominator);
extern s32 FixedMul(s32 a, s32 b);
extern s32 FixedInverse16(s32 denominator);
extern s32 FixedDiv16(s32 numerator, s32 denominator);
extern s32 FixedMul16(s32 lhs, s32 rhs);

/* engine/src/engine/util/line.cpp, engine/src/engine/util/line_step.cpp */
extern void InitBresenhamLine(struct bresenham_line *line);
extern void StepBresenhamLine(struct bresenham_line *line);

/* engine/src/engine/util/number_format.cpp */
extern s32 itoa(s32 value, u8 *buffer, s32 base);
extern u8 *FormatPaddedNumber(u8 *dest, u8 *fmt, s32 *valuePtr, u8 padChar, s32 *charsConsumedPtr);

/* engine/src/engine/util/printf.cpp */
extern void vsprintf(u8 *dest, u8 *fmt, uintptr_t *args);
extern void sprintf(u8 *dest, u8 *fmt, ...);

/* engine/src/engine/util/rand.cpp */
extern void srand(u32 seed);
extern u16 RandRange(s32 max);
extern u16 rand(void);

/* engine/src/engine/util/string.cpp */
extern u8 *FindSubstring(u8 *haystack, u8 *needle, s32 caseInsensitive);
extern s32 CountNonSpaceChars(u8 *s);
extern void strcat(u8 *dst, u8 *src);
extern void strncpy(u8 *dst, u8 *src, s32 n);

/* engine/src/engine/util/time_format.cpp */
extern void FormatCentiseconds(s32 value, u8 *buf);

#endif /* GUARD_UTIL_H */
