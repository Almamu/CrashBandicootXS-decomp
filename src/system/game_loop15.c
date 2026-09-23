#include "core.h"

extern void sub_8024DAC(void *self, s32 bgIndex);
extern u8 gStaticData_087E4C14[];

/* Initializes a BG-scroll-layer object (see game_loop6.c's viewport/
 * parallax-scroll-layer family) for hardware BG `bgIndex`: caches
 * `gStaticData_087E4C14` at `+0x30`, a packed affine-ish constant at
 * `+0x4c`, the `BGnCNT` register address at `+0x38`
 * (`0x04000008 + bgIndex*2`), the `BGnHOFS` register address at `+0x58`
 * (`0x04000010 + bgIndex*4`), and two bitfields at `+0x34`/`+0x35`
 * (a 5-bit `(bgIndex+0x1c) & 0x1f` value packed into `+0x35`, and a
 * fixed `8` packed into `+0x34`'s low nibble).
 *
 * Was NAKED asm, not plain C - see
 * docs/matching/naked-sub_8025d74-matched.md for the derivation. The
 * `& -0x20`/`& -0xd` masks always fold to their positive
 * byte-immediate equivalent instead of the ROM's runtime `movs`+`rsbs`
 * negation, closed the same way as `sub_8001524`/`sub_80109A4`:
 * materializing each fold as an opaque inline-asm block. A second gap
 * (the three pointer-sized constants this function loads all need to
 * land in one shared literal pool, in the ROM's own order, for the
 * function to stay byte-exact - letting even one of them fall back to
 * an ordinary C reference lets the compiler's own pool disagree on
 * count/order with the other two) is closed by materializing all
 * three loads too, against one explicit trailing pool this function
 * owns outright. */
void *sub_8025D74(void *self, s32 bgIndex)
{
    register u8 *s asm("r5") = (u8 *)self;
    register s32 idx asm("r4") = bgIndex;
    register s32 t asm("r1");
    register u8 *addr34 asm("r2");
    register u8 *addr35 asm("r3");

    sub_8024DAC(self, bgIndex);

    { register void *gsPtr asm("r0");
      asm volatile("ldr %0, 90f" : "=r"(gsPtr));
      *(void **)(s + 0x30) = gsPtr; }

    t = idx + 0x1c;
    *(s32 *)(s + 0x4c) = (t << 0xb) + (0xc0 << 0x13);

    { register s32 shifted1 asm("r0") = idx << 1;
      register s32 bgnCntAddr asm("r3");
      asm volatile("ldr %0, 90f+4" : "=r"(bgnCntAddr));
      *(s32 *)(s + 0x38) = shifted1 + bgnCntAddr; }

    idx = idx << 2;
    { register s32 bgnHofsAddr asm("r0");
      asm volatile("ldr %0, 90f+8" : "=r"(bgnHofsAddr));
      idx = idx + bgnHofsAddr; }
    *(s32 *)(s + 0x58) = idx;

    *(u16 *)(s + 0x34) = 0;

    addr34 = s + 0x34;
    asm volatile(
        "mov r0, #0x7f\n\t"
        "ldrb r3, [%0]\n\t"
        "and r0, r0, r3\n\t"
        "strb r0, [%0]\n\t"
        : : "r"(addr34) : "r0", "r3", "memory"
    );

    addr35 = s + 0x35;
    t = t & 0x1f;
    asm volatile(
        "mov r0, #0x20\n\t"
        "neg r0, r0\n\t"
        "ldrb r4, [%0]\n\t"
        "and r0, r0, r4\n\t"
        "orr r0, r0, %1\n\t"
        "strb r0, [%0]\n\t"
        : : "r"(addr35), "r"(t) : "r0", "r4", "memory"
    );

    asm volatile(
        "mov r0, #0xd\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [%0]\n\t"
        "and r0, r0, r1\n\t"
        "mov r1, #8\n\t"
        "orr r0, r0, r1\n\t"
        "strb r0, [%0]\n\t"
        : : "r"(addr34) : "r0", "r1", "memory"
    );

    return (void *)s;
}
asm(".align 2, 0\n90: .word gStaticData_087E4C14\n.word 0x04000008\n.word 0x04000010");

