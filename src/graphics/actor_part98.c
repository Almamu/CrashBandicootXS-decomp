#include "core.h"
#include "gba/io_reg.h"

extern s32 gUnknown_030013B4;

extern s32 sub_803ADB4(s32 a, s32 b);

/* `dest` is materialized (and pinned to the callee-saved `r4`) before
 * the `sub_803ADB4` call, not after - the ROM loads the store
 * destination's address ahead of the call so it survives across it in
 * a register `bl` doesn't clobber, rather than recomputing it from the
 * return value's position afterward (see docs/workflow.md step 3). */
void sub_8029BAC(s32 arg0)
{
    register s32 *dest asm("r4") = &gUnknown_030013B4;

    *dest = sub_803ADB4(arg0 << 8, 0x3c);
}

/* NAKED - a genuine gcc-2.9 register-allocation gap. Every plain-C
 * reconstruction tried keeps the outer-loop base pointer and the
 * per-column tile-index counter alive across the whole nested loop but
 * this compiler never spills the "0x7c0 second-screen-block offset"
 * constant into r8 (a callee-saved high register, needing its own
 * push/pop pair) the way the ROM's own build does unless the register
 * pressure/live-range shape is reproduced exactly - and even once r8
 * was pinned to match, the remaining low-register roles (which of
 * base/tile/row/col/nextBase/nextCol/p1/p2 lands in which of
 * r0-r3/r5/r6) kept coming out as a different, but equivalent,
 * permutation from several structurally-faithful rewrites. Documented
 * per docs/workflow.md's NAKED escape hatch - see docs/matching.md's
 * many-high-register-loop entries for the same class of gap elsewhere
 * in this project. */
NAKED void sub_8029BC4(s32 arg0, s32 count1, s32 count2)
{
    asm(
        "push {r4, r5, r6, r7, lr}\n\t"
        "mov r7, r8\n\t"
        "push {r7}\n\t"
        "add r7, r1, #0\n\t"
        "mov ip, r2\n\t"
        "ldr r4, 9f\n\t"
        "cmp r0, #0\n\t"
        "beq 1f\n\t"
        "ldr r4, 10f\n\t"
        "mov r0, ip\n\t"
        "mul r0, r7, r0\n\t"
        "add r0, #1\n\t"
        "b 2f\n\t"
        ".align 2, 0\n"
    "9: .4byte 0x0600E400\n"
    "10: .4byte 0x0600F400\n"
    "1:\n\t"
        "mov r0, #1\n\t"
    "2:\n\t"
        "mov r3, #0\n\t"
        "cmp r3, ip\n\t"
        "bge 8f\n\t"
        "mov r1, #0xf8\n\t"
        "lsl r1, r1, #3\n\t"
        "mov r8, r1\n\t"
    "3:\n\t"
        "mov r2, #0\n\t"
        "add r6, r4, #0\n\t"
        "add r6, #0x40\n\t"
        "add r5, r3, #1\n\t"
        "cmp r2, r7\n\t"
        "bge 7f\n\t"
        "mov r3, r8\n\t"
        "add r1, r4, r3\n\t"
        "add r3, r4, #0\n\t"
    "4:\n\t"
        "cmp r2, #0x1f\n\t"
        "bgt 5f\n\t"
        "strh r0, [r3]\n\t"
        "b 6f\n\t"
    "5:\n\t"
        "strh r0, [r1]\n\t"
    "6:\n\t"
        "add r0, #1\n\t"
        "add r1, #2\n\t"
        "add r3, #2\n\t"
        "add r2, #1\n\t"
        "cmp r2, r7\n\t"
        "blt 4b\n\t"
    "7:\n\t"
        "add r4, r6, #0\n\t"
        "add r3, r5, #0\n\t"
        "cmp r3, ip\n\t"
        "blt 3b\n\t"
    "8:\n\t"
        "pop {r3}\n\t"
        "mov r8, r3\n\t"
        "pop {r4, r5, r6, r7}\n\t"
        "pop {r0}\n\t"
        "bx r0\n\t"
    );
}

extern s32 gUnknown_030013DC;
extern s32 gUnknown_030013C0;
extern s32 gUnknown_030013C4;
extern s32 gUnknown_030013C8;
extern s32 gUnknown_030013F8;
extern s32 gUnknown_030013FC;
extern s32 gUnknown_030013E0;
extern s32 gUnknown_030013E4;
extern s32 gUnknown_030013E8;
extern s32 gUnknown_030013F4;
extern s32 gUnknown_030013EC;
extern s32 gUnknown_030013F0;
extern s32 gUnknown_030013D0;
extern s32 gUnknown_030013CC;
extern s32 gUnknown_030013D4;
extern s32 gUnknown_030013D8;

