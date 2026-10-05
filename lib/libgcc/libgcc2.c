#include "gba/types.h"
#include <libgcc.h>

/* gcc 2.x's libgcc2.c, the 64-bit arithmetic helpers `__divdi3`,
 * `__udivdi3` and `__muldi3`. As in gcc's own build, this one file is
 * compiled once per routine with `-DL_<name>` (Makefile LIBGCC2_FUNCS),
 * giving one object per function: _divdi3.o, _udivdi3.o, _muldi3.o.
 * lib1funcs.asm's `__udivsi3` (lib1funcs.s) sits between _udivdi3.o and
 * _muldi3.o in the ROM.
 *
 * These copies were linked in with the GAX2 library (they sit inside its
 * address range, ahead of the engine code), separately from the game's
 * own lib1funcs routines at the end of the code (`__divsi3`/`__modsi3`/
 * `__umodsi3`/`_call_via_rN`, lib1funcs.s).
 *
 * Built WITHOUT `-mthumb-interwork` (Makefile NO_INTERWORK_OBJS): all
 * three return via a combined `pop {r4-r7, pc}`, a shape the rest of
 * this ROM never uses and agbcc only emits when interworking is off.
 * With that flag dropped, `__divdi3`, `__udivdi3` and `__muldi3` below
 * are libgcc2.c *verbatim* (generic `longlong.h` macros -
 * `__udiv_qrnnd_c`, the `__clz_tab` lookup `count_leading_zeros`,
 * `umul_ppmm` - plus the `UDIV_NEEDS_NORMALIZATION` branch of
 * `__udivmoddi4`) and match byte-for-byte under agbcc. Each of the two
 * division objects carried its own static `__clz_tab`, as old
 * libgcc2.c did (data/clz_tab_5a4c70.c). See
 * docs/matching/gax-toolchain-retry.md.
 *
 * `__divdi3` is UNUSED - nothing in the ROM calls it; it rode along
 * with the GAX2 library. `__udivdi3` is called from `GaxChannelInit`. */

typedef unsigned int USItype;
typedef int SItype;
typedef long long DItype;
typedef unsigned long long UDItype;
typedef unsigned char UQItype;
typedef int word_type;

struct DIstruct {
    SItype low, high;
};

typedef union {
    struct DIstruct s;
    DItype ll;
} DIunion;

/* `/` and `%` on USItype below compile to calls to lib1funcs.s's
 * `__udivsi3` and `__umodsi3`. */

extern void __div0(void);
extern const UQItype __clz_tab_divdi3[256];
extern const UQItype __clz_tab_udivdi3[256];

#define SI_TYPE_SIZE 32
#define __BITS4 (SI_TYPE_SIZE / 4)
#define __ll_B (1L << (SI_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((USItype)(t) % __ll_B)
#define __ll_highpart(t) ((USItype)(t) / __ll_B)

#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
    do {                                   \
        USItype __x;                       \
        __x = (al) - (bl);                 \
        (sh) = (ah) - (bh) - (__x > (al)); \
        (sl) = __x;                        \
    } while (0)

#define umul_ppmm(w1, w0, u, v)                                         \
    do {                                                                \
        USItype __x0, __x1, __x2, __x3;                                 \
        USItype __ul, __vl, __uh, __vh;                                 \
                                                                        \
        __ul = __ll_lowpart(u);                                         \
        __uh = __ll_highpart(u);                                        \
        __vl = __ll_lowpart(v);                                         \
        __vh = __ll_highpart(v);                                        \
                                                                        \
        __x0 = (USItype)__ul * __vl;                                    \
        __x1 = (USItype)__ul * __vh;                                    \
        __x2 = (USItype)__uh * __vl;                                    \
        __x3 = (USItype)__uh * __vh;                                    \
                                                                        \
        __x1 += __ll_highpart(__x0); /* this can't give carry */        \
        __x1 += __x2;                /* but this indeed can */          \
        if (__x1 < __x2)             /* did we get it? */               \
            __x3 += __ll_B;          /* yes, add it in the proper pos. */ \
                                                                        \
        (w1) = __x3 + __ll_highpart(__x1);                              \
        (w0) = __ll_lowpart(__x1) * __ll_B + __ll_lowpart(__x0);        \
    } while (0)

