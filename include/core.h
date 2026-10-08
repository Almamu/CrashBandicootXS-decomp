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

/* The number of elements of an array (not a pointer): a compile-time
 * constant of type size_t. */
#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

#define STATIC_ASSERT(COND, MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1]
/* COMPILE_TIME_ASSERT(TAG, COND): TAG is the file's name with the dot
 * replaced (`frontend_hpp`, `actor_spawn_c`). The typedef is named after
 * the tag and __LINE__, so asserts in different files can sit on the same
 * line number (agbcc has no __COUNTER__). A typedef emits no code. */
#define COMPILE_TIME_ASSERT3(T, X, L) STATIC_ASSERT(X,T##_at_line_##L)
#define COMPILE_TIME_ASSERT2(T, X, L) COMPILE_TIME_ASSERT3(T,X,L)
#define COMPILE_TIME_ASSERT(T, X)     COMPILE_TIME_ASSERT2(T,X,__LINE__)

#endif /* __CORE_H__ */
