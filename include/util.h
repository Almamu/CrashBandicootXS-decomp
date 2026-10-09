#ifndef GUARD_UTIL_H
#define GUARD_UTIL_H

/* The util subsystem (src/util/): box tests, fixed-point math, the RNG,
 * the Bresenham line stepper and the string/number formatting helpers.
 *
 * Declarations here are the functions' real prototypes, copied from
 * their definitions. A .c file that needs a different local declaration
 * for codegen keeps it as an asm-label alias with a `codegen:` comment
 * (docs/headers_plan.md, "Codegen exceptions").
 *
 * DestroyLargeFont/DestroySmallFont (src/util/aabb_setup.cpp) are
 * LargeFont's and SmallFont's destructors (include/font.hpp).
 * strcpy/strlen (src/util/string.cpp, defined under the C names
 * CopyString/StringLength to avoid gcc's built-ins) are not declared
 * here: nothing outside string.cpp calls them. */

#include "core.h"
#include "aabb.h"
#include "line_util.h"

/* The RNG's state (src/iwram/iwram_data.cpp). */
extern u32 gRandSeed;

/* src/util/aabb.cpp */
extern void CommitBlendRegs(void);
extern u8 AabbOverlapsInclusiveX(struct aabb *a, struct aabb *b);
extern u8 AabbOverlaps(struct aabb *a, struct aabb *b);
extern void IwramFree(u8 *address);
extern void *IwramAlloc(u32 size);

/* src/util/aabb_setup.cpp */
extern void SetAabbSize(struct aabb *dest, s32 w, s32 h);
extern void SetAabbPos(struct aabb *dest, s32 x, s32 y);

/* src/util/fixed_math.cpp */
extern s32 FixedDistSq(s32 x1, s32 x2, s32 y1, s32 y2);
extern u32 FixedDist(s32 x1, s32 x2, s32 y1, s32 y2);
extern s32 FixedDiv(s32 a, s32 b);
extern s32 FixedMul(s32 a, s32 b);
extern s32 FixedInverse16(s32 a);
extern s32 FixedDiv16(s32 a, s32 b);
extern s32 FixedMul16(s32 a, s32 b);

/* src/util/line.cpp, src/util/line_step.cpp */
extern void InitBresenhamLine(struct bresenham_line *line);
extern void StepBresenhamLine(struct bresenham_line *line);

/* src/util/number_format.cpp */
extern s32 itoa(s32 value, u8 *buffer, s32 base);
extern u8 *FormatPaddedNumber(u8 *dest, u8 *fmt, s32 *valuePtr, u8 padChar, s32 *charsConsumedPtr);

/* src/util/printf.cpp */
extern void vsprintf(u8 *dest, u8 *fmt, u32 *args);
extern void sprintf(u8 *dest, u8 *fmt, ...);

/* src/util/rand.cpp */
extern void srand(u32 seed);
extern u16 RandRange(s32 max);
extern u16 rand(void);

/* src/util/string.cpp */
extern u8 *FindSubstring(u8 *haystack, u8 *needle, s32 caseInsensitive);
extern s32 CountNonSpaceChars(u8 *s);
extern void strcat(u8 *dst, u8 *src);
extern void strncpy(u8 *dst, u8 *src, s32 n);

/* src/util/time_format.cpp */
extern void FormatCentiseconds(s32 value, u8 *buf);

#endif /* GUARD_UTIL_H */
