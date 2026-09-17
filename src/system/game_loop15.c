#include "core.h"

#if NON_MATCHING
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
 * PARKED, NOT BYTE-MATCHING: every field/offset confirmed against the
 * ROM. Two gaps remain: (1) the `& -0x20`/`& -0xd` masks - written as
 * negative-literal register pins matching this codebase's established
 * idiom (see `sub_8009F50` in actor_part8.c) - correctly produce the
 * ROM's two-instruction `mov`+`neg` sequence for `-0xd`, but the
 * `-0x20` mask still gets folded to a single `mov r0, #0xe0` immediate
 * regardless of how it's expressed in C; (2) hoisting `bgIndex+0x1c`
 * into its own local (needed since the ROM keeps it alive in a single
 * register from just after the `sub_8024DAC` call through the `+0x35`
 * bitfield store) pulls in an extra callee-saved register the ROM's
 * 3-register (`r4`/`r5`) frame doesn't need. Parked - see
 * docs/matching/issue-41-game-loop-25894.md. */
void *sub_8025D74(void *self, s32 bgIndex)
{
    u8 *s = (u8 *)self;

    sub_8024DAC(self, bgIndex);

    *(void **)(s + 0x30) = gStaticData_087E4C14;
    *(s32 *)(s + 0x4c) = ((bgIndex + 0x1c) << 0xb) + (0xc0 << 0x13);
    *(s32 *)(s + 0x38) = 0x04000008 + (bgIndex << 1);
    *(s32 *)(s + 0x58) = 0x04000010 + (bgIndex << 2);
    *(u16 *)(s + 0x34) = 0;
    *(u8 *)(s + 0x34) &= 0x7f;
    *(u8 *)(s + 0x35) = (*(u8 *)(s + 0x35) & -0x20) | ((bgIndex + 0x1c) & 0x1f);
    *(u8 *)(s + 0x34) = (*(u8 *)(s + 0x34) & -0xd) | 8;

    return self;
}
#endif /* NON_MATCHING */
asm(".align 2, 0");

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