#define udiv_qrnnd(q, r, n1, n0, d)                   \
    do {                                              \
        USItype __d1, __d0, __q1, __q0;               \
        USItype __r1, __r0, __m;                      \
        __d1 = __ll_highpart(d);                      \
        __d0 = __ll_lowpart(d);                       \
                                                      \
        __r1 = (n1) % __d1;                           \
        __q1 = (n1) / __d1;                           \
        __m = (USItype)__q1 * __d0;                   \
        __r1 = __r1 * __ll_B | __ll_highpart(n0);     \
        if (__r1 < __m) {                             \
            __q1--, __r1 += (d);                      \
            if (__r1 >= (d))                          \
                if (__r1 < __m)                       \
                    __q1--, __r1 += (d);              \
        }                                             \
        __r1 -= __m;                                  \
                                                      \
        __r0 = __r1 % __d1;                           \
        __q0 = __r1 / __d1;                           \
        __m = (USItype)__q0 * __d0;                   \
        __r0 = __r0 * __ll_B | __ll_lowpart(n0);      \
        if (__r0 < __m) {                             \
            __q0--, __r0 += (d);                      \
            if (__r0 >= (d))                          \
                if (__r0 < __m)                       \
                    __q0--, __r0 += (d);              \
        }                                             \
        __r0 -= __m;                                  \
                                                      \
        (q) = (USItype)__q1 * __ll_B | __q0;          \
        (r) = __r0;                                   \
    } while (0)

#define count_leading_zeros(clz_tab, count, x)                                   \
    do {                                                                         \
        USItype __xr = (x);                                                      \
        USItype __a;                                                             \
                                                                                 \
        __a = __xr < ((USItype)1 << 2 * __BITS4)                                 \
            ? (__xr < ((USItype)1 << __BITS4) ? 0 : __BITS4)                     \
            : (__xr < ((USItype)1 << 3 * __BITS4) ? 2 * __BITS4 : 3 * __BITS4);  \
                                                                                 \
        (count) = SI_TYPE_SIZE - ((clz_tab)[__xr >> __a] + __a);                 \
    } while (0)

static inline DItype __negdi2(DItype u)
{
    DIunion w;
    DIunion uu;

    uu.ll = u;

    w.s.low = -uu.s.low;
    w.s.high = -uu.s.high - ((USItype)w.s.low > 0);

    return w.ll;
}

#ifdef L_divdi3
#define UDIVMODDI4 __udivmoddi4
#define CLZ_TAB __clz_tab_divdi3
#include "libgcc2_udivmoddi4.h"

/* `__divdi3`: signed 64-bit division, truncating toward zero. UNUSED. */
DItype __divdi3(DItype u, DItype v)
{
    word_type c = 0;
    DIunion uu, vv;
    DItype w;

    uu.ll = u;
    vv.ll = v;

    if (uu.s.high < 0)
        c = ~c,
        uu.ll = __negdi2(uu.ll);
    if (vv.s.high < 0)
        c = ~c,
        vv.ll = __negdi2(vv.ll);

    w = __udivmoddi4(uu.ll, vv.ll, (UDItype *)0);
    if (c)
        w = __negdi2(w);

    return w;
}

#endif /* L_divdi3 */

#ifdef L_udivdi3
#define UDIVMODDI4 __udivmoddi4
#define CLZ_TAB __clz_tab_udivdi3
#include "libgcc2_udivmoddi4.h"

/* `__udivdi3`: unsigned 64-bit division. */
UDItype __udivdi3(UDItype n, UDItype d)
{
    return __udivmoddi4(n, d, (UDItype *)0);
}


#endif /* L_udivdi3 */

#ifdef L_muldi3
/* `__muldi3`: 64x64->64 truncating multiply (`__umulsidi3` inlined). */
DItype __muldi3(DItype u, DItype v)
{
    DIunion w;
    DIunion uu, vv;

    uu.ll = u,
    vv.ll = v;

    {
        DIunion __w;
        umul_ppmm(__w.s.high, __w.s.low, uu.s.low, vv.s.low);
        w.ll = __w.ll;
    }
    w.s.high += ((USItype)uu.s.low * (USItype)vv.s.high
                 + (USItype)uu.s.high * (USItype)vv.s.low);

    return w.ll;
}
#endif /* L_muldi3 */
