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

/* Fills screen block 28 from row 16 on (block 30 when `arg0` is set,
 * numbering on from `w * h + 1`) with consecutive tile numbers for a
 * `w` x `h` cell grid; columns past 31 go to the next screen block
 * (+0x7c0 bytes). actor_part95.c's `sub_802996C` inlines the same body
 * twice.
 *
 * The old NAKED note blamed an unreachable register permutation. The
 * fix is `tile++` inside each branch of the column test: with one
 * increment after the `if`, `tile` has fewer references than `base`
 * and the two swap registers (the `r8`/`ip` roles of the 0x7c0 offset
 * and `h` follow from that). Matches under both compilers. */
void sub_8029BC4(s32 arg0, s32 w, s32 h)
{
    u16 *base = (u16 *)(BG_SCREEN_ADDR(28) + 0x400);
    s32 tile;
    s32 row, col;

    if (arg0 != 0) {
        base = (u16 *)(BG_SCREEN_ADDR(30) + 0x400);
        tile = h * w + 1;
    } else {
        tile = 1;
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            if (col <= 0x1f)
                base[col] = tile++;
            else
                base[col + 0x3e0] = tile++;
        }
        base += 0x20;
    }
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
