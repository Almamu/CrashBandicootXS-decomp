#ifndef __LIBGCC_H__
#define __LIBGCC_H__

#include "gba/types.h"

/* The libgcc support routines of lib/libgcc: the 32-bit division and
 * modulo routines of lib1funcs.s and the 64-bit ones of libgcc2.c.
 * The compiler calls them for `/` and `%` on its own; the game's C calls
 * the 32-bit ones by name where the ROM does (a `%` would let gcc pick
 * the signedness, or fold a constant divisor into a shift).
 *
 * `_call_via_r0`..`_call_via_r7` are not declared here: each call site
 * declares the shape it calls with (docs/headers_plan.md, rule 5). */

extern u32 __udivsi3(u32 dividend, u32 divisor);
extern s32 __divsi3(s32 dividend, s32 divisor);
extern s32 __modsi3(s32 dividend, s32 divisor);
extern u32 __umodsi3(u32 dividend, u32 divisor);

extern s64 __divdi3(s64 u, s64 v);
extern u64 __udivdi3(u64 n, u64 d);
extern s64 __muldi3(s64 u, s64 v);

#endif /* __LIBGCC_H__ */