/* Selects one of two fixed BG0/BG1 scroll-effect parameter sets
 * (arg0 == 0 vs nonzero), then derives the shared initial scroll
 * position/register state from them. */
void sub_8029C30(s32 arg0)
{
    register s32 v asm("r1");

    gUnknown_030013DC = arg0;

    if (arg0 == 0) {
        gUnknown_030013C0 = 0x88 << 5;
        gUnknown_030013C4 = 0xa0 << 8;
        gUnknown_030013C8 = 0xbc << 6;
        gUnknown_030013F8 = 0x98 << 9;
        gUnknown_030013FC = 0xd0 << 8;
        gUnknown_030013E0 = 2;
        gUnknown_030013E4 = 0x64;
        gUnknown_030013E8 = 0x51;
        gUnknown_030013F4 = arg0;
    } else {
        gUnknown_030013C0 = 0xd0 << 5;
        gUnknown_030013C4 = 0xaa << 8;
        gUnknown_030013C8 = 0xe0 << 5;
        gUnknown_030013F8 = 0x98 << 9;
        gUnknown_030013FC = 0xce << 8;
        gUnknown_030013E0 = 3;
        gUnknown_030013E4 = 3 + 0xfd;
        gUnknown_030013E8 = 0x96;
        gUnknown_030013F4 = 2;
    }

    /* `v` is register-pinned to `r1` from the moment it's first computed
     * (as `gUnknown_030013EC`'s new value) through the sign-rounded
     * `/2`/`>>9`/`>>9` triple below, all reusing that same register in
     * place rather than reloading `gUnknown_030013EC` fresh - matching
     * the ROM's own single, unbroken chain of `r1` uses (see
     * docs/workflow.md step 3). `REG_BG0VOFS` is just
     * `gUnknown_030013F4` alone here, NOT
     * `gUnknown_030013F4 + (v >> 9)` - there's no addition in the ROM's
     * own instructions for this store. */
    {
        register s32 *ecPtr asm("r0") = &gUnknown_030013EC;
        register s32 delta asm("r2") = (s32)0xFFFF1000;

        v = gUnknown_030013F8 + delta;
        *ecPtr = v;
    }
    gUnknown_030013F0 = gUnknown_030013FC + (s32)0xFFFF6000;

    {
        register s32 *d0Ptr asm("r2") = &gUnknown_030013D0;

        v = v + (s32)((u32)v >> 31);
        *d0Ptr = v >> 1;
    }
    gUnknown_030013CC = 0;

    {
        register vu16 *bg0hofsPtr asm("r0") = &REG_BG0HOFS;

        v >>= 9;
        *bg0hofsPtr = v;
    }
    REG_BG0VOFS = gUnknown_030013F4;
    REG_BG1HOFS = v;
    REG_BG1VOFS = 0;

    gUnknown_030013D4 = 0;
    gUnknown_030013D8 = gUnknown_030013C4;
}

/* Eases the BG0/BG1 scroll accumulators (gUnknown_030013D0/gUnknown_030013CC)
 * toward their per-axis target/scale-derived offsets
 * (gUnknown_030013EC/gUnknown_030013F0), clamping each to
 * [0, target]. arg0/arg1 are the two axes' own driving values. */
void sub_8029D8C(s32 arg0, s32 arg1)
{
    s32 target;
    register s32 delta asm("r0");
    s32 cur;
    s32 shift;

    target = gUnknown_030013EC;
    delta = sub_803ADB4(arg0 * (target >> 8), gUnknown_030013E4);
    delta += target / 2;
    cur = gUnknown_030013D0;
    delta -= cur;
    shift = gUnknown_030013E0;
    delta >>= shift;
    cur += delta;
    gUnknown_030013D0 = cur;
    if (cur < 0) {
        cur = 0;
    }
    {
        register s32 clamped asm("r1") = target;
        if (clamped > cur) {
            clamped = cur;
        }
        cur = clamped;
    }
    gUnknown_030013D0 = cur;

    target = gUnknown_030013F0;
    delta = sub_803ADB4(arg1 * (target >> 8), gUnknown_030013E8);
    delta += target / 2;
    cur = gUnknown_030013CC;
    delta -= cur;
    delta >>= shift;
    delta -= gUnknown_030013D4;
    cur += delta;
    gUnknown_030013CC = cur;
    if (cur < 0) {
        cur = 0;
    }
    {
        /* Same clamp idiom as the X-axis block above, but the ROM
         * happens to keep this second copy in `r0` instead of `r1` -
         * see docs/workflow.md step 3. */
        register s32 clamped asm("r0") = target;
        if (clamped > cur) {
            clamped = cur;
        }
        cur = clamped;
    }
    gUnknown_030013CC = cur;
}
