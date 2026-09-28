#include "core.h"
#include "gba/defines.h"

/* The GAX2 library's own bundled copy of the libgcc 64-bit arithmetic
 * helpers: `sub_8037648` = `__divdi3`, `sub_8037A7C` = `__udivdi3`,
 * `sub_8037E54` = `__udivsi3`, `sub_8037ECC` = `__muldi3`. (The game's
 * own copies of the 32-bit helpers are `math_div_util.c`'s
 * `sub_803ADB4`/`sub_803AE4C`/`sub_803AF1C`.)
 *
 * This translation unit is built WITHOUT `-mthumb-interwork` (see the
 * Makefile): all four return via a combined `pop {r4-r7, pc}` / `mov
 * pc, lr`, a shape the rest of this ROM never uses and agbcc only emits
 * when interworking is off. With that flag dropped, `__divdi3`,
 * `__udivdi3` and `__muldi3` below are gcc 2.x's `libgcc2.c` *verbatim*
 * (generic `longlong.h` macros - `__udiv_qrnnd_c`, the `__clz_tab`
 * lookup `count_leading_zeros`, `umul_ppmm` - plus the
 * `UDIV_NEEDS_NORMALIZATION` branch of `__udivmoddi4`) and match
 * byte-for-byte under agbcc. Each of the two division objects carried
 * its own static `__clz_tab` (`gStaticData_085A4C70`/`085A4D70`), as
 * old libgcc2.c did.
 *
 * `sub_8037E54` (`__udivsi3`) stays a NAKED transcription: it's
 * `lib1funcs.asm`'s hand-written Thumb routine (shift-4-then-1
 * normalization, per-path `push {r4}`/`push {lr}` with `bl __div0` =
 * `nullsub_8` on a zero divisor), not compiler output - the same
 * situation as `math_div_util.c`'s `__divsi3`/`__modsi3`/`__umodsi3`.
 *
 * `sub_8037648` (`__divdi3`) is UNUSED - nothing in the ROM calls it;
 * it rode along with the GAX2 library. `sub_8037A7C` is called from
 * `sub_8039518`. */

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

/* `/` and `%` on USItype below compile to these. */
asm(".set __udivsi3, sub_8037E54\n"
    ".set __umodsi3, sub_803AF1C\n");

extern void nullsub_8(void);
extern const UQItype gStaticData_085A4C70[256];
extern const UQItype gStaticData_085A4D70[256];

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

#define UDIVMODDI4 __udivmoddi4_divdi3
#define CLZ_TAB gStaticData_085A4C70
#include "libgcc2_udivmoddi4.h"
#undef UDIVMODDI4
#undef CLZ_TAB

#define UDIVMODDI4 __udivmoddi4_udivdi3
#define CLZ_TAB gStaticData_085A4D70
#include "libgcc2_udivmoddi4.h"
#undef UDIVMODDI4
#undef CLZ_TAB

/* `__divdi3`: signed 64-bit division, truncating toward zero. UNUSED. */
DItype sub_8037648(DItype u, DItype v)
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

    w = __udivmoddi4_divdi3(uu.ll, vv.ll, (UDItype *)0);
    if (c)
        w = __negdi2(w);

    return w;
}

/* `__udivdi3`: unsigned 64-bit division. */
UDItype sub_8037A7C(UDItype n, UDItype d)
{
    return __udivmoddi4_udivdi3(n, d, (UDItype *)0);
}

/* `__udivsi3`: unsigned 32-bit division, quotient only. Callers
 * project-wide declare it `s32 sub_8037E54(s32 value, s32 divisor)`.
 * NAKED because it's lib1funcs.asm's hand-written routine (see the
 * header comment): its per-path `push {r4}` ... `mov pc, lr` vs `push
 * {lr}; bl __div0; ...; pop {pc}` shape isn't compiler output. */
NAKED s32 sub_8037E54(s32 value, s32 divisor)
{
    asm(
        "cmp r1, #0\n\t"
        "beq 10f\n\t"
        "mov r3, #1\n\t"
        "mov r2, #0\n\t"
        "push {r4}\n\t"
        "cmp r0, r1\n\t"
        "blo 9f\n\t"
        "mov r4, #1\n\t"
        "lsl r4, r4, #0x1c\n\t"
    "1:\n\t"
        "cmp r1, r4\n\t"
        "bhs 2f\n\t"
        "cmp r1, r0\n\t"
        "bhs 2f\n\t"
        "lsl r1, r1, #4\n\t"
        "lsl r3, r3, #4\n\t"
        "b 1b\n\t"
    "2:\n\t"
        "lsl r4, r4, #3\n\t"
    "3:\n\t"
        "cmp r1, r4\n\t"
        "bhs 4f\n\t"
        "cmp r1, r0\n\t"
        "bhs 4f\n\t"
        "lsl r1, r1, #1\n\t"
        "lsl r3, r3, #1\n\t"
        "b 3b\n\t"
    "4:\n\t"
        "cmp r0, r1\n\t"
        "blo 5f\n\t"
        "sub r0, r0, r1\n\t"
        "orr r2, r3\n\t"
    "5:\n\t"
        "lsr r4, r1, #1\n\t"
        "cmp r0, r4\n\t"
        "blo 6f\n\t"
        "sub r0, r0, r4\n\t"
        "lsr r4, r3, #1\n\t"
        "orr r2, r4\n\t"
    "6:\n\t"
        "lsr r4, r1, #2\n\t"
        "cmp r0, r4\n\t"
        "blo 7f\n\t"
        "sub r0, r0, r4\n\t"
        "lsr r4, r3, #2\n\t"
        "orr r2, r4\n\t"
    "7:\n\t"
        "lsr r4, r1, #3\n\t"
        "cmp r0, r4\n\t"
        "blo 8f\n\t"
        "sub r0, r0, r4\n\t"
        "lsr r4, r3, #3\n\t"
        "orr r2, r4\n\t"
    "8:\n\t"
        "cmp r0, #0\n\t"
        "beq 9f\n\t"
        "lsr r3, r3, #4\n\t"
        "beq 9f\n\t"
        "lsr r1, r1, #4\n\t"
        "b 4b\n\t"
    "9:\n\t"
        "add r0, r2, #0\n\t"
        "pop {r4}\n\t"
        "mov pc, lr\n\t"
    "10:\n\t"
        "push {lr}\n\t"
        "bl nullsub_8\n\t"
        "mov r0, #0\n\t"
        "pop {pc}\n\t"
    );
}
asm(".align 2, 0");

/* `__muldi3`: 64x64->64 truncating multiply (`__umulsidi3` inlined). */
DItype sub_8037ECC(DItype u, DItype v)
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
