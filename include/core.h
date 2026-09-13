#ifndef __CORE_H__
#define __CORE_H__

#include "gba/gba.h"

/* Set to 1 (via `make NON_MATCHING=1`) to compile in-progress C
 * reconstructions that aren't byte-matching yet, guarded with
 * `#if NON_MATCHING` at their definition, in place of the checked-in
 * assembly for the same function (which is correspondingly guarded with
 * `.if NON_MATCHING == 0` in its .s file). Default (matching) builds
 * never define this, so it must default to 0 here rather than relying on
 * undefined-is-0 preprocessor behavior everywhere it's used. */
#ifndef NON_MATCHING
#define NON_MATCHING 0
#endif

#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1]
// token pasting madness:
#define COMPILE_TIME_ASSERT3(X,L) STATIC_ASSERT(X,static_assertion_at_line_##L)
#define COMPILE_TIME_ASSERT2(X,L) COMPILE_TIME_ASSERT3(X,L)
#define COMPILE_TIME_ASSERT(X)    COMPILE_TIME_ASSERT2(X,__LINE__)

#endif /* __CORE_H__ */