extern void sub_803AD80(void *arg0, s32 arg1, void *fn);

/* Grows `self+0x3c` down to `lo` and `self+0x40` up to `hi` one step
 * at a time, firing `self->0x30`'s `+0x30`-offset/`+0x34`-fn trampoline
 * (via `sub_803AD80`, an interworking veneer picked automatically by
 * the compiler for indirect calls - see `sub_8025D28` in
 * game_loop14.c) after every step - the streamed-tile-range grower
 * `sub_8025E98` drives for one axis; `sub_8025E2C` is its twin for the
 * other axis's `+0x44`/`+0x48` fields. */
void sub_8025DE8(void *self, s32 lo, s32 hi)
{
    while (*(s32 *)((u8 *)self + 0x3c) > lo) {
        u8 *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = *(s32 *)((u8 *)self + 0x3c) - 1;
        *(s32 *)((u8 *)self + 0x3c) = v;
        layer = *(u8 **)((u8 *)self + 0x30);
        off = *(s16 *)(layer + 0x30);
        addr = (u8 *)self + off;
        fn = *(void **)(layer + 0x34);
        sub_803AD80(addr, v, fn);
    }
    while (*(s32 *)((u8 *)self + 0x40) < hi) {
        u8 *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = *(s32 *)((u8 *)self + 0x40) + 1;
        *(s32 *)((u8 *)self + 0x40) = v;
        layer = *(u8 **)((u8 *)self + 0x30);
        off = *(s16 *)(layer + 0x30);
        addr = (u8 *)self + off;
        fn = *(void **)(layer + 0x34);
        sub_803AD80(addr, v, fn);
    }
}

/* Same shape as `sub_8025DE8` above, but grows `self+0x44`/`self+0x48`
 * via `self->0x30`'s `+0x38`-offset/`+0x3c`-fn trampoline instead. */
void sub_8025E2C(void *self, s32 lo, s32 hi)
{
    while (*(s32 *)((u8 *)self + 0x44) > lo) {
        u8 *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = *(s32 *)((u8 *)self + 0x44) - 1;
        *(s32 *)((u8 *)self + 0x44) = v;
        layer = *(u8 **)((u8 *)self + 0x30);
        off = *(s16 *)(layer + 0x38);
        addr = (u8 *)self + off;
        fn = *(void **)(layer + 0x3c);
        sub_803AD80(addr, v, fn);
    }
    while (*(s32 *)((u8 *)self + 0x48) < hi) {
        u8 *layer;
        s32 off;
        void *addr;
        void *fn;
        s32 v;

        v = *(s32 *)((u8 *)self + 0x48) + 1;
        *(s32 *)((u8 *)self + 0x48) = v;
        layer = *(u8 **)((u8 *)self + 0x30);
        off = *(s16 *)(layer + 0x38);
        addr = (u8 *)self + off;
        fn = *(void **)(layer + 0x3c);
        sub_803AD80(addr, v, fn);
    }
}

/* Clamp-to-at-least/at-most pair on `self+0x44`(max with `a`)/
 * `self+0x48`(min with `b`) - the bookkeeping half of the
 * `sub_8025DE8`/`sub_8025E2C` streamed-range growers above (this
 * variant just widens the recorded extent, without firing any
 * trampoline). */
void sub_8025E70(void *self, s32 a, s32 b)
{
    if (*(s32 *)((u8 *)self + 0x44) < a) {
        *(s32 *)((u8 *)self + 0x44) = a;
    }
    if (*(s32 *)((u8 *)self + 0x48) > b) {
        *(s32 *)((u8 *)self + 0x48) = b;
    }
}

/* Same shape as `sub_8025E70` above, on `self+0x3c`/`self+0x40`. */
void sub_8025E84(void *self, s32 a, s32 b)
{
    if (*(s32 *)((u8 *)self + 0x3c) < a) {
        *(s32 *)((u8 *)self + 0x3c) = a;
    }
    if (*(s32 *)((u8 *)self + 0x40) > b) {
        *(s32 *)((u8 *)self + 0x40) = b;
    }
}
