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
 * NAKED, not plain C: every field/offset is confirmed against the ROM
 * (see the plain-C reconstruction this replaced, still visible in git
 * history) and a real C version gets everything right except the
 * `& -0x20`/`& -0xd` masks, which always fold to their positive
 * byte-immediate equivalent (`0xe0`/`0xf3`) instead of the ROM's
 * runtime `movs`+`rsbs` negation - tried the established
 * negative-literal register-pin idiom (`sub_8023168`/`sub_80374D0` in
 * docs/matching.md) but this compiler's constant folding still
 * collapses the pinned mask onto the previously-loaded `0x1f`/`-0x20`
 * constant via a cheaper `subs`/`adds`, the same unfixable
 * value-propagation documented on `sub_8001524` (docs/matching.md) and
 * independently reconfirmed on `sub_8025A64`'s identical `& -0x10`
 * idiom (game_loop29.c) - no C-level phrasing found stops it.
 * Transcribed straight from the confirmed-correct ROM disassembly. */
NAKED void *sub_8025D74(void *self, s32 bgIndex)
{
    asm(
        "push {r4, r5, lr}\n\t"
        "add r5, r0, #0\n\t"
        "add r4, r1, #0\n\t"
        "bl sub_8024DAC\n\t"
        "ldr r0, 1f\n\t"
        "str r0, [r5, #0x30]\n\t"
        "add r1, r4, #0\n\t"
        "add r1, r1, #0x1c\n\t"
        "lsl r0, r1, #0xb\n\t"
        "mov r2, #0xc0\n\t"
        "lsl r2, r2, #0x13\n\t"
        "add r0, r0, r2\n\t"
        "str r0, [r5, #0x4c]\n\t"
        "lsl r0, r4, #1\n\t"
        "ldr r3, 2f\n\t"
        "add r0, r0, r3\n\t"
        "str r0, [r5, #0x38]\n\t"
        "lsl r4, r4, #2\n\t"
        "ldr r0, 3f\n\t"
        "add r4, r4, r0\n\t"
        "str r4, [r5, #0x58]\n\t"
        "mov r0, #0\n\t"
        "strh r0, [r5, #0x34]\n\t"
        "add r2, r5, #0\n\t"
        "add r2, r2, #0x34\n\t"
        "mov r0, #0x7f\n\t"
        "ldrb r3, [r2]\n\t"
        "and r0, r3\n\t"
        "strb r0, [r2]\n\t"
        "add r3, r5, #0\n\t"
        "add r3, r3, #0x35\n\t"
        "mov r0, #0x1f\n\t"
        "and r1, r0\n\t"
        "mov r0, #0x20\n\t"
        "neg r0, r0\n\t"
        "ldrb r4, [r3]\n\t"
        "and r0, r4\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r3]\n\t"
        "mov r0, #0xd\n\t"
        "neg r0, r0\n\t"
        "ldrb r1, [r2]\n\t"
        "and r0, r1\n\t"
        "mov r1, #8\n\t"
        "orr r0, r1\n\t"
        "strb r0, [r2]\n\t"
        "add r0, r5, #0\n\t"
        "pop {r4, r5}\n\t"
        "pop {r1}\n\t"
        "bx r1\n\t"
        ".align 2, 0\n"
    "1: .4byte gStaticData_087E4C14\n"
    "2: .4byte 0x04000008\n"
    "3: .4byte 0x04000010\n"
    );
}

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
